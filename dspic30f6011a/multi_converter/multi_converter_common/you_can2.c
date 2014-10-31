

#include <p30f6011a.h>            
#include    "you_can2.h" 


unsigned    int     CAN2bit;

unsigned    char    Can2RxLiveCnt;
unsigned    char    Can2Tx1ConfirmCnt;
unsigned    char    Can2Tx0ConfirmCnt;


unsigned    int     Can2Block_Nm=0;
unsigned    int     Can2DataSeq=0;
unsigned 	int		Can2TxDataCnt=0;
unsigned 	int   	Can2DataCnt=0;
unsigned    char    Can2DataFrameNm=0;
unsigned    char	Can2RxDataTotalCnt=0;
unsigned    char    Can2RxDataTxPointer=0;
unsigned    char	Can2TxDataTotalCnt=0;
unsigned    char    Can2TxDataTxPointer=0;
unsigned	char	Can2PollingTimer=0;
unsigned	char	Can2RxTimer=0;
unsigned    int    	Can2ContCrc;

unsigned    char    Can2MySrcAddr=0;
unsigned    char    Can2MyDestAddr=0;
unsigned    char    Can2MyProductId=0;
unsigned    char    Can2MyGroupAddr=0;
unsigned    char    Can2MyLocalAddr=0;

unsigned    char    Can2RxSrcAddr=0;
unsigned    char    Can2RxDestAddr=0;
unsigned    char    Can2RxProductId=0;
unsigned    char    Can2RxGroupAddr=0;
unsigned    char    Can2RxLocalAddr=0;


unsigned	char	Can2RxCnt=0;
unsigned	char	Can2RxThisPt=0;
unsigned	char	Can2TxCnt=0;
unsigned	char	Can2TxThisPt=0;

unsigned    long    Can2RxEidFilter;
unsigned    int     Can2RxSidFilter;
unsigned    long    Can2RxEidMask;
unsigned    int     Can2RxSidMask;
unsigned    long    Can2TxEid;
unsigned    int     Can2TxSid;
unsigned    char    Can2TxDlc;
unsigned    long    Can2RxEid;
unsigned    int     Can2RxSid;
unsigned    char    Can2RxDlc;

unsigned    long    BefCan2RxEid;
unsigned    int     BefCan2RxSid;


unsigned 	int   	Can2ThisRxBuf[8];
unsigned 	int   	Can2ThisTxBuf[8];



void  __attribute__((section(".usercode")))  Can2TxSidSet(int MsgFlag,unsigned int sid)
{
    unsigned int  uppereid;

    uppereid=((sid << 5) & 0xf800);
    sid=((sid<<2) & 0x00fc);
    sid=(sid | uppereid);

    switch(MsgFlag)
    {
    case 0:
        C2TX0SID  = (C2TX0SID & 0x0003); 
        C2TX0SID  = (C2TX0SID | sid);
        C2TX0SID  = (C2TX0SID | 0x01);
        break;
    case 1:
        C2TX1SID  = (C2TX1SID & 0x0003); 
        C2TX1SID  = (C2TX1SID | sid);
        C2TX1SID  = (C2TX1SID | 0x01);
        break;
    case 2:
        C2TX2SID  = (C2TX2SID & 0x0003); 
        C2TX2SID  = (C2TX2SID | sid);
        C2TX2SID  = (C2TX2SID | 0x01);
	    break;
    default:
        C2TX0SID  = (C2TX0SID & 0x0003); 
        C2TX0SID  = (C2TX0SID | sid);
        C2TX0SID  = (C2TX0SID | 0x01);
        break;
    }
}


