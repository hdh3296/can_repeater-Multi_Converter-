
#include <p30f6011a.h>            

#include  "..\multi_converter_common\host_io.h"            
#include  "..\multi_converter_common\30f6010_io.h"
#include  "..\multi_converter_common\you_can1.h"
#include  "..\multi_converter_common\you_can2.h"
#include  "..\multi_converter_common\ComPort1.h"
#include  "..\multi_converter_common\ComPort2.h"
#include  "..\multi_converter_common\memory_map.h"
#include  "..\multi_converter_common\WorkingRam.h"

#include	"ezville_HomeCall.h"



/*
unsigned int   __attribute__((section(".usercode"))) Com1HeaderChk(void)
{
//	if(Com1RxBuffer[0] !=  ((unsigned char)(HOMECALL_MY_SET_ADDRESS)))	return(0);
	if(Com1RxBuffer[0] >  0x07)											return(0);
	if(Com1RxBuffer[1] !=  ((unsigned char)(HOMECALL_PC_ADDRESS)))		return(0);

	if(Com1RxBuffer[2] !=  ((unsigned char)(HOMECALL_WRITE_CMD)) &&  (Com1RxBuffer[2] !=  ((unsigned char)(HOMECALL_READ_CMD))))	return(0); 
	return(1);
}

*/



unsigned int   __attribute__((section(".usercode"))) Com1ReceiveData(unsigned char rdata)
{

	HomeCallProtocolChk(rdata);

}
