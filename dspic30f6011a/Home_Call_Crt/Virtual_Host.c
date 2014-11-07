
// stx + '<' + (r/w) + (u,c,m) + (0,1,f,s) + 
//
//
//
//
//
//
//

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
#include  "prj_app.h"

/////////#include        "xyou_can.h"
#include        "xsetup.h"
#include        "xesdots.h"


_FOSC(CSW_FSCM_OFF & XT_PLL8 & 0xc71f);
_FWDT(WDT_ON & WDTPSA_64 & WDTPSB_1 & 0x803f);
//_FWDT(WDT_OFF & WDTPSA_64 & WDTPSB_1 & 0x803f);
_FBORPOR(PBOR_ON & BORV_27 & PWRT_64 & MCLR_EN & 0x87b3);
_FGS(0x0007);


#define         STX_CHK         0
#define         ADDRESS_CHK     1
#define         COMMAND_CHK     2
#define         DATACNT_CHK     3
#define         DATA_CHK        4
#define         CHKSUM_CHK      5
#define         ETX_CHK         6
#define         RX_ERROR        7
#define         RX_GOOD         8
#define         TX_SET          9
#define         CHKSUM_LOW_CHK  10
#define         CHKSUM_HIGH_CHK 11
#define         TX_ALL          12



#define		VERSION					1

#define		TRUE					1
#define		FALSE					0


extern	unsigned    int    LoopTimer;





/*
#define  	MAX_AIRCON_BUF         	80
#define  	AIRCON_CMD_BUF         	MAX_AIRCON_BUF-8
#define		AIRCON_ADDR_HIGH		0xcf
#define		AIRCON_ADDR_LOW			0xc0


#define		I_AM_HOST			0x2000
#define		HOST_LAST_DATA		0x08
#define		PC_ADDR				0xfe


#define		RCV_DATA			0

*/




/////////////////////////////////
long_field  UserPgmConfigBuf[FLASH_WRITE_BLOCK]; 
long_field  UserPgmConfig[USER_FLASH_BYTE]    	__attribute__((aligned(USER_FLASH_BYTE) , section(".auto_psv,r")));

////////////////////////////////

const unsigned char StatusMessage[50][16]={0};




unsigned long    		Ram_Area1_bit=0;
unsigned long    		Ram_Area2_bit=0;
unsigned long    		Ram_Area3_bit=0;
unsigned long    		Ram_Area4_bit=0;

unsigned    int		msec10=0;
//unsigned    int		msec100=0;


unsigned	char  	AirconBuf[4][MAX_AIRCON_BUF];     



unsigned    char    Can2_Hib_Retun=0;
//unsigned    char    MainTimer=0;
unsigned    char    HostReqAddr=0xff;
unsigned    char    SaveVerify;
unsigned    char    ThisTargetHostAdr,ThisTargetAirconAdr,ThisBufPt=0; 
unsigned    char    Can1VirtualHost=0;

unsigned    char    YouAirconAckTimer[4]={0,0,0,0};



unsigned	char  	CommonBuf[MAX_RTX_BUF];     



///////////베트남 테스트  //////////




//////////////////////////////////

void  __attribute__((section(".usercode"))) flash_write(unsigned int pt)
{

    unsigned int write_verify,y;
    unsigned long src_data,target_data;

	if((SaveVerify == 0x55)){

	    asm("CLRWDT");
	
	    DISABLE_INTERRUPTS;
	
	    do{
	        asm("CLRWDT");
	
	        write_verify=0;
	
	    	// Erase the 32 location row of flash
	    	erase_flash_row((unsigned int)&UserPgmConfig[pt]);
	    	// Write the 32 parameters back 
	    	program_flash((unsigned int)&UserPgmConfig[pt],(unsigned int)&UserPgmConfigBuf[0]);
	        
	        for(y=0;y<16;y++){
	            target_data=UserPgmConfig[pt+y].long_data;    
	            src_data=UserPgmConfigBuf[y].long_data;  
	
	            if(src_data != target_data){  
	                write_verify=1;
	                break;
	            }
	        }                        
	    }while(write_verify);
	
	
	    ENABLE_INTERRUPTS;
	}
    asm("CLRWDT");
}



void  __attribute__((section(".usercode"))) flash_write_Data(unsigned int pt)
{
    unsigned int i;

    i=((pt/64) * 16);
    flash_write(i);
}


unsigned int  __attribute__((section(".usercode"))) ReadInitSetupData(void)
{

    unsigned int  i,j;

     
    if(cF_Version != VERSION){
		SaveVerify = 0x55;


		bitF_HibProtocolCom1_Buf=TRUE;		
		bitF_HibProtocolCom2_Buf=TRUE;
		bitF_HibProtocolCan1_Buf=FALSE;
		bitF_HibProtocolCan2_Buf=FALSE;
		bitF_HibProtocolSet_Buf=FALSE;	
		bitF_VirtualHostSet_Buf=TRUE;
		bitF_IamRepeater_Buf=FALSE;
  		bitF_Sid_Eid_Dl_Buf=FALSE; 



		Set_Byte_Flash_Buf((unsigned int)(cF_Version_A))           	= VERSION;

		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyProductIdValue_A)) = HOMECALL_COMPANY;
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr1Value_A))     = HOMECALL_MY_SET_ADDRESS;  // my set address
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr2Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr3Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr4Value_A))     = 0xfd;

		Set_Byte_Flash_Buf((unsigned int)(cF_Com1MyProductIdValue_A)) = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_Com1MyAddr1Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com1MyAddr2Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com1MyAddr3Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com1MyAddr4Value_A))     = 0xfd;

		Set_Byte_Flash_Buf((unsigned int)(cF_Com2MyProductIdValue_A)) = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_Com2MyAddr1Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com2MyAddr2Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com2MyAddr3Value_A))     = 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_Com2MyAddr4Value_A))     = 0xfd;


		Set_Byte_Flash_Buf((unsigned int)(cF_Can1MyProductIdValue_A))    = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_Can1MyGroupAddrValue_A))    = 0;
		Set_Byte_Flash_Buf((unsigned int)(cF_Can1MyLocalAddrValue_A))    = 0;

		Set_Byte_Flash_Buf((unsigned int)(cF_Can2MyProductIdValue_A))    = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_Can2MyGroupAddrValue_A))    = 0;
		Set_Byte_Flash_Buf((unsigned int)(cF_Can2MyLocalAddrValue_A))    = 0;


		Set_Byte_Flash_Buf((unsigned int)(cF_AirconAdd1_A))    			= 0xc0;
		Set_Byte_Flash_Buf((unsigned int)(cF_AirconAdd2_A))    			= 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_AirconAdd3_A))    			= 0xfd;
		Set_Byte_Flash_Buf((unsigned int)(cF_AirconAdd4_A))    			= 0xfd;

        flash_write_Data((unsigned int)(cF_Version_A));


		Set_Integer_Flash_Buf((unsigned int)(iF_Com1Baudrate_A))           	= BAUD_RATE_X;
		Set_Integer_Flash_Buf((unsigned int)(iF_Com2Baudrate_A))           	= BAUD_RATE_X;


		Set_Integer_Flash_Buf((unsigned int)(iF_Can1Baudrate_A))           	= 0;
		Set_Integer_Flash_Buf((unsigned int)(iF_Can2Baudrate_A))           	= 0;
		Set_Integer_Flash_Buf((unsigned int)(iF_Can1RxSidMask_A))           = 0;
		Set_Integer_Flash_Buf((unsigned int)(iF_Can1RxSidFilter_A))         = 'A';
		Set_Integer_Flash_Buf((unsigned int)(iF_Can2RxSidMask_A))           = 0x7f;
		Set_Integer_Flash_Buf((unsigned int)(iF_Can2RxSidFilter_A))         = 'A';

        flash_write_Data((unsigned int)(iF_Com1Baudrate_A));

		Set_Long_Flash_Buf((unsigned int)(lF_Can1RxEidMask_A))           = 0x0;
		Set_Long_Flash_Buf((unsigned int)(lF_Can1RxEidFilter_A))		 = 0x0;
		Set_Long_Flash_Buf((unsigned int)(lF_Can2RxEidMask_A))           = 0x0;
		Set_Long_Flash_Buf((unsigned int)(lF_Can2RxEidFilter_A))         = 0x0;

        flash_write_Data((unsigned int)(lF_Can1RxEidMask_A));

	}  


	SaveVerify = 0x0;

   	return(0);     
}