void  __attribute__((section(".usercode")))  Can2TxEidSet(int MsgFlag,unsigned long eid)
{

    unsigned int tmpeidH,tmpeidL;


	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(tmpeidH & 0x00ff);
	tmpeidH=(unsigned int)((tmpeidH << 4) & 0xf000);
	tmpeidH=(unsigned int)(tmpeidH | tmpeidL);

	tmpeidL=(unsigned int)((eid << 10) & 0xfc00);


    switch(MsgFlag)
    {
    case 0:
        C2TX0EID = (C2TX0EID & 0x0f00);
        C2TX0EID = (C2TX0EID | tmpeidH);

        C2TX0DLC = (C2TX0DLC & 0x03ff); 
        C2TX0DLC = (C2TX0DLC | tmpeidL); 
        break;
    case 1:
        C2TX1EID = (C2TX1EID & 0x0f00);
        C2TX1EID = (C2TX1EID | tmpeidH);

        C2TX1DLC = (C2TX1DLC & 0x03ff); 
        C2TX1DLC = (C2TX1DLC | tmpeidL); 
        break;
    case 2:
        C2TX2EID = (C2TX2EID & 0x0f00);
        C2TX2EID = (C2TX2EID | tmpeidH);

        C2TX2DLC = (C2TX2DLC & 0x03ff); 
        C2TX2DLC = (C2TX2DLC | tmpeidL); 
	    break;
    default:
        C2TX0EID = (C2TX0EID & 0x0f00);
        C2TX0EID = (C2TX0EID | tmpeidH);

        C2TX0DLC = (C2TX0DLC & 0x03ff); 
        C2TX0DLC = (C2TX0DLC | tmpeidL); 
        break;
    }
}



void  __attribute__((section(".usercode")))  Can2RxFilterSet(int filter_no, unsigned int sid, unsigned long eid)
{      

    unsigned int tmpeidH,tmpeidL;

//	C2CTRLbits.REQOP    = 4;

    sid=((sid << 2) & 0x1ffc);

	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(eid << 10);


    switch(filter_no)
    {
    case 0:
      C2RXF0SID  = (C2RXF0SID & 0xe003);
      C2RXF0SID  = (C2RXF0SID | sid);
      C2RXF0SID  = (C2RXF0SID | 0x0001); 

      C2RXF0EIDH = (C2RXF0EIDH & 0xf000  );   //uppereid1;   
      C2RXF0EIDH = (C2RXF0EIDH | tmpeidH );   //uppereid1;   

      C2RXF0EIDL = (C2RXF0EIDL & 0x03ff );   //uppereid;        
      C2RXF0EIDL = (C2RXF0EIDL | tmpeidL );   //uppereid;        
      break;
    case 1:
      C2RXF1SID  = (C2RXF1SID & 0xe003);
      C2RXF1SID  = (C2RXF1SID | sid);
      C2RXF1SID  = (C2RXF1SID | 0x0001); 

      C2RXF1EIDH = (C2RXF1EIDH & 0xf000  );   //uppereid1;   
      C2RXF1EIDH = (C2RXF1EIDH | tmpeidH );   //uppereid1;   

      C2RXF1EIDL = (C2RXF1EIDL & 0x03ff );   //uppereid;        
      C2RXF1EIDL = (C2RXF1EIDL | tmpeidL );   //uppereid;        
      break;
    case 2:
      C2RXF2SID  = (C2RXF2SID & 0xe003);
      C2RXF2SID  = (C2RXF2SID | sid);
      C2RXF2SID  = (C2RXF2SID | 0x0001); 

      C2RXF2EIDH = (C2RXF2EIDH & 0xf000  );   //uppereid1;   
      C2RXF2EIDH = (C2RXF2EIDH | tmpeidH );   //uppereid1;   

      C2RXF2EIDL = (C2RXF2EIDL & 0x03ff );   //uppereid;        
      C2RXF2EIDL = (C2RXF2EIDL | tmpeidL );   //uppereid;        
      break;
    case 3:
      C2RXF3SID  = (C2RXF3SID & 0xe003);
      C2RXF3SID  = (C2RXF3SID | sid);
      C2RXF3SID  = (C2RXF3SID | 0x0001); 

      C2RXF3EIDH = (C2RXF3EIDH & 0xf000  );   //uppereid1;   
      C2RXF3EIDH = (C2RXF3EIDH | tmpeidH );   //uppereid1;   

      C2RXF3EIDL = (C2RXF3EIDL & 0x03ff );   //uppereid;        
      C2RXF3EIDL = (C2RXF3EIDL | tmpeidL );   //uppereid;        
      break;
    case 4:
      C2RXF4SID  = (C2RXF4SID & 0xe003);
      C2RXF4SID  = (C2RXF4SID | sid);
      C2RXF4SID  = (C2RXF4SID | 0x0001); 

      C2RXF4EIDH = (C2RXF4EIDH & 0xf000  );   //uppereid1;   
      C2RXF4EIDH = (C2RXF4EIDH | tmpeidH );   //uppereid1;   

      C2RXF4EIDL = (C2RXF4EIDL & 0x03ff );   //uppereid;        
      C2RXF4EIDL = (C2RXF4EIDL | tmpeidL );   //uppereid;        
      break;
    case 5:
      C2RXF5SID  = (C2RXF5SID & 0xe003);
      C2RXF5SID  = (C2RXF5SID | sid);
      C2RXF5SID  = (C2RXF5SID | 0x0001); 

      C2RXF5EIDH = (C2RXF5EIDH & 0xf000  );   //uppereid1;   
      C2RXF5EIDH = (C2RXF5EIDH | tmpeidH );   //uppereid1;   

      C2RXF5EIDL = (C2RXF5EIDL & 0x03ff );   //uppereid;        
      C2RXF5EIDL = (C2RXF5EIDL | tmpeidL );   //uppereid;        
      break;
    default:
      C2RXF0SID  = (C2RXF0SID & 0xe003);
      C2RXF0SID  = (C2RXF0SID | sid);
      C2RXF0SID  = (C2RXF0SID | 0x0001); 

      C2RXF0EIDH = (C2RXF0EIDH & 0xf000  );   //uppereid1;   
      C2RXF0EIDH = (C2RXF0EIDH | tmpeidH );   //uppereid1;   

      C2RXF0EIDL = (C2RXF0EIDL & 0x03ff );   //uppereid;        
      C2RXF0EIDL = (C2RXF0EIDL | tmpeidL );   //uppereid;        
      break;
    }

//	C2CTRLbits.REQOP    = 0;

}

