#include "AFE4400.h"
#include "AFE4400_Types.h"


extern uint8_t AFE4400_MemDump(AFE4400_Data_t *Data)
{
    return AFE4400_Read(LED2STC, &Data->LED2STC, CONTROL2);
}

extern uint8_t AFE4400_MemCmp(const AFE4400_Data_t *DataSend, AFE4400_Data_t *DataReceive, AFE4400_REGS_ADDRESS_t Address, uint8_t Size)
{
    uint8_t i;
    uint8_t status = 0;
    uint32_t *Tx = (uint32_t*) &DataSend->CONTROL0;
    uint32_t *Rx = (uint32_t*) &DataReceive->CONTROL0;

    for(i=Address; i<=Size; i++)
    {
        if (Tx[i] != Rx[i])
        {
            status = i;
            break;
        }    
    }

    return status;
}