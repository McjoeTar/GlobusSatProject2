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

typedef enum {
    RADFET, TEMP, PIC32SEL, PIC32SEU
} PayloadExperiment;

#define MAX_FILE_NAME_SIZE 50
#define END_FILE_NAME_RADFET "radfet"
#define END_FILE_NAME_TEMP "temp"
#define END_FILE_NAME_PIC32SEL "pic32sel"
#define END_FILE_NAME_PIC32SEU "pic32seu"


Boolean payloadWriteToFile(void* data, PayloadExperiment exp);

Boolean payloadReadFromFile(PayloadExperiment exp, Time startTime, Time endTime);

Boolean payloadDeleteFromFile(PayloadExperiment exp, Time startTime, Time endTime);

#endif /* ISIS_SOREQ_PAYLOAD_TLM_H_ */
