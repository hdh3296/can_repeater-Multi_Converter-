
#include <p30f6011a.h>            


#include  	"..\..\multi_converter_common\host_io.h"            
#include  	"..\..\multi_converter_common\30f6010_io.h"
#include  	"..\..\multi_converter_common\you_can1.h"
#include  	"..\..\multi_converter_common\you_can2.h"
#include  	"..\..\multi_converter_common\ComPort1.h"
#include  	"..\..\multi_converter_common\ComPort2.h"
#include  	"..\..\multi_converter_common\memory_map.h"
#include  	"..\..\multi_converter_common\WorkingRam.h"
#include  	"..\..\multi_converter_common\Multi_converter.h"

#include	"hyundai_HomeCall_Kwangkyo_HomeCall.h"



const unsigned char DspFloorName[] = {"B7B6B5B4B3B2B10102030405060708091011121314151617181920212223242526272829303132"}; 


const unsigned char TypeHeaderName[]= {"Type=ELEVATOR&"};
const unsigned char Hyundai_Local_Call_UpDn[3][10]={"down","up","info"};


unsigned 	char   Com1TxBuffer[MAX_COM1_TXBUF];
unsigned 	char   Com1RxBuffer[MAX_COM1_RXBUF];

unsigned	char   Com2TxBuffer[MAX_COM2_TXBUF];
unsigned 	char   Com2RxBuffer[MAX_COM2_RXBUF];
			
unsigned	char    Can1TxBuf[CAN1_MAX_TX_BUF];
unsigned    char    Can1RxBuf[CAN1_MAX_RX_BUF];

unsigned    char    Can2TxBuf[CAN2_MAX_TX_BUF];
unsigned    char    Can2RxBuf[CAN2_MAX_RX_BUF];



volatile tagReqHost ReqHost[MAX_ELEV];

unsigned    char    RcvBuf[MAX_SAVE_BUF];




	
unsigned char   UpRegistry[32][MAX_HO_PER_FLOOR];  		///층 //세대수							        
unsigned char   DnRegistry[32][MAX_HO_PER_FLOOR];  		///층 //세대수
unsigned char   InfoCall[32][MAX_HO_PER_FLOOR];    		///층 //세대수
unsigned char   UpKeyTx[32][MAX_HO_PER_FLOOR];
unsigned char   DnKeyTx[32][MAX_HO_PER_FLOOR];
unsigned char   UpKeyRcvAck[32][MAX_HO_PER_FLOOR];
unsigned char   DnKeyRcvAck[32][MAX_HO_PER_FLOOR];


unsigned 	char 	ThisHostSelect=0; 


unsigned 	int  	StatusTxTime=0; 
unsigned 	int  	TxGoOnTime=0; 
unsigned 	int  	Com1HomeCallTxTotalDataCnt=0; 
unsigned 	int  	Com1HomeCallRxDataCnt; 
//unsigned 	char 	Com1HomeCallFloor;
//unsigned 	char 	Com1HomeCallHo; 

//unsigned 	char 	Com1HomeCallCurReqWard; 
unsigned 	char 	BufferFull=0; 

unsigned 	char	ThisSystemTopFloor=0;


unsigned 	int 	StatusUpDnTxByteOrder=0;
unsigned 	int 	StatusUpDnFirstPt=0;
unsigned 	int 	StatusTxOrder=0;
unsigned 	int 	StatusTxHeader=0;

unsigned 	char 	LastFloor_Ho_Byte=0;
unsigned 	char 	LastFloor_Ho_Bit=0;
			
unsigned 	char	Key_LastFloor_Ho_Bit = 0;
unsigned 	char	Key_LastFloor_Ho_Byte= 0;
unsigned 	char	Key_StatusUpDnTxByteOrder=0;


unsigned char org_rx_cmd[200]; 



unsigned 	int		ElevatorServerAliveTime=0;


unsigned 	char	Cmd_Nm=0;
unsigned 	char	TxOnece=0;



unsigned	char  	HostElevLive[MAX_ELEV];
unsigned 	char	ThisCmd[20];

unsigned 	int		iThisDong;
unsigned 	int		iThisHo;
unsigned 	int		iThisFlow;
unsigned 	int		iThisLine;
unsigned 	int		iThisCmdSort;

unsigned 	char	ThisHostReceive;







//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

unsigned char   __attribute__((section(".usercode"))) zeroclear(unsigned char *buf)
{
	unsigned char i=0;
	do{
		if( *(buf+i) > '0'){
			return(0);
		}
		else{
			strcpy(buf,(buf+1));
			i=0;
		}
	}while(*(buf+i));
}


unsigned char   __attribute__((section(".usercode"))) xStringCompare(unsigned char *s1,unsigned char *s2)
{
	unsigned char i,j,k;

	for(;(*s1) && (*s2);){
		do{
			if( *s1 != *s2)		return(0);
			else{
				s1++;
				s2++;
				if(*(s1) == 0){
					if(*(s2) == 0){
						return(1);
					}
				}
			}
		}while(1);
	}
	return(0);
}


unsigned char   __attribute__((section(".usercode"))) HexToAsc(unsigned char *buf,unsigned int src)
{
	unsigned int tmpsrc;

	*(buf + 0)='0';						
	*(buf + 1)='0';						
	*(buf + 2)='0';						
	*(buf + 3)='0';
	*(buf + 4)='0';
	*(buf + 5)=0;
				
	tmpsrc=src;	
	tmpsrc=(tmpsrc / 10000);
	*(buf + 0)= (tmpsrc + '0');


	tmpsrc=(src % 10000);
	tmpsrc=(tmpsrc / 1000);
	*(buf + 1)= (tmpsrc + '0');
						
	tmpsrc=(src % 1000);
	tmpsrc=(tmpsrc / 100);
	*(buf + 2)= (tmpsrc + '0');
	
	tmpsrc=(src % 100);
	tmpsrc=(tmpsrc / 10);
	*(buf + 3)= (tmpsrc + '0');

	tmpsrc=(src % 10);
	*(buf + 4)= (tmpsrc + '0');

	return(0);
}


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












/////////// tx string load///////////////////////////////


