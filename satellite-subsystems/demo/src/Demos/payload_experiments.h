/*
 * IsisTRXVUdemo.h
 *
 *  Created on: 6 feb. 2015
 *      Author: malv
 */

#ifndef ISIS_SOREQ_PAYLOAD_EXPERIMENTS_H_
#define ISIS_SOREQ_PAYLOAD_EXPERIMENTS_H_
#include <hal/boolean.h>
#include <hal/Timing/Time.h>

#define SOREQ_I2C_ADDRESS 0x55
#define READ_PIC32_RESETS 0x66
#define READ_Radiation 0x33
#define READ_DELAY 200
#define TIMEOUT 4000
#define READ_PIC32_RESETS 0x66
#define PAYLOAD_I2C_ADDRESS 0x55
#define SOREQ_DEBUGGING 0x32
#define CLEARWATCHDOG 0x3F
#define SOFTRESET 0xF8
#define OPCODE_MASTER_READ_FROM_PAYLOAD 0x45 // in tau-1

#define RADIATION_DELAY 2500
#define Temperature_DELAY 1690
#define PIC32SEL_DELAY 20
#define PIC32SEU_DELAY 200
#define CLEARWATCHDOG_DELAY 10 //TODO: What is the correct delay?
#define SOFTRESET_DELAY 10 //TODO: What is the correct delay?
/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */

typedef enum {
    RADFET, TEMP, PIC32SEL, PIC32SEU
} PayloadExperiment;

typedef enum {
    PAYLOAD_SUCCESS, PAYLOAD_I2C_Write_Error, PAYLOAD_I2C_Read_Error, PAYLOAD_TIMEOUT
} SoreqResult;

typedef enum {
    ADC_VALID // When Bit 29 is 1 and Bit 30 is 0 and Bit 31 is 0
    , STILL_PROCESSING // When Bit 31 is 1 TODO: Check if we need to address this issue with a rerun or different indication.
    , ADC_INVALID // When Bit 29 is 0 and not 1 OR Bit 30 is 1 and not 0
} ADCResult;

typedef struct {
    unsigned char STATUS[1];
    unsigned char RADFET1[4];
    unsigned char RADFET2[4];
    unsigned char OPCODE[2];
    int expTime;
} RadiationData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char TEMP[4];
    unsigned char OPCODE[2];
    int expTime;
} TemperatureData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
    unsigned char PIC32SEL[4];
    int* COUNTPIC32SEL;
    unsigned char PIC32SELBACKUP[4];
    int* COUNTPIC32SELBACKUP;
    int expTime;
} PIC32SELData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
    unsigned char PIC32SEU[4];
    int expTime;
} PIC32SEUData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
    int expTime;
} ActionData;



#endif /* ISIS_SOREQ_PAYLOAD_EXPERIMENTS_H_ */