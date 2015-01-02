// 멀티컨버터 
// 2015.01.02 : MyCall 제어 요청시에 리턴 값 오도록 수정 

#include <p30f6011a.h>            


#include  	"..\multi_converter_common\host_io.h"            
#include  	"..\multi_converter_common\30f6010_io.h"
#include  	"..\multi_converter_common\you_can1.h"
#include  	"..\multi_converter_common\you_can2.h"
#include  	"..\multi_converter_common\ComPort1.h"
#include  	"..\multi_converter_common\ComPort2.h"
#include  	"..\multi_converter_common\memory_map.h"
#include  	"..\multi_converter_common\WorkingRam.h"
#include  	"..\multi_converter_common\Multi_converter.h"

#include	"myCall_crt.h"



unsigned 	char   Com1TxBuffer[MAX_COM1_TXBUF];
unsigned 	char   Com1RxBuffer[MAX_COM1_RXBUF];

unsigned	char   Com2TxBuffer[MAX_COM2_TXBUF];
unsigned 	char   Com2RxBuffer[MAX_COM2_RXBUF];
			
unsigned	char    Can1TxBuf[CAN1_MAX_TX_BUF];
unsigned    char    Can1RxBuf[CAN1_MAX_RX_BUF];

unsigned    char    Can2TxBuf[CAN2_MAX_TX_BUF];
unsigned    char    Can2RxBuf[CAN2_MAX_RX_BUF];


unsigned    char    RcvBuf[MAX_SAVE_BUF];
unsigned	char  	HostElevLive[MAX_ELEV];

volatile tagReqHost ReqHost[MAX_ELEV];


unsigned	char  	ThisHostSelect=0;
unsigned	char  	AckWrCmd=0;

unsigned	char	bWrCmd = 0;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int   __attribute__((section(".usercode"))) My_Project_Start(void)
{
#ifdef	I_AM_CALL_HOST
	if(Com1RxStatus==RX_GOOD){
		if(CrtCom1Check()){
			Com1RxStatus= STX_CHK;
		}
	}


	if(ThisHostSelect >= MAX_ELEV){
		ThisHostSelect=0;
	}

	if( !PcCmdWr()){
		if( !Normal_Read(ThisHostSelect)){
			ThisHostSelect++;
			bWrCmd = 0;
		}
	}
#else
	if(Com1RxStatus==RX_GOOD){
		if(CrtCom1Check()){
			Com1RxStatus= STX_CHK;
			bCan1RxAll=0;
		}
	}

	if(bCan1RxAll){		
		BufTxCom1(Com1RxBuffer[0],0);
	}

#endif

	return(0);
}







void  __attribute__((section(".usercode")))   Filter_Mask_Set(void)
{

	Can1RxSidFilter=CRT_COMPANY;
	Can1RxEidFilter=0;
	Can1RxSidMask=0x007f;
	Can1RxEidMask=0x0000;


	Can2RxSidFilter=CRT_COMPANY;
	Can2RxEidFilter=0;
	Can2RxSidMask=0x007f;
	Can2RxEidMask=0x0000;

	Can1RxFilterMaskSet(0);
	Can1RxFilterMaskSet(1);
	Can1RxFilterMaskSet(2);
	Can1RxFilterMaskSet(3);
	Can1RxFilterMaskSet(4);
	Can1RxFilterMaskSet(5);


	Can2RxFilterMaskSet(0);
	Can2RxFilterMaskSet(1);
	Can2RxFilterMaskSet(2);
	Can2RxFilterMaskSet(3);
	Can2RxFilterMaskSet(4);
	Can2RxFilterMaskSet(5);


}




void    GetTxSidEid(unsigned char tmpCompany,unsigned char tmpGroup,unsigned char tmpLocal)
{
    unsigned int  itmp1,itmp2;
    unsigned long ltmp1;

    itmp1=(unsigned int)(tmpCompany);
    itmp2=(unsigned int)(tmpGroup);
    itmp2=(itmp2 << 7);
    Can1TxSid=(unsigned int)(itmp1 | itmp2);


	Can1TxEid=(unsigned long)(tmpLocal);
	Can1TxEid = (Can1TxEid << 14);
	if(tmpGroup & 0x20) 
	Can1TxEid = (Can1TxEid | 0x200000);

	Can1TxEid = (Can1TxEid | 0x2000);
}



