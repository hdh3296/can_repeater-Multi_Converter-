
#include <p30f6011a.h>            

#include  	"..\multi_converter_common\host_io.h"            
#include  	"..\multi_converter_common\30f6010_io.h"
#include  	"..\multi_converter_common\you_can1.h"
#include  	"..\multi_converter_common\you_can2.h"
#include  	"..\multi_converter_common\ComPort1.h"
#include  	"..\multi_converter_common\ComPort2.h"
#include  	"..\multi_converter_common\memory_map.h"
#include  	"..\multi_converter_common\WorkingRam.h"
#include	"repeater.h"






unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{	
	Can2TxEid= Can1RxEid;
	Can2TxSid= Can1RxSid;
	Can2TxDlc= Can1RxDlc;	

	Can2TxBuf[0]=Can1ThisRxBuf[0];		
	Can2TxBuf[1]=Can1ThisRxBuf[1];		
	Can2TxBuf[2]=Can1ThisRxBuf[2];		
	Can2TxBuf[3]=Can1ThisRxBuf[3];		
	Can2TxBuf[4]=Can1ThisRxBuf[4];		
	Can2TxBuf[5]=Can1ThisRxBuf[5];		
	Can2TxBuf[6]=Can1ThisRxBuf[6];		
	Can2TxBuf[7]=Can1ThisRxBuf[7];

	Can2PollingTimer=10;	
	Can2TxData(0,0);


/*
	Com1TxBuffer[0]=(unsigned char)Can1ThisRxBuf[0];		
	Com1TxBuffer[1]=(unsigned char)Can1ThisRxBuf[1];		
	Com1TxBuffer[2]=(unsigned char)Can1ThisRxBuf[2];		
	Com1TxBuffer[3]=(unsigned char)Can1ThisRxBuf[3];		
	Com1TxBuffer[4]=(unsigned char)Can1ThisRxBuf[4];		
	Com1TxBuffer[5]=(unsigned char)Can1ThisRxBuf[5];		
	Com1TxBuffer[6]=(unsigned char)Can1ThisRxBuf[6];		
	Com1TxBuffer[7]=(unsigned char)Can1ThisRxBuf[7];
	
	Com1TxCnt=Can1RxDlc;
	Com1TxThisPt=0;	

	Com1TxStart();



	Com2TxBuffer[0]=Com1TxBuffer[0];		
	Com2TxBuffer[1]=Com1TxBuffer[1];		
	Com2TxBuffer[2]=Com1TxBuffer[2];		
	Com2TxBuffer[3]=Com1TxBuffer[3];		
	Com2TxBuffer[4]=Com1TxBuffer[4];		
	Com2TxBuffer[5]=Com1TxBuffer[5];		
	Com2TxBuffer[6]=Com1TxBuffer[6];		
	Com2TxBuffer[7]=Com1TxBuffer[7];
	
	Com2TxCnt=Can1RxDlc;
	Com2TxThisPt=0;	
	Com2TxStart();

*/





	Com1TxBuffer[0]=(unsigned char)(Can1RxSid >> 8);		
	Com1TxBuffer[1]=(unsigned char)(Can1RxSid);		
	Com1TxBuffer[2]=(unsigned char)(Can1RxEid >> 16);		
	Com1TxBuffer[3]=(unsigned char)(Can1RxEid >> 8);		
	Com1TxBuffer[4]=(unsigned char)(Can1RxEid);		
	Com1TxBuffer[5]=(unsigned char)(Can1TxDlc);	
	
	Com1TxBuffer[6]=(unsigned char)Can1ThisRxBuf[0];		
	Com1TxBuffer[7]=(unsigned char)Can1ThisRxBuf[1];		
	Com1TxBuffer[8]=(unsigned char)Can1ThisRxBuf[2];		
	Com1TxBuffer[9]=(unsigned char)Can1ThisRxBuf[3];		
	Com1TxBuffer[10]=(unsigned char)Can1ThisRxBuf[4];		
	Com1TxBuffer[11]=(unsigned char)Can1ThisRxBuf[5];		
	Com1TxBuffer[12]=(unsigned char)Can1ThisRxBuf[6];		
	Com1TxBuffer[13]=(unsigned char)Can1ThisRxBuf[7];
	
	Com1TxCnt=14;
	Com1TxThisPt=0;	



	Com2TxBuffer[0]=Com1TxBuffer[0];		
	Com2TxBuffer[1]=Com1TxBuffer[1];		
	Com2TxBuffer[2]=Com1TxBuffer[2];		
	Com2TxBuffer[3]=Com1TxBuffer[3];		
	Com2TxBuffer[4]=Com1TxBuffer[4];		
	Com2TxBuffer[5]=Com1TxBuffer[5];	
	
	Com2TxBuffer[6]=Com1TxBuffer[6];		
	Com2TxBuffer[7]=Com1TxBuffer[7];		
	Com2TxBuffer[8]=Com1TxBuffer[8];		
	Com2TxBuffer[9]=Com1TxBuffer[9];		
	Com2TxBuffer[10]=Com1TxBuffer[10];		
	Com2TxBuffer[11]=Com1TxBuffer[11];		
	Com2TxBuffer[12]=Com1TxBuffer[12];		
	Com2TxBuffer[13]=Com1TxBuffer[13];
	
	Com2TxCnt=14;
	Com2TxThisPt=0;	


	Com1TxStart();
	Com2TxStart();

	return(0);
}


