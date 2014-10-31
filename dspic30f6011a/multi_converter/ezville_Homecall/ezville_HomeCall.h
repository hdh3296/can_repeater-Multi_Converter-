


#define         I_AM_LISTENING_MODE	1


#define         MAX_COM1_TXBUF     1000
#define         MAX_COM1_RXBUF     200
#define         MAX_COM2_TXBUF     100
#define         MAX_COM2_RXBUF     100

#define         CAN1_MAX_TX_BUF     100
#define         CAN1_MAX_RX_BUF     100
#define         CAN2_MAX_TX_BUF     200
#define         CAN2_MAX_RX_BUF     100



#define     CRT_COMPANY				'A'


#define     MAX_ELEV            6

#define     HOST_DATA_RECOD     50      
#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV


			
#define		HOMECALL_BUTTON_SET_CMD		0x87
#define		INFO_TX_TIME				15   	//1.5sec
#define		MAX_ALIVE_TIME				120  	// 120sec



#define	MAX_HO_PER_FLOOR	4    						// 1 Ãþ´ç ÃÖ´ë ¹æ °¹¼ö bit    101,102,103,104È£ µîµî 
#define	MAX_SUB_FLOOR_CNT	(MAX_HO_PER_FLOOR-1) * 8    // 1 ºÎÅÍ 6  ±îÁö  6*8 = 48È£½Ç/Ãþ´ç 


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



extern	unsigned char   UpRegistry[32][MAX_HO_PER_FLOOR];  		///Ãþ //¼¼´ë¼ö							        
extern	unsigned char   DnRegistry[32][MAX_HO_PER_FLOOR];  		///Ãþ //¼¼´ë¼ö
extern	unsigned char   UpCall[32][MAX_HO_PER_FLOOR];      		///Ãþ //¼¼´ë¼ö							        
extern	unsigned char   DnCall[32][MAX_HO_PER_FLOOR];      		///Ãþ //¼¼´ë¼ö
extern	unsigned char   InfoCall[32][MAX_HO_PER_FLOOR];    		///Ãþ //¼¼´ë¼ö
extern	unsigned char   LocationCall[32][MAX_HO_PER_FLOOR];    	///Ãþ //¼¼´ë¼ö
extern	unsigned char   CloseCall[32][MAX_HO_PER_FLOOR];    	///Ãþ //¼¼´ë¼ö
extern	unsigned char   UpKeyTx[32][MAX_HO_PER_FLOOR];
extern	unsigned char   DnKeyTx[32][MAX_HO_PER_FLOOR];
extern	unsigned char   UpCommonCall[32][MAX_HO_PER_FLOOR];   /// Çö°ü 
extern	unsigned char   DnCommonCall[32][MAX_HO_PER_FLOOR];   /// Çö°ü 
extern	unsigned char   LocationCallTime[32][MAX_HO_PER_FLOOR];               // 01,02,03,04,05,06  È£    

/*
extern	unsigned char   UpRegistry[32][7];  				///Ãþ //¼¼´ë¼ö							        
extern	unsigned char   DnRegistry[32][7];  				///Ãþ //¼¼´ë¼ö
extern	unsigned char   UpCall[32][7];      				///Ãþ //¼¼´ë¼ö							        
extern	unsigned char   DnCall[32][7];      				///Ãþ //¼¼´ë¼ö
extern	unsigned char   InfoCall[32][7];    				///Ãþ //¼¼´ë¼ö
extern	unsigned char   UpKeyTx[32][7];
extern	unsigned char   DnKeyTx[32][7];
extern	unsigned char   LocationCallTime[32][7];            // 01,02,03,04,05,06  È£    
*/

extern	unsigned 	int  	StatusTxTime; 
extern	unsigned 	int		ElevatorServerAliveTime;
extern	unsigned 	char	ThisHost;



extern  unsigned    char    RcvBuf[MAX_SAVE_BUF];
extern	unsigned	char  	HostElevLive[MAX_ELEV];


extern	unsigned    char    HomeCallUpKey[4];extern	unsigned    char    HomeCallDnKey[4];extern	unsigned    char    HomeCallCarKey[4];