unsigned int   __attribute__((section(".usercode"))) Hyundai_UpDnCallSet(void)
{
	unsigned char i,j,k,value_bit;

	k=iThisLine-1;

	value_bit=0x01;

	i=(k / 8);
	j=(k % 8);
	value_bit =(value_bit << j);
	i++;


	k=iThisFlow;


	switch(iThisCmdSort){
		case	1:
			if(iThisFlow > 0){ 
				DnRegistry[k][i] 	= (DnRegistry[k][i] | value_bit);	
				DnKeyTx[k][i] 		= (DnKeyTx[k][i] 	| value_bit);	
				DnKeyRcvAck[k][i] 	= (DnKeyRcvAck[k][i]| value_bit);
			}

/*
			for(j=0;j<17;j++){
				DnRegistry[j][i] 	= 0x03;	
				DnKeyTx[j][i] 		= 0x03;	
				DnKeyRcvAck[j][i] 	= 0x03;
			}
*/

			break;
		case	2:
			if(iThisFlow < 31){ 
				UpRegistry[k][i] 	= (UpRegistry[k][i] | value_bit);	
				UpKeyTx[k][i] 		= (UpKeyTx[k][i] 	| value_bit);	
				UpKeyRcvAck[k][i] 	= (UpKeyRcvAck[k][i]| value_bit);	
			}

/*
			for(j=1;j<25;j++){
				UpRegistry[j][i] 	= 0x03;	
				UpKeyTx[j][i] 		= 0x03;	
				UpKeyRcvAck[j][i] 	= 0x03;
			}
*/

			break;
		case	3:
			InfoCall[k][i] = (InfoCall[k][i] | value_bit);		
			break;
	}


	return(0);
}


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

/*
unsigned char   __attribute__((section(".usercode"))) KeyTxOnChk(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{

	unsigned int 	idptx=0;	
	unsigned char 	tmp_hogi=0;
	unsigned char	key_give=1;


    asm("CLRWDT");


	tmp_hogi=ReqHost[ev_hogi].HostGroup;
	idptx=(tmp_hogi * HOST_DATA_RECOD);


	key_give=UpDownArriveKeyChk(tfloor,ThisHoDnUpInfo,ev_hogi);

	if(key_give){
		if( (RcvBuf[idptx + DEST_FLR] & ONLY_FLR) == tfloor){
			if(ThisHoDnUpInfo==1){
				if((RcvBuf[idptx + DEST_FLR] & DN_READY)){
					key_give=0;
				}
			}
			else if(ThisHoDnUpInfo==2){
				if((RcvBuf[idptx + DEST_FLR] & UP_READY)){
					key_give=0;
				}
			}
		}
	}


	if(key_give){
		if( (RcvBuf[idptx + ELEV_CRT_KEY] & (~UPDN_READY)) == (tfloor+1)){
			if(ThisHoDnUpInfo==1){
				if((RcvBuf[idptx + ELEV_CRT_KEY] & DN_READY)){
					key_give=0;
				}
			}
			else if(ThisHoDnUpInfo==2){
				if((RcvBuf[idptx + ELEV_CRT_KEY] & UP_READY)){
					key_give=0;
				}
			}
		}
	}


	return(key_give);

}
*/


