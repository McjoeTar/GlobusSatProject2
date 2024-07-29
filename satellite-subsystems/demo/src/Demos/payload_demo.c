
#include <hal/Drivers/I2C.h>
#include <hal/boolean.h>
#include <hal/errors.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <hal/Timing/Time.h>
#include "hal/Drivers/I2C.h"
#include <hal/Utility/util.h>
#include "satellite-subsystems/demo/src/Demos/payload_demo.h"
#include "satellite-subsystems/demo/src/Demos/payload_experiments.h"



Boolean checkStatus(unsigned char* status,unsigned char* buffer)
{
    *status = buffer[2];
    if (status == 0)
    {
        return TRUE;
    }
    return FALSE;
}

void extractOpcode(unsigned char* opcode, unsigned char* buffer)
{
    *opcode = buffer[0] << 8 | buffer[1]; 
}

int changeIntIndian(int num)
{
    return ((num>>24)&0xff) | // move byte 3 to byte 0
                        ((num<<8)&0xff0000) | // move byte 1 to byte 2
                        ((num>>8)&0xff00) | // move byte 2 to byte 1
                        ((num<<24)&0xff000000); // byte 0 to byte 3
}

SoreqResult masterRead(int size,unsigned char* buffer)
{
    unsigned char *opcode_read = OPCODE_MASTER_READ_FROM_PAYLOAD;
    int err=0;
    int i=0;
    
    do
    {
        err = I2C_write(PAYLOAD_I2C_ADDRESS,opcode_read,1);
        if(err!=0)
        {
            return PAYLOAD_I2C_Write_Error;
        }
        vTaskDelay(READ_DELAY);
        err = I2C_read(SOREQ_I2C_ADDRESS,buffer,size);
        if(err!=0)
        {
            return PAYLOAD_I2C_Read_Error;
        }
        if(buffer[3] == 0)
        {
            return PAYLOAD_SUCCESS;
        }
        vTaskDelay(READ_DELAY);
    }while(i++<TIMEOUT);
    return PAYLOAD_TIMEOUT;
}
SoreqResult payloadSendCommand(char opcode, int size, unsigned char* buffer,int delay)
{
	vTaskDelay(50);
	int err = I2C_write(PAYLOAD_I2C_ADDRESS, &opcode, 1);
	if(err!=0)
	{
		return PAYLOAD_I2C_Write_Error;
	}
	if(delay>0)
	{
		vTaskDelay(delay);
	}
	SoreqResult res =  masterRead(size,buffer);

	return res;
}
ADCResult retrieveADCData(unsigned char sample[4])
{
    if ((*sample & 0x00000001 == 0) && (*sample & 0x00000002 == 0) && (*sample & 0x00000004 == 1))
    {
        return ADC_VALID;
    }
    else if (*sample & 0x00000001 == 1)
    {
        return STILL_PROCESSING;
    }
    return ADC_INVALID;
}

