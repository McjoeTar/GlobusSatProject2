
#include <hal/Drivers/I2C.h>
#include <hal/boolean.h>
#include <hal/errors.h>
#include <hcc/api_fat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>s
#include "hal/Drivers/I2C.h"
#include <hal/Utility/util.h>
#include "satellite-subsystems/demo/src/Demos/payload_TLM.h"
#include "satellite-subsystems/demo/src/Demos/payload_experiments.h"
#include "satellite-subsystems/demo/src/Demos/payload_demo.h"
#include <time.h>


void getExpName(PayloadExperiment exp, char* endFileName, int* structSize){

	if (exp==RADFET){
		memcpy(endFileName,END_FILE_NAME_RADFET,sizeof(END_FILE_NAME_RADFET));
		*structSize = sizeof(RadiationData);
	}
	else if (exp==TEMP){
		memcpy(endFileName,END_FILE_NAME_TEMP,sizeof(END_FILE_NAME_TEMP));
		*structSize = sizeof(TemperatureData);
	}
	else if (exp==PIC32SEL){
		memcpy(endFileName,END_FILE_NAME_PIC32SEL,sizeof(END_FILE_NAME_PIC32SEL));
		*structSize = sizeof(PIC32SELData);
	}
	else if (exp==PIC32SEU){
		memcpy(endFileName,END_FILE_NAME_PIC32SEU,sizeof(END_FILE_NAME_PIC32SEU));
		*structSize = sizeof(PIC32SEUData);
	}

}

void writeExpTelemetry(F_FILE *fp,PayloadExperiment exp, void* data){
	
    // Write the common fields (OPCODE and STATUS)
    switch (exp) {
        case RADFET: {
            RadiationData *radData = (RadiationData *)data;
			f_write(radData->expTime, sizeof(radData->expTime), 1, fp);
            f_write(radData->OPCODE, sizeof(radData->OPCODE), 1, fp);
            f_write(radData->STATUS, sizeof(radData->STATUS), 1, fp);
            f_write(radData->RADFET1, sizeof(radData->RADFET1), 1, fp);
            f_write(radData->RADFET2, sizeof(radData->RADFET2), 1, fp);
            break;
        }
        case TEMP: {
            TemperatureData *tempData = (TemperatureData *)data;
			f_write(tempData->expTime, sizeof(tempData->expTime), 1, fp);
            f_write(tempData->OPCODE, sizeof(tempData->OPCODE), 1, fp);
            f_write(tempData->STATUS, sizeof(tempData->STATUS), 1, fp);
            f_write(tempData->TEMP, sizeof(tempData->TEMP), 1, fp);
            break;
        }
        case PIC32SEL: {
            PIC32SELData *picData = (PIC32SELData *)data;
			f_write(picData->expTime, sizeof(picData->expTime), 1, fp);
            f_write(picData->OPCODE, sizeof(picData->OPCODE), 1, fp);
            f_write(picData->STATUS, sizeof(picData->STATUS), 1, fp);
            f_write(picData->PIC32SEL, sizeof(picData->PIC32SEL), 1, fp);
            f_write(picData->PIC32SELBACKUP, sizeof(picData->PIC32SELBACKUP), 1, fp);
            // Note: For the COUNTPIC32SEL and COUNTPIC32SELBACKUP fields, you need to handle writing the integers appropriately if needed.
            break;
        }
        case PIC32SEU: {
            PIC32SEUData *picSeudata = (PIC32SEUData *)data;
			f_write(picSeudata->expTime, sizeof(picSeudata->expTime), 1, fp);
            f_write(picSeudata->OPCODE, sizeof(picSeudata->OPCODE), 1, fp);
            f_write(picSeudata->STATUS, sizeof(picSeudata->STATUS), 1, fp);
            f_write(picSeudata->PIC32SEU, sizeof(picSeudata->PIC32SEU), 1, fp);
            break;
        }
    }
}