void  __attribute__((section(".usercode")))  Can2RxMaskSet(int mask_no, unsigned int sid, unsigned long eid)
{ 
    unsigned int tmpeidH,tmpeidL;

//	C2CTRLbits.REQOP    = 4;

    sid=((sid << 2) & 0x1ffc);
	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(eid << 10);
 
    switch(mask_no)
    {
    case 0:
      C2RXM0SID = (C2RXM0SID & 0x0001);
      C2RXM0SID = (C2RXM0SID | sid);
      C2RXM0SID = (C2RXM0SID | 0x0001);

      C2RXM0EIDH = (C2RXM0EIDH & 0xf000);   //uppereid1;     
      C2RXM0EIDH = (C2RXM0EIDH | tmpeidH);   //uppereid1;     
      C2RXM0EIDL = (C2RXM0EIDL & 0x03ff);   //uppereid;          
      C2RXM0EIDL = (C2RXM0EIDL | tmpeidL);   //uppereid;          
      break;
    case 1:
      C2RXM1SID = (C2RXM1SID & 0x0001);
      C2RXM1SID = (C2RXM1SID | sid);
      C2RXM1SID = (C2RXM1SID | 0x0001);

      C2RXM1EIDH = (C2RXM1EIDH & 0xf000);   //uppereid1;     
      C2RXM1EIDH = (C2RXM1EIDH | tmpeidH);   //uppereid1;     
      C2RXM1EIDL = (C2RXM1EIDL & 0x03ff);   //uppereid;          
      C2RXM1EIDL = (C2RXM1EIDL | tmpeidL);   //uppereid;          
      break;
    default:
      C2RXM0SID = (C2RXM0SID & 0x0001);
      C2RXM0SID = (C2RXM0SID | sid);
      C2RXM0SID = (C2RXM0SID | 0x0001);

      C2RXM0EIDH = (C2RXM0EIDH & 0xf000);   //uppereid2;     
      C2RXM0EIDH = (C2RXM0EIDH | tmpeidH);   //uppereid2;     
      C2RXM0EIDL = (C2RXM0EIDL & 0x03ff);   //uppereid;          
      C2RXM0EIDL = (C2RXM0EIDL | tmpeidL);   //uppereid;          
      break;
    }

//	C2CTRLbits.REQOP    = 0;

}


