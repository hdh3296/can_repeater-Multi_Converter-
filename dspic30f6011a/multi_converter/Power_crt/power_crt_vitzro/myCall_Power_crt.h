


#define     CRT_COMPANY			'D'
#define     MAX_ELEV            9
#define     HOST_DATA_RECOD     60      

#define     CRT_TX_DATA_TO_PC   60   

#define		COM1_BAUD		BAUD_RATE_38400		
#define		COM2_BAUD		BAUD_RATE_19200	


#define         MAX_COM1_TXBUF     	100
#define         MAX_COM1_RXBUF     	100
#define         MAX_COM2_TXBUF     	100
#define         MAX_COM2_RXBUF     	100

#define         CAN1_MAX_TX_BUF     100
#define         CAN1_MAX_RX_BUF     100
#define         CAN2_MAX_TX_BUF     100
#define         CAN2_MAX_RX_BUF     100




#define		HOST_LIVE_TIME     10		//10 sec


#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV


#define		HOST_CALL_TIME     40		//30msec


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

extern	unsigned	char  	ThisHostSelect;
extern	unsigned	char  	SubRdCmd;



#define	VIPAM_3000_FI		1	
#define	VIDER_M20			2	
#define	GIPAM_115			3    
#define	VIDER_DC			4	
