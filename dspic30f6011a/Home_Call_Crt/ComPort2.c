


#include <p30f6011a.h>   
#include  "30f6010_io.h"            
#include  "host_io.h"            
#include  "WorkingRam.h"
#include  "ComPort2.h"


unsigned char   Com2RxBuffer[MAX_RTX_BUF]={0,0};
unsigned char   Com2RxStatus=0;
unsigned char   Com2TxOn=0;
unsigned int    Com2SerialTime=0x0;
unsigned char   Com2AckTime=0;
unsigned char   Com2Chksum=0;

unsigned char   Com2RxCnt=0;
unsigned char   Com2RxThisPt=0;
unsigned char   Com2TxCnt=0;
unsigned char   Com2TxThisPt=0;
unsigned char   TXEN485Timer=0;



unsigned char   TestBuffer[MAX_RTX_BUF]={0,0};
unsigned char   TestCnt=0;


void  __attribute__((section(".usercode")))   Com2Init(void)
{
	TRISFbits.TRISF4=1;  //uart rx
	TRISFbits.TRISF5=0;  //uart tx

//    U2BRG=17;               //56000
//    U2BRG=26;               //38400
//    U2BRG=52;               //19200
//    U2BRG=104;              //9600
/*
	if( (iF_Com2Baudrate == 17) || (iF_Com2Baudrate == 26) || (iF_Com2Baudrate == 52) || (iF_Com2Baudrate == 104) ){   
    	U2BRG=(unsigned int)iF_Com2Baudrate;			
	}
	else{
    	U2BRG=(unsigned int)(26);			
	}
*/
    	
	U2BRG=(unsigned int)iF_Com2Baudrate;			

    U2STA=0x0;

    _U2RXIP=4;
    _U2TXIP=4;

    _U2PDSEL=0;

    _U2STSEL  = 0;

    _U2ABAUD  = 0;
    _U2LPBACK = 0;
    _U2WAKE   = 0;
    _U2ALTIO  = 0;
    _U2USIDL  = 0;
    _U2UARTEN = 1;

    _U2TXEN=1;
    _U2TXISEL=1;

    _U2TXIF=0;
    _U2RXIF=0;

    _U2RXIE=1;
    _U2TXIE=1;

}






void      __attribute__((section(".usercode"))) Com2TxStart(void)
{
//	unsigned int i;

//	for(i=0;i<150;i++);

   	Com2RxStatus=TX_SET;
	TXEN_485=1;
    U2TXREG=Com2RxBuffer[Com2TxThisPt];
   	Com2TxThisPt++;   
	TXEN485Timer=0;
}



void _ISR _U2TXInterrupt(void)
{

    _U2TXIF=0;
    
	Com2SerialTime=0;

    if(Com2TxThisPt >=  Com2TxCnt){
		TXEN485Timer=0;
		Com2TxCnt=0;
		Com2TxThisPt=0;	
    }
    else{
      	U2TXREG=Com2RxBuffer[Com2TxThisPt];
      	Com2TxThisPt++;
      	Com2RxStatus = TX_SET;
		TXEN485Timer=0;
    }

}








void _ISR _U2RXInterrupt(void)
{
   	unsigned char   buf1=0;


    _U2RXIF=0;

   	if(_U2RXDA)    buf1=U2RXREG;
   	if(_U2RXDA)    buf1=U2RXREG;
   	if(_U2RXDA)    buf1=U2RXREG;
   	if(_U2RXDA)    buf1=U2RXREG;
   	if(_U2RXDA)    buf1=U2RXREG;
   	if(_U2RXDA)    buf1=U2RXREG;

    Com2SerialTime=0;

    if(_U2OERR){
        _U2OERR=0;
    }

    if(_U2FERR){
        _U2FERR=0;
    }

    if(_U2PERR){
        _U2PERR=0;
    }

    if(Com2RxStatus != TX_SET){
		Com2ReceiveData(buf1);
	}
}


