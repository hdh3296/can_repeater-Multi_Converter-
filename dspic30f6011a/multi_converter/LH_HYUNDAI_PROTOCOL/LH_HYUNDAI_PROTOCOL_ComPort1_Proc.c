
#include <p30f6011a.h>            

#include  "..\multi_converter_common\host_io.h"            
#include  "..\multi_converter_common\30f6010_io.h"
#include  "..\multi_converter_common\you_can1.h"
#include  "..\multi_converter_common\you_can2.h"
#include  "..\multi_converter_common\ComPort1.h"
#include  "..\multi_converter_common\ComPort2.h"
#include  "..\multi_converter_common\memory_map.h"
#include  "..\multi_converter_common\WorkingRam.h"
#include  "..\multi_converter_common\multi_converter.h"
#include  "..\multi_converter_common\my_util.h"

#include	"LH_HYUNDAI_PROTOCOL.h"


unsigned    char    LH_ProtocolBuf[MAX_SAVE_BUF];			// normal data buf 
unsigned    char    LH_ProtocolBufErr[MAX_ELEV][55][10];  	// err buf   
unsigned    int     LH_ProtocolBufDoor[MAX_ELEV][32];  		// door open count
unsigned    int     LH_ProtocolBufDrive[MAX_ELEV][24][4];	// 0 = drive count, 1= drive time, 2= drive length,3=err time
unsigned    char    LH_ProtocolBufTime[6];                  // 0 = year 1= mon
unsigned    char    LH_Protocol_Move[MAX_ELEV]={0};         // 

//unsigned    int     tmpSec[MAX_ELEV]={0};  		// drive start sec
//unsigned    int     tmpErrSec[MAX_ELEV]={0};  	// err start sec

unsigned    char    tmpFloor[MAX_ELEV]={0};  	// drive start floor(length)
unsigned    char    tmpEleErr[MAX_ELEV]={0,0,0,0};  	// error on
unsigned    char    tmpDoorOpen[MAX_ELEV]={0};  // door open once




/// error table ¾øÀ½
// È¦¼ö¿îÀü ,Â¦¼ö¿îÀü

unsigned int __attribute__((section(".usercode"))) LH_SecTimer(void)
{

	unsigned char i,j;

	for(i=0;i<MAX_ELEV;i++){          
		if(LH_Protocol_Move[i]==1){
			if(LH_ProtocolBufDrive[i][LH_ProtocolBufTime[3]][1] < 3600){
				LH_ProtocolBufDrive[i][LH_ProtocolBufTime[3]][1]++;
			}
		}
		

		if(	tmpEleErr[i]==1){
			if(LH_ProtocolBufDrive[i][LH_ProtocolBufTime[3]][3] < 3600){
				LH_ProtocolBufDrive[i][LH_ProtocolBufTime[3]][3]++;
			}
		}
	}
	

	LH_ProtocolBufTime[5]++;
	if(LH_ProtocolBufTime[5] >= 60){
		LH_ProtocolBufTime[5]=0;
		LH_ProtocolBufTime[4]++;
		if(LH_ProtocolBufTime[4] >= 60){
			LH_ProtocolBufTime[4]=0;
			LH_ProtocolBufTime[3]++;

			j=LH_ProtocolBufTime[3];
			if(j >= 24)	j=0;

			for(i=0;i<MAX_ELEV;i++){          			
				LH_ProtocolBufDrive[i][j][0]=0;
				LH_ProtocolBufDrive[i][j][1]=0;
				LH_ProtocolBufDrive[i][j][2]=0;
				LH_ProtocolBufDrive[i][j][3]=0;
			}

			if(LH_ProtocolBufTime[3] >= 24){
				LH_ProtocolBufTime[3]=0;
				LH_ProtocolBufTime[2]++;
				if(LH_ProtocolBufTime[2] >= 32){
					LH_ProtocolBufTime[2]=1;
					LH_ProtocolBufTime[1]++;
					if(LH_ProtocolBufTime[1] >= 13){
						LH_ProtocolBufTime[1]=1;
						LH_ProtocolBufTime[0]++;
						if(LH_ProtocolBufTime[0] >= 100){
							LH_ProtocolBufTime[0]=0;
						}
					}
				}
			}
		}
	}	
}



