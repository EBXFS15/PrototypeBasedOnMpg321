#include "hal.h"

hal::hal(QObject *parent, int led_Timeout, int key_Timeout) : QObject(parent)
{
    hardware = UNDEF;
    Led_Timeout = led_Timeout;
    Key_Timeout = key_Timeout;
    detect();
    setup();    

    if (hardware == BBB)
    {
        // --- Set and start LED timer ---
        QTimer *timer_LED = new QTimer(this);
        connect(timer_LED, SIGNAL(timeout()), this, SLOT(timeout_LED()));
        timer_LED->start(Led_Timeout);

        // --- Set and start KEY timer ---
        QTimer *timer_KEY = new QTimer(this);
        connect(timer_KEY, SIGNAL(timeout()), this, SLOT(timeout_KEY()));
        timer_KEY->start(Key_Timeout);
    }
    if (hardware == UNDEF)
    {
        uartListener = new QThread();
        rfidListener * uartWorker = new rfidListener(hardware);
        uartWorker->moveToThread(uartListener);

        connect(uartListener,SIGNAL(started()),uartWorker,SLOT(process()));
        connect(uartWorker,SIGNAL(newTagDetected(QString)), this,SLOT(rfidTagDetected(QString)));
        uartListener->start();
    }
}

bool hal::detect()
{
    QFile file("/proc/device-tree/model");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(file.readLine());
        QString hardware(in.readLine());
        if (hardware.contains(HARDWARE_DESCIPTOR_RASPI3)){
            hardware = PI3B;
            return true;
        }
    }
    return false;
}

bool hal::setup()
{
    Key_SW1_GPIO = -1;
    Key_SW2_GPIO = -1;
    Key_SW3_GPIO = -1;
    Key_SW4_GPIO = -1;
    Key_SW5_GPIO = -1;
    Led_1_GPIO = -1;
    Led_2_GPIO = -1;

    if (hardware == BBB)
    {
        Key_SW1_GPIO = 7;
        Key_SW2_GPIO = 60;
        Key_SW3_GPIO = 48;
        Key_SW4_GPIO = 51;
        Key_SW5_GPIO = 115;
        Led_1_GPIO = 68;
        Led_2_GPIO = 69;
        Led_Timeout = 1000;
        Key_Timeout = 180;

        // --- Setup GPIO's for KEY's and LED's ---
        // LED 1
        gpio_export(Led_1_GPIO);
        gpio_setToOutput(Led_1_GPIO);
        // LED 2
        gpio_export(Led_2_GPIO);
        gpio_setToOutput(Led_2_GPIO);
        // Turn on AMPLIFIER
        gpio_set(Led_1_GPIO);
        // SW1
        gpio_export(Key_SW1_GPIO);
        gpio_setToInput(Key_SW1_GPIO);
        // SW2
        gpio_export(Key_SW2_GPIO);
        gpio_setToInput(Key_SW2_GPIO);
        // SW3
        gpio_export(Key_SW3_GPIO);
        gpio_setToInput(Key_SW3_GPIO);
        // SW4
        gpio_export(Key_SW4_GPIO);
        gpio_setToInput(Key_SW4_GPIO);
        // SW5
        gpio_export(Key_SW5_GPIO);
        gpio_setToInput(Key_SW5_GPIO);
        return true;
    }
    return false;
}

void hal::rfidTagDetected(QString tagId){
    emit newTagDetected(tagId);
    uartListener->quit();
    uartListener->start();
}

bool hal::teardown()
{
    if (hardware == BBB)
    {
        // --- Free GIO resources ---
        // SW1
        gpio_unexport(Key_SW1_GPIO);
        // SW2
        gpio_unexport(Key_SW2_GPIO);
        // SW3
        gpio_unexport(Key_SW3_GPIO);
        // SW4
        gpio_unexport(Key_SW4_GPIO);
        // SW5
        gpio_unexport(Key_SW5_GPIO);

        // LED1
        gpio_clear(Led_1_GPIO);
        gpio_unexport(Led_1_GPIO);
        // LED2
        gpio_clear(Led_2_GPIO);
        gpio_unexport(Led_2_GPIO);
    }
    return true;
}

