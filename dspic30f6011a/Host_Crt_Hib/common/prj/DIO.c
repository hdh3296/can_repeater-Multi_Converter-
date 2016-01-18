

#include <p30f6011a.h>            

#include  "..\header\host_io.h"            
#include  "..\header\30f6010_io.h"
#include  "..\can\you_can1.h"
#include  "..\can\you_can2.h"
#include  "..\uart\ComPort1.h"

#include  "..\uart\ComPort2.h"
#include  "..\memory_map.h"



unsigned	char  	sRamDArry[4][END_SRAM];     
unsigned    int		WaitTime;
unsigned    char    huntingtime;
unsigned    char	updn;
unsigned    int		mpm=0;

const unsigned char DftFlrName[] = {"0102030405060708091011121314151617181920212223242526272829303132"}; 


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define  SEG_AL         RG6_I  	// X0  
#define  SEG_BL         RG7_I  	// X1 
#define  SEG_CL         RG8_I  	// X2 
#define  SEG_DL         RG9_I  	// X3
#define  SEG_EL         RB5_I  	// X4
#define  SEG_FL         RB4_I  	// X5
#define  SEG_G1L        RB3_I  	// X6
#define  SEG_G2L        RB2_I  	// X7
#define  SEG_ADGH       RB1_I  	// X8
#define  SEG_BH         RB0_I  	// X9
#define  SEG_CH         RB8_I  	// X10 
#define  SEG_EH         RB9_I  	// X11 
#define  SEG_NFH        RB10_I  // X12 
#define  UP             RB11_I  // X13 
#define  DN             RB12_I  // X14 
#define  AT             RB13_I  // X15 

#define  DOOR_CLOSE     RB14_I  // X16 
#define  OVERLOAD       RB15_I  // X17 
#define  FLOW           RF6_I  	// X18 
#define  MOVE           RG3_I  	// X19 
#define  EMG            RG2_I  	// X20 
#define  PARKING        RD8_I  	// X21 
#define  FULL           RD9_I  	// X22 
#define  X23            RD10_I  // X23 
#define  X24            RD11_I 	// X24 
//////////////////////////////////////////////////////////////////////////
#define  sINSPECT 			  0 
#define  sInvertErr			  1                             
#define  sI_C_N_FLR           2
#define  sEMERGENCY           3
#define  sLOPE_BRK            4
#define  sReserve1            5
#define  sSLIP                6
#define  sULS                 7
#define  sDLS                 8
#define  sLULD_NO_OFF         9
#define  sNEXT_FLOOR          10
#define  sInPortError         11
#define  sHDS_RUN_OFF         12
#define  sCLE_RUN_OFF         13
#define  sReserve2            14
/////////////////////////////////////
#define  sEncoderErr          15
#define  sEncoderABErr        16 
#define  sSpeedSetError       17 
#define  sSpeedPortError      18 
#define  sNoUseSpeed          19 
#define  sMinLengthErr        20 
#define  sEqualFloorError     21 
#define  sSystemErr           22
#define  sBreakMgtOpen        23  
#define  sBreakOpen           24  
#define  sSusErr              25  
#define  sSdsErr              26  
//////////////////////////////////
#define  sLuOrLdErr0          27  
#define  sLuOrLdErr1          28  
#define  sLuOrLdErr2          29  
#define  sLuOrLdErr3          30  
#define  sLuOrLdErr4          31  
#define  sLuOrLdErr5          32  
#define  sCarDoor_Jumper      33
#define  sHoleDoor_Jumper     34
#define  sCarHoleDoor_Jumper  35

/////////////////////////////////////
#define  sFhmError            43
//////////////////////////////////////
#define  sHDS_NO_ON           44
#define  sCLE_NO_ON           45
#define  sOVL                 46 
#define  sPARKING             47   
#define  sFireOn              48         
#define  sWaterSense          49         
#define  sOPE_NO_ON           50                                                                            
#define  sVIP                 51
#define  sFHM_MODE			  52
#define  sMANUAL_UP           53         
#define  sMANUAL_DN           54         
#define  sMANUAL              55         
#define  sSFT                 56
#define  sOPEN                57
#define  sCLOSE               58         
#define  sUP                  59         
#define  sDN                  60         
#define  sSLOW_SPEED          61         
#define  sWAIT_LULD           62         
#define  sSTOP                63        
#define  sHOME                64         
#define  sREADY               65




unsigned int   __attribute__((section(".usercode"))) SimIOInit(void)
{
	unsigned char i,j;

	TRIS_RG6_IO=1;
	TRIS_RG7_IO=1;
	TRIS_RG8_IO=1;
	TRIS_RG9_IO=1;
	
	TRIS_RB5_IO=1;
	TRIS_RB4_IO=1;
	TRIS_RB3_IO=1;
	TRIS_RB2_IO=1;
	TRIS_RB1_IO=1;
	TRIS_RB0_IO=1;
	
	TRIS_RB8_IO=1;
	TRIS_RB9_IO=1;
	TRIS_RB10_IO=1;
	TRIS_RB11_IO=1;
	TRIS_RB12_IO=1;
	TRIS_RB13_IO=1;
	TRIS_RB14_IO=1;
	TRIS_RB15_IO=1;
	
	TRIS_RF6_IO=1;
	
	TRIS_RG3_IO=1;
	TRIS_RG2_IO=1;
	
	TRIS_RD8_IO=1;
	TRIS_RD9_IO=1;
	TRIS_RD10_IO=1;
	TRIS_RD11_IO=1;


	for(j=0;j<4;j++){
		for(i=0;i<(END_SRAM-1);i++)			sRamDArry[j][i]=0;
		sRamDArry[j][DSP1]='0';
		sRamDArry[j][DSP2]='1';
		sRamDArry[j][mSysStatus]=sMANUAL;
		sRamDArry[j][21]=sMANUAL;
	}

}



