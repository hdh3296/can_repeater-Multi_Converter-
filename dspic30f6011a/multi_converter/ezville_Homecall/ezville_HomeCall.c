
#include <p30f6011a.h>            


#include  	"..\multi_converter_common\host_io.h"            
#include  	"..\multi_converter_common\30f6010_io.h"
#include  	"..\multi_converter_common\you_can1.h"
#include  	"..\multi_converter_common\you_can2.h"
#include  	"..\multi_converter_common\ComPort1.h"
#include  	"..\multi_converter_common\ComPort2.h"
#include  	"..\multi_converter_common\memory_map.h"
#include  	"..\multi_converter_common\WorkingRam.h"
#include  	"..\multi_converter_common\Multi_converter.h"

#include	"ezville_HomeCall.h"



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


	
unsigned char   UpRegistry[32][MAX_HO_PER_FLOOR];  		///Ãþ //¼¼´ë¼ö							        
unsigned char   DnRegistry[32][MAX_HO_PER_FLOOR];  		///Ãþ //¼¼´ë¼ö
unsigned char   UpCall[32][MAX_HO_PER_FLOOR];      		///Ãþ //¼¼´ë¼ö							        
unsigned char   DnCall[32][MAX_HO_PER_FLOOR];      		///Ãþ //¼¼´ë¼ö
unsigned char   InfoCall[32][MAX_HO_PER_FLOOR];    		///Ãþ //¼¼´ë¼ö
unsigned char   LocationCall[32][MAX_HO_PER_FLOOR];    	///Ãþ //¼¼´ë¼ö
unsigned char   CloseCall[32][MAX_HO_PER_FLOOR];    	///Ãþ //¼¼´ë¼ö
unsigned char   UpKeyTx[32][MAX_HO_PER_FLOOR];
unsigned char   DnKeyTx[32][MAX_HO_PER_FLOOR];
unsigned char   UpCommonCall[32][MAX_HO_PER_FLOOR];   /// Çö°ü 
unsigned char   DnCommonCall[32][MAX_HO_PER_FLOOR];   /// Çö°ü 
unsigned char   LocationCallTime[32][MAX_HO_PER_FLOOR];               // 01,02,03,04,05,06  È£    


//bit map
//UpRegistry,DnRegistry(1-32Ãþ)     
//									byte0	byte1	byte2	byte3	byte4	byte5	byte6
//									byte0= host select(0 = ¾øÀ½,1=local0, 1=local1,2=local2 ....  0=1Ãþ 31=32Ãþ
//									byte1-byte6= ÃþÀÇ È£(101È£,102È£ ....148È£±îÁö) 
//UpCall,DnCall,InfoCall(1-32Ãþ)
//									byte0	byte1	byte2	byte3	byte4	byte5	byte6
//									byte0= not use 
//									byte1-byte6= ÃþÀÇ È£(101È£,102È£ ....148È£±îÁö)  call ¶Ç´Â Á¤º¸¿äÃ»Ãþ





unsigned 	char 	ThisHostSelect=0; 



// Çö°ü Àç ¼³Á¤
const unsigned char eZvilleCommonCall_Recopyx[][7][5]= { {"B301","B201","B101","cccc","cccc","0301","xxxx"},{"B303","B203","B103","cccc","cccc","0303","xxxx"},{"B305","B205","B105","cccc","cccc","0305","xxxx"},{"B307","B207","B107","cccc","cccc","0307","xxxx"},{"B309","B209","B109","cccc","cccc","0309","xxxx"}}; 
const unsigned char eZvilleCommonCall_Orgx[][7][5]   = { {"9004","9003","9002","cccc","cccc","9001","xxxx"},{"9008","9007","9006","cccc","cccc","9005","xxxx"},{"9012","9011","9010","cccc","cccc","9009","xxxx"},{"9016","9015","9014","cccc","cccc","9013","xxxx"},{"9020","9019","9018","cccc","cccc","9017","xxxx"}}; 



//const unsigned char DspFloorName[] = {"B3B2B10102030405060708091011121314151617181920212223242526272829303132"}; 

const unsigned char DspFloorName[] = {"B3B2B1030405060708091011121314151617181920212223242526272829303132"}; 

 

const unsigned char eZvilleAliveCmd[]              = {"<start=0000&0>$version=2.0$copy=0-0$cmd=10$dongho=100&700$target=server"}; 
const unsigned char eZvilleReturnCmd[]             = {"<start=0000&0>$version=2.0$copy=0-0$cmd=11$target=gateway#dongho=100&700#ip=10.100.0.254#status=0"}; 


const unsigned char eZvilleHeaderCmd[][10] = {"version","cmd","copy","dongho","target"}; 
const unsigned char eZvilleTarget[][20]    = {"target=gateway","target=elevator"}; 
const unsigned char eZvilleLocalCmd[][15]  = {"mode","dongho","direction","riding_ev","destination","nm","flow","status"}; 


const unsigned char eZville_Local_mode[][15]      ={"mode=close","mode=open","mode=location","mode=call","mode=current"};

const unsigned char eZville_Local_Call_UpDn[2][15]={"direction=down","direction=up"};


unsigned 	int  	StatusTxTime=0; 
unsigned 	int  	Com1HomeCallTxTotalDataCnt=0; 
unsigned 	int  	Com1HomeCallRxDataCnt; 
unsigned 	char 	Com1HomeCallFloor;
unsigned 	char 	Com1HomeCallHo; 

unsigned 	char 	Com1HomeCallCurReqWard; 
unsigned 	char 	BufferFull=0; 

unsigned 	char	ThisSystemTopFloor=0;


unsigned 	int 	StatusUpDnTxByteOrder=0;
unsigned 	int 	StatusUpDnFirstPt=0;
unsigned 	int 	StatusTxOrder=0;
unsigned 	int 	StatusTxHeader=0;

unsigned 	char 	LastFloor_Ho_Byte=0;
unsigned 	char 	LastFloor_Ho_Bit=0;



unsigned char org_rx_cmd[200]; 



unsigned 	int		ElevatorServerAliveTime=0;


unsigned 	char	Cmd_Nm=0;
unsigned 	char	EZB_MyReturnMode=0;
unsigned 	char	TxOnece=0;


/////////////////////////////
//0:start
//1:version
//2:cmd
//3:copy
//4:dongho
//5:target
unsigned 	char	StartCmd[15];
unsigned 	char	VersionCmd[15];
unsigned 	char	CmdCmd[15];
unsigned 	char	CopyCmd[15];
unsigned 	char	DongHoCmd[15];
unsigned 	char	TargetCmd[100];



