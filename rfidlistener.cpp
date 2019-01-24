#include "rfidlistener.h"


rfidListener::rfidListener(Hardware hwType)
{
    openport(hwType);
}

rfidListener::~rfidListener()
{    
    closeport();
}

void rfidListener::process(){

    char rfidBuff[16]=BLANK_RFID_TAG;

    readport(rfidBuff);
    emit newTagDetected(QString(rfidBuff));

}