unsigned int   __attribute__((section(".usercode"))) Simulation()
{
	unsigned char x=0;


	if(WaitTime < 1500)	return(0);
	WaitTime=0;

    sRamDArry[x][S1_STATE]= 0;
    sRamDArry[x][S2_STATE]= 0;
    sRamDArry[x][S3_STATE]= 0;
    sRamDArry[x][S4_STATE]= 0;

    sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] |  S1_CLOSE); 
    sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] |  S1_AUTO); 
    sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] |  S2_CAR_MOVE); 
    sRamDArry[x][S3_STATE]= (sRamDArry[x][S3_STATE] |  S3_SHIFT); 

	sRamDArry[x][mSysStatus]= 65;


	if(updn){
		sRamDArry[x][0]++;
		if(sRamDArry[x][0] > 15){
			sRamDArry[x][0] = 15;	
			sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] & ~S1_UP);
			sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] | S2_DN); 
			sRamDArry[x][mSysStatus]= 65;
			updn=0;
		}	
		else{
			sRamDArry[x][mSysStatus]= 59;
			sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] |  S1_UP);
			sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] & ~S2_DN);  
		}			
	}
	else{
		sRamDArry[x][0]--;
		if((sRamDArry[x][0] == 0) || (sRamDArry[x][0] > 15)){
			sRamDArry[x][0]=1;	
			sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] |  S1_UP);
			sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] & ~S2_DN);  
			sRamDArry[x][mSysStatus]= 65;
			updn=1;
		}	
		else{
			sRamDArry[x][mSysStatus]= 60;
			sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] & ~S1_UP);
			sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] | S2_DN); 
		}
	}


    sRamDArry[x][DSP1]=(sRamDArry[x][0]/10) + '0';
    sRamDArry[x][DSP2]=(sRamDArry[x][0]%10) + '0';;

	return(0);

}








