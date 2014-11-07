
#include  "ComPort1.h"
#include  "ComPort2.h"
#include  "you_can1.h"
#include  "you_can2.h"
#include  "memory_map.h"
#include  "WorkingRam.h"
#include  "dio.h"
#include  "prj_app.h"
#include  "xesdots.h"
#include  "xkeysort.h"
#include  "xkeysort.h"

////////////#include  "xyou_can.h"
#include  "you_can1.h"

unsigned    int  	Crc;  


void    Crc_Calulate(unsigned int crcdata)
{
	register unsigned int    i;

   Crc= Crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (Crc & 0x0001) == 0x0001)    Crc=(Crc >> 1) ^ 0xA001;
      else Crc=Crc >> 1;
   }
}


void  __attribute__((section(".usercode"))) Com1xHomeCallAck(void)
{
	unsigned char i;

	Com1TxThisPt=0;
	Com1TxCnt=19;

	Com1RxBuffer[0]=((unsigned char)(HOMECALL_PC_ADDRESS));
	Com1RxBuffer[1]=(unsigned char)(CommonBuf[0]);
//	Com1RxBuffer[1]=0x01;
	Com1RxBuffer[2]=((unsigned char)(CommonBuf[2] + 0x80));
	Com1RxBuffer[3]=0x0d;
	Com1RxBuffer[4]= ((unsigned char)(HOMECALL_BUTTON_SET_CMD));

	Com1RxBuffer[5]=HomeCallUpKey[0];
	Com1RxBuffer[6]=HomeCallUpKey[1];
	Com1RxBuffer[7]=HomeCallUpKey[2];
	Com1RxBuffer[8]=HomeCallUpKey[3];

	Com1RxBuffer[9]=HomeCallDnKey[0];
	Com1RxBuffer[10]=HomeCallDnKey[1];
	Com1RxBuffer[11]=HomeCallDnKey[2];
	Com1RxBuffer[12]=HomeCallDnKey[3];

	Com1RxBuffer[13]=HomeCallCarKey[0];
	Com1RxBuffer[14]=HomeCallCarKey[1];
	Com1RxBuffer[15]=HomeCallCarKey[2];
	Com1RxBuffer[16]=HomeCallCarKey[3];

    Crc=0xffff;
    for(i=0;i< Com1RxBuffer[3]+4;i++){
        Crc_Calulate( (unsigned int)(Com1RxBuffer[i]) );
    }

	Com1RxBuffer[17]=(unsigned char)(Crc);
	Com1RxBuffer[18]=(unsigned char)(Crc >> 8);

	Com1TxStart();
}



void  __attribute__((section(".usercode"))) Com2xHomeCallAck(void)
{
	unsigned char i;


	Com2TxThisPt=0;
	Com2TxCnt=19;

	Com2RxBuffer[0]=((unsigned char)(HOMECALL_PC_ADDRESS));
	Com2RxBuffer[1]=(unsigned char)(CommonBuf[0]);
	Com2RxBuffer[2]=((unsigned char)(CommonBuf[2] + 0x80));
	Com2RxBuffer[3]=0x0d;
	Com2RxBuffer[4]=((unsigned char)(HOMECALL_BUTTON_SET_CMD));

	Com2RxBuffer[5]=HomeCallUpKey[0];
	Com2RxBuffer[6]=HomeCallUpKey[1];
	Com2RxBuffer[7]=HomeCallUpKey[2];
	Com2RxBuffer[8]=HomeCallUpKey[3];

	Com2RxBuffer[9]=HomeCallDnKey[0];
	Com2RxBuffer[10]=HomeCallDnKey[1];
	Com2RxBuffer[11]=HomeCallDnKey[2];
	Com2RxBuffer[12]=HomeCallDnKey[3];

	Com2RxBuffer[13]=HomeCallCarKey[0];
	Com2RxBuffer[14]=HomeCallCarKey[1];
	Com2RxBuffer[15]=HomeCallCarKey[2];
	Com2RxBuffer[16]=HomeCallCarKey[3];

    Crc=0xffff;
    for(i=0;i< Com2RxBuffer[3]+4;i++){
        Crc_Calulate( (unsigned int)(Com2RxBuffer[i]) );
    }

	Com2RxBuffer[17]=(unsigned char)(Crc);
	Com2RxBuffer[18]=(unsigned char)(Crc >> 8);

	Com2TxStart();
}