/*
unsigned int __attribute__((section(".usercode"))) CmdReLoad(void)
{
	unsigned char i,pt;
	unsigned int  tmpsid1,tmpsid2;

	pt=2;
	for(i=0;i<8;i++)	Can1TxBuf[i] = Com1RxBuffer[pt+i]; 
	
	tmpsid1=CRT_COMPANY;
	tmpsid2=((Com1RxBuffer[0] & 0x78) << 4);
	tmpsid1=(tmpsid1 | tmpsid2);

	
	Can1TxDlc=8;
	Can1TxSid=(unsigned int)(tmpsid1);
	Can1TxEid=0xfe;
	if(Com1RxBuffer[0] & 0x01)	Can1TxEid=(Can1TxEid | 0x4000);	
	if(Com1RxBuffer[0] & 0x02)	Can1TxEid=(Can1TxEid | 0x8000);	
	if(Com1RxBuffer[0] & 0x04)	Can1TxEid=(Can1TxEid | 0x10000);	
	if(Com1RxBuffer[0] & 0x80)	Can1TxEid=(Can1TxEid | 0x20000);	

	if(I_AM_LISTENING_MODE==1){
		if(Com1RxBuffer[4] == 0x0){
			Can1RxSid=  Can1TxSid;
			Can1RxEid= (Can1TxEid & 0xffffff00);
			RxCan1AndTxCom1();
			return(1);			
		}
	}

	if(Can1RxTimer >= 30){
		if( !Can1TxData(0,0)){
			return(1);                      
		}
	}
	return(0);                      
}
*/

unsigned int __attribute__((section(".usercode")))	LH_Protocol_PcCmd(void)
{
	if(CommonPcCmd_Buf[0]==0)	return(0);

	if(Can1PcCmdTx(CommonPcCmd_Buf[19])){
		CommonPcCmd_Buf[0]=0;	
	}	

	return(1);
}


unsigned int __attribute__((section(".usercode")))	CommonTxCom1(void)
{  
	unsigned char i;


    Crc=0xffff;
    for(i=0;i<Com1TxBuffer[4]+ 5;i++){
        Crc_Calulate((unsigned int)Com1TxBuffer[i]);
    }
//    Com1TxBuffer[i]=(unsigned char)(Crc & 0x00ff);
//    Com1TxBuffer[i+1]=(unsigned char)((Crc >> 8) & 0x00ff);
    Com1TxBuffer[i]=(unsigned char)((Crc >> 8) & 0x00ff);
    Com1TxBuffer[i+1]=(unsigned char)(Crc & 0x00ff);
    Com1TxBuffer[i+2]=0xfe;

	Com1TxCnt = (8 + Com1TxBuffer[4]);

	Com1TxStart();

	return(1);
}



unsigned int __attribute__((section(".usercode")))	NormalDataReload(unsigned char Hogi)
{  
	unsigned char tmp_buf,i,tmp_err_nm;
	unsigned int ThisHostDataPt;

    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

	LH_ProtocolBuf[ThisHostDataPt]=RcvBuf[ThisHostDataPt + RCV_DATA];

	LH_ProtocolBuf[ThisHostDataPt+1]=0;
	if( (RcvBuf[ThisHostDataPt	+ RCV_DATA + 1] & S1_UP))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x01); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 2] & S2_DN))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x02); 
	if( !(RcvBuf[ThisHostDataPt + RCV_DATA + 1] & S1_AUTO))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x04); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_AUTO))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x08); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 2] & S2_FIRE))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x10); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 3] & S3_PARKING))	LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x20); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 3] & S3_VIP))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x40); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 3] & S3_VIP))		LH_ProtocolBuf[ThisHostDataPt+1]=(LH_ProtocolBuf[ThisHostDataPt+1] | 0x80); 

	LH_ProtocolBuf[ThisHostDataPt+2]=0;
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_OVERLOAD))	LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x01); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 3] & S3_FULL))		LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x02); 
	if( (RcvBuf[ThisHostDataPt + RCV_DATA + 2]  & S2_CAR_MOVE))	LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x04); 
