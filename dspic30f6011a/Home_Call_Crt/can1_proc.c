
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

/////////#include        "xyou_can.h"
#include        "xsetup.h"
#include        "prj_app.h"
#include        "xesdots.h"



extern		void			CmdSetupChk(unsigned char id);
extern     	void         	MyLampCheck(void);
extern      unsigned char  	SyncButtonCheck(void);
extern      unsigned    char    CallMeAdr;
extern  	unsigned    char    LocalNumber;
//extern      unsigned    char    SrcAddress;


unsigned    char    RcvBuf[MAX_SAVE_BUF];
unsigned    char    CanKeyValue[8];
unsigned    char    CanCmd;
////////////////////////////////unsigned    char    Company;
unsigned    char    SelHostAdr;
////////////////unsigned    char    ReceiveAdr;
unsigned    char    CanLiveTimeAdr[MAX_ELEV];
unsigned    char    EqualDataBuf[8];


/*
//unsigned    char    CanReceiveCnt;
//unsigned    long    EidBuffer;
//unsigned    int     SidBuffer;
//unsigned    long    RxEidBuffer;
//unsigned    int     RxSidBuffer;
*/


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



void    TmpCan1TxSidEidLoad(void)
{
    unsigned int  itmp1,itmp2;
    unsigned long ltmp1;

    itmp1=(unsigned int)(HOMECALL_COMPANY);
    itmp2=(unsigned int)GROUP_NUMBER;
    itmp2=(itmp2 << 7);
    Can1TxSid=(unsigned int)(itmp1 | itmp2);


	Can1TxEid=(unsigned long)CommonBuf[0];
	Can1TxEid = (0x07 & Can1TxEid); 
	Can1TxEid = (Can1TxEid << 14); 
	Can1TxEid = (Can1TxEid | HOMECALL_PC_ADDRESS); 

	Can1TxDlc=8;
}


void    Can1TxSidEidLoad(void)
{
    unsigned int  itmp1,itmp2;
    unsigned long ltmp1;

    itmp1=(unsigned int)(HOMECALL_COMPANY);
    itmp2=(unsigned int)GROUP_NUMBER;
    itmp2=(itmp2 << 7);
    Can1TxSid=(unsigned int)(itmp1 | itmp2);

	ltmp1= 0;
    ltmp1= (ltmp1 | (unsigned long)SelHostAdr);
    ltmp1= (ltmp1 << 14);
    ltmp1= (ltmp1 | (unsigned long)MyAddress);

    Can1TxEid=ltmp1;

	Can1TxDlc=8;
}



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


unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j,hostnm,i;
	unsigned char	tmpbuf[10];
	unsigned long	ltmp;


	Can1RxLiveCnt=0;
		
	if(Can1RxEid & I_AM_HOST){                                 //host board                 
		bCan1RxGood=1;                    
		bCan1RxAll=0;                    

    	ltmp=((Can1RxEid & NEW_HOST_ADDRESS) >> 14);
	    Can1RxLocalAddr=(unsigned char)(ltmp & 0x07);

		if((Can1RxEid & HOST_LAST_DATA)){
			j=4;
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
		
		j=(unsigned char)(j * 8);
        hostnm = (unsigned char)(Can1RxLocalAddr * HOST_DATA_RECOD);
        hostnm = (unsigned char)(hostnm + j);

		Can1RxDataLoadTmpbuf(tmpbuf);

		for(i=0;i<Can1RxDlc;i++){
//   			Can1RxBuf[hostnm+i+RCV_DATA+Can1RxCnt]=tmpbuf[i];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
//			RcvBuf[hostnm   +i+RCV_DATA+Can1RxCnt]=tmpbuf[i];
   			Can1RxBuf[hostnm+i+RCV_DATA]=tmpbuf[i];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			RcvBuf[hostnm   +i+RCV_DATA]=tmpbuf[i];
            HostElevLive[Can1RxLocalAddr]=0;				
		}
		
//		Can1RxCnt=(Can1RxCnt + Can1RxDlc);
		Can1RxCnt=(j + Can1RxDlc);
		Can1RxThisPt=Can1RxCnt;

		if(bCan1RxAll){
			CanLiveTimeAdr[Can1RxLocalAddr]=0x0;
			RxHostData(1);
		}
	}
	else{
        bCan1RxAll=0;
	}

	return(0);
}
