
// stx + '<' + (r/w) + (u,c,m) + (0,1,f,s) + 
//
//
//
//
//
//
//

#include <p30f6011a.h>            

#include  "..\common\header\host_io.h"            
#include  "..\common\header\30f6010_io.h"
#include  "..\common\can\you_can1.h"
#include  "..\common\can\you_can2.h"
#include  "..\common\uart\ComPort1.h"
#include  "..\common\uart\ComPort2.h"
#include  "..\common\memory_map.h"
#include  "..\common\working_ram\WorkingRam.h"
#include  "..\common\prj\dio.h"


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







#define  	MAX_AIRCON_BUF         	80
#define  	AIRCON_CMD_BUF         	MAX_AIRCON_BUF-8
#define		AIRCON_ADDR_HIGH		0xcf
#define		AIRCON_ADDR_LOW			0xc0


#define		I_AM_HOST			0x2000
#define		HOST_LAST_DATA		0x08
#define		PC_ADDR				0xfe


#define		RCV_DATA			0






/////////////////////////////////
long_field  UserPgmConfigBuf[FLASH_WRITE_BLOCK]; 
long_field  UserPgmConfig[USER_FLASH_BYTE]    	__attribute__((aligned(USER_FLASH_BYTE) , section(".auto_psv,r")));

////////////////////////////////

const unsigned char StatusMessage[50][16]={0};


unsigned    char    xParkingCmd[10]={0,0,0};
unsigned    char    xCarCallCmd[16][10]={0,};
unsigned    char    xFlrCmd[10]={0,0,0};



unsigned    int		msec10=0;
unsigned    int		msec100=0;
unsigned    int  	Crc;  


unsigned	char  	AirconBuf[4][MAX_AIRCON_BUF];     



unsigned    char    Can2_Hib_Retun=0;
unsigned    char    MainTimer=0;
unsigned    char    HostReqAddr=0xff;
unsigned    char    SaveVerify;
unsigned    char    ThisTargetHostAdr,ThisTargetAirconAdr,ThisBufPt=0; 
unsigned    char    Can1VirtualHost=0;

unsigned    char    YouAirconAckTimer[4]={0,0,0,0};


void    Crc_Calulate(unsigned int crcdata);



///////////베트남 테스트  //////////

unsigned    int		TestTimer=0;

void  __attribute__((section(".usercode"))) PLC_Call_Test(void)
{
	if((TestTimer > 500)){
		TestTimer=0;
 		Com2TxThisPt=0;
		Com2TxCnt=36;
		Com2RxBuffer[0]='S';
		Com2RxBuffer[3]='0';
		Com2RxBuffer[4]='2';
		Com2RxBuffer[35]='E';
		Com2TxStart();
	}
}

/////////////////////////////////



#define	COM_CRT		1

#ifdef	COM_CRT

#define	PLC_HEADER	0x02
#define	PLC_TAIL	0x03

///////////////////
void  __attribute__((section(".usercode"))) PLC_Call(void)
{
//				if((Com2RxStatus != TX_SET) && (Com2TxCnt != Com2TxThisPt) && (Com2SerialTime > 100)){

	if((Com2SerialTime > 500)){
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=4;
		Com2RxBuffer[0]=PLC_HEADER;
		Com2RxBuffer[1]='1';
		Com2RxBuffer[2]='1';
		Com2RxBuffer[3]=PLC_TAIL;
		Com2TxStart();
	}
}