unsigned int   __attribute__((section(".usercode"))) My_Project_Init(void)
{
	unsigned int i;

	unsigned	char	tmpCompany,tmpGroupadr,tmpLocaladr;

 

    Com1Init(COM1_BAUD);
    Com2Init(COM1_BAUD);

	Filter_Mask_Set();

	for(i=0;i<(CAN1_MAX_RX_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_TX_BUF-1);i++)	Can1TxBuf[i]=0;

	Com1TxThisPt =0;
	Com1TxCnt=0;


	for(i=0;i<MAX_ELEV;i++)	HostElevLive[i]=100;

	for(i=0;i<MAX_ELEV;i++){
		tmpCompany=CRT_COMPANY;
		tmpGroupadr=i;
		tmpLocaladr=0;    
		GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
		if(Can1TxSid & 0x800){
			Can1TxSid= (Can1TxSid & 0x7ff);
			Can1TxEid=(Can1TxEid | 0x20000);
		}	

		ReqHost[i].HostSid=Can1TxSid;
		ReqHost[i].HostEid=Can1TxEid;
		ReqHost[i].HostGroup=i;
		ReqHost[i].Ho_Host[0]=(i * 8);  // address
		ReqHost[i].Ho_Host[1]=i;        // sequence  넘버
		ReqHost[i].Ho_Host[3]=0xff;
		ReqHost[i].Ho_Host[4]=0xff;
		ReqHost[i].Ho_Host[5]=0x0;
	}




//4001 동
///////////////////////////////////////////////////
	i=32;
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=0;
	tmpLocaladr=1;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
	if(Can1TxSid & 0x800){
		Can1TxSid= (Can1TxSid & 0x7ff);
		Can1TxEid=(Can1TxEid | 0x20000);
	}	

	ReqHost[i].HostSid=Can1TxSid;
	ReqHost[i].HostEid=Can1TxEid;
	ReqHost[i].HostGroup=0;
	ReqHost[i].Ho_Host[0]=1;  // address
	ReqHost[i].Ho_Host[1]=i;  // // sequence  넘버 
	ReqHost[i].Ho_Host[3]=0xff;
	ReqHost[i].Ho_Host[4]=0xff;
	ReqHost[i].Ho_Host[5]=0x0;

///////////////////////////////////////////////////
	i=33;
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=0;
	tmpLocaladr=2;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
	if(Can1TxSid & 0x800){
		Can1TxSid= (Can1TxSid & 0x7ff);
		Can1TxEid=(Can1TxEid | 0x20000);
	}	

	ReqHost[i].HostSid=Can1TxSid;
	ReqHost[i].HostEid=Can1TxEid;
	ReqHost[i].HostGroup=0;
	ReqHost[i].Ho_Host[0]=2;  // address
	ReqHost[i].Ho_Host[1]=i;  // // sequence  넘버 
	ReqHost[i].Ho_Host[3]=0xff;
	ReqHost[i].Ho_Host[4]=0xff;
	ReqHost[i].Ho_Host[5]=0x0;


#ifdef	BLOCK_4000
///////////////////////////////////////////////////
	i=34;
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=1;
	tmpLocaladr=1;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
	if(Can1TxSid & 0x800){
		Can1TxSid= (Can1TxSid & 0x7ff);
		Can1TxEid=(Can1TxEid | 0x20000);
	}	

	ReqHost[i].HostSid=Can1TxSid;
	ReqHost[i].HostEid=Can1TxEid;
	ReqHost[i].HostGroup=0;
	ReqHost[i].Ho_Host[0]=9;  // address
	ReqHost[i].Ho_Host[1]=i;  // // sequence  넘버 
	ReqHost[i].Ho_Host[3]=0xff;
	ReqHost[i].Ho_Host[4]=0xff;
	ReqHost[i].Ho_Host[5]=0x0;


///////////////////////////////////////////////////
	i=35;
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=1;
	tmpLocaladr=2;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
	if(Can1TxSid & 0x800){
		Can1TxSid= (Can1TxSid & 0x7ff);
		Can1TxEid=(Can1TxEid | 0x20000);
	}	

	ReqHost[i].HostSid=Can1TxSid;
	ReqHost[i].HostEid=Can1TxEid;
	ReqHost[i].HostGroup=0;
	ReqHost[i].Ho_Host[0]=10;  // address
	ReqHost[i].Ho_Host[1]=i;  // // sequence  넘버 
	ReqHost[i].Ho_Host[3]=0xff;
	ReqHost[i].Ho_Host[4]=0xff;
	ReqHost[i].Ho_Host[5]=0x0;
#endif

	return(0);	
}



unsigned char __attribute__((section(".usercode"))) msecTimer1(void)
{
	return(0);
}


unsigned char __attribute__((section(".usercode"))) MsecTimer_100(void)
{
	return(0);
}


unsigned char __attribute__((section(".usercode"))) SecTimer_1(void)
{

	unsigned char i,j;

	for(i=0;i<MAX_ELEV;i++){          
		if(HostElevLive[i] < 100)	HostElevLive[i]++;
	}
	
/*
	if(ElevatorServerAliveTime < 250)	ElevatorServerAliveTime++;
	
	for(i=0;i<32;i++){ 
		for(j=1;j<7;j++){          
			if(LocationCallTime[i][j] < 250)	LocationCallTime[i][j]++;
		}
	}
*/
	return(0);
}

