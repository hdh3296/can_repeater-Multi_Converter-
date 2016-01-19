
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

#include	"LH_HYUNDAI_PROTOCOL.h"



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
unsigned    char    CommonPcCmd_Buf[20];  


volatile tagReqHost ReqHost[MAX_ELEV];


unsigned 	char 	ThisHostSelect=0; 

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int   __attribute__((section(".usercode"))) My_Project_Start(void)
{

	if(Com1RxStatus==RX_GOOD){
		if(CrtCom1Check()){
			Com1RxStatus= STX_CHK;
		}
	}


	if(ThisHostSelect >= MAX_ELEV){
			ThisHostSelect=0;
	}

	if(I_AM_LISTENING_MODE == 0){
		if( !LH_Protocol_PcCmd()){
			if(Can1HostDataReq(ThisHostSelect)){
				ThisHostSelect++;
			}
		}
	}	


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


// ----> 현대 엘리베이터 ID 설정 <----//
unsigned int   __attribute__((section(".usercode"))) My_Project_Init(void)
{
	unsigned int i;

	unsigned	char	tmpCompany,tmpGroupadr,tmpLocaladr;

 

    Com1Init(BAUD_RATE_19200);
    Com2Init(BAUD_RATE_19200);

//    Com1Init(BAUD_RATE_38400);
//    Com2Init(BAUD_RATE_38400);

	Filter_Mask_Set();

	for(i=0;i<(CAN1_MAX_RX_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_TX_BUF-1);i++)	Can1TxBuf[i]=0;

	Com1TxThisPt =0;
	Com1TxCnt=0;


	for(i=0;i<MAX_ELEV;i++)	HostElevLive[i]=100;


///////////////////////////////////////////////
/// CRT_COMPANY
/// host id 		= tmpGroupadr
/// Hyundai host id	= Ho_Host[0]


	tmpCompany=CRT_COMPANY;
	tmpGroupadr=0;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
		
	ReqHost[0].HostSid=Can1TxSid;
	ReqHost[0].HostEid=Can1TxEid;
	ReqHost[0].HostGroup=tmpGroupadr;
	ReqHost[0].Ho_Host[0]=6;       // hyundai  call id
	ReqHost[0].Ho_Host[1]=0xff;
	ReqHost[0].Ho_Host[3]=0xff;
	ReqHost[0].Ho_Host[4]=0xff;
	ReqHost[0].HostB=0;
//////////////////////////////////////////////////

	tmpCompany=CRT_COMPANY;
	tmpGroupadr=1;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[1].HostSid=Can1TxSid;
	ReqHost[1].HostEid=Can1TxEid;
	ReqHost[1].HostGroup=tmpGroupadr;
	ReqHost[1].Ho_Host[0]=7;		// hyundai  call id
	ReqHost[1].Ho_Host[1]=0xff;
	ReqHost[1].Ho_Host[3]=0xff;
	ReqHost[1].Ho_Host[4]=0xff;
	ReqHost[0].HostB=1;


//////////////////////////////////////////////////

	tmpCompany=CRT_COMPANY;
	tmpGroupadr=2;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[2].HostSid=Can1TxSid;
	ReqHost[2].HostEid=Can1TxEid;
	ReqHost[2].HostGroup=tmpGroupadr;
	ReqHost[2].Ho_Host[0]=8;		// hyundai  call id
	ReqHost[2].Ho_Host[1]=0xff;
	ReqHost[2].Ho_Host[3]=0xff;
	ReqHost[2].Ho_Host[4]=0xff;
	ReqHost[0].HostB=2;

//////////////////////////////////////////////////

	tmpCompany=CRT_COMPANY;
	tmpGroupadr=3;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[3].HostSid=Can1TxSid;
	ReqHost[3].HostEid=Can1TxEid;
	ReqHost[3].HostGroup=tmpGroupadr;
	ReqHost[3].Ho_Host[0]=9;		// hyundai  call id
	ReqHost[3].Ho_Host[1]=0xff;
	ReqHost[3].Ho_Host[3]=0xff;
	ReqHost[3].Ho_Host[4]=0xff;
	ReqHost[0].HostB=3;





/*
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=4;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[4].HostSid=Can1TxSid;
	ReqHost[4].HostEid=Can1TxEid;
	ReqHost[4].HostGroup=4;
	ReqHost[4].Ho_Host[0]=9;
	ReqHost[4].Ho_Host[1]=12;
	ReqHost[4].Ho_Host[3]=0xff;
	ReqHost[4].Ho_Host[4]=0xff;



	tmpCompany=CRT_COMPANY;
	tmpGroupadr=4;
	tmpLocaladr=1;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[5].HostSid=Can1TxSid;
	ReqHost[5].HostEid=Can1TxEid;
	ReqHost[5].HostGroup=4;
	ReqHost[5].Ho_Host[0]=9;
	ReqHost[5].Ho_Host[1]=12;
	ReqHost[5].Ho_Host[3]=0xff;
	ReqHost[5].Ho_Host[4]=0xff;
*/

	return(0);	
}



unsigned char __attribute__((section(".usercode"))) msecTimer1(void)
{
	return(0);
}


unsigned char __attribute__((section(".usercode"))) MsecTimer_100(void)
{
//	if(StatusTxTime < 100)			StatusTxTime++;
	return(0);
}




unsigned char __attribute__((section(".usercode"))) SecTimer_1(void)
{

	unsigned char i,j;

	for(i=0;i<MAX_ELEV;i++){          
		if(HostElevLive[i] < 100)	HostElevLive[i]++;
	}
	
	LH_SecTimer();

	return(0);
}

