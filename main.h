/*
*   Auto Log Collector App
*   version : 0.9
*   Author : lkhmack@thinkware.co.kr
*
*
*
*/


#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>


#define ARRA_SIZE 32
#define DEVICE_FULL 16
#define LOG_ENABLE 1

typedef enum LogState{
    INIT,ENABLE,RUNNING,STOP
}appState;

typedef enum BoolState{
    TRUE,FALSE
}boolState;

typedef struct{
    char* serial_No;
    int serial_target;
    char filename[ARRA_SIZE];
}serial_Info;

const char *message = "List of devices attached";
const char *logFile = "./logFile_";
const char *endFile = "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++END_OF_FILE";
char command[ARRA_SIZE];
char buff[ARRA_SIZE];
FILE *list_fp;

static char* id_target[DEVICE_FULL];
static int id_resource;
static int used_resource[DEVICE_FULL]={INIT};



int get_adblist(serial_Info **, int);
int check_adblist(serial_Info **, int);


#endif