//	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_AUTO))		LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x08); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_CLOSE))	LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x10); 
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_CLOSE))	LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x20); 
//	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 21] <= 35 ))		LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x40);  // error
//	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 3] & S3_VIP))		LH_ProtocolBuf[ThisHostDataPt+2]=(LH_ProtocolBuf[ThisHostDataPt+2] | 0x80); 

//	LH_ProtocolBuf[ThisHostDataPt+3]=0;
//	LH_ProtocolBuf[ThisHostDataPt+4]=0;
//	LH_ProtocolBuf[ThisHostDataPt+5]=0;

	LH_ProtocolBuf[ThisHostDataPt+6]=RcvBuf[ThisHostDataPt 	+ RCV_DATA + 17];
	LH_ProtocolBuf[ThisHostDataPt+7]=RcvBuf[ThisHostDataPt 	+ RCV_DATA + 18];
	LH_ProtocolBuf[ThisHostDataPt+8]=RcvBuf[ThisHostDataPt 	+ RCV_DATA + 19];
	LH_ProtocolBuf[ThisHostDataPt+9]=RcvBuf[ThisHostDataPt 	+ RCV_DATA + 20];


	LH_ProtocolBuf[ThisHostDataPt+10]=0;  // up
	LH_ProtocolBuf[ThisHostDataPt+11]=0;  // up
	LH_ProtocolBuf[ThisHostDataPt+12]=0;  // up
	LH_ProtocolBuf[ThisHostDataPt+13]=0;  // up

	LH_ProtocolBuf[ThisHostDataPt+14]=0;	// dn
	LH_ProtocolBuf[ThisHostDataPt+15]=0;	// dn
	LH_ProtocolBuf[ThisHostDataPt+16]=0;	// dn
	LH_ProtocolBuf[ThisHostDataPt+17]=0;	// dn


	LH_ProtocolBuf[ThisHostDataPt+18]= ~(RcvBuf[ThisHostDataPt 	+ RCV_DATA + 8]);
	LH_ProtocolBuf[ThisHostDataPt+19]= ~(RcvBuf[ThisHostDataPt 	+ RCV_DATA + 9]);
	LH_ProtocolBuf[ThisHostDataPt+20]= ~(RcvBuf[ThisHostDataPt 	+ RCV_DATA + 10]);
	LH_ProtocolBuf[ThisHostDataPt+21]= ~(RcvBuf[ThisHostDataPt 	+ RCV_DATA + 11]);





