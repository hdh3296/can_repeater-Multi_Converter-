

#include  "ComPort1.h"
#include  "ComPort2.h"


//#define		OLD_CRT		1


#define		BAUD_RATE_9600		104
#define		BAUD_RATE_19200		52	
#define		BAUD_RATE_38400		26	
#define		BAUD_RATE_57600		16	


/*
#ifdef	OLD_CRT
	#define     HOST_DATA_RECOD     40      
	#define		BAUD_RATE_X	        BAUD_RATE_19200
#else
// ¹Ú±â»ï
//	#define     HOST_DATA_RECOD     88      
	#define     HOST_DATA_RECOD     40      
	#define		BAUD_RATE_X	        BAUD_RATE_38400
#endif
*/


#define     HOST_DATA_RECOD     40      
#define		BAUD_RATE_X	        BAUD_RATE_19200



	



#define		GROUP_NUMBER				0x0  

#define     MAX_SAVE_BUF        HOST_DATA_RECOD * MAX_ELEV



#define		HOMECALL_COMPANY			'D'
#define		HOMECALL_PC_ADDRESS			0xfe

#define		HOMECALL_MY_SET_ADDRESS		0xfe

#define		HOMECALL_WRITE_CMD			0x24
#define		HOMECALL_READ_CMD			0x23

#define		HOMECALL_BUTTON_SET_CMD		0x87
	

#define		DATA_LENGTH					HOST_DATA_RECOD     //88


extern	unsigned    int  	Crc;  
extern	void    Crc_Calulate(unsigned int crcdata);


extern	unsigned	char  	CommonBuf[MAX_RTX_BUF];     
//////////////extern  unsigned    char    EqualDataBuf[8];



	


//#define		XXXX		1