Boolean testTimes(Time* startTime, Time* endTime){

	int startTimeEpoch = Time_convertTimeToEpoch(startTime);
	int endTimeEpoch = Time_convertTimeToEpoch(endTime);
	int currentTimeEpoch = Time_convertTimeToEpoch(Time_get());


	if (startTimeEpoch > endTimeEpoch){
		printf("Start time is greater than end time\n");
		return FALSE;
	}

	if(startTimeEpoch < 0 || endTimeEpoch < 0){
		printf("Invalid time\n");
		return FALSE;
	}

	if(startTimeEpoch == endTimeEpoch){
		printf("Start time is equal to end time\n");
		return FALSE;
	}

	if(startTimeEpoch > currentTimeEpoch){
		printf("Start time is in the future\n");
		return FALSE;
	}

	return TRUE;
}

Boolean payloadWriteToFile(void* data, PayloadExperiment exp){

	int size;
	F_FILE *fp;
	char curr_date_str[6];
	char file_name[MAX_FILE_NAME_SIZE];

	getExpName(exp,file_name,&size);
	fp = f_open(file_name, "a");

	if (!fp)
	{
		//printf("Unable to open file %s, f_open error=%d\n",file_name, err);
		return FALSE;
	}

	writeExpTelemetry(fp,exp,data);

	/* close the file*/
	f_flush(fp);
	f_close (fp);
	return TRUE;
}


Boolean payloadDeleteAllExpFile(PayloadExperiment exp){
	int size;
	char file_name[MAX_FILE_NAME_SIZE] = {0};
	getExpName(exp,file_name,&size);
	f_delete(*file_name);
}

Boolean payloadDeleteFromFile(PayloadExperiment exp, Time* deleteStartTime, Time* deleteEndTime){

	if(!testTimes(deleteStartTime,deleteEndTime)){
		printf("Invalid times\n");
		return FALSE;
	}

	int size;
	F_FILE *fp,*fp_backup;
	char rowTime[MAX_FILE_NAME_SIZE] = {0};
	char file_name[MAX_FILE_NAME_SIZE] = {0};

	getExpName(exp,&file_name,&size);
	fp = f_open(file_name, "r");
	if (!fp)
	{
		printf("Unable to open file %s\n",file_name);
		return FALSE;
	}
	fp_backup = f_open("temp", "w");
	if (!fp_backup)
	{
		printf("Unable to open file %s%s\n",file_name,"temp");
		return FALSE;
	}
	int startTimeEpoch = Time_convertTimeToEpoch(deleteStartTime);
	int endTimeEpoch = Time_convertTimeToEpoch(deleteEndTime);
	char rowData[size];

	while (f_eof(fp) == 0)
	{
		f_read(rowTime, sizeof(int), 1, fp);
		if (atoi(rowTime) < startTimeEpoch || atoi(rowTime) > endTimeEpoch)
		{	
			f_write(rowTime, sizeof(int), 1, fp_backup);
			f_read(rowData, size, 1, fp);
			f_write(rowData, size, 1, fp_backup);
			f_write("\n", 1, 1, fp_backup);
		}
	}

	f_close(fp);
	f_delete(*file_name);
	f_rename("temp",file_name);
	f_close(fp_backup);
}

Boolean determineExp(PayloadExperiment exp, int* size,void* data){
	if (exp==RADFET){
		*size = sizeof(RadiationData);
		data = malloc(*size);
		if(data == NULL){
			printf("Memory allocation failed.\n");
			return FALSE;
		}
		data = (RadiationData*)data;
		return TRUE;
	}
	else if (exp==TEMP){
		*size = sizeof(TemperatureData);
		data = malloc(*size);
		if(data == NULL){
			printf("Memory allocation failed.\n");
			return FALSE;
		}
		data = (TemperatureData*)data;
		return TRUE;
	}
	else if (exp==PIC32SEL){
		*size = sizeof(PIC32SELData);
		data = malloc(*size);
		if(data == NULL){
			printf("Memory allocation failed.\n");
			return FALSE;
		}
		data = (PIC32SELData*)data;
		return TRUE;
	}
	else if (exp==PIC32SEU){
		*size = sizeof(PIC32SEUData);
		data = malloc(*size);
		if(data == NULL){
			printf("Memory allocation failed.\n");
			return FALSE;
		}
		data = (PIC32SEUData*)data;
		return TRUE;
	}
	return FALSE;
}