unsigned int   __attribute__((section(".usercode"))) Can2RxFilterMaskSet(int mask_no)
{
    C2CTRLbits.REQOP    = 0x4;

	Can2PollingTimer=0;
	while(Can2PollingTimer<10);
	Can2PollingTimer=0;
    asm("CLRWDT");

	Can2RxFilterSet(mask_no,Can2RxSidFilter,Can2RxEidFilter);
	Can2RxMaskSet(mask_no,Can2RxSidMask,Can2RxEidMask);

	Can2PollingTimer=0;
	while(Can2PollingTimer<10);
	Can2PollingTimer=0;
    asm("CLRWDT");

    C2CTRLbits.REQOP    = 0x0;
	return(0);
}



void __attribute__((section(".usercode")))  CAN2SendMessageyou(char MsgFlag,unsigned int * data, unsigned int  datalen) 
{
    int i;

    for(i = 0;i < datalen;i++)
    {
        switch(MsgFlag)
        {
        case 0: 
            *((unsigned char *)&C2TX0B1+i)= data[i];
            break;
        case 1: *((unsigned char *)&C2TX1B1+i)= data[i];
            break;
        case 2: *((unsigned char *)&C2TX2B1+i)= data[i];
            break;
        default:*((unsigned char *)&C2TX0B1+i)= data[i];
            break;
        }
    }


    
    switch(MsgFlag)
    {
    case 0:
        C2TX0DLCbits.DLC = datalen;
        C2TX0CON=7;         
        C2TX0CONbits.TXREQ = 1;
        break;
    case 1:
        C2TX1DLCbits.DLC = datalen;
        C2TX1CON=7; 
        C2TX1CONbits.TXREQ = 1;
        break;
    case 2:
        C2TX2DLCbits.DLC = datalen;
        C2TX2CON=7; 
        C2TX2CONbits.TXREQ = 1;
        break;
    default:
        C2TX0DLCbits.DLC = datalen;
        C2TX0CON=7; 
        C2TX0CONbits.TXREQ = 1;
        break;
    }
}




void  __attribute__((section(".usercode")))     Can2GetRxSidEidDlc(void)
{
    unsigned long ltmp1,ltmp2;

	ltmp1=(unsigned long)C2RX0EID;
	ltmp1=(ltmp1 << 6);

	ltmp2=(unsigned long)C2RX0DLC;
	ltmp2=(ltmp2 >> 10);

	Can2RxEid=(ltmp2 | ltmp1);
	Can2RxSid=(C2RX0SID >> 2);
	Can2RxDlc=(C2RX0DLC & 0x0f);	
}




void  __attribute__((section(".usercode")))     Can2TxDataLoad(unsigned char pt)
{
/*		
	Can2_Buf[0]=Can2TxBuf[pt+0];           
	Can2_Buf[1]=Can2TxBuf[pt+1];
	Can2_Buf[2]=Can2TxBuf[pt+2];
	Can2_Buf[3]=Can2TxBuf[pt+3];
	Can2_Buf[4]=Can2TxBuf[pt+4];
	Can2_Buf[5]=Can2TxBuf[pt+5];
	Can2_Buf[6]=Can2TxBuf[pt+6];
	Can2_Buf[7]=Can2TxBuf[pt+7];
*/

	Can2ThisTxBuf[0]=Can2TxBuf[pt+0];           
	Can2ThisTxBuf[1]=Can2TxBuf[pt+1];
	Can2ThisTxBuf[2]=Can2TxBuf[pt+2];
	Can2ThisTxBuf[3]=Can2TxBuf[pt+3];
	Can2ThisTxBuf[4]=Can2TxBuf[pt+4];
	Can2ThisTxBuf[5]=Can2TxBuf[pt+5];
	Can2ThisTxBuf[6]=Can2TxBuf[pt+6];
	Can2ThisTxBuf[7]=Can2TxBuf[pt+7];
}





