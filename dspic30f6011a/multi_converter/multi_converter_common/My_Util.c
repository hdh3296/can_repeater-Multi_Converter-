
#include  "ComPort1.h"
#include  "ComPort2.h"
#include  "you_can1.h"
#include  "you_can2.h"
#include  "memory_map.h"
#include  "WorkingRam.h"


unsigned    int  	Crc;  



void    Crc_Calulate(unsigned int crcdata)
{
	register unsigned int    i;

   Crc= Crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (Crc & 0x0001) == 0x0001)    Crc=(Crc >> 1) ^ 0xA001;
      else Crc=Crc >> 1;
   }
}




unsigned int	Common_Crc_Calulate(unsigned int orginal_crc,unsigned int crcdata)
{
	register unsigned int    i;

   orginal_crc= orginal_crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (orginal_crc & 0x0001) == 0x0001)    orginal_crc=(orginal_crc >> 1) ^ 0xA001;
      else orginal_crc=orginal_crc >> 1;
   }

	return(orginal_crc);
}


