/*
 * IsisTRXVUdemo.h
 *
 *  Created on: 6 feb. 2015
 *      Author: malv
 */

#ifndef ISIS_SOREQ_PAYLOAD_H_
#define ISIS_SOREQ_PAYLOAD_H_

#include <hal/boolean.h>
#include "satellite-subsystems/demo/src/Demos/payload_experiments.h"


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

typedef enum {
    PAYLOAD_SUCCESS, PAYLOAD_I2C_Write_Error, PAYLOAD_I2C_Read_Error, PAYLOAD_TIMEOUT
} SoreqResult;

typedef enum {
    ADC_VALID // When Bit 29 is 1 and Bit 30 is 0 and Bit 31 is 0
    , STILL_PROCESSING // When Bit 31 is 1 TODO: Check if we need to address this issue with a rerun or different indication.
    , ADC_INVALID // When Bit 29 is 0 and not 1 OR Bit 30 is 1 and not 0
} ADCResult;

/***
 * Starts demo.
 * Calls Init and Menu in sequence.
 * Returns FALSE on failure to initialize.
 */


Boolean masterRadiationWrite(RadiationData* data);
/***
 * This function reads the voltages for RADFET1 and RADFET2 from the Soreq payload.
 * OPCODE sent to the payload is 0x33.
 * Returns FALSE on failure.
 */

Boolean masterTemperatureWrite(TemperatureData* data);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x77.
 * Returns FALSE on failure.
 */

Boolean masterPIC32SELWrite(PIC32SELData *data);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x66.
 * Returns FALSE on failure.
 */

Boolean masterPIC32SEUWrite(PIC32SEUData *data);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x47.
 * Returns FALSE on failure.
 */


Boolean clearWatchdog (ActionData *action);
/***
 * This function sends a command to the Soreq payload to clear the watchdog.
 * OPCODE sent to the payload is 0x3F.
 * Returns FALSE on failure.
 */

Boolean softReset (ActionData *action);
/***
 * This function sends a command to the Soreq payload to perform a soft reset.
 * OPCODE sent to the payload is 0xF8.
 * Returns FALSE on failure.
 */

Boolean soreqDebugging(char buffer[12]);
/***
 * This function sends a command to the Soreq payload to start the radiation experiment.
 * OPCODE sent to the payload is 0x32.
 * Returns FALSE on failure.
 */

Boolean soreqPayloadInit(void);

/***
 * Loop producing an interactive
 * text menu for invoking subsystem functions
 * note:
 * Depends on an initialized TRXVU subsystem driver.
 */


#endif /* ISIS_SOREQ_PAYLOAD_H_ */
