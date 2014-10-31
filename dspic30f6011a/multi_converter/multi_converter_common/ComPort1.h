

extern	unsigned char   Com1RxBuffer[];
extern	unsigned char   Com1TxBuffer[];

extern	unsigned char   Com1RxStatus;
extern	unsigned char   Com1TxOn;
extern	unsigned char   Com1SerialTime;
extern	unsigned char   Com1Chksum;

extern	unsigned int    Com1AckTime;
extern	unsigned int    Com1RxCnt;
extern	unsigned int    Com1RxThisPt;
extern	unsigned int   Com1TxCnt;
extern	unsigned int   Com1TxThisPt;


extern  void __attribute__((section(".usercode")))   Com1Init(unsigned int baudrate_val);

