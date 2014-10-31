
#include <p30f6011a.h>            

#include  "host_io.h"            
#include  "30f6010_io.h"
#include  "you_can1.h"
#include  "you_can2.h"
#include  "ComPort1.h"
#include  "ComPort2.h"
#include  "memory_map.h"
#include  "WorkingRam.h"
#include  "dio.h"
#include  "prj_app.h"


////////////#include        "xyou_can.h"
#include        "xsetup.h"
#include        "xesdots.h"



/*

void  __attribute__((section(".usercode"))) Com2HomeCallAck(void)
{
	unsigned char i;

	Com2TxThisPt=0;
	Com2TxCnt=19;

	Com2RxBuffer[0]=((unsigned char)(HOMECALL_PC_ADDRESS));
	Com2RxBuffer[1]=(unsigned char)(cF_SetMyAddr1Value_A);
	Com2RxBuffer[2]=((unsigned char)(HOMECALL_BUTTON_CMD + 0x80));
	Com2RxBuffer[3]=0x0d;
	Com2RxBuffer[4]=((unsigned char)(HOMECALL_BUTTON_SET_CMD));

	Com2RxBuffer[5]=HomeCallUpKey[0];
	Com2RxBuffer[6]=HomeCallUpKey[1];
	Com2RxBuffer[7]=HomeCallUpKey[2];
	Com2RxBuffer[8]=HomeCallUpKey[3];

	Com2RxBuffer[9]=HomeCallDnKey[0];
	Com2RxBuffer[10]=HomeCallDnKey[1];
	Com2RxBuffer[11]=HomeCallDnKey[2];
	Com2RxBuffer[12]=HomeCallDnKey[3];

	Com2RxBuffer[13]=HomeCallCarKey[0];
	Com2RxBuffer[14]=HomeCallCarKey[1];
	Com2RxBuffer[15]=HomeCallCarKey[2];
	Com2RxBuffer[16]=HomeCallCarKey[3];

    Crc=0xffff;
    for(i=0;i< Com2RxBuffer[3]+4;i++){
        Crc_Calulate( (unsigned int)(Com2RxBuffer[i]) );
    }

	Com2RxBuffer[17]=(unsigned char)(Crc);
	Com2RxBuffer[18]=(unsigned char)(Crc >> 8);

	Com2TxStart();
}





*/

unsigned int   __attribute__((section(".usercode"))) Com2HeaderChk(void)
{
//	if(Com2RxBuffer[0] !=  ((unsigned char)(HOMECALL_MY_SET_ADDRESS)))	return(0);

	if(Com2RxBuffer[0] > 0x07)										return(0);
	if(Com2RxBuffer[1] !=  ((unsigned char)(HOMECALL_PC_ADDRESS)))	return(0);

	if(Com2RxBuffer[2] !=  ((unsigned char)(HOMECALL_WRITE_CMD)) &&  (Com2RxBuffer[2] !=  ((unsigned char)(HOMECALL_READ_CMD))))	return(0); 
	return(1);
}


unsigned int   __attribute__((section(".usercode"))) Com2ReceiveData(unsigned char rdata)
{
   	unsigned char   tmp,i;

/*
	Com1RxBuffer[Com1RxCnt]=rdata;
	if(Com1RxCnt >= 50)	Com1RxCnt=0;
	else	Com1RxCnt++;
*/



	if(Com2RxCnt >= (MAX_RTX_BUF-1))	Com2RxCnt=0;

	Com2RxBuffer[Com2RxCnt]=rdata;
	
	switch(Com2RxCnt){
		case	0:
//			Com2RxCnt++;	
//			if(Com2RxBuffer[Com2RxCnt] ==  (unsigned char)(cF_SetMyAddr1Value)){

			if(Com2RxBuffer[0] <= 0x07){
				Com2RxCnt++;	
			}
			else	Com2RxCnt=0;	

			break;
		case	1:
			if(Com2RxBuffer[Com2RxCnt] ==  ((unsigned char)(HOMECALL_PC_ADDRESS))){
				Com2RxCnt++;	
			}
			else	Com2RxCnt=0;	
			break;
		case	2:
			if(Com2RxBuffer[Com2RxCnt] ==  ((unsigned char)(HOMECALL_WRITE_CMD))){
				Com2RxCnt++;	
			}
			else if(Com2RxBuffer[Com2RxCnt] ==  ((unsigned char)(HOMECALL_READ_CMD))){
				Com2RxCnt++;	
			}
			else	Com2RxCnt=0;	
			break;
		case	3:
			if(Com2HeaderChk()){
				Com2RxCnt++;	
			} 
			else	Com2RxCnt=0;	
			break;
		case	4:
			if(Com2HeaderChk()){
				Com2RxCnt++;	
			} 
			else	Com2RxCnt=0;	
			break;
		default:
			if((Com2RxBuffer[3]+5) == Com2RxCnt){
			    Crc=0xffff;
			    for(i=0;i< Com2RxBuffer[3]+4;i++){
			        Crc_Calulate( (unsigned int)(Com2RxBuffer[i]) );
			    }

				tmp=(unsigned char)(Crc);
				if(Com2RxBuffer[i] == tmp){
					tmp=(unsigned char)(Crc >> 8);
					if(Com2RxBuffer[i+1] == tmp){
						for(i=0;i<MAX_RTX_BUF;i++){	
							CommonBuf[i]=Com2RxBuffer[i];
						}
						HomeCallCmd(2);
					}
				} 
				Com2RxCnt=0;	
			}  
			else if(Com2RxCnt > (Com2RxBuffer[3]+5)){
				Com2RxCnt=0;	
			}
			else{
				Com2RxCnt++;	
			}
			break;
	}

	return(0);
}





