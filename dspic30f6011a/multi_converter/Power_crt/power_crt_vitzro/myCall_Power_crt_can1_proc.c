
#include <p30f6011a.h>            

#include  	"..\..\multi_converter_common\host_io.h"            
#include  	"..\..\multi_converter_common\30f6010_io.h"
#include  	"..\..\multi_converter_common\you_can1.h"
#include  	"..\..\multi_converter_common\you_can2.h"
#include  	"..\..\multi_converter_common\ComPort1.h"
#include  	"..\..\multi_converter_common\ComPort2.h"
#include  	"..\..\multi_converter_common\memory_map.h"
#include  	"..\..\multi_converter_common\WorkingRam.h"
#include  	"..\..\multi_converter_common\My_Util.h"
#include  	"..\..\multi_converter_common\multi_converter.h"

#include	"myCall_Power_crt.h"




unsigned    char    CanLiveTimeAdr[MAX_ELEV];








unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	return(0);
}







