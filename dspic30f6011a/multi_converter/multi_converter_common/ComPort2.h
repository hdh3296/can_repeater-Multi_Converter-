



extern	unsigned char   Com2RxBuffer[];
extern	unsigned char   Com2TxBuffer[];

extern	unsigned char   Com2RxStatus;
extern	unsigned char   Com2TxOn;
extern	unsigned int  	Com2SerialTime;
extern	unsigned char   Com2Chksum;

extern	unsigned char   Com2RxCnt;
extern	unsigned char   Com2RxThisPt;
extern	unsigned char   Com2TxCnt;
extern	unsigned char   Com2TxThisPt;
extern  unsigned char   TXEN485Timer;


extern  unsigned int 	Com2Crc;


extern	void  __attribute__((section(".usercode")))   Com2Init(unsigned int baudrate_val);

