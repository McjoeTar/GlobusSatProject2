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

/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */

typedef enum {
    RADFET, TEMP, PIC32SEL, PIC32SEU
} PayloadExperiment;

Boolean payloadWriteToFile(void* data, PayloadExperiment exp);

Boolean payloadReadFromFile(PayloadExperiment exp, Time startTime, Time endTime);

Boolean patloadDeleteFromFile(PayloadExperiment exp, Time startTime, Time endTime);

#endif /* ISIS_SOREQ_PAYLOAD_TLM_H_ */