unsigned int   __attribute__((section(".usercode"))) HomeCallButtonReceiveChk(unsigned char HostAdr)
{

	unsigned char 	floor;
	unsigned char 	*TmpUpKeyTx,*TmpDnKeyTx;
	unsigned char 	aa_bit_chk=0x01;
	unsigned char 	aa_start_byte,aa_start_bit;
	unsigned char 	thiskey;
	unsigned int 	idptx=0;	


	idptx=(HostAdr * HOST_DATA_RECOD);

	thiskey=RcvBuf[idptx + ELEV_CRT_KEY];
	floor=(thiskey & (~UPDN_READY))-1;


	if( !(thiskey & UPDN_CAR_READY))	return(0);


	aa_start_byte=(ReqHost[HostAdr].Ho_Host[0]/8);  
	aa_start_bit=ReqHost[HostAdr].Ho_Host[0]-1;  
	aa_start_bit=(aa_start_bit % 8);

	aa_bit_chk=(ReqHost[HostAdr].Ho_Host[3]);
	aa_bit_chk=(aa_bit_chk << aa_start_bit);
	aa_bit_chk= ~aa_bit_chk;


	if(thiskey & UP_READY){
		TmpUpKeyTx=&UpKeyTx[floor][aa_start_byte + 1];
		*TmpUpKeyTx=( *TmpUpKeyTx & aa_bit_chk);  
	}


	if(thiskey & DN_READY){
		TmpDnKeyTx=&DnKeyTx[floor][aa_start_byte+1];
		*TmpDnKeyTx=( *TmpDnKeyTx & aa_bit_chk);  
	}

	return(0);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////



unsigned char   __attribute__((section(".usercode"))) UpDownArriveKeyChk(unsigned char tfloor,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{
	unsigned int 	idptx;
	unsigned char	dest_flr=0;
	


	idptx=(ev_hogi * HOST_DATA_RECOD);
	dest_flr=RcvBuf[idptx + DEST_FLR];

	if(RcvBuf[idptx + 0] != (tfloor+1))	return(1);

//if(!(RcvBuf[idptx + 1] & S1_OPEN))	return(1);

	if(ThisHoDnUpInfo == 1){
		if(RcvBuf[idptx + 2] & S2_DN){
			if((RcvBuf[idptx + 1] & S1_OPEN)){	
				return(0);
			}
		}	
		if(dest_flr & DN_READY){
			if( (dest_flr & ONLY_FLR) == (tfloor))	return(0);	
		}
	}
	else if(ThisHoDnUpInfo == 2){
		if(RcvBuf[idptx + 1] & S1_UP){
			if((RcvBuf[idptx + 1] & S1_OPEN)){	
				return(0);
			}
		}
		if(dest_flr & UP_READY){
			if( (dest_flr & ONLY_FLR) == (tfloor))	return(0);	
		}
	}
	return(1);
}




unsigned char   __attribute__((section(".usercode"))) KeyTxValidChk(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{

	unsigned int 	idptx=0;	
	unsigned char 	tmp_hogi=0;
	unsigned char	key_give=0;
	unsigned char	cur_flr,dest_flr=0;

	

	tmp_hogi=ReqHost[ev_hogi].HostGroup;
	idptx=(tmp_hogi * HOST_DATA_RECOD);

	cur_flr =RcvBuf[idptx + 0] -1;
	dest_flr=RcvBuf[idptx + DEST_FLR];
	

	if(RcvBuf[idptx + ELEV_TMP_STATUS] <= 35)	return(1);
	if( !(RcvBuf[idptx + 1] & S1_AUTO))			return(1);
	if(HostElevLive[tmp_hogi] > 30)				return(1);





/*
	if(RcvBuf[idptx + 1] & S1_UP){
		if(cur_flr > tfloor)	return(1);

		if(ThisHoDnUpInfo==1){    					//down key
			if(dest_flr & UP_READY)				return(1);
			if((dest_flr & ONLY_FLR) > tfloor)	return(1);
 		}
		else if(ThisHoDnUpInfo==2){
			if( ( (dest_flr & ONLY_FLR) < tfloor) && ((dest_flr & ONLY_FLR) != cur_flr) ){
				if(dest_flr & UP_READY)	return(1);
			}	
		}
	}
	else if(RcvBuf[idptx + 2] & S2_DN){
		if(cur_flr < tfloor)	return(1);

		if(ThisHoDnUpInfo==1){
			if( ( (dest_flr & ONLY_FLR) > tfloor) && ((dest_flr & ONLY_FLR) != cur_flr) ){
				if(dest_flr & DN_READY)	return(1);
			}	
 		}
		else if(ThisHoDnUpInfo==2){
			if(dest_flr & DN_READY)				return(1);
			if((dest_flr & ONLY_FLR) < tfloor)	return(1);
		}
	}
*/

	return(0);

}



unsigned char   __attribute__((section(".usercode"))) KeyTxClearChk(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{

	unsigned int 	idptx=0;	
	unsigned char 	tmp_hogi=0;
	unsigned char	key_give=1;


    asm("CLRWDT");


	tmp_hogi=ReqHost[ev_hogi].HostGroup;
	idptx=(tmp_hogi * HOST_DATA_RECOD);


	key_give=UpDownArriveKeyChk(tfloor,ThisHoDnUpInfo,ev_hogi);

	if(key_give){
		if( (RcvBuf[idptx + DEST_FLR] & ONLY_FLR) == tfloor){
			if(ThisHoDnUpInfo==1){
				if((RcvBuf[idptx + DEST_FLR] & DN_READY)){
					key_give=0;
				}
			}
			else if(ThisHoDnUpInfo==2){
				if((RcvBuf[idptx + DEST_FLR] & UP_READY)){
					key_give=0;
				}
			}
		}
	}


	if(key_give){
		if( (RcvBuf[idptx + ELEV_CRT_KEY] & (~UPDN_READY)) == (tfloor+1)){
			if(ThisHoDnUpInfo==1){
				if((RcvBuf[idptx + ELEV_CRT_KEY] & DN_READY)){
					key_give=0;
				}
			}
			else if(ThisHoDnUpInfo==2){
				if((RcvBuf[idptx + ELEV_CRT_KEY] & UP_READY)){
					key_give=0;
				}
			}
		}
	}


	return(key_give);

}



unsigned char   __attribute__((section(".usercode"))) Hyundai_Tx_HeaderSet(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{

	unsigned int idptx;	

	unsigned int i;
	unsigned int TmpCnt;	
	unsigned char arrive_sig=0;
	unsigned char tmp_hogi=0;
	unsigned char tmp_dong=0;
	unsigned char buf1[10];	
	unsigned char tmpbuf[20];
	unsigned char tmp_ev[20];
	unsigned char tmp_floor_offset=0;


    asm("CLRWDT");



	StatusUpDnFirstPt=Com1HomeCallTxTotalDataCnt;
	Com1TxCnt=0;

	tmp_hogi=ReqHost[ev_hogi].HostGroup;
	tmp_dong=ReqHost[ev_hogi].Ho_Host[2];
	tmp_floor_offset= (ReqHost[ev_hogi].Ho_Host[4] * 2);


	idptx=(tmp_hogi * HOST_DATA_RECOD);

	
	strcpy(&org_rx_cmd[0],"00000000Type=ELEVATOR&Dong=");
	HexToAsc( &tmpbuf[0],(unsigned int)(BASE_DONG + tmp_dong));
	zeroclear(&tmpbuf[0]);
	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	strcat(&org_rx_cmd[0],"&Ho=");
	tmpbuf[0]=DspFloorName[(tfloor * 2)+     tmp_floor_offset];      
	tmpbuf[1]=DspFloorName[(tfloor * 2)+ 1 + tmp_floor_offset];      
	tmpbuf[2]=0;      


	HexToAsc( &buf1[0],(unsigned int)(Ho));
	strcpy(&buf1[0],&buf1[3]);
	strcat( &tmpbuf[0],&buf1[0]);
	zeroclear(&tmpbuf[0]);
	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	strcat(&org_rx_cmd[0],"&Direction=");
	if(ThisHoDnUpInfo==3)	strcat(&org_rx_cmd[0],"info");
	else if(ThisHoDnUpInfo==1)	strcat(&org_rx_cmd[0],"down");
	else if(ThisHoDnUpInfo==2)	strcat(&org_rx_cmd[0],"up");


	strcat(&org_rx_cmd[0],"&Status=1^");

	tmpbuf[0]=RcvBuf[idptx + DSP1];
	if( !((tmpbuf[0] >= '0') && (tmpbuf[0] <= 'Z'))){
		tmpbuf[0]='0';
	}

	tmpbuf[1]=RcvBuf[idptx + DSP2];
	if( !((tmpbuf[1] >= '0') && (tmpbuf[1] <= 'Z'))){
		tmpbuf[1]='0';
	}

	tmpbuf[2]=0;
	strcat(&org_rx_cmd[0],&tmpbuf[0]);
	strcat(&org_rx_cmd[0],"^");


	if(ThisHoDnUpInfo==3)		strcat(&org_rx_cmd[0],"^");
	else if(ThisHoDnUpInfo==1)	strcat(&org_rx_cmd[0],"down^");
	else if(ThisHoDnUpInfo==2)	strcat(&org_rx_cmd[0],"up^");



	if(RcvBuf[idptx + 1] & S1_UP)					strcat(&org_rx_cmd[0],"up^");
	else if(RcvBuf[idptx + 2] & S2_DN)				strcat(&org_rx_cmd[0],"down^");
	else											strcat(&org_rx_cmd[0],"stop^");

	if(RcvBuf[idptx + ELEV_TMP_STATUS] <= 35)		strcat(&org_rx_cmd[0],"error^");
	else if(!(RcvBuf[idptx + 1] & S1_AUTO))			strcat(&org_rx_cmd[0],"check^");
	else											strcat(&org_rx_cmd[0],"normal^");




	i=(RcvBuf[idptx + DEST_FLR] & ONLY_FLR);

	if(i<32){
		tmpbuf[0]=DspFloorName[(i * 2)+     tmp_floor_offset];      
		tmpbuf[1]=DspFloorName[(i * 2)+ 1 + tmp_floor_offset];      
		tmpbuf[2]=0;      
	}
	else{
		tmpbuf[0]='0';
		tmpbuf[1]='0';
		tmpbuf[2]=0;	
	}
	strcat(&org_rx_cmd[0],&tmpbuf[0]);



	strcat(&org_rx_cmd[0],"&Result=ok");


	strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&org_rx_cmd[0] );
	Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);


	TmpCnt=(Com1HomeCallTxTotalDataCnt-StatusUpDnFirstPt);
	TmpCnt=(TmpCnt-8);

	Com1TxBuffer[StatusUpDnFirstPt +4]=((TmpCnt/1000) + 0x30);
	TmpCnt=(TmpCnt%1000);

	Com1TxBuffer[StatusUpDnFirstPt +5]=((TmpCnt/100) + 0x30);
	TmpCnt=(TmpCnt%100);

	Com1TxBuffer[StatusUpDnFirstPt +6]=((TmpCnt/10) + 0x30);
	Com1TxBuffer[StatusUpDnFirstPt +7]=((TmpCnt%10) + 0x30);


	Com1TxCnt=Com1HomeCallTxTotalDataCnt;
	Com1TxThisPt=0;

	BufferFull=1;



/*
	if(Com1TxCnt > (MAX_COM1_TXBUF - 300)){
		BufferFull=1;
	}
*/

	TxOnece=1;

	return(arrive_sig);
}





unsigned char   __attribute__((section(".usercode"))) Hyundai_Tx_HeaderSet_xx(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{

	unsigned int idptx;	

	unsigned int i;
	unsigned int TmpCnt;	
	unsigned char arrive_sig=0;
	unsigned char tmpbuf[20];
	unsigned char tmp_hogi=0;
	unsigned char tmp_dong=0;
	unsigned char buf1[10];	
	unsigned char StatusNumber[]={"00="};	
	unsigned char tmp_floor_offset=0;



    asm("CLRWDT");



//	StatusUpDnFirstPt=Com1HomeCallTxTotalDataCnt;

	Com1TxCnt=0;

	tmp_hogi=ReqHost[ev_hogi].HostGroup;
	tmp_dong=ReqHost[ev_hogi].Ho_Host[2];
	tmp_floor_offset= (ReqHost[ev_hogi].Ho_Host[4] * 2);

	idptx=(tmp_hogi * HOST_DATA_RECOD);

	if(StatusTxHeader==0){
		strcpy(&org_rx_cmd[0],"00000000Type=ELEVATOR");
		strcat(&org_rx_cmd[0],"&Count=01");
		strcat(&org_rx_cmd[0],"&Status1=");
		StatusUpDnFirstPt=Com1HomeCallTxTotalDataCnt;
	}
	else{
		strcpy(&org_rx_cmd[0],"&Status");
		if( (StatusTxHeader + 1) >= 10){
			StatusNumber[0]= ((StatusTxHeader + 1) / 10) + '0';	
			StatusNumber[1]= ((StatusTxHeader + 1) % 10) + '0';	
			StatusNumber[2]= '=';	
			StatusNumber[3]= 0;

			Com1TxBuffer[StatusUpDnFirstPt + 28]=StatusNumber[0];
			Com1TxBuffer[StatusUpDnFirstPt + 29]=StatusNumber[1];
		}
		else{
			StatusNumber[0]= ((StatusTxHeader + 1) % 10) + '0';	
			StatusNumber[1]= '=';	
			StatusNumber[2]= 0;	
			Com1TxBuffer[StatusUpDnFirstPt + 29]=StatusNumber[0];
		}
		strcat(&org_rx_cmd[0], &StatusNumber[0]);
		

	}
	StatusTxHeader++;

	
	HexToAsc( &tmpbuf[0],(unsigned int)(BASE_DONG + tmp_dong));
	zeroclear(&tmpbuf[0]);
	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	strcat(&org_rx_cmd[0],"-");
	tmpbuf[0]=DspFloorName[(tfloor * 2)+     tmp_floor_offset];      
	tmpbuf[1]=DspFloorName[(tfloor * 2)+ 1 + tmp_floor_offset];      
	tmpbuf[2]=0;      


	HexToAsc( &buf1[0],(unsigned int)(Ho));
	strcpy(&buf1[0],&buf1[3]);
	strcat( &tmpbuf[0],&buf1[0]);
	zeroclear(&tmpbuf[0]);
	strcat(&org_rx_cmd[0],&tmpbuf[0]);
	strcat(&org_rx_cmd[0],"^");

	strcat(&org_rx_cmd[0],"1^");


	tmpbuf[0]=RcvBuf[idptx + DSP1];
	if( !((tmpbuf[0] >= '0') && (tmpbuf[0] <= 'Z'))){
		tmpbuf[0]='0';
	}

	tmpbuf[1]=RcvBuf[idptx + DSP2];
	if( !((tmpbuf[1] >= '0') && (tmpbuf[1] <= 'Z'))){
		tmpbuf[1]='0';
	}

	tmpbuf[2]=0;
	strcat(&org_rx_cmd[0],&tmpbuf[0]);
	strcat(&org_rx_cmd[0],"^");


	if(ThisHoDnUpInfo==3)		strcat(&org_rx_cmd[0],"^");
	else if(ThisHoDnUpInfo==1)	strcat(&org_rx_cmd[0],"down^");
	else if(ThisHoDnUpInfo==2)	strcat(&org_rx_cmd[0],"up^");



	if( !UpDownArriveKeyChk( tfloor, ThisHoDnUpInfo,ev_hogi)){
		strcat(&org_rx_cmd[0],"arrive^");
		arrive_sig=1;		
	}
	else if(RcvBuf[idptx + 1] & S1_UP)		strcat(&org_rx_cmd[0],"up^");
	else if(RcvBuf[idptx + 2] & S2_DN)	strcat(&org_rx_cmd[0],"down^");
	else								strcat(&org_rx_cmd[0],"stop^");


	if(RcvBuf[idptx + 15] <= 35)			strcat(&org_rx_cmd[0],"error^");
	else if(!(RcvBuf[idptx + 1] & S1_AUTO))	strcat(&org_rx_cmd[0],"check^");
	else									strcat(&org_rx_cmd[0],"normal^");


	i=(RcvBuf[idptx + DEST_FLR] & ONLY_FLR);

	if(i<32){
		tmpbuf[0]=DspFloorName[(i * 2)+     tmp_floor_offset];      
		tmpbuf[1]=DspFloorName[(i * 2)+ 1 + tmp_floor_offset];      
		tmpbuf[2]=0;      
	}
	else{
		tmpbuf[0]='0';
		tmpbuf[1]='0';
		tmpbuf[2]=0;	
	}
	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	if(Com1HomeCallTxTotalDataCnt==0){
		strcpy( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&org_rx_cmd[0] );
	}
	else{
		strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&org_rx_cmd[0] );
	}

	Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);


	TmpCnt=(Com1HomeCallTxTotalDataCnt-StatusUpDnFirstPt);
	TmpCnt=(TmpCnt-8);

	Com1TxBuffer[StatusUpDnFirstPt +4]=((TmpCnt/1000) + 0x30);
	TmpCnt=(TmpCnt%1000);

	Com1TxBuffer[StatusUpDnFirstPt +5]=((TmpCnt/100) + 0x30);
	TmpCnt=(TmpCnt%100);

	Com1TxBuffer[StatusUpDnFirstPt +6]=((TmpCnt/10) + 0x30);
	Com1TxBuffer[StatusUpDnFirstPt +7]=((TmpCnt%10) + 0x30);


	Com1TxCnt=Com1HomeCallTxTotalDataCnt;
	Com1TxThisPt=0;

	if(Com1TxCnt > (MAX_COM1_TXBUF - 100)){
		BufferFull=1;
	}

	TxOnece=1;


	return(arrive_sig);
}




unsigned int   __attribute__((section(".usercode"))) Hyundai_BitSetFind_Ho(unsigned char funcNm,unsigned char floor)
{
	unsigned char 	i,k,l;
	unsigned char 	bitval,equal_group;
	unsigned char 	slect_room;
	unsigned char	ThisHoDnUpInfo=0;
	unsigned char 	select_ev_hogi=0;
	unsigned char 	ThisHo=0;


	unsigned char *TmpInfo,*TmpUpRegistry,*TmpDnRegistry,*TmpUpKeyRcv,*TmpDnKeyRcv;

	unsigned char ret_CloseCmd;


	i=LastFloor_Ho_Byte;

	switch(LastFloor_Ho_Bit){
		case	0x0:
		case	0x1:
			bitval 		=0x01;
			slect_room	=0;
			k=0;
			break;
		case	0x02:
			bitval 		=0x02;
			slect_room	=1;
			k=1;
			break;
		case	0x04:
			bitval 		=0x04;
			slect_room	=2;
			k=2;
			break;
		case	0x08:
			bitval 		=0x08;
			slect_room	=3;
			k=3;
			break;
		case	0x10:
			bitval 		=0x10;
			slect_room	=4;
			k=4;
			break;
		case	0x20:
			bitval 		=0x20;
			slect_room	=5;
			k=5;
			break;
		case	0x40:
			bitval 		=0x40;
			slect_room	=6;
			k=6;
			break;
		case	0x80:
			bitval 		=0x80;
			slect_room	=7;
			k=7;
			break;
	}

	slect_room = ((LastFloor_Ho_Byte * 8) + slect_room); 


	l=MAX_HO_PER_FLOOR-1;

	TmpInfo=&InfoCall[floor][1];
	TmpUpRegistry=&UpRegistry[floor][1];
	TmpDnRegistry=&DnRegistry[floor][1];
	TmpUpKeyRcv=&UpKeyRcvAck[floor][1];
	TmpDnKeyRcv=&DnKeyRcvAck[floor][1];


	for(;i<l;i++){
		for(;k<8;k++){
			slect_room++;
			ThisHoDnUpInfo=0;

			for(equal_group=0;equal_group<MAX_ELEV;equal_group++){
				if(ReqHost[equal_group].Ho_Host[0] <= slect_room){
					if(ReqHost[equal_group].Ho_Host[1] >= slect_room){
						select_ev_hogi=equal_group;
						equal_group=MAX_ELEV;
					}
				} 
			}

			ThisHo=(slect_room % 4);
			if(ThisHo==0)	ThisHo=4;

			if( *(TmpDnKeyRcv+i) 	& bitval){
				ThisHoDnUpInfo=1;
				Hyundai_Tx_HeaderSet(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi);
				*(TmpDnKeyRcv+i)   = ( *(TmpDnKeyRcv+i)   &   (~bitval));	
			}


			if( *(TmpUpKeyRcv+i) 	& bitval){				
				ThisHoDnUpInfo=2;
				Hyundai_Tx_HeaderSet(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi);
				*(TmpUpKeyRcv+i)   = ( *(TmpUpKeyRcv+i)   &   (~bitval));
			}


			if( *(TmpInfo+i) & bitval){
				ThisHoDnUpInfo=3;
				Hyundai_Tx_HeaderSet(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi);
				*(TmpInfo+i) = ( *(TmpInfo+i) & (~bitval));
			}		


			if(BufferFull==0){
				if( *(TmpDnRegistry+i) 	& bitval){
					ThisHoDnUpInfo=1;
					ret_CloseCmd=Hyundai_Tx_HeaderSet_xx(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi);
					if(ret_CloseCmd ==1){
						*(TmpDnRegistry+i) = ( *(TmpDnRegistry+i) & (~bitval));
					}	
				}
	
				if( *(TmpUpRegistry+i)	& bitval){
					ThisHoDnUpInfo=2;
					ret_CloseCmd=Hyundai_Tx_HeaderSet_xx(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi);
					if(ret_CloseCmd ==1){
						*(TmpUpRegistry+i) = ( *(TmpUpRegistry+i) & (~bitval));
					}
				}
			}



			if(BufferFull==1){
				LastFloor_Ho_Bit = (bitval << 1);
				LastFloor_Ho_Byte= i;
				if(LastFloor_Ho_Bit == 0){
					LastFloor_Ho_Byte= (i+1);
					if(LastFloor_Ho_Byte >= (MAX_HO_PER_FLOOR-1)){
						StatusUpDnTxByteOrder++;
					}
				}
				return(0);
			}	
			else{
				bitval=(bitval << 1);
			}
		}
		bitval=0x01;
		k=0;
	}


	LastFloor_Ho_Bit = 0;
	LastFloor_Ho_Byte= 0;
	StatusUpDnTxByteOrder++;
 
	return(0);
}





unsigned int   __attribute__((section(".usercode"))) Hyundai_KeyTx_Func(unsigned char floor)
{
	unsigned char 	i,k,l;
	unsigned char 	bitval,equal_group;
	unsigned char 	slect_room;
	unsigned char	ThisHoDnUpInfo=0;
	unsigned char 	select_ev_hogi=0;
	unsigned char 	ThisHo=0;

	unsigned char *TmpUpKeyTx,*TmpDnKeyTx;
	unsigned char *TmpUpRegistry,*TmpDnRegistry;


	unsigned char 	OneKeyTx=0;



	i=Key_LastFloor_Ho_Byte;

	switch(Key_LastFloor_Ho_Bit){
		case	0x0:
		case	0x1:
			bitval 		=0x01;
			slect_room	=0;
			k=0;
			break;
		case	0x02:
			bitval 		=0x02;
			slect_room	=1;
			k=1;
			break;
		case	0x04:
			bitval 		=0x04;
			slect_room	=2;
			k=2;
			break;
		case	0x08:
			bitval 		=0x08;
			slect_room	=3;
			k=3;
			break;
		case	0x10:
			bitval 		=0x10;
			slect_room	=4;
			k=4;
			break;
		case	0x20:
			bitval 		=0x20;
			slect_room	=5;
			k=5;
			break;
		case	0x40:
			bitval 		=0x40;
			slect_room	=6;
			k=6;
			break;
		case	0x80:
			bitval 		=0x80;
			slect_room	=7;
			k=7;
			break;
	}

	slect_room = ((Key_LastFloor_Ho_Byte * 8) + slect_room); 


	l=MAX_HO_PER_FLOOR-1;


	TmpUpKeyTx=&UpKeyTx[floor][1];
	TmpDnKeyTx=&DnKeyTx[floor][1];
	
	TmpUpRegistry=&UpRegistry[floor][1];
	TmpDnRegistry=&DnRegistry[floor][1];;


	for(;i<l;i++){
		for(;k<8;k++){
			slect_room++;
			ThisHoDnUpInfo=0;

			for(equal_group=0;equal_group<MAX_ELEV;equal_group++){
				if(ReqHost[equal_group].Ho_Host[0] <= slect_room){
					if(ReqHost[equal_group].Ho_Host[1] >= slect_room){
						select_ev_hogi=equal_group;
						equal_group=MAX_ELEV;
					}
				} 
			}

				ThisHo=(slect_room % 4);
				if(ThisHo==0)	ThisHo=4;


			if(  (*(TmpDnKeyTx+i) & bitval)  && (*(TmpDnRegistry+i) & bitval)){
				ThisHoDnUpInfo=1;
				if( !KeyTxValidChk(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi)){					
					if(Can1UpDnTxCheck(select_ev_hogi,((floor+1) | 0x40))){
						OneKeyTx=1;
					}
				}
			}

			if(OneKeyTx==0){
				if(  (*(TmpUpKeyTx+i) & bitval) && (*(TmpUpRegistry+i) & bitval)){				
					ThisHoDnUpInfo=2;
					if( !KeyTxValidChk(floor,ThisHo,ThisHoDnUpInfo,select_ev_hogi)){
						if(Can1UpDnTxCheck(select_ev_hogi,( (floor+1) | 0x80))){
							OneKeyTx=1;
						}
					}
				}
			}

			if(OneKeyTx==1){
				Key_LastFloor_Ho_Bit = (bitval << 1);
				Key_LastFloor_Ho_Byte= i;
				if(Key_LastFloor_Ho_Bit == 0){
					Key_LastFloor_Ho_Byte= (i+1);
					if(Key_LastFloor_Ho_Byte >= (MAX_HO_PER_FLOOR-1)){
						Key_StatusUpDnTxByteOrder++;
					}
				}
				return(0);
			}	
			else{
				bitval=(bitval << 1);
			}
		}
		bitval=0x01;
		k=0;
	}


	Key_LastFloor_Ho_Bit = 0;
	Key_LastFloor_Ho_Byte= 0;
	Key_StatusUpDnTxByteOrder++;
 
	return(0);
}






unsigned int   __attribute__((section(".usercode"))) GetRealHex(unsigned char *buf)
{
	unsigned char t;
	unsigned int  tCnt=0;
	unsigned int  j,i;
	unsigned int  BsaeCounter;

	j=strlen(buf);
	if(j==5)		BsaeCounter=10000;	
	else if(j==4)	BsaeCounter=1000;	
	else if(j==3)	BsaeCounter=100;	
	else if(j==2)	BsaeCounter=10;	
	else if(j==1)	BsaeCounter=1;	

	for(i=0;*(buf+i);i++){
		t=AscToHex(*(buf+i));
		tCnt=(t * BsaeCounter) + tCnt;
		BsaeCounter=(BsaeCounter/10);
	}
	return(tCnt);
}



unsigned char   __attribute__((section(".usercode"))) SeperateCmd(void)
{
	unsigned int 	i,j,k;
	unsigned int	valid;
	unsigned int  	rxbyte;
	unsigned char 	tmpbuf[40];
	unsigned char 	tmprxbuf[80];
	unsigned char 	tmpfloorbuf[5];
	unsigned char 	tmp_floor_offset;

	rxbyte= ((Com1RxBuffer[5] - '0') * 100);
	rxbyte= ((Com1RxBuffer[6] - '0') * 10) 	+ rxbyte;
	rxbyte= ((Com1RxBuffer[7] - '0'))  		+ rxbyte;


	Com1RxBuffer[rxbyte+8]=0x0;

	strcpy(&tmprxbuf[0],&Com1RxBuffer[0]);


	j=strlen(&TypeHeaderName[0]);
	for(i=0;i<j;i++){
		if(tmprxbuf[i+8] != TypeHeaderName[i]){
			return(0);
		} 	
	}

	strcpy(&tmpbuf[0],&tmprxbuf[i+8]);
	strcpy(&tmprxbuf[0],&tmpbuf[0]);

	for(i=0;tmpbuf[0];i++){
		if(tmpbuf[i] == '&'){
			strcpy(&tmprxbuf[0],&tmpbuf[i+1]);   		// Dong=4001
			tmpbuf[i]=0;

			for(i=0;tmpbuf[0];i++){
				if(tmpbuf[i] == '='){
					iThisDong=GetRealHex(&tmpbuf[i+1]);
					iThisDong=(iThisDong % 100);
					iThisDong=(iThisDong -1);
					break;
				}	
			}
			break;
		}	
	}



	strcpy(&tmpbuf[0],&tmprxbuf[0]);
	for(i=0;tmpbuf[0];i++){
		if(tmpbuf[i] == '&'){
			strcpy(&tmprxbuf[0],&tmpbuf[i+1]);		// Ho=101
			tmpbuf[i]=0;

			for(i=0;tmpbuf[0];i++){
				if(tmpbuf[i] == '='){
					iThisHo=GetRealHex(&tmpbuf[i+1]);
					iThisHo=(iThisHo % 100);					
					strcpy(&tmpbuf[0],&tmpbuf[i+1]);		// Ho=101
					strcpy(&tmpfloorbuf[0],"0000");		// Ho=101

					valid=0;
					for(i=0;i<MAX_ELEV;i++){
						if(ReqHost[i].Ho_Host[2]== iThisDong){
							tmp_floor_offset=ReqHost[i].Ho_Host[4];
							valid=1;	
						}
					}
					if(valid==0)	return(0);


					j=strlen(&tmpbuf[0]);
					if(j==3){
						strcpy(&tmpfloorbuf[1],&tmpbuf[0]);		// Ho=101
					}
					else{
						strcpy(&tmpfloorbuf[0],&tmpbuf[0]);		// Ho=101
					}
					tmpfloorbuf[2]=0x0;
					

					j=0;
					for(i=0;DspFloorName[i];i=(i+2),j++){
						if(tmpfloorbuf[0]== DspFloorName[i]){
							if(tmpfloorbuf[1]== DspFloorName[i+1]){
								iThisFlow=(j-tmp_floor_offset);
								break;
							}
						}	
					}
					break;
				}	
			}
			break;
		}	
	}


////////////////////////modify project////////////////////
//////////////////////////////////////////////////////////
	valid=0;
	for(i=0;i<MAX_ELEV;i++){
		if(ReqHost[i].Ho_Host[2]== iThisDong){
			valid=1;	
		}
	}
	if(valid==0)	return(0);
	
	if(iThisHo > 4)		return(0);

	iThisDong=(iThisDong * 4);
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


	iThisLine=(iThisDong + iThisHo); 


	strcpy(&tmpbuf[0],&tmprxbuf[0]);
	for(i=0;tmpbuf[0];i++){
		if(tmpbuf[i] == '='){
			strcpy(&tmprxbuf[0],&tmpbuf[i+1]);
			break;
		}	
	}


	iThisCmdSort=0;
	strcpy(&tmpbuf[0],&tmprxbuf[0]);
	for(j=0;j<3;j++){
		k=1;
		for(i=0;Hyundai_Local_Call_UpDn[j][i];i++){
			if(tmpbuf[i] != Hyundai_Local_Call_UpDn[j][i]){
				k=1;
				break;
			} 	
			else{
				k=0;
			}
		}

		if(k==0){
			iThisCmdSort=j+1;
			break;
		}
	}


	switch(iThisCmdSort){
		case	1:
		case	2:
		case	3:
			Hyundai_UpDnCallSet();
			TxGoOnTime=0;		
			break;
	}
	return(0);		
}



unsigned char   __attribute__((section(".usercode"))) CmdChk(void)
{
	unsigned char  tmpadr;
	tmpadr=SeperateCmd();
	return(0);
}




unsigned int   __attribute__((section(".usercode"))) Hyundai_InfoCmdReturn(void)
{
	unsigned int i;


	if(StatusUpDnTxByteOrder > 31)		StatusUpDnTxByteOrder=0;


	for(;1;){
		Hyundai_BitSetFind_Ho(3,StatusUpDnTxByteOrder);

		if(StatusUpDnTxByteOrder > 31){
			if(TxOnece==1){
				TxOnece=0;
			}
			return(0);
		}

		if(BufferFull==1){
			return(0);
		}
	}

	return(0);
}




unsigned int   __attribute__((section(".usercode"))) CurStatusCmdReturnReSetting(void)
{

	unsigned int i;


	if(StatusTxHeader > 0){
		if(StatusTxHeader < 10){
			for(i=0;Com1TxBuffer[StatusUpDnFirstPt+29+i];i++){
				Com1TxBuffer[StatusUpDnFirstPt+28+i] = Com1TxBuffer[StatusUpDnFirstPt+29+i]; 
			}
			Com1TxBuffer[StatusUpDnFirstPt+28+i] = 0;

			i=(StatusTxHeader % 10);		
			Com1TxBuffer[StatusUpDnFirstPt + 28]=(i + 0x30);

			Com1TxCnt--;

		}
		else{
			i=(StatusTxHeader/10);		
			Com1TxBuffer[StatusUpDnFirstPt + 28]=(i + 0x30);
			i=(StatusTxHeader % 10);		
			Com1TxBuffer[StatusUpDnFirstPt + 29]=(i + 0x30);
		}


		i=strlen( &Com1TxBuffer[StatusUpDnFirstPt+8]);
		Com1TxBuffer[StatusUpDnFirstPt + 4]=((i/1000) + 0x30);
		i=(i%1000);
		Com1TxBuffer[StatusUpDnFirstPt + 5]=((i/100) + 0x30);
		i=(i%100);
		Com1TxBuffer[StatusUpDnFirstPt+6]=((i/10) + 0x30);
		Com1TxBuffer[StatusUpDnFirstPt+7]=((i%10) + 0x30);

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


unsigned int   __attribute__((section(".usercode"))) HomeCallBaseHeaderchk(void)
{
	if(Com1RxCnt == 6){
	 	if(Com1RxBuffer[0] != '0')	return(0);
	 	if(Com1RxBuffer[1] != '0')	return(0);
	 	if(Com1RxBuffer[2] != '0')	return(0);
	 	if(Com1RxBuffer[3] != '0')	return(0);
	 	if(Com1RxBuffer[4] != '0')	return(0);
	 	if(Com1RxBuffer[5] != '0')	return(0);
	}
	else if(Com1RxCnt < 6){
 		if(Com1RxBuffer[Com1RxCnt-1] != '0')	return(0);
	}	
	return(1);
}



unsigned int   __attribute__((section(".usercode"))) HomeCallProtocolChk(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;


   	buf=rdata;

	if(Com1SerialTime > 100){
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


		if(!HomeCallBaseHeaderchk()){
			Com1RxCnt=0;
			return(0);
		}


		if(Com1RxCnt < 8){
			return(0);
		}
		else{
			if(Com1RxCnt == 8){
				Com1HomeCallRxDataCnt=DataCntCheck();
				if(Com1HomeCallRxDataCnt==0){
		            Com1RxCnt=0;
					return(0);
				}
			}
			else{
				if( ((unsigned int)Com1RxCnt) >= (Com1HomeCallRxDataCnt + 8)){
						CmdChk();
			            Com1RxCnt=0;
						return(0);												
				}		
			}
		}

	return(0);
}


/*
unsigned int   __attribute__((section(".usercode"))) MoniteringCan2(void)
{
	unsigned char j;

	if(Can2TxCnt > Can2TxThisPt){
		Can2TxSid=0;
		Can2TxEid=0;
		j=Can2TxCnt-Can2TxThisPt;
		if(j > 8)	j=8;
		Can2TxDlc=j;

Can2PollingTimer=11;

		if( !Can2TxData(Can2TxThisPt,0)){
			Can2TxThisPt= (Can2TxThisPt+j);
		}
	}
}
*/

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


unsigned int   __attribute__((section(".usercode"))) My_Project_Start(void)
{
	unsigned int retx,i,j;
		
///////////////	MoniteringCan2();

	if(I_AM_LISTENING_MODE == 0){
		if(ThisHostSelect >= MAX_ELEV){
			ThisHostSelect=0;
		}
	
		if(Can1HostDataReq(ThisHostSelect)){
			ThisHostSelect++;
		}
	}


	if(Key_StatusUpDnTxByteOrder > 31)	Key_StatusUpDnTxByteOrder=0;
	Hyundai_KeyTx_Func(Key_StatusUpDnTxByteOrder);



	if(Com1RxStatus == TX_SET){
		StatusTxTime=0;
		return(0);
	}	

	if(StatusTxTime < 4)	return(0);					


	Com1TxBuffer[0]=0;
	Com1HomeCallTxTotalDataCnt=0;	
	BufferFull=0;

	if(BufferFull==0){
		Hyundai_InfoCmdReturn();
	}




	if(Com1HomeCallTxTotalDataCnt > 0){
		Com1TxStart();
		Com1HomeCallTxTotalDataCnt=0;
		StatusTxHeader=0;
		StatusTxTime=0;

		if(TxGoOnTime > 300){						//5분
			for(j=0;j<32;j++){
				for(i=0;i<MAX_HO_PER_FLOOR;i++){
					UpRegistry[j][i]=0;  								        
					DnRegistry[j][i]=0;  		
					InfoCall[j][i]=0;   
					UpKeyTx[j][i]=0;
					DnKeyTx[j][i]=0;
					UpKeyRcvAck[j][i]=0;
					DnKeyRcvAck[j][i]=0;

				}
			}
		}
	}


	return(0);
}







void  __attribute__((section(".usercode")))   Filter_Mask_Set(void)
{

	Can1RxSidFilter=CRT_COMPANY;
	Can1RxEidFilter=0;
	Can1RxSidMask=0x007f;
	Can1RxEidMask=0x0000;


	Can2RxSidFilter=CRT_COMPANY;
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



void    GetTxSidEid(unsigned char tmpCompany,unsigned char tmpGroup,unsigned char tmpLocal)
{
    unsigned int  itmp1,itmp2;
    unsigned long ltmp1;

    itmp1=(unsigned int)(tmpCompany);
    itmp2=(unsigned int)(tmpGroup);
    itmp2=(itmp2 << 7);
    Can1TxSid=(unsigned int)(itmp1 | itmp2);


	Can1TxEid=(unsigned long)(tmpLocal);
	Can1TxEid = (Can1TxEid << 14);
	if(tmpGroup & 0x20) 
	Can1TxEid = (Can1TxEid | 0x200000);

	Can1TxEid = (Can1TxEid | 0x2000);
}



unsigned int   __attribute__((section(".usercode"))) My_Project_Init(void)
{
	unsigned int i,j;

	unsigned	char	tmpCompany,tmpGroupadr,tmpLocaladr;

 	TRIS_RG13_IO=1;
	RG13_OUT=1;
	RG13_I=1;


    Com1Init(COM1_BAUD);
    Com2Init(COM2_BAUD);

	Filter_Mask_Set();

	for(i=0;i<(CAN1_MAX_RX_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_TX_BUF-1);i++)	Can1TxBuf[i]=0;

	Com1TxThisPt =0;
	Com1TxCnt=0;

	for(i=0;i<MAX_ELEV;i++){
		HostElevLive[i]=100;	
		tmpCompany=CRT_COMPANY;
		tmpGroupadr=i;
		tmpLocaladr=0;    
		GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
			
		if(Can1TxSid & 0x800){
			Can1TxSid= (Can1TxSid & 0x7ff);
			Can1TxEid=(Can1TxEid | 0x20000);
		}	

		ReqHost[i].HostSid=Can1TxSid;
		ReqHost[i].HostEid=Can1TxEid;
		ReqHost[i].HostGroup=i;

		j=(i * 2);
				
		ReqHost[i].Ho_Host[0]=j+1;					// line = 1,2  3,4 
		ReqHost[i].Ho_Host[1]=j+2;
		ReqHost[i].Ho_Host[2]=(i/2);				//j=1,2,3,4 line  == 1동  
		ReqHost[i].Ho_Host[3]=0x03;      			//elev_data_number
		ReqHost[i].Ho_Host[4]=BASE_FLOOR_B2;       // BASE_FLOOR= B2    //0=B7 1=B6 2=B5 3=B4 4=B3 5=B2 6=B1 7=01
	}


	if(BASE_DONG == 4001){
		//4001동
		ReqHost[2].Ho_Host[4]=BASE_FLOOR_B1;        // BASE_FLOOR = B1 (4002동  12 line)     
		ReqHost[3].Ho_Host[4]=BASE_FLOOR_B1;        // BASE_FLOOR = B1 (4002동  34 line)     
	}

/*
	if(BASE_DONG == 4501){
		//4501동
		for(i=6;i<MAX_ELEV;i++){						// 7호기 부터 B1(4504,4505,4506,4507동)
			ReqHost[i].Ho_Host[4]=BASE_FLOOR_B1;        // BASE_FLOOR = B1      
		}
	}
*/





/*
	for(i=0;i<MAX_ELEV;i++)		HostElevLive[i]=100;	
	tmpCompany=CRT_COMPANY;
	tmpGroupadr=0;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);
		
	ReqHost[0].HostSid=Can1TxSid;
	ReqHost[0].HostEid=Can1TxEid;
	ReqHost[0].HostGroup=0;
	ReqHost[0].Ho_Host[0]=1;
	ReqHost[0].Ho_Host[1]=2;
	ReqHost[0].Ho_Host[3]=0xff;
	ReqHost[0].Ho_Host[4]=0xff;


	tmpCompany=CRT_COMPANY;
	tmpGroupadr=1;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[1].HostSid=Can1TxSid;
	ReqHost[1].HostEid=Can1TxEid;
	ReqHost[1].HostGroup=1;
	ReqHost[1].Ho_Host[0]=3;
	ReqHost[1].Ho_Host[1]=4;
	ReqHost[1].Ho_Host[3]=0xff;
	ReqHost[1].Ho_Host[4]=0xff;



	tmpCompany=CRT_COMPANY;
	tmpGroupadr=2;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[2].HostSid=Can1TxSid;
	ReqHost[2].HostEid=Can1TxEid;
	ReqHost[2].HostGroup=2;
	ReqHost[2].Ho_Host[0]=5;
	ReqHost[2].Ho_Host[1]=6;
	ReqHost[2].Ho_Host[3]=0xff;
	ReqHost[2].Ho_Host[4]=0xff;


	tmpCompany=CRT_COMPANY;
	tmpGroupadr=3;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[3].HostSid=Can1TxSid;
	ReqHost[3].HostEid=Can1TxEid;
	ReqHost[3].HostGroup=3;
	ReqHost[3].Ho_Host[0]=7;
	ReqHost[3].Ho_Host[1]=8;
	ReqHost[3].Ho_Host[3]=0xff;
	ReqHost[3].Ho_Host[4]=0xff;



	tmpCompany=CRT_COMPANY;
	tmpGroupadr=4;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[4].HostSid=Can1TxSid;
	ReqHost[4].HostEid=Can1TxEid;
	ReqHost[4].HostGroup=4;
	ReqHost[4].Ho_Host[0]=9;
	ReqHost[4].Ho_Host[1]=12;
	ReqHost[4].Ho_Host[3]=0xff;
	ReqHost[4].Ho_Host[4]=0xff;



	tmpCompany=CRT_COMPANY;
	tmpGroupadr=5;
	tmpLocaladr=0;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[5].HostSid=Can1TxSid;
	ReqHost[5].HostEid=Can1TxEid;
	ReqHost[5].HostGroup=5;
	ReqHost[5].Ho_Host[0]=9;
	ReqHost[5].Ho_Host[1]=12;
	ReqHost[5].Ho_Host[3]=0xff;
	ReqHost[5].Ho_Host[4]=0xff;
*/

/*
	for(i=0;i<MAX_ELEV;i++){
		j=(i * HOST_DATA_RECOD);
		RcvBuf[j + 0]= 1;
		RcvBuf[j + DEST_FLR]= 5;
		RcvBuf[j + DSP1]= (i/10) + '0';
		RcvBuf[j + DSP2]= (i%10) + '0';
		RcvBuf[j + ELEV_TMP_STATUS]= 0;
	}
*/
	return(0);	
}



unsigned char __attribute__((section(".usercode"))) msecTimer1(void)
{
	return(0);
}


unsigned char __attribute__((section(".usercode"))) MsecTimer_100(void)
{
	if(StatusTxTime < 100)			StatusTxTime++;

	if(RG13_I)	RUN_LED=1;


	return(0);
}


unsigned char __attribute__((section(".usercode"))) SecTimer_1(void)
{
	unsigned char	i;

	for(i=0;i<MAX_ELEV;i++){          
		if(HostElevLive[i] < 100)	HostElevLive[i]++;
	}
	
	if(ElevatorServerAliveTime < 250)	ElevatorServerAliveTime++;

/////////////	
	if(TxGoOnTime  < 10000)			TxGoOnTime++;

	
	return(0);
}

