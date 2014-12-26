
#include <p30f6011a.h>            

#include  "..\working_ram\workingram.h"
#include  "..\can\you_can1.h"
#include  "..\can\you_can2.h"
#include  "..\uart\ComPort1.h"
#include  "..\uart\ComPort2.h"


unsigned int   __attribute__((section(".usercode"))) Can1RxBufSave(void)
{
	Can1RxBuf[0]=(unsigned char)C1RX0B1;		
	Can1RxBuf[1]=(unsigned char)(C1RX0B1 >> 8);		
	Can1RxBuf[2]=(unsigned char)C1RX0B2;		
	Can1RxBuf[3]=(unsigned char)(C1RX0B2 >> 8);;		
	Can1RxBuf[4]=(unsigned char)C1RX0B3;		
	Can1RxBuf[5]=(unsigned char)(C1RX0B3 >> 8);;		
	Can1RxBuf[6]=(unsigned char)C1RX0B4;		
	Can1RxBuf[7]=(unsigned char)(C1RX0B4 >> 8);;		

	return(0);
}

unsigned int   __attribute__((section(".usercode"))) TxCan2Buf(void)
{
	unsigned char i;
	Can1RxBufSave();


	Com1TxThisPt=0;
	Com2TxThisPt=0;

	Can2TxSid=Can1RxSid;
	Can2TxEid=Can1RxEid;
	Can2TxDlc=Can1RxDlc;

	Com1TxCnt=Can1RxDlc + 6;	
	Com2TxCnt=Can1RxDlc + 6;	
	for(i=0;i<Can1RxDlc;i++){
		Can2TxBuf[i]   =Can1RxBuf[i];		
		Com1RxBuffer[i+6]=Can1RxBuf[i];
		Com2RxBuffer[i+6]=Can1RxBuf[i];
	}

	Com1RxBuffer[0]=(unsigned char)(Can1RxSid >> 8);
	Com1RxBuffer[1]=(unsigned char)(Can1RxSid);
	Com1RxBuffer[2]=(unsigned char)(Can1RxEid >> 16);
	Com1RxBuffer[3]=(unsigned char)(Can1RxEid >> 8);
	Com1RxBuffer[4]=(unsigned char)(Can1RxEid);
	Com1RxBuffer[5]=(unsigned char)(Can1RxDlc);

	Com2RxBuffer[0]=Com1RxBuffer[0];
	Com2RxBuffer[1]=Com1RxBuffer[1];
	Com2RxBuffer[2]=Com1RxBuffer[2];
	Com2RxBuffer[3]=Com1RxBuffer[3];
	Com2RxBuffer[4]=Com1RxBuffer[4];
	Com2RxBuffer[5]=Com1RxBuffer[5];

	if(!bitF_Sid_Eid_Dl){
		Com1TxThisPt=6;
		Com2TxThisPt=6;
	}

	Can2TxData(0);
	Com1TxStart();
	Com2TxStart();

	return(0);
}





////for repeater//////////
unsigned int   __attribute__((section(".usercode"))) Can2RxBufSave(void)
{
	Can2RxBuf[0]=(unsigned char)C2RX0B1;		
	Can2RxBuf[1]=(unsigned char)(C2RX0B1 >> 8);		
	Can2RxBuf[2]=(unsigned char)C2RX0B2;		
	Can2RxBuf[3]=(unsigned char)(C2RX0B2 >> 8);;		
	Can2RxBuf[4]=(unsigned char)C2RX0B3;		
	Can2RxBuf[5]=(unsigned char)(C2RX0B3 >> 8);;		
	Can2RxBuf[6]=(unsigned char)C2RX0B4;		
	Can2RxBuf[7]=(unsigned char)(C2RX0B4 >> 8);;		
}

unsigned int   __attribute__((section(".usercode"))) TxCan1Buf(void)
{
	unsigned char i;

	Can2RxBufSave();

	Com1TxThisPt=0;
	Com2TxThisPt=0;

	Can1TxSid=Can2RxSid;
	Can1TxEid=Can2RxEid;
	Can1TxDlc=Can2RxDlc;

	Com1TxCnt=Can2RxDlc+6;	
	Com2TxCnt=Can2RxDlc+6;	
	for(i=0;i<Can2RxDlc;i++){
		Can1TxBuf[i]     =Can2RxBuf[i];		
		Com1RxBuffer[i+6]=Can2RxBuf[i];
		Com2RxBuffer[i+6]=Can2RxBuf[i];
	}

	Com1RxBuffer[0]=(unsigned char)(Can2RxSid >> 8);
	Com1RxBuffer[1]=(unsigned char)(Can2RxSid);
	Com1RxBuffer[2]=(unsigned char)(Can2RxEid >> 16);
	Com1RxBuffer[3]=(unsigned char)(Can2RxEid >> 8);
	Com1RxBuffer[4]=(unsigned char)(Can2RxEid);
	Com1RxBuffer[5]=(unsigned char)(Can2RxDlc);

	Com2RxBuffer[0]=Com1RxBuffer[0];
	Com2RxBuffer[1]=Com1RxBuffer[1];
	Com2RxBuffer[2]=Com1RxBuffer[2];
	Com2RxBuffer[3]=Com1RxBuffer[3];
	Com2RxBuffer[4]=Com1RxBuffer[4];
	Com2RxBuffer[5]=Com1RxBuffer[5];

	if(!bitF_Sid_Eid_Dl){
		Com1TxThisPt=6;
		Com2TxThisPt=6;
	}

	Can1TxData(0);
	Com1TxStart();
	Com2TxStart();

	return(0);
}




