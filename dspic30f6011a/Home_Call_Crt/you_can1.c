

#include <p30f6011a.h>      
#include    "you_can1.h" 
///#include    "..\..\can_repeater_main\can_repeater_main.h" 


unsigned    char    Can1TxBuf[CAN1_MAX_SAVE_BUF];
unsigned    char    Can1RxBuf[CAN1_MAX_SAVE_BUF];


unsigned    int     CAN1bit;

//unsigned    char    Can1MySrcAddr=0;
//unsigned    char    Can1MyDestAddr=0;
//unsigned    char    Can1MyProductId=0;
//unsigned    char    Can1RxSrcAddr=0;
//unsigned    char    Can1RxDestAddr=0;
//unsigned    char    Can1RxProductId=0;

unsigned    char    Can1RxLiveCnt=0;
unsigned    char    Can1Tx1ConfirmCnt=0;
unsigned    char    Can1Tx0ConfirmCnt=0;


unsigned    int     Can1Block_Nm=0;
unsigned    int     Can1DataSeq=0;
unsigned 	int		Can1TxDataCnt=0;
unsigned 	int   	Can1DataCnt=0;
unsigned    char    Can1DataFrameNm=0;
unsigned    char	Can1RxDataTotalCnt=0;
unsigned    char    Can1RxDataTxPointer=0;
unsigned    char	Can1TxDataTotalCnt=0;
unsigned    char    Can1TxDataTxPointer=0;
unsigned	char	Can1PollingTimer=0;
unsigned	char	Can1RxTimer=0;
unsigned    int    	Can1ContCrc;


unsigned    char    Can1MySrcAddr=0;
unsigned    char    Can1MyDestAddr=0;
unsigned    char    Can1MyProductId=0;
unsigned    char    Can1MyGroupAddr=0;
unsigned    char    Can1MyLocalAddr=0;

unsigned    char    Can1RxSrcAddr=0;
unsigned    char    Can1RxDestAddr=0;
unsigned    char    Can1RxProductId=0;
unsigned    char    Can1RxGroupAddr=0;
unsigned    char    Can1RxLocalAddr=0;

/*
unsigned    char    Can1RxSrcAddr=0;
unsigned    char    Can1RxDestAddr=0;
unsigned    char    Can1RxProductId=0;
*/

unsigned	char	Can1RxCnt=0;
unsigned	char	Can1RxThisPt=0;
unsigned	char	Can1TxCnt=0;
unsigned	char	Can1TxThisPt=0;


unsigned    long    Can1RxEidFilter;
unsigned    int     Can1RxSidFilter;
unsigned    long    Can1RxEidMask;
unsigned    int     Can1RxSidMask;
unsigned    long    Can1TxEid;
unsigned    int     Can1TxSid;
unsigned    char    Can1TxDlc;
unsigned    long    Can1RxEid;
unsigned    int     Can1RxSid;
unsigned    char    Can1RxDlc;


unsigned 	int   	Can1_Buf[8];



void   __attribute__((section(".usercode"))) Can1TxSidSet(int MsgFlag,unsigned int sid)
{
    unsigned int  uppereid;

    uppereid=((sid << 5) & 0xf800);
    sid=((sid<<2) & 0x00fc);
    sid=(sid | uppereid);

    switch(MsgFlag)
    {
    case 0:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);
        C1TX0SID  = (C1TX0SID | 0x01);
        break;
    case 1:
        C1TX1SID  = (C1TX1SID & 0x0003); 
        C1TX1SID  = (C1TX1SID | sid);
        C1TX1SID  = (C1TX1SID | 0x01);
        break;
    case 2:
        C1TX2SID  = (C1TX2SID & 0x0003); 
        C1TX2SID  = (C1TX2SID | sid);
        C1TX2SID  = (C1TX2SID | 0x01);
	    break;
    default:
        C1TX0SID  = (C1TX0SID & 0x0003); 
        C1TX0SID  = (C1TX0SID | sid);
        C1TX0SID  = (C1TX0SID | 0x01);
        break;
    }
}


