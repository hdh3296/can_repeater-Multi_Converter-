


#include <p30f6011a.h>            
#include  "..\header\30f6010_io.h"            
#include  "..\working_ram\WorkingRam.h"
//#include  "host_io.h"            
//#include  "iodef.h"
#include  "ComPort1.h"
#include  "ComPort2.h"



unsigned char   Com1RxBuffer[MAX_RTX_BUF]={0,0};
unsigned char   Com1RxStatus=0;
unsigned char   Com1TxOn=0;
unsigned char   Com1SerialTime=0x0;
unsigned char   Com1AckTime=0;
unsigned char   Com1Chksum=0;

unsigned char   Com1RxCnt=0;
unsigned char   Com1RxThisPt=0;
unsigned char   Com1TxCnt=0;
unsigned char   Com1TxThisPt=0;



void  __attribute__((section(".usercode")))   Com1Init(void)
{
	
	TRISFbits.TRISF2=1;  //uart rx
	TRISFbits.TRISF3=0;  //uart tx

//    U1BRG=104;		//9600
//    U1BRG=52;			//19200
//    U1BRG=26;			//38400
//    U1BRG=17;			//56000


	if( (iF_Com1Baudrate == 17) || (iF_Com1Baudrate == 26) || (iF_Com1Baudrate == 52) || (iF_Com1Baudrate == 104) ){   
    	U1BRG=(unsigned int)iF_Com1Baudrate;			
	}
	else{
    	U1BRG=(unsigned int)(26);			
	}


    U1STA=0x0;

    _U1RXIP=4;
    _U1TXIP=4;

    _U1PDSEL=0;

    _U1STSEL  = 0;

    _U1ABAUD  = 0;
    _U1LPBACK = 0;
    _U1WAKE   = 0;
    _U1ALTIO  = 0;
    _U1USIDL  = 0;
    _U1UARTEN = 1;

    _U1TXEN=1;
    _U1TXISEL=1;

    _U1TXIF=0;
    _U1RXIF=0;

    _U1RXIE=1;
    _U1TXIE=1;

}








void      __attribute__((section(".usercode"))) Com1TxStart(void)
{
/*	
   	Com1RxCnt=0;
   	Com1RxStatus=TX_SET;
    U1TXREG=Com1RxBuffer[Com1RxCnt];
   	Com1RxCnt=1;   
*/


   	Com1RxStatus=TX_SET;
    U1TXREG=Com1RxBuffer[Com1TxThisPt];
//   	Com1TxThisPt=1;   
   	Com1TxThisPt++;   
}





void _ISR _U1TXInterrupt(void)
{

    _U1TXIF=0;

	Com1SerialTime=0;
 
   if(Com1TxThisPt >= Com1TxCnt){
		Com1TxCnt=0;
		Com1TxThisPt=0;	
      	Com1RxStatus = STX_CHK;
    }
    else{
      	U1TXREG=Com1RxBuffer[Com1TxThisPt];
      	Com1TxThisPt++;
      	Com1RxStatus = TX_SET;
    }
}








void _ISR _U1RXInterrupt(void)
{
   	unsigned char   buf1=0;

    _U1RXIF=0;    

   	if(_U1RXDA)    buf1=U1RXREG;
   	if(_U1RXDA)    buf1=U1RXREG;
   	if(_U1RXDA)    buf1=U1RXREG;
   	if(_U1RXDA)    buf1=U1RXREG;
   	if(_U1RXDA)    buf1=U1RXREG;
   	if(_U1RXDA)    buf1=U1RXREG;


    Com1SerialTime=0;

    if(_U1OERR){
        _U1OERR=0;
    }

    if(_U1FERR){
        _U1FERR=0;
    }

    if(_U1PERR){
        _U1PERR=0;
    }

	if(Com1RxStatus != TX_SET){
		Com1ReceiveData(buf1);
	}
	
}


