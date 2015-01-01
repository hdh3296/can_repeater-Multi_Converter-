

#include <p30f6011a.h>            

#include  	"host_io.h"            
#include  	"30f6010_io.h"
#include  	"multi_converter.h"
#include  	"you_can1.h"
#include  	"you_can2.h"
#include  	"ComPort1.h"
#include  	"ComPort2.h"
#include  	"WorkingRam.h"


_FOSC(CSW_FSCM_OFF & XT_PLL8 & 0xc71f);
_FWDT(WDT_ON & WDTPSA_64 & WDTPSB_1 & 0x803f);
//_FWDT(WDT_OFF & WDTPSA_64 & WDTPSB_1 & 0x803f);
_FBORPOR(PBOR_ON & BORV_27 & PWRT_64 & MCLR_EN & 0x87b3);
_FGS(0x0007);



#define		VERSION					1

#define		TRUE					1
#define		FALSE					0


/*
const unsigned char StatusMessage[][16]={
                                      "Inspection Stop ",      //000                               
                                      "Inverter  Error ",      //001                               
                                      "INV Floor Error ",      //002                               
                                      "Emergency Stop  ",      //003                               
                                      "GOV Error       ",      //004                               
                                      "Motor Overheat  ",      //005                                                                           
                                      "Slip Stop       ",      //006                               
                                      "ULS Stop        ",      //007                                                                     
                                      "DLS Stop        ",      //008                               
                                      "LuLd Off Stop   ",      //009
                                      "Next Floor Stop ",      //010
                                      "InPort Error    ",      //011
                                      "Hall Door Stop  ",      //012
                                      "Car Door  Stop  ",      //013
                                      "Reseve Error    ",      //014
//                                      "LuLd Error      ",      //014
                                      "No Encoder Pulse",      //015
                                      "Encoder A-B Err ",      //016
                                      "ElevSpeed Error ",      //017
                                      "Speed Port Error",      //018
                                      "No Use Speed    ",      //019
                                      "Min Dec Length  ",      //020
                                      "Flr Match Error ",      //021
                                      "System Error    ",      //022
                                      "Break Mgt Error ",      //023
                                      "Break On/Off Err",      //024
                                      "Sus Error       ",      //025
                                      "Sds Error       ",      //026
									  "LuLd ChangeError",      //027 
									  "Ld On Error	   ",      //028
									  "Ld Off Error    ",      //029
									  "Lu On Error     ",      //030
									  "Lu Off Error    ",      //031
									  "LuLd OnOff Error",      //032
                                      "CarDoor Jumper  ",      //033                                                                           
                                      "HallDoor Jumper ",      //034
                                      "CarHall Jumper  ",      //035
                                      "036             ",      //036
                                      "037             ",      //037
                                      "038             ",      //038
                                      "039             ",      //039
                                      "040             ",      //040
                                      "041             ",      //041
                                      "042             ",      //042
                                      "FHM_Error       ",      //043
                                      "No Hall Door Sw ",      //044
                                      "No Close End    ",      //045
                                      "Over Load Stop  ",      //046
                                      "Parking Mode    ",      //047
                                      "   Fire         ",      //048
                                      "Water_Sensing   ",      //049                                       
                                      "No Open End     ",      //050
                                      "VIP Drive       ",      //051                                     
                                      "FHM_Mode        ",      //052                                     
                                      "Manual Up       ",      //053
                                      "Manual Dn       ",      //054
                                      "Manual Mode     ",      //055
                                      "Safety Open     ",      //056
                                      "Open Door       ",      //057
                                      "Close Door      ",      //058                                     
                                      "Auto Up         ",      //059                                     
                                      "Auto Dn         ",      //060                                     
                                      "Slow Speed      ",      //061
                                      "Wait LuLd       ",      //062
                                      "Auto Stop       ",      //063
                                      "Wait... Home !  ",      //064
                                      "Auto Ready      ",      //065
                                    };                                       
*/



const unsigned char default_x[50]={"0"}; 


/////////////////////////////////

long_field  UserPgmConfigBuf[FLASH_WRITE_BLOCK]; 
long_field  UserPgmConfig[USER_FLASH_BYTE]    	__attribute__((aligned(USER_FLASH_BYTE) , section(".auto_psv,r")));

////////////////////////////////


unsigned    char  	msec100=0;
unsigned    char  	sec1=0;
unsigned    char	SaveVerify=0;


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

		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyProductIdValue_A)) = 'A';
		Set_Byte_Flash_Buf((unsigned int)(cF_SetMyAddr1Value_A))     = 0xfd;  // my set address
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


		Set_Integer_Flash_Buf((unsigned int)(iF_Com1Baudrate_A))           	= 52;   //19200
		Set_Integer_Flash_Buf((unsigned int)(iF_Com2Baudrate_A))           	= 52;	//19200


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


	TRIS_RG15_IO=0;
	RG15_OUT=0;


	for(j=0;j<1000;j++){
		asm("CLRWDT");
	}

    asm("CLRWDT");
	ReadInitSetupData();
    asm("CLRWDT");


	RG15_OUT=1;



    INTCON1bits.NSTDIS=1;
    LvdInit();
    Timer1Init();
    Com1Init(BAUD_RATE_19200);
    Com2Init(BAUD_RATE_19200);
    Can1Init();
	Can2Init();
    INTCON1bits.NSTDIS=1;


    if(!SWR){
        asm("RESET");
    }
    SWR=0;


	My_Project_Init();

    do{
		My_Project_Start();


		if( (Com2TxThisPt >= Com2TxCnt) && (_U2TRMT)){
			TXEN_485= 0; 		
			if(Com2RxStatus == TX_SET){
				Com2RxStatus = STX_CHK;
			}
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
		if(Can1RxTimer < 250) 		Can1RxTimer++;
		if(Can2RxTimer < 250) 		Can2RxTimer++;
      	if(Com1SerialTime < 250)	Com1SerialTime++;
      	if(Com2SerialTime < 650)	Com2SerialTime++;
      	if(TXEN485Timer < 250)		TXEN485Timer++;

		msecTimer1();
	

		msec100++;
        if(msec100 >= 100){
            msec100=0;		
			RUN_LED= !RUN_LED;
			MsecTimer_100();

            sec1++;
            if(sec1 >= 10){
                sec1=0;
				SecTimer_1();
            }
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









