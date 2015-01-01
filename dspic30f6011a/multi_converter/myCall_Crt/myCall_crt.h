


#define     BLOCK_4000				1   // 40단지 인경우 사용해야함




#define     CRT_COMPANY				'A'
#define     MAX_ELEV            	1
#define     CRT_TX_DATA_TO_PC   	88   //40   40= baudrate=19200   88= baudrate=38400

////////////////
//////BAUD_RATE_9600,BAUD_RATE_19200,BAUD_RATE_38400,BAUD_RATE_57600//////////
///////////////
#define		COM1_BAUD		BAUD_RATE_38400		
#define		COM2_BAUD		BAUD_RATE_38400	



#define		I_AM_CALL_HOST		1
//#define		I_AM_VIRTUAL_HOST	1
//#define		I_AM_232_CAN		1
//#define		I_AM_485_CAN		1








#define 	MAX_COM1_TXBUF     	100
#define     MAX_COM1_RXBUF     	100
#define     MAX_COM2_TXBUF     	100
#define     MAX_COM2_RXBUF     	100

#define    	CAN1_MAX_TX_BUF     100
#define     CAN1_MAX_RX_BUF     100
#define     CAN2_MAX_TX_BUF     100
#define     CAN2_MAX_RX_BUF     100



#define		HOST_LIVE_TIME     10		//10 sec


#define     HOST_DATA_RECOD     50      
#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV


#define		HOST_CALL_TIME     		20		//polling time


#ifndef	MY_STRUCTURE
#define	MY_STRUCTURE	1
typedef	struct x_tagReqHost{
        unsigned char	HostCmpanyId;
        unsigned int  	HostSid;
        unsigned long   HostEid;
        unsigned char   HostGroup;
        unsigned char   Ho_Host[10];
        unsigned char   HostB;
}tagReqHost;

#endif


extern volatile tagReqHost ReqHost[MAX_ELEV];
extern  unsigned    char    RcvBuf[MAX_SAVE_BUF];
extern	unsigned	char  	HostElevLive[MAX_ELEV];


extern	unsigned	char  	AckWrCmd;
extern	unsigned	char  		bWrCmd;




#if  defined(I_AM_CALL_HOST)  && defined(I_AM_VIRTUAL_HOST) && defined(I_AM_232_CAN) && defined(I_AM_485_CAN)
	#error  "Multiple Define Error" 
#endif


#if		defined(I_AM_CALL_HOST)
#elif 	defined(I_AM_CALL_HOST)
#elif	defined(I_AM_VIRTUAL_HOST)
#elif	defined(I_AM_232_CAN)
#elif	defined(I_AM_485_CAN)
#else
	#error  "No Define Error" 
#endif


	

