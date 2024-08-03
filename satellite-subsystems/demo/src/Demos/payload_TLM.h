/*
 * IsisTRXVUdemo.h
 *
 *  Created on: 6 feb. 2015
 *      Author: malv
 */

#ifndef ISIS_SOREQ_PAYLOAD_TLM_H_
#define ISIS_SOREQ_PAYLOAD_TLM_H_
#include <hal/boolean.h>
#include <hal/Timing/Time.h>
#include "satellite-subsystems/demo/src/Demos/payload_experiments.h"

/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */

#define MAX_FILE_NAME_SIZE 50
#define MAX_FILE_LINE_SIZE 100
#define END_FILE_NAME_RADFET "radfet"
#define END_FILE_NAME_TEMP "temp"
#define END_FILE_NAME_PIC32SEL "pic32sel"
#define END_FILE_NAME_PIC32SEU "pic32seu"

typedef struct {

    void *data;
    Time time;
    readData *next;

} readData;

Boolean payloadWriteToFile(void* data, PayloadExperiment exp);

Boolean payloadReadFromFile(PayloadExperiment exp, Time* readStartTime, Time* readEndTime, readData *head);

Boolean payloadDeleteFromFile(PayloadExperiment exp, Time* deleteStartTime, Time* deleteEndTime);

Boolean payloadDeleteAllExpFile(PayloadExperiment exp);

#endif /* ISIS_SOREQ_PAYLOAD_TLM_H_ */