#define IR_STATE_MUTE 7
#define IR_STATE_START IR_STATE_MUTE + 1
#define IR_STATE_PREPARE_SET_IN    IR_STATE_MUTE - 1
#define IR_STATE_SET_AM    IR_STATE_PREPARE_SET_IN - 1
#define IR_STATE_SET_AUX   IR_STATE_SET_AM - 1
#define IR_STATE_UNMUTE IR_STATE_SET_AUX - 4

void
hal::timeout_LED(void)
{
    static unsigned short x = 0;
    // irTimeout normally set to IR_STATE_MUTE + 1
    static unsigned short irTimeout = (IR_STATE_MUTE + 1);
    QByteArray data;
    QFile file("/dev/jvc-remote");
    x++;
    if (x % 2) {
      gpio_set(Led_2_GPIO);
    } else {
      gpio_clear(Led_2_GPIO);
    }
    if(irTimeout == IR_STATE_MUTE)
    {
        int i = 0;

        //Mute
        data.clear();
        data.append(0x47);
        data.append(0x05);
        for(i = 0 ; i< 20;i++)
        {
            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
        }
        irTimeout--;
    }
    if(irTimeout == IR_STATE_PREPARE_SET_IN)
    {
        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        irTimeout--;
    }
    if(irTimeout == IR_STATE_SET_AM)
    {
        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();
        irTimeout--;
    }

    if(irTimeout == IR_STATE_SET_AUX)
    {
        //Set to AUX
        data.clear();
        data.append(0x47);
        data.append(0x08);
        file.open(QIODevice::WriteOnly);
        file.write(data);
        file.close();

        irTimeout--;
    }
    if(irTimeout == IR_STATE_UNMUTE)
    {
        int i = 0;
        // UnMute
        data.clear();
        data.append(0x47);
        data.append(0x04);
        for(i = 0 ; i< 12;i++)
        {
            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
        }

        irTimeout--;
        emit ready();
    }
    if(irTimeout > IR_STATE_UNMUTE)
    {
        irTimeout--;
    }


} // MainWindow::timeout_LED

void
hal::timeout_KEY(void)
{
    // --- Handle KEY SW1 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw1Pressed = false;

    int key = 1;
    gpio_get(Key_SW1_GPIO, &key);
    if ((0 == key) && (false == isSw1Pressed)) {
        // SW pressed
        emit key_SW1_pressed();
        isSw1Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw1Pressed = false;
        }
    }

    // --- Handle KEY SW2 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw2Pressed = false;

    key = 1;
    gpio_get(Key_SW2_GPIO, &key);
    if ((0 == key) && (false == isSw2Pressed)) {
        // SW pressed
        emit key_SW2_pressed();
        isSw2Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw2Pressed = false;
        }
    }

    // --- Handle KEY SW3 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw3Pressed = false;

    key = 1;
    gpio_get(Key_SW3_GPIO, &key);
    if ((0 == key) && (false == isSw3Pressed)) {
        // SW pressed
        emit key_SW3_pressed();
        isSw3Pressed = false; //true; // do NOT set to true to allow fast ff
    } else {
        // Wait for key released
        if (1 == key) {
          isSw3Pressed = false;
        }
    }

    // --- Handle KEY SW4 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw4Pressed = false;

    key = 1;
    gpio_get(Key_SW4_GPIO, &key);
    if ((0 == key) && (false == isSw4Pressed)) {
        // SW pressed
        emit key_SW4_pressed();
        isSw4Pressed = false; //true; // do NOT set to true to allow fast rew
    } else {
        // Wait for key released
        if (1 == key) {
          isSw4Pressed = false;
        }
    }

    // --- Handle KEY SW5 --
    // Detect whether user is sleeping and does hold the key,
    // means a release key has to be detected before we accept
    // a new keystroke.
    static bool isSw5Pressed = false;

    key = 1;
    gpio_get(Key_SW5_GPIO, &key);
    if ((0 == key) && (false == isSw5Pressed)) {
        // SW pressed
        emit key_SW5_pressed();
        isSw5Pressed = true;
    } else {
        // Wait for key released
        if (1 == key) {
          isSw5Pressed = false;

        }
    }
} // MainWindow::timeout_KEY

hal::~hal()
{
    teardown();
    uartListener->terminate();
    delete uartListener;
}

