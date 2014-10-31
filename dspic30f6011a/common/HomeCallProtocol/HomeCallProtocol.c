
#include <p30f6011a.h>            

#include  "..\header\host_io.h"            
#include  "..\header\30f6010_io.h"
#include  "..\can\you_can1.h"
#include  "..\can\you_can2.h"
#include  "..\uart\ComPort1.h"
#include  "..\uart\Com1App.h"
#include  "..\uart\ComPort2.h"
#include  "..\uart\Com2App.h"

#include  "..\..\crt\crt_main.h"




#define  	S1_OPEN           0x01
#define  	S1_CLOSE          0x02
#define  	S1_OVERLOAD       0x04
#define  	S1_EMG            0x08
#define  	S1_BAT            0x10
#define  	S1_AUTO           0x20
#define  	S1_MANUAL         0x40
#define  	S1_UP             0x80

#define  	S2_DN             0x01
#define  	S2_CAR_MOVE       0x02
#define  	S2_EXT_BUT_CLR    0x04
#define  	S2_FLOW           0x08
#define  	S2_FIRE           0x10
#define  	S2_CAR_BUT_CLR    0x20
#define  	S2_OPEN_LAMP_ON   0x40    
#define  	S2_LAMP_USER      0x80

#define  	S3_VIP            0x01
#define  	S3_PARKING        0x02
#define  	S3_SHIFT          0x04
#define  	S3_UPDN_VO        0x08
#define  	S3_OPEN_SUB       0x10
#define  	S3_FULL           0x20
#define  	S3_CURKEY         0x40    
#define  	S3_STOP           0x80

#define  	S4_CAR_MOST_SERVICE    	0x01
#define  	S4_FAMILY_SERVICE		0x02
//#define  	S4_ARRIVE				0x04
#define  	S4_WAIT_FLR_SERVICE		0x08
#define  	S4_PARKING_READY		0x10
#define  	S4_DOOROPEN_WAIT		0x20



                                 //    89012345678901234567890123456789012345678901234
const unsigned char TypeUpName[]   = {"Type=ELEVATOR&Dong=0101&Ho=1001&Direction=up"}; 
const unsigned char TypeDownName[] = {"Type=ELEVATOR&Dong=0101&Ho=1001&Direction=down"}; 
const unsigned char TypeInfoName[] = {"Type=ELEVATOR&Dong=0101&Ho=1001&Direction=info"}; 




const unsigned char DspFloorName[] = {"B5B4B3B2B10102030405060708091011121314151617181920212223242526272829303132"}; 





unsigned int  Com1HomeCallDataCnt; 
unsigned char Com1HomeCallFloor; 
unsigned char HostAddressSel; 

unsigned char LastCmdBuf[]={0x0,0x0,0x0,0x0,0x0}; 



unsigned char org_rx_cmd[80]; 
unsigned char tmp_org_rx_cmd[80]; 


#define	DongPt	27-8	
#define	HoPt	35-8	


unsigned char   __attribute__((section(".usercode"))) AscToHex(unsigned char tmp)
{
	if( (tmp >= 0x30) && (tmp <= 0x39)){ 
		tmp=(tmp - 0x30);
		return(tmp);	
	}
	else{
		return(0xff);
	}	
}

unsigned char   __attribute__((section(".usercode"))) DataCntCheck(void)
{
	unsigned char t;
	unsigned int  tCnt;

	t=AscToHex(Com1RxBuffer[0]);
	if(t >0)	return(0);

	t=AscToHex(Com1RxBuffer[1]);
	if(t >0)	return(0);

	t=AscToHex(Com1RxBuffer[2]);
	if(t >0)	return(0);

	t=AscToHex(Com1RxBuffer[3]);
	if(t >0)	return(0);

	tCnt=0;

	t=AscToHex(Com1RxBuffer[5]);
	tCnt=((unsigned int)(t * 100) + tCnt);

	t=AscToHex(Com1RxBuffer[6]);
	tCnt=((unsigned int)(t * 10) + tCnt);

	t=AscToHex(Com1RxBuffer[7]);
	tCnt=((unsigned int)(t * 1) + tCnt);

	if(tCnt> 254)	tCnt=0;
	return(tCnt);
}


unsigned char   __attribute__((section(".usercode"))) HostAddressSelect(void)
{
	unsigned char i,j;
	HostAddressSel=0;

	i=AscToHex(tmp_org_rx_cmd[30]);
	HostAddressSel=(i-1);
}


unsigned char   __attribute__((section(".usercode"))) FloorChk(void)
{
	unsigned char i,j;

	Com1HomeCallFloor=0;

	i=AscToHex(tmp_org_rx_cmd[35]);
	Com1HomeCallFloor=((unsigned char)(i * 10) + Com1HomeCallFloor);
	i=AscToHex(tmp_org_rx_cmd[36]);
	Com1HomeCallFloor=((unsigned char)(i * 1) + Com1HomeCallFloor);

	Com1HomeCallFloor=(Com1HomeCallFloor + 5);
}

