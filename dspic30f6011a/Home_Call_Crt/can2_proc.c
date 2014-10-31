
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

#include        "xsetup.h"
#include        "prj_app.h"
#include        "xesdots.h"
#include        "xyou_can.h"




unsigned char   Can2_TmpBuf[8];


#define  I_AM_MASTER    0x2000




unsigned int   __attribute__((section(".usercode"))) Can2ReceiveAddress(void)
{
	Can2RxLocalAddr=(unsigned char)((Can2RxEid >> 14) & (0x07));

	Can2RxGroupAddr=(unsigned char)(Can2RxSid >> 7);
//	if(Can2RxEid & 0x10000)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x10);
	if(Can2RxEid & 0x20000)	Can2RxGroupAddr=(Can2RxGroupAddr | 0x10);
	
	Can2RxSrcAddr=(unsigned char)(Can2RxGroupAddr << 3);
	Can2RxSrcAddr=(unsigned char)(Can2RxSrcAddr | Can2RxLocalAddr);
}



unsigned int   __attribute__((section(".usercode"))) Can2RxDataLoadTmpbuf(unsigned char *buf)
{
	*(buf+0)=(unsigned char)C2RX0B1;		
	*(buf+1)=(unsigned char)(C2RX0B1 >> 8);		
	*(buf+2)=(unsigned char)C2RX0B2;		
	*(buf+3)=(unsigned char)(C2RX0B2 >> 8);		
	*(buf+4)=(unsigned char)C2RX0B3;		
	*(buf+5)=(unsigned char)(C2RX0B3 >> 8);		
	*(buf+6)=(unsigned char)C2RX0B4;		
	*(buf+7)=(unsigned char)(C2RX0B4 >> 8);
		
	return(0);	
}




unsigned int  __attribute__((section(".usercode"))) HostReadMyData(void)
{
	
	unsigned	int	i,j;
 
	j=(Can2RxLocalAddr * HOST_DATA_RECOD);
	for(i=0;i<40;i++)	Can2TxBuf[i]=RcvBuf[j+i];

	Can2TxDataTotalCnt=40;
	Can2TxDataTxPointer=0;
																								
}


unsigned int  __attribute__((section(".usercode"))) Can2ButtonCallReturn(void)
{
	unsigned	int	i;
	

	Can2TxBuf[0]=((unsigned char)(HOMECALL_BUTTON_SET_CMD));

	Can2TxBuf[1]=HomeCallUpKey[0];
	Can2TxBuf[2]=HomeCallUpKey[1];
	Can2TxBuf[3]=HomeCallUpKey[2];
	Can2TxBuf[4]=HomeCallUpKey[3];

	Can2TxBuf[5]=HomeCallDnKey[0];
	Can2TxBuf[6]=HomeCallDnKey[1];
	Can2TxBuf[7]=HomeCallDnKey[2];
	Can2TxBuf[8]=HomeCallDnKey[3];

	Can2TxBuf[9] =HomeCallCarKey[0];
	Can2TxBuf[10]=HomeCallCarKey[1];
	Can2TxBuf[11]=HomeCallCarKey[2];
	Can2TxBuf[12]=HomeCallCarKey[3];

	Can2TxDataTotalCnt=13;
	Can2TxDataTxPointer=0;																								
}



unsigned int   __attribute__((section(".usercode"))) Can2CommandCheck(void)
{
	unsigned char i;

    switch(Can2_TmpBuf[0]){
        case    0x23:
            HostReadMyData();             
            break;
        case    0x24:
			CommonBuf[0]=Can2RxSrcAddr;
			for(i=0;i<8;i++)	CommonBuf[i+2]=Can2_TmpBuf[i];
			HomeCallButtonCallCmd(0);
			HomeCallControlCmd(0);

    		if((CommonBuf[4]    ==   (PC_COMMAND | 0x07)) && (CommonBuf[5] == 0x00)){				
				Can2ButtonCallReturn();
    		}
			else{
            	HostReadMyData();             
			}
            break;
		default:
			break;
    }        
}



unsigned int   __attribute__((section(".usercode"))) Can2ReceiveData(void)
{
	unsigned char	adr;

	if(Can2TxDataTotalCnt == Can2TxDataTxPointer){
		if(Can2TxDataTotalCnt == 0){
			Can2ReceiveAddress();

			adr= (GROUP_NUMBER << 3);
			if((Can2RxSrcAddr &   0xf8) != adr){
				return(0);
			}


			if(HostElevLive[Can2RxLocalAddr] < 20){
				Can2RxDataLoadTmpbuf(Can2_TmpBuf);
				Can2CommandCheck();
			}
		}
	}

	return(0);
}







void    Can2TxSidEidLoad(void)
{
	Can2TxSid=Can2RxSid;
	if(bitF_IamRepeater){
		Can2TxEid=(Can2RxEid);	
	}	
	else{
		Can2TxEid=(Can2RxEid  | I_AM_MASTER);	
		Can2TxEid=(Can2TxEid & 0xffffff00);
	}
	Can2TxDlc=8;
}


void    __attribute__((section(".usercode")))  Can2AckCurrentNormalData(void)
{
	unsigned int i,j;

	j=(Can2TxDataTotalCnt - Can2TxDataTxPointer);
	if(j >= 8)	j=8;
	Can2TxDlc=j;


	if( Can2TxDataTotalCnt <= (Can2TxDataTxPointer + Can2TxDlc)){
		j=8;
	}
	else{
		if(Can2TxDataTxPointer > 0){
			j= (Can2TxDataTxPointer / 8);
		}
		else	j=0;
	}

	Can2TxEid=(Can2TxEid | j);
}





void    __attribute__((section(".usercode")))  Can2TxCheck(void)
{
	if(Can2TxDataTotalCnt > Can2TxDataTxPointer){	
	    if(!C2TX0CONbits.TXREQ){
			Can2TxSidEidLoad();
			Can2AckCurrentNormalData();	
			
			if( !Can2TxData(Can2TxDataTxPointer,0)){
				Can2TxDataTxPointer=Can2TxDataTxPointer+8;
				if(Can2TxDataTotalCnt <= Can2TxDataTxPointer){
					Can2TxDataTotalCnt =0;
				 	Can2TxDataTxPointer=0;
				}					
			}	
		}
	}
}