unsigned 	char	Local_mode[20];
unsigned 	char	Local_dongho[20];
unsigned 	char	Local_direction[20];
unsigned 	char	Local_riding_ev[20];
unsigned 	char	Local_destination[30];
unsigned 	char	Local_nm[20];
unsigned 	char	Local_flow[20];
unsigned 	char	Local_status[15];




unsigned    char    HomeCallUpKey[4]={0,0,0,0};unsigned    char    HomeCallDnKey[4]={0,0,0,0};unsigned    char    HomeCallCarKey[4]={0,0,0,0};

unsigned	char  	HostElevLive[MAX_ELEV];
unsigned	char  	RcvKeyBuf[5];     


/*
unsigned char __attribute__((section(".usercode"))) BitChk(unsigned char *buf,unsigned char val_bit)
{
	if(*buf & val_bit)	return(1);
	else				return(0);
}

unsigned char __attribute__((section(".usercode"))) BitSet(unsigned char *buf,unsigned char val_bit)
{
	*buf = (*buf | val_bit);
	return( *buf );
}

unsigned char __attribute__((section(".usercode"))) BitClear(unsigned char *buf,unsigned char val_bit)
{
	*buf=( *buf & (~val_bit));
	return( *buf );
}

*/








//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

unsigned int   __attribute__((section(".usercode"))) HomeCallButtonCallCmd(unsigned char *buf)
{
	unsigned char i,j,k,value_bit;

	if( *(buf+0) == ((unsigned char)(HOMECALL_BUTTON_SET_CMD))){	
		k= ( (*(buf+1)) & 0x3f);
		value_bit=0x01;

		i=((k-1) / 8);
		j=((k-1) % 8);

		value_bit =(value_bit << j);

		if( *(buf+1) & 0x80){
			HomeCallUpKey[i] = (HomeCallUpKey[i] | value_bit);	
		}
		if( *(buf+1) & 0x40){
			HomeCallDnKey[i] = (HomeCallDnKey[i] | value_bit);	
		}
	}

}



unsigned int	UpFloorAllclearChk(unsigned char tfloor)
{
	unsigned int i,j;
	
	for(j=1;j<MAX_HO_PER_FLOOR;j++){
		if(UpRegistry[tfloor][j] != 0)	return(0);
	}
	UpRegistry[tfloor-1][0]=0;

	return(0);
}

unsigned int	DnFloorAllclearChk(unsigned char tfloor)
{
	unsigned int i,j;
	
	for(j=1;j<MAX_HO_PER_FLOOR;j++){
		if(DnRegistry[tfloor][j] != 0)	return(0);
	}
	DnRegistry[tfloor-1][0]=0;

	return(0);
}