unsigned int   __attribute__((section(".usercode"))) PortSevenSeGIn(void)
{

    unsigned char tmp,change,status;
    unsigned char dsp1,dsp2;
    unsigned char in0,in1,x;

	x=0;
    change=0;

    dsp1=sRamDArry[x][DSP1];
    dsp2=sRamDArry[x][DSP2];

       
    tmp=0;
    if(!SEG_ADGH)   tmp=(tmp | 0x49);   
    if(!SEG_BH)     tmp=(tmp | 0x02);   
    if(!SEG_CH)     tmp=(tmp | 0x04);   
    if(!SEG_EH)     tmp=(tmp | 0x10);   
    if(!SEG_NFH)    tmp=(tmp | 0x20);   
    
	in0=tmp;
    switch(tmp){
       case  0x06:
          sRamDArry[x][DSP1]='1'; 
          break;
       case  0x5b:
       case  0x10:        
          sRamDArry[x][DSP1]='2';         
          break;
       case  0x4f:
       case  0x49:
          sRamDArry[x][DSP1]='3';         
          break;
       case  0x7f:
       case  0x20:
          sRamDArry[x][DSP1]='B';         
          break;
       default:
          sRamDArry[x][DSP1]='0';         
          break;               
    } 



    tmp=0;
    if(!SEG_AL)     tmp=(tmp | 0x01);   
    if(!SEG_BL)     tmp=(tmp | 0x02);   
    if(!SEG_CL)     tmp=(tmp | 0x04);   
    if(!SEG_DL)     tmp=(tmp | 0x08);   
    if(!SEG_EL)     tmp=(tmp | 0x10);   
    if(!SEG_FL)     tmp=(tmp | 0x20);   
    if(!SEG_G1L)    tmp=(tmp | 0x40);   
    if(!SEG_G2L)    tmp=(tmp | 0x80);   
	in1=tmp;


    switch(tmp){
       case    0x06:
       case    0x30:
           sRamDArry[x][DSP2]='1';         
           break;
       case    0x5b:
       case    0x9b:
       case    0xdb:
           sRamDArry[x][DSP2]='2';         
           break;
       case    0x4f:
       case    0x8f:
       case    0xcf:
           sRamDArry[x][DSP2]='3';         
           break;
       case  0x66:
       case  0xa6:
       case  0xe6:
          sRamDArry[x][DSP2]='4';         
          break;     
       case  0x71:
       case  0xb1:
       case  0xf1:
          sRamDArry[x][DSP2]='F';         
          break;     
       case  0x73:
       case  0xb3:
       case  0xf3:
          sRamDArry[x][DSP2]='P';         
          break;     
       case  0x6d:
       case  0xad:
       case  0xed:
          sRamDArry[x][DSP2]='5';         
          break;
       case  0x7d:
       case  0xbd:
       case  0xfd:
          sRamDArry[x][DSP2]='6';         
          break;
       case  0x07:
       case  0x27:
          sRamDArry[x][DSP2]='7';         
          break;
       case  0x7f:
       case  0xbf:
       case  0xff:
          sRamDArry[x][DSP2]='8';         
          break;
       case  0x67:
       case  0xaf:
       case  0xef:
          sRamDArry[x][DSP2]='9';         
          break;             
       case  0x3f:
          sRamDArry[x][DSP2]='0';         
          break;  
       default:
          sRamDArry[x][DSP2]='0';         
          break;  
    }       

    if((dsp1 != sRamDArry[x][DSP1]) || (dsp2 != sRamDArry[x][DSP2])){
        change=1;
    }


    if((in0==0x7f) && (in1==0xff)){
		status=sMANUAL;
		sRamDArry[x][mSysStatus]=status;
    	sRamDArry[x][DSP1]='0';
		sRamDArry[x][DSP2]='1';

        sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] & ~S1_AUTO);
		return(0);
    }


     UP=1;
     if(!UP) sRamDArry[x][1]= (sRamDArry[x][1] |  S1_UP); 
     else    sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_UP); 
 
     DN=1;
     if(!DN) sRamDArry[x][2]= (sRamDArry[x][2] |  S2_DN); 
     else    sRamDArry[x][2]= (sRamDArry[x][2] & ~S2_DN); 


    AT=1;
    if(AT){  //normal open
        sRamDArry[x][S2_STATE]= (sRamDArry[x][S2_STATE] |  S2_LAMP_USER); 
        sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] |  S1_AUTO); 
        sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] & ~S1_MANUAL);
		status=sREADY;
    }
    else{
        sRamDArry[x][S1_STATE]= (sRamDArry[x][S1_STATE] & ~S1_AUTO);
		status=sMANUAL;
		sRamDArry[x][mSysStatus]=status;

		return(0);
    }



    DOOR_CLOSE=1;
    if(DOOR_CLOSE){   //no close
		if((!UP || !DN)){
        	sRamDArry[x][3]= (sRamDArry[x][3] | S3_UPDN_VO);
		}
        sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_OPEN);
        sRamDArry[x][1]= (sRamDArry[x][1] | S1_CLOSE);
    }
    else{
        sRamDArry[x][3]= (sRamDArry[x][3] & ~S3_UPDN_VO);

        sRamDArry[x][1]= (sRamDArry[x][1] | S1_OPEN);
        sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_CLOSE);
    }


    FLOW=1;
    if(!FLOW){
        if(huntingtime>10){
            sRamDArry[x][2]= (sRamDArry[x][2] |  S2_FLOW); 
            WaitTime=0;
        }  
    }
    else{
        huntingtime=0;
        if(WaitTime > 500){
            sRamDArry[x][2]= (sRamDArry[x][2] & ~S2_FLOW);
            WaitTime=0;
        }
    }


    MOVE=1;
    if(MOVE){  //normal close
       	sRamDArry[x][3]= (sRamDArry[x][3] &  ~S3_SHIFT); 
       	sRamDArry[x][2]= (sRamDArry[x][2] &  ~S2_CAR_MOVE); 

		UP=1;
		if(!UP){
        	sRamDArry[x][3]= (sRamDArry[x][3] |  S3_SHIFT); 
        	sRamDArry[x][2]= (sRamDArry[x][2] |  S2_CAR_MOVE); 

			sRamDArry[x][1]= (sRamDArry[x][1] |  S1_UP); 
			status=sUP;
		}
		else    sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_UP); 
		
		DN=1;
		if(!DN){
        	sRamDArry[x][3]= (sRamDArry[x][3] |  S3_SHIFT); 
        	sRamDArry[x][2]= (sRamDArry[x][2] |  S2_CAR_MOVE); 

			sRamDArry[x][2]= (sRamDArry[x][2] |  S2_DN);
			status=sDN;
		}
        else    sRamDArry[x][2]= (sRamDArry[x][2] & ~S2_DN);
 
    }
    else{
       	sRamDArry[x][3]= (sRamDArry[x][3] &  ~S3_SHIFT); 
       	sRamDArry[x][2]= (sRamDArry[x][2] &  ~S2_CAR_MOVE); 

        UP=1;
        if(!UP) sRamDArry[x][1]= (sRamDArry[x][1] |  S1_UP); 
        else    sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_UP); 
    
        DN=1;
        if(!DN) sRamDArry[x][2]= (sRamDArry[x][2] |  S2_DN); 
        else    sRamDArry[x][2]= (sRamDArry[x][2] & ~S2_DN); 
    }


    FULL=1;
    if(!FULL)   sRamDArry[x][3]= (sRamDArry[x][3] | S3_FULL);
    else        sRamDArry[x][3]= (sRamDArry[x][3] & ~S3_FULL);                    

    OVERLOAD=1;
    if(!OVERLOAD){
 		sRamDArry[x][1]= (sRamDArry[x][1] | S1_OVERLOAD); 
		status=sOVL;

	}
    else	sRamDArry[x][1]= (sRamDArry[x][1] &     ~S1_OVERLOAD); 



    PARKING=1;
    if(!PARKING){
		sRamDArry[x][3]= (sRamDArry[x][3] | S3_PARKING);
		status=sPARKING;
	}
    else	sRamDArry[x][3]= (sRamDArry[x][3] & ~S3_PARKING);                    


    EMG=1;
    if(!EMG){
		sRamDArry[x][1]= (sRamDArry[x][1] | S1_EMG);
		status=sEMERGENCY;
	}
    else{
		sRamDArry[x][1]= (sRamDArry[x][1] & ~S1_EMG);
	}

	sRamDArry[x][mSysStatus]=status;


	return(0);

	
}    