void   __attribute__((section(".usercode"))) Can1TxEidSet(int MsgFlag,unsigned long eid)
{

    unsigned int tmpeidH,tmpeidL,tmpbuf;


	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(tmpeidH & 0x00ff);
	tmpeidH=(unsigned int)((tmpeidH << 4) & 0xf000);
	tmpeidH=(unsigned int)(tmpeidH | tmpeidL);
	tmpeidL=(unsigned int)((eid << 10) & 0xfc00);


    switch(MsgFlag)
    {
    case 0:
/*
		tmpbuf=C1TX0EID;
		tmpbuf=(tmpbuf & 0x0f00);
		tmpbuf=(tmpbuf | tmpeidH);
 		C1TX0EID = tmpbuf;

		tmpbuf=C1TX0DLC;
		tmpbuf=(tmpbuf & 0x03ff);
		tmpbuf=(tmpbuf | tmpeidL);
		C1TX0DLC = tmpbuf;
*/

 
        C1TX0EID = (C1TX0EID & 0x0f00);
        C1TX0EID = (C1TX0EID | tmpeidH);

        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | tmpeidL); 
        break;
    case 1:
        C1TX1EID = (C1TX1EID & 0x0f00);
        C1TX1EID = (C1TX1EID | tmpeidH);

        C1TX1DLC = (C1TX1DLC & 0x03ff); 
        C1TX1DLC = (C1TX1DLC | tmpeidL); 
        break;
    case 2:
        C1TX2EID = (C1TX2EID & 0x0f00);
        C1TX2EID = (C1TX2EID | tmpeidH);

        C1TX2DLC = (C1TX2DLC & 0x03ff); 
        C1TX2DLC = (C1TX2DLC | tmpeidL); 
	    break;
    default:
        C1TX0EID = (C1TX0EID & 0x0f00);
        C1TX0EID = (C1TX0EID | tmpeidH);

        C1TX0DLC = (C1TX0DLC & 0x03ff); 
        C1TX0DLC = (C1TX0DLC | tmpeidL); 
        break;
    }


}




void  __attribute__((section(".usercode")))  Can1RxFilterSet(int filter_no, unsigned int sid, unsigned long eid)
{      

    unsigned int tmpeidH,tmpeidL;

///////////////////////////	C1CTRLbits.REQOP    = 4;

    sid=((sid << 2) & 0x1ffc);

	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(eid << 10);


    switch(filter_no)
    {
    case 0:
      C1RXF0SID  = (C1RXF0SID & 0xe003);
      C1RXF0SID  = (C1RXF0SID | sid);
      C1RXF0SID  = (C1RXF0SID | 0x0001); 

      C1RXF0EIDH = (C1RXF0EIDH & 0xf000  );   //uppereid1;   
      C1RXF0EIDH = (C1RXF0EIDH | tmpeidH );   //uppereid1;   

      C1RXF0EIDL = (C1RXF0EIDL & 0x03ff );   //uppereid;        
      C1RXF0EIDL = (C1RXF0EIDL | tmpeidL );   //uppereid;        
      break;
    case 1:
      C1RXF1SID  = (C1RXF1SID & 0xe003);
      C1RXF1SID  = (C1RXF1SID | sid);
      C1RXF1SID  = (C1RXF1SID | 0x0001); 

      C1RXF1EIDH = (C1RXF1EIDH & 0xf000  );   //uppereid1;   
      C1RXF1EIDH = (C1RXF1EIDH | tmpeidH );   //uppereid1;   

      C1RXF1EIDL = (C1RXF1EIDL & 0x03ff );   //uppereid;        
      C1RXF1EIDL = (C1RXF1EIDL | tmpeidL );   //uppereid;        
      break;
    case 2:
      C1RXF2SID  = (C1RXF2SID & 0xe003);
      C1RXF2SID  = (C1RXF2SID | sid);
      C1RXF2SID  = (C1RXF2SID | 0x0001); 

      C1RXF2EIDH = (C1RXF2EIDH & 0xf000  );   //uppereid1;   
      C1RXF2EIDH = (C1RXF2EIDH | tmpeidH );   //uppereid1;   

      C1RXF2EIDL = (C1RXF2EIDL & 0x03ff );   //uppereid;        
      C1RXF2EIDL = (C1RXF2EIDL | tmpeidL );   //uppereid;        
      break;
    case 3:
      C1RXF3SID  = (C1RXF3SID & 0xe003);
      C1RXF3SID  = (C1RXF3SID | sid);
      C1RXF3SID  = (C1RXF3SID | 0x0001); 

      C1RXF3EIDH = (C1RXF3EIDH & 0xf000  );   //uppereid1;   
      C1RXF3EIDH = (C1RXF3EIDH | tmpeidH );   //uppereid1;   

      C1RXF3EIDL = (C1RXF3EIDL & 0x03ff );   //uppereid;        
      C1RXF3EIDL = (C1RXF3EIDL | tmpeidL );   //uppereid;        
      break;
    case 4:
      C1RXF4SID  = (C1RXF4SID & 0xe003);
      C1RXF4SID  = (C1RXF4SID | sid);
      C1RXF4SID  = (C1RXF4SID | 0x0001); 

      C1RXF4EIDH = (C1RXF4EIDH & 0xf000  );   //uppereid1;   
      C1RXF4EIDH = (C1RXF4EIDH | tmpeidH );   //uppereid1;   

      C1RXF4EIDL = (C1RXF4EIDL & 0x03ff );   //uppereid;        
      C1RXF4EIDL = (C1RXF4EIDL | tmpeidL );   //uppereid;        
      break;
    case 5:
      C1RXF5SID  = (C1RXF5SID & 0xe003);
      C1RXF5SID  = (C1RXF5SID | sid);
      C1RXF5SID  = (C1RXF5SID | 0x0001); 

      C1RXF5EIDH = (C1RXF5EIDH & 0xf000  );   //uppereid1;   
      C1RXF5EIDH = (C1RXF5EIDH | tmpeidH );   //uppereid1;   

      C1RXF5EIDL = (C1RXF5EIDL & 0x03ff );   //uppereid;        
      C1RXF5EIDL = (C1RXF5EIDL | tmpeidL );   //uppereid;        
      break;
    default:
      C1RXF0SID  = (C1RXF0SID & 0xe003);
      C1RXF0SID  = (C1RXF0SID | sid);
      C1RXF0SID  = (C1RXF0SID | 0x0001); 

      C1RXF0EIDH = (C1RXF0EIDH & 0xf000  );   //uppereid1;   
      C1RXF0EIDH = (C1RXF0EIDH | tmpeidH );   //uppereid1;   

      C1RXF0EIDL = (C1RXF0EIDL & 0x03ff );   //uppereid;        
      C1RXF0EIDL = (C1RXF0EIDL | tmpeidL );   //uppereid;        
      break;
    }

///////////////////////////	C1CTRLbits.REQOP    = 0;

}

