/*
 *  Created on: 01 aug. 2024
 *  Authors: Yoav & Tomer 
 */

#ifndef ISIS_SOREQ_PAYLOAD_TLM_H_
#define ISIS_SOREQ_PAYLOAD_TLM_H_
#include <hal/boolean.h>
#include <hal/Timing/Time.h>
#include "satellite-subsystems/demo/src/Demos/payload_experiments.h"

/***
 * This file contains the functions to write and read payload telemetry data to and from a file.
 * The file name is determined by the experiment type.
 * When read by ground station, the satellite will receive a link list of readData structures that contain each of the 4 experiments results.
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
/***
 * This function writes the data to a file.
 * The file name is determined by the experiment type.
 * Returns FALSE on failure.
 */

Boolean payloadReadFromFile(PayloadExperiment exp, Time* readStartTime, Time* readEndTime, readData *head);
/***
 * This function reads the data from a file.
 * The data is read between the start and end times.
 * The file name is determined by the experiment type.
 * Returns FALSE on failure.
 */

Boolean payloadDeleteFromFile(PayloadExperiment exp, Time* deleteStartTime, Time* deleteEndTime);
/***
 * This function deletes the data from a file.
 * The data is deleted between the start and end times.
 * The file name is determined by the experiment type.
 * Returns FALSE on failure.
 */

Boolean payloadDeleteAllExpFile(PayloadExperiment exp);
/***
 * This function deletes an experiment file completely.
 * The file name is determined by the experiment type.
 * Returns FALSE on failure.
 */

#endif /* ISIS_SOREQ_PAYLOAD_TLM_H_ */
