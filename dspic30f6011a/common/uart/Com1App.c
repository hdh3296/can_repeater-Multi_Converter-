
#include <p30f6011a.h>            
#include "ComPort1.h"            
#include  "..\header\30f6010_io.h"
#include  "..\can\you_can1.h"
#include  "..\can\you_can2.h"



int   __attribute__((section(".usercode"))) Com1RxDataChk(void)
{
/*
	unsigned int	i;

	if(Com1RxStatus==RX_GOOD){
		Com1RxWork();
		Com1RxStatus=STX_CHK;
	}
*/
	return(0);	
}




int   __attribute__((section(".usercode"))) Com1TxAckChk(void)
{
	unsigned char i;

	if(bCan1RxAll || bCan2RxAll){
		if(bCan1RxAll){
			bCan1RxAll=0;
			Com1TxCnt=8;	
			for(i=0;i<Com1TxCnt;i++){
				Com1RxBuffer[i]=Can1RxBuf[i];
			}			
			Com1TxStart();
		}
		else if(bCan2RxAll){
			bCan2RxAll=0;
			Com1TxCnt=8;	
			for(i=0;i<Com1TxCnt;i++){
				Com1RxBuffer[i]=Can2RxBuf[i];
			}
			Com1TxStart();
		}



//		if(!Com1TxWork()){
//			Com1TxStart();
//		}
//		Com1SerialTime=0;

	}


//    if((Com1RxStatus != TX_SET) && _U1TRMT){
//		Com1TxStart();
//	}


	return(0);	
}



int   __attribute__((section(".usercode"))) Com1App(void)
{
	Com1RxDataChk();
	Com1TxAckChk();
	return(0);	
}



