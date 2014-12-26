
#include <p30f6011a.h>            
#include "ComPort2.h"            
#include  "..\header\30f6010_io.h"
#include  "..\can\you_can1.h"
#include  "..\can\you_can2.h"


//#include <p30f6011a.h>            
//#include "ComPort2.h"            
//#include "iodef.h"            
//#include "ds12c887.h"            



int   __attribute__((section(".usercode"))) Com2RxDataChk(void)
{
/*
	if(Com2RxStatus==RX_GOOD){
		Com2RxWork();
		Com2RxStatus=STX_CHK;
	}
*/
	return(0);	
}




int   __attribute__((section(".usercode"))) Com2TxAckChk(void)
{
	unsigned char i;

	if(bCan1RxAll || bCan2RxAll){
		if(bCan1RxAll){
			bCan1RxAll=0;
			Com2TxCnt=8;	
			for(i=0;i<Com2TxCnt;i++){
				Com2RxBuffer[i]=Can1RxBuf[i];
			}			
			
			Com2TxStart();
		}
		else if(bCan2RxAll){
			bCan2RxAll=0;
			Com2TxCnt=8;	

			for(i=0;i<Com2TxCnt;i++){
				Com2RxBuffer[i]=Can2RxBuf[i];
			}			

			Com2TxStart();
		}

/*
		if(!Com1TxWork()){
			Com1TxStart();
		}
		Com1SerialTime=0;
*/
	}

    if(_U2TRMT){
//    if((Com2RxStatus != TX_SET) || _U2TRMT){
		TXEN_485=0;
	}


	return(0);	


/*
	unsigned int val;

	if(Com2SerialTime >= 29){
		if(!Com2TxWork()){
			Com2TxStart();
		}

		Com2SerialTime=0;
	}
*/
	return(0);	
}



int   __attribute__((section(".usercode"))) Com2App(void)
{
	Com2RxDataChk();
	Com2TxAckChk();
	return(0);	
}