/*
	tmp_buf=LH_ProtocolBuf[ThisHostDataPt+5];
	if(tmp_buf < 49){
		tmp_err_nm=RcvBuf[ThisHostDataPt + RCV_DATA + 21];
		if(tmp_err_nm <= 35 ){
			if(tmpEleErr[Hogi] == 0){
				tmpEleErr[Hogi]=1;
				LH_ProtocolBufErr[Hogi][tmp_buf][0]= 1;
				LH_ProtocolBufErr[Hogi][tmp_buf][1]= 0x0f;
				LH_ProtocolBufErr[Hogi][tmp_buf][2]= tmp_err_nm;
				for(i=0;i<6;i++){
					LH_ProtocolBufErr[Hogi][tmp_buf][3+i]= LH_ProtocolBufTime[i];
				}
				LH_ProtocolBuf[ThisHostDataPt+5]++;  // error
			}	
		}		
		else{
			tmpEleErr[Hogi]=0;
			for(i=0;i<tmp_buf;i++){
				if(LH_ProtocolBufErr[Hogi][i][0] >= 1)	LH_ProtocolBufErr[Hogi][i][0]= 0;
			}
		}
	}
*/	

	tmp_buf=LH_ProtocolBuf[ThisHostDataPt+5];
	if(tmp_buf < 49){
		tmp_err_nm=RcvBuf[ThisHostDataPt + RCV_DATA + 21];
		if(tmp_err_nm <= 35 ){
			if(tmpEleErr[Hogi] == 0){
				tmpEleErr[Hogi]=1;
				LH_ProtocolBufErr[Hogi][tmp_buf][0]= 1;
				LH_ProtocolBufErr[Hogi][tmp_buf][1]= 0x0f;
				LH_ProtocolBufErr[Hogi][tmp_buf][2]= tmp_err_nm;
				for(i=0;i<6;i++){
					LH_ProtocolBufErr[Hogi][tmp_buf][3+i]= LH_ProtocolBufTime[i];
				}
				LH_ProtocolBuf[ThisHostDataPt+5]++;  // error
			}	
		}		
		else{
			if( (tmpEleErr[Hogi]==1) && (tmp_err_nm >= 56)){    //safety- auto ready			
				tmpEleErr[Hogi]=0;
				LH_ProtocolBufErr[Hogi][tmp_buf][0]= 0;
				LH_ProtocolBufErr[Hogi][tmp_buf][1]= 0x0f;
				LH_ProtocolBufErr[Hogi][tmp_buf][2]= 65;		// error all clear auto ready
				for(i=0;i<6;i++){
					LH_ProtocolBufErr[Hogi][tmp_buf][3+i]= LH_ProtocolBufTime[i];
				}
				LH_ProtocolBuf[ThisHostDataPt+5]++;  // error
			}
		}
	}


	tmp_buf=(RcvBuf[ThisHostDataPt + RCV_DATA]-1);
	if( (RcvBuf[ThisHostDataPt 	+ RCV_DATA + 1] & S1_OPEN)){
		if( !(tmpDoorOpen[Hogi] & 0x01)){
			tmpDoorOpen[Hogi] = (tmpDoorOpen[Hogi] | 0x01);
			LH_ProtocolBufDoor[Hogi][tmp_buf]++;
		}
	}
	else{
		tmpDoorOpen[Hogi] = (tmpDoorOpen[Hogi] & 0xfe);
	}


	if( (RcvBuf[ThisHostDataPt + RCV_DATA + 2]  & S2_CAR_MOVE)){
		if(LH_Protocol_Move[Hogi]==0){
			LH_Protocol_Move[Hogi]=1;    
			tmpFloor[Hogi]=RcvBuf[ThisHostDataPt + RCV_DATA]; 
		}
	}
	else{
		if(LH_Protocol_Move[Hogi]==1){
			LH_Protocol_Move[Hogi]=0;
			LH_ProtocolBufDrive[Hogi][LH_ProtocolBufTime[3]][0]++;

			if(tmpFloor[Hogi] >= RcvBuf[ThisHostDataPt + RCV_DATA]){
				i=(tmpFloor[Hogi] - RcvBuf[ThisHostDataPt + RCV_DATA]);
			}
			else{
				i=(RcvBuf[ThisHostDataPt + RCV_DATA]-tmpFloor[Hogi]);
			}
			i=(i * 3);			
			LH_ProtocolBufDrive[Hogi][LH_ProtocolBufTime[3]][2]=(LH_ProtocolBufDrive[Hogi][LH_ProtocolBufTime[3]][2] + (unsigned int)i);			
		}
	}
}


unsigned int __attribute__((section(".usercode")))	NormalDataTxCom1(unsigned char Hogi)
{  
	unsigned int ThisHostDataPt;
	unsigned char i;

	Com1TxCnt=0;
	Com1TxThisPt=0;
    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

//	NormalDataReload(Hogi);

    Com1TxBuffer[0]  = 0xff;   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                          
	Com1TxBuffer[3]  = 0;       
	Com1TxBuffer[4]  = 0x17;    
	Com1TxBuffer[5]  = Com1RxBuffer[5];    

    for(i=0;(i < Com1TxBuffer[4]);i++){
		Com1TxBuffer[6+i]=LH_ProtocolBuf[ThisHostDataPt + i + RCV_DATA];
//		Com1TxBuffer[6+i]=RcvBuf[ThisHostDataPt + i + RCV_DATA];
    }
	CommonTxCom1();
}


unsigned int __attribute__((section(".usercode")))	ErrorDataTxCom1(unsigned char Hogi)
{  
	unsigned int ThisHostDataPt;
	unsigned char i,j,k;


	Com1TxCnt=0;
	Com1TxThisPt=0;
    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                          
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
	Com1TxBuffer[5]  = Com1RxBuffer[5];  //0x02  err cmd     
	Com1TxBuffer[6]  = Com1RxBuffer[6];  //0x02  err count  

	Com1TxBuffer[4] = (Com1RxBuffer[6] * 9)+2;

	k=7;
    for(j=0;(j < Com1RxBuffer[6]);j++){
    	for(i=0;(i < 9);i++){
			Com1TxBuffer[k+i]=LH_ProtocolBufErr[Hogi][j][i];
		}
		k=k+9;
    }

	CommonTxCom1();
}