unsigned char   __attribute__((section(".usercode"))) UpCmdChk(void)
{
	unsigned int  i;

	for(i=0;i<Com1HomeCallDataCnt;i++){
		if(i==DongPt)	i= DongPt+4;		
		if(i==HoPt)		i= HoPt+4;		
		if(tmp_org_rx_cmd[i+8] != TypeUpName[i])	return(0);		
	}

	HostAddressSelect();	
	FloorChk();
	Com1HomeCallFloor=(Com1HomeCallFloor | 0x80);
	return(1);
}

unsigned char   __attribute__((section(".usercode"))) DnCmdChk(void)
{
	unsigned int i;

	for(i=0;i<Com1HomeCallDataCnt;i++){
		if(i==DongPt)	i= DongPt+4;		
		if(i==HoPt)		i= HoPt+4;		
		if(tmp_org_rx_cmd[i+8] != TypeDownName[i])	return(0);
		
	}

	HostAddressSelect();	
	FloorChk();
	Com1HomeCallFloor=(Com1HomeCallFloor | 0x40);

	return(1);
}

unsigned char   __attribute__((section(".usercode"))) InfoCmdChk(void)
{
	unsigned int i;

	for(i=0;i<Com1HomeCallDataCnt;i++){
		if(i==DongPt)	i= DongPt+4;		
		if(i==HoPt)		i= HoPt+4;		
		if(tmp_org_rx_cmd[i+8] != TypeInfoName[i])	return(0);
	}
	HostAddressSelect();	
	return(1);
}



unsigned char   __attribute__((section(".usercode"))) SeperateCmd(void)
{

	unsigned int i,j;
	unsigned char tmpbuf[50];


	Com1RxBuffer[8+Com1HomeCallDataCnt] = 0x0;
	for(i=0;Com1RxBuffer[i];i++){
		tmp_org_rx_cmd[i]=Com1RxBuffer[i]; 
		org_rx_cmd[i]=Com1RxBuffer[i]; 
	}
	org_rx_cmd[i]=0x0; 


	if(tmp_org_rx_cmd[31] != '&'){
		for(i=0;tmp_org_rx_cmd[i+27];i++)	tmpbuf[i+1]=tmp_org_rx_cmd[i+27]; 
		tmpbuf[0]='0'; 
		tmpbuf[i+1]=0; 

		for(i=0;tmpbuf[i];i++)	tmp_org_rx_cmd[i+27]=tmpbuf[i]; 
		tmp_org_rx_cmd[i+27]=0x0; 
	}

	if(tmp_org_rx_cmd[39] != '&'){
		for(i=0;tmp_org_rx_cmd[i+35];i++)	tmpbuf[i+1]=tmp_org_rx_cmd[i+35]; 
		tmpbuf[0]='0'; 
		tmpbuf[i+1]=0; 

		for(i=0;tmpbuf[i];i++)	tmp_org_rx_cmd[i+35]=tmpbuf[i]; 
		tmp_org_rx_cmd[i+35]=0x0; 
	}

		
}



