
#include <p30f6011a.h>            

#include  "..\..\multi_converter_common\host_io.h"            
#include  "..\..\multi_converter_common\30f6010_io.h"
#include  "..\..\multi_converter_common\you_can1.h"
#include  "..\..\multi_converter_common\you_can2.h"
#include  "..\..\multi_converter_common\ComPort1.h"
#include  "..\..\multi_converter_common\ComPort2.h"
#include  "..\..\multi_converter_common\memory_map.h"


unsigned char   Can2_TmpBuf[8];






unsigned int   __attribute__((section(".usercode"))) Can2ReceiveData(void)
{
/*
	unsigned char  i;

	unsigned char	tmpbuf[10];

	if(Com1SerialTime > 20){
	    Com1RxCnt=0;
	}

	Com1SerialTime=0;


	Can2RxDataLoadTmpbuf(&tmpbuf[0]);
	for(i=0;i<Can2RxDlc;i++){
		Com1RxBuffer[Com1RxCnt+i]=tmpbuf[i];
	}
	
	Com1RxCnt=(Com1RxCnt + Can2RxDlc);	
	FromCan2_HomeCallProtocolChk();
*/
	return(0);
}




/*
void    __attribute__((section(".usercode")))  Can2AckCurrentNormalData(void)
{
	unsigned int i,j;

	j=(Can2TxDataTotalCnt - Can2TxDataTxPointer);
	if(j >= 8)	j=8;
	Can2TxDlc=j;


	if( Can2TxDataTotalCnt <= (Can2TxDataTxPointer + Can2TxDlc)){
		j=8;
	}
	else{
		if(Can2TxDataTxPointer > 0){
			j= (Can2TxDataTxPointer / 8);
		}
		else	j=0;
	}

	Can2TxEid=(Can2TxEid | j);
}




void    __attribute__((section(".usercode")))  Can2ReloadData(void)
{
	unsigned char i,j;

	i=Can2TxDataTxPointer;

	for(j=0;(i<Can2TxDataTotalCnt) && (j<8);i++,j++){
		Can2TxBuf[j]=Com1TxBuffer[i];
 	}          
	Can2TxDlc=j;
}



void    __attribute__((section(".usercode")))  Can2TxCheck(void)
{

	if(Can2TxDataTotalCnt > Can2TxDataTxPointer){	
	    if(!C2TX0CONbits.TXREQ){

			Can2TxSid=0;
			Can2TxEid=0;
			Can2TxDlc=0;
			Can2ReloadData();
			
			if( !Can2TxData(0,0)){
				Can2TxDataTxPointer=Can2TxDataTxPointer+Can2TxDlc;
				if(Can2TxDataTotalCnt <= Can2TxDataTxPointer){
					Can2TxDataTotalCnt =0;
				 	Can2TxDataTxPointer=0;
				}					
			}	
		}
	}
}
*/







