


#include <p30f6011a.h>            

#include        "xiodef.h"
///////////////#include        "fontout.h"
#include        "xcomdata.h"
#include        "xesdots.h"
#include        "xkeysort.h"
#include        "xsetup.h"
#include        "memory_map.h"
#include  		"you_can1.h"
#include  		"WorkingRam.h"







/*
*/



/*
void    CompanyChk(unsigned char id)        
{
    unsigned int    IdPt;

	IdPt=IsBufferPt(id);
    
    if(!MaskSetBit){
       if((RcvBuf[IdPt+DSP1] == 'C') && (RcvBuf[IdPt+DSP2] == 'M') && !(RcvBuf[IdPt+1] & S1_AUTO))  {
            Company=(unsigned char)Can1RxSid;
            CmpSetBit=1;      
        }
    }
}
*/


unsigned char   Lamp(unsigned char id)
{
    unsigned int    IdPt;

	IdPt=IsBufferPt(id);
    
    FDsp=0;

/*
    HostCallMe=0;
	if(RcvBuf[IdPt + SL_mCallMe] > 0){
	    if(CallMeAdr == RcvBuf[IdPt + SL_mCallMe])  HostCallMe=1;
	}
*/

    if((RcvBuf[IdPt+DSP1] == 'J') && (RcvBuf[IdPt+DSP2] == 'D') && !(RcvBuf[IdPt+1] & S1_AUTO))	SetupBit=1;
    else                                                                                        SetupBit=0;      

    if(SetupBit)    return(0);

    TopFloor=RcvBuf[IdPt + SL_mTopFlr];

    CurFloor=RcvBuf[IdPt];


 
////////////////dot type dsp hib,hpi////////////////////////////////////////////
    if((CurFloor > BefCurFloor) || (CurFloor < BefCurFloor)){
        FloorChange=1;   
        BefCurFloor=CurFloor;
		shift=0;
    }
//////////////////////    Floor_Char_load(0,RcvBuf[IdPt+DSP1],RcvBuf[IdPt+DSP2]);



    Open=0;

	if(bIamXSubDoor){
        if((RcvBuf[IdPt+3] & S3_OPEN_SUB))  Open=1;   
    }
    else{
        if(RcvBuf[IdPt+1] & S1_OPEN)        Open=1;   
    }    


   Close=0;
   if((RcvBuf[IdPt+1] & S1_CLOSE)){
      Close=1;   
   }


   CarMove=0;
   if(RcvBuf[IdPt+2] & S2_CAR_MOVE)         CarMove=1;


//   AUTO_LAMP=0;
//   if(RcvBuf[IdPt+2] & S2_LAMP_USER)        AUTO_LAMP=1;

//   MANUAL_LAMP=0;
//   if(RcvBuf[IdPt+1] & S1_MANUAL)           MANUAL_LAMP=1;

   UpMove=0;
   if(RcvBuf[IdPt+1] & S1_UP)               UpMove=1;

   DnMove=0;
   if(RcvBuf[IdPt+2] & S2_DN)               DnMove=1;

   Vip=0;
   if((RcvBuf[IdPt+3] & S3_VIP))            Vip=1;

   Parking=0;
   if((RcvBuf[IdPt+3] & S3_PARKING))        Parking=1;

   ShiftOn=0;
   if((RcvBuf[IdPt+3] & S3_SHIFT))          ShiftOn=1;
   
   Fire=0;
   if(RcvBuf[IdPt+2] & S2_FIRE)             Fire=1;

   KeyClrExt=0;
   if(RcvBuf[IdPt+2] & S2_EXT_BUT_CLR)      KeyClrExt=1;

   KeyClr=0;
   if(RcvBuf[IdPt+2] & S2_CAR_BUT_CLR)      KeyClr=1;

   
	Emg=0;
   if(RcvBuf[IdPt+1] & S1_EMG)              Emg=1;

   StopBit=0;
   if(RcvBuf[IdPt+3] & S3_STOP)             StopBit=1;

	CurKey=0;
   if(RcvBuf[IdPt+3] & S3_CURKEY)           CurKey=1;
         
	Auto=0;
   if(RcvBuf[IdPt+1] & S1_AUTO)             Auto=1;

	OverLoad=0;
   if(RcvBuf[IdPt+1] & S1_OVERLOAD)         OverLoad=1;

	Full=0;
   if(RcvBuf[IdPt+3] & S3_FULL)             Full=1;



    return(0);

}













