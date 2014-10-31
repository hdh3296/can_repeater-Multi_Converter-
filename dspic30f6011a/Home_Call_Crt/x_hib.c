




#include <p30f6011a.h>            

#include        "memory_map.h"
#include        "xiodef.h"
#include        "xcomdata.h"
#include        "xkeysort.h"
#include        "xsetup.h"
#include  		"you_can1.h"
#include  		"WorkingRam.h"

#include  "prj_app.h"



extern  unsigned char	Lamp(unsigned char id);  //4
extern  void    		ButtonLampClrChk(void);
extern  void    		CompanyChk(unsigned char id);  //new

                            

#define     MSEC_H    0xfc
#define     MSEC_L    0x18




unsigned    char  DisableFlr[4]   ={0,0,0,0};            

unsigned    char  CurStatus;       /*current floor */
unsigned    char  CurFloor;       /*current floor */
unsigned    char  BefCurFloor;   /*current floor */
unsigned    char  Arrowshift;    /*current floor */
unsigned    char  htpt;
unsigned    char  shift;
unsigned    char  shiftTime;
unsigned    char  MyAddress;        
unsigned    char  src;
unsigned    char  MainTimer=0;
unsigned    char  msec100=0;
unsigned    char  sec1=0;
unsigned    char  UpButtonTime=0;
unsigned    char  DnButtonTime=0;
unsigned    char  UpKeyCalu[MAX_ELEV];
unsigned    char  DnKeyCalu[MAX_ELEV];
unsigned    char  MoveTime[MAX_ELEV];
unsigned    char  BlinkTime;
unsigned    char  UpHallElev;
unsigned    char  DnHallElev;
unsigned    char  TopFloor;
unsigned    char  TwoDoorDrive;
//unsigned    char  CallMeAdr;
unsigned    char  OnTime;
unsigned    char  SensorTime;


//unsigned    char    SrcAddress=0;
unsigned    char    LocalNumber=0;
//unsigned    char    SubkDoor=0;
unsigned    char    offset_flr=0;
unsigned    char    ButtonType=0;
unsigned    char    CallOffset=0;


unsigned    char    HomeCallUpKey[4]={0,0,0,0};unsigned    char    HomeCallDnKey[4]={0,0,0,0};unsigned    char    HomeCallCarKey[4]={0,0,0,0};
unsigned	char  	HostElevLive[MAX_ELEV];

unsigned    char  FirstUpKeyCalu;
unsigned    char  FirstDnKeyCalu;
unsigned    char  FirstUpMyAddress,FirstDnMyAddress;

//unsigned    char    CanLiveTimeAdr[MAX_ELEV];
//unsigned    char    EqualDataBuf[8];


unsigned    int    LoopTimer=0;


//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
#define				X_MAX_SUB_SELECT_BUF			5

#define				X_SUB_PRIORITY					0
#define				X_SUB_ADDRESS   				1
#define				X_SUB_UPDN   					2
#define				X_SUB_LOCAL_NM					3

unsigned	char  	ThisSelHost[MAX_ELEV][X_MAX_SUB_SELECT_BUF];



/*
bit   HostCallMe;
bit   UpMove;
bit   DnMove;
bit   FloorChange;
bit   ManualToggle;
bit   CarMove;
bit   KeyClr;
bit   Auto;
bit   FDsp;
bit   Parking;
bit   Vip;
bit   Fire;
bit   ShiftOn;
bit   Open;
bit   Close;
bit   UpKeyBit;
bit   DnKeyBit;
bit   Emg;
bit   StopBit;
bit   Full;
bit   OverLoad;
bit   KeyClrExt;
bit   LowOn;
bit   HighOn;
bit   BlinkOn;
bit   HallLampUpBit;
bit   HallLampDnBit;
bit   Up_Key_Valid;
bit   Up_Key_Clear;
bit   Dn_Key_Valid;
bit   Dn_Key_Clear;
bit   CurKey;
bit   SetupBit=0;                     
bit   TogUpKeyBitSet;                     
bit   TogUpKeyBitReset;                     
bit   TogDnKeyBitSet;                     
bit   TogDnKeyBitReset;                     
bit   b_vip=0;                    
bit   b_MyFamily=0;                    
bit   bIamXSubDoor=0;                    


bit   ExtCallKey=0;                    
bit   bHALL_LAMP_UP=0;
bit   bSensorButton=0;

bit   bCardKeyValid=0;

*/



