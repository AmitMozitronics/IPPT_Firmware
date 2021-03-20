/* 
 * File:   vmADS1232.h
 * Author: mrinmoy
 *
 * Created on October 10, 2019, 12:50 PM
 */

#ifndef VM_ADS1232_H
#define	VM_ADS1232_H

#define ADS1232_ADC_BITS                    19
#define ADS1232_ADC_EXCLUSION_BITS          3

//unsigned int32 read_adc(void)
//{
//    unsigned int32 dataa  = 0;
//    unsigned int8 i       = 0;
//    
//    // Delay required as per ADC1232 data-sheet; after data ready pin goes down
//    // data is ready but after a certain time i.e. data uploading time which is 
//    // 39us minimum when read-back is not allowed.[Page No. 18]
//    delay_us(50);
//    
//    // Retrieve data from DOUT/DRDY pin over SPI. 
//    // Here 24-bit ADC is used. But we retrieve 20-bit as it gives upto 19.2 
//    // resolution.
//    for(i = 0; i<20; i++)
//    {         
//        output_high(PIN_ADC_SCLK);
//        delay_us(1);
//        shift_left(&dataa,3,input_state(PIN_ADC_DOUT_DRDY));
//        output_low(PIN_ADC_SCLK);
//        delay_us(1);
//    }
//    
//   // Apply last clock pulse to make DOUT PIN high i.e.data retrieve done.
//   delay_us(1);
//   output_high(PIN_ADC_SCLK);
//   delay_us(1);
//   output_low(PIN_ADC_SCLK);    
//    
//   return dataa;
//}

unsigned int32 read(void)
{
    unsigned int32 RawADC  = 0; 
    unsigned int8 i       = 0;

    // Delay required as per ADC1232 data-sheet; after data ready pin goes down
    // data is ready but after a certain time i.e. data uploading time which is 
    // 39us minimum when read-back is not allowed.[Page No. 18]
    delay_us(50);
    
    // Retrieve data from DOUT/DRDY pin over SPI. 
    // Here 24-bit ADC is used. But we retrieve 24-bit as it gives upto 19.2 
    // resolution.
    
//    // Option 1: Taking all 24-bits for high resolution.
    for(i = 0; i<24; i++)
    {

        output_high(PIN_ADC_SCLK);
        delay_us(1);
        shift_left(&RawADC,3,input(PIN_ADC_DOUT_DRDY));
        output_low(PIN_ADC_SCLK);
        delay_us(1);
    } 
    
    // TESTING.
    // To reduce the noise-full or erroneous bits.
    RawADC = (RawADC & 0xFFFFFFF0);
    //RawADC >>= 4;
    
    // Option 2: Taking 21-bits to minimize data-size overhead by discarding 
    //           3-bits from MSB of ADC output.
//    for(i = 0; i<24; i++)
//    {
//        // Unacceptable.
//        if(i < ADS1232_ADC_EXCLUSION_BITS)
//        {
//            output_high(PIN_ADC_SCLK);
//            delay_us(1);  
//            output_low(PIN_ADC_SCLK);
//            delay_us(1);            
//        }
//        
//        // Acceptable.
//        else
//        {
//            output_high(PIN_ADC_SCLK);
//            delay_us(1);
//            shift_left(&RawADC,3,input(PIN_ADC_DOUT_DRDY));
//            output_low(PIN_ADC_SCLK);
//            delay_us(1);            
//        }
//        
//    }
    
    // Apply additional clock pulse to make DOUT/DRDY PIN high i.e.data retrieve done.
    delay_us(1);
    output_high(PIN_ADC_SCLK);
    delay_us(1);
    output_low(PIN_ADC_SCLK);    
     
    return RawADC;
}


#endif	/* VM_ADS1232_H */

