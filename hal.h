#ifndef HAL_H
#define HAL_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include "gpio.h"
#include "hardware.h"
#include "rfidlistener.h"

#define HARDWARE_DESCIPTOR_RASPI3 "Raspberry Pi 3 Model B Rev 1.2"
#define HARDWARE_DESCIPTOR_UNDEF "-UNDEF-"


class hal : public QObject
{
    Q_OBJECT    
    QThread * uartListener;

public:
    explicit hal(QObject *parent = 0, int led_Timeout = 1000, int key_Timeout = 180);

    ~hal();     

private:
    // Holds the GPIO for key SW1
    int Key_SW1_GPIO;
    // Holds the GPIO for key SW2
    int Key_SW2_GPIO;
    // Holds the GPIO for key SW3
    int Key_SW3_GPIO;
    // Holds the GPIO for key SW4
    int Key_SW4_GPIO;
    // Holds the GPIO for key SW5
    int Key_SW5_GPIO;
    // Holds the GPIO for LED1
    int Led_1_GPIO;
    // Holds the GPIO for LED2
    int Led_2_GPIO;
    // Holds the LED blink timeout value in ms
    int Led_Timeout;
    // Holds the key poll timeout value in ms
    int Key_Timeout;  

    Hardware hardware;
    /*!
     * \brief detect initiated the harware detection.
     * \returns true if known hardware has been detected.
     * Detect is normally called on constructor level. If not called then the
     * HAL will assume that it is an unknonw hardware and no interaction will be initiated.
     */
    bool detect();
    /*!
     * \brief setup
     * \return true if setup has been completed without errors.
     * Setup initiateds the setup household activties for the detected hardware.
     */
    bool setup();
    /*!
     * \brief teardown
     * \return true if teardown has been completed without errors.
     * Setup initiateds the setup household activties for the detected hardware.
     */
    bool teardown();

    /*!
     * \brief setLed1 manipulates the first indicator LED.
     * \param turnOn ture > LED ON, false > LED off
     */
    void setLed1(bool turnOn);
    /*!
     * \brief setLed2 manipulates the first indicator LED.
     * \param turnOn ture > LED ON, false > LED off
     */
    void setLed2(bool turnOn);

signals:
    /*!
     * \brief readiness signal for hardware access.
     * The ready signal has to be avaited before the main applicaiton can start using the hardware.
     */
    void ready();    

    /*!
     * \brief key_SW1_pressed allows the detection of harware interaction.
     */
    void key_SW1_pressed();
    /*!
     * \brief key_SW2_pressed allows the detection of harware interaction.
     */
    void key_SW2_pressed();
    /*!
     * \brief key_SW3_pressed allows the detection of harware interaction.
     */
    void key_SW3_pressed();
    /*!
     * \brief key_SW4_pressed allows the detection of harware interaction.
     */
    void key_SW4_pressed();
    /*!
     * \brief key_SW5_pressed allows the detection of harware interaction.
     */
    void key_SW5_pressed();

    /*!
     * \brief signalization if a new tag was discovered.
     * last discovered tag does not issue a signal.
     * \param tagId TAG-ID of the newly discovered tag.
     */
    void newTagDetected(QString tagId);

public slots:
    // Handle the LED timeout.
    void timeout_LED(void);

    void rfidTagDetected(QString tagId);

    // Handle the KEY timeout.
    // The method checks for a pressed key and invokes the appropriate function.
    void timeout_KEY(void);
};

#endif // HAL_H