unsigned char __attribute__((section(".usercode"))) BitWriteCmd(unsigned char *buf)
{
	unsigned int adr,i,base_adr,j;
	adr=(unsigned int)(*(buf+4) << 8);
	i=(unsigned int)(*(buf+5));
	adr=(adr | i);
	

	if(adr & 0x8000){
		adr=(adr & 0x7fff);

		if((adr + *(buf+3)) > 160)	return(2);

		base_adr=0;

		for(i=0;i<16;i++){
			Set_Long_Flash_Buf(i)=Get_Long_Flash(base_adr+i);
		}

		j = *(buf+3);

		for(i=0;i<j;i++){
			if(*(buf+6+i) == 0xff)	Set_BitNm_Flash_Buf(adr+i)= TRUE;
			else					Set_BitNm_Flash_Buf(adr+i)= FALSE;
		}

		SaveVerify = 0x55;
        flash_write_Data(base_adr);
		SaveVerify = 0x0;

		return(0);
	}
	else{
		return(5);

	}
}


unsigned char __attribute__((section(".usercode"))) BitReadCmd(unsigned char *buf)
{
	unsigned int adr,i,j;
	adr=(unsigned int)(*(buf+4) << 8);
	i=(unsigned int)(*(buf+5));
	adr=(adr | i);
	if(adr & 0x8000){
		adr=(adr & 0x7fff);

		if((adr + *(buf+3)) > 160)	return(2);

		j=*(buf+6);
		for(i=0;i<j;i++){
			if(Get_BitNm_Flash(adr+i) == 1)	*(buf+4+i)= 0xff;
			else							*(buf+4+i)= 0x00;
		}

		return(0);
	}
	else{
		return(5);
	}
}



unsigned char __attribute__((section(".usercode"))) ByteWriteCmd(unsigned char *buf)
{
	unsigned int adr,i,base_adr,j;
	*(buf+3)=(*(buf+3) - 2);
	adr=(unsigned int)(*(buf+4) << 8);
	i=(unsigned int)(*(buf+5));
	adr=(adr | i);
	if(adr & 0x8000){
		adr=(adr & 0x7fff);

		if(adr > 255)	return(2);
		i=(adr % 64) + *(buf+3);
		if(i>=64)		return(2);

		i=(adr / 64);
		base_adr=(i * 64);

		for(i=0;i<64;i++){
			Set_Byte_Flash_Buf((unsigned int)(i))= Get_Byte_Flash((unsigned int)(base_adr+i));;
		}

		j=*(buf+3);
		for(i=0;i<j;i++){
			Set_Byte_Flash_Buf((unsigned int)(adr+i))=*(buf+6+i);
		}

		SaveVerify = 0x55;
        flash_write_Data((unsigned int)(base_adr));
		SaveVerify = 0x0;

		return(0);

	}
	else{
		adr=(adr & 0x7ffff);

		if(adr > 255)	return(2);
		i= *(buf+3);
		if(i>=64)		return(2);

		for(i=0;i<*(buf+3);i++){
			sRamDArry[0][adr + i]=*(buf+6+i);
		}
		return(0);
	}
	return(0);
}


unsigned char __attribute__((section(".usercode"))) ByteReadCmd(unsigned char *buf)
{
	unsigned int adr,i,j;
	adr=(unsigned int)(*(buf+4) << 8);
	i=(unsigned int)(*(buf+5));
	adr=(adr | i);
	if(adr & 0x8000){
		adr=(adr & 0x7fff);

		if(adr > 255)	return(2);
		i= *(buf+3);
		if(i>=64)		return(2);

		j= *(buf+6);
		for(i=0;i<j;i++){
			*(buf+4+i) = Get_Byte_Flash((unsigned int)(adr+i));
		}
		return(0);
	}
	else{
		adr=(adr & 0x7ffff);
		if(adr > 255)	return(2);
		i= *(buf+3);
		if(i>=64)		return(2);

		for(i=0;i<*(buf+6);i++){
			*(buf+4+i)=sRamDArry[0][adr+i];
		}
		return(0);
	}
}