Boolean getTimestamp(int *expTime){
    Time_getUnixEpoch(expTime);
    if(expTime == NULL)
    {
        return FALSE;
    }
    return TRUE;
}
Boolean clearWatchdog(ActionData* action){
    unsigned char* buffer = malloc(sizeof(unsigned char)*3);
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    SoreqResult res = payloadSendCommand(CLEARWATCHDOG, 3, buffer, CLEARWATCHDOG_DELAY); //TODO: What is the correct delay?
    if (res != PAYLOAD_SUCCESS)
    {
        printf("Error when trying to clear the watchdog.\n");
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&action->STATUS, buffer))
    {
        printf("Data is not ready.\n");
        free(buffer);
        return FALSE;
    }
    extractOpcode(&action->OPCODE, buffer);
    free(buffer);
    return TRUE;

}
Boolean softReset(ActionData* action){
    unsigned char* buffer = malloc(sizeof(unsigned char)*3);
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    SoreqResult res = payloadSendCommand(SOFTRESET, 3, buffer, SOFTRESET_DELAY); //TODO: What is the correct delay?
    if (res != PAYLOAD_SUCCESS)
    {
        printf("Error when trying to reset the payload.\n");
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&action->STATUS, buffer))
    {
        printf("Data is not ready.\n");
        free(buffer);
        return FALSE;
    }
    extractOpcode(&action->OPCODE, buffer);
    free(buffer);
    return TRUE;
}
Boolean masterPIC32SELWrite(PIC32SELData *pic32sel)
{
    unsigned char* buffer = malloc(sizeof(unsigned char)*12);
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    SoreqResult res = payloadSendCommand(READ_PIC32_RESETS, 12, buffer, PIC32SEL_DELAY);
    if (res != PAYLOAD_SUCCESS)
    {
        printf("Error when trying to read PIC32SEL data.\n");
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&pic32sel->STATUS, buffer))
    {
        printf("Data is not ready.\n");
        free(buffer);
        return FALSE;
    }
    if (!getTimestamp(&pic32sel->expTime))
    {
        printf("Error when trying to get the timestamp for experiment pic32SEL.\n");
    }
    extractOpcode(&pic32sel->OPCODE, buffer);
    *pic32sel->PIC32SEL = buffer[3] << 24 | buffer[4] << 16 | buffer[5] << 8 | buffer[6];
    *pic32sel->PIC32SELBACKUP = buffer[7] << 24 | buffer[8] << 16 | buffer[9] << 8 | buffer[10];
    memcpy(pic32sel->COUNTPIC32SEL,pic32sel->PIC32SEL,4);
    changeIntIndian(pic32sel->COUNTPIC32SEL);
    memcpy(pic32sel->COUNTPIC32SELBACKUP,pic32sel->PIC32SELBACKUP,4);
    changeIntIndian(pic32sel->COUNTPIC32SELBACKUP);
    
    free(buffer);
    return TRUE;
    
}
Boolean masterPIC32SEUWrite(PIC32SEUData *pic32seu)
{
    unsigned char* buffer = malloc(sizeof(unsigned char)*12); //TODO: check if we need 12 or lower? (11 Bytes).
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    
    SoreqResult res = payloadSendCommand(READ_PIC32_RESETS, 3, buffer, PIC32SEU_DELAY);
    if (res != PAYLOAD_SUCCESS)
    {
        printf("Error when trying to read PIC32SEU data.\n");
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&pic32seu->STATUS, buffer))
    {
        printf("Data is not ready.\n");
        free(buffer);
        return FALSE;
    }
    if (!getTimestamp(&pic32seu->expTime))
    {
        printf("Error when trying to get the timestamp for experiment pic32SEU.\n");
    }
    extractOpcode(&pic32seu->OPCODE, buffer);
    *pic32seu->PIC32SEU = buffer[3] << 24 | buffer[4] << 16 | buffer[5] << 8 | buffer[6];
    free(buffer);
    return TRUE;
}
Boolean masterRadiationWrite(RadiationData* rad)
{
    unsigned char* buffer = malloc(sizeof(unsigned char)*12); //TODO: check if we need 12 or lower? (11 Bytes).
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    SoreqResult res = payloadSendCommand(READ_Radiation, 12, buffer, RADIATION_DELAY);
    if (res != PAYLOAD_SUCCESS)
    {
        printf("Error when trying to read radiation data.\n");
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&rad->STATUS, buffer))
    {
        printf("Data is not ready.\n");
        free(buffer);
        return FALSE;
    }
    if (!getTimestamp(&rad->expTime))
    {
        printf("Error when trying to get the timestamp for experiment pic32SEL.\n");
    }
    extractOpcode(&rad->OPCODE, buffer);
    *rad->RADFET1 = buffer[3] << 24 | buffer[4] << 16 | buffer[5] << 8 | buffer[6];
    if (retrieveADCData(rad->RADFET1) != ADC_VALID) //TODO: Should we address PROCESSING?
    {
        printf("ADC data 1 is invalid.\n");
        free(buffer);
        return FALSE;
    }
    *rad->RADFET2 = buffer[7] << 24 | buffer[8] << 16 | buffer[9] << 8 | buffer[10];
    if(retrieveADCData(rad->RADFET2) != ADC_VALID) //TODO: Should we address PROCESSING?
    {
        printf("ADC data 2 is invalid.\n");
        free(buffer);
        return FALSE;
    }
    free(buffer);
    return TRUE;
}
Boolean MasterTemperatureWrite(TemperatureData * temprature)
{
    unsigned char* buffer = malloc(sizeof(unsigned char)*12); //TODO: check if we need 12 or lower? (11 Bytes).
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }

    SoreqResult res = payloadSendCommand(READ_Radiation, 12, buffer, 200);
    if (res != PAYLOAD_SUCCESS)
    {
        free(buffer);
        return FALSE;
    }
    if(!checkStatus(&temprature->STATUS, buffer))
    {
        free(buffer);
        return FALSE;
    }
    if (!getTimestamp(&temprature->expTime))
    {
        printf("Error when trying to get the timestamp for experiment pic32SEL.\n");
    }
    extractOpcode(&temprature->OPCODE, buffer);
    *temprature->TEMP = buffer[3] << 24 | buffer[4] << 16 | buffer[5] << 8 | buffer[6];
    if (retrieveADCData(temprature->TEMP) != ADC_VALID) //TODO: Should we address PROCESSING?
    {
        free(buffer);
        return FALSE;
    }
    free(buffer);
    return TRUE;
}
Boolean soreqDebugging()
{
    unsigned char* buffer = malloc(sizeof(unsigned char)*12); //TODO: check if we need 12 or lower? (11 Bytes).
    if(buffer == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    SoreqResult res = payloadSendCommand(SOREQ_DEBUGGING, 12, buffer, 0);
    if (res != PAYLOAD_SUCCESS)
    {
        return FALSE;
    }
    unsigned char *status=malloc(sizeof(char));
    if(status == NULL){
        printf("Memory allocation failed.\n");
        return FALSE;
    }
    if(!checkStatus(status, buffer))
    {
        return FALSE;
    }
    Boolean first_check = (buffer[3] << 8 | buffer[4]) == 0x3000;
    Boolean second_check = (buffer[5] << 8 | buffer[6]) == 0x3100;
    Boolean third_check = (buffer[7] << 8 | buffer[8]) == 0x3200;
    if(first_check && second_check && third_check)
    {
        return TRUE;
    }
    return FALSE;
}
