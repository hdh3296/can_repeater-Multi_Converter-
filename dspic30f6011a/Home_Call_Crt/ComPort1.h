


#define         MAX_RTX_BUF     100

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


extern	unsigned int    Com1AckTime;

extern	unsigned char   Com1RxBuffer[MAX_RTX_BUF];
extern	unsigned char   Com1RxStatus;
extern	unsigned char   Com1TxOn;
extern	unsigned char   Com1SerialTime;
extern	unsigned char   Com1Chksum;

extern	unsigned char   Com1RxCnt;
extern	unsigned char   Com1RxThisPt;
extern	unsigned char   Com1TxCnt;
extern	unsigned char   Com1TxThisPt;


extern  void __attribute__((section(".usercode")))   Com1Init(void);