unsigned char __attribute__((section(".usercode"))) SetupCmd(unsigned char *buf,unsigned char com_nm)
{
		
	unsigned char ret=1;
	unsigned char rx_src,rx_dest,cnt,err;
	unsigned int 	i;

	rx_dest= *(buf+0);
	rx_src = *(buf+1);
	if( rx_src != 0xfe)	return(0);  //src address	 	

	switch(*(buf+2)){
		case	0x21:  	// bit read
		    *(buf+0)  	= rx_src;   
		    *(buf+1)  	= rx_dest;
		    *(buf+2)  	= 0xA1;                                        
		    *(buf+3)  	= *(buf+6);                                        
			err=(unsigned char)BitReadCmd(buf);
			if(err){
			    *(buf+2)  	= 0x81;                                        
			    *(buf+3)  	= 0x01;                                        
			    *(buf+4)  	= err;                                        
			}
			ret=0;
			break;
		case	0x22:  	//bit write
			err=(unsigned char)BitWriteCmd(buf);
		    *(buf+0)  = rx_src;   
		    *(buf+1)  = rx_dest;
		    *(buf+2)  = 0xA2;                                        
		    *(buf+3)  = 0x01;                                        
		    *(buf+4)  = err; 

			if(err)	*(buf+2)=0x82;	
                                       
			ret=0;
			break;
		case	0x25:	//byte read
		    *(buf+0)  	= rx_src;   
		    *(buf+1)  	= rx_dest;
		    *(buf+2)  	= 0xA5;                                        
		    *(buf+3)  	= *(buf+6);                                        
			err=(unsigned char)ByteReadCmd(buf);
			if(err){
			    *(buf+2)  	= 0x85;                                        
			    *(buf+3)  	= 0x01;                                        
			    *(buf+4)  	= err;                                        
			}

			ret=0;
			break;
		case	0x26:	//byte write
			err=(unsigned char)ByteWriteCmd(buf);
	
		    *(buf+0)  = rx_src;   
		    *(buf+1)  = rx_dest;
		    *(buf+2)  = 0xA6;                                        
		    *(buf+3)  = 0x1;                                        
		    *(buf+4)  = err;                                        

			if(err)	*(buf+2)=0x82;	

			ret=0;
			break;
	}

	if( !ret ){
	    Crc=0xffff;
	    for(i=0;i< *(buf+3)+4;i++){
	        Crc_Calulate((unsigned int)(*(buf + i)));
	    }
	    *(buf + i)	=(unsigned char)(Crc & 0x00ff);
	    *(buf +i+1)	=(unsigned char)((Crc >> 8) & 0x00ff);


		if(com_nm==1){
			Com1TxCnt=Com1RxBuffer[3]+6;
			Com1RxStatus = TX_SET;
			Com1TxThisPt = 0;
			Com1TxStart();
		}
		else{
			Com2TxCnt=Com2RxBuffer[3]+6;
			Com2RxStatus = TX_SET;
			Com2TxThisPt = 0;
			Com2TxStart();
		}
		return(0);
	}


	return(ret);
}

/////////////////////////////////
//////////////////////////////////////////////////////////////////////////



/*
void    Crc_Calulate(unsigned int crcdata)
{
	register unsigned int    i;

   Crc= Crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (Crc & 0x0001) == 0x0001)    Crc=(Crc >> 1) ^ 0xA001;
      else Crc=Crc >> 1;
   }
}
*/

/*
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


unsigned int   __attribute__((section(".usercode"))) Can2RxDataLoadTmpbuf(unsigned char *buf)
{
	*(buf+0)=(unsigned char)C2RX0B1;		
	*(buf+1)=(unsigned char)(C2RX0B1 >> 8);		
	*(buf+2)=(unsigned char)C2RX0B2;		
	*(buf+3)=(unsigned char)(C2RX0B2 >> 8);		
	*(buf+4)=(unsigned char)C2RX0B3;		
	*(buf+5)=(unsigned char)(C2RX0B3 >> 8);		
	*(buf+6)=(unsigned char)C2RX0B4;		
	*(buf+7)=(unsigned char)(C2RX0B4 >> 8);		
	return(0);	
}

*/



unsigned char    __attribute__((section(".usercode"))) MyAirconAddrChk(unsigned char ThisRcvAdr)
{
	if(ThisRcvAdr == (unsigned char)cF_AirconAdd1){
		ThisTargetAirconAdr=ThisRcvAdr; 
		ThisTargetHostAdr=(unsigned char)cF_SetMyAddr1Value; 
		ThisBufPt=0; 
		return(0);
	}
	else if(ThisRcvAdr == (unsigned char)cF_AirconAdd2){
		ThisTargetAirconAdr=ThisRcvAdr; 
		ThisTargetHostAdr=(unsigned char)cF_SetMyAddr2Value; 
		ThisBufPt=0; 
		return(0);
	}
	else if(ThisRcvAdr == (unsigned char)cF_AirconAdd3){
		ThisTargetAirconAdr=ThisRcvAdr; 
		ThisTargetHostAdr=(unsigned char)cF_SetMyAddr3Value; 
		ThisBufPt=0; 
		return(0);
	}
	else if(ThisRcvAdr == (unsigned char)cF_AirconAdd4){
		ThisTargetAirconAdr=ThisRcvAdr; 
		ThisTargetHostAdr=(unsigned char)cF_SetMyAddr4Value; 
		ThisBufPt=0; 
		return(0);
	}
	else	return(1);
}

unsigned char  __attribute__((section(".usercode")))  MySetAddrChk(unsigned char ThisRcvAdr)
{

	if(ThisRcvAdr == (unsigned char)cF_SetMyAddr1Value){
		ThisTargetHostAdr=ThisRcvAdr; 
		ThisTargetAirconAdr=(unsigned char)cF_AirconAdd1; 
		ThisBufPt=0; 
		return(0);
	}
	else if(ThisRcvAdr == cF_SetMyAddr2Value){
		ThisTargetHostAdr=ThisRcvAdr; 
		ThisTargetAirconAdr=(unsigned char)cF_AirconAdd2; 
		ThisBufPt=1; 
		return(0);
	}
	else if(ThisRcvAdr == cF_SetMyAddr3Value){
		ThisTargetHostAdr=ThisRcvAdr; 
		ThisTargetAirconAdr=(unsigned char)cF_AirconAdd3; 
		ThisBufPt=2; 
		return(0);
	}
	else if(ThisRcvAdr == cF_SetMyAddr4Value){
		ThisTargetHostAdr=ThisRcvAdr; 
		ThisTargetAirconAdr=(unsigned char)cF_AirconAdd4; 
		ThisBufPt=3; 
		return(0);
	}
	else	return(100);

}

unsigned char   __attribute__((section(".usercode"))) Can1GetSrcAddr(void)
{
	unsigned char src_adr1,src_adr2;

	src_adr1=(unsigned char)( (Can1RxSid >> 7) & 0x0f);
	if(Can1RxEid & 0x20000)	src_adr1=(src_adr1 | 0x10);	
	
	src_adr1=(src_adr1 << 3);	

	if(Can1RxEid & 0x4000)	src_adr1=(src_adr1 | 0x01);	
	if(Can1RxEid & 0x8000)	src_adr1=(src_adr1 | 0x02);	
	if(Can1RxEid & 0x10000)	src_adr1=(src_adr1 | 0x04);	

/*
	src_adr2=(unsigned char)((Can1RxEid >> 14) & 0x03);
	if(Can1RxEid & 0x10000)	src_adr1=(src_adr1 | 0x40);	
	if(Can1RxEid & 0x20000)	src_adr1=(src_adr1 | 0x80);	

	src_adr1=(src_adr1 | src_adr2);
*/

	return(src_adr1);
}