unsigned int __attribute__((section(".usercode")))	ErrorClearDataTxCom1(unsigned char Hogi)
{  
	unsigned char i,j;
	unsigned int ThisHostDataPt;

    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

	for(i=0;i<50;i++){
		for(j=0;j<10;j++)	LH_ProtocolBufErr[Hogi][i][j]=0;
	}
	LH_ProtocolBuf[ThisHostDataPt+5]=0;

	Com1TxCnt=0;
	Com1TxThisPt=0;

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                           
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
    Com1TxBuffer[5]  = Com1RxBuffer[5];   
    Com1TxBuffer[6]  = Com1RxBuffer[6];	 
    Com1TxBuffer[7]  = 0x01;   			                                        

	CommonTxCom1();
}



unsigned int __attribute__((section(".usercode")))	SimpleControlCmd(unsigned char Hogi)
{  
	unsigned int ThisHostDataPt;
    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

	switch(Com1RxBuffer[6]){
		case	1:
//			odd_Control;
			break;
		case	2:
//			even)control;
			break;
		case	3:  //parking
			CommonPcCmd_Buf[0]=0x24;
			CommonPcCmd_Buf[1]=0x04;
			CommonPcCmd_Buf[2]=0x81;
			CommonPcCmd_Buf[3]=0x00;
			CommonPcCmd_Buf[4]=Com1RxBuffer[7];
			CommonPcCmd_Buf[19]=Hogi;
			if(Com1RxBuffer[7] == 0){	
				LH_ProtocolBuf[ThisHostDataPt+4]= ( LH_ProtocolBuf[ThisHostDataPt+4] & 0xfb);
			}
			else{	
				LH_ProtocolBuf[ThisHostDataPt+4]= ( LH_ProtocolBuf[ThisHostDataPt+4] | 0x04);
			}

			break;
		case	4:  // fire
			CommonPcCmd_Buf[0]=0x24;
			CommonPcCmd_Buf[1]=0x04;
			CommonPcCmd_Buf[2]=0x84;
			CommonPcCmd_Buf[3]=0x00;
			CommonPcCmd_Buf[4]=Com1RxBuffer[7];
			CommonPcCmd_Buf[19]=Hogi;

			if(Com1RxBuffer[7] == 0){	
				LH_ProtocolBuf[ThisHostDataPt+4]= ( LH_ProtocolBuf[ThisHostDataPt+4] & 0xef);
			}
			else{	
				LH_ProtocolBuf[ThisHostDataPt+4]= ( LH_ProtocolBuf[ThisHostDataPt+4] | 0x10);
			}

			break;
		case	5:
			break;
		default:
			break;

	}
}


unsigned int __attribute__((section(".usercode")))	NonStopCall(unsigned char Hogi)
{  
	CommonPcCmd_Buf[0]=0x24;
	CommonPcCmd_Buf[1]=0x06;
	CommonPcCmd_Buf[2]=0x02;
	CommonPcCmd_Buf[3]=0x00;
	CommonPcCmd_Buf[4]= ~Com1RxBuffer[6];
	CommonPcCmd_Buf[5]= ~Com1RxBuffer[7];
	CommonPcCmd_Buf[6]= ~Com1RxBuffer[8];
	CommonPcCmd_Buf[7]= ~Com1RxBuffer[9];
	CommonPcCmd_Buf[19]=Hogi;
}