unsigned int   __attribute__((section(".usercode"))) HomeCallButtonCallCmd(unsigned int ComNm)
{
	unsigned char i,j,k,value_bit,button;

	if(CommonBuf[4] == ((unsigned char)(HOMECALL_BUTTON_SET_CMD))){	
		k= (CommonBuf[6] & 0x3f);
		button=k;

		value_bit=0x01;

		k--;
		i=(k / 8);
		j=(k % 8);

		value_bit =(value_bit << j);

		if(CommonBuf[6] & 0x80){
			HomeCallUpKey[i] = (HomeCallUpKey[i] | value_bit);	
			ToHibButtonAlarm(button,1);
		}
		if(CommonBuf[6] & 0x40){
			HomeCallDnKey[i] = (HomeCallDnKey[i] | value_bit);	
			ToHibButtonAlarm(button,2);
		}

		if(ComNm==1)		Com1xHomeCallAck();
		else if(ComNm==2)	Com2xHomeCallAck();
	}
}


void  __attribute__((section(".usercode"))) ComNormalDataReturn(unsigned char *buf)
{
    unsigned int   j,IdBfPt; 

	IdBfPt=IsBufferPt(CommonBuf[0]);

	*(buf + 0)=0xfe;
	*(buf + 1)=CommonBuf[0];
	*(buf + 2)=0xa3;
	*(buf + 3)=DATA_LENGTH;

	for(j=0;j<DATA_LENGTH;j++){
		*(buf + 4 + j)=RcvBuf[IdBfPt + j];
	}	


    Crc=0xffff;
    for(j=0;j<(*(buf + 3) +4);j++){
        Crc_Calulate((unsigned int)(*(buf+j)));
    }
    *(buf+j+0)=(unsigned char)(Crc & 0x00ff);
    *(buf+j+1)=(unsigned char)((Crc >> 8) & 0x00ff);
    *(buf+j+2)=0;
}


void __attribute__((section(".usercode"))) HomeCall_NormalDataReturn(unsigned int ComNm)
{
	if(ComNm==1){
		ComNormalDataReturn( &Com1RxBuffer[0]);
		Com1TxThisPt=0;
		Com1TxCnt=DATA_LENGTH+6;
		Com1TxStart();
	}
	else if(ComNm==2){
		ComNormalDataReturn( &Com2RxBuffer[0]);
		Com2TxThisPt=0;
		Com2TxCnt=DATA_LENGTH+6;
		Com2TxStart();
	}
}