unsigned char  __attribute__((section(".usercode")))  Can2GetSrcAddr(void)
{
	unsigned char src_adr1,src_adr2;

	src_adr1=(unsigned char)((Can2RxSid >> 5) & 0x3c);
	src_adr2=(unsigned char)((Can2RxEid >> 14) & 0x03);
	if(Can2RxEid & 0x10000)	src_adr1=(src_adr1 | 0x40);	
	if(Can2RxEid & 0x20000)	src_adr1=(src_adr1 | 0x80);	

	src_adr1=(src_adr1 | src_adr2);

	return(src_adr1);
}







unsigned char   __attribute__((section(".usercode"))) Can2GetAirconSrcAddr(void)
{	
	unsigned char srcadr;
	srcadr=(unsigned char)Can2RxEid;
	if(srcadr >= AIRCON_ADDR_LOW){
		if(srcadr <= AIRCON_ADDR_HIGH){
			if( !MyAirconAddrChk(srcadr)){
				return(0);
			}
		}
	}
	return(1);
}

//////////////////////////Host Sim Hib Start(Can1)///////////////////////////////////





void  __attribute__((section(".usercode"))) ElevDataLoad(unsigned char *buf)
{
	unsigned char i=0;

	if(bitF_VirtualHostSet == TRUE){
		*(buf+0)	=(sRamDArry[ThisBufPt][S0_FLOOR]);
		*(buf+1)	= sRamDArry[ThisBufPt][S1_STATE];
		*(buf+2)	= sRamDArry[ThisBufPt][S2_STATE];
		*(buf+3)	= sRamDArry[ThisBufPt][S3_STATE];
		*(buf+4)	= sRamDArry[ThisBufPt][S4_STATE];
		*(buf+5)	=(sRamDArry[ThisBufPt][DEST_FLR]);
		*(buf+6)	= sRamDArry[ThisBufPt][DSP1];
		*(buf+7)	= sRamDArry[ThisBufPt][DSP2];
	
	    *(buf+8)	= sRamDArry[ThisBufPt][FLR_ON_OFF0];
	    *(buf+9) 	= sRamDArry[ThisBufPt][FLR_ON_OFF1];
	    *(buf+10) 	= sRamDArry[ThisBufPt][FLR_ON_OFF2];
	    *(buf+11) 	= sRamDArry[ThisBufPt][FLR_ON_OFF3];     
	    *(buf+12) 	= sRamDArry[ThisBufPt][mEqualFloor];
	    *(buf+13) 	= sRamDArry[ThisBufPt][mHighFloor];
	    *(buf+14) 	= sRamDArry[ThisBufPt][mLowFloor];     
	    *(buf+15)	= sRamDArry[ThisBufPt][mDoor];
	
		*(buf+16)	= sRamDArry[ThisBufPt][mCarOpCl];
		*(buf+17)	= sRamDArry[ThisBufPt][mCarKey1];
		*(buf+18)	= sRamDArry[ThisBufPt][mCarKey9];
		*(buf+19)	= sRamDArry[ThisBufPt][mCarKey17];     
		*(buf+20)	= sRamDArry[ThisBufPt][mCarKey25];
		*(buf+21)	= sRamDArry[ThisBufPt][mSysStatus];
		*(buf+22)	= sRamDArry[ThisBufPt][O_OP];
		*(buf+23)	= sRamDArry[ThisBufPt][O_U_W];
		
		*(buf+24)	= sRamDArry[ThisBufPt][O_Y_0];
		*(buf+25)	= sRamDArry[ThisBufPt][I_EMG];
		*(buf+26)	= sRamDArry[ThisBufPt][I_SU1];
		*(buf+27)	= sRamDArry[ThisBufPt][I_GR];     
		*(buf+28)	= sRamDArry[ThisBufPt][I_FIRE];
		*(buf+29)	= sRamDArry[ThisBufPt][mMostLongDst];
		*(buf+30)	= 0;
		*(buf+31)	= 32; //top floor

		*(buf+32)	= (unsigned char)mpm;
		*(buf+33)	= (unsigned char)(mpm >> 8);

		*(buf+34)	= sRamDArry[ThisBufPt][mUnKnown23];
		*(buf+35)	= sRamDArry[ThisBufPt][I_X_0];     
		*(buf+36)	= sRamDArry[ThisBufPt][I_FS0];
		*(buf+37)	= sRamDArry[ThisBufPt][mUnKnown26];
		*(buf+38)	= sRamDArry[ThisBufPt][mUnKnown27];
		*(buf+39)	= sRamDArry[ThisBufPt][mCallMe];
	}
	else{
		for(i=0;i<40;i++)	*(buf+i)	= sRamDArry[ThisBufPt][0+i];
	}

	for(i=0;i<48;i++){
		*(buf+40+i)	= AirconBuf[ThisBufPt][i+4];
	}
}



unsigned int   __attribute__((section(".usercode"))) Can1TxSimulationData(void)
{
	Can1VirtualHost=ThisTargetHostAdr;

	Can1TxDlc=8;
	Can1DataSeq=0;
	ElevDataLoad(Can1TxBuf);
//	Can1_Hib_Retun=0;
	bCan1TxAct=1;
}


unsigned int   __attribute__((section(".usercode"))) Can2TxSimulationData(void)
{
	Can2TxDlc=8;
	Can2DataSeq=0;
	ElevDataLoad(Can2TxBuf);
	Can2_Hib_Retun=0;
	bCan2TxAct=1;
}




unsigned int   __attribute__((section(".usercode"))) Can1RxAndCan2TxAirconCmd(void)
{
	unsigned char i;
	
	if(Can1RxBuf[0] & 0x40){
		for(i=0;i<8;i++)	AirconBuf[ThisBufPt][AIRCON_CMD_BUF+i]=Can1RxBuf[i];
		Can2TxSimulationData();
		Can2_Hib_Retun=1;
	}
}






//////////////// CAN1 /////////////////////////////////////






