


#define         I_AM_485_232_HOST		1		// 0 == data req to host    //1 == not data req
//#define         I_AM_HOST_CALL			1		// 0 == data req to host    //1 == not data req




#define     CRT_COMPANY				'D'
#define     CRT_TX_DATA_TO_PC   	40   //40   88



////////////////
//////BAUD_RATE_9600,BAUD_RATE_19200,BAUD_RATE_38400,BAUD_RATE_57600//////////
///////////////
#define		COM1_BAUD		BAUD_RATE_19200		
#define		COM2_BAUD		BAUD_RATE_19200	


#define     MAX_ELEV            32



#define         MAX_COM1_TXBUF     	100
#define         MAX_COM1_RXBUF     	100
#define         MAX_COM2_TXBUF     	100
#define         MAX_COM2_RXBUF     	100

#define         CAN1_MAX_TX_BUF     100
#define         CAN1_MAX_RX_BUF     100
#define         CAN2_MAX_TX_BUF     100
#define         CAN2_MAX_RX_BUF     100


#define		HOST_LIVE_TIME     10		//10 sec


#define     HOST_DATA_RECOD     50      
#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV




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







