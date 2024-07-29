#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "payload_demo.h"

Boolean TEST_checkStatus(unsigned char* status,unsigned char* buffer)
{
    *status = buffer[2];
    if (status == 0)
    {
        return TRUE;
    }
    return FALSE;
}

int main() {

    unsigned char buffer1[12] = {0xAB, 0xCD, 0x00, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0xDE, 0xF0, 0x11};
    unsigned char buffer2[12] = {0xAB, 0xCD, 0x11, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0xDE, 0xF0, 0x00};
    unsigned char buffer3[12] = {0xAB, 0xCD, 0x00, 0x30, 0x00, 0x31, 0x00, 0x32, 0x00, 0xDE, 0xF0, 0x00};
    


    test_payloadSendCommand();
    test_retrieveADCData();
    test_clearWatchdog();
    test_softReset();
    test_masterPIC32SELWrite();
    test_masterPIC32SEUWrite();
    test_masterRadiationWrite();
    test_MasterTemperatureWrite();
    test_soreqDebugging();
    
    return 0;
}