void  __attribute__((section(".usercode")))  Can1RxMaskSet(int mask_no, unsigned int sid, unsigned long eid)
{ 
    unsigned int tmpeidH,tmpeidL;

//////////////////////	C1CTRLbits.REQOP    = 4;

    sid=((sid << 2) & 0x1ffc);
	tmpeidH=(unsigned int)(eid >> 6);
	tmpeidL=(unsigned int)(eid << 10);
 
    switch(mask_no)
    {
    case 0:
      C1RXM0SID = (C1RXM0SID & 0x0001);
      C1RXM0SID = (C1RXM0SID | sid);
      C1RXM0SID = (C1RXM0SID | 0x0001);

      C1RXM0EIDH = (C1RXM0EIDH & 0xf000);   //uppereid1;     
      C1RXM0EIDH = (C1RXM0EIDH | tmpeidH);   //uppereid1;     
      C1RXM0EIDL = (C1RXM0EIDL & 0x03ff);   //uppereid;          
      C1RXM0EIDL = (C1RXM0EIDL | tmpeidL);   //uppereid;          
      break;
    case 1:
      C1RXM1SID = (C1RXM1SID & 0x0001);
      C1RXM1SID = (C1RXM1SID | sid);
      C1RXM1SID = (C1RXM1SID | 0x0001);

      C1RXM1EIDH = (C1RXM1EIDH & 0xf000);   //uppereid1;     
      C1RXM1EIDH = (C1RXM1EIDH | tmpeidH);   //uppereid1;     
      C1RXM1EIDL = (C1RXM1EIDL & 0x03ff);   //uppereid;          
      C1RXM1EIDL = (C1RXM1EIDL | tmpeidL);   //uppereid;          
      break;
    default:
      C1RXM0SID = (C1RXM0SID & 0x0001);
      C1RXM0SID = (C1RXM0SID | sid);
      C1RXM0SID = (C1RXM0SID | 0x0001);

      C1RXM0EIDH = (C1RXM0EIDH & 0xf000);   //uppereid1;     
      C1RXM0EIDH = (C1RXM0EIDH | tmpeidH);   //uppereid1;     
      C1RXM0EIDL = (C1RXM0EIDL & 0x03ff);   //uppereid;          
      C1RXM0EIDL = (C1RXM0EIDL | tmpeidL);   //uppereid;          
      break;
    }

////////////////////	C1CTRLbits.REQOP    = 0;

}