unsigned int __attribute__((section(".usercode")))	CarExtCall(unsigned char Hogi)
{  
	unsigned char tmpfloor;
	tmpfloor=(Com1RxBuffer[6]);
	switch(Com1RxBuffer[5]){
		case	3:  // car call
			tmpfloor=(Com1RxBuffer[6]-1);
			CommonPcCmd_Buf[0]=0x24;
			CommonPcCmd_Buf[1]=0x04;
			CommonPcCmd_Buf[2]=0x86;
			CommonPcCmd_Buf[3]=0x00;
			CommonPcCmd_Buf[4]=tmpfloor;
			CommonPcCmd_Buf[5]=0x00;
			CommonPcCmd_Buf[19]=Hogi;
			break;

		case	4: // up call
			CommonPcCmd_Buf[0]=0x24;
			CommonPcCmd_Buf[1]=0x04;
			CommonPcCmd_Buf[2]=0x87;
			CommonPcCmd_Buf[3]=0x00;
			CommonPcCmd_Buf[4]=(tmpfloor | 0x80);
			CommonPcCmd_Buf[5]=0x00;
			CommonPcCmd_Buf[19]=Hogi;
			break;
		case	5: // down call
			CommonPcCmd_Buf[0]=0x24;
			CommonPcCmd_Buf[1]=0x04;
			CommonPcCmd_Buf[2]=0x87;
			CommonPcCmd_Buf[3]=0x00;
			CommonPcCmd_Buf[4]=(tmpfloor | 0x40);
			CommonPcCmd_Buf[5]=0x00;
			CommonPcCmd_Buf[19]=Hogi;
			break;

		default:
			break;

	}
}


unsigned int __attribute__((section(".usercode")))	EleSimpleControlTxCom1(unsigned char Hogi)
{  
	unsigned char i;

	Com1TxCnt=0;
	Com1TxThisPt=0;

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                           
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
    Com1TxBuffer[5]  = Com1RxBuffer[5];   
    Com1TxBuffer[6]  = Com1RxBuffer[6];	 
    Com1TxBuffer[7]  = 0x01;   			                                        

	CommonTxCom1();

	SimpleControlCmd(Hogi);
  	
}


unsigned int __attribute__((section(".usercode")))	EleCallControlTxCom1(unsigned char Hogi)
{  
	unsigned char i;

	Com1TxCnt=0;
	Com1TxThisPt=0;

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                           
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
    Com1TxBuffer[5]  = Com1RxBuffer[5];   
    Com1TxBuffer[6]  = 0x01;	 
    Com1TxBuffer[7]  = 0x01;   			                                        

	CommonTxCom1();
}



unsigned int __attribute__((section(".usercode")))	EleReportTxCom1(unsigned char Hogi)
{  
//	unsigned int ThisHostDataPt;
	unsigned char i,j,k;

	Com1TxCnt=0;
	Com1TxThisPt=0;
//////////    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                          
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
	Com1TxBuffer[5]  = Com1RxBuffer[5];  //0x02  err cmd     
	Com1TxBuffer[6]  = Com1RxBuffer[6];  //0x02  err count  

	Com1TxBuffer[4] = (Com1RxBuffer[6] * 9)+2;
//	Com1TxBuffer[6] = LH_ProtocolBufTime[3];

	k=7;
    for(j=0;(j < Com1RxBuffer[6]);j++){
		Com1TxBuffer[k]=j+1;
		Com1TxBuffer[k+1]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][0] >> 8);
		Com1TxBuffer[k+2]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][0]);
		Com1TxBuffer[k+3]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][1] >> 8);
		Com1TxBuffer[k+4]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][1]);
		Com1TxBuffer[k+5]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][2] >> 8);
		Com1TxBuffer[k+6]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][2]);
		Com1TxBuffer[k+7]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][3] >> 8);
		Com1TxBuffer[k+8]=(unsigned char)(LH_ProtocolBufDrive[Hogi][j][3]);
		k=k+9;
    }


	CommonTxCom1();

}


unsigned int __attribute__((section(".usercode")))	DoorOpenReportClear(unsigned char Hogi)
{  
	unsigned char i;

	for(i=0;i<32;i++){
		LH_ProtocolBufDoor[Hogi][i]=0;
//		LH_ProtocolBufDoor[Hogi][i][1]=0;
	}
	return(1);
}



unsigned int __attribute__((section(".usercode")))	DoorReportTxCom1(unsigned char Hogi)
{  
	unsigned int ThisHostDataPt;
	unsigned char i,j;


	Com1TxCnt=0;
	Com1TxThisPt=0;
    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                          
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
	Com1TxBuffer[5]  = Com1RxBuffer[5];  //0x02  err cmd     
	Com1TxBuffer[6]  = Com1RxBuffer[6];  //0x02  err count  

	Com1TxBuffer[4] = (Com1RxBuffer[6] * 3)+2;

	j=0;
    for(i=0;(i < Com1TxBuffer[4]);i++){
		Com1TxBuffer[7+i]=j+1;
		i++;		
		Com1TxBuffer[7+i]=(unsigned char)(LH_ProtocolBufDoor[Hogi][j] >> 8);
		i++;
		Com1TxBuffer[7+i]=(unsigned char)(LH_ProtocolBufDoor[Hogi][j]);

		j++;
    }

	CommonTxCom1();
}