/*

void    PortInBcd(void)
{

    unsigned char tmp,change;
    unsigned char dsp1,dsp2;

    change=0;

    dsp1=sRamDArry[DSP1];
    dsp2=sRamDArry[DSP2];


    if((MainTimer>10) && AT){
        MainTimer=0;

        sRamDArry[2]= (sRamDArry[2] &  ~S2_FLOW); 
        if(up1){
            up1=1;
            dn1=0;
            pt=(pt+1)%15;
            if(pt==14){
                up1=0;
                dn1=1;
            }
        }            
        else{
            up1=0;
            dn1=1;

            if(pt>0)   pt--;

            if(pt==0){
                up1=1;
                dn1=0;
            }
        }
        sRamDArry[DSP1]=DspChar[(pt*2)];
        sRamDArry[DSP2]=DspChar[(pt*2)+1];
    }

    if((MainTimer>2) && AT){
        sRamDArry[2]= (sRamDArry[2] |  S2_FLOW); 
    }

    if((dsp1 != sRamDArry[DSP1]) || (dsp2 != sRamDArry[DSP2])){
        sRamDArry[0]=sRamDArry[0]++;
        change=1;
    }


    AT=1;

    if(AT){
        sRamDArry[S2_STATE]= (sRamDArry[S2_STATE] |  S2_LAMP_USER); 
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] |  S1_AUTO); 
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] & ~S1_MANUAL);
    }
    else{
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] & ~S1_AUTO);
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] |  S1_MANUAL); 

        up1=0;
        dn1=0;
        sRamDArry[2]= (sRamDArry[2] &  ~S2_FLOW); 
    }

    OVERLOAD=1;
    if(!OVERLOAD)   sRamDArry[1]= (sRamDArry[1] | S1_OVERLOAD); 
    else            sRamDArry[1]= (sRamDArry[1] & ~S1_OVERLOAD); 


    MOVE=1;
    if(MOVE && AT){
		sRamDArry[3]= (sRamDArry[3] |  S3_SHIFT); 
		if(up1) sRamDArry[1]= (sRamDArry[1] |  S1_UP); 
		else    sRamDArry[1]= (sRamDArry[1] & ~S1_UP); 
		
		if(dn1) sRamDArry[2]= (sRamDArry[2] |  S2_DN); 
		else    sRamDArry[2]= (sRamDArry[2] & ~S2_DN); 
    }
    else{
        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 
        sRamDArry[1]= (sRamDArry[1] & ~S1_UP);     
        sRamDArry[2]= (sRamDArry[2] & ~S2_DN); 
    }


    EMG=1;
    if(!EMG){
        sRamDArry[1]= (sRamDArry[1] | S1_EMG);

        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 
        sRamDArry[1]= (sRamDArry[1] & ~S1_UP);     
        sRamDArry[2]= (sRamDArry[2] & ~S2_DN);    
    }
    else{
        sRamDArry[1]= (sRamDArry[1] & ~S1_EMG);                    
    }

    PARKING=1;
    if(!PARKING){
        sRamDArry[3]= (sRamDArry[3] | S3_PARKING);

        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 
        sRamDArry[1]= (sRamDArry[1] & ~S1_UP);     
        sRamDArry[2]= (sRamDArry[2] & ~S2_DN);    
    }
    else            sRamDArry[3]= (sRamDArry[3] & ~S3_PARKING);                    
}    



void    DirectPortIn(void)
{

    unsigned char change;
    unsigned char dsp1,dsp2;

    change=0;

    dsp1=sRamDArry[DSP1];
    dsp2=sRamDArry[DSP2];

    if(!SEG_AL && (SEG_AL_time < 30)){
        sRamDArry[DSP1]='B';         
        sRamDArry[DSP2]='2';         
    }
    else if(!SEG_BL && (SEG_BL_time < 30)){
        sRamDArry[DSP1]='B';         
        sRamDArry[DSP2]='1';         
    }
    else if(!SEG_CL && (SEG_CL_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='1';         
    }
    else if(!SEG_DL && (SEG_DL_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='2';         
    }
    else if(!SEG_EL && (SEG_EL_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='3';         
    }
    else if(!SEG_FL && (SEG_FL_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='4';         
    }
    else if(!SEG_G1L && (SEG_G1L_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='5';         
    }
    else if(!SEG_G2L && (SEG_G2L_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='6';         
    }
    else if(!SEG_ADGH && (SEG_ADGH_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='7';         
    }
    else if(!SEG_BH && (SEG_BH_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='8';         
    }
    else if(!SEG_CH && (SEG_CH_time < 30)){
        sRamDArry[DSP1]='0';         
        sRamDArry[DSP2]='9';         
    }
    else if(!SEG_EH && (SEG_EH_time < 30)){
        sRamDArry[DSP1]='1';         
        sRamDArry[DSP2]='0';         
    }
    else if(!SEG_NFH && (SEG_NFH_time < 30)){
        sRamDArry[DSP1]='1';         
        sRamDArry[DSP2]='1';         
    }
       

    if((dsp1 != sRamDArry[DSP1]) || (dsp2 != sRamDArry[DSP2])){
        sRamDArry[0]=sRamDArry[0]++;
        change=1;
    }


    FULL=1;
    if(!FULL && (FULL_time < 30))   sRamDArry[3]= (sRamDArry[3] | S3_FULL);
    else        					sRamDArry[3]= (sRamDArry[3] & ~S3_FULL);                    


    OVERLOAD=1;
    if(!OVERLOAD && (OVERLOAD_time < 30))   sRamDArry[1]= (sRamDArry[1] |      S1_OVERLOAD); 
    else            sRamDArry[1]= (sRamDArry[1] &     ~S1_OVERLOAD); 

    AT=1;
    if(!AT && (AT_time < 30)){
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] |  S1_AUTO); 
        if(!OVERLOAD){
            sRamDArry[S2_STATE]= (sRamDArry[S2_STATE] |  S2_LAMP_USER); 
        }
        else{
            sRamDArry[S2_STATE]= (sRamDArry[S2_STATE] & ~S2_LAMP_USER); 
        }
    }
    else{
        sRamDArry[S1_STATE]= (sRamDArry[S1_STATE] & ~S1_AUTO);
        sRamDArry[S2_STATE]= (sRamDArry[S2_STATE] & ~S2_LAMP_USER); 
        sRamDArry[3]       = (sRamDArry[3] & ~S3_FULL);            
    }



    MOVE=1;
    if(!MOVE && (MOVE_time < 30)){
        sRamDArry[3]= (sRamDArry[3] |  S3_SHIFT); 
        if( !(sRamDArry[1] & S1_UP) && !(sRamDArry[2] & S2_DN)){
            UP=1;
            if(!UP && (UP_time < 30)) sRamDArry[1]= (sRamDArry[1] |  S1_UP); 
            else    sRamDArry[1]= (sRamDArry[1] & ~S1_UP); 
        
            DN=1;
            if(!DN && (DN_time < 30)) sRamDArry[2]= (sRamDArry[2] |  S2_DN); 
            else    sRamDArry[2]= (sRamDArry[2] & ~S2_DN); 
        } 
    }
    else{
        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 

        UP=1;
        if(!UP && (UP_time < 30)) sRamDArry[1]= (sRamDArry[1] |  S1_UP); 
        else    sRamDArry[1]= (sRamDArry[1] & ~S1_UP); 
    
        DN=1;
        if(!DN && (DN_time < 30)) sRamDArry[2]= (sRamDArry[2] |  S2_DN); 
        else    sRamDArry[2]= (sRamDArry[2] & ~S2_DN); 
    }

    DOOR_CLOSE=1;
    if((!DOOR_CLOSE  && (DOOR_CLOSE_time < 30)) && ( (!UP  && (UP_time < 30))  || (!DN  && (DN_time < 30)))){
        sRamDArry[3]= (sRamDArry[3] | S3_UPDN_VO);
    }
    else{
        sRamDArry[3]= (sRamDArry[3] & ~S3_UPDN_VO);
    }


    EMG=1;
    if(!EMG && (EMG_time < 30)){
        sRamDArry[1]= (sRamDArry[1] | S1_EMG);

        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 
        sRamDArry[1]= (sRamDArry[1] & ~S1_UP);     
        sRamDArry[2]= (sRamDArry[2] & ~S2_DN);    
        sRamDArry[3]= (sRamDArry[3] & ~S3_FULL);            

    }
    else{
        sRamDArry[1]= (sRamDArry[1] & ~S1_EMG);                    
    }


    PARKING=1;
    if(!PARKING && (PARKING_time < 30)){
        sRamDArry[3]= (sRamDArry[3] | S3_PARKING);

        sRamDArry[3]= (sRamDArry[3] & ~S3_SHIFT); 
        sRamDArry[1]= (sRamDArry[1] & ~S1_UP);     
        sRamDArry[2]= (sRamDArry[2] & ~S2_DN);    
    }
    else            sRamDArry[3]= (sRamDArry[3] & ~S3_PARKING);                    

}    
*/





