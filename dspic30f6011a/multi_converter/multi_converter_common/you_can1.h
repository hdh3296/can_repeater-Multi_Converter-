

#include  "host_io.h"
#include  "workingram.h"




extern  void    __attribute__((section(".usercode"))) Can1Init(void);
extern  void    __attribute__((section(".usercode"))) Can1Check(void);
extern  void    __attribute__((section(".usercode"))) Can1Interrupt(void);


extern	unsigned    char    Can1TxBuf[];
extern	unsigned    char    Can1RxBuf[];



extern	unsigned    char    Can1RxLiveCnt;
extern	unsigned    char    Can1Tx1ConfirmCnt;
extern	unsigned    char    Can1Tx0ConfirmCnt;



extern  unsigned    int     Can1DataSeq;
extern	unsigned    int     Can1Block_Nm;


extern	unsigned    char    Can1DataFrameNm;
extern	unsigned    char	Can1RxDataTotalCnt;
extern	unsigned    char    Can1RxDataTxPointer;
extern	unsigned    char	Can1TxDataTotalCnt;
extern	unsigned    char    Can1TxDataTxPointer;
extern	unsigned	char	Can1PollingTimer;
extern	unsigned	int	Can1RxTimer;


extern	unsigned    char    Can1MySrcAddr;
extern	unsigned    char    Can1MyDestAddr;
extern	unsigned    char    Can1MyProductId;
extern	unsigned    char    Can1MyGroupAddr;
extern	unsigned    char    Can1MyLocalAddr;

extern	unsigned    char    Can1RxSrcAddr;
extern	unsigned    char    Can1RxDestAddr;
extern	unsigned    char    Can1RxProductId;
extern	unsigned    char    Can1RxGroupAddr;
extern	unsigned    char    Can1RxLocalAddr;

extern	unsigned	char	Can1RxCnt;
extern	unsigned	char	Can1RxThisPt;
extern	unsigned	char	Can1TxCnt;
extern	unsigned	char	Can1TxThisPt;

extern	unsigned    long    Can1RxEidFilter;
extern	unsigned    int     Can1RxSidFilter;
extern	unsigned    long    Can1RxEidMask;
extern	unsigned    int     Can1RxSidMask;
extern	unsigned    long    Can1TxEid;
extern	unsigned    int     Can1TxSid;
extern	unsigned    char    Can1TxDlc;
extern	unsigned    long    Can1RxEid;
extern	unsigned    int     Can1RxSid;
extern	unsigned    char    Can1RxDlc;

extern	unsigned 	int   	Can1TxDataCnt;
extern	unsigned 	int   	Can1DataCnt;


extern	unsigned 	int   	Can1ThisRxBuf[8];
extern	unsigned 	int   	Can1ThisTxBuf[8];

extern	unsigned    long    BefCan1RxEid;
extern	unsigned    int     BefCan1RxSid;

extern	unsigned 	int   	Can1xxTimer;

extern	 unsigned    int     CAN1bit;

#define  bCan1Data_Continue		GET_BITFIELD(&CAN1bit).bit0 
#define  bCan1Crc_Include    	GET_BITFIELD(&CAN1bit).bit1 
#define  bCan1TxAct    			GET_BITFIELD(&CAN1bit).bit2 
#define  bCan1RxAll    			GET_BITFIELD(&CAN1bit).bit3 
#define  bCan1TxReady    		GET_BITFIELD(&CAN1bit).bit4 
#define  bCan1RxGood    		GET_BITFIELD(&CAN1bit).bit5 
#define  bCan1CmdAct    		GET_BITFIELD(&CAN1bit).bit6 
#define  bCan1TxStart    		GET_BITFIELD(&CAN1bit).bit7 