unsigned int __attribute__((section(".usercode")))	CurTimeTxCom1(unsigned char Hogi)
{  
	unsigned int ThisHostDataPt;
	unsigned char i;


	Com1TxCnt=0;
	Com1TxThisPt=0;
    ThisHostDataPt = (unsigned int)(Hogi * HOST_DATA_RECOD);

    Com1TxBuffer[0]  = Com1RxBuffer[0];   
    Com1TxBuffer[1]  = Com1RxBuffer[1];	  
    Com1TxBuffer[2]  = Com1RxBuffer[2];                                          
	Com1TxBuffer[3]  = Com1RxBuffer[3];       
	Com1TxBuffer[4]  = Com1RxBuffer[4];    
	Com1TxBuffer[5]  = Com1RxBuffer[5];       

	Com1TxBuffer[4] = 0x07;

    for(i=0;(i < Com1TxBuffer[4]);i++){
		Com1TxBuffer[6+i]=LH_ProtocolBufTime[i];
    }

	CommonTxCom1();
}


unsigned int __attribute__((section(".usercode")))	HogiSelect(void)
{
	unsigned char i;
	for(i=0;i<MAX_ELEV;i++){
		if(Com1RxBuffer[1] == ReqHost[i].Ho_Host[0]){
			return(i);
		}
	}
	return(0xff);	
}