unsigned int   __attribute__((section(".usercode"))) RepeatePLCCmdSort(void)
{
	unsigned char i;
	static char bCarCall = 0;
	static char CarCallFlr = 0;
	unsigned char curCarKeyState1, curCarKeyState9;
	

// 파킹 시
	if(xParkingCmd[7] == '1'){
		if(Com2RxBuffer[36]=='1'){
			xParkingCmd[0]=0;
		}					
	}
	else if(xParkingCmd[7] == '0'){
		if(Com2RxBuffer[36]=='0'){
			xParkingCmd[0]=0;
		}					
	}
	else{
		xParkingCmd[0]=0;
	}


	bCarCall = 0;
	for(i=0; i<16; i++)
	{
		if (xCarCallCmd[i][0] != 0)
		{
			bCarCall = 1;
			break;
		}
	}

	curCarKeyState1 =  sRamDArry[0][mCarKey1]; 
	curCarKeyState9 =  sRamDArry[0][mCarKey9];	

	

	if(xParkingCmd[0] > 0){  //parking
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=8;

		Com2RxBuffer[0]=xParkingCmd[1];
		Com2RxBuffer[1]=xParkingCmd[2];
		Com2RxBuffer[2]=xParkingCmd[3];
		Com2RxBuffer[3]=xParkingCmd[4];
		Com2RxBuffer[4]=xParkingCmd[5];
		Com2RxBuffer[5]=xParkingCmd[6];
		Com2RxBuffer[6]=xParkingCmd[7];
		Com2RxBuffer[7]=xParkingCmd[8];
		xParkingCmd[0]--;
	}

	else if(xFlrCmd[0] > 0){   // flr cmd
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=8;

		Com2RxBuffer[0]=xFlrCmd[1];
		Com2RxBuffer[1]=xFlrCmd[2];
		Com2RxBuffer[2]=xFlrCmd[3];
		Com2RxBuffer[3]=xFlrCmd[4];
		Com2RxBuffer[4]=xFlrCmd[5];
		Com2RxBuffer[5]=xFlrCmd[6];
		Com2RxBuffer[6]=xFlrCmd[7];
		Com2RxBuffer[7]=xFlrCmd[8];
		xFlrCmd[0]--;
	}
	
	else if (bCarCall) 
	{				
		switch (CarCallFlr)
		{
		case 0:

			CarCallFlr = 1;

			if (curCarKeyState1 & 0x01) xCarCallCmd[0][0] = 0;
			
			if(xCarCallCmd[0][0] > 0){  
				
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[0][1];
				Com2RxBuffer[1]=xCarCallCmd[0][2];
				Com2RxBuffer[2]=xCarCallCmd[0][3];
				Com2RxBuffer[3]=xCarCallCmd[0][4];
				Com2RxBuffer[4]=xCarCallCmd[0][5];
				Com2RxBuffer[5]=xCarCallCmd[0][6];
				Com2RxBuffer[6]=xCarCallCmd[0][7];
				Com2RxBuffer[7]=xCarCallCmd[0][8];
				xCarCallCmd[0][0]--;

				break;
			}			
			
		case 1:

			CarCallFlr = 2;	
		
			if (curCarKeyState1 & 0x02) xCarCallCmd[1][0] = 0;
			
			if(xCarCallCmd[1][0] > 0){   //car call
				CarCallFlr = 2;	
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[1][1];
				Com2RxBuffer[1]=xCarCallCmd[1][2];
				Com2RxBuffer[2]=xCarCallCmd[1][3];
				Com2RxBuffer[3]=xCarCallCmd[1][4];
				Com2RxBuffer[4]=xCarCallCmd[1][5];
				Com2RxBuffer[5]=xCarCallCmd[1][6];
				Com2RxBuffer[6]=xCarCallCmd[1][7];
				Com2RxBuffer[7]=xCarCallCmd[1][8];
				xCarCallCmd[1][0]--;

				break;
			}	

		case 2:

			CarCallFlr = 3;	

			if (curCarKeyState1 & 0x04) xCarCallCmd[2][0] = 0;
			
			if(xCarCallCmd[2][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[2][1];
				Com2RxBuffer[1]=xCarCallCmd[2][2];
				Com2RxBuffer[2]=xCarCallCmd[2][3];
				Com2RxBuffer[3]=xCarCallCmd[2][4];
				Com2RxBuffer[4]=xCarCallCmd[2][5];
				Com2RxBuffer[5]=xCarCallCmd[2][6];
				Com2RxBuffer[6]=xCarCallCmd[2][7];
				Com2RxBuffer[7]=xCarCallCmd[2][8];
				xCarCallCmd[2][0]--;

				break;
			}
		case 3:

			CarCallFlr = 4;	

			if (curCarKeyState1 & 0x08) xCarCallCmd[3][0] = 0;
			
			if(xCarCallCmd[3][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[3][1];
				Com2RxBuffer[1]=xCarCallCmd[3][2];
				Com2RxBuffer[2]=xCarCallCmd[3][3];
				Com2RxBuffer[3]=xCarCallCmd[3][4];
				Com2RxBuffer[4]=xCarCallCmd[3][5];
				Com2RxBuffer[5]=xCarCallCmd[3][6];
				Com2RxBuffer[6]=xCarCallCmd[3][7];
				Com2RxBuffer[7]=xCarCallCmd[3][8];
				xCarCallCmd[3][0]--;

				break;
			}
		case 4:

			CarCallFlr = 5;	

			if (curCarKeyState1 & 0x10) xCarCallCmd[4][0] = 0;
			
			if(xCarCallCmd[4][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[4][1];
				Com2RxBuffer[1]=xCarCallCmd[4][2];
				Com2RxBuffer[2]=xCarCallCmd[4][3];
				Com2RxBuffer[3]=xCarCallCmd[4][4];
				Com2RxBuffer[4]=xCarCallCmd[4][5];
				Com2RxBuffer[5]=xCarCallCmd[4][6];
				Com2RxBuffer[6]=xCarCallCmd[4][7];
				Com2RxBuffer[7]=xCarCallCmd[4][8];
				xCarCallCmd[4][0]--;

				break;
			}
		case 5:

			CarCallFlr = 6;	

			if (curCarKeyState1 & 0x20) xCarCallCmd[5][0] = 0;
			
			if(xCarCallCmd[5][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[5][1];
				Com2RxBuffer[1]=xCarCallCmd[5][2];
				Com2RxBuffer[2]=xCarCallCmd[5][3];
				Com2RxBuffer[3]=xCarCallCmd[5][4];
				Com2RxBuffer[4]=xCarCallCmd[5][5];
				Com2RxBuffer[5]=xCarCallCmd[5][6];
				Com2RxBuffer[6]=xCarCallCmd[5][7];
				Com2RxBuffer[7]=xCarCallCmd[5][8];
				xCarCallCmd[5][0]--;

				break;
			}
		case 6:

			CarCallFlr = 7;	

			if (curCarKeyState1 & 0x40) xCarCallCmd[6][0] = 0;
			
			if(xCarCallCmd[6][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[6][1];
				Com2RxBuffer[1]=xCarCallCmd[6][2];
				Com2RxBuffer[2]=xCarCallCmd[6][3];
				Com2RxBuffer[3]=xCarCallCmd[6][4];
				Com2RxBuffer[4]=xCarCallCmd[6][5];
				Com2RxBuffer[5]=xCarCallCmd[6][6];
				Com2RxBuffer[6]=xCarCallCmd[6][7];
				Com2RxBuffer[7]=xCarCallCmd[6][8];
				xCarCallCmd[6][0]--;

				break;
			}
		case 7:

			CarCallFlr = 8;	

			if (curCarKeyState1 & 0x80) xCarCallCmd[7][0] = 0;
			
			if(xCarCallCmd[7][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[7][1];
				Com2RxBuffer[1]=xCarCallCmd[7][2];
				Com2RxBuffer[2]=xCarCallCmd[7][3];
				Com2RxBuffer[3]=xCarCallCmd[7][4];
				Com2RxBuffer[4]=xCarCallCmd[7][5];
				Com2RxBuffer[5]=xCarCallCmd[7][6];
				Com2RxBuffer[6]=xCarCallCmd[7][7];
				Com2RxBuffer[7]=xCarCallCmd[7][8];
				xCarCallCmd[7][0]--;

				break;
			}
		case 8:

			CarCallFlr = 9;	

			if (curCarKeyState9 & 0x01) xCarCallCmd[8][0] = 0;
			
			if(xCarCallCmd[8][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[8][1];
				Com2RxBuffer[1]=xCarCallCmd[8][2];
				Com2RxBuffer[2]=xCarCallCmd[8][3];
				Com2RxBuffer[3]=xCarCallCmd[8][4];
				Com2RxBuffer[4]=xCarCallCmd[8][5];
				Com2RxBuffer[5]=xCarCallCmd[8][6];
				Com2RxBuffer[6]=xCarCallCmd[8][7];
				Com2RxBuffer[7]=xCarCallCmd[8][8];
				xCarCallCmd[8][0]--;

				break;
			}
		case 9:

			CarCallFlr = 10;	

			if (curCarKeyState9 & 0x02) xCarCallCmd[9][0] = 0;
			
			if(xCarCallCmd[9][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[9][1];
				Com2RxBuffer[1]=xCarCallCmd[9][2];
				Com2RxBuffer[2]=xCarCallCmd[9][3];
				Com2RxBuffer[3]=xCarCallCmd[9][4];
				Com2RxBuffer[4]=xCarCallCmd[9][5];
				Com2RxBuffer[5]=xCarCallCmd[9][6];
				Com2RxBuffer[6]=xCarCallCmd[9][7];
				Com2RxBuffer[7]=xCarCallCmd[9][8];
				xCarCallCmd[9][0]--;

				break;
			}
		case 10:

			CarCallFlr = 11;	

			if (curCarKeyState9 & 0x04) xCarCallCmd[10][0] = 0;
			
			if(xCarCallCmd[10][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[10][1];
				Com2RxBuffer[1]=xCarCallCmd[10][2];
				Com2RxBuffer[2]=xCarCallCmd[10][3];
				Com2RxBuffer[3]=xCarCallCmd[10][4];
				Com2RxBuffer[4]=xCarCallCmd[10][5];
				Com2RxBuffer[5]=xCarCallCmd[10][6];
				Com2RxBuffer[6]=xCarCallCmd[10][7];
				Com2RxBuffer[7]=xCarCallCmd[10][8];
				xCarCallCmd[10][0]--;

				break;
			}
		case 11:

			CarCallFlr = 12;	

			if (curCarKeyState9 & 0x08) xCarCallCmd[11][0] = 0;
			
			if(xCarCallCmd[11][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[11][1];
				Com2RxBuffer[1]=xCarCallCmd[11][2];
				Com2RxBuffer[2]=xCarCallCmd[11][3];
				Com2RxBuffer[3]=xCarCallCmd[11][4];
				Com2RxBuffer[4]=xCarCallCmd[11][5];
				Com2RxBuffer[5]=xCarCallCmd[11][6];
				Com2RxBuffer[6]=xCarCallCmd[11][7];
				Com2RxBuffer[7]=xCarCallCmd[11][8];
				xCarCallCmd[11][0]--;

				break;
			}
		case 12:

			CarCallFlr = 13;	

			if (curCarKeyState9 & 0x10) xCarCallCmd[12][0] = 0;
			
			if(xCarCallCmd[12][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[12][1];
				Com2RxBuffer[1]=xCarCallCmd[12][2];
				Com2RxBuffer[2]=xCarCallCmd[12][3];
				Com2RxBuffer[3]=xCarCallCmd[12][4];
				Com2RxBuffer[4]=xCarCallCmd[12][5];
				Com2RxBuffer[5]=xCarCallCmd[12][6];
				Com2RxBuffer[6]=xCarCallCmd[12][7];
				Com2RxBuffer[7]=xCarCallCmd[12][8];
				xCarCallCmd[12][0]--;

				break;
			}
		case 13:

			CarCallFlr = 14;	

			if (curCarKeyState9 & 0x20) xCarCallCmd[13][0] = 0;
			
			if(xCarCallCmd[13][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[13][1];
				Com2RxBuffer[1]=xCarCallCmd[13][2];
				Com2RxBuffer[2]=xCarCallCmd[13][3];
				Com2RxBuffer[3]=xCarCallCmd[13][4];
				Com2RxBuffer[4]=xCarCallCmd[13][5];
				Com2RxBuffer[5]=xCarCallCmd[13][6];
				Com2RxBuffer[6]=xCarCallCmd[13][7];
				Com2RxBuffer[7]=xCarCallCmd[13][8];
				xCarCallCmd[13][0]--;

				break;
			}
		case 14:

			CarCallFlr = 15;	

			if (curCarKeyState9 & 0x40) xCarCallCmd[14][0] = 0;
			
			if(xCarCallCmd[14][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[14][1];
				Com2RxBuffer[1]=xCarCallCmd[14][2];
				Com2RxBuffer[2]=xCarCallCmd[14][3];
				Com2RxBuffer[3]=xCarCallCmd[14][4];
				Com2RxBuffer[4]=xCarCallCmd[14][5];
				Com2RxBuffer[5]=xCarCallCmd[14][6];
				Com2RxBuffer[6]=xCarCallCmd[14][7];
				Com2RxBuffer[7]=xCarCallCmd[14][8];
				xCarCallCmd[14][0]--;

				break;
			}
		case 15:

			CarCallFlr = 0;	

			if (curCarKeyState9 & 0x80) xCarCallCmd[15][0] = 0;
			
			if(xCarCallCmd[15][0] > 0){   //car call
				Com2SerialTime=0;
		 		Com2TxThisPt=0;
				Com2TxCnt=8;

				Com2RxBuffer[0]=xCarCallCmd[15][1];
				Com2RxBuffer[1]=xCarCallCmd[15][2];
				Com2RxBuffer[2]=xCarCallCmd[15][3];
				Com2RxBuffer[3]=xCarCallCmd[15][4];
				Com2RxBuffer[4]=xCarCallCmd[15][5];
				Com2RxBuffer[5]=xCarCallCmd[15][6];
				Com2RxBuffer[6]=xCarCallCmd[15][7];
				Com2RxBuffer[7]=xCarCallCmd[15][8];
				xCarCallCmd[15][0]--;

				break;
			}
		default:

			CarCallFlr = 0;
			
			break;
			
		}
	}
	


	Com2TxStart();

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) PLCCmdSort(void)
{
	unsigned char i;

	if(Can1RxBuf[2]==0x81){  //parking
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=8;

		Com2RxBuffer[0]=PLC_HEADER;
		Com2RxBuffer[1]='1';
		Com2RxBuffer[2]='5';
		Com2RxBuffer[3]='0';
		Com2RxBuffer[4]='0';
		Com2RxBuffer[5]='0';
		Com2RxBuffer[6]='0';
		Com2RxBuffer[7]=PLC_TAIL;

		if(Can1RxBuf[4] == 0x01)	Com2RxBuffer[6]='1';

		for(i=0;i<8;i++){
			xParkingCmd[i+1]=Com2RxBuffer[i];
		}
		xParkingCmd[0]=10;
	}
	else if(Can1RxBuf[2]==0x86){   //car call
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=8;

		Com2RxBuffer[0]=PLC_HEADER;
		Com2RxBuffer[1]='1';
		Com2RxBuffer[2]='0';
		Com2RxBuffer[3]='0';
		Com2RxBuffer[4]='0';
		Com2RxBuffer[5]='0';
		Com2RxBuffer[6]='0';
		Com2RxBuffer[7]=PLC_TAIL;

		switch(Can1RxBuf[4]){
			case	0:
				Com2RxBuffer[6]='1';
				for(i=0;i<8;i++){
					xCarCallCmd[0][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[0][0]=20;
				break;		
			case	1:
				Com2RxBuffer[6]='2';
				for(i=0;i<8;i++){
					xCarCallCmd[1][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[1][0]=20;
				break;		
			case	2:
				Com2RxBuffer[6]='4';
				for(i=0;i<8;i++){
					xCarCallCmd[2][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[2][0]=20;
				break;		
			case	3:
				Com2RxBuffer[6]='8';
				for(i=0;i<8;i++){
					xCarCallCmd[3][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[3][0]=20;
				break;		
			case	4:
				Com2RxBuffer[5]='1';
				for(i=0;i<8;i++){
					xCarCallCmd[4][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[4][0]=20;
				break;		
			case	5:
				Com2RxBuffer[5]='2';
				for(i=0;i<8;i++){
					xCarCallCmd[5][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[5][0]=20;
				break;		
			case	6:
				Com2RxBuffer[5]='4';
				for(i=0;i<8;i++){
					xCarCallCmd[6][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[6][0]=20;
				break;		
			case	7:
				Com2RxBuffer[5]='8';
				for(i=0;i<8;i++){
					xCarCallCmd[7][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[7][0]=20;
				break;		
			case	8:
				Com2RxBuffer[4]='1';
				for(i=0;i<8;i++){
					xCarCallCmd[8][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[8][0]=20;
				break;		
			case	9:
				Com2RxBuffer[4]='2';
				for(i=0;i<8;i++){
					xCarCallCmd[9][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[9][0]=20;
				break;		
			case	10:
				Com2RxBuffer[4]='4';
				for(i=0;i<8;i++){
					xCarCallCmd[10][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[10][0]=20;
				break;		
			case	11:
				Com2RxBuffer[4]='8';
				for(i=0;i<8;i++){
					xCarCallCmd[11][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[11][0]=20;
				break;		
			case	12:
				Com2RxBuffer[3]='1';
				for(i=0;i<8;i++){
					xCarCallCmd[12][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[12][0]=20;
				break;		
			case	13:
				Com2RxBuffer[3]='2';
				for(i=0;i<8;i++){
					xCarCallCmd[13][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[13][0]=20;
				break;		
			case	14:
				Com2RxBuffer[3]='4';
				for(i=0;i<8;i++){
					xCarCallCmd[14][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[14][0]=20;
				break;		
			case	15:
				Com2RxBuffer[3]='8';
				for(i=0;i<8;i++){
					xCarCallCmd[15][i+1]=Com2RxBuffer[i];
				}
				xCarCallCmd[15][0]=20;
				break;		
		}
	}

	else if(Can1RxBuf[2]==0x02){   // flr cmd
		Com2SerialTime=0;
 		Com2TxThisPt=0;
		Com2TxCnt=8;

		Com2RxBuffer[0]=PLC_HEADER;
		Com2RxBuffer[1]='1';
		Com2RxBuffer[2]='2';
		Com2RxBuffer[3]=0;
		Com2RxBuffer[4]=0;
		Com2RxBuffer[5]=0;
		Com2RxBuffer[6]=0;
		Com2RxBuffer[7]=PLC_TAIL;

		Can1RxBuf[4]= ~Can1RxBuf[4];

		if(Can1RxBuf[4] & 0x01)	Com2RxBuffer[6] = (Com2RxBuffer[6] | 0x01);	
		if(Can1RxBuf[4] & 0x02)	Com2RxBuffer[6] = (Com2RxBuffer[6] | 0x02);	
		if(Can1RxBuf[4] & 0x04)	Com2RxBuffer[6] = (Com2RxBuffer[6] | 0x04);	
		if(Can1RxBuf[4] & 0x08)	Com2RxBuffer[6] = (Com2RxBuffer[6] | 0x08);	

		if(Can1RxBuf[4] & 0x10)	Com2RxBuffer[5] = (Com2RxBuffer[5] | 0x01);	
		if(Can1RxBuf[4] & 0x20)	Com2RxBuffer[5] = (Com2RxBuffer[5] | 0x02);	
		if(Can1RxBuf[4] & 0x40)	Com2RxBuffer[5] = (Com2RxBuffer[5] | 0x04);	
		if(Can1RxBuf[4] & 0x80)	Com2RxBuffer[5] = (Com2RxBuffer[5] | 0x08);	


		Can1RxBuf[5]= ~Can1RxBuf[5];

		if(Can1RxBuf[5] & 0x01)	Com2RxBuffer[4] = (Com2RxBuffer[4] | 0x01);	
		if(Can1RxBuf[5] & 0x02)	Com2RxBuffer[4] = (Com2RxBuffer[4] | 0x02);	
		if(Can1RxBuf[5] & 0x04)	Com2RxBuffer[4] = (Com2RxBuffer[4] | 0x04);	
		if(Can1RxBuf[5] & 0x08)	Com2RxBuffer[4] = (Com2RxBuffer[4] | 0x08);	

		if(Can1RxBuf[5] & 0x10)	Com2RxBuffer[3] = (Com2RxBuffer[3] | 0x01);	
		if(Can1RxBuf[5] & 0x20)	Com2RxBuffer[3] = (Com2RxBuffer[3] | 0x02);	
		if(Can1RxBuf[5] & 0x40)	Com2RxBuffer[3] = (Com2RxBuffer[3] | 0x04);	
		if(Can1RxBuf[5] & 0x80)	Com2RxBuffer[3] = (Com2RxBuffer[3] | 0x08);	

		if(Com2RxBuffer[6] >= 0x0a)	Com2RxBuffer[6]=(Com2RxBuffer[6] + '7');		
		else						Com2RxBuffer[6]=(Com2RxBuffer[6] + '0');		
		if(Com2RxBuffer[5] >= 0x0a)	Com2RxBuffer[5]=(Com2RxBuffer[5] + '7');		
		else						Com2RxBuffer[5]=(Com2RxBuffer[5] + '0');		
		if(Com2RxBuffer[4] >= 0x0a)	Com2RxBuffer[4]=(Com2RxBuffer[4] + '7');		
		else						Com2RxBuffer[4]=(Com2RxBuffer[4] + '0');		
		if(Com2RxBuffer[3] >= 0x0a)	Com2RxBuffer[3]=(Com2RxBuffer[3] + '7');		
		else						Com2RxBuffer[3]=(Com2RxBuffer[3] + '0');		

		for(i=0;i<8;i++){
			xFlrCmd[i+1]=Com2RxBuffer[i];
		}
		xFlrCmd[0]=10;
	}

	Com2TxStart();

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) PLCAck(unsigned char rdata)
{
	if(Com2RxBuffer[0] != 'S'){ // S: 0x53
		Com2RxCnt=1;		
		Com2RxBuffer[0]=rdata;
	}
	else if( (rdata == 'E') && (Com2RxCnt >= 35)){ // E: 0x45
			Com2RxCnt=0;
			PLCInData();

			YouAirconAckTimer[0]=0;
			Com2RxBuffer[0]=0;

			RepeatePLCCmdSort();

	}
	else if(Com2RxCnt >= 38){
			Com2RxCnt=0;		
	}	
}

#endif

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


		bitF_HibProtocolCom1_Buf=FALSE;		
		bitF_HibProtocolCom2_Buf=FALSE;
		bitF_HibProtocolCan1_Buf=FALSE;
		bitF_HibProtocolCan2_Buf=FALSE;
		bitF_HibProtocolSet_Buf=FALSE;	
		bitF_VirtualHostSet_Buf=TRUE;	//VH
		bitF_IamRepeater_Buf=FALSE;		//Repeater
  		bitF_Sid_Eid_Dl_Buf=FALSE; 



		Set_Byte_Flash_Buf((unsigned int)(cF_Version_A))           	= VERSION;

		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyProductIdValue_A)) = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr1Value_A))     = 1;
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


		Set_Integer_Flash_Buf((unsigned int)(iF_Com1Baudrate_A))           	= 26;
		Set_Integer_Flash_Buf((unsigned int)(iF_Com2Baudrate_A))           	= 26;

		Set_Integer_Flash_Buf((unsigned int)(iF_Com1Baudrate_A))           	= 52;
		Set_Integer_Flash_Buf((unsigned int)(iF_Com2Baudrate_A))           	= 52;

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

/*
		adr=(adr & 0x7ffff);
		adr=(adr >> 4);

		for(i=0;i<*(buf+3);i++){
			value_bit=0x01;
			value_bit=(value_bit << start_bit);

			if(*(buf+6+i) == 0xff){
				sRamDArry[0][adr]= (sRamDArry[0][adr] | (unsigned char)(value_bit));
			}
			else{
				value_bit= ~value_bit;
				sRamDArry[0][adr]= (sRamDArry[0][adr] & (unsigned char)(value_bit));
			}

			start_bit++;
			if(start_bit >= 8){
				adr++;
				start_bit=0;		 
			}
		}
*/
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




void    Crc_Calulate(unsigned int crcdata)
{
	register unsigned int    i;

   Crc= Crc ^ (crcdata & 0x00ff);
   for(i=0;i<=7;i++){
      if( (Crc & 0x0001) == 0x0001)    Crc=(Crc >> 1) ^ 0xA001;
      else Crc=Crc >> 1;
   }
}


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


/*
unsigned int   __attribute__((section(".usercode"))) Can1RxFilterMaskSet(void)
{

    C1CTRLbits.REQOP    = 0x4;

	Can1PollingTimer=0;
	while(Can1PollingTimer<2);
	Can1PollingTimer=0;
	
	Can1RxFilterSet(0,Can1RxSidFilter,Can1RxEidFilter);
	Can1RxMaskSet(0,Can1RxSidMask,Can1RxEidMask);

	Can1PollingTimer=0;
	while(Can1PollingTimer<2);
	Can1PollingTimer=0;

    C1CTRLbits.REQOP    = 0x0;

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) Can2RxFilterMaskSet(void)
{
    C2CTRLbits.REQOP    = 0x4;

	Can2PollingTimer=0;
	while(Can2PollingTimer<2);
	Can2PollingTimer=0;

	Can2RxFilterSet(0,Can2RxSidFilter,Can2RxEidFilter);
	Can2RxMaskSet(0,Can2RxSidMask,Can2RxEidMask);

	Can2PollingTimer=0;
	while(Can2PollingTimer<2);
	Can2PollingTimer=0;

    C2CTRLbits.REQOP    = 0x0;
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

	src_adr1=(unsigned char)((Can1RxSid >> 5) & 0x3c);
	src_adr2=(unsigned char)((Can1RxEid >> 14) & 0x03);
	if(Can1RxEid & 0x10000)	src_adr1=(src_adr1 | 0x40);	
	if(Can1RxEid & 0x20000)	src_adr1=(src_adr1 | 0x80);	

	src_adr1=(src_adr1 | src_adr2);

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
				
/*
				if(Can1RxCnt != hostnm){
					j=4;
					hostnm=(j * 8);
					bCan1RxAll=1;
				}	
				else{
					j=(Can1RxCnt + Can1RxDlc);
					if(j >= 88){
						bCan1RxAll=1;
					}
				}	
*/
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


/*
				if(Can2RxCnt != hostnm){
					j=4;
					hostnm=(j * 8);
					bCan2RxAll=1;
				}	
				else{
					j=(Can2RxCnt + Can2RxDlc);
					if(j >= 88){
						bCan2RxAll=1;
					}
				}	
*/

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




//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////
//////////////// COM1 /////////////////////////////////////



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
/*
        switch(Com1RxStatus){
        	case    STX_CHK:
                if(Com1RxCnt == 1){
                    Com1RxBuffer[0]=buf;				
                    Crc=0xffff;
                    Crc_Calulate((unsigned int)buf);
                }
                else if(Com1RxCnt == 2){
                	Crc_Calulate((unsigned int)buf);
                }
                else if(Com1RxCnt == 3){
                    if(((Com1RxBuffer[Com1RxCnt-1] >= 0x20)  && (Com1RxBuffer[Com1RxCnt-1] <= 0x2f))
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0xa0)  && (Com1RxBuffer[Com1RxCnt-1] <= 0xaf)) 
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0x30)  && (Com1RxBuffer[Com1RxCnt-1] <= 0x3f))
                        || ((Com1RxBuffer[Com1RxCnt-1] >= 0xb0)  && (Com1RxBuffer[Com1RxCnt-1] <= 0xbf)) ){       
						Com1RxStatus=DATA_CHK;
                        Crc_Calulate((unsigned int)buf);
                    }
                    else{
                        Crc=0xffff;
                        Com2RxBuffer[0]=Com2RxBuffer[1];
                        Com2RxBuffer[1]=Com2RxBuffer[2];
						Com2RxCnt=2;
                        Crc_Calulate((unsigned int)Com2RxBuffer[0]);
                        Crc_Calulate((unsigned int)Com2RxBuffer[1]);
                    }   
                }                
                else{
	                Com1RxCnt=1;    
	                Com1RxBuffer[0]=buf;
	                Crc=0xffff;
	                Crc_Calulate((unsigned int)buf);
                }
        		break;
        	case    DATA_CHK:                                    
	            if(Com1RxCnt == (Com1RxBuffer[3] + 4) ){
                    Com1RxStatus=CHKSUM_LOW_CHK;
                }
                Crc_Calulate((unsigned int)buf);
                break;
        	case    CHKSUM_LOW_CHK:
        		temp=(unsigned char)(Crc & 0x00ff);
        		if(temp==buf){
        			Com1RxStatus=CHKSUM_HIGH_CHK;
        		}
        		else    Com1RxStatus=RX_ERROR;
        		break;
        	case    CHKSUM_HIGH_CHK:
        		Crc=((Crc >> 8 ) & 0x00ff);
        		temp=(unsigned char)Crc;
        		if(buf==temp){
                    Com1RxStatus=RX_GOOD;
        		}
        		else    Com1RxStatus=RX_ERROR;
        		break;
        	case    RX_GOOD:
				break;
        	case    RX_ERROR:
        		Com1RxStatus=STX_CHK;
        		Com1RxCnt=0;
        		break;
        	default:
        		Com1RxStatus=STX_CHK;
        		Com1RxCnt=0;
        		break;
        }
*/
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


//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////
//////////////// COM2 /////////////////////////////////////



unsigned int   __attribute__((section(".usercode"))) Com2ReceiveData(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;

//	Com2RxBuffer[Com2RxCnt]=rdata;

   	buf=rdata;

    if(Com2RxStatus != TX_SET){   

      
  		Com2RxBuffer[Com2RxCnt]=buf;

        if(Com2RxCnt < (MAX_RTX_BUF-1)){
            Com2RxCnt++;
        }
        else{
            Com2RxCnt=1;
            Com2RxBuffer[0]=buf;
        }


#ifdef	COM_CRT
		PLCAck(buf);
#else

        switch(Com2RxStatus){
        	case    STX_CHK:
                if(Com2RxCnt == 1){
                    Com2RxBuffer[0]=buf;				
                    Crc=0xffff;
                    Crc_Calulate((unsigned int)buf);
                }
                else if(Com2RxCnt == 2){
                	Crc_Calulate((unsigned int)buf);
                }
                else if(Com2RxCnt == 3){
                    if(((Com2RxBuffer[Com2RxCnt-1] >= 0x20)  && (Com2RxBuffer[Com2RxCnt-1] <= 0x2f))
                        || ((Com2RxBuffer[Com2RxCnt-1] >= 0xa0)  && (Com2RxBuffer[Com2RxCnt-1] <= 0xaf)) 
                        || ((Com2RxBuffer[Com2RxCnt-1] >= 0x30)  && (Com2RxBuffer[Com2RxCnt-1] <= 0x3f))
                        || ((Com2RxBuffer[Com2RxCnt-1] >= 0xb0)  && (Com2RxBuffer[Com2RxCnt-1] <= 0xbf)) ){       
						Com2RxStatus=DATA_CHK;
                        Crc_Calulate((unsigned int)buf);
                    }
                    else{
                        Crc=0xffff;
                        Com2RxBuffer[0]=Com2RxBuffer[1];
                        Com2RxBuffer[1]=Com2RxBuffer[2];
						Com2RxCnt=2;
                        Crc_Calulate((unsigned int)Com2RxBuffer[0]);
                        Crc_Calulate((unsigned int)Com2RxBuffer[1]);
                    }   
                }                
                else{
	                Com2RxCnt=1;    
	                Com2RxBuffer[0]=buf;
	                Crc=0xffff;
	                Crc_Calulate((unsigned int)buf);
                }
        		break;
        	case    DATA_CHK:                                    
	            if(Com2RxCnt == (Com2RxBuffer[3] + 4) ){
                    Com2RxStatus=CHKSUM_LOW_CHK;
                }
                Crc_Calulate((unsigned int)buf);
                break;
        	case    CHKSUM_LOW_CHK:
        		temp=(unsigned char)(Crc & 0x00ff);
        		if(temp==buf){
        			Com2RxStatus=CHKSUM_HIGH_CHK;
        		}
        		else    Com2RxStatus=RX_ERROR;
        		break;
        	case    CHKSUM_HIGH_CHK:
        		Crc=((Crc >> 8 ) & 0x00ff);
        		temp=(unsigned char)Crc;
        		if(buf==temp){
//repeater                    Com2RxStatus=RX_GOOD;
        		}
        		else    Com2RxStatus=RX_ERROR;
        		break;
        	case    RX_GOOD:
				break;
        	case    RX_ERROR:
        		Com2RxStatus=STX_CHK;
        		Com2RxCnt=0;
        		break;
        	default:
        		Com2RxStatus=STX_CHK;
        		Com2RxCnt=0;
        		break;
        }
		if(bitF_IamRepeater){
		    if(Com2RxStatus==RX_GOOD){
///				Com2Rx(rdata);
				SetupCmd(Com2RxBuffer,2);
				Com2RxStatus=STX_CHK;

				Com1TxThisPt=0;
				Com1TxCnt=0;	
			}
			else{
				Com2Rx(rdata);
			}
		}
#endif
    }
	return(0);
}





void  __attribute__((section(".usercode")))  RxCan1AndTxCom1(void)
{
	unsigned char i,src_adr;
  
	if(Can1TxThisPt == 0){
		src_adr=Can1GetSrcAddr();

	    Com1RxBuffer[0]  = PC_ADDR;   
	    Com1RxBuffer[1]  = src_adr;
	    Com1RxBuffer[2]  = 0xA3;                                        
		
		if(bitF_HibProtocolCom1){
			Com1RxBuffer[3]  = 40;
		}
		else{
			Com1RxBuffer[3]  = 88;
		}
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
	}


	if((Com1RxStatus != TX_SET) && (Com1TxCnt != Com1TxThisPt)){
		Com1TxStart();
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


unsigned int   __attribute__((section(".usercode"))) HostSimulationRxCom(unsigned char *buf,unsigned char com_nm)
{
	unsigned char i,pt;

	HostReqAddr=*(buf+0);

	if( !MySetAddrChk(*(buf+0)) ){


//		if(YouHostAckTimer[ThisBufPt] > 10){
//			if(com_nm==1)	Com1RxStatus=STX_CHK;
//			else			Com2RxStatus=STX_CHK;
//
//			return(0);
//		}


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
				if(*(buf+0) & 0x40)	Can2TxEid=(Can2TxEid | 0x010000);				
				if(*(buf+0) & 0x80)	Can2TxEid=(Can2TxEid | 0x020000);
							
				if(*(buf+0) & 0x04)	Can2TxSid=(Can2TxSid | 0x0080);				
				if(*(buf+0) & 0x08)	Can2TxSid=(Can2TxSid | 0x0100);				
				if(*(buf+0) & 0x10)	Can2TxSid=(Can2TxSid | 0x0200);				
				if(*(buf+0) & 0x20)	Can2TxSid=(Can2TxSid | 0x0400);				
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
		if(*(buf+0) & 0x40)	Can1TxEid=(Can1TxEid | 0x010000);				
		if(*(buf+0) & 0x80)	Can1TxEid=(Can1TxEid | 0x020000);
					
		if(*(buf+0) & 0x04)	Can1TxSid=(Can1TxSid | 0x0080);				
		if(*(buf+0) & 0x08)	Can1TxSid=(Can1TxSid | 0x0100);				
		if(*(buf+0) & 0x10)	Can1TxSid=(Can1TxSid | 0x0200);				
		if(*(buf+0) & 0x20)	Can1TxSid=(Can1TxSid | 0x0400);				
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
    unsigned char i,cnt,tmpCan1DataSeq;
    unsigned long tmpCan1TxEid;

	if(!bCan1TxAct){
		Can1DataSeq=0;
		return(0);
	}

	Can1TxDlc=8;
	cnt=(Can1DataSeq * 8);
	tmpCan1DataSeq=Can1DataSeq;


	Can1TxSid=(unsigned char)(cF_SetMyProductIdValue);
	Can1TxEid=0;
	Can1TxEid=(Can1TxEid | (unsigned long)I_AM_HOST);
 


	if(Can1VirtualHost & 0x01)	Can1TxEid=(Can1TxEid | 0x004000);				
	if(Can1VirtualHost & 0x02)	Can1TxEid=(Can1TxEid | 0x008000);
	if(Can1VirtualHost & 0x40)	Can1TxEid=(Can1TxEid | 0x010000);				
	if(Can1VirtualHost & 0x80)	Can1TxEid=(Can1TxEid | 0x020000);				
	if(Can1VirtualHost & 0x04)	Can1TxSid=(Can1TxSid | 0x0080);				
	if(Can1VirtualHost & 0x08)	Can1TxSid=(Can1TxSid | 0x0100);				
	if(Can1VirtualHost & 0x10)	Can1TxSid=(Can1TxSid | 0x0200);				
	if(Can1VirtualHost & 0x20)	Can1TxSid=(Can1TxSid | 0x0400);				


	if(bitF_HibProtocolCan1){
		if(cnt >= 40){
			Can1DataSeq=0;
			bCan1TxAct=0;
	//		Can1_Hib_Retun=0;
			return(0);
		}
	} 									
	else{
		if(cnt >= 88){
			Can1DataSeq=0;
			bCan1TxAct=0;
	//		Can1_Hib_Retun=0;
			return(0);
		}
	} 									


	tmpCan1TxEid=Can1TxEid;
	Can1TxEid=Can1TxEid + (unsigned long)Can1DataSeq;

	if( !Can1TxData(cnt)){
		Can1DataSeq++;
	}
	else{
		Can1DataSeq=tmpCan1DataSeq;
	}

	Can1TxEid=tmpCan1TxEid;

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




	SimIOInit();



	for(j=0;j<4;j++){
		for(i=0;i<(MAX_AIRCON_BUF-1);i++)	AirconBuf[j][i]=0;
	}


	for(i=0;i<(CAN1_MAX_SAVE_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_SAVE_BUF-1);i++)	Can1TxBuf[i]=0;
	for(i=0;i<(CAN2_MAX_SAVE_BUF-1);i++)	Can2RxBuf[i]=0;
	for(i=0;i<(CAN2_MAX_SAVE_BUF-1);i++)	Can2TxBuf[i]=0;


	for(j=0;j<1000;j++){
		for(i=0;i<6500;i++)	asm("CLRWDT");
	}


    if(!SWR){
        asm("RESET");
    }
    SWR=0;


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




	Can1RxSidFilter=(unsigned int)(cF_SetMyProductIdValue);
	Can1RxEidFilter=0;
	Can1RxSidMask=0x007f;
	Can1RxEidMask=0x0000;


	Can2RxSidFilter=(unsigned int)(cF_SetMyProductIdValue);
	Can2RxEidFilter=0;

	Can2RxSidMask=0x007f;
	Can2RxEidMask=0x0000;

//////////////////////////////////////////////////////////////


	if(bitF_IamRepeater){
		Can1RxSidMask=0x0000;
		Can1RxEidMask=0x0000;
		Can2RxSidMask=0x0000;
		Can2RxEidMask=0x0000;
	}


	Can1RxFilterMaskSet(0);
	Can2RxFilterMaskSet(0);


//////////////filter mask set/////////////////////////

    do{

		if( bitF_IamRepeater){

			if(bCan1TxStart)	Can1TxData(0);
			if(bCan2TxStart)	Can2TxData(0);
	

			Can1TxRepeater();
			Can2TxRepeater();

			Com1TxRepeater();
			Com2TxRepeater();
		}
		else{
			if(bitF_VirtualHostSet){
//				PortSevenSeGIn();
//				Simulation();
//				PLC_Call();
			}
	
	 		Can1TxCrtData();
	 		Can2TxCrtData();
			CrtComCheck();
		}


//		PLC_Call_Test();



//		if(PORTFbits.RF5){  !_U1TRMT
//		if( (Com2RxStatus == TX_SET) && (TXEN485Timer > 1)){

		if( (Com2RxStatus == TX_SET) && (_U2TRMT)){
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

    if( _T1IF ){

        _T1IF = 0;
        TMR1  = 0;
	

		if(Can1PollingTimer < 250) 	Can1PollingTimer++;
		if(Can2PollingTimer < 250) 	Can2PollingTimer++;
      	if(Com1SerialTime < 250)	Com1SerialTime++;
      	if(Com2SerialTime < 650)	Com2SerialTime++;
      	if(TXEN485Timer < 250)		TXEN485Timer++;

if(TestTimer < 1000)	TestTimer++;
			
		WaitTime++;

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

		}

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