unsigned int   __attribute__((section(".usercode"))) Can1RxFilterMaskSet(int mask_no)
{

    C1CTRLbits.REQOP    = 0x4;

	Can1PollingTimer=0;
	while(Can1PollingTimer<10);
	Can1PollingTimer=0;
    asm("CLRWDT");

	
	Can1RxFilterSet(mask_no,Can1RxSidFilter,Can1RxEidFilter);
	Can1RxMaskSet(mask_no,Can1RxSidMask,Can1RxEidMask);

	Can1PollingTimer=0;
	while(Can1PollingTimer<10);
	Can1PollingTimer=0;
    asm("CLRWDT");

    C1CTRLbits.REQOP    = 0x0;

	return(0);
}



void __attribute__((section(".usercode")))  CAN1SendMessageyou(char MsgFlag,unsigned int * data, unsigned int  datalen) 
{
    int i;

    for(i = 0;i < datalen;i++)
    {
        switch(MsgFlag)
        {
        case 0: 
            *((unsigned char *)&C1TX0B1+i)= data[i];
            break;
        case 1: *((unsigned char *)&C1TX1B1+i)= data[i];
            break;
        case 2: *((unsigned char *)&C1TX2B1+i)= data[i];
            break;
        default:*((unsigned char *)&C1TX0B1+i)= data[i];
            break;
        }
    }


    
    switch(MsgFlag)
    {
    case 0:
        C1TX0DLCbits.DLC = datalen;
        C1TX0CON=7;         
        C1TX0CONbits.TXREQ = 1;
        break;
    case 1:
        C1TX1DLCbits.DLC = datalen;
        C1TX1CON=7; 
        C1TX1CONbits.TXREQ = 1;
        break;
    case 2:
        C1TX2DLCbits.DLC = datalen;
        C1TX2CON=7; 
        C1TX2CONbits.TXREQ = 1;
        break;
    default:
        C1TX0DLCbits.DLC = datalen;
        C1TX0CON=7; 
        C1TX0CONbits.TXREQ = 1;
        break;
    }
}






///////////////////////
///////////////////////




/*
void  __attribute__((section(".usercode")))     Can1TxSidEidArrary(void)
{
    unsigned char ThisDataCnt;
    unsigned int  itmp1,itmp2;
    unsigned long ltmp1,ltmp2,ltmp3;

    itmp1=(unsigned int)Can1ProductId;
    itmp2=(unsigned int)(Can1DataSeq);   
    itmp2=(itmp2 << 7);   
    Can1TxSid=(unsigned int)(itmp1 | itmp2);   


    ltmp1= 0;
    ltmp2= 0;
    ltmp3= 0;

    ltmp1= (unsigned long)(Can1SrcAddr);
    ltmp2= (unsigned long)(Can1DestAddr);
    ltmp2= (ltmp2 << 8);

	if(Can1DataCnt > ((Can1DataSeq+1) * 8)){
		ltmp3=(ltmp3 | 0x10000);
	}
	if(bCan1Crc_Include)		ltmp3=(ltmp3 | 0x20000);

    Can1TxEid=(ltmp1 | ltmp2 | ltmp3);


	if(Can1DataCnt >= ((Can1DataSeq+1) * 8))		ThisDataCnt=8;	
	else{
		ThisDataCnt=(Can1DataCnt % 8);
	}
	Can1TxDataCnt=ThisDataCnt;

}
*/



