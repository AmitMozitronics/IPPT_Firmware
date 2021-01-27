/* 
 * File:   mzEEPROMConfig_MCU1.h
 * Author: mrinmoy
 *
 * Created on January 10, 2020, 4:11 PM
 */

#ifndef MZ_EEPROM_CONFIG_MCU1_H
#define	MZ_EEPROM_CONFIG_MCU1_H

#include "../Common/vmEEPROMConfig.h"


#define  EEPROM_ADDRESS_FOR_MACHINE_ID                      10
#define  EEPROM_ADDRESS_FOR_INTERVAL                        12
#define  EEPROM_ADDRESS_FOR_CURRENT_UNIX_TIME               17 // 4-byte data. It will take 4 address memory block.
#define  EEPROM_ADDRESS_FOR_SIM_OPERATOR_ID                 22

void WriteMachineIDToEEPROM(unsigned int8 ui8MachineID)
{
    // Write the data into memory.
    WriteEEPROMInt8(EEPROM_ADDRESS_FOR_MACHINE_ID,ui8MachineID);
}

 unsigned int8 ReadMachineIDFromEEPROM(void)
{
    unsigned int8 ui8MachineID = 0;
    
    // Fetch Machine ID from memory.
    ui8MachineID = ReadEEPROMInt8(EEPROM_ADDRESS_FOR_MACHINE_ID);
    
    return ui8MachineID;
}
 


void WriteIntervalToEEPROM(unsigned int8 ui8IntervalTime)
{
    // Write the data into memory.
    WriteEEPROMInt8(EEPROM_ADDRESS_FOR_INTERVAL,ui8IntervalTime);
}

 unsigned int8 ReadIntervalFromEEPROM(void)
{
    unsigned int8 ui8DataSendInterval = 0;
    
    // Fetch Machine ID from memory.
    ui8DataSendInterval = ReadEEPROMInt8(EEPROM_ADDRESS_FOR_INTERVAL);
    
    return ui8DataSendInterval;
}

 
void WriteCurrentUnixTimeToEEPROM(unsigned int32 ui32CurrentUnixTime) 
{
    unsigned int16 ui16Temp = 0;
    unsigned int8   ui8EEPROMAddress = 0;
    
    ui8EEPROMAddress = EEPROM_ADDRESS_FOR_CURRENT_UNIX_TIME;
    
    // Parse the MSB part of unix time register.
    ui16Temp = (unsigned int16)((ui32CurrentUnixTime >> 16) & 0x0000FFFF);
    
    // Write data in EEPROM.
    WriteEEPROMInt16(ui8EEPROMAddress, ui16Temp);
    
    // Increment address.
    ui8EEPROMAddress += 2;
    
    // Parse LSB part of the unix time register.
    ui16Temp = (unsigned int16)((ui32CurrentUnixTime >> 0) & 0x0000FFFF);
    
        // Write data in EEPROM.
    WriteEEPROMInt16(ui8EEPROMAddress, ui16Temp);
}

unsigned int32 ReadStoredUnixTimeFromEEPROM(void)
{
    unsigned int32 ui32StoredUnixTime = 0;
    unsigned int16 ui16Temp = 0;
    unsigned int8  ui8EEPROMAddress = 0;
    
    ui8EEPROMAddress = EEPROM_ADDRESS_FOR_CURRENT_UNIX_TIME;
    
    ui16Temp = ReadEEPROMInt16(ui8EEPROMAddress);
    ui32StoredUnixTime = ui16Temp;
    
    ui32StoredUnixTime = (ui32StoredUnixTime << 16);
    ui8EEPROMAddress += 2;
    
    ui16Temp = ReadEEPROMInt16(ui8EEPROMAddress);
    ui32StoredUnixTime = ((ui32StoredUnixTime & 0xFFFF0000) | ui16Temp);
    
    
    // This stored data may be fresh or may be 1 minute past time.
    // To covered up we manipulate it; we add 30 seconds with stored time.
    ui32StoredUnixTime = ui32StoredUnixTime + 30;
    
    return ui32StoredUnixTime;    
}

void WriteSIMOperatorIDToEEPROM(unsigned int8 ui8SIMOperatorID)
{
    // Write the data into memory.
    WriteEEPROMInt8(EEPROM_ADDRESS_FOR_SIM_OPERATOR_ID,ui8SIMOperatorID);    
}


unsigned int8 ReadSIMOPeratorIDFromEEPROM(void)
{
    unsigned int8 ui8SIMOperatorID = 0;
    
    // Fetch Machine ID from memory.
    ui8SIMOperatorID = ReadEEPROMInt8(EEPROM_ADDRESS_FOR_SIM_OPERATOR_ID);
    
    return ui8SIMOperatorID;    
}
    

#endif	/* MZ_EEPROM_CONFIG_MCU1_H */

