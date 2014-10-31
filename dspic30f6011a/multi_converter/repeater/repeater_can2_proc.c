
#include <p30f6011a.h>            

#include  "..\multi_converter_common\host_io.h"            
#include  "..\multi_converter_common\30f6010_io.h"
#include  "..\multi_converter_common\you_can1.h"
#include  "..\multi_converter_common\you_can2.h"
#include  "..\multi_converter_common\ComPort1.h"
#include  "..\multi_converter_common\ComPort2.h"
#include  "..\multi_converter_common\memory_map.h"




unsigned int   __attribute__((section(".usercode"))) Can2ReceiveData(void)
{
	Can1TxEid= Can2RxEid;
	Can1TxSid= Can2RxSid;
	Can1TxDlc= Can2RxDlc;	

	Can1TxBuf[0]=Can2ThisRxBuf[0];		
	Can1TxBuf[1]=Can2ThisRxBuf[1];		
	Can1TxBuf[2]=Can2ThisRxBuf[2];		
	Can1TxBuf[3]=Can2ThisRxBuf[3];		
	Can1TxBuf[4]=Can2ThisRxBuf[4];		
	Can1TxBuf[5]=Can2ThisRxBuf[5];		
	Can1TxBuf[6]=Can2ThisRxBuf[6];		
	Can1TxBuf[7]=Can2ThisRxBuf[7];

	Can1PollingTimer=10;	
	Can1TxData(0,0);



	Com1TxBuffer[0]=(unsigned char)Can2ThisRxBuf[0];		
	Com1TxBuffer[1]=(unsigned char)Can2ThisRxBuf[1];		
	Com1TxBuffer[2]=(unsigned char)Can2ThisRxBuf[2];		
	Com1TxBuffer[3]=(unsigned char)Can2ThisRxBuf[3];		
	Com1TxBuffer[4]=(unsigned char)Can2ThisRxBuf[4];		
	Com1TxBuffer[5]=(unsigned char)Can2ThisRxBuf[5];		
	Com1TxBuffer[6]=(unsigned char)Can2ThisRxBuf[6];		
	Com1TxBuffer[7]=(unsigned char)Can2ThisRxBuf[7];
	
	Com1TxCnt=Can2RxDlc;
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
	
	Com2TxCnt=Can2RxDlc;
	Com2TxThisPt=0;	
	Com2TxStart();



/*

	Com1TxBuffer[0]=(unsigned char)(Can2RxSid >> 8);		
	Com1TxBuffer[1]=(unsigned char)(Can2RxSid);		
	Com1TxBuffer[2]=(unsigned char)(Can2RxEid >> 16);		
	Com1TxBuffer[3]=(unsigned char)(Can2RxEid >> 8);		
	Com1TxBuffer[4]=(unsigned char)(Can2RxEid);		
	Com1TxBuffer[5]=(unsigned char)(Can2TxDlc);	
	
	Com1TxBuffer[6]=(unsigned char)Can2ThisRxBuf[0];		
	Com1TxBuffer[7]=(unsigned char)Can2ThisRxBuf[1];		
	Com1TxBuffer[8]=(unsigned char)Can2ThisRxBuf[2];		
	Com1TxBuffer[9]=(unsigned char)Can2ThisRxBuf[3];		
	Com1TxBuffer[10]=(unsigned char)Can2ThisRxBuf[4];		
	Com1TxBuffer[11]=(unsigned char)Can2ThisRxBuf[5];		
	Com1TxBuffer[12]=(unsigned char)Can2ThisRxBuf[6];		
	Com1TxBuffer[13]=(unsigned char)Can2ThisRxBuf[7];
	
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
*/
	return(0);
}







