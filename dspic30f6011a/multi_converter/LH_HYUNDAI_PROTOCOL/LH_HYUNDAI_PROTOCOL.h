

#define         I_AM_LISTENING_MODE	0		// 0 == data req to host    //1 == not data req

#define         MAX_COM1_TXBUF     	100
#define         MAX_COM1_RXBUF     	100
#define         MAX_COM2_TXBUF     	100
#define         MAX_COM2_RXBUF     	100

#define         CAN1_MAX_TX_BUF     100
#define         CAN1_MAX_RX_BUF     100
#define         CAN2_MAX_TX_BUF     100
#define         CAN2_MAX_RX_BUF     100


#define     CRT_COMPANY				'A'
#define     CRT_TX_DATA_TO_PC   	88   //40   40= baudrate=19200   88= baudrate=38400


#define		HOST_LIVE_TIME     10		//10 sec


#define     MAX_ELEV            4
#define     HOST_DATA_RECOD     50      
#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV




#ifndef	MY_STRUCTURE
#define	MY_STRUCTURE	1
typedef	struct x_tagReqHost{
        unsigned char	HostCmpanyId;
        unsigned int  	HostSid;
        unsigned long   HostEid;
        unsigned char   HostGroup;
        unsigned char   Ho_Host[4];
        unsigned char   HostB;
}tagReqHost;

#endif


extern volatile tagReqHost ReqHost[MAX_ELEV];
extern  unsigned    char    RcvBuf[MAX_SAVE_BUF];
extern	unsigned	char  	HostElevLive[MAX_ELEV];
extern  unsigned    char    CommonPcCmd_Buf[20];  



/*
extern	unsigned char   UpRegistry[32][7];  				///類 //撮渠熱							        
extern	unsigned char   DnRegistry[32][7];  				///類 //撮渠熱
extern	unsigned char   UpCall[32][7];      				///類 //撮渠熱							        
extern	unsigned char   DnCall[32][7];      				///類 //撮渠熱
extern	unsigned char   InfoCall[32][7];    				///類 //撮渠熱
extern	unsigned char   UpKeyTx[32][7];
extern	unsigned char   DnKeyTx[32][7];
extern	unsigned char   LocationCallTime[32][7];            // 01,02,03,04,05,06  ��    


extern	unsigned 	int  	StatusTxTime; 
extern	unsigned 	int		ElevatorServerAliveTime;
extern	unsigned 	char	ThisHost;





extern	unsigned    char    HomeCallUpKey[4];extern	unsigned    char    HomeCallDnKey[4];extern	unsigned    char    HomeCallCarKey[4];
*/