unsigned int   __attribute__((section(".usercode"))) Can1AckMyData(unsigned char thisadr)
{
	unsigned char i;

	if(YouAirconAckTimer[0] > 100)	return(0);


	if(Can1RxBuf[0]==0x24){
		for(i=0;i<8;i++)	Can2TxBuf[i] = Can1RxBuf[i];

		Can2TxDlc=8;
		Can2TxSid=(unsigned int)cF_SetMyProductIdValue;
		Can2TxEid=0xfe;

		if(thisadr & 0x01)	Can2TxEid=(Can2TxEid | 0x004000);				
		if(thisadr & 0x02)	Can2TxEid=(Can2TxEid | 0x008000);
		if(thisadr & 0x40)	Can2TxEid=(Can2TxEid | 0x010000);				
		if(thisadr & 0x80)	Can2TxEid=(Can2TxEid | 0x020000);
					
		if(thisadr & 0x04)	Can2TxSid=(Can2TxSid | 0x0080);				
		if(thisadr & 0x08)	Can2TxSid=(Can2TxSid | 0x0100);				
		if(thisadr & 0x10)	Can2TxSid=(Can2TxSid | 0x0200);				
		if(thisadr & 0x20)	Can2TxSid=(Can2TxSid | 0x0400);				

		Can2TxData(0);	

#ifdef	COM_CRT
		PLCCmdSort();
#endif


	}
	else if(Can1RxBuf[0] & 0x40){
		Can1RxAndCan2TxAirconCmd();
	}

	Can1TxSimulationData();

	return(0);
}


/*
unsigned int   __attribute__((section(".usercode"))) Can1ReceiveData(void)
{
	unsigned char	j,hostnm;
	unsigned char	tmpbuf[10];

	if(bitF_IamRepeater){
		bCan2TxStart=1;
		TxCan2Buf();
	}

	else{
		if(Can1RxEid & I_AM_HOST){                                 //host board                 
			bCan1RxGood=1;                    
			bCan1RxAll=0;                    
			
			j=(unsigned char)(Can1RxEid & 0x0f);	
			
			hostnm=(unsigned char)(j * 8);
	
			if(j==0){
				Can1RxCnt=0;
				Can1RxThisPt=0;
				Can1TxCnt=0;
				Can1TxThisPt=0;
			}
			else{

				if(bitF_HibProtocolCan1){
					j=(Can1RxCnt + Can1RxDlc);
					if(j >= 40){
						bCan1RxAll=1;
					}
				}
				else{
					j=(Can1RxCnt + Can1RxDlc);
					if(j >= 88){
						bCan1RxAll=1;
					}
				}
				
			}
	
			Can1RxDataLoadTmpbuf(tmpbuf);
	
			for(j=0;j<Can1RxDlc;j++){
				if((hostnm+j) < (CAN1_MAX_SAVE_BUF-1)){
	    			Can1RxBuf[hostnm+j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
				}	
			}
			
			Can1RxCnt=(Can1RxCnt + Can1RxDlc);
			Can1RxThisPt=Can1RxCnt;
	
		}
	
	
		else{
			j=Can1GetSrcAddr();
			if( !MySetAddrChk(j)){
				Can1RxDataLoadTmpbuf(tmpbuf);		
				for(j=0;j<Can1RxDlc;j++){
			   		Can1RxBuf[j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
				}
				Can1AckMyData(ThisTargetHostAdr);
			}
		}
	}
	return(0);
}

*/


//////////////// CAN2 /////////////////////////////////////
//////////////// CAN2 /////////////////////////////////////
//////////////// CAN2 /////////////////////////////////////


void __attribute__((section(".usercode")))  Can2AirConData(void) 
{
	unsigned char AirConDataRcvPt;

    AirConDataRcvPt=(unsigned char)((Can2RxSid >> 7) & 0x000f);
	AirConDataRcvPt=(AirConDataRcvPt * 8);

	Can2RxDataLoadTmpbuf( &AirconBuf[ThisBufPt][AirConDataRcvPt]);
}



unsigned int   __attribute__((section(".usercode"))) Can2AckMyData(void)
{
	unsigned char i;

	if( (Can2RxBuf[0]==0x23) && (Can2RxBuf[2] == 0x00) && (Can2RxBuf[3] == 0x00)){
		Can2DataSeq = 0;	
		ElevDataLoad(Can2TxBuf);
		bCan2TxAct=1;
	}
	return(0);
}



/*
///////////
unsigned int   __attribute__((section(".usercode"))) Can2ReceiveData(void)
{


	unsigned char	j,hostnm;
	unsigned char	tmpbuf[10];

	if(bitF_IamRepeater){
		bCan1TxStart=1;
		TxCan1Buf();
	}

	else{
		j=bitF_VirtualHostSet;
		if( (Can2RxEid & I_AM_HOST) && (bitF_VirtualHostSet)){                                 //other host write to status .. iam virtual host and converter                  		
			j=Can2GetSrcAddr();
			if(MySetAddrChk(j)){
				return(0);
			}
	
			bCan2RxGood=1;                    
			bCan2RxAll=0;                    
			
			j=(unsigned char)(Can2RxEid & 0x0f);
			
			hostnm=(unsigned char)(j * 8);
			if(j==0){
				Can2RxCnt=0;
				Can2RxThisPt=0;
				Can2TxCnt=0;
				Can2TxThisPt=0;
			}
	
			else{

				if(bitF_HibProtocolCan2){
					j=(Can2RxCnt + Can2RxDlc);
					if(j >= 40){
						bCan2RxAll=1;
					}
				}
				else{
					j=(Can2RxCnt + Can2RxDlc);
					if(j >= 88){
						bCan2RxAll=1;
					}
				}



			}
		
			Can2RxDataLoadTmpbuf(tmpbuf);
	
			for(j=0;j<Can2RxDlc;j++){
	    		Can2RxBuf[hostnm+j+RCV_DATA]=tmpbuf[j];   // Buff배열에 있는 데이터를 TXB0D0~D7까지 차례로 저장 
			}
			
			Can2RxCnt=(Can2RxCnt + Can2RxDlc);
			Can2RxThisPt=Can2RxCnt;
	
	
			if(bCan2RxAll){	
				YouAirconAckTimer[0]=0;

//				YouHostAckTimer[ThisBufPt]=0;
				for(j=0;j<Can2RxCnt;j++)	sRamDArry[ThisBufPt][j]=Can2RxBuf[j];
			}
		}
	
		else{
			if(!Can2GetAirconSrcAddr()){
				Can2AirConData();

				YouAirconAckTimer[0]=0;
//				YouHostAckTimer[ThisBufPt]=0;
			}
		}
	}

	return(0);
}

*/


//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////


