
#include <p30f6011a.h>            

#include  "..\..\multi_converter_common\host_io.h"            
#include  "..\..\multi_converter_common\30f6010_io.h"
#include  "..\..\multi_converter_common\you_can1.h"
#include  "..\..\multi_converter_common\you_can2.h"
#include  "..\..\multi_converter_common\ComPort1.h"
#include  "..\..\multi_converter_common\ComPort2.h"
#include  "..\..\multi_converter_common\memory_map.h"
#include  "..\..\multi_converter_common\WorkingRam.h"
#include  "..\..\multi_converter_common\multi_converter.h"
#include  "..\..\multi_converter_common\my_util.h"

#include	"myCall_Power_crt.h"




unsigned char __attribute__((section(".usercode")))	HogiSelect(void)
{
	unsigned char i;
	for(i=0;i<MAX_ELEV;i++){
		if(Com1RxBuffer[0] == ReqHost[i].HostGroup){
			return(i);
		}
	}
	return(0xff);	
}


unsigned int	BufTxCom1(unsigned char adr)
{  
	unsigned char i;
	unsigned tmpGroup,tmpLocal;
	unsigned int idpt;
  
 
    Com1TxBuffer[0]  = 0xfe;   
    Com1TxBuffer[1]  = adr;	
    Com1TxBuffer[2]  = 0xA3;                                        
	Com1TxBuffer[3]  = CRT_TX_DATA_TO_PC;        //HOST_DATA_RECOD;

	Com1TxCnt=0;
	Com1TxThisPt=0;

	idpt=HogiSelect();


	if( !(ReqHost[idpt].Ho_Host[4] & 0x01))	return(0);
	else{
		ReqHost[idpt].Ho_Host[4] = (ReqHost[idpt].Ho_Host[4] & 0xfe);
	}				


	if(idpt >= MAX_ELEV)	return(0);
	idpt=(idpt * HOST_DATA_RECOD);




/*
if(RcvBuf[41 + RCV_DATA + idpt] == 0)	return(0);
else{
	RcvBuf[41 + RCV_DATA + idpt]=0;
}

	ReqHost[i].Ho_Host[4]=0xff;

*/


    for(i=0;i<Com1TxBuffer[3];i++){
		Com1TxBuffer[4+i]=RcvBuf[i + RCV_DATA + idpt];
    }

/*
    for(;(i <Com1TxBuffer[3]);i++){
		Com1TxBuffer[4+i]=0;
    }
*/


	Com1TxCnt = (4 + Com1TxBuffer[3]);

    Crc=0xffff;
    for(i=0;i<Com1TxBuffer[3]+4;i++){
        Crc_Calulate((unsigned int)Com1TxBuffer[i]);
    }
    Com1TxBuffer[i]=(unsigned char)(Crc & 0x00ff);
    Com1TxBuffer[i+1]=(unsigned char)((Crc >> 8) & 0x00ff);

	Com1TxCnt = (Com1TxCnt + 2);

	Com1TxStart();

}




unsigned int  CrtCom1Check(void)
{
	unsigned char i,pt;
	unsigned int  tmpsid1,tmpsid2;

	if(Com1RxBuffer[2] == 0x23){
		BufTxCom1(Com1RxBuffer[0]);
		return(0);
	}
	else if(Com1RxBuffer[2] == 0x24){

/*
		for(i=0;i<MAX_ELEV;i++){
			if(Com1RxBuffer[0] == ReqHost[i].Ho_Host[0]){
				Can1TxDlc=8;
				Can1TxSid=ReqHost[i].HostSid;
				Can1TxEid=ReqHost[i].HostEid;
                break;
			}
		}

		Can1TxBuf[0]=Com1RxBuffer[2];
		Can1TxBuf[1]=Com1RxBuffer[3];
		Can1TxBuf[2]=Com1RxBuffer[4];
		Can1TxBuf[3]=Com1RxBuffer[5];
		Can1TxBuf[4]=Com1RxBuffer[6];
		Can1TxBuf[5]=Com1RxBuffer[7];
		Can1TxBuf[6]=Com1RxBuffer[8];
		Can1TxBuf[7]=Com1RxBuffer[9];

		AckWrCmd=1;
*/

		return(0);
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