unsigned int __attribute__((section(".usercode")))  Can2TxData(unsigned char pt,unsigned char nm)
{
	C2EC=0;

    if( (!C2TX0CONbits.TXREQ ) && (Can2PollingTimer > 5)){
		Can2PollingTimer=0;
		Can2TxSidSet(nm,Can2TxSid);
		Can2TxEidSet(nm,Can2TxEid);
		Can2TxDataLoad(pt);
		CAN2SendMessageyou(nm,&Can2ThisTxBuf[0], Can2TxDlc);
        
		bCan2TxStart=0;	
		return(0);
	}
	return(1);
}





unsigned int   __attribute__((section(".usercode"))) Can2ThisRxDataLoad(void)
{
	Can2ThisRxBuf[0]=(unsigned char)C2RX0B1;		
	Can2ThisRxBuf[1]=(unsigned char)(C2RX0B1 >> 8);		
	Can2ThisRxBuf[2]=(unsigned char)C2RX0B2;		
	Can2ThisRxBuf[3]=(unsigned char)(C2RX0B2 >> 8);		
	Can2ThisRxBuf[4]=(unsigned char)C2RX0B3;		
	Can2ThisRxBuf[5]=(unsigned char)(C2RX0B3 >> 8);		
	Can2ThisRxBuf[6]=(unsigned char)C2RX0B4;		
	Can2ThisRxBuf[7]=(unsigned char)(C2RX0B4 >> 8);
		
	return(0);	
}



void _ISR _C2Interrupt(void)
{
	unsigned char	j;

    if(C2INTFbits.RX0IF){
		Can2RxTimer=0;
		Can2GetRxSidEidDlc();
		Can2ThisRxDataLoad();
		Can2ReceiveData();
        C2INTFbits.RX0IF=0;
        C2RX0CONbits.RXFUL=0;
    }    
    else{
        C2CTRLbits.ABAT=1;      
        C2INTFbits.RX1IF=0; 
        C2RX1CONbits.RXFUL=0;
        C2INTFbits.WAKIF=0;
        C2INTF=0;
        C2INTFbits.ERRIF=0;

    }

//	IFS2bits.C2IF=0;

	_C2IF=0;

}




