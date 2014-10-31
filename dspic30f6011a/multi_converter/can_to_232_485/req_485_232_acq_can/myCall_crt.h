
// rxtimer1
// rxtimer1
// HOST_CALL_WAIT_TIME

#define		IAM_RECEIVE_BUF		1

#define     CRT_COMPANY			'D'
#define     MAX_ELEV            8



#define         I_AM_LISTENING_MODE	0		// 0 == data req to host    //1 == not data req

#define         MAX_COM1_TXBUF     	100
#define         MAX_COM1_RXBUF     	100
#define         MAX_COM2_TXBUF     	100
#define         MAX_COM2_RXBUF     	100

#define         CAN1_MAX_TX_BUF     100
#define         CAN1_MAX_RX_BUF     100
#define         CAN2_MAX_TX_BUF     100
#define         CAN2_MAX_RX_BUF     100


#define     CRT_TX_DATA_TO_PC   	88   //40   40= baudrate=19200   88= baudrate=38400


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





