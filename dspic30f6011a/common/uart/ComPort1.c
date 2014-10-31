


#include <p30f6011a.h>            
#include  "..\header\30f6010_io.h"            
//#include  "host_io.h"            
//#include  "iodef.h"
#include  "ComPort1.h"



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


/*
void     __attribute__((section(".usercode")))  Com1Chksum_Sum(void)
{
        unsigned char	i;
        unsigned char	temp;

        Com1Chksum=0;
        for(i=0;Com1RxBuffer[i];i++){
                Com1Chksum=Com1Chksum+Com1RxBuffer[i];
        }
        temp=(Com1Chksum & 0xf0) >> 4;
        if(temp < 0x0a) temp=temp+'0';
        else            temp=temp+'7';
        Com1RxBuffer[i]=temp;

        temp=(Com1Chksum & 0x0f);
        if(temp<0x0a)   temp=temp+'0';        
        else            temp=temp+'7';
        Com1RxBuffer[i+1]=temp;
        Com1RxBuffer[i+2]=0;
}

unsigned char	Com1SaveData(unsigned char ch)
{

	unsigned char dummy;

	dummy=ch;
    if(Com1RxStatus==RX_GOOD)	return(0);
    if(Com1RxStatus==TX_SET)	return(0);


	if(Com1RxCnt < (MAX_RTX_BUF-1)){
	   	Com1RxCnt++;
	}
	else{
	   	Com1RxCnt=0;
	}
			
	Com1RxBuffer[Com1RxCnt]=dummy;

	if(dummy == '>'){
        Com1RxCnt=0;
		Com1RxBuffer[Com1RxCnt]=dummy;
	}
	else if((dummy == '\n')){ 
		if((Com1RxBuffer[0] == '>') && (Com1RxBuffer[Com1RxCnt-1] == '\r')){
			Com1RxStatus=RX_GOOD;
		}     
	}
	else if(Com1RxBuffer[0] != '>'){
		Com1RxCnt=0;
	}

	return(0);
}
*/

void  __attribute__((section(".usercode")))   Com1Init(void)
{
	TRISFbits.TRISF2=1;  //uart rx
	TRISFbits.TRISF3=0;  //uart tx

//    U1BRG=104;			//9600
//    U1BRG=52;			//19200
    U1BRG=26;			//38400
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


//	Com1TxThisPt=0;
   	Com1RxStatus=TX_SET;
    U1TXREG=Com1RxBuffer[Com1TxThisPt];
//   	Com1TxThisPt=1;   
   	Com1TxThisPt++;   
}





void _ISR _U1TXInterrupt(void)
{

    _U1TXIF=0;

////   	Com1RxStatus=STX_CHK;

    
/////////////    
	if(Com1TxThisPt == Com1TxCnt){
      	Com1SerialTime=0;
      	Com1RxStatus = STX_CHK;
//		Com1TxThisPt =0;
//		Com1TxCnt=0;
    }
    else{
      	U1TXREG=Com1RxBuffer[Com1TxThisPt];
      	Com1TxThisPt++;
      	Com1SerialTime=0;
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


	Com1ReceiveData(buf1);
}