void    __attribute__((section(".usercode"))) Can2Init(void)
{
	unsigned int 	pdid;
	unsigned long 	myid;


    C2CTRLbits.REQOP    = 0x4;      // Entry Configration Mode
    C2CTRLbits.CANCAP   = 0x0;      // disable CAN Capture mode
    C2CTRLbits.CSIDL    = 0x1;      // when idle mode,stop can
    C2CTRLbits.ABAT     = 0x1;      // aboart all pending transmit
    C2CTRLbits.CANCKS   = 0x1;      // fcan=fcy

    C2CFG2bits.SAM      = 0x1 ;     // Bus Line is sampled three times prior to the sample point
    C2CFG2bits.WAKFIL   = 0x0 ; 	// CAN bus line filter is not used for wake-up
    C2CFG2bits.SEG2PHTS = 0x1 ;     // Freely programmable;

    C2CFG1bits.BRP      = 19;      // tq=0.0000025
  
    C2CFG1bits.SJW      = 0x0;      // Synchronized Jump Width bits 1xTq
    C2CFG2bits.SEG1PH   = 0x1 ;     // 2 Tq
    C2CFG2bits.PRSEG    = 0x2 ;     // 3 Tq
    C2CFG2bits.SEG2PH   = 0x1 ; 	// 2 Tq

    C2TX0CON            = 0x3;
    C2TX1CON            = 0x3;

    C2TX0SIDbits.TXIDE  = 0x1;
    C2TX0SIDbits.SRR    = 0x0;
    C2TX0DLCbits.TXRB0  = 0x0;
    C2TX0DLCbits.TXRB1  = 0x0;
    C2TX0DLCbits.TXRTR  = 0x0;

    C2TX1SIDbits.TXIDE  = 0x1;
    C2TX1SIDbits.SRR    = 0x0;
    C2TX1DLCbits.TXRB0  = 0x0;
    C2TX1DLCbits.TXRB1  = 0x0;
    C2TX1DLCbits.TXRTR  = 0x0;

    C2TX2SIDbits.TXIDE  = 0x1;
    C2TX2SIDbits.SRR    = 0x1;
    C2TX2DLCbits.TXRB0  = 0x0;
    C2TX2DLCbits.TXRB1  = 0x0;
    C2TX2DLCbits.TXRTR  = 0x0;


    C2RXM0SID           = 0x0;
    C2RXM0EIDH          = 0x0;
    C2RXM0EIDL          = 0x0;
    C2RXM0SIDbits.MIDE  = 0x0;

    C2RXM1SID           = 0x0;
    C2RXM1EIDH          = 0x0;
    C2RXM1EIDL          = 0x0;
    C2RXM1SIDbits.MIDE  = 0x0;

    C2RXF0SID           = 0x0;
    C2RXF0EIDH          = 0x0;
    C2RXF0EIDL          = 0x0;
    C2RXF0SIDbits.EXIDE = 0x0;

    C2RXF1SID           = 0x0;
    C2RXF1EIDH          = 0x0;
    C2RXF1EIDL          = 0x0;
    C2RXF1SIDbits.EXIDE = 0x0;

    C2RXF2SID           = 0x0;
    C2RXF2EIDH          = 0x0;
    C2RXF2EIDL          = 0x0;
    C2RXF2SIDbits.EXIDE = 0x0;

    C2RXF3SID           = 0x0;
    C2RXF3EIDH          = 0x0;
    C2RXF3EIDL          = 0x0;
    C2RXF3SIDbits.EXIDE = 0x0;

    C2RXF4SID           = 0x0;
    C2RXF4EIDH          = 0x0;
    C2RXF4EIDL          = 0x0;
    C2RXF4SIDbits.EXIDE = 0x0;

    C2RXF5SID           = 0x0;
    C2RXF5EIDH          = 0x0;
    C2RXF5EIDL          = 0x0;
    C2RXF5SIDbits.EXIDE = 0x0;

    C2RX0CONbits.RXFUL  = 0x0;
    C2RX0CONbits.DBEN   = 0x0;
    C2RX0CONbits.JTOFF  = 0x0;
    C2RX0CONbits.FILHIT0= 0x0;

    C2RX1CONbits.RXFUL  = 0x0;


    C2INTE              = 0x0;
    C2INTEbits.ERRIE    = 0x1;
    C2INTEbits.RX0IE    = 0x1;
    C2INTEbits.RX1IE    = 0x1;


	Can2RxFilterSet(0,0xffff, 0xffffff);
	Can2RxFilterSet(1,0xffff, 0xffffff);
	Can2RxFilterSet(2,0xffff, 0xffffff);
	Can2RxFilterSet(3,0xffff, 0xffffff);
	Can2RxFilterSet(4,0xffff, 0xffffff);
	Can2RxFilterSet(5,0xffff, 0xffffff);

	Can2RxMaskSet(0,0xffff, 0xffffff);
	Can2RxMaskSet(1,0xffff, 0xffffff);


    _C2IE       = 0x1;
    IPC9bits.C2IP=4;
    C2CTRLbits.REQOP    = 0x0;
    C2EC=0;
}