unsigned int __attribute__((section(".usercode")))  CrtCom1Check(void)
{
	unsigned char Hogi;

	Hogi=HogiSelect();
	if(Hogi == 0xff)			return(1);
	if(HostElevLive[Hogi] > 20)	return(1);  // 30 sec

	switch(Com1RxBuffer[2]){
		case	0x01:
			switch(Com1RxBuffer[5]){
				case	0x01:
					NormalDataTxCom1(Hogi);
					break;
				case	0x02:
					ErrorDataTxCom1(Hogi);
					break;
				case	0x03:
					ErrorClearDataTxCom1(Hogi);
					break;
				default:
					break;
			}
			break;
		case	0x02:
			switch(Com1RxBuffer[5]){
				case	0x01:
					EleSimpleControlTxCom1(Hogi);
					break;
				case	0x02:   // non stop floor
					NonStopCall(Hogi);
					Com1RxBuffer[4]=0x03;
					EleCallControlTxCom1(Hogi);
					break;
				case	0x03:
				case	0x04:
				case	0x05:
					CarExtCall(Hogi);
					EleCallControlTxCom1(Hogi);
					break;
				default:
					break;
			}
			break;
		case	0x03:
			switch(Com1RxBuffer[5]){
				case	0x01:
					EleReportTxCom1(Hogi);
					break;
				case	0x02:
					DoorReportTxCom1(Hogi);
					break;
				case	0x03:
					DoorOpenReportClear(Hogi);
					Com1RxBuffer[4]=0x03;
					EleCallControlTxCom1(Hogi);
					break;
				default:
					break;
			}
			break;
		case	0x04:
			switch(Com1RxBuffer[5]){
				case	0x01:
					CurTimeTxCom1(Hogi);
					break;
				case	0x02:
					LH_ProtocolBufTime[0]=Com1RxBuffer[6];
					LH_ProtocolBufTime[1]=Com1RxBuffer[7];
					LH_ProtocolBufTime[2]=Com1RxBuffer[8];
					LH_ProtocolBufTime[3]=Com1RxBuffer[9];
					LH_ProtocolBufTime[4]=Com1RxBuffer[10];
					LH_ProtocolBufTime[5]=Com1RxBuffer[11];
					CurTimeTxCom1(Hogi);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
				
	return(1);

/*
	unsigned char i,pt;
	unsigned int  tmpsid1,tmpsid2;

	pt=2;
	for(i=0;i<8;i++)	Can1TxBuf[i] = Com1RxBuffer[pt+i]; 
	
	tmpsid1=CRT_COMPANY;
	tmpsid2=((Com1RxBuffer[0] & 0x78) << 4);
	tmpsid1=(tmpsid1 | tmpsid2);

	
	Can1TxDlc=8;
	Can1TxSid=(unsigned int)(tmpsid1);
	Can1TxEid=0xfe;
	if(Com1RxBuffer[0] & 0x01)	Can1TxEid=(Can1TxEid | 0x4000);	
	if(Com1RxBuffer[0] & 0x02)	Can1TxEid=(Can1TxEid | 0x8000);	
	if(Com1RxBuffer[0] & 0x04)	Can1TxEid=(Can1TxEid | 0x10000);	
	if(Com1RxBuffer[0] & 0x80)	Can1TxEid=(Can1TxEid | 0x20000);	

	if(I_AM_LISTENING_MODE==1){
		if(Com1RxBuffer[4] == 0x0){
			Can1RxSid=  Can1TxSid;
			Can1RxEid= (Can1TxEid & 0xffffff00);
			RxCan1AndTxCom1();
			return(1);			
		}
	}

	if(Can1RxTimer >= 10){
		if( !Can1TxData(0,0)){
			return(1);                      
		}
	}
*/

	return(0);                      
}




unsigned int __attribute__((section(".usercode")))   __attribute__((section(".usercode"))) Com1Crt_Protocol(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;

	Com1RxBuffer[Com1RxCnt]=rdata;


   	buf=rdata;

    if(Com1RxStatus != TX_SET){   

		if(Com1SerialTime > 4){
            Com1RxCnt=0;
		}

        Com1SerialTime=0;
        Com1RxBuffer[Com1RxCnt]=buf;

        if(Com1RxCnt < (MAX_COM1_RXBUF-1)){
            Com1RxCnt++;
        }
        else{
            Com1RxCnt=0;
            Com1RxBuffer[Com1RxCnt]=buf;
        }

        switch(Com1RxStatus){
        	case    STX_CHK:
                if(Com1RxCnt == 1){
					if(buf == 0xff){
                    	Com1RxBuffer[0]=buf;				
                    	Crc=0xffff;
                    	Crc_Calulate((unsigned int)buf);
					}
					else{
            			Com1RxCnt=0;
					}
                }
                else if(Com1RxCnt == 5){
                        Crc_Calulate((unsigned int)buf);
						Com1RxStatus=DATA_CHK;
                }                
                else{
                	if(Com1RxCnt < 5){
                		Crc_Calulate((unsigned int)buf);
					}
					else{
		                Com1RxCnt=0;    
					}
                }
        		break;
        	case    DATA_CHK:                                    
	            if(Com1RxCnt >= (Com1RxBuffer[4] + 5) ){
                    Com1RxStatus=CHKSUM_LOW_CHK;
                }
                Crc_Calulate((unsigned int)buf);
                break;
        	case    CHKSUM_LOW_CHK:

//        			Com1RxStatus=CHKSUM_HIGH_CHK;


        		temp=(unsigned char)(Crc >> 8);           // 0x00ff);
//        		temp=(unsigned char)(Crc & 0x00ff);
        		if(temp==buf){
        			Com1RxStatus=CHKSUM_HIGH_CHK;
        		}
        		else    Com1RxStatus=RX_ERROR;

        		break;
        	case    CHKSUM_HIGH_CHK:
//                    Com1RxStatus=ETX_CHK;


//        		Crc=((Crc >> 8 ) & 0x00ff);
        		temp=(unsigned char)Crc;
        		if(buf==temp){
                    Com1RxStatus=ETX_CHK;
        		}
        		else    Com1RxStatus=RX_ERROR;

        		break;
        	case    RX_GOOD:
				break;
        	case    ETX_CHK:
					if(buf == 0xfe){
                    	Com1RxStatus=RX_GOOD;
					}
					else{
		        		Com1RxStatus=STX_CHK;
		        		Com1RxCnt=0;
					}
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
	}	

	return(0);
}



unsigned int __attribute__((section(".usercode")))   __attribute__((section(".usercode"))) Com1ReceiveData(unsigned char rdata)
{
	Com1Crt_Protocol(rdata);
}
