

			
#define    	I_AM_LISTENING_MODE	1

#define     CRT_COMPANY			'D'
#define     MAX_ELEV            14				// 수정 4001 동 36,4501동 14, 2101동 6,2201동 8   
#define     BASE_DONG			4501			// 수정



#define     BASE_FLOOR_B7		0			// 수정
#define     BASE_FLOOR_B6		1			// 수정
#define     BASE_FLOOR_B5		2			// 수정
#define     BASE_FLOOR_B4		3			// 수정
#define     BASE_FLOOR_B3		4			// 수정
#define     BASE_FLOOR_B2		5			// 수정		// 40001동, 4501-4503동
#define     BASE_FLOOR_B1		6			// 수정     //2101,2201,4504-4006
#define     BASE_FLOOR_01		7			// 수정

////////////////
//////BAUD_RATE_9600,BAUD_RATE_19200,BAUD_RATE_38400,BAUD_RATE_57600//////////
///////////////
#define		COM1_BAUD		BAUD_RATE_57600		
#define		COM2_BAUD		BAUD_RATE_57600	


#define		MAX_COM1_TXBUF     1000
#define     MAX_COM1_RXBUF     200
#define     MAX_COM2_TXBUF     10
#define     MAX_COM2_RXBUF     10

#define     CAN1_MAX_TX_BUF     100
#define     CAN1_MAX_RX_BUF     100
#define     CAN2_MAX_TX_BUF     10
#define     CAN2_MAX_RX_BUF     10




#define     HOST_DATA_RECOD     20      
#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV


			
#define		HOMECALL_BUTTON_SET_CMD		0x87
#define		INFO_TX_TIME				15   	//1.5sec
#define		MAX_ALIVE_TIME				120  	// 120sec



#define	MAX_HO_PER_FLOOR	9    						// 1 층당 최대 방 갯수 bit    101,102,103,104호 등등 
#define	MAX_SUB_FLOOR_CNT	(MAX_HO_PER_FLOOR-1) * 8    // 1 부터 6  까지  6*8 = 48호실/층당 


#ifndef	MY_STRUCTURE
#define	MY_STRUCTURE	1
typedef	struct x_tagReqHost{
        unsigned char	HostCmpanyId;
        unsigned int  	HostSid;
        unsigned long   HostEid;
        unsigned char   HostGroup;
        unsigned char   Ho_Host[10];
}tagReqHost;


#endif


extern volatile tagReqHost ReqHost[MAX_ELEV];



extern	unsigned char   UpRegistry[32][MAX_HO_PER_FLOOR];  		///층 //세대수							        
extern	unsigned char   DnRegistry[32][MAX_HO_PER_FLOOR];  		///층 //세대수
extern	unsigned char   UpCall[32][MAX_HO_PER_FLOOR];      		///층 //세대수							        
extern	unsigned char   DnCall[32][MAX_HO_PER_FLOOR];      		///층 //세대수
extern	unsigned char   InfoCall[32][MAX_HO_PER_FLOOR];    		///층 //세대수
extern	unsigned char   LocationCall[32][MAX_HO_PER_FLOOR];    	///층 //세대수
extern	unsigned char   CloseCall[32][MAX_HO_PER_FLOOR];    	///층 //세대수
extern	unsigned char   UpKeyTx[32][MAX_HO_PER_FLOOR];
extern	unsigned char   DnKeyTx[32][MAX_HO_PER_FLOOR];
extern	unsigned char   UpCommonCall[32][MAX_HO_PER_FLOOR];   /// 현관 
extern	unsigned char   DnCommonCall[32][MAX_HO_PER_FLOOR];   /// 현관 
extern	unsigned char   LocationCallTime[32][MAX_HO_PER_FLOOR];               // 01,02,03,04,05,06  호    

/*
extern	unsigned char   UpRegistry[32][7];  				///층 //세대수							        
extern	unsigned char   DnRegistry[32][7];  				///층 //세대수
extern	unsigned char   UpCall[32][7];      				///층 //세대수							        
extern	unsigned char   DnCall[32][7];      				///층 //세대수
extern	unsigned char   InfoCall[32][7];    				///층 //세대수
extern	unsigned char   UpKeyTx[32][7];
extern	unsigned char   DnKeyTx[32][7];
extern	unsigned char   LocationCallTime[32][7];            // 01,02,03,04,05,06  호    
*/

extern	unsigned 	int  	StatusTxTime; 
extern	unsigned 	int		ElevatorServerAliveTime;
extern	unsigned 	char	ThisHost;


extern  unsigned    char    RcvBuf[MAX_SAVE_BUF];
extern	unsigned	char  	HostElevLive[MAX_ELEV];


extern	unsigned    char    HomeCallUpKey[4];extern	unsigned    char    HomeCallDnKey[4];extern	unsigned    char    HomeCallCarKey[4];
extern	unsigned 	char	ThisHostReceive;




#define	ELEV_TMP_STATUS	15
#define	ELEV_CRT_KEY	16