#define  sINSPECT 			  0 
#define  sInvertErr			  1                             
#define  sI_C_N_FLR           2
#define  sEMERGENCY           3
#define  sLOPE_BRK            4
#define  sReserve1            5
#define  sSLIP                6
#define  sULS                 7
#define  sDLS                 8
#define  sLULD_NO_OFF         9
#define  sNEXT_FLOOR          10
#define  sInPortError         11
#define  sHDS_RUN_OFF         12
#define  sCLE_RUN_OFF         13
#define  sReserve2            14
/////////////////////////////////////
#define  sEncoderErr          15
#define  sEncoderABErr        16 
#define  sSpeedSetError       17 
#define  sSpeedPortError      18 
#define  sNoUseSpeed          19 
#define  sMinLengthErr        20 
#define  sEqualFloorError     21 
#define  sSystemErr           22
#define  sBreakMgtOpen        23  
#define  sBreakOpen           24  
#define  sSusErr              25  
#define  sSdsErr              26  
//////////////////////////////////
#define  sLuOrLdErr0          27  
#define  sLuOrLdErr1          28  
#define  sLuOrLdErr2          29  
#define  sLuOrLdErr3          30  
#define  sLuOrLdErr4          31  
#define  sLuOrLdErr5          32  
#define  sCarDoor_Jumper      33
#define  sHoleDoor_Jumper     34
#define  sCarHoleDoor_Jumper  35