unsigned char	UpDownArriveKeyChk(unsigned char xfloor,unsigned char DownUp)
{

	unsigned char ret,i,j,value_bit=0x01;
        
	ret=1;

	i=((xfloor-1) / 8);
	j=((xfloor-1) % 8);

	value_bit =(value_bit << j);

	if(DownUp==2){
		if( !(HomeCallUpKey[i] & value_bit)){
			ret=0;
		}
	}
	else if(DownUp==1){
		if( !(HomeCallDnKey[i] & value_bit)){
			ret=0;
		}
	}
	return(ret);
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

unsigned char   __attribute__((section(".usercode"))) DataCntCheck(void)
{
	unsigned char t;
	unsigned int  tCnt;

	tCnt=0;

	if( (Com1RxBuffer[10] >= '0') &&  (Com1RxBuffer[10] <= '9')){
		t=AscToHex(Com1RxBuffer[7]);
		tCnt=((unsigned int)(t * 1000) + tCnt);
	
		t=AscToHex(Com1RxBuffer[8]);
		tCnt=((unsigned int)(t * 100) + tCnt);
	
		t=AscToHex(Com1RxBuffer[9]);
		tCnt=((unsigned int)(t * 10) + tCnt);
	
		t=AscToHex(Com1RxBuffer[10]);
		tCnt=((unsigned int)(t * 1) + tCnt);
	}
	else{
		t=AscToHex(Com1RxBuffer[7]);
		tCnt=((unsigned int)(t * 100) + tCnt);
	
		t=AscToHex(Com1RxBuffer[8]);
		tCnt=((unsigned int)(t * 10) + tCnt);
	
		t=AscToHex(Com1RxBuffer[9]);
		tCnt=((unsigned int)(t * 1) + tCnt);
	}


	return(tCnt);
}






















/////////// tx string load///////////////////////////////


unsigned char   __attribute__((section(".usercode"))) eZville_Floor_Ho_Find(unsigned char *buf)
{
	unsigned char i,j;

	Com1HomeCallFloor=0xff;


	Com1HomeCallHo=0;
	i=AscToHex( *(buf+2));
	Com1HomeCallHo=((unsigned char)(i * 10) + Com1HomeCallHo);
	i=AscToHex( *(buf+3));
	Com1HomeCallHo=((unsigned char)(i * 1) + Com1HomeCallHo);


	j=0;
	for(i=0;i<32;i++){
		j=(i*2);
		if(DspFloorName[j] == *(buf + 0)){
			j++;
			if(DspFloorName[j] == *(buf + 1)){
				Com1HomeCallFloor=(i+1);
				i=0xfe;
			}
		}	
	}

	return(Com1HomeCallFloor);

}




unsigned char   __attribute__((section(".usercode"))) eZville_CommonButtonChk(void)
{
	unsigned char i;

	for(i=0;(Local_dongho[i] != '&');i++);
	i++;

	if(strlen(&Local_dongho[i]) == 4){
		if( (Local_dongho[i] == '9') && (Local_dongho[i+1] == '0')){
			if( (Local_dongho[i+2] >= '0') && (Local_dongho[i+2] <= '9')){
				if( (Local_dongho[i+3] >= '0') && (Local_dongho[i+3] <= '9')){
					return(1);
				}
			}  
		}
	}
	return(0);
}



unsigned int   __attribute__((section(".usercode"))) eZville_UpDnCallSet(void)
{
	unsigned char i,j,k,value_bit;

	k=Com1HomeCallHo-1;
	value_bit=0x01;

	i=(k / 8);
	j=(k % 8);
	value_bit =(value_bit << j);
	i++;


	k = (RcvKeyBuf[1] & 0x3f);
	k=(k-1);


	if(CmdCmd[5] == '1')	return(0); 

	if(EZB_MyReturnMode==1){
		CloseCall[k][i] = (CloseCall[k][i] | value_bit);	
	}
	else if(EZB_MyReturnMode==2){
		InfoCall[k][i] = (InfoCall[k][i] | value_bit);	
		LocationCall[k][i] = (LocationCall[k][i] | value_bit);
		LocationCallTime[k][i+j] = 0;
	
	}
	else if(EZB_MyReturnMode==3){
		LocationCall[k][i] = (LocationCall[k][i] | value_bit);	
		LocationCallTime[k][i+j] = 0;
	}
	else{
		if(RcvKeyBuf[1] & 0x80){
			if(eZville_CommonButtonChk()){
				UpCommonCall[k][i]= (UpCommonCall[k][i] | value_bit);
				UpKeyTx[k][i] = (UpKeyTx[k][i] | value_bit);	
			}
			else{
				UpRegistry[k][i] = (UpRegistry[k][i] | value_bit);	
				UpCall[k][i] = (UpCall[k][i] | value_bit);	
				UpKeyTx[k][i] = (UpKeyTx[k][i] | value_bit);	
				LocationCallTime[k][i+j] = 0;
			}
		}
	
		else if(RcvKeyBuf[1] & 0x40){
			if(eZville_CommonButtonChk()){
				DnCommonCall[k][i]= (DnCommonCall[k][i] | value_bit);
				DnKeyTx[k][i] = (DnKeyTx[k][i] | value_bit);	
			}
			else{
				DnRegistry[k][i] = (DnRegistry[k][i] | value_bit);	
				DnCall[k][i]  = (DnCall[k][i] | value_bit);	
				DnKeyTx[k][i] = (DnKeyTx[k][i] | value_bit);	
				LocationCallTime[k][i+j] = 0;
			}
		}
	}



	return(0);
}




unsigned char	eZville_UpDownArriveKeyChk(unsigned char ev_Hogi,unsigned char tfloor,unsigned char ThisHoDnUpInfo)
{
	unsigned int idptx;
	unsigned char buf;


	if(ev_Hogi > 0)	idptx=((ev_Hogi-1) * HOST_DATA_RECOD);
	else			idptx=((ev_Hogi) * HOST_DATA_RECOD);


	if( (ThisHoDnUpInfo != 4) &&	 (ThisHoDnUpInfo != 5))	return(0);
	if( (tfloor + 1) != RcvBuf[idptx + 0])					return(0);
	if( !(S1_OPEN & RcvBuf[idptx + 1]))						return(0);




	if(ThisHoDnUpInfo == 4){
		buf=RcvBuf[idptx + 2];
		if(S2_DN & buf)					return(1);
		else{									
			return(0);
		}		
	}
	else if(ThisHoDnUpInfo == 5){
		buf=RcvBuf[idptx + 1];
		if(S1_UP & buf)					return(2);		
		else{
			return(0);
		}		
	}

	return(0);
}


unsigned char   __attribute__((section(".usercode"))) eZville_HomeCallProtocolDataLoad(unsigned char ev_Hogi,unsigned char tfloor,unsigned char ThisHoDnUpInfo)
{

	unsigned char arrive_sig=0;
	unsigned int i;
	unsigned int idptx;	

	unsigned char tmpbuf[30];

    asm("CLRWDT");



	if(ev_Hogi > 0)	idptx=((ev_Hogi-1) * HOST_DATA_RECOD);
	else 			idptx=((ev_Hogi) * HOST_DATA_RECOD);



	strcpy(&tmpbuf[0],"#no=0");

	if(ev_Hogi > 9){
		tmpbuf[4]=(ev_Hogi/10) + '0';
		tmpbuf[5]=(ev_Hogi%10) + '0';
		tmpbuf[6]=0;
	}
	else{
		tmpbuf[4]=ev_Hogi + '0';
		tmpbuf[5]=0;
	}
	strcpy(&org_rx_cmd[0],&tmpbuf[0]);


	strcpy(&tmpbuf[0],"#direction=");
	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	arrive_sig=eZville_UpDownArriveKeyChk(ev_Hogi,tfloor,ThisHoDnUpInfo);
	if(arrive_sig){
		strcpy(&tmpbuf[0],"arrive");
	}	
	else{
		if(S1_UP & RcvBuf[idptx + 1]){
			strcpy(&tmpbuf[0],"up");
		}
		else if(S2_DN & RcvBuf[idptx + 2]){
			strcpy(&tmpbuf[0],"down");
		}
		else{
			strcpy(&tmpbuf[0],"stop");
		}
	}
	strcat(&org_rx_cmd[0],&tmpbuf[0]);




	strcpy(&tmpbuf[0],"#flow=");
	tmpbuf[6]= RcvBuf[idptx + 6];
	tmpbuf[7]= RcvBuf[idptx + 7];
	tmpbuf[8]= 0x0;

	if(tmpbuf[6] < 0x20)	tmpbuf[6]='0';	
	if(tmpbuf[7] < 0x20)	tmpbuf[7]='0';	
	if(tmpbuf[6] > 0x7f)	tmpbuf[6]='0';	
	if(tmpbuf[7] > 0x7f)	tmpbuf[7]='0';	

	strcat(&org_rx_cmd[0],&tmpbuf[0]);


	return(arrive_sig);
}




unsigned char   __attribute__((section(".usercode"))) eZville_Tx_HeaderSet(unsigned char tfloor,unsigned char Ho,unsigned char ThisHoDnUpInfo,unsigned char ev_hogi)
{


	unsigned int idptx;	

	unsigned int i;
	unsigned int TmpCnt;	


	unsigned char arrive_sig=0;
	unsigned char tmpbuf[20];
	unsigned char riding_ev[20];
	unsigned char ev_flow[20];
	unsigned char tmp_hogi=0;

	unsigned char tmpCommondongHo[20];
	unsigned char TmpCmd[10]={"$cmd=30"};	

    asm("CLRWDT");


	     if(ev_hogi == 1)						tmp_hogi=0;	
	else if(ev_hogi == 2)						tmp_hogi=1;	
	else if(ev_hogi == 3) 						tmp_hogi=2;	
	else if(ev_hogi == 4) 						tmp_hogi=3;	
	else if((ev_hogi == 5) || (ev_hogi == 6))	tmp_hogi=4;	


	StatusUpDnFirstPt=Com1HomeCallTxTotalDataCnt;

	Com1TxCnt=0;


	if( (ThisHoDnUpInfo==1) || (ThisHoDnUpInfo==2) || (ThisHoDnUpInfo==6) || (ThisHoDnUpInfo==7) || (ThisHoDnUpInfo==9) || (ThisHoDnUpInfo==10)){
		TmpCmd[6]='1';
	}


	strcpy( &riding_ev[0],"#riding_ev=1");
	riding_ev[11]=(ev_hogi + '0');


	if(ev_hogi > 0)	idptx=((ev_hogi-1) * HOST_DATA_RECOD);
	else 			idptx=((ev_hogi) * HOST_DATA_RECOD);

	strcpy(&ev_flow[0],"#flow=");
	ev_flow[6]= RcvBuf[idptx + 6];
	ev_flow[7]= RcvBuf[idptx + 7];
	ev_flow[8]= 0x0;
	if(ev_flow[6] < 0x20)	ev_flow[6]='0';	
	if(ev_flow[7] < 0x20)	ev_flow[7]='0';	
	if(ev_flow[6] > 0x7f)	ev_flow[6]='0';	
	if(ev_flow[7] > 0x7f)	ev_flow[7]='0';	



/*
	if((ThisHoDnUpInfo==9) || (ThisHoDnUpInfo==10)){
//	if((eZvilleCommonCall_Orgx[tmp_hogi][tfloor][0] == '9') && (tfloor <= 5)){
		strcpy( &tmpCommondongHo[0],"dongho=101&");
		strcat( &tmpCommondongHo[0],&eZvilleCommonCall_Orgx[tmp_hogi][tfloor][0]);	
	}
	else{
*/

		strcpy( &tmpCommondongHo[0],"dongho=100&700");
		tmpbuf[0]=DspFloorName[(tfloor * 2)];
		tmpbuf[1]=DspFloorName[(tfloor * 2)+1];
		tmpbuf[2]=((Ho/10) + 0x30);	
		tmpbuf[3]=((Ho%10) + 0x30);
		tmpbuf[4]=0;

		if(tmpbuf[0] == '0'){
			strcpy( &tmpbuf[0],&tmpbuf[1]);			
		}
//	}


	strcpy(&org_rx_cmd[0],"<start=0000&0>$version=2.0");
	strcat(&org_rx_cmd[0],"$");
	strcat(&org_rx_cmd[0],&CopyCmd[0]);
	strcat(&org_rx_cmd[0],&TmpCmd[0]);
	strcat(&org_rx_cmd[0],"$");
	strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
	strcat(&org_rx_cmd[0],"$target=elevator#mode=");



	if((ThisHoDnUpInfo==9) || (ThisHoDnUpInfo==10)){
		strcpy( &tmpCommondongHo[0],"dongho=101&");
		strcat( &tmpCommondongHo[0],&eZvilleCommonCall_Orgx[tmp_hogi][tfloor][0]);	
	}
	else{
		strcpy( &tmpCommondongHo[0],"dongho=101&");
		tmpbuf[0]=DspFloorName[(tfloor * 2)];
		tmpbuf[1]=DspFloorName[(tfloor * 2)+1];
		tmpbuf[2]=((Ho/10) + 0x30);	
		tmpbuf[3]=((Ho%10) + 0x30);
		tmpbuf[4]=0;

		if(tmpbuf[0] == '0'){
			strcpy( &tmpbuf[0],&tmpbuf[1]);			
		}
		strcat( &tmpCommondongHo[0], &tmpbuf[0]);
	}





	if(ThisHoDnUpInfo==1){
		strcat(&org_rx_cmd[0],"close#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
	}
	else if(ThisHoDnUpInfo==2){
		strcat(&org_rx_cmd[0],"open#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
	}
	else if(ThisHoDnUpInfo==3){
		strcat(&org_rx_cmd[0],"location#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#riding_ev=1");
	}
	else if(ThisHoDnUpInfo == 4){
		strcat(&org_rx_cmd[0],"arrival#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=down");
		strcat(&org_rx_cmd[0],&ev_flow[0]);
		strcat(&org_rx_cmd[0],&riding_ev[0]);
	}
	else if(ThisHoDnUpInfo == 5){
		strcat(&org_rx_cmd[0],"arrival#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=up");
		strcat(&org_rx_cmd[0],&ev_flow[0]);
		strcat(&org_rx_cmd[0],&riding_ev[0]);
	}
	else if(ThisHoDnUpInfo == 6){
		strcat(&org_rx_cmd[0],"call#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=down");
		strcat(&org_rx_cmd[0],&riding_ev[0]);
		strcat(&org_rx_cmd[0],"#destination=101&1003");
	}
	else if(ThisHoDnUpInfo == 7){
		strcat(&org_rx_cmd[0],"call#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=up");
		strcat(&org_rx_cmd[0],&riding_ev[0]);
		strcat(&org_rx_cmd[0],"#destination=101&1003");
	}
	else if(ThisHoDnUpInfo==8){
		strcat(&org_rx_cmd[0],"close#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
	}
	else if(ThisHoDnUpInfo == 9){
		strcat(&org_rx_cmd[0],"call#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=down");
		strcat(&org_rx_cmd[0],&riding_ev[0]);
		strcat(&org_rx_cmd[0],"#destination=101&1003");
	}
	else if(ThisHoDnUpInfo == 10){
		strcat(&org_rx_cmd[0],"call#");
		strcat(&org_rx_cmd[0],&tmpCommondongHo[0]);
		strcat(&org_rx_cmd[0],"#direction=up");
		strcat(&org_rx_cmd[0],&riding_ev[0]);
		strcat(&org_rx_cmd[0],"#destination=101&1003");
	}



	strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&org_rx_cmd[0] );
	Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);


	for(i=0;i<MAX_ELEV;i++){
		if(ReqHost[i].HostGroup == ReqHost[(ev_hogi - 1)].HostGroup){
			eZville_HomeCallProtocolDataLoad((i+1),tfloor,0);
			strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&org_rx_cmd[0] );
			Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);
		}	
	}	


	TmpCnt=(Com1HomeCallTxTotalDataCnt-StatusUpDnFirstPt);

	Com1TxBuffer[StatusUpDnFirstPt +7]=((TmpCnt/1000) + 0x30);
	TmpCnt=(TmpCnt%1000);

	Com1TxBuffer[StatusUpDnFirstPt +8]=((TmpCnt/100) + 0x30);
	TmpCnt=(TmpCnt%100);

	Com1TxBuffer[StatusUpDnFirstPt +9]=((TmpCnt/10) + 0x30);
	Com1TxBuffer[StatusUpDnFirstPt +10]=((TmpCnt%10) + 0x30);


	Com1TxCnt=Com1HomeCallTxTotalDataCnt;
	Com1TxThisPt=0;


	if(Com1TxCnt > (MAX_COM1_TXBUF - 300)){
		BufferFull=1;
	}

	TxOnece=1;

	return(arrive_sig);
}