//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////



unsigned int   __attribute__((section(".usercode"))) Can1TxRepeater(void)
{
	unsigned char i;

	if(Can1TxCnt > 0){
		if((Can1RxCnt >= 8) || ((Com1SerialTime > 2) && (Com2SerialTime > 2)) ){
			Can1TxSid=0;
			Can1TxEid=0;
			if(Can1TxCnt>=8)		Can1TxDlc=8;
			else					Can1TxDlc=Can1TxCnt;	
			i=Can1TxData(Can1TxThisPt);
			if(i==0){
				Can1TxThisPt=(Can1TxThisPt + Can1TxDlc);
				if(Can1TxThisPt >= Can1TxCnt){
					Can1TxThisPt=0;
					Can1TxCnt=0;
				} 	
			}
		}
	}
	else{
		Can1TxThisPt=0;
		Can1TxCnt=0;
	}
}


unsigned int   __attribute__((section(".usercode"))) Can2TxRepeater(void)
{
	unsigned char i;

	if(Can2TxCnt > 0){
		if((Can2RxCnt >= 8) || ( (Com1SerialTime > 2) && (Com2SerialTime > 2))){
			Can2TxSid=0;
			Can2TxEid=0;
			if(Can2TxCnt >= 8)		Can2TxDlc=8;
			else					Can2TxDlc=Can2TxCnt;	
			i=Can2TxData(Can2TxThisPt);
			if(i==0){
				Can2TxThisPt=(Can2TxThisPt + Can2TxDlc);
				if(Can2TxThisPt >= Can2TxCnt){
					Can2TxThisPt=0;
					Can2TxCnt=0;
				} 	
			}
		}
	}
	else{
		Can2TxThisPt=0;
		Can2TxCnt=0;
	}
}






unsigned int   __attribute__((section(".usercode"))) Com1Rx(unsigned char rdata)
{
	unsigned char i;

	Com2RxBuffer[Com2TxCnt]=rdata;
//////////////////////////////////////////	Com2TxThisPt=0;
	Com2TxCnt++;	

//	Com2TxCnt=1;	
//	Com2TxStart();

	Can1TxBuf[Can1TxCnt]   =rdata;		
	Can2TxBuf[Can2TxCnt]   =rdata;
	Can1TxCnt++;		
	Can2TxCnt++;		

	if(Can1TxCnt > (CAN1_MAX_SAVE_BUF-2))	Can1TxCnt=0;	
	if(Can2TxCnt > (CAN2_MAX_SAVE_BUF-2))	Can2TxCnt=0;	

	return(0);
}



//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////

unsigned int   __attribute__((section(".usercode"))) Com2Rx(unsigned char rdata)
{
	unsigned char i;

	Com1RxBuffer[Com1TxCnt]=rdata;
//	Com1TxThisPt=0;
	Com1TxCnt++;	

//	Com1TxCnt=1;	
//	Com1TxStart();



	Can1TxBuf[Can1TxCnt]   =rdata;		
	Can2TxBuf[Can2TxCnt]   =rdata;
	Can1TxCnt++;		
	Can2TxCnt++;		

	if(Can1TxCnt > (CAN1_MAX_SAVE_BUF-2))	Can1TxCnt=0;	
	if(Can2TxCnt > (CAN2_MAX_SAVE_BUF-2))	Can2TxCnt=0;	


	return(0);
}




unsigned int   __attribute__((section(".usercode"))) Com1TxRepeater(void)
{
	unsigned char i;

	if( (Com1TxCnt > 0) && (Com2SerialTime > 2) && (Com1SerialTime > 2)){
		Com1TxThisPt=0;
		Com1TxStart();
		Com1SerialTime=0;
	}
}


unsigned int   __attribute__((section(".usercode"))) Com2TxRepeater(void)
{
	unsigned char i;

	if( (Com2TxCnt > 0) && (Com2SerialTime > 2) && (Com1SerialTime > 2)){
//        if((Com2RxBuffer[0] != '0') || (Com2RxBuffer[1] != '0') || (Com2RxBuffer[2] != 'f')){
//			i=0;
//		}
		Com2TxThisPt=0;
		Com2TxStart();
		Com2SerialTime=0;

	}
}
