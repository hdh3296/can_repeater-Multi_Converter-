
#include <p30f6011a.h>            

#include  "..\multi_converter_common\host_io.h"            
#include  "..\multi_converter_common\30f6010_io.h"
#include  "..\multi_converter_common\you_can1.h"
#include  "..\multi_converter_common\you_can2.h"
#include  "..\multi_converter_common\ComPort1.h"
#include  "..\multi_converter_common\ComPort2.h"
#include  "..\multi_converter_common\memory_map.h"
#include  "..\multi_converter_common\WorkingRam.h"
#include  "..\multi_converter_common\multi_converter.h"
#include  "..\multi_converter_common\my_util.h"

#include	"pcCmd_crt.h"




unsigned int  CrtCom1Check(void)
{
	unsigned char i,pt;
	unsigned int  tmpsid1,tmpsid2;

	pt=2;
	for(i=0;i<8;i++)	Can1TxBuf[i] = Com1RxBuffer[pt+i]; 
	
	tmpsid1=CRT_COMPANY;
	tmpsid2=((Com1RxBuffer[0] & 0x78) << 4);
	tmpsid1=(tmpsid1 | tmpsid2);

	
	Can1TxDlc=8;
	Can1TxSid=(unsigned int)(tmpsid1);
	Can1TxEid=0xfe;
	if(Com1RxBuffer[0] & 0x01)	Can1TxEid=(Can1TxEid | 0x4000);	
	if(Com1RxBuffer[0] & 0x02)	Can1TxEid=(Can1TxEid | 0x8000);	
	if(Com1RxBuffer[0] & 0x04)	Can1TxEid=(Can1TxEid | 0x10000);	
	if(Com1RxBuffer[0] & 0x80)	Can1TxEid=(Can1TxEid | 0x20000);	

	if(I_AM_LISTENING_MODE==1){
		if(Com1RxBuffer[4] == 0x0){
			Can1RxSid=  Can1TxSid;
			Can1RxEid= (Can1TxEid & 0xffffff00);
			RxCan1AndTxCom1();
			return(1);			
		}
	}

	if(Can1RxTimer >= 10){
		if( !Can1TxData(0,0)){
			Can1RxTimer=0;	
			return(1);                      
		}
	}
	return(0);                      
}



unsigned int   __attribute__((section(".usercode"))) Com1Crt_Protocol(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;

	Com1RxBuffer[Com1RxCnt]=rdata;


   	buf=rdata;

    if(Com1RxStatus != TX_SET){   

		if(Com1SerialTime > 4){
            Com1RxCnt=0;
		}

        Com1SerialTime=0;
        Com1RxBuffer[Com1RxCnt]=buf;

        if(Com1RxCnt < (MAX_COM1_RXBUF-1)){
            Com1RxCnt++;
        }
        else{
            Com1RxCnt=0;
            Com1RxBuffer[Com1RxCnt]=buf;
        }

        switch(Com1RxStatus){
        	case    STX_CHK:
                if(Com1RxCnt == 1){
                    Com1RxBuffer[0]=buf;				
                    Crc=0xffff;
                    Crc_Calulate((unsigned int)buf);
                }
                else if(Com1RxCnt == 2){
                	Crc_Calulate((unsigned int)buf);
                }
                else if(Com1RxCnt == 3){
                    if(((Com1RxBuffer[Com1RxCnt-1] >= 0x20)  && (Com1RxBuffer[Com1RxCnt-1] <= 0x2f))
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0xa0)  && (Com1RxBuffer[Com1RxCnt-1] <= 0xaf)) 
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0x30)  && (Com1RxBuffer[Com1RxCnt-1] <= 0x3f))
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0xb0)  && (Com1RxBuffer[Com1RxCnt-1] <= 0xbf)) ){       
						Com1RxStatus=DATA_CHK;
                        Crc_Calulate((unsigned int)buf);
                    }
                    else{
                        Crc=0xffff;
                        Com1RxBuffer[0]=Com1RxBuffer[1];
                        Com1RxBuffer[1]=Com1RxBuffer[2];
						Com1RxCnt=2;
                        Crc_Calulate((unsigned int)Com1RxBuffer[0]);
                        Crc_Calulate((unsigned int)Com1RxBuffer[1]);
                    }   
                }                
                else{
	                Com1RxCnt=1;    
	                Com1RxBuffer[0]=buf;
	                Crc=0xffff;
	                Crc_Calulate((unsigned int)buf);
                }
        		break;
        	case    DATA_CHK:                                    
	            if(Com1RxCnt >= (Com1RxBuffer[3] + 4) ){
                    Com1RxStatus=CHKSUM_LOW_CHK;
                }
                Crc_Calulate((unsigned int)buf);
                break;
        	case    CHKSUM_LOW_CHK:
        		temp=(unsigned char)(Crc & 0x00ff);
        		if(temp==buf){
        			Com1RxStatus=CHKSUM_HIGH_CHK;
        		}
        		else    Com1RxStatus=RX_ERROR;
        		break;
        	case    CHKSUM_HIGH_CHK:
        		Crc=((Crc >> 8 ) & 0x00ff);
        		temp=(unsigned char)Crc;
        		if(buf==temp){
                    Com1RxStatus=RX_GOOD;
        		}
        		else    Com1RxStatus=RX_ERROR;
        		break;
        	case    RX_GOOD:
				break;
        	case    RX_ERROR:
        		Com1RxStatus=STX_CHK;
        		Com1RxCnt=0;
        		break;
        	default:
        		Com1RxStatus=STX_CHK;
        		Com1RxCnt=0;
        		break;
        }
	}	

	return(0);
}



unsigned int   __attribute__((section(".usercode"))) Com1ReceiveData(unsigned char rdata)
{
	Com1Crt_Protocol(rdata);
}
