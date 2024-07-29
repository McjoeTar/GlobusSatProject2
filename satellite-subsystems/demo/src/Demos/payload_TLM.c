
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


void getExpDate(Time curr_date, int days2Add, char *curr_date_str)
{
	/* initialize */
	struct tm t = { .tm_year = curr_date.year + 100, .tm_mon = curr_date.month - 1, .tm_mday = curr_date.date };
	/* modify */
	t.tm_mday += days2Add;
	mktime(&t);

	strftime(curr_date_str, 7, "%y%0m%0d", &t);
}


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

void writeExpTelemetry(F_FILE *fp,PayloadExperiment exp, void* data, char curr_date_str, int size){
	
	// Write the date
    f_write(curr_date_str, 6, 1, fp);
    
    // Write the common fields (OPCODE and STATUS)
    switch (exp) {
        case RADFET: {
            RadiationData *radData = (RadiationData *)data;
            f_write(radData->OPCODE, sizeof(radData->OPCODE), 1, fp);
            f_write(radData->STATUS, sizeof(radData->STATUS), 1, fp);
            f_write(radData->RADFET1, sizeof(radData->RADFET1), 1, fp);
            f_write(radData->RADFET2, sizeof(radData->RADFET2), 1, fp);
            break;
        }
        case TEMP: {
            TemperatureData *tempData = (TemperatureData *)data;
            f_write(tempData->OPCODE, sizeof(tempData->OPCODE), 1, fp);
            f_write(tempData->STATUS, sizeof(tempData->STATUS), 1, fp);
            f_write(tempData->TEMP, sizeof(tempData->TEMP), 1, fp);
            break;
        }
        case PIC32SEL: {
            PIC32SELData *picData = (PIC32SELData *)data;
            f_write(picData->OPCODE, sizeof(picData->OPCODE), 1, fp);
            f_write(picData->STATUS, sizeof(picData->STATUS), 1, fp);
            f_write(picData->PIC32SEL, sizeof(picData->PIC32SEL), 1, fp);
            f_write(picData->PIC32SELBACKUP, sizeof(picData->PIC32SELBACKUP), 1, fp);
            // Note: For the COUNTPIC32SEL and COUNTPIC32SELBACKUP fields, you need to handle writing the integers appropriately if needed.
            break;
        }
        case PIC32SEU: {
            PIC32SEUData *picSeudata = (PIC32SEUData *)data;
            f_write(picSeudata->OPCODE, sizeof(picSeudata->OPCODE), 1, fp);
            f_write(picSeudata->STATUS, sizeof(picSeudata->STATUS), 1, fp);
            f_write(picSeudata->PIC32SEU, sizeof(picSeudata->PIC32SEU), 1, fp);
            break;
        }
        default:
            ActionData *actionData = (ActionData *)data;
            f_write(actionData->OPCODE, sizeof(actionData->OPCODE), 1, fp);
            f_write(actionData->STATUS, sizeof(actionData->STATUS), 1, fp);
            break;
    }
}

Boolean payloadWriteToFile(void* data, PayloadExperiment exp){
	//printf("writing tlm: %d to SD\n",tlmType);

	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);

	Time curr_date;
	Time_get(&curr_date);

	int size;
	F_FILE *fp;
	char curr_date_str[MAX_FILE_NAME_SIZE] = {0};
	char file_name[MAX_FILE_NAME_SIZE] = {0};

	getExpName(exp,file_name,&size);
	getExpDate(curr_date,0,curr_date_str);
	fp = f_open(file_name, "a");

	if (!fp)
	{
		//printf("Unable to open file %s, f_open error=%d\n",file_name, err);
		return -1;
	}

	writeExpTelemetry(fp,exp,data,curr_date_str,size);

	/* close the file*/
	f_flush(fp);
	f_close (fp);
	return 0;
}


Boolean payloadDeleteFromFile(PayloadExperiment exp, Time startTime, Time endTime){
	//printf("writing tlm: %d to SD\n",tlmType);

	int size;
	F_FILE *fp;
	char rowTime[MAX_FILE_NAME_SIZE] = {0};
	char file_name[MAX_FILE_NAME_SIZE] = {0};

	getExpName(exp,file_name,&size);
	fp = f_open(file_name, "r");
	

}

Boolean payloadReadFromFile(){

}