unsigned int   __attribute__((section(".usercode"))) eZville_BitSetFind_Ho(unsigned char funcNm,unsigned char floor)
{
	unsigned char i,k,l;
	unsigned char bitval,equal_group;
	unsigned char slect_room;
	unsigned char  ThisHoDnUpInfo=0;
	unsigned char select_ev_hogi=0;

	unsigned char *TmpInfo,*TmpUpCall,*TmpDnCall,*TmpLocationCall,*TmpCloseCall,*TmpUpRegistry,*TmpDnRegistry,*TmpUpKeyTx,*TmpDnKeyTx;
	unsigned char *TmpUpCommon,*TmpDnCommon;


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
	TmpUpCall=&UpCall[floor][1];
	TmpDnCall=&DnCall[floor][1];
	TmpLocationCall=&LocationCall[floor][1];
	TmpCloseCall=&CloseCall[floor][1];
	TmpUpRegistry=&UpRegistry[floor][1];
	TmpDnRegistry=&DnRegistry[floor][1];
	TmpUpKeyTx=&UpKeyTx[floor][1];
	TmpDnKeyTx=&DnKeyTx[floor][1];

	TmpUpCommon=&UpCommonCall[floor][1];
	TmpDnCommon=&DnCommonCall[floor][1];

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


			if( *(TmpUpKeyTx+i) 	& bitval){						
				if(Can1UpDnTxCheck( select_ev_hogi,( (floor+1) | 0x80) )){
					*(TmpUpKeyTx+i)   = ( *(TmpUpKeyTx+i)   &   (~bitval));
				}
			}


			if( *(TmpDnKeyTx+i) 	& bitval){
				if(Can1UpDnTxCheck(select_ev_hogi,((floor+1) | 0x40))){
					*(TmpDnKeyTx+i)   = ( *(TmpDnKeyTx+i)   &   (~bitval));
				}
			}


			if( *(TmpCloseCall+i) 	& bitval){
				ThisHoDnUpInfo=1;				
				ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));

				*(TmpCloseCall+i)   = ( *(TmpCloseCall+i)   &   (~bitval));

				*(TmpInfo+i) 		= ( *(TmpInfo+i) 		& 	(~bitval));
				*(TmpLocationCall+i)= ( *(TmpLocationCall+i)& 	(~bitval));

				*(TmpUpCall+i) 		= ( *(TmpUpCall+i) 		&	(~bitval));
				*(TmpDnCall+i) 		= ( *(TmpDnCall+i) 		& 	(~bitval));

				*(TmpUpRegistry+i) 		= ( *(TmpUpRegistry+i) 		&	(~bitval));
				*(TmpDnRegistry+i) 		= ( *(TmpDnRegistry+i) 		& 	(~bitval));
			
			}

			if( *(TmpInfo+i) & bitval){
				ThisHoDnUpInfo=2;
				ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
				*(TmpInfo+i) = ( *(TmpInfo+i) & (~bitval));

				StatusTxTime = INFO_TX_TIME;

			}		


			if( *(TmpDnRegistry+i) 	& bitval){
				ThisHoDnUpInfo=4;

				if( *(TmpDnCall+i) 	& bitval){
					ThisHoDnUpInfo=6;
					ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
					*(TmpDnCall+i) = ( *(TmpDnCall+i) & (~bitval));
				}
				else{
				
					ret_CloseCmd = 0;
					for(equal_group=0;equal_group<MAX_ELEV;equal_group++){
						if(ReqHost[equal_group].HostGroup == ReqHost[select_ev_hogi].HostGroup){
							ret_CloseCmd = eZville_HomeCallProtocolDataLoad( (equal_group+1),floor,ThisHoDnUpInfo);
							if(ret_CloseCmd){
								equal_group=MAX_ELEV;	
							}								
						}	
					}	




					if(ret_CloseCmd){
						eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
///////////////////////////
	//				ThisHoDnUpInfo=1;				
					ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,8,(select_ev_hogi+1));
	
					*(TmpCloseCall+i)   = ( *(TmpCloseCall+i)   &   (~bitval));
	
					*(TmpInfo+i) 		= ( *(TmpInfo+i) 		& 	(~bitval));
					*(TmpLocationCall+i)= ( *(TmpLocationCall+i)& 	(~bitval));
	
					*(TmpUpCall+i) 		= ( *(TmpUpCall+i) 		&	(~bitval));
					*(TmpDnCall+i) 		= ( *(TmpDnCall+i) 		& 	(~bitval));
	
					*(TmpUpRegistry+i) 		= ( *(TmpUpRegistry+i) 		&	(~bitval));
					*(TmpDnRegistry+i) 		= ( *(TmpDnRegistry+i) 		& 	(~bitval));
////////////////////////////
					}
				}
			}
	
			if( *(TmpUpRegistry+i)	& bitval){
				ThisHoDnUpInfo=5;
				if( *(TmpUpCall+i) 	& bitval){
					ThisHoDnUpInfo=7;
					ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
					*(TmpUpCall+i) = ( *(TmpUpCall+i) & (~bitval));
				}
				else{

					ret_CloseCmd = 0;
					for(equal_group=0;equal_group<MAX_ELEV;equal_group++){
						if(ReqHost[equal_group].HostGroup == ReqHost[select_ev_hogi].HostGroup){
							ret_CloseCmd = eZville_HomeCallProtocolDataLoad((equal_group+1),floor,ThisHoDnUpInfo);
							if(ret_CloseCmd){
								equal_group=MAX_ELEV;	
							}								
						}	
					}	



					if(ret_CloseCmd){
						eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
///////////////////////////
	//				ThisHoDnUpInfo=1;				
					ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,8,(select_ev_hogi+1));
	
					*(TmpCloseCall+i)   = ( *(TmpCloseCall+i)   &   (~bitval));
	
					*(TmpInfo+i) 		= ( *(TmpInfo+i) 		& 	(~bitval));
					*(TmpLocationCall+i)= ( *(TmpLocationCall+i)& 	(~bitval));
	
					*(TmpUpCall+i) 		= ( *(TmpUpCall+i) 		&	(~bitval));
					*(TmpDnCall+i) 		= ( *(TmpDnCall+i) 		& 	(~bitval));
	
					*(TmpUpRegistry+i) 		= ( *(TmpUpRegistry+i) 		&	(~bitval));
					*(TmpDnRegistry+i) 		= ( *(TmpDnRegistry+i) 		& 	(~bitval));
////////////////////////////
					}
				}
			}

			if( (*(TmpLocationCall+i) 	& bitval) && (StatusTxTime >= INFO_TX_TIME) ){
				ThisHoDnUpInfo=3;				
				ret_CloseCmd=eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));

				if(	LocationCallTime[floor][(i+1+k)] > 120){
					*(TmpLocationCall+i) = ( *(TmpLocationCall+i) & (~bitval));
				}
			}



			if( (*(TmpDnCommon+i) 	& bitval)){
				ThisHoDnUpInfo=9;				
				eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
				*(TmpDnCommon+i) = ( *(TmpDnCommon+i) & (~bitval));
			}

			if( (*(TmpUpCommon+i) 	& bitval) ){
				ThisHoDnUpInfo=10;				
				eZville_Tx_HeaderSet(floor,slect_room,ThisHoDnUpInfo,(select_ev_hogi+1));
				*(TmpUpCommon+i) = ( *(TmpUpCommon+i) & (~bitval));
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


///////////command check///////////////////////////////

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



unsigned char   __attribute__((section(".usercode"))) eZville_Local_FindMyDong(void)
{
	unsigned char i,j,len;
	unsigned char tmpbuf[20];

	unsigned char tmpMyHo[10];
	unsigned char tmpMyDong[20];


	strcpy( &tmpbuf[0],&Local_dongho[0]);
	for(i=0;(tmpbuf[i] != '=');i++){
		if(tmpbuf[i] == 0)	return(0);
	}
	i++;


	strcpy( &tmpMyDong[0],&tmpbuf[i]);
	for(i=0;(tmpMyDong[i] != '&');i++){
		if(tmpMyDong[i] == 0)	return(0);
	}
	tmpMyDong[i]=0;



	strcpy( &tmpbuf[0],&Local_dongho[0]);
	for(i=0;(tmpbuf[i] != '&');i++){
		if(tmpbuf[i] == 0)	return(0);
	}

	i++;

	strcpy( &tmpbuf[0],&Local_dongho[i]);
	len=strlen(&tmpbuf[0]);
	if(len==4)		strcpy( &tmpMyHo[0],&tmpbuf[i+2]);
	else if(len==3)	strcpy( &tmpMyHo[0],&tmpbuf[i+1]);
	else	return(0);	

	strcpy(&tmpbuf[0],"101");
	i=xStringCompare( &tmpbuf[0],&tmpMyDong[0] );
	return(i);
}



unsigned char   __attribute__((section(".usercode"))) eZville_Local_CallFloorCallHo(void)
{
	unsigned char i,z;
	unsigned char tmpbuf[20];
	unsigned char src_tmpbuf[20];

	for(i=0;(Local_dongho[i] != '&');i++);
	i++;

	if(strlen(&Local_dongho[i]) == 3){
		strcpy( &tmpbuf[0], &Local_dongho[i-1]);
		tmpbuf[0]='0';
	}
	else{
		strcpy(&tmpbuf[0],&Local_dongho[i]);
	}



// Çö°ü Àç ¼³Á¤  // 900x -> 0301

	for(z=0;z<5;z++){
		for(i=0;eZvilleCommonCall_Orgx[z][i][0] != 'x';i++){
			strcpy( &src_tmpbuf[0], &eZvilleCommonCall_Orgx[z][i][0]);	
			if( xStringCompare( &tmpbuf[0], &src_tmpbuf[0]) ){
				strcpy(&tmpbuf[0],&eZvilleCommonCall_Recopyx[z][i][0]);
				z=5;
				break;
			}	
		}
	}

	eZville_Floor_Ho_Find(&tmpbuf[0]);

}


unsigned char   __attribute__((section(".usercode"))) eZville_Local_FindDongHo_And_UpDn(void)
{
	unsigned char i,j,k;

//	if( !eZville_Local_FindMyDong())	return(0);

	eZville_Local_CallFloorCallHo();


	if(EZB_MyReturnMode==4){						//call cmd
		i=xStringCompare( &Local_direction[0],"direction=down");	// down check
		if(i>0){
			Com1HomeCallFloor=(Com1HomeCallFloor | 0x40);
			RcvKeyBuf[0] = ((unsigned char)(HOMECALL_BUTTON_SET_CMD));
			RcvKeyBuf[1]=Com1HomeCallFloor;		
			HomeCallButtonCallCmd(&RcvKeyBuf[0]);
			eZville_UpDnCallSet();
			return(1);		
		}
	
		i=xStringCompare( &Local_direction[0],"direction=up");	// up check
		if(i>0){
			Com1HomeCallFloor=(Com1HomeCallFloor | 0x80);
			RcvKeyBuf[0] = ((unsigned char)(HOMECALL_BUTTON_SET_CMD));
			RcvKeyBuf[1]=Com1HomeCallFloor;		
			HomeCallButtonCallCmd(&RcvKeyBuf[0]);
			eZville_UpDnCallSet();
			return(1);				
		}
	}
	else{
		RcvKeyBuf[0] = 0;
		RcvKeyBuf[1]=Com1HomeCallFloor;			
		eZville_UpDnCallSet();
	}

	return(0);
}



unsigned char   __attribute__((section(".usercode"))) eZville_Local_mode_Search(void)
{

	unsigned char i,j,k;

	for(j=0;eZville_Local_mode[j][0];j++){
		i=0;
		do{
			if( Local_mode[i] != eZville_Local_mode[j][i])	break;
			else{
				i++;
				if(eZville_Local_mode[j][i] == 0)	return(j+1);  // close=1,open =2 ,call =3, location=4,current=5
			}
		}while(1);
	}

	return(0);
}




unsigned char   __attribute__((section(".usercode"))) LocalStringCompare(unsigned char *pt)
{
	unsigned char i,j,k;

	for(j=0;eZvilleLocalCmd[j][0];j++){
		i=0;
		do{
			if( *(pt+i) != eZvilleLocalCmd[j][i])	break;
			else{
				i++;
				if(eZvilleLocalCmd[j][i] == 0)	return(j+1);
			}
		}while(1);
	}
	return(0);
}



unsigned char   __attribute__((section(".usercode"))) LocalCmdSeperate(void)
{
	unsigned char	i,j,ret;
	unsigned char	*c_pt;


	for(i=0;(TargetCmd[i] != '#');i++);
	i++;

	for(;TargetCmd[i];){
		ret=LocalStringCompare(&TargetCmd[i]);
		if(ret){
			switch(ret){
				case	1:
					c_pt=(&Local_mode[0]);
					break;
				case	2:
					c_pt=(&Local_dongho[0]);
					break;
				case	3:
					c_pt=(&Local_direction[0]);
					break;
				case	4:
					c_pt=(&Local_riding_ev[0]);
					break;
				case	5:
					c_pt=(&Local_nm[0]);
					break;
				case	6:
					c_pt=(&Local_flow[0]);
					break;
				case	7:
					c_pt=(&Local_status[0]);
					break;
				default:
					return(0);
					break;
			}
		}

		for(j=0;(TargetCmd[i] != '#') && (TargetCmd[i] != 0);i++,j++){
			*(c_pt + j)= TargetCmd[i];
		}
		*(c_pt + j)= 0x0;

		if(TargetCmd[i] == '#')		i++;
		else{
			if( !eZville_Local_FindMyDong())	return(0);
			else								return(1);
		}	

	}
	return(0);

}





unsigned char   __attribute__((section(".usercode"))) LocalCmdCheck(void)
{
	unsigned char ret;

	ret=LocalCmdSeperate();
	if(ret){
		EZB_MyReturnMode=eZville_Local_mode_Search();
		if(EZB_MyReturnMode==5){
			ret=5;
		}
		else if(EZB_MyReturnMode>0){
			eZville_Local_FindDongHo_And_UpDn();
		}
		else{
			ret=0;
		}
	}
	return(ret);
}



unsigned char   __attribute__((section(".usercode"))) TargetCheck(void)
{
	unsigned char i,j,k;

	for(j=0;eZvilleTarget[j][0];j++){
		i=0;
		do{
			if( TargetCmd[i] != eZvilleTarget[j][i])	break;
			else{
				i++;
				if(eZvilleTarget[j][i] == 0)	return(j+1);
			}
		}while(1);
	}
	return(0);
}



unsigned char   __attribute__((section(".usercode"))) HeaderStringCompare(unsigned char *pt)
{
	unsigned char i,j,k;


	for(j=0;eZvilleHeaderCmd[j][0];j++){
		i=0;
		do{
			if( *(pt+i) != eZvilleHeaderCmd[j][i])	break;
			else{
				i++;
				if(eZvilleHeaderCmd[j][i] == 0)	return(j+1);
			}
		}while(1);
	}
	return(0);
}



unsigned char   __attribute__((section(".usercode"))) HeaderSeperateCmd(void)
{
	unsigned char	i,j,ret;
	unsigned char	*c_pt;

	for(i=0;(Com1RxBuffer[i] != '$');i++);
	i++;

	for(;Com1RxBuffer[i];){
		ret=HeaderStringCompare(&Com1RxBuffer[i]);
		if(ret){
			switch(ret){
				case	1:
					c_pt=(&VersionCmd[0]);
					break;
				case	2:
					c_pt=(&CmdCmd[0]);
					break;
				case	3:
					c_pt=(&CopyCmd[0]);
					break;
				case	4:
					c_pt=(&DongHoCmd[0]);
					break;
				case	5:
					c_pt=(&TargetCmd[0]);
					break;
				default:
					return(0);
					break;
			}
		}

		for(j=0;(Com1RxBuffer[i] != '$') && (Com1RxBuffer[i] != 0);i++,j++){
			*(c_pt + j)= Com1RxBuffer[i];
		}
		*(c_pt + j)= 0;

		if(Com1RxBuffer[i] == '$')	i++;
		else 						return(1);	

	}
	return(0);

}



unsigned char   __attribute__((section(".usercode"))) SeperateCmd(void)
{
	unsigned char	ret;

	ret=HeaderSeperateCmd();
	if(ret){
		ret=TargetCheck();
		if(ret == 1){
			Cmd_Nm=ret;
		}
		else if(ret == 2){
			ret=LocalCmdCheck();
		}
	}
	return(ret);
}


//////////////////////////////////////////////////////


unsigned char   __attribute__((section(".usercode"))) CmdChk(void)
{
	unsigned char  i;

	i=SeperateCmd();

	return(i);
}



unsigned int   __attribute__((section(".usercode"))) eZville_InfoCmdReturn(void)
{
	unsigned int i;


	if(StatusUpDnTxByteOrder > 31)	StatusUpDnTxByteOrder=0;


	for(;1;){
		Com1HomeCallCurReqWard=0;
		eZville_BitSetFind_Ho(3,StatusUpDnTxByteOrder);


		if(StatusUpDnTxByteOrder > 31){
			if(TxOnece==1){
				StatusTxTime=0;
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



unsigned int   __attribute__((section(".usercode"))) eZville_ConnectionCmdReturn(void)
{
	unsigned int i;
	unsigned int this_len,pt;
	unsigned char	tmpbbf[200];
 

	if(Cmd_Nm == 1){
		Cmd_Nm=0;
		pt=Com1HomeCallTxTotalDataCnt+7;

		strcpy( &tmpbbf[0],&eZvilleReturnCmd[0] );
		for(i=0;CopyCmd[i];i++){
			tmpbbf[27+i]=CopyCmd[i];
		}
		tmpbbf[27+i]=0;

		strcat( &tmpbbf[0],&eZvilleReturnCmd[35] );
		

		this_len=strlen(&tmpbbf[0]);
		strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&tmpbbf[0] );



		Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);
		
		Com1TxCnt=Com1HomeCallTxTotalDataCnt;
		Com1TxThisPt=0;

		

		Com1TxBuffer[pt+0]=((this_len/1000) + 0x30);
		this_len=(this_len%1000);
		Com1TxBuffer[pt+1]=((this_len/100) + 0x30);
		this_len=(this_len%100);
		Com1TxBuffer[pt+2]=((this_len/10) + 0x30);
		Com1TxBuffer[pt+3]=((this_len%10) + 0x30);

	

		if(Com1TxCnt > (MAX_COM1_TXBUF - 300)){
			BufferFull=1;
			return(0);
		}


	}
	return(0);
}


unsigned int   __attribute__((section(".usercode"))) eZville_AliveCmdReturn(void)
{
	unsigned int i;
	unsigned int this_len,pt;


	if(ElevatorServerAliveTime >= MAX_ALIVE_TIME){
		ElevatorServerAliveTime=0;

		pt=Com1HomeCallTxTotalDataCnt+7;
		this_len=strlen(&eZvilleAliveCmd[0]);

		strcat( &Com1TxBuffer[Com1HomeCallTxTotalDataCnt],&eZvilleAliveCmd[0] );
		Com1HomeCallTxTotalDataCnt=strlen(&Com1TxBuffer[0]);
		
		Com1TxCnt=Com1HomeCallTxTotalDataCnt;
		Com1TxThisPt=0;

		

		Com1TxBuffer[pt+0]=((this_len/1000) + 0x30);
		this_len=(this_len%1000);
		Com1TxBuffer[pt+1]=((this_len/100) + 0x30);
		this_len=(this_len%100);
		Com1TxBuffer[pt+2]=((this_len/10) + 0x30);
		Com1TxBuffer[pt+3]=((this_len%10) + 0x30);
	
		if(Com1TxCnt > (MAX_COM1_TXBUF - 300)){
			BufferFull=1;
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




unsigned int   __attribute__((section(".usercode"))) HomeCallBaseHeaderchk(void)
{

	if(Com1RxCnt == 14){
 		if( (Com1RxBuffer[Com1RxCnt-2] == '>') || (Com1RxBuffer[Com1RxCnt-1] == '>')){
			Com1HomeCallRxDataCnt=DataCntCheck();

			if(Com1HomeCallRxDataCnt == 0){
				return(0);
			}
		}
		else{
			return(0);
		}
	}
	else{
		if(Com1RxCnt == 7){
	 		if(Com1RxBuffer[0] != '<')	return(0);			
	 		if(Com1RxBuffer[1] != 's')	return(0);			
	 		if(Com1RxBuffer[2] != 't')	return(0);			
	 		if(Com1RxBuffer[3] != 'a')	return(0);			
	 		if(Com1RxBuffer[4] != 'r')	return(0);			
	 		if(Com1RxBuffer[5] != 't')	return(0);			
	 		if(Com1RxBuffer[6] != '=')	return(0);			
		}
		
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

/*
	if( (buf < 0x10) || (buf > 0x7f)){
    	Com1RxCnt=0;
		return(0);
	}
*/

	Com1RxBuffer[Com1RxCnt]=buf;

	if(Com1RxCnt < (MAX_COM1_RXBUF-1)){
	    Com1RxCnt++;
	}
	else{
	    Com1RxCnt=0;
	    Com1RxBuffer[Com1RxCnt]=buf;
	}

	if(Com1RxCnt <= 14){
		if(!HomeCallBaseHeaderchk()){
			Com1RxCnt=0;
			return(0);
		}
	}
	else{
		if( ((unsigned int)Com1RxCnt) >= (Com1HomeCallRxDataCnt)){			
	    	Com1RxBuffer[Com1RxCnt]=0;


for(buf=0;buf<Com1RxCnt;buf++){
	Can2TxBuf[buf]=Com1RxBuffer[buf];
}
Can2TxCnt=Com1RxCnt;
Can2TxThisPt=0;


			CmdChk();
            Com1RxCnt=0;

			return(0);												
		}		
	}

	return(0);
}






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


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int   __attribute__((section(".usercode"))) My_Project_Start(void)
{
	unsigned int retx;
		
	MoniteringCan2();

	if(ThisHostSelect >= MAX_ELEV){
			ThisHostSelect=0;
	}

	if(I_AM_LISTENING_MODE == 0){
		if(Can1HostDataReq(ThisHostSelect)){
			ThisHostSelect++;
		}
	}	


	if(Com1RxStatus == TX_SET){
		return(0);
	}	

	Com1TxBuffer[0]=0;
	Com1HomeCallTxTotalDataCnt=0;	
	BufferFull=0;

	if(BufferFull==0){
		eZville_ConnectionCmdReturn();
	}
	if(BufferFull==0){
		eZville_AliveCmdReturn();
	}


	if(BufferFull==0){
		eZville_InfoCmdReturn();
	}

	if(Com1HomeCallTxTotalDataCnt > 0){
		Com1TxStart();
		Com1HomeCallTxTotalDataCnt=0;
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
	unsigned int i;

	unsigned	char	tmpCompany,tmpGroupadr,tmpLocaladr;

 

    Com1Init(BAUD_RATE_57600);
    Com2Init(BAUD_RATE_57600);

	Filter_Mask_Set();

	for(i=0;i<(CAN1_MAX_RX_BUF-1);i++)	Can1RxBuf[i]=0;
	for(i=0;i<(CAN1_MAX_TX_BUF-1);i++)	Can1TxBuf[i]=0;

	Com1TxThisPt =0;
	Com1TxCnt=0;


	for(i=0;i<MAX_ELEV;i++)	HostElevLive[i]=100;


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
	tmpGroupadr=4;
	tmpLocaladr=1;    
	GetTxSidEid(tmpCompany,tmpGroupadr,tmpLocaladr);

	ReqHost[5].HostSid=Can1TxSid;
	ReqHost[5].HostEid=Can1TxEid;
	ReqHost[5].HostGroup=4;
	ReqHost[5].Ho_Host[0]=9;
	ReqHost[5].Ho_Host[1]=12;
	ReqHost[5].Ho_Host[3]=0xff;
	ReqHost[5].Ho_Host[4]=0xff;


	return(0);	
}



unsigned char __attribute__((section(".usercode"))) msecTimer1(void)
{
	return(0);
}


unsigned char __attribute__((section(".usercode"))) MsecTimer_100(void)
{
	if(StatusTxTime < 100)			StatusTxTime++;
	return(0);
}


unsigned char __attribute__((section(".usercode"))) SecTimer_1(void)
{
	unsigned char i,j;

	for(i=0;i<MAX_ELEV;i++){          
		if(HostElevLive[i] < 100)	HostElevLive[i]++;
	}
	
	if(ElevatorServerAliveTime < 250)	ElevatorServerAliveTime++;
	
	for(i=0;i<32;i++){ 
		for(j=1;j<7;j++){          
			if(LocationCallTime[i][j] < 250)	LocationCallTime[i][j]++;
		}
	}
	return(0);
}

