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

/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */

typedef enum {
    RADFET, TEMP, PIC32SEL, PIC32SEU
} PayloadExperiment;

typedef struct {
    unsigned char STATUS[1];
    unsigned char RADFET1[4];
    unsigned char RADFET2[4];
    unsigned char OPCODE[2];
} RadiationData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char TEMP[4];
    unsigned char OPCODE[2];
} TemperatureData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
    unsigned char PIC32SEL[4];
    int* COUNTPIC32SEL;
    unsigned char PIC32SELBACKUP[4];
    int* COUNTPIC32SELBACKUP;
} PIC32SELData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
    unsigned char PIC32SEU[4];
} PIC32SEUData;

typedef struct {
    unsigned char STATUS[1];
    unsigned char OPCODE[2];
} ActionData;


#endif /* ISIS_SOREQ_PAYLOAD_EXPERIMENTS_H_ */