void  __attribute__((section(".usercode")))     GetCan1RxSidEidDlcArrary(void)
{
    unsigned long ltmp1,ltmp2;

	ltmp1=(unsigned long)C1RX0EID;
	ltmp1=(ltmp1 << 6);

	ltmp2=(unsigned long)C1RX0DLC;
	ltmp2=(ltmp2 >> 10);

	Can1RxEid=(ltmp2 |  ltmp1);
	Can1RxSid=(C1RX0SID >> 2);
	Can1RxDlc=(C1RX0DLC & 0x0f);	


/*

	Can1RxBuf[1]	=(unsigned char)(Can1RxEidBuffer >> 8); 
	Can1RxBuf[2]	=(unsigned char)(Can1RxEidBuffer); 
	Can1RxBuf[3]	=(unsigned char)(Can1RxSidBuffer & 0x7f); 

	Can1ContCrc=0;
	if(C1RX0EID & 0x0800)	Can1ContCrc=(Can1ContCrc | 0x02);
	if(C1RX0EID & 0x0400)	Can1ContCrc=(Can1ContCrc | 0x01);

	Can1DataFrameNm=(unsigned char)(Can1RxSidBuffer >> 7);
*/
}



void  __attribute__((section(".usercode")))     Can1RxSidEidArrary(void)
{
    unsigned long ltmp1,ltmp2;

	ltmp1=(unsigned long)C1RX0EID;
	ltmp1=(ltmp1 << 6);

	ltmp2=(unsigned long)C1RX0DLC;
	ltmp2=(ltmp2 >> 10);

	Can1RxEid=(ltmp2 |  ltmp1);
	Can1RxSid=(C1RX0SID >> 2);
	Can1RxDlc=(C1RX0DLC & 0x0f);	


	Can1RxBuf[1]	=(unsigned char)(Can1RxEid >> 8); 
	Can1RxBuf[2]	=(unsigned char)(Can1RxEid); 
	Can1RxBuf[3]	=(unsigned char)(Can1RxSid & 0x7f); 

	Can1ContCrc=0;
	if(C1RX0EID & 0x0800)	Can1ContCrc=(Can1ContCrc | 0x02);
	if(C1RX0EID & 0x0400)	Can1ContCrc=(Can1ContCrc | 0x01);

	Can1DataFrameNm=(unsigned char)(Can1RxSid >> 7);
}




void  __attribute__((section(".usercode")))     Can1TxDataLoad(unsigned char pt)
{		
	Can1_Buf[0]=Can1TxBuf[pt+0];           
	Can1_Buf[1]=Can1TxBuf[pt+1];
	Can1_Buf[2]=Can1TxBuf[pt+2];
	Can1_Buf[3]=Can1TxBuf[pt+3];
	Can1_Buf[4]=Can1TxBuf[pt+4];
	Can1_Buf[5]=Can1TxBuf[pt+5];
	Can1_Buf[6]=Can1TxBuf[pt+6];
	Can1_Buf[7]=Can1TxBuf[pt+7];
}







/*  참고   ////////////////////
void    CanTx0(void)
{
    if(!TXB0REQ && CanTxAct && (ReceiveAdr == SelHostAdr) ){
        TxSidEidArrary();
        CANTxDLC(0,CanDatacnt0);
        CANTxSidEidSet(0,SidBuffer,EidBuffer);

        CanKeyValue[0]=CanCmd;

		LoadCanTxBuf((unsigned char *)&TXB0D0);

//        TXB0D0=CanKeyValue[0];           
//        TXB0D1=CanKeyValue[1];
//        TXB0D2=CanKeyValue[2];
//        TXB0D3=CanKeyValue[3];
//        TXB0D4=CanKeyValue[4];
//        TXB0D5=CanKeyValue[5];
//        TXB0D6=CanKeyValue[6];
//        TXB0D7=CanKeyValue[7];


        IPR3=0;
        TXB0IP=1;

        TXB0REQ = 1 ; // 전송 요구 비트 셋

        CanReceiveCnt=0;

//        CanTxAct=0;

        if(Tx0ConfirmCnt>0)    Tx0ConfirmCnt--;    
    }
}
*/

unsigned int __attribute__((section(".usercode")))  Can1TxData(unsigned char pt,unsigned char nm)
{

	C1EC=0;

    if(!C1TX0CONbits.TXREQ){
		Can1TxSidSet(nm,Can1TxSid);
		Can1TxEidSet(nm,Can1TxEid);
		Can1TxDataLoad(pt);
		CAN1SendMessageyou(nm,&Can1_Buf[0], Can1TxDlc);
		bCan1TxStart=0;	
		return(0);
	}

	return(1);
}