unsigned char   __attribute__((section(".usercode"))) CmdChk(void)
{
	
	SeperateCmd();
	
	if(UpCmdChk()){
		LastCmdBuf[0]='u';
		LastCmdBuf[1]='p';
		LastCmdBuf[2]=0x0;

		Com1RxBuffer[0]=HostAddressSel;		
		Com1RxBuffer[1]=0xfe;		
		Com1RxBuffer[2]=0x24;		
		Com1RxBuffer[3]=0x04;		
		Com1RxBuffer[4]=0x87;		
		Com1RxBuffer[5]=0x00;		
		Com1RxBuffer[6]=Com1HomeCallFloor;		
		Com1RxBuffer[7]=0x00;		
		return(1);		
	}
	else if(DnCmdChk()){
		LastCmdBuf[0]='d';
		LastCmdBuf[1]='o';
		LastCmdBuf[2]='w';
		LastCmdBuf[3]='n';
		LastCmdBuf[4]=0;


		Com1RxBuffer[0]=HostAddressSel;		
		Com1RxBuffer[1]=0xfe;		
		Com1RxBuffer[2]=0x24;		
		Com1RxBuffer[3]=0x04;		
		Com1RxBuffer[4]=0x87;		
		Com1RxBuffer[5]=0x00;		
		Com1RxBuffer[6]=Com1HomeCallFloor;		
		Com1RxBuffer[7]=0x00;		
		return(1);		
	}
	else if(InfoCmdChk()){
		LastCmdBuf[0]=0x0;
		LastCmdBuf[1]=0x0;

		Com1RxBuffer[0]=HostAddressSel;		
		Com1RxBuffer[1]=0xfe;		
		Com1RxBuffer[2]=0x23;		
		Com1RxBuffer[3]=0x02;		
		Com1RxBuffer[4]=0x00;		
		Com1RxBuffer[5]=0x00;		
		return(1);
	}

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) HomeCallProtocolChk(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;


   	buf=rdata;

    if(Com1RxStatus != TX_SET){   

        Com1SerialTime=0;

		if( (buf < 0x10) || (buf > 0x7f)){
            Com1RxCnt=0;
			return(0);
		}

        Com1RxBuffer[Com1RxCnt]=buf;

        if(Com1RxCnt < (MAX_RTX_BUF-1)){
            Com1RxCnt++;
        }
        else{
            Com1RxCnt=0;
            Com1RxBuffer[Com1RxCnt]=buf;
        }

		if(Com1RxCnt < 8){
			return(0);
		}
		else{
			if(Com1RxCnt == 8){
				Com1HomeCallDataCnt=DataCntCheck();
				if(Com1HomeCallDataCnt==0){
		            Com1RxCnt=0;
					return(0);
				}
			}
			else{
				if( ((unsigned int)Com1RxCnt) >= (Com1HomeCallDataCnt + 8)){
					if(!CmdChk()){
			            Com1RxCnt=0;
						return(0);						
					}
					else{
						Com1RxStatus=RX_GOOD;
					}						
				}		
			}
		}
		
	}


	return(0);
}





unsigned int   __attribute__((section(".usercode"))) HomeCallProtocolDataLoad(void)
{

	unsigned char i;

	unsigned char tmpbuf[50]={"&Status="};
	unsigned char ResultOK[]="&Result=ok";
	
	unsigned char EleHogi[10];
	unsigned char EleDsp[10];
	unsigned char CurEleStatus[10];
	unsigned char CurEleRun[10];
	unsigned char EleDest[10];



	EleHogi[0]= (HostAddressSel+1) + 0x30;
	EleHogi[1]= 0x0;

	EleDsp[0]= Com1RxBuffer[4+6];
	EleDsp[1]= Com1RxBuffer[4+7];
	EleDsp[2]= 0x0;
	if(EleDsp[0]==0x0){
		EleDsp[0]= '?';
		EleDsp[1]= '?';
	}

	if(S1_OPEN & Com1RxBuffer[4+1]){
		strcpy(&CurEleStatus[0],"open");
	}
	else if(S1_UP & Com1RxBuffer[4+1]){
		strcpy(&CurEleStatus[0],"up");
	}
	else if(S2_DN & Com1RxBuffer[4+2]){
		strcpy(&CurEleStatus[0],"down");
	}
	else{
		strcpy(&CurEleStatus[0],"stop");
	}



	if(Com1RxBuffer[4+21] < 40){
		strcpy(&CurEleRun[0],"error");
	}
	else if(S1_AUTO & Com1RxBuffer[4+1]){
		strcpy(&CurEleRun[0],"normal");
	}
	else{
		strcpy(&CurEleRun[0],"check");
	}




	i=(Com1RxBuffer[4+5] & 0x1f);  // dest floor
	i=(i * 2);  // dest floor

	EleDest[0]= DspFloorName[i];
	EleDest[1]= DspFloorName[i+1];
	EleDest[2]= 0;



	strcpy( &tmpbuf[0],"&Status=");
	strcat( &tmpbuf[0], &EleHogi[0]);
	strcat( &tmpbuf[0], "^");
	strcat( &tmpbuf[0], &EleDsp[0]);
	strcat( &tmpbuf[0], "^");

	if(LastCmdBuf[0]){
		strcat( &tmpbuf[0], &LastCmdBuf[0]);
		strcat( &tmpbuf[0], "^");
	}
	strcat( &tmpbuf[0], &CurEleStatus[0]);
	strcat( &tmpbuf[0], "^");
	strcat( &tmpbuf[0], &CurEleRun[0]);
	strcat( &tmpbuf[0], "^");
	strcat( &tmpbuf[0], &EleDest[0]);
	strcat( &tmpbuf[0], "&Result=ok");

	strcat( &org_rx_cmd[0], &tmpbuf[0]);

	strcpy( &Com1RxBuffer[0],&org_rx_cmd[0] );

	i=strlen( &Com1RxBuffer[8]);
	Com1RxBuffer[5]=((i/100) + 0x30);
	i=(i%100);
	Com1RxBuffer[6]=((i/10) + 0x30);
	Com1RxBuffer[7]=((i%10) + 0x30);

	Com1TxCnt=strlen( &Com1RxBuffer[0]);


	return(0);
}
