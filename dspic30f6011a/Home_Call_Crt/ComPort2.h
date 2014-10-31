

#define        MAX_RTX_BUF     	    100

#define         EOT             0x04
#define         ETX             0x03
#define         ENQ             0x05
#define         ACK             0x06

#define         STX_CHK         0
#define         ETX_CHK         6
#define         RX_ERROR        7
#define         RX_GOOD         8
#define         TX_SET          9
#define         BCC1_CHK        13
#define         BCC2_CHK        14


extern	unsigned char   Com2RxBuffer[MAX_RTX_BUF];
extern	unsigned char   Com2RxStatus;
extern	unsigned char   Com2TxOn;
extern	unsigned int   Com2SerialTime;
extern	unsigned char   Com2Chksum;

extern	unsigned char   Com2RxCnt;
extern	unsigned char   Com2RxThisPt;
extern	unsigned char   Com2TxCnt;
extern	unsigned char   Com2TxThisPt;
extern  unsigned char   TXEN485Timer;


extern	unsigned char   TestBuffer[MAX_RTX_BUF];
extern	unsigned char   TestCnt;


extern	void  __attribute__((section(".usercode")))   Com2Init(void);