void    __attribute__((section(".usercode")))  Can1Check(void)
{
    unsigned char sel;
    unsigned char pt;
	
	pt=(Can1DataSeq * 8) + CAN1_DATA_PT; 

//    unsigned int tmpeidH,tmpeidL;
//    unsigned long ltmpeid;

    sel=0;


	C1EC=0;

/*
    tmpeidH=C1EC;
    tmpeidL=C1EC & 0x00ff;


    if(tmpeidH >= 0x8100){
        Can1Init();
    }
    else if(tmpeidL >= 0x81){
        Can1Init();
    } 
*/


/*
    if(!C1TX0CONbits.TXREQ && bCan1TxAct){
			Can1TxSidEidArrary();
            Can1TxSidSet(sel,Can1TxSid);
            Can1TxEidSet(sel,Can1TxEid);
			Can1TxDataLoad(pt);
            CAN1SendMessageyou(sel,&Can1_Buf[0], Can1TxDataCnt);        

			Can1DataSeq++;		
			if(!(Can1TxEid & 0x10000)){
				bCan1TxAct=0;
			}
	}
*/
}


/*
unsigned int   __attribute__((section(".usercode"))) Can1RxFilterMaskSet(unsigned int nm,unsigned int sid,unsigned long eid)
{

	C1CTRLbits.REQOP    = 4;

	Can1PollingTimer=0;
	while(Can1PollingTimer <= 2);


	Can1RxFilterSet(nm,sid,eid);
	Can1RxMaskSet(nm,0x007f,0x0000ff00);

	C1CTRLbits.REQOP    = 0;

	return(0);
}
*/




/*
unsigned int   __attribute__((section(".usercode"))) TxCan2Buf(void)
{
	int_field	tmp_int;

	tmp_int.intger=C1RX0SID;
	tmp_int.byte[1]=(tmp_int.byte[1] << 3);
	C2TX0SID=tmp_int.intger;

	tmp_int.intger=C1RX0EID;
	tmp_int.byte[1]=(tmp_int.byte[1] << 4);
	C2TX0EID=tmp_int.intger;

	tmp_int.intger=C1RX0DLC;
	tmp_int.byte[0]=(tmp_int.byte[0] << 3);
	C2TX0DLC=tmp_int.intger;
	
	C2TX0B1= C1RX0B1;
	C2TX0B2= C1RX0B2;
	C2TX0B3= C1RX0B3;
	C2TX0B4= C1RX0B4;

	C2TX0CON=7; 
    C2TX0CONbits.TXREQ = 1;

}
*/


//#define     HOST_BOARD          0x2000
//#define     HOST_LAST_DATA      0x08




void _ISR _C1Interrupt(void)
{
	unsigned char	j;
	
	C1EC=0;

    if(C1INTFbits.RX0IF){
		GetCan1RxSidEidDlcArrary();
		Can1ReceiveData();

        C1INTFbits.RX0IF=0;
        C1RX0CONbits.RXFUL=0;
    }    
    else{
        C1CTRLbits.ABAT=1;      
        C1INTFbits.RX1IF=0;
        C1RX1CONbits.RXFUL=0;
        C1INTFbits.WAKIF=0;
        C1INTF=0;
        C1INTFbits.ERRIF=0;

    }

    IFS1bits.C1IF=0;
}



