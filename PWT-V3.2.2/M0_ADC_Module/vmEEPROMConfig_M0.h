#ifndef _VM_EEPROM_CONFIG_BRITANIA_H_
#define _VM_EEPROM_CONFIG_BRITANIA_H_


#define EEPROM_ADDRESS_FOR_LCF                       100
#define EEPROM_ADDRESS_FOR_MAX_WT                    110    // 4-Byte
#define EEPROM_ADDRESS_FOR_MIN_WT                    114    // 4-Byte
#define EEPROM_ADDRESS_FOR_PIPE_PRESENCE_DELAY       118
#define EEPROM_ADDRESS_FOR_PIPE_STABILIZATION_DELAY  120
#define EEPROM_ADDRESS_FOR_PIPE_TRANSFER_DELAY       122
#define EEPROM_ADDRESS_FOR_WEGHNG_CYLDR_UP_ACT_DELAY 124    // 2-Byte

void WriteLCFToEEPROM(int16 i16CurrLoadCellFactor)
{
    unsigned int16 ui16CurrLoadCellFactor = 0;
    
    // cast the receive parameter in unsugned.
    ui16CurrLoadCellFactor = (unsigned int16)i16CurrLoadCellFactor;
    
    // Store data in EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_LCF,ui16CurrLoadCellFactor);
}

int16 ReadLCFFromEEPROM(void)
{
    unsigned int16 ui16RetrieveLCF = 0;
    
    // Retrieve stored LCF from EEPROM.
    ui16RetrieveLCF = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_LCF);
    
    return((int16)ui16RetrieveLCF);
}

void WriteMaxWeightToEEPROM(int32 i32MaxWeight)
{
    unsigned int16 ui16WordValue = 0;
    unsigned int16 ui16EEPROMAddress = 0;
    
    // First store the MSB of 32-bit data.
    ui16WordValue = (unsigned int16)(i32MaxWeight >> 16);
    // Copy the address.
    ui16EEPROMAddress = EEPROM_ADDRESS_FOR_MAX_WT;
    WriteEEPROMInt16(ui16EEPROMAddress, ui16WordValue);
    
    // Now store the LSB of 32-bit data.
    ui16WordValue = (unsigned int16)(i32MaxWeight >> 0);
    // Increment the address.
    ui16EEPROMAddress = ui16EEPROMAddress + 2;
    WriteEEPROMInt16(ui16EEPROMAddress, ui16WordValue);
}

void WriteMinWeightToEEPROM(int32 i32MinWeight)
{
    unsigned int16 ui16WordValue = 0;
    unsigned int16 ui16EEPROMAddress = 0;
    
    // First store the MSB of 32-bit data.
    ui16WordValue = (unsigned int16)(i32MinWeight >> 16);
    // Copy the address.
    ui16EEPROMAddress = EEPROM_ADDRESS_FOR_MIN_WT;
    WriteEEPROMInt16(ui16EEPROMAddress, ui16WordValue);
    
    // Now store the LSB of 32-bit data.
    ui16WordValue = (unsigned int16)(i32MinWeight >> 0);
    // Increment the address.
    ui16EEPROMAddress = ui16EEPROMAddress + 2;
    WriteEEPROMInt16(ui16EEPROMAddress, ui16WordValue);    
}

int32 ReadMaxWeightFromEEPROM(void)
{
    unsigned int16 ui16WordValue     = 0;
    unsigned int16 ui16EEPROMAddress = 0;
    int32          i32MaxWeight      = 0;
    
    // Copy the address.
    ui16EEPROMAddress = EEPROM_ADDRESS_FOR_MAX_WT;
    ui16WordValue = ReadEEPROMInt16(ui16EEPROMAddress);
    // Retrieved the MSB of 32-bit data.
    i32MaxWeight = (int32)ui16WordValue;
    // Shift data to MSB.
    i32MaxWeight = (i32MaxWeight << 16);

    // Now retrieve LSB of 32-bit data.
    // Increment EEPROM address.
    ui16EEPROMAddress = ui16EEPROMAddress + 2;
    ui16WordValue = ReadEEPROMInt16(ui16EEPROMAddress);
    i32MaxWeight = (((int16)ui16WordValue) | (i32MaxWeight & 0xFFFF0000));
    
    return i32MaxWeight;
}

int32 ReadMinWeightFromEEPROM(void)
{
    unsigned int16 ui16WordValue = 0;
    unsigned int16 ui16EEPROMAddress = 0;
    int32          i32MinWeight      = 0;
    
    // Copy the address.
    ui16EEPROMAddress = EEPROM_ADDRESS_FOR_MIN_WT;
    ui16WordValue = ReadEEPROMInt16(ui16EEPROMAddress);
    // Retrieved the MSB of 32-bit data.
    i32MinWeight = (int32)ui16WordValue;
    // Shift data to MSB.
    i32MinWeight = (i32MinWeight << 16);

    // Now retrieve LSB of 32-bit data.
    // Increment EEPROM address.
    ui16EEPROMAddress = ui16EEPROMAddress + 2;
    ui16WordValue = ReadEEPROMInt16(ui16EEPROMAddress);
    i32MinWeight = (((int16)ui16WordValue) | (i32MinWeight & 0xFFFF0000));
    
    return i32MinWeight;    
}

void WritePipePresenceDelayToEEPROM(unsigned int16 ui16PipePresenceDelay)
{
    // Write data to EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_PRESENCE_DELAY, ui16PipePresenceDelay);
}

unsigned int16 ReadPipePresenceDelayFromEEPROM(void)
{
    unsigned int16 ui16PipePresenceDelayTime = 0;
    
    // Read data from EEPROM.    
    ui16PipePresenceDelayTime = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_PRESENCE_DELAY);
    
    return ui16PipePresenceDelayTime;
}

void WritePipeStabilizationDelayToEEPROM(unsigned int16 ui16PipeStabilizationDelay)
{
    // Write data to EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_STABILIZATION_DELAY, ui16PipeStabilizationDelay);    
}

unsigned int16 ReadPipeStabilizationDelayFromEEPROM(void)
{
    unsigned int16 ui16PipeStabilizationDelayTime = 0;
    
    // Read data from EEPROM.    
    ui16PipeStabilizationDelayTime = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_STABILIZATION_DELAY);
    
    return ui16PipeStabilizationDelayTime;    
}


void WritePipeTransferDelayToEEPROM(unsigned int16 g_ui16PipeTransferDelayTime)
{
    // Write data to EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_TRANSFER_DELAY, g_ui16PipeTransferDelayTime);    
}

unsigned int16 ReadPipeTransferDelayTime(void)
{
    unsigned int16 ui16PipeTransferDelayTime = 0;
    
    // Read data from EEPROM.
    ui16PipeTransferDelayTime = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_PIPE_TRANSFER_DELAY);
    
    return ui16PipeTransferDelayTime;     
}    

void WriteWeighingCylinderActivationDelayToEEPROM(unsigned int16 ui16DelayTime)
{
    // Write data to EEPROM.
    WriteEEPROMInt16(EEPROM_ADDRESS_FOR_WEGHNG_CYLDR_UP_ACT_DELAY, ui16DelayTime);    
}

unsigned int16 ReadWeighingCylinderActivationDelayfromEEPROM(void)
{
    unsigned int16 ui16DelayTime = 0;
    
    // Read data from EEPROM.
    ui16DelayTime = ReadEEPROMInt16(EEPROM_ADDRESS_FOR_WEGHNG_CYLDR_UP_ACT_DELAY);
    
    return ui16DelayTime;    
}
#endif /* _VM_EEPROM_CONFIG_BRITANIA_H_ */