/////////////////////////////////////
#define  sFhmError            43
//////////////////////////////////////
#define  sHDS_NO_ON           44
#define  sCLE_NO_ON           45
#define  sOVL                 46 
#define  sPARKING             47   
#define  sFireOn              48         
#define  sWaterSense          49         
#define  sOPE_NO_ON           50                                                                            
#define  sVIP                 51
#define  sFHM_MODE			  52
#define  sMANUAL_UP           53         
#define  sMANUAL_DN           54         
#define  sMANUAL              55         
#define  sSFT                 56
#define  sOPEN                57
#define  sCLOSE               58         
#define  sUP                  59         
#define  sDN                  60         
#define  sSLOW_SPEED          61         
#define  sWAIT_LULD           62         
#define  sSTOP                63        
#define  sHOME                64         
#define  sREADY               65


/*  ���� 485 ������ �������� ����Ʈ ���� */
// 'S' : 0x53
#define mySTART			0	
// ȣ�� ��ȣ 
#define	mySTX			1	
#define NotUse2			2
// �� ��ġ 
#define myFLOOR_DSP1	3	
#define myFLOOR_DSP2	4
// -�̻��-
#define myNotUse5		5
#define myNotUse6		6
#define myNotUse7		7
#define myNotUse8		8
#define myNotUse9		9
#define myNotUse10		10
// �ӵ� 
#define mySPEED1		11
#define mySPEED2		12
#define mySPEED3		13
// ��, �ٿ� ����, 0�� ���� ���� 
#define myUPDN			14
// ���� Open/CLose
#define myDOOR			15
// �ڵ�, ����, ȭ�� ���� ���� ���� 
#define mySTATE			16
// �˶��ڵ� 
#define myALARM1		17
#define myALARM2		18
// ī Ű ���� 
#define myCAR_KEY1		19	
#define myCAR_KEY2		20	
#define myCAR_KEY3		21	
#define myCAR_KEY4		22	
// Skip(Service)
#define mySERVICE1		23
#define mySERVICE2		24
#define mySERVICE3		25
#define mySERVICE4		26
// -�̻��-
#define	myNotUse27		27
#define	myNotUse28		28
#define	myNotUse29		29
#define	myNotUse30		30
#define	myNotUse31		31
#define	myNotUse32		32
#define	myNotUse33		33
#define	myNotUse34		34
#define	myNotUse35		35
// ��ŷ 
#define myPARKING		36

unsigned int   __attribute__((section(".usercode"))) PLCInData(void)
{
    unsigned int bitflr;

    unsigned char dsp1,dsp2;
    unsigned char in0,in1,x;

	x=0;

	if(Com2RxBuffer[myFLOOR_DSP1] >= 'A')	dsp1=(Com2RxBuffer[3] - '7');
	else						dsp1=(Com2RxBuffer[3] - '0');
	dsp1=(dsp1 << 4);

	if(Com2RxBuffer[myFLOOR_DSP2] >= 'A')	dsp2=(Com2RxBuffer[4] - '7');
	else						dsp2=(Com2RxBuffer[4] - '0');
	 
	dsp1=(dsp1 | dsp2);

	sRamDArry[x][S0_FLOOR]=dsp1;


	dsp1--;
	dsp1=(dsp1 *2);
	sRamDArry[x][DSP1]=DftFlrName[dsp1];
	sRamDArry[x][DSP2]=DftFlrName[dsp1 + 1];
//567890

	mpm= (Com2RxBuffer[mySPEED1] - '0') * 100;
	mpm=((Com2RxBuffer[mySPEED2] - '0') * 10) + mpm;
	mpm= (Com2RxBuffer[mySPEED3] - '0') + mpm;
	mpm= (mpm * 10);


	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_UP);
	sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] & ~S2_DN);
	sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] & ~S3_STOP);


//	sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] | S2_CAR_MOVE);

	if( (Com2RxBuffer[myUPDN]== '3') ||  (Com2RxBuffer[myUPDN]== '4')){
		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] | S2_CAR_MOVE);
	}
	else{
		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] & ~S2_CAR_MOVE);
	}	