unsigned char  SubDoorMainDoorCheck(void)
{
    unsigned char ret=1;
    if( bIamXSubDoor  &&  (EqualDataBuf[2] & SUB_DOOR_BIT))    ret=0;
    if( !bIamXSubDoor && !(EqualDataBuf[2] & SUB_DOOR_BIT))    ret=0;

    return(ret);
}


void	MyLampCheck(void)
{			
	if( !(Can1RxEid & 0x80)){
	    if((EqualDataBuf[1] & 0x1f) == (MyAddress-1)){
	        if(!SubDoorMainDoorCheck()){
	            if(EqualDataBuf[0] == CAN_KEY_CLEAR){
	                if(EqualDataBuf[1] & UP_READY){
	                    UP_KEY_LAMP=0;
	                    Up_Key_Valid=0;
	                }
	                if(EqualDataBuf[1] & DN_READY){
	                    DN_KEY_LAMP=0;
						Dn_Key_Valid=0;
	                }
	            }
	            else if((EqualDataBuf[0] != CAN_NO_KEY_SET)){
					if(ButtonType>0){
		                if( (EqualDataBuf[1] & CAR_READY)  && (Up_Key_Clear==0)){
		                    UP_KEY_LAMP=1;
		                }
					}
					else{
		                if( (EqualDataBuf[1] & UP_READY)  && (Up_Key_Clear==0)){
		                    UP_KEY_LAMP=1;
		                }
		                if((EqualDataBuf[1] & DN_READY) && (Dn_Key_Clear==0)){
		                    DN_KEY_LAMP=1;
		                }
					}
	            }
	        }          
	    }    
	}
}            











void  MyAdrRead(void)
{
	MyAddress=0x01;
}



/*
void  WaitMyAdr(void)
{
   unsigned char i;

   if(FDsp){   
      string_load_ram(0,MyAddress);
      EngDisplay(11,EXT_VERSION);               
      for(i=0;i<=15;i++){
         shiftDataBuf[i]=DspBuf[i];
      }
   }
   else{
      for(i=0;i<=15;i++){
         shiftDataBuf[i]=0x0;
      }   
   }                    
}
*/






void    PortInit(void)
{
/*
	TRISA=0;
	TRISB=0;
	TRISC=0;
	TRISD=0;
	TRISE=0;

  	LATA=0;	
  	LATB=2;	
  	LATC=0;	
  	LATD=0;	
  	LATE=0;
*/	
}

unsigned char SelHostUpKey[8];
unsigned char SelHostDnKey[8];

unsigned char SelHostUpKeyAdr[8];
unsigned char SelHostDnKeyAdr[8];


void	xxx(void)
{
	if(ExtCallKey){
 		CanCmd=CAN_KEY_SET;
		SelHostAdr=Can1RxLocalAddr=LocalNumber;
     	CanKeyValue[1] = ((MyAddress-1) | CAR_READY);

	    if(bIamXSubDoor)  	CanKeyValue[2] = (CanKeyValue[2] | SUB_DOOR_BIT);
	    else            	CanKeyValue[2] = (CanKeyValue[2] & ~SUB_DOOR_BIT);
	}
}

void Cmd_Ld_Can1_TxBuf(void)
{
    unsigned char i;

    CanKeyValue[0]=CanCmd;                
	for(i=0;i<8;i++)	Can1TxBuf[i]=CanKeyValue[i];                

}



unsigned int __attribute__((section(".usercode")))  ReChkButton(void)
{
	unsigned int j;

	if(SelHostUpKeyAdr[0] 		!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[1] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[2] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[3] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[4] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[5] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[6] 	!= 0xff)	return(0);
	else if(SelHostUpKeyAdr[7] 	!= 0xff)	return(0);

	if(SelHostDnKeyAdr[0] 		!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[1] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[2] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[3] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[4] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[5] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[6] 	!= 0xff)	return(0);
	else if(SelHostDnKeyAdr[7] 	!= 0xff)	return(0);


	for(j=0;j<MAX_ELEV;j++){
		SelHostUpKey[j]=0xff;
		SelHostDnKey[j]=0xff;
	}

	return(1);
}