void __attribute__((section(".usercode"))) HomeCallControlCmd(unsigned int ComNm)
{
	unsigned int i;

    if((CommonBuf[4]    ==   (PC_COMMAND | CMD_PARKING)) && (CommonBuf[5] == 0x00)){
		TmpCan1TxSidEidLoad();
		for(i=0;i<8;i++)	Can1TxBuf[i]=CommonBuf[2+i];
		if( !Can1TxData(0,0)){
			CommonBuf[2]=0;
		}
		HomeCall_NormalDataReturn(ComNm);
    }    
    else if((CommonBuf[4]    ==   (PC_COMMAND | CMD_FIRE)) && (CommonBuf[5] == 0x00)){
		TmpCan1TxSidEidLoad();
		for(i=0;i<8;i++)	Can1TxBuf[i]=CommonBuf[2+i];
		if( !Can1TxData(0,0)){
			CommonBuf[2]=0;
		}
		HomeCall_NormalDataReturn(ComNm);
    }
    else if((CommonBuf[4]    ==   (PC_COMMAND | CMD_FAMILY_SEV)) && (CommonBuf[5] == 0x00)){
		TmpCan1TxSidEidLoad();
		for(i=0;i<8;i++)	Can1TxBuf[i]=CommonBuf[2+i];
		if( !Can1TxData(0,0)){
			CommonBuf[2]=0;
		}
		HomeCall_NormalDataReturn(ComNm);
    }
    else if((CommonBuf[4]    ==   (PC_COMMAND | CMD_CALL_SEV)) && (CommonBuf[5] == 0x00)){
		TmpCan1TxSidEidLoad();
		for(i=0;i<8;i++)	Can1TxBuf[i]=CommonBuf[2+i];
		if( !Can1TxData(0,0)){
			CommonBuf[2]=0;
		}
		HomeCall_NormalDataReturn(ComNm);
    }
    else if((CommonBuf[4]    ==   (PC_COMMAND | 0x07)) && (CommonBuf[5] == 0x00)){
//////////		CrtMoveFlr=(unsigned char)(CommonBuf[6] | CAR_READY);
		HomeCallButtonCallCmd(ComNm);
    }

    else if((CommonBuf[4]    ==  CMD_EVEN_ODD) || (CommonBuf[4]    ==  CMD_MORNING)){
		TmpCan1TxSidEidLoad();
		for(i=0;i<8;i++)	Can1TxBuf[i]=CommonBuf[2+i];
		if( !Can1TxData(0,0)){
			CommonBuf[2]=0;
		}
		HomeCall_NormalDataReturn(ComNm);
	}
}





void  __attribute__((section(".usercode"))) HomeCallCmd(unsigned int ComNm)
{
	unsigned char i;

#ifdef	XXXX
	if(ComNm==1){
		Can1MyDestAddr = Com1RxBuffer[0];
		HostSimulationRxCom(Com1RxBuffer,1);
	}
	else if(ComNm==2){
		Can2MyDestAddr = Com2RxBuffer[0];
		HostSimulationRxCom(Com2RxBuffer,2);
	}
#else
	i=(CommonBuf[0] & 0x07);
	if(HostElevLive[i] < 20){
		if(CommonBuf[2] == ((unsigned char)(HOMECALL_READ_CMD))){
			HomeCall_NormalDataReturn(ComNm);
		}
		else if(CommonBuf[2] == ((unsigned char)(HOMECALL_WRITE_CMD))){
			HomeCallControlCmd(ComNm);
		}
	}
#endif

}



void  __attribute__((section(".usercode"))) RxHostData(unsigned int ComNm)
{
	unsigned char tmp_pt;

#ifdef	XXXX
    if(bCan1RxAll){
		tmp_pt=Can1GetSrcAddr();

		if(Can1MyDestAddr == tmp_pt){	
			Can1TxThisPt=0;	
			tmp_pt=Can1TxThisPt;
	
			Can1TxThisPt=tmp_pt;
	        RxCan1AndTxCom1();
	
			Can1TxThisPt=tmp_pt;
			RxCan1AndTxCom2();

		}

		bCan1RxAll=0;
   	}

    if(bCan2RxAll){
/*
		tmp_pt=Can2GetSrcAddr();

		if(Can2MyDestAddr == tmp_pt){		
			tmp_pt=Can2TxThisPt;
	
			Can2TxThisPt=tmp_pt;
	        RxCan2AndTxCom1();
	
			Can2TxThisPt=tmp_pt;
			RxCan2AndTxCom2();
		}
*/
			bCan2RxAll=0;

   	}
#endif


/*
	if(ComNm==1){
		HostSimulationRxCom(Com1RxBuffer,1);
	}
	else if(ComNm==2){
		HostSimulationRxCom(Com2RxBuffer,2);
	}
#else
	i=(CommonBuf[0] & 0x07);
	if(HostElevLive[i] < 20){
		if(CommonBuf[2] == ((unsigned char)(HOMECALL_READ_CMD))){
			HomeCall_NormalDataReturn(ComNm);
		}
		else if(CommonBuf[2] == ((unsigned char)(HOMECALL_WRITE_CMD))){
			HomeCallControlCmd(ComNm);
		}
	}
#endif
*/

}