/*
	if(Com2RxBuffer[14]== '0'){
		sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] | S3_STOP);		
		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] & ~S2_CAR_MOVE);
	}
*/

	if(Com2RxBuffer[myUPDN]== '1')		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_UP);		
	if(Com2RxBuffer[myUPDN]== '2')		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] | S2_DN);		
//	if(Com2RxBuffer[14]== '3')		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_UP);		
//	if(Com2RxBuffer[14]== '4')		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] | S2_DN);		



	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_OPEN);
	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_CLOSE);
	if(Com2RxBuffer[myDOOR]== '0')		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_OPEN);		
	if(Com2RxBuffer[myDOOR]== '2')		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_OPEN);		
	if(Com2RxBuffer[myDOOR]== '1')		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_CLOSE);		


	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_AUTO);
	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_BAT);
	sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_EMG);

	sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] & ~S2_FIRE);
	sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] & ~S3_VIP);

	if(Com2RxBuffer[mySTATE]== '9')			
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_EMG);
	else if(Com2RxBuffer[mySTATE]== '5'){
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_BAT); //  
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);
	}
	else if(Com2RxBuffer[mySTATE]== '0')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);		
	else if(Com2RxBuffer[mySTATE]== '1')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);		
	else if(Com2RxBuffer[mySTATE]== '2')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);		
	else if(Com2RxBuffer[mySTATE]== '3')		
		sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] | S3_VIP); // ������� 		
	else if(Com2RxBuffer[mySTATE]== '4')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] & ~S1_AUTO);	// ����, ������ 	
	else if(Com2RxBuffer[mySTATE]== '6')		
		sRamDArry[x][S2_STATE]=(sRamDArry[x][S2_STATE] | S2_FIRE); // ȭ�� 
	else if(Com2RxBuffer[mySTATE]== '7')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);
	else if(Com2RxBuffer[mySTATE]== '8')		
		sRamDArry[x][S1_STATE]=(sRamDArry[x][S1_STATE] | S1_AUTO);


	sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] & ~S3_PARKING);
	if(Com2RxBuffer[myPARKING]=='1')	
		sRamDArry[x][S3_STATE]=(sRamDArry[x][S3_STATE] | S3_PARKING);;



	if(sRamDArry[x][S1_STATE] & S1_EMG){
		sRamDArry[x][mSysStatus]=sEMERGENCY;
	}
	else if(sRamDArry[x][S3_STATE] & S3_VIP){
		sRamDArry[x][mSysStatus]=sVIP;
	}
	else if(sRamDArry[x][S2_STATE] & S2_FIRE){
		sRamDArry[x][mSysStatus]=sFireOn;
	}
	else if(sRamDArry[x][S3_STATE] & S3_PARKING){
		sRamDArry[x][mSysStatus]=sPARKING;
	}
	else if( !(sRamDArry[x][S1_STATE] & S1_AUTO)){
		if(sRamDArry[x][S3_STATE] & S3_STOP){
			sRamDArry[x][mSysStatus]=sMANUAL;
		}
		else if(sRamDArry[x][S1_STATE] & S1_UP){
			sRamDArry[x][mSysStatus]=sMANUAL_UP;
		}
		else if(sRamDArry[x][S2_STATE] & S2_DN){
			sRamDArry[x][mSysStatus]=sMANUAL_DN;
		} 
	}
	else{
		if(sRamDArry[x][S1_STATE] & S1_UP){
			sRamDArry[x][mSysStatus]=sUP;
		}
		else if(sRamDArry[x][S2_STATE] & S2_DN){
			sRamDArry[x][mSysStatus]=sDN;
		}

 
//		else if(sRamDArry[x][S3_STATE] & S3_STOP){
		else if( !(sRamDArry[x][S2_STATE] & S2_CAR_MOVE)){
			if(sRamDArry[x][S1_STATE] & S1_CLOSE){
				sRamDArry[x][mSysStatus]=sREADY;
			}
			else if(sRamDArry[x][S1_STATE] & S1_OPEN){
				sRamDArry[x][mSysStatus]=sOPEN;
			}
		}

	}