/*
unsigned int __attribute__((section(".usercode")))  ButtonPriorityChk(void)
{
	unsigned char j,k;

	if(ReChkButton()){
		for(MyAddress=1;MyAddress<32;MyAddress++){	
	        UpDownKeyLoad();
			if(UpKeyBit || DnKeyBit){			                  			
	           	UpDownKey();
				if(UpKeyBit){
					if(!UpKeyLoad(Can1RxLocalAddr)){
						if(UpKeyCalu[SelHostAdr] < 100){
							if(SelHostUpKey[SelHostAdr] > UpKeyCalu[SelHostAdr]){
								SelHostUpKey[SelHostAdr] = UpKeyCalu[SelHostAdr];	
								SelHostUpKeyAdr[SelHostAdr]=MyAddress;
							}  
						} 
					}
				}
				if(DnKeyBit){
					if(!DnKeyLoad(Can1RxLocalAddr)){
						if(DnKeyCalu[SelHostAdr] < 100){
							if(SelHostDnKey[SelHostAdr] > DnKeyCalu[SelHostAdr]){
								SelHostDnKey[SelHostAdr] = DnKeyCalu[SelHostAdr];	
								SelHostDnKeyAdr[SelHostAdr]=MyAddress;
							}  
						} 
					}
				}
			}
		}
	}	



    DnKeyBit=0;
	DN_KEY_LAMP=0;
	Dn_Key_Valid=0;

	UpKeyBit=0;
	UP_KEY_LAMP=0;
	Up_Key_Valid=0;


	j=0;
	do{
		if(SelHostUpKeyAdr[j] != 0xff){
			MyAddress=SelHostUpKeyAdr[j];
			UpKeyBit=1;
			UP_KEY_LAMP=1;
			Up_Key_Valid=1; 	

            if(!UpKeyLoad(Can1RxLocalAddr)){
				xxx();
				Cmd_Ld_Can1_TxBuf();
				Can1TxSidEidLoad();
				if(	!Can1TxData(0,1)){
					SelHostUpKeyAdr[j]=0xff;
				}	
            }
			else{
				SelHostUpKeyAdr[j]=0xff;
				j++;
			}
		}
		else	j++;		
	}while(j<MAX_ELEV);



	j=0;
	do{
		if(SelHostDnKeyAdr[j] != 0xff){
			MyAddress=SelHostDnKeyAdr[j];
			DnKeyBit=1;
			DN_KEY_LAMP=1;
			Dn_Key_Valid=1; 	

            if(!DnKeyLoad(Can1RxLocalAddr)){
				Cmd_Ld_Can1_TxBuf();
				Can1TxSidEidLoad();
				if( !Can1TxData(0,0)){
					SelHostDnKeyAdr[j]=0xff;
				}
            }
			else{
				SelHostDnKeyAdr[j]=0xff;
				j++;
			}
		}
		else	j++;		
	}while(j<MAX_ELEV);

	bCan1RxAll=0;

	return(0);
}




*/




void Init_Home_Call(void)
{
 
	unsigned int i;

/*   
    Initial();
    PortInit();
    Timer0Init(); 
*/


  
	UpButtonClear();
	DownButtonClear();
            

	Can1Tx0ConfirmCnt=0;
	Can1Tx1ConfirmCnt=0;


    CmpSetBit=0;            //new
    MaskSetBit=0;           //new
    
    MyAdrRead();

//////////////////////////////////////////////////    LoadSetupValue();  //1

            
    FloorChange=0;
    BefCurFloor=0;
    ManualToggle=0;
    



///////////setup
    LocalNumber=0;
//    SubkDoor=0;
    offset_flr=0;
    ButtonType=0;
/////////////////////////////////    Company='A';
	bToggleOn=0;
	bCurOpenKey=0;
	bUserLamp4=0;
	CallOffset=0;

///////////setup 




        
    htpt=0;

    sec1=0;
    MoveTime[0]=0; 


/* no delete
	do{
        CompanyChk(LocalNumber);  //new
        CLRWDT();
    }while(MoveTime[0] < 4);
       

    if(CmpSetBit){              //new
    	new_value[5]=Company;   //
        CompanyWrite();  		//new
    }                           //new

*/

/*
    di();                       //new
    MaskSetBit=1;               //new
    CAN_Init();                 //new
    ei();                       //new
*/

    FDsp=1;
    MainTimer=0;

       
//    CanDatacnt0=3;
//    CanDatacnt1=3;

    CanCmd=0;


/*
	CallMeAdr=MyAddress;
	bIamXSubDoor=0;

    if(SubkDoor & 0x01){
  		CallMeAdr=(MyAddress + 64);
		bIamXSubDoor=1;
	}		
*/
/*
	CallMeAdr=0;
	bIamXSubDoor=0;
    if(SubkDoor & 0x01){
  		CallMeAdr=(0 + 64);
		bIamXSubDoor=1;
	}		

*/

    HostCallMe=0;    
    SetupBit=0;     //2
	ChangeSetup=0;

    OnTime=0;

	if(!bToggleOn){ 
	    if(ButtonType == 1){
	        OnTime=2;
	    }
	}


    for(i=0;i<MAX_ELEV;i++){          
        HostElevLive[i] =0;
    }        

}