/*
unsigned int   __attribute__((section(".usercode"))) Com1ReceiveData(unsigned char rdata)
{


   	unsigned char   buf;
   	unsigned char   temp;

//	Com1RxBuffer[Com1RxCnt]=rdata;
	
   	buf=rdata;


    if(Com1RxStatus != TX_SET){   

        Com1RxBuffer[Com1RxCnt]=buf;

        if(Com1RxCnt < (MAX_RTX_BUF-2)){
            Com1RxCnt++;
        }
        else{
            Com1RxCnt=0;
            Com1RxBuffer[Com1RxCnt]=buf;
        }


#ifdef	COM_CRT
		if(Com1RxBuffer[0] != 'S'){
			Com1RxCnt=1;		
			Com1RxBuffer[0]=rdata;
		}
		else if( (rdata == 'E') && (Com1RxCnt >= 35)){
			for(buf=0;buf<MAX_RTX_BUF;buf++)		Com2RxBuffer[buf]=Com1RxBuffer[buf];
			Com2RxCnt=Com1RxCnt;			
			PLCAck(rdata);
	
			Com1RxCnt=0;
		}
		else if(Com1RxCnt >= 38){
				Com1RxCnt=0;		
		}		
#else
		if(bitF_IamRepeater){
		    if(Com1RxStatus==RX_GOOD){

///////////////////				Com1Rx(rdata);
				SetupCmd(Com1RxBuffer,1);
				Com1RxStatus=STX_CHK;

				Com2TxThisPt=0;
				Com2TxCnt=0;
	
			}
			else{
				Com1Rx(rdata);
			}
		}
#endif
    }

	return(0);
}
*/

//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////







void  __attribute__((section(".usercode")))  RxCan1AndTxCom1(void)
{
	unsigned char i,src_adr;
  


	if(Can1TxThisPt == 0){
		src_adr=Can1GetSrcAddr();

	    Com1RxBuffer[0]  = PC_ADDR;   
	    Com1RxBuffer[1]  = src_adr;
	    Com1RxBuffer[2]  = (Com1RxBuffer[2] + 0x80);                                        
		

		if(bitF_HibProtocolCom1){
			Com1RxBuffer[3]  = 40;
		}
		else{
			Com1RxBuffer[3]  = 88;
		}

//Com1RxBuffer[3]=Can1RxThisPt;
		Com1TxCnt=0;
		Com1TxThisPt=0;
	}


    for(;(Can1RxThisPt > Can1TxThisPt);Can1TxThisPt++){
        Com1RxBuffer[4+Can1TxThisPt]= Can1RxBuf[Can1TxThisPt];
    }

	Com1TxCnt = (4 + Can1TxThisPt);

	if(bCan1RxAll){
	    Crc=0xffff;
	    for(i=0;i<Com1RxBuffer[3]+4;i++){
	        Crc_Calulate((unsigned int)Com1RxBuffer[i]);
	    }
	    Com1RxBuffer[i]=(unsigned char)(Crc & 0x00ff);
	    Com1RxBuffer[i+1]=(unsigned char)((Crc >> 8) & 0x00ff);
		Com1TxCnt=Com1RxBuffer[3]+6;
		HostReqAddr=0xff;

		Com1TxStart();

	}


	if((Com1RxStatus != TX_SET) && (Com1TxCnt != Com1TxThisPt)){
////		Com1TxStart();
	}

}



void   __attribute__((section(".usercode"))) RxCan1AndTxCom2(void)
{
	unsigned char i,src_adr;
       
	if(Can1TxThisPt == 0){
		src_adr=Can1GetSrcAddr();

	    Com2RxBuffer[0]  = PC_ADDR;   
	    Com2RxBuffer[1]  = src_adr;
	    Com2RxBuffer[2]  = 0xA3;                                        

		if(bitF_HibProtocolCom2){
			Com2RxBuffer[3]  = 40;
		}
		else{
			Com2RxBuffer[3]  = 88;
		}

		Com2TxCnt=0;
		Com2TxThisPt=0;
	}

    for(;(Can1RxThisPt > Can1TxThisPt);Can1TxThisPt++){
        Com2RxBuffer[4+Can1TxThisPt]= Can2RxBuf[Can1TxThisPt];
    }

	Com2TxCnt = (4 + Can1TxThisPt);

	if(bCan1RxAll){
	    Crc=0xffff;
	    for(i=0;i<Com2RxBuffer[3]+4;i++){
	        Crc_Calulate((unsigned int)Com2RxBuffer[i]);
	    }
	    Com2RxBuffer[i]=(unsigned char)(Crc & 0x00ff);
	    Com2RxBuffer[i+1]=(unsigned char)((Crc >> 8) & 0x00ff);

		Com2TxCnt=Com2RxBuffer[3]+6;
		HostReqAddr=0xff;
	}


	if((Com2RxStatus != TX_SET) && (Com2TxCnt != Com2TxThisPt)){
		Com2TxStart();
	}
}



/*
unsigned int   __attribute__((section(".usercode"))) HostSimulationRxCom(unsigned char *buf,unsigned char com_nm)
{
}
*/