//17,18
	

	if(Com2RxBuffer[myCAR_KEY4] >= 'A')		dsp1= Com2RxBuffer[myCAR_KEY4] - '7';
	else							dsp1= Com2RxBuffer[myCAR_KEY4] - '0';
	sRamDArry[x][mCarKey1]=dsp1;

	if(Com2RxBuffer[myCAR_KEY3] >= 'A')		dsp1= Com2RxBuffer[myCAR_KEY3] - '7';
	else							dsp1= Com2RxBuffer[myCAR_KEY3] - '0';
	dsp1=(dsp1 << 4);
	sRamDArry[x][mCarKey1]=(sRamDArry[x][mCarKey1] | dsp1);


	if(Com2RxBuffer[myCAR_KEY2] >= 'A')		dsp1= Com2RxBuffer[myCAR_KEY2] - '7';
	else							dsp1= Com2RxBuffer[myCAR_KEY2] - '0';
	sRamDArry[x][mCarKey9]=dsp1;

	if(Com2RxBuffer[myCAR_KEY1] >= 'A')		dsp1= Com2RxBuffer[myCAR_KEY1] - '7';
	else							dsp1= Com2RxBuffer[myCAR_KEY1] - '0';
	dsp1=(dsp1 << 4);
	sRamDArry[x][mCarKey9]=(sRamDArry[x][mCarKey9] | dsp1);



	if(Com2RxBuffer[mySERVICE4] >= 'A')		dsp1= Com2RxBuffer[mySERVICE4] - '7';
	else							dsp1= Com2RxBuffer[mySERVICE4] - '0';
	sRamDArry[x][FLR_ON_OFF0]=dsp1;

	if(Com2RxBuffer[mySERVICE3] >= 'A')		dsp1= Com2RxBuffer[mySERVICE3] - '7';
	else							dsp1= Com2RxBuffer[mySERVICE3] - '0';
	dsp1=(dsp1 << 4);
	sRamDArry[x][FLR_ON_OFF0]=(sRamDArry[x][FLR_ON_OFF0] | dsp1);

	if(Com2RxBuffer[mySERVICE2] >= 'A')		dsp1= Com2RxBuffer[mySERVICE2] - '7';
	else							dsp1= Com2RxBuffer[mySERVICE2] - '0';
	sRamDArry[x][FLR_ON_OFF1]=dsp1;

	if(Com2RxBuffer[mySERVICE1] >= 'A')		dsp1= Com2RxBuffer[mySERVICE1] - '7';
	else							dsp1= Com2RxBuffer[mySERVICE1] - '0';
	dsp1=(dsp1 << 4);
	sRamDArry[x][FLR_ON_OFF1]=(sRamDArry[x][FLR_ON_OFF1] | dsp1);


//27,28,29,30



//error status 
	dsp1=Com2RxBuffer[myALARM1];
	dsp2=Com2RxBuffer[myALARM2];
	if(Com2RxBuffer[myALARM1] >= 'A')	dsp1=(Com2RxBuffer[myALARM1] - '7');
	else						dsp1=(Com2RxBuffer[myALARM1] - '0');
	dsp1=(dsp1 << 4);

	if(Com2RxBuffer[myALARM2] >= 'A')	dsp2=(Com2RxBuffer[myALARM2] - '7');
	else						dsp2=(Com2RxBuffer[myALARM2] - '0');
	 
	dsp1=(dsp1 | dsp2);

	if((dsp1 > 0) && (dsp1 < 33)){
		if((dsp1 < 17) || (dsp1 > 27)){
			sRamDArry[x][mSysStatus]=dsp1;
		}
	}

/*
	switch(dsp1){
		case	1:	
			sRamDArry[x][mSysStatus]=sEMERGENCY;
			break;
		case	2:	
			sRamDArry[x][mSysStatus]=sHDS_RUN_OFF;
			break;
		case	3:	
			sRamDArry[x][mSysStatus]=sCLE_RUN_OFF;
			break;
		case	4:	
			sRamDArry[x][mSysStatus]=sOPE_NO_ON;
			break;
		case	5:	
			sRamDArry[x][mSysStatus]=sCLE_NO_ON;
			break;
		case	6:	
			sRamDArry[x][mSysStatus]=sCarDoor_Jumper;
			break;
		case	7:	
			sRamDArry[x][mSysStatus]=sLOPE_BRK;
			break;
		case	8:	
			sRamDArry[x][mSysStatus]=sInvertErr;
			break;
		case	9:
			sRamDArry[x][mSysStatus]=sLuOrLdErr4;  // lu no on	
			break;
		case	10:	
			sRamDArry[x][mSysStatus]=sLuOrLdErr2;  // ld no on	
			break;
		case	11:	
			sRamDArry[x][mSysStatus]=sSusErr;
			break;
		case	12:	
			sRamDArry[x][mSysStatus]=sSdsErr;
			break;
		case	13:	
			sRamDArry[x][mSysStatus]=sULS;
			break;
		case	14:	
			sRamDArry[x][mSysStatus]=sDLS;
			break;
		case	15:	
			sRamDArry[x][mSysStatus]=sBreakOpen;
			break;
		case	16:	
			sRamDArry[x][mSysStatus]=sNEXT_FLOOR;
			break;
		case	17:	
			break;
		case	18:	
			break;
		case	19:	
			break;
		case	20:	
			break;
		case	21:	
			break;
		case	22:	
			break;
		case	23:	
			break;
		case	24:	
			break;
		case	25:	
			break;
		case	26:	
			break;
		case	27:	
			break;
		case	28:	
			sRamDArry[x][mSysStatus]=sINSPECT;  //??
			break;
		case	29:	
			sRamDArry[x][mSysStatus]=sINSPECT;  //??
			break;
		case	30:	
			sRamDArry[x][mSysStatus]=sSFT;
			break;
		case	31:	
			sRamDArry[x][mSysStatus]=sOPE_NO_ON;
			break;
		case	32:	
			sRamDArry[x][mSysStatus]=sCLE_NO_ON;
			break;
	}
*/
	return(0);	
}    

