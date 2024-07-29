
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
#include <time.h>
#define MAX_FILE_NAME_SIZE 50
#define END_FILE_NAME_RADFET "radfet"
#define END_FILE_NAME_TEMP "temp"
#define END_FILE_NAME_PIC32SEL "pic32sel"
#define END_FILE_NAME_PIC32SEU "pic32seu"

void getExpData(PayloadExperiment exp, char* endFileName, int* structSize){

	if (exp==RADFET){
		memcpy(endFileName,END_FILE_NAME_RADFET,sizeof(END_FILE_NAME_RADFET));
		*structSize = sizeof(RadiationData);
	}

}

void calculateFileName(Time curr_date,char* file_name, char* endFileName, int days2Add)
{
	/* initialize */
	struct tm t = { .tm_year = curr_date.year + 100, .tm_mon = curr_date.month - 1, .tm_mday = curr_date.date };
	/* modify */
	t.tm_mday += days2Add;
	mktime(&t);

	char buff[7];
	strftime(buff, sizeof buff, "%y%0m%0d", &t);
	snprintf(file_name, MAX_FILE_NAME_SIZE, "%s.%s", buff, endFileName);
}

Boolean payloadWriteToFile(void* data, PayloadExperiment exp){
	//printf("writing tlm: %d to SD\n",tlmType);

	unsigned int curr_time;
	Time_getUnixEpoch(&curr_time);

	Time curr_date;
	Time_get(&curr_date);

	int size;
	F_FILE *fp;
	char file_name[MAX_FILE_NAME_SIZE] = {0};
	char end_file_name[3] = {0};

	getTlmTypeInfo(exp,end_file_name,&size);
	calculateFileName(curr_date,&file_name,end_file_name,0);
	fp = f_open(file_name, "a");

	if (!fp)
	{
		//printf("Unable to open file %s, f_open error=%d\n",file_name, err);
		return -1;
	}

	f_write(&curr_time , sizeof(curr_time) ,1, fp );
	f_write(data , size , 1, fp );

	/* close the file*/
	f_flush(fp);
	f_close (fp);
	return 0;
}