void  __attribute__((section(".usercode")))  Can1Init(void)
{
	unsigned int 	pdid;
	unsigned long 	myid;

    
    C1CTRLbits.REQOP    = 0x4;      
    C1CTRLbits.CANCAP   = 0x0;       

    C1CTRLbits.CSIDL    = 0x1;      
    C1CTRLbits.ABAT     = 0x1;      
    C1CTRLbits.CANCKS   = 0x1;      

    C1CFG2bits.SAM      = 0x1 ;     
    C1CFG2bits.WAKFIL   = 0x0 ; 	
    C1CFG2bits.SEG2PHTS = 0x1 ;     

    C1CFG1bits.BRP      = 19;      
  
    C1CFG1bits.SJW      = 0x0;      
    C1CFG2bits.SEG1PH   = 0x1 ;     
    C1CFG2bits.PRSEG    = 0x2 ;     
    C1CFG2bits.SEG2PH   = 0x1 ; 	

    C1TX0CON            = 0x3;
    C1TX1CON            = 0x3;

    C1TX0SIDbits.TXIDE  = 0x1;
    C1TX0SIDbits.SRR    = 0x0;
    C1TX0DLCbits.TXRB0  = 0x0;
    C1TX0DLCbits.TXRB1  = 0x0;
    C1TX0DLCbits.TXRTR  = 0x0;

    C1TX1SIDbits.TXIDE  = 0x1;
    C1TX1SIDbits.SRR    = 0x0;
    C1TX1DLCbits.TXRB0  = 0x0;
    C1TX1DLCbits.TXRB1  = 0x0;
    C1TX1DLCbits.TXRTR  = 0x0;

    C1TX2SIDbits.TXIDE  = 0x1;
    C1TX2SIDbits.SRR    = 0x1;
    C1TX2DLCbits.TXRB0  = 0x0;
    C1TX2DLCbits.TXRB1  = 0x0;
    C1TX2DLCbits.TXRTR  = 0x0;


    C1RXM0SID           = 0x0;
    C1RXM0EIDH          = 0x0;
    C1RXM0EIDL          = 0x0;
    C1RXM0SIDbits.MIDE  = 0x0;

    C1RXM1SID           = 0x0;
    C1RXM1EIDH          = 0x0;
    C1RXM1EIDL          = 0x0;
    C1RXM1SIDbits.MIDE  = 0x0;

    C1RXF0SID           = 0x0;
    C1RXF0EIDH          = 0x0;
    C1RXF0EIDL          = 0x0;
    C1RXF0SIDbits.EXIDE = 0x0;

    C1RXF1SID           = 0x0;
    C1RXF1EIDH          = 0x0;
    C1RXF1EIDL          = 0x0;
    C1RXF1SIDbits.EXIDE = 0x0;

    C1RXF2SID           = 0x0;
    C1RXF2EIDH          = 0x0;
    C1RXF2EIDL          = 0x0;
    C1RXF2SIDbits.EXIDE = 0x0;

    C1RXF3SID           = 0x0;
    C1RXF3EIDH          = 0x0;
    C1RXF3EIDL          = 0x0;
    C1RXF3SIDbits.EXIDE = 0x0;

    C1RXF4SID           = 0x0;
    C1RXF4EIDH          = 0x0;
    C1RXF4EIDL          = 0x0;
    C1RXF4SIDbits.EXIDE = 0x0;

    C1RXF5SID           = 0x0;
    C1RXF5EIDH          = 0x0;
    C1RXF5EIDL          = 0x0;
    C1RXF5SIDbits.EXIDE = 0x0;

    C1RX0CONbits.RXFUL  = 0x0;
    C1RX0CONbits.DBEN   = 0x0;
    C1RX0CONbits.JTOFF  = 0x0;
    C1RX0CONbits.FILHIT0= 0x0;

    C1RX1CONbits.RXFUL  = 0x0;

    C1INTE              = 0x0;
    C1INTEbits.ERRIE    = 0x1;
    C1INTEbits.RX0IE    = 0x1;
    C1INTEbits.RX1IE    = 0x1;



	Can1RxFilterSet(0,0xffff, 0xffffff);
	Can1RxFilterSet(1,0xffff, 0xffffff);
	Can1RxFilterSet(2,0xffff, 0xffffff);
	Can1RxFilterSet(3,0xffff, 0xffffff);
	Can1RxFilterSet(4,0xffff, 0xffffff);
	Can1RxFilterSet(5,0xffff, 0xffffff);

	Can1RxMaskSet(1,0xffff, 0xffffff);
	Can1RxMaskSet(0,0xffff, 0xffffff);


/*
	pdid=(unsigned int)(cF_FLRDSPCH(CAN1_MY_PRODUCT_ID));
	myid=(unsigned long)(cF_FLRDSPCH(CAN1_MY_ADDRESS));
	myid=(unsigned long)(myid << 8);

	Can1RxFilterSet(0,pdid,myid);
	Can1RxMaskSet(0,0x007f,0x0000ff00);
*/

//	Can1RxFilterSet(0,0x0000,0x00000000);
//	Can1RxMaskSet(0,0x0000,0x00000000);


    IEC1bits.C1IE       = 0x1;
    IPC6bits.C1IP=4;
    C1CTRLbits.REQOP    = 0x0;
    C1EC=0;

}