unsigned int __attribute__((section(".usercode")))  ToHibButtonAlarm(unsigned char button,unsigned char updn)
{

	CanCmd=CAN_KEY_CONFIRM;
	MyAddress=(button-1);
	if(updn==1)			CanKeyValue[1]=(MyAddress | UP_READY);
	else if(updn==2)	CanKeyValue[1]=(MyAddress | DN_READY);
	else				return(0);
	SelHostAdr=Can1RxLocalAddr;

/*
	MyAddress=ThisSelHost[i][X_SUB_ADDRESS];
	SelHostAdr=i;

	if(ThisSelHost[i][X_SUB_UPDN]==1)		CanKeyValue[1]= ((MyAddress-1) | UP_READY);
	else if(ThisSelHost[i][X_SUB_UPDN]==2)	CanKeyValue[1]= ((MyAddress-1) | DN_READY);
	else	break;
*/

 	CanKeyValue[2]=0;
	Cmd_Ld_Can1_TxBuf();
	Can1TxSidEidLoad();
	Can1TxData(0,1);

	return(0);
}

unsigned int  Home_Call(void)
{
    unsigned char confirmkey,i;

		if(bToggleOn){
			ButtonType = 0;
		}


        ButtonLampClrChk();  //rechk


		for(i=0;i<MAX_ELEV;i++){
			ThisSelHost[i][X_SUB_PRIORITY]=0xff;
		}

		for(MyAddress=1;MyAddress<=32;MyAddress++){
			UpDownKeyLoad();
			if(UpKeyBit){				
	            if(!UpKeyLoad(1)){
					if(ThisSelHost[SelHostAdr][X_SUB_PRIORITY] > UpKeyCalu[SelHostAdr]){
						ThisSelHost[SelHostAdr][X_SUB_PRIORITY] = UpKeyCalu[SelHostAdr];
						ThisSelHost[SelHostAdr][X_SUB_ADDRESS] 	= MyAddress;
						ThisSelHost[SelHostAdr][X_SUB_UPDN] 	= 1;
					}
	            }
////////////				UpRegistry[MyAddress-1][0]=(SelHostAdr+1);
			}			
		}
		   
		for(MyAddress=1;MyAddress<=32;MyAddress++){
			UpDownKeyLoad();
			if(DnKeyBit){				
	            if(!DnKeyLoad(1)){
					if(ThisSelHost[SelHostAdr][X_SUB_PRIORITY] > DnKeyCalu[SelHostAdr]){
						ThisSelHost[SelHostAdr][X_SUB_PRIORITY] = DnKeyCalu[SelHostAdr];
						ThisSelHost[SelHostAdr][X_SUB_ADDRESS] 	= MyAddress;
						ThisSelHost[SelHostAdr][X_SUB_UPDN] 	= 2;
					}
	            }
/////////////////				DnRegistry[MyAddress-1][0]=(SelHostAdr+1);
			}			
		}
     



        if(bCan1RxAll){
	 		LoopTimer=0;

			for(i=0;i<MAX_ELEV;){
        		asm("CLRWDT");
				if(LoopTimer > 10)	return(1);

				if(ThisSelHost[i][X_SUB_PRIORITY] != 0xff){
					CanCmd=CAN_KEY_SET;
					MyAddress=ThisSelHost[i][X_SUB_ADDRESS];
					SelHostAdr=i;
	
					if(ThisSelHost[i][X_SUB_UPDN]==1)		CanKeyValue[1]= ((MyAddress-1) | UP_READY);
					else if(ThisSelHost[i][X_SUB_UPDN]==2)	CanKeyValue[1]= ((MyAddress-1) | DN_READY);
					else	break;
	
	    			CanKeyValue[2]=0;
					Cmd_Ld_Can1_TxBuf();
					Can1TxSidEidLoad();
					if( !Can1TxData(0,1)){
						ThisSelHost[i][X_SUB_PRIORITY] = 0xff;
						i++;
					}		
				}
				else	i++;
			}
			bCan1RxAll=0;
		}

	return(0);
}




/*

void Home_Call(void)
{
    unsigned char confirmkey,i;
 //////////////////////       CanLiveChk();
        Lamp(LocalNumber);                  
        ButtonLampClrChk();
		ButtonPriorityChk();

}
*/




