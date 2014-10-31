
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


///////////#include        "xyou_can.h"
#include        "xsetup.h"
#include        "xesdots.h"



/*

void  __attribute__((section(".usercode"))) Com1HomeCallAck(void)
{
	unsigned char i;

	Com1TxThisPt=0;
	Com1TxCnt=19;

	Com1RxBuffer[0]=((unsigned char)(HOMECALL_PC_ADDRESS));
	Com1RxBuffer[1]=(unsigned char)(cF_SetMyAddr1Value_A);
	Com1RxBuffer[2]=((unsigned char)(HOMECALL_BUTTON_CMD + 0x80));
	Com1RxBuffer[3]=0x0d;
	Com1RxBuffer[4]= ((unsigned char)(HOMECALL_BUTTON_SET_CMD));






*/

unsigned int   __attribute__((section(".usercode"))) Com1HeaderChk(void)
{
//	if(Com1RxBuffer[0] !=  ((unsigned char)(HOMECALL_MY_SET_ADDRESS)))	return(0);
//	if(Com1RxBuffer[0] >  0x07)											return(0);
	if(Com1RxBuffer[1] !=  ((unsigned char)(HOMECALL_PC_ADDRESS)))		return(0);

	if(Com1RxBuffer[2] !=  ((unsigned char)(HOMECALL_WRITE_CMD)) &&  (Com1RxBuffer[2] !=  ((unsigned char)(HOMECALL_READ_CMD))))	return(0); 
	return(1);
}


unsigned int   __attribute__((section(".usercode"))) Com1ReceiveData(unsigned char rdata)
{
   	unsigned char   tmp,i;

	if(Com1RxCnt >= (MAX_RTX_BUF-1))	Com1RxCnt=0;
	
	Com1RxBuffer[Com1RxCnt]=rdata;
	

	switch(Com1RxCnt){
		case	0:
#ifdef	XXXX
			Com1RxCnt++;	
#else
			i= (GROUP_NUMBER << 3);
			if((Com1RxBuffer[0] &   0xf8) == i){
				Com1RxCnt++;	
			}
			else	Com1RxCnt=0;
#endif	
			break;
		case	1:
			if(Com1RxBuffer[Com1RxCnt] ==  ((unsigned char)(HOMECALL_PC_ADDRESS))){
				Com1RxCnt++;	
			}
			else	Com1RxCnt=0;	
			break;
		case	2:
			if(Com1RxBuffer[Com1RxCnt] ==  ((unsigned char)(HOMECALL_WRITE_CMD))){
				Com1RxCnt++;	
			}
			else if(Com1RxBuffer[Com1RxCnt] ==  ((unsigned char)(HOMECALL_READ_CMD))){
				Com1RxCnt++;	
			}
			else	Com1RxCnt=0;	
			break;
		case	3:
			if(Com1HeaderChk()){
				Com1RxCnt++;	
			}	
			else	Com1RxCnt=0;	
			break;
		case	4:
			if(Com1HeaderChk()){
				Com1RxCnt++;	
			}	
			else	Com1RxCnt=0;	
			break;
		default:
			if((Com1RxBuffer[3]+5) == Com1RxCnt){
			    Crc=0xffff;
			    for(i=0;i< Com1RxBuffer[3]+4;i++){
			        Crc_Calulate( (unsigned int)(Com1RxBuffer[i]) );
			    }

				tmp=(unsigned char)(Crc);
				if(Com1RxBuffer[i] == tmp){
					tmp=(unsigned char)(Crc >> 8);
					if(Com1RxBuffer[i+1] == tmp){

						for(i=0;i<MAX_RTX_BUF;i++){	
							CommonBuf[i]=Com1RxBuffer[i];
						}
						HomeCallCmd(1);
					}
				} 
				Com1RxCnt=0;	
			}  
			else if(Com1RxCnt > (Com1RxBuffer[3]+5)){
				Com1RxCnt=0;	
			}
			else{
				Com1RxCnt++;	
			}
			break;
	}

	return(0);
}





