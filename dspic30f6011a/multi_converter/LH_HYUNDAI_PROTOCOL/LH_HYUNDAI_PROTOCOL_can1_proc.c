
#include <p30f6011a.h>            

#include  	"..\multi_converter_common\host_io.h"            
#include  	"..\multi_converter_common\30f6010_io.h"
#include  	"..\multi_converter_common\you_can1.h"
#include  	"..\multi_converter_common\you_can2.h"
#include  	"..\multi_converter_common\ComPort1.h"
#include  	"..\multi_converter_common\ComPort2.h"
#include  	"..\multi_converter_common\memory_map.h"
#include  	"..\multi_converter_common\WorkingRam.h"
#include  	"..\multi_converter_common\My_Util.h"
#include	"LH_HYUNDAI_PROTOCOL.h"




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



unsigned int   __attribute__((section(".usercode"))) ThisAckHostSel(unsigned int mode)
{
	unsigned int i,j,ThisHostDataPt=0xffff;


	if(I_AM_LISTENING_MODE == 0){
		j=0;
		ThisHostDataPt=0;		
	}
	else{


		for(i=0;i<MAX_ELEV;i++){
			if( ( (ReqHost[i].HostSid == Can1RxSid)  && ( (ReqHost[i].HostEid & 0xffffff00) == (Can1RxEid & 0xffffff00) ))){
				j=i;
        		ThisHostDataPt = (unsigned int)(i * HOST_DATA_RECOD);
				i=MAX_ELEV;		
			}  
		}
	}		



	if(mode == 0){
		HostElevLive[j]=0;
	}
	else{
		if(HostElevLive[j] > HOST_LIVE_TIME){
			ThisHostDataPt=0xffff;		
		}		
	}

	return(ThisHostDataPt);				
}




unsigned int	RxCan1AndTxCom1(void)
{  
	unsigned int ThisHostDataPt;
	unsigned char i;
	unsigned tmpGroup,tmpLocal;
   
 	tmpGroup= (unsigned char)(Can1RxSid >> 4);
	if(Can1RxEid & 0x20000)	tmpGroup= (tmpGroup | 0x80);	
 	tmpGroup= (unsigned char)(tmpGroup & 0xf8);

 	tmpLocal= 0;
	if(Can1RxEid & 0x4000)	tmpLocal= (tmpLocal | 0x01);	
	if(Can1RxEid & 0x8000)	tmpLocal= (tmpLocal | 0x02);	
	if(Can1RxEid & 0x10000)	tmpLocal= (tmpLocal | 0x04);	




	if(Com1RxBuffer[0] != (tmpGroup | tmpLocal)){
		bCan1RxAll=0;
		return(0);
	}



	if(Com1TxBuffer[2] == 0x24){
	    Com1TxBuffer[0]  = 0xfe;   
	    Com1TxBuffer[1]  = (tmpGroup | tmpLocal);
	
	    Com1TxBuffer[2]  = 0xA4;                                        
		Com1TxBuffer[3]  = 13;
	}		
	else{ 
	    Com1TxBuffer[0]  = 0xfe;   
	    Com1TxBuffer[1]  = (tmpGroup | tmpLocal);	
	    Com1TxBuffer[2]  = 0xA3;                                        
		Com1TxBuffer[3]  = CRT_TX_DATA_TO_PC;        //HOST_DATA_RECOD;

	}
	Com1TxCnt=0;
	Com1TxThisPt=0;


	ThisHostDataPt=ThisAckHostSel(1);

    for(i=0;(i < Com1TxBuffer[3]);i++){
		Com1TxBuffer[4+i]=RcvBuf[ThisHostDataPt + i + RCV_DATA];
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

	Com1TxStart();


}




/*
unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j;
	unsigned char	tmpbuf[10];
	unsigned int 	ThisHostDataPt=0x0;


	Can1RxLiveCnt=0;
		
	if(Can1RxEid & I_AM_HOST){                                 //host board                 
		bCan1RxGood=1;                    
		bCan1RxAll=0;                    

		if((Can1RxEid & HOST_LAST_DATA)){
			bCan1RxAll=1;			
		}
		else{
			j=(unsigned char)(Can1RxEid & 0x0f);	
			if(j==0){
				Can1RxCnt=0;
				Can1RxThisPt=0;
				Can1TxCnt=0;
				Can1TxThisPt=0;
			}
		}
		Can1RxDataLoadTmpbuf(tmpbuf);

		ThisHostDataPt=ThisAckHostSel(0);

		if(ThisHostDataPt==0xffff){
			bCan1RxAll=1;			
			return(0);
		}

		for(j=0;j<Can1RxDlc;j++){
   			Can1RxBuf[0    			+j+RCV_DATA+Can1RxCnt]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[ThisHostDataPt  	+j+RCV_DATA+Can1RxCnt]=tmpbuf[j];
		}
		

		Can1RxCnt=(Can1RxCnt + Can1RxDlc);
		Can1RxThisPt=Can1RxCnt;

		if(bCan1RxAll){
			RxCan1AndTxCom1();
			bCan1RxAll=0;			
		}


	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}

*/




//////////////////

/*
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
*/



/*

unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j,hostnm,AckHost;
	unsigned char	tmpbuf[10];


	Can1RxLiveCnt=0;
		
	if(Can1RxEid & I_AM_HOST){                                 //host board                 
		bCan1RxGood=1;                    
		bCan1RxAll=0;                    
		Can1PollingTimer=0;



		if((Can1RxEid & HOST_LAST_DATA)){
			bCan1RxAll=1;
//			hostnm = hostnm + 32; 
		}
		else{
			j=(unsigned char)(Can1RxEid & 0x0f);	
			if(j==0){
				Can1RxCnt=0;
				Can1RxThisPt=0;
				Can1TxCnt=0;
				Can1TxThisPt=0;
//				Can1BefAdr=AckHost;
			}
//			hostnm = hostnm + (j * 8); 

		}
		Can1RxDataLoadTmpbuf(tmpbuf);

		for(AckHost=0;AckHost<MAX_ELEV;AckHost++){
			if( ( (ReqHost[AckHost].HostSid == Can1RxSid)  && ( (ReqHost[AckHost].HostEid & 0xffffff00) == (Can1RxEid & 0xffffff00) ))){
        		hostnm = (unsigned char)(AckHost * HOST_DATA_RECOD);
            	HostElevLive[AckHost]=0;
				break;				
			}  
		}

		for(j=0;j<Can1RxDlc;j++){
   			Can1RxBuf[0    +j+RCV_DATA+Can1RxCnt]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[hostnm  +j+RCV_DATA+Can1RxCnt]=tmpbuf[j];
//////////////			RcvBuf[hostnm  +j+RCV_DATA]=tmpbuf[j];
		}
		
		Can1RxCnt=(Can1RxCnt + Can1RxDlc);
		Can1RxThisPt=Can1RxCnt;

		if(bCan1RxAll)	NormalDataReload(AckHost);

	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}
*/



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


		for(AckHost=0;AckHost<MAX_ELEV;AckHost++){
			if( ( (ReqHost[AckHost].HostSid == Can1RxSid)  && ( (ReqHost[AckHost].HostEid & 0xffffff00) == (Can1RxEid & 0xffffff00) ))){
        		hostnm = (unsigned int)(AckHost * HOST_DATA_RECOD);
            	HostElevLive[AckHost]=0;
				break;				
			}  
		}


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
			}
			hostnm = hostnm + (j * 8); 

		}
		Can1RxDataLoadTmpbuf(tmpbuf);

		for(j=0;j<Can1RxDlc;j++){
   			Can1RxBuf[0    +j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[hostnm  +j+RCV_DATA]=tmpbuf[j];
		}
		
		Can1RxCnt=(Can1RxCnt + Can1RxDlc);
		Can1RxThisPt=Can1RxCnt;

		if(bCan1RxAll)	NormalDataReload(AckHost);

	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}



