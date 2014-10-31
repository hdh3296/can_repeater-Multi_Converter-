
#include <p30f6011a.h>            

#include  "..\..\multi_converter_common\host_io.h"            
#include  "..\..\multi_converter_common\30f6010_io.h"
#include  "..\..\multi_converter_common\you_can1.h"
#include  "..\..\multi_converter_common\you_can2.h"
#include  "..\..\multi_converter_common\ComPort1.h"
#include  "..\..\multi_converter_common\ComPort2.h"
#include  "..\..\multi_converter_common\memory_map.h"
#include  "..\..\multi_converter_common\WorkingRam.h"
#include  "..\..\multi_converter_common\multi_converter.h"
#include  "..\..\multi_converter_common\my_util.h"

#include	"req_485_232_ack_can.h"


/*
#include  "..\..\multi_converter_common\host_io.h"            
#include  "..\..\multi_converter_common\30f6010_io.h"
#include  "..\..\multi_converter_common\you_can1.h"
#include  "..\..\multi_converter_common\you_can2.h"
#include  "..\..\multi_converter_common\ComPort1.h"
#include  "..\..\multi_converter_common\ComPort2.h"
#include  "..\..\multi_converter_common\memory_map.h"
#include  "..\..\multi_converter_common\WorkingRam.h"

#include	"req_485_232_ack_can.h"
*/

unsigned int   __attribute__((section(".usercode"))) Com2ReceiveData(unsigned char rdata)
{
    if(Com2RxStatus != TX_SET){   
		Com1SerialTime=0;
		Com1Crt_Protocol(rdata);
	}
	return(0);
}





