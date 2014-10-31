
#include <p30f6011a.h>            

#include  	"..\..\multi_converter_common\host_io.h"            
#include  	"..\..\multi_converter_common\30f6010_io.h"
#include  	"..\..\multi_converter_common\you_can1.h"
#include  	"..\..\multi_converter_common\you_can2.h"
#include  	"..\..\multi_converter_common\ComPort1.h"
#include  	"..\..\multi_converter_common\ComPort2.h"
#include  	"..\..\multi_converter_common\memory_map.h"
#include  	"..\..\multi_converter_common\WorkingRam.h"
#include  	"..\..\multi_converter_common\My_Util.h"
#include  	"..\..\multi_converter_common\multi_converter.h"

#include	"req_485_232_ack_can.h"




unsigned    char    CanLiveTimeAdr[MAX_ELEV];


/*
void    CanLiveChk(void)
{
    unsigned char id,i;
    unsigned int  IdPt;

	C1EC=0;

//    TXERRCNT=0;
//    RXERRCNT=0;

    for(id=0;id<MAX_ELEV;id++){
        if(CanLiveTimeAdr[id] > 120){    
            IdPt=(id * HOST_DATA_RECOD);
            for(i=0;i<HOST_DATA_RECOD;i++)  RcvBuf[IdPt+i]=0x0;  
            CanLiveTimeAdr[id]=0;
        }
    }

	if(Can1RxLiveCnt > 100){
		Can1Init();
		Can1RxLiveCnt=0;
	}
}
*/



unsigned int   __attribute__((section(".usercode"))) Can1RxDataLoadTmpbuf(unsigned char *buf)
{
	*(buf+0)=(unsigned char)C1RX0B1;		
	*(buf+1)=(unsigned char)(C1RX0B1 >> 8);		
	*(buf+2)=(unsigned char)C1RX0B2;		
	*(buf+3)=(unsigned char)(C1RX0B2 >> 8);		
	*(buf+4)=(unsigned char)C1RX0B3;		
	*(buf+5)=(unsigned char)(C1RX0B3 >> 8);		
	*(buf+6)=(unsigned char)C1RX0B4;		
	*(buf+7)=(unsigned char)(C1RX0B4 >> 8);
		
	return(0);	
}


unsigned char __attribute__((section(".usercode")))	HogiSelect(void)
{
	unsigned char i;
	for(i=0;i<MAX_ELEV;i++){
		if(Com1RxBuffer[0] == ReqHost[i].Ho_Host[0]){
			return(i);
		}
	}
	return(0xff);	
}


unsigned int	RxCan1AndTxCom1(void)
{  
	unsigned char 	i;
	unsigned char 	tmpGroup,tmpLocal;
	unsigned int	idptx;

	#ifdef	I_AM_HOST_CALL
		i=HogiSelect();
		if(i >= 0xff)	return(0);
		idptx=(unsigned int)(i * HOST_DATA_RECOD);
	#else
		i=0;
		idptx=0;
	#endif

	if(ReqHost[i].Ho_Host[5]==0)	return(0);
	else							ReqHost[i].Ho_Host[5]=0;
   

 	tmpGroup= (unsigned char)(Can1RxSid >> 4);
	if(Can1RxEid & 0x20000)	tmpGroup= (tmpGroup | 0x80);	
 	tmpGroup= (unsigned char)(tmpGroup & 0xf8);

 	tmpLocal= 0;
	if(Can1RxEid & 0x4000)	tmpLocal= (tmpLocal | 0x01);	
	if(Can1RxEid & 0x8000)	tmpLocal= (tmpLocal | 0x02);	
	if(Can1RxEid & 0x10000)	tmpLocal= (tmpLocal | 0x04);	


#ifdef	I_AM_485_232_HOST
    Com1TxBuffer[0]  = 0xfe;   
    Com1TxBuffer[1]  = (tmpGroup | tmpLocal);	
    Com1TxBuffer[2]  = 0xA3;                                        
	Com1TxBuffer[3]  = CRT_TX_DATA_TO_PC;        //HOST_DATA_RECOD;	
#else
	#ifndef	I_AM_HOST_CALL
    	if(Com1RxBuffer[0]  != (tmpGroup | tmpLocal))	return(0);	
	#endif

    Com1TxBuffer[0]  = 0xfe;   
    Com1TxBuffer[1]  = Com1RxBuffer[0];	
    Com1TxBuffer[2]  = Com1RxBuffer[2] + 0x80;                                        
	Com1TxBuffer[3]  = CRT_TX_DATA_TO_PC;
        
    Com1RxBuffer[0]  = 0xff;   
#endif

	Com1TxCnt=0;
	Com1TxThisPt=0;

	
    for(i=0;(i < Com1TxBuffer[3]);i++){
		Com1TxBuffer[4+i]=RcvBuf[idptx + i + RCV_DATA];
    }


	bCan1RxAll=0;

	Com1TxCnt = (4 + Com1TxBuffer[3]);

    Crc=0xffff;
    for(i=0;i<Com1TxBuffer[3]+4;i++){
        Crc_Calulate((unsigned int)Com1TxBuffer[i]);
    }
    Com1TxBuffer[i]=(unsigned char)(Crc & 0x00ff);
    Com1TxBuffer[i+1]=(unsigned char)((Crc >> 8) & 0x00ff);

	Com1TxCnt = (Com1TxCnt + 2);


	Com2TxCnt=Com1TxCnt;
	Com2TxThisPt=0;
    for(i=0;i<Com1TxBuffer[3]+6;i++){
        Com2TxBuffer[i]=Com1TxBuffer[i];
    }

	Com1TxStart();
	Com2TxStart();
}



unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j,AckHost;
	unsigned int	hostnm;
	unsigned char	tmpbuf[10];


	Can1RxLiveCnt=0;
		
	if(Can1RxEid & I_AM_HOST){                                 //host board                 
		bCan1RxGood=1;                    
		bCan1RxAll=0;                    
		Can1PollingTimer=0;
		hostnm = 0;



#ifdef	I_AM_HOST_CALL
		for(AckHost=0;AckHost<MAX_ELEV;AckHost++){
			if( ( (ReqHost[AckHost].HostSid == Can1RxSid)  && ( (ReqHost[AckHost].HostEid & 0xffffff00) == (Can1RxEid & 0xffffff00) ))){
        		hostnm = (unsigned int)(AckHost * HOST_DATA_RECOD);
            	HostElevLive[AckHost]=0;
				break;				
			}  
		}
#else
		AckHost=0;
   		hostnm = (unsigned int)(AckHost * HOST_DATA_RECOD);
       	HostElevLive[AckHost]=0;
#endif



		if((Can1RxEid & HOST_LAST_DATA)){
			bCan1RxAll=1;
			hostnm = hostnm + 32; 
		}
		else{
			j=(unsigned char)(Can1RxEid & 0x0f);	
			if(j==0){
				Can1RxCnt=0;
				Can1RxThisPt=0;
				Can1TxCnt=0;
				Can1TxThisPt=0;
				BefCan1RxEid = (Can1RxEid & 0xffffff00);
				BefCan1RxSid = Can1RxSid;
			}
			hostnm = hostnm + (j * 8); 
		}
		Can1RxDataLoadTmpbuf(tmpbuf);



		if( (BefCan1RxEid != (Can1RxEid & 0xffffff00)) || (BefCan1RxSid != Can1RxSid)){
			bCan1RxGood=0;                    
			bCan1RxAll=0;                    
			return(0);
		}



		for(j=0;j<Can1RxDlc;j++){
   			Can1RxBuf[0    +j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[hostnm  +j+RCV_DATA]=tmpbuf[j];
		}

		if(bCan1RxAll){
			ReqHost[AckHost].Ho_Host[5]=1;
		}		


		if(bCan1RxAll){		
			Can1RxCnt=(Can1RxCnt + Can1RxDlc);
			Can1RxThisPt=Can1RxCnt;
		}
	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}

/*
unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j,AckHost;
	unsigned int	hostnm;
	unsigned char	tmpbuf[10];


	Can1RxLiveCnt=0;
		
	if(Can1RxEid & I_AM_HOST){                                 //host board                 
		bCan1RxGood=1;                    
		bCan1RxAll=0;                    
		Can1PollingTimer=0;
		hostnm = 0;

		if((Can1RxEid & HOST_LAST_DATA)){
			bCan1RxAll=1;
			hostnm = hostnm + 32; 
		}
		else{
			j=(unsigned char)(Can1RxEid & 0x0f);	
			if(j==0){
				Can1RxCnt=0;
				Can1RxThisPt=0;
				Can1TxCnt=0;
				Can1TxThisPt=0;
				BefCan1RxEid = (Can1RxEid & 0xffffff00);
				BefCan1RxSid = Can1RxSid;
			}
			hostnm = hostnm + (j * 8); 
		}
		Can1RxDataLoadTmpbuf(tmpbuf);



		if( (BefCan1RxEid != (Can1RxEid & 0xffffff00)) || (BefCan1RxSid != Can1RxSid)){
			bCan1RxGood=0;                    
			bCan1RxAll=0;                    
			return(0);
		}


		for(j=0;j<Can1RxDlc;j++){
   			Can1RxBuf[0    +j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[hostnm  +j+RCV_DATA]=tmpbuf[j];
		}

		if(bCan1RxAll){		
			Can1RxCnt=(Can1RxCnt + Can1RxDlc);
			Can1RxThisPt=Can1RxCnt;
		}
	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}

*/