void    __attribute__((section(".usercode")))  Can1HostReqCmdLoad(void)
{
	Can1TxBuf[0]=0x23;
	Can1TxBuf[1]=0x02;
	Can1TxBuf[2]=0x00;
	Can1TxBuf[3]=0x00;
	Can1TxDlc=4;
}



void    __attribute__((section(".usercode")))  Can1UpDnButtonCmdLoad(void)
{
	Can1TxBuf[0]=0x24;
	Can1TxBuf[1]=0x04;
	Can1TxBuf[2]=0x87;
	Can1TxBuf[3]=0x00;
	Can1TxBuf[4]=0x00;
	Can1TxBuf[5]=0x00;
	Can1TxDlc=6;
}




/*
unsigned char  __attribute__((section(".usercode")))  Can1UpDnTxCheck(unsigned char This_Host_Sel,unsigned char This_button)
{
		
	if(Can1RxTimer >= 10){
	    if( (!C1TX0CONbits.TXREQ) && (Can1PollingTimer > 10) ){
			Can1TxSid=ReqHost[This_Host_Sel].HostSid;
			Can1TxEid=ReqHost[This_Host_Sel].HostEid;
			Can1UpDnButtonCmdLoad();
			Can1TxBuf[4]=This_button;			
			if( !Can1TxData(0,0)){
				return(1);
			}
		}
	}
	return(0);
}
*/

void    __attribute__((section(".usercode")))  Can1PcCmdLoad(void)
{
	Can1TxBuf[0]=CommonPcCmd_Buf[0];
	Can1TxBuf[1]=CommonPcCmd_Buf[1];
	Can1TxBuf[2]=CommonPcCmd_Buf[2];
	Can1TxBuf[3]=CommonPcCmd_Buf[3];
	Can1TxBuf[4]=CommonPcCmd_Buf[4];
	Can1TxBuf[5]=CommonPcCmd_Buf[5];
	Can1TxBuf[6]=CommonPcCmd_Buf[6];
	Can1TxBuf[7]=CommonPcCmd_Buf[7];
	Can1TxDlc=CommonPcCmd_Buf[1]+2;
}


unsigned char  __attribute__((section(".usercode")))  Can1PcCmdTx(unsigned char This_Host_Sel)
{
		

	if(Can1RxTimer >= 20){
	    if( (!C1TX0CONbits.TXREQ)  && (Can1PollingTimer > 10) ){
			Can1TxSid=ReqHost[This_Host_Sel].HostSid;
			Can1TxEid=ReqHost[This_Host_Sel].HostEid;
			Can1PcCmdLoad();
			if( !Can1TxData(0,0)){
				Can1RxTimer=0;
				return(1);
			}
		}
	}
	return(0);
}


unsigned char  __attribute__((section(".usercode")))  Can1HostDataReq(unsigned char This_Host_Sel)
{
		
	if(Can1RxTimer >= 50){
	    if( (!C1TX0CONbits.TXREQ)  && (Can1PollingTimer > 10) ){
			Can1TxSid=ReqHost[This_Host_Sel].HostSid;
			Can1TxEid=ReqHost[This_Host_Sel].HostEid;
			Can1HostReqCmdLoad();
			if( !Can1TxData(0,0)){
				Can1RxTimer=0;
				return(1);
			}
		}
	}
	return(0);
}