unsigned int   __attribute__((section(".usercode"))) HostSimulationRxCom(unsigned char *buf,unsigned char com_nm)
{
	unsigned char i,pt;


	HostReqAddr=*(buf+0);


	if( !MySetAddrChk(*(buf+0)) ){

/*
		if(YouAirconAckTimer[ThisBufPt] > 50){
			if(com_nm==1)	Com1RxStatus=STX_CHK;
			else			Com2RxStatus=STX_CHK;

			return(0);
		}

*/
	    if((*(buf+2) >= 0x20) && (*(buf+2) <= 0x3f) ){				
	    	if((*(buf+2) >= 0x30) && (*(buf+2) <= 0x3f) ){				
				pt=4;	
				if(*(buf+pt) & 0x40){
					for(i=0;i<8;i++){
						AirconBuf[ThisBufPt][AIRCON_CMD_BUF+i]=*(buf+pt+i);
					}						
					Can2TxSimulationData();
					Can2_Hib_Retun=com_nm;
				}		
			}
	    	else if( (*(buf+2) == 0x24)){				
				pt=2;	

				for(i=0;i<8;i++)	Can2TxBuf[i] = *(buf+pt+i);
	 
				Can2TxDlc=8;
				Can2TxSid=(unsigned int)cF_SetMyProductIdValue;
				Can2TxEid=0xfe;
	
				if(*(buf+0) & 0x01)	Can2TxEid=(Can2TxEid | 0x004000);				
				if(*(buf+0) & 0x02)	Can2TxEid=(Can2TxEid | 0x008000);
				if(*(buf+0) & 0x04)	Can2TxEid=(Can2TxEid | 0x010000);				
				if(*(buf+0) & 0x80)	Can2TxEid=(Can2TxEid | 0x020000);
							
				if(*(buf+0) & 0x08)	Can2TxSid=(Can2TxSid | 0x0080);				
				if(*(buf+0) & 0x10)	Can2TxSid=(Can2TxSid | 0x0100);				
				if(*(buf+0) & 0x20)	Can2TxSid=(Can2TxSid | 0x0200);				
				if(*(buf+0) & 0x40)	Can2TxSid=(Can2TxSid | 0x0400);				
				Can2TxData(0);	
			}
			
		    *(buf+0)  = 0xfe;   
		    *(buf+1)  = ThisTargetHostAdr;
		    *(buf+2)  = 0xA3;

			if(com_nm==1){
				if(bitF_HibProtocolCom1)		*(buf+3)=40;	
				else							*(buf+3)=88;	
			}
			else{
				if(bitF_HibProtocolCom2)		*(buf+3)=40;	
				else							*(buf+3)=88;	
			}

			ElevDataLoad( (buf+4));			

		    Crc=0xffff;
		    for(i=0;i< (*(buf+3)+4);i++){
		        Crc_Calulate((unsigned int)(*(buf+i)));
		    }
		    *(buf+i)=(unsigned char)(Crc & 0x00ff);
		    *(buf+i+1)=(unsigned char)((Crc >> 8) & 0x00ff);
	

			if(com_nm==1){
				Com1TxCnt = (*(buf+3)+6);
				Com1TxThisPt=0;

				if((Com1RxStatus != TX_SET) && (Com1TxCnt != Com1TxThisPt)){
					Com1TxStart();
				}
			}
			else if(com_nm==2){
				Com2TxCnt = (*(buf+3)+6);
				Com2TxThisPt=0;

				if((Com2RxStatus != TX_SET) && (Com2TxCnt != Com2TxThisPt)){
					Com2TxStart();
				}
			}
	    }
	}
	else{		

	    if((*(buf+2) >= 0x20) && (*(buf+2) <= 0x2f)){
			pt=2;
	    }
	    else if((*(buf+2) >= 0x30) && (*(buf+2) <= 0x3f)){
			pt=4;
	    }

		for(i=0;i<8;i++)	Can1TxBuf[i] = *(buf+pt+i); 

		Can1TxDlc=8;
		Can1TxSid=(unsigned int)cF_SetMyProductIdValue;
		Can1TxEid=0xfe;
		

				
		if(*(buf+0) & 0x01)	Can1TxEid=(Can1TxEid | 0x004000);				
		if(*(buf+0) & 0x02)	Can1TxEid=(Can1TxEid | 0x008000);
		if(*(buf+0) & 0x04)	Can1TxEid=(Can1TxEid | 0x010000);				
		if(*(buf+0) & 0x80)	Can1TxEid=(Can1TxEid | 0x020000);
					
		if(*(buf+0) & 0x08)	Can1TxSid=(Can1TxSid | 0x0080);				
		if(*(buf+0) & 0x10)	Can1TxSid=(Can1TxSid | 0x0100);				
		if(*(buf+0) & 0x20)	Can1TxSid=(Can1TxSid | 0x0200);				
		if(*(buf+0) & 0x40)	Can1TxSid=(Can1TxSid | 0x0400);				
		Can1TxData(0);	

		if(com_nm==1)		Com1RxStatus=STX_CHK;	
		else if(com_nm==2)	Com2RxStatus=STX_CHK;	
	}
	
}


void __attribute__((section(".usercode"))) CrtComCheck(void)
{

	unsigned char tmp_pt;


    if(Com1RxStatus==RX_GOOD){
		tmp_pt=SetupCmd(Com1RxBuffer,1);
		if(tmp_pt)	HostSimulationRxCom(Com1RxBuffer,1);

    }             
    else if(Com2RxStatus==RX_GOOD){
		tmp_pt=SetupCmd(Com2RxBuffer,2);
		if(tmp_pt)	HostSimulationRxCom(Com2RxBuffer,2);
    }             


                       
    if(bCan1RxAll){
		tmp_pt=Can1GetSrcAddr();

		if(HostReqAddr == tmp_pt){		
			tmp_pt=Can1TxThisPt;
	
			Can1TxThisPt=tmp_pt;
	        RxCan1AndTxCom1();
	
			Can1TxThisPt=tmp_pt;
			RxCan1AndTxCom2();

			bCan1RxAll=0;
		}
   	}

}

//////////////// CRT COM1 END /////////////////////////////////////








unsigned int   __attribute__((section(".usercode"))) Can1TxCrtData(void)
{

	return(0);
}



unsigned int   __attribute__((section(".usercode"))) Can2TxCrtData(void)
{
    unsigned char i,cnt,tmpCan2DataSeq;
    unsigned long tmpCan2TxEid;


	if(!bCan2TxAct){
		Can2DataSeq=0;
		return(0);
	}

	Can2TxDlc=8;
	cnt=(Can2DataSeq * 8);

	tmpCan2DataSeq=Can2DataSeq;




	if(Can2_Hib_Retun){
		if(Can2DataSeq == 4){
			Can2DataSeq=8;
		}
		else if(Can2DataSeq == 9){
			if(AirconBuf[ThisBufPt][AIRCON_CMD_BUF] == 0){
				bCan2TxAct=0;
				Can2DataSeq = 0;
				Can2_Hib_Retun=0;
				return(0);
			}
			else{
				Can2TxSid=(unsigned int)cF_SetMyProductIdValue;
				Can2TxEid=(unsigned long)(ThisTargetAirconAdr);
				Can2TxEid=(unsigned long)(Can2TxEid << 8);
				for(i=0;i<8;i++){
					Can2TxBuf[i]	= AirconBuf[ThisBufPt][AIRCON_CMD_BUF+i];
				}		
				Can2DataSeq = 5;
				cnt=0;
			}
		}
		else if(Can2DataSeq == 6){
			AirconBuf[ThisBufPt][AIRCON_CMD_BUF] = 0;
			bCan2TxAct=0;
			Can2DataSeq = 0;
			Can2_Hib_Retun=0;
			return(0);
		}
	}



	tmpCan2TxEid=Can2TxEid;
	Can2TxEid=Can2TxEid + (unsigned long)Can2DataSeq;

	if( !Can2TxData(cnt)){
		Can2DataSeq++;
	}
	else{
		Can2DataSeq=tmpCan2DataSeq;
	}

	Can2TxEid=tmpCan2TxEid;


	return(0);
}

    


//////////////////////////Hib END///////////////////////////////////

