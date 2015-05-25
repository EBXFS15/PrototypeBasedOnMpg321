/**
 ***************************************************************************
 * @brief   CAS EBX-FS15 Embedded Linux \n
 *	    Implements GPIO functions, e.g.: \n
 *	    export / unexport / set output / input.\n
 * @file    gpio.h
 * @version 1.0
 * @date    09.05.2015
 * @author  Michel Grundmann
 *
 * @remark  Last Modifications:
 * @remark  V1.0, mg, 09.05.2015
 ***************************************************************************
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include "gpio.h"

/*
 ***************************************************************************
 * Defines and macros
 ***************************************************************************
 */

/*
 ***************************************************************************
 * Vars
 ***************************************************************************
 */



void
gpio_export(const int inGpioNumber)
{
  FILE* exportP = fopen("/sys/class/gpio/export","w");
  if(exportP!=NULL){
    fprintf(exportP, "%d", inGpioNumber);
    fclose(exportP);
  }
}

void
gpio_unexport(const int inGpioNumber)
{
  FILE* unexportP = fopen("/sys/class/gpio/unexport","w");
  if(unexportP!=NULL){
    fprintf(unexportP, "%d", inGpioNumber);
    fclose(unexportP);
  }
}

void
gpio_setToOutput(const int inGpioNumber)
{
  char gpioPath[100];
  sprintf(gpioPath, "/sys/class/gpio/gpio%d/direction", inGpioNumber);
  FILE* directionP = fopen(gpioPath, "w");
  if(directionP!=NULL){
    fprintf(directionP, "out");
    fclose(directionP);
  }
}

void
gpio_setToInput(const int inGpioNumber)
{
  char gpioPath[100];
  sprintf(gpioPath, "/sys/class/gpio/gpio%d/direction", inGpioNumber);
  FILE* directionP = fopen(gpioPath,"w");
  if(directionP!=NULL){
    fprintf(directionP, "in");
    fclose(directionP);
  }
}

void
gpio_set(const int inGpioNumber)
{
  char valuePath[100];
  sprintf(valuePath, "/sys/class/gpio/gpio%d/value", inGpioNumber);
  FILE* valueP = fopen(valuePath,"w");
  if(valueP!=NULL){
    fprintf(valueP, "1");
    fclose(valueP);
  }
}

void
gpio_get(const int inGpioNumber, int* outValueP)
{
  char valuePath[100];
  char value[5];
  sprintf(valuePath, "/sys/class/gpio/gpio%d/value", inGpioNumber);
  FILE* valueP = fopen(valuePath,"r");
  if(valueP!=NULL){
    size_t bytes_read = fread (value, 1, sizeof(value), valueP);
    fclose(valueP);

    value[bytes_read] = '\0';
    if (outValueP != NULL) {
        sscanf(value, "%d", outValueP);
    }
  }
}

void
gpio_clear(const int inGpioNumber)
{
  char valuePath[100];
  sprintf(valuePath, "/sys/class/gpio/gpio%d/value", inGpioNumber);
  FILE* valueP = fopen(valuePath,"w");
  if(valueP!=NULL){
    fprintf(valueP, "0");
    fclose(valueP);
  }
}

