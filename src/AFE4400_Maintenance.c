#include "AFE4400.h"
#include "AFE4400_Types.h"


uint8_t AFE4400_MemDump(AFE4400_Data_t *Data)
{
    return AFE4400_Read(LED2STC, &Data->LED2STC, CONTROL2);
}

uint8_t AFE4400_MemCmp(const AFE4400_Data_t *DataSend, AFE4400_Data_t *DataReceive, AFE4400_REGS_ADDRESS_t Address, uint8_t Size)
{

}