void  __attribute__((section(".usercode")))   Filter_Mask_Set(void)
{

	Can1RxSidFilter=(unsigned int)(cF_SetMyProductIdValue);
	Can1RxSidFilter=(unsigned int)(cF_SetMyProductIdValue);
	Can1RxEidFilter=0;
	Can1RxSidMask=0x007f;
	Can1RxEidMask=0x0000;


	Can2RxSidFilter=(unsigned int)(cF_SetMyProductIdValue);
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


//////////////////////////Host Sim Hib END///////////////////////////////////


void  __attribute__((section(".usercode")))   Timer1Init(void)
{
     
	T1CON = 0 ;//Turn off Timer1 by clearing control register
    IPC0bits.T1IP=1;
	TMR1 = 0 ;  //Start Timer1 at zero
    PR1=16000;

	T1CON = 0x8000;
    _T1IE = 1;
}



void  __attribute__((section(".usercode")))   LvdInit(void)
{
    LVDIE=0;

    RCON=RCON & 0xc0ff;
    RCON=RCON | 0x1c00;    //4.1v

    LVDIF=0;
    LVDIE=1;
}





int   __attribute__((section(".usercode"))) main(void)
{
    unsigned int i,j;
    unsigned long tmplong;



    PORTB=0xffff;
    PORTC=0xffff;
    PORTD=0xffff;
    PORTF=0xffff;
    PORTG=0xffff;

    TRISB=0xffff;
    TRISC=0xffff;
    TRISD=0xffff;
    TRISF=0xffff;  
    TRISG=0xffff;   
    SPLIM=SPLIM;
    ADPCFG=0xffff;  



	TRIS_RUN_LED=0;		
	TRIS_TXEN_485=0;

	RUN_LED=1;
	TXEN_485=0;
	CAN1_RX=1;
	CAN1_TX=1;
	CAN2_RX=1;
	CAN2_TX=1;

	UART1_RX=1;
	UART1_TX=1;
	UART2_RX=1;
	UART2_TX=1;


/*
	for(j=0;j<4;j++){
		for(i=0;i<(MAX_AIRCON_BUF-1);i++)	AirconBuf[j][i]=0;
	}

*/



	for(i=0;i<(CAN1_MAX_SAVE_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_SAVE_BUF-1);i++)	Can1TxBuf[i]=0;
	for(i=0;i<(CAN2_MAX_SAVE_BUF-1);i++)	Can2RxBuf[i]=0;
	for(i=0;i<(CAN2_MAX_SAVE_BUF-1);i++)	Can2TxBuf[i]=0;


	for(j=0;j<1000;j++){
		asm("CLRWDT");
	}

    asm("CLRWDT");
	ReadInitSetupData();
    asm("CLRWDT");





    INTCON1bits.NSTDIS=1;
    LvdInit();
    Timer1Init();
    Com1Init();
    Com2Init();
    Can1Init();
    Can2Init();
    INTCON1bits.NSTDIS=1;


    if(!SWR){
        asm("RESET");
    }
    SWR=0;

	Filter_Mask_Set();


	Init_Home_Call();

	for(i=0;i<MAX_ELEV;i++)	HostElevLive[i]=100;


//////////////filter mask set/////////////////////////

    do{
#ifdef	XXXX
    for(i=0;i<MAX_ELEV;i++){          
        HostElevLive[i] =0;
    }        
//	CrtComCheck();

#else
	Home_Call();
	Can2TxCheck();
#endif


/*
		if( bitF_IamRepeater){

			if(bCan1TxStart)	Can1TxData(0);
			if(bCan2TxStart)	Can2TxData(0);
	

			Can1TxRepeater();
			Can2TxRepeater();

			Com1TxRepeater();
			Com2TxRepeater();
		}
		else{	
	 		Can1TxCrtData();
	 		Can2TxCrtData();
			CrtComCheck();
		}


//		PLC_Call_Test();

*/


//		HomeCallComPort1Ack();


/*
		if( (Com2RxStatus == TX_SET) && (_U2TRMT)){
			TXEN_485= 0; 		
			Com2RxStatus = STX_CHK;
		}
*/

		if( (Com2TxCnt == Com2TxThisPt) && (_U2TRMT)){
			TXEN_485= 0; 		
			Com2RxStatus = STX_CHK;
		}
		if(Com2RxStatus != TX_SET){
			TXEN_485= 0; 		
		}

        asm("CLRWDT");

   	}while(1);                                  
}




void _ISR _T1Interrupt(void)
{

	unsigned char i;

    if( _T1IF ){

        _T1IF = 0;
        TMR1  = 0;


		if(Can1PollingTimer < 250) 	Can1PollingTimer++;
		if(Can2PollingTimer < 250) 	Can2PollingTimer++;
      	if(Com1SerialTime < 250)	Com1SerialTime++;
      	if(Com2SerialTime < 650)	Com2SerialTime++;
      	if(TXEN485Timer < 250)		TXEN485Timer++;
			
//		WaitTime++;

		msec10++;
        if(msec10 > 10){
            msec10=0;
		}

		msec100++;
        if(msec100 > 100){
            msec100=0;
			RUN_LED= !RUN_LED;


			if(YouAirconAckTimer[0] < 250)	YouAirconAckTimer[0]++;
			if(YouAirconAckTimer[1] < 250)	YouAirconAckTimer[1]++;
			if(YouAirconAckTimer[2] < 250)	YouAirconAckTimer[2]++;
			if(YouAirconAckTimer[3] < 250)	YouAirconAckTimer[3]++;
            if(LoopTimer            < 250)	LoopTimer++; 
 


	///////////////////////////////		NoCanInt++;
			Can1RxLiveCnt++;
/*                       
            for(i=0;i<MAX_ELEV;i++){          
                CanLiveTimeAdr[i]++;
            }        

*/

        
            MainTimer++;
//            BlinkTime++;
  
/*      
            if(UpButtonTime < (BUT_CANLE_T)){
                UpButtonTime++;   
				if(bCurOpenKey){
					if(UpButtonTime > 150)	UpButtonTime=150;
				}
            }
            if(DnButtonTime < (BUT_CANLE_T)){
                DnButtonTime++;
				if(bCurOpenKey){
					if(DnButtonTime > 150)	DnButtonTime=150;
				}
            }
*/
            UpButtonTime=10;
            DnButtonTime=10;


            sec1++;
            if(sec1>10){
                sec1=0;
                for(i=0;i<MAX_ELEV;i++){          
                    MoveTime[i]++;
                }        

	            for(i=0;i<MAX_ELEV;i++){          
	                if(HostElevLive[i] < 100)	HostElevLive[i]++;
	            }        
            }
		}


/*
        shiftTime++;		
        UpKeyBit=0;
        DnKeyBit=0;
*/   
		ExtCallKey=0;	 
	}
}




void _ISR _OscillatorFail(void)
{

    INTCON1bits.OSCFAIL=0;
    asm("RESET");
}


void _ISR _AddressError(void)
{   
    INTCON1bits.ADDRERR=0;
    asm("RESET");
}


void _ISR _StackError(void)
{   
    INTCON1bits.STKERR=0;
    asm("RESET");
}




void _ISR _MathError(void)
{   
    INTCON1bits.MATHERR=0;
    asm("RESET");
}




void _ISR _LVDInterrupt(void)
{
    LVDIF=0;
    asm("RESET");
}




void _ISR _DefaultInterrupt(void)
{
    asm("RESET");
}









