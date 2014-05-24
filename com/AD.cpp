// RB-100: Controll AX-12 by COM3
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "roboard.h"


int InitAD()
{
// first set the correct RoBoard version
    //roboio_SetRBVer(RB_100);
    //roboio_SetRBVer(RB_100RD);  // if your RoBoard is RB-100RD
    roboio_SetRBVer(RB_110);    // if your RoBoard is RB-110
    //roboio_SetRBVer(RB_050);    // if your RoBoard is RB-050

    if (spi_Init(SPICLK_21400KHZ) == false)  // init SPI lib for accessing the A/D converter
    {
        printf("ERROR: fail to init SPI lib (%s)!\n", roboio_GetErrMsg());
        return -1;
    }
return 0;
}

int * ReadAD()
{
int val[8]={0,0,0,0,0,0,0,0};//storage the data from AD
int i;
//InitAD();

 for (i=0; i<8; i++)
    {
        val[i] = adc_ReadCH(i);  // read the ADC input value on channel i

        if (val[i] == ADC_READFAIL)//val==(0x7fff)
            printf("ERROR: fail to read ADC value on channel %d!\n", i);
        else
            printf("ADC value on channel %d = %d (equiv. to %1.2fV)\n", i, val[i], 5.0*(float)val[i]/1023.0);
    }

    //spi_Close();  // close SPI lib
    return val;
} 

int  ReadADByID(unsigned int i)
{
int val=0;//storage the data from AD
//InitAD();


        val = adc_ReadCH(i);  // read the ADC input value on channel i

        if (val == ADC_READFAIL)//val==(0x7fff)
            printf("ERROR: fail to read ADC value on channel %d!\n", i);
        else
            printf("ADC value on channel %d = %d (equiv. to %1.2fV)\n", i, val, 5.0*(float)val/1023.0);

	
    //spi_Close();  // close SPI lib
    return val;
} 