Boolean readTempData(F_FILE *fp, TemperatureData *tempData){
	f_read(tempData->OPCODE, sizeof(tempData->OPCODE), 1, fp);
	f_read(tempData->STATUS, sizeof(tempData->STATUS), 1, fp);
	f_read(tempData->TEMP, sizeof(tempData->TEMP), 1, fp);
}

Boolean readRadData(F_FILE *fp, RadiationData *radData){
	f_read(radData->OPCODE, sizeof(radData->OPCODE), 1, fp);
	f_read(radData->STATUS, sizeof(radData->STATUS), 1, fp);
	f_read(radData->RADFET1, sizeof(radData->RADFET1), 1, fp);
	f_read(radData->RADFET2, sizeof(radData->RADFET2), 1, fp);
}

Boolean readPIC32SELData(F_FILE *fp, PIC32SELData *picData){
	f_read(picData->OPCODE, sizeof(picData->OPCODE), 1, fp);
	f_read(picData->STATUS, sizeof(picData->STATUS), 1, fp);
	f_read(picData->PIC32SEL, sizeof(picData->PIC32SEL), 1, fp);
	f_read(picData->PIC32SELBACKUP, sizeof(picData->PIC32SELBACKUP), 1, fp);
}

Boolean readPIC32SEUData(F_FILE *fp, PIC32SEUData *picSeuData){
	f_read(picSeuData->OPCODE, sizeof(picSeuData->OPCODE), 1, fp);
	f_read(picSeuData->STATUS, sizeof(picSeuData->STATUS), 1, fp);
	f_read(picSeuData->PIC32SEU, sizeof(picSeuData->PIC32SEU), 1, fp);
}

Boolean payloadReadFromFile(PayloadExperiment exp, Time* readStartTime, Time* readEndTime, readData *head){
	
	if(head == NULL){
		printf("Head is NULL\n");
		return FALSE;
	}
	if(!testTimes(readStartTime,readEndTime)){
		printf("Invalid times\n");
		return FALSE;
	}

	int size;
	F_FILE *fp;
	char rowTime[MAX_FILE_NAME_SIZE];
	char file_name[MAX_FILE_NAME_SIZE];
	int startTimeEpoch = Time_convertTimeToEpoch(readStartTime);
	int endTimeEpoch = Time_convertTimeToEpoch(readEndTime);
	readData* current = head;

	getExpName(exp,file_name,&size);
	fp = f_open(file_name, "r");
	if(!fp){
		printf("Unable to open file %s\n",file_name);
		return FALSE;
	}

	void *rowData;
	if(!determineExp(exp,&size,rowData)){
		printf("Unable to determine experiment\n");
		return FALSE;
	}

	while(f_eof(fp) == 0){
		f_read(rowTime, size, 1, fp);
		if (atoi(rowTime) >= startTimeEpoch && atoi(rowTime) <= endTimeEpoch){
			switch (exp) {
				case RADFET: {
					readRadData(fp,rowData);
					break;
				}
				case TEMP: {
					readTempData(fp,rowData);
					break;
				}
				case PIC32SEL: {
					readPIC32SELData(fp,rowData);
					break;
				}
				case PIC32SEU: {
					readPIC32SEUData(fp,rowData);
					break;
				}
			}
		}
		if(head->data == NULL){
			head->data = rowData;
			if(Time_convertEpochToTime(atoi(rowTime),&head->time)){
				printf("Error converting epoch to time\n");
				return FALSE;
			}
		}
		else{
			readData* new = malloc(sizeof(readData));
			new->data = rowData;
			if(Time_convertEpochToTime(atoi(rowTime),&new->time)){
				printf("Error converting epoch to time\n");
				return FALSE;
			}
			current->next = new;
			current = new;
		}

	}
	return TRUE;
}