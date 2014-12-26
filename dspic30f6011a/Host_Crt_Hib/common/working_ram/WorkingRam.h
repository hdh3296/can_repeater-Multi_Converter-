



/*
struct USER_WORKING_RAM{
	unsigned	int  	I_sRamDArry[10];     
	unsigned	char  	C_sRamDArry[10];     

	unsigned	char  	C_Buffer[100];     

//	unsigned	int		msec10;
//	unsigned    int		msec100;
	unsigned    int 	Crc;  
	unsigned    char	updn;
	unsigned    int		WaitTime;
		
//	unsigned	char  	AirconBuf[MAX_AIRCON_BUF];     
//	unsigned	char  	sRamDArry[END_SRAM];     
	unsigned    char    MyAddress;
//	unsigned    char    MyGroupAddr;
//	unsigned    char    MyLocalAddr;
//	unsigned    char    MyProductId;		
	unsigned    char    Can1_Hib_Retun;
	unsigned    char    Can2_Hib_Retun;
	unsigned    char    Can1AirconSrcAddr;
	unsigned    char    Can1AirconLocalAddr;
	unsigned    char    Can2AirconSrcAddr;
	unsigned    char    Can2AirconLocalAddr;
	unsigned    char    huntingtime;
	unsigned    char    MainTimer;
	unsigned    char    YouHostAckTimer;
	unsigned    char    HostReqAddr;
};


extern	struct USER_WORKING_RAM	UWR;



#define msec10	UWR.I_sRamDArry[0]
#define msec100	UWR.I_sRamDArry[1]


#define z_Buffer	&(UWR.C_Buffer)	

//#define	msec10	UWR.I_sRamDArry[0];	
//msec100	UWR.I_sRamDArry[2];	
*/


/*
const struct UART_PARAMETER{	
	unsigned    char    MyProductId;
	unsigned    char    MyGroupAddr;
	unsigned    char    MyLocalAddr;
	unsigned	char  	Frame;
	unsigned	int  	BaudRate;
}__UART_PARAMETER;


const struct CAN_PARAMETER{	
	unsigned    char    MyGroupAddr;
	unsigned    char    MyLocalAddr;
	unsigned    char    MyProductId;
	unsigned    char    MyAddr;
	unsigned    char    MyTargetAddr;

	unsigned    char    cUnKnown0;
	unsigned    char    cUnKnown1;
	unsigned    char    cUnKnown2;
	unsigned    char    cUnKnown3;
	unsigned    char    cUnKnown4;
	unsigned    char    cUnKnown5;
	unsigned    char    cUnKnown6;
	unsigned    char    cUnKnown7;
	unsigned    char    cUnKnown8;
	unsigned    char    cUnKnown9;

	unsigned	int  	RxSid;
	unsigned	int  	TxSid;
	unsigned	int  	RxSidMask;
	unsigned	int  	RxSidFilter;
	unsigned	int  	baudrate;


	unsigned    int		iUnKnown0;
	unsigned    int		iUnKnown1;
	unsigned    int		iUnKnown2;
	unsigned    int		iUnKnown3;
	unsigned    int		iUnKnown4;
	unsigned    int		iUnKnown5;
	unsigned    int		iUnKnown6;
	unsigned    int		iUnKnown7;
	unsigned    int		iUnKnown8;
	unsigned    int		iUnKnown9;

	unsigned	long  	RxEid;
	unsigned	long  	TxEid;
	unsigned	long  	RxEidMask;
	unsigned	long  	RxEidFilter;

	unsigned    long	lUnKnown0;
	unsigned    long	lUnKnown1;
	unsigned    long	lUnKnown2;
	unsigned    long	lUnKnown3;
	unsigned    long	lUnKnown4;
	unsigned    long	lUnKnown5;
	unsigned    long	lUnKnown6;
	unsigned    long	lUnKnown7;
	unsigned    long	lUnKnown8;
	unsigned    long	lUnKnown9;

}__CAN_PARAMETER;



const struct USER_CONFIG_VALUE{	
	struct CAN_PARAMETER	Can1;
	struct CAN_PARAMETER	Can2;
	struct UART_PARAMETER	Com1;	
}__USER_CONFIG_VALUE;



extern	const struct USER_CONFIG_VALUE	UCV;


#define cCan1MyGroupAddrValue	UCV.Can1.MyGroupAddr
#define cCan1MyLocalAddrValue	UCV.Can1.MyLocalAddr
#define cCan1MyProductIdValue	UCV.Can1.MyProductId
#define iCan1RxSidMask			UCV.Can1.RxSidMask
#define iCan1RxSidFilter		UCV.Can1.RxSidFilter
#define lCan1RxEidMask			UCV.Can1.RxEidMask
#define lCan1RxEidFilter		UCV.Can1.RxEidFilter


#define cCan2MyGroupAddrValue	UCV.Can2.MyGroupAddr
#define cCan2MyLocalAddrValue	UCV.Can2.MyLocalAddr
#define cCan2MyProductIdValue	UCV.Can2.MyProductId
#define iCan2RxSidMask			UCV.Can2.RxSidMask
#define iCan2RxSidFilter		UCV.Can2.RxSidFilter
#define lCan2RxEidMask			UCV.Can2.RxEidMask
#define lCan2RxEidFilter		UCV.Can2.RxEidFilter
*/





#define	FLASH_WRITE_BLOCK	16			//(16 *4)   == 64byte
#define	USER_FLASH_BYTE		128		// 8block   == 64 * 8 == 512 byte



#ifndef	BIT_STR
#define	BIT_STR	1
typedef struct _bit_struct
{
    unsigned bit0  : 1;
    unsigned bit1  : 1;
    unsigned bit2  : 1;
    unsigned bit3  : 1;
    unsigned bit4  : 1;
    unsigned bit5  : 1;
    unsigned bit6  : 1;
    unsigned bit7  : 1;
    unsigned bit8  : 1;
}bit_field;



typedef struct _bit_struct_32
{
    unsigned bit0  : 1;
    unsigned bit1  : 1;
    unsigned bit2  : 1;
    unsigned bit3  : 1;
    unsigned bit4  : 1;
    unsigned bit5  : 1;
    unsigned bit6  : 1;
    unsigned bit7  : 1;
    unsigned bit8  : 1;
    unsigned bit9  : 1;
    unsigned bit10  : 1;
    unsigned bit11  : 1;
    unsigned bit12  : 1;
    unsigned bit13  : 1;
    unsigned bit14  : 1;
    unsigned bit15  : 1;
    unsigned bit16  : 1;
    unsigned bit17  : 1;
    unsigned bit18  : 1;
    unsigned bit19  : 1;
    unsigned bit20  : 1;
    unsigned bit21  : 1;
    unsigned bit22  : 1;
    unsigned bit23  : 1;
    unsigned bit24  : 1;
    unsigned bit25  : 1;
    unsigned bit26  : 1;
    unsigned bit27  : 1;
    unsigned bit28  : 1;
    unsigned bit29  : 1;
    unsigned bit30  : 1;
    unsigned bit31  : 1;
}bit_field32;



typedef  union  _long_union
{
	bit_field32		bitx;
    unsigned char 	byte[4];
    unsigned int  	intger[2];
    unsigned long 	long_data; 
}long_field;


#endif


#define GET_BITFIELD(addr)              (*((volatile bit_field*) (addr)))
#define GET_LONGFIELD(addr)             (*((volatile long_field*) (addr)))




extern long_field  UserPgmConfigBu[FLASH_WRITE_BLOCK]; 
extern long_field  UserPgmConfig[USER_FLASH_BYTE]    	__attribute__((aligned(USER_FLASH_BYTE) , section(".auto_psv,r")));





//#define  c_LdTmpBufRam(adr)         GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).byte[(adr % 4)]
//#define  i_LdTmpBufRam(adr)         GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/2]).intger[adr%2]
//#define  l_LdTmpBufRam(adr)         GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)]).long_data
//#define  bit_LdTmpBufRam(adr,bitnm) bitnm==0 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit0 : bitnm==1 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit1 : bitnm == 2 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit2 : bitnm==3 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit3 : bitnm==4 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit4 : bitnm==5 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit5 : bitnm==6 ? GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit6 : GET_BITFIELD(&UserPgmConfigBuf[(adr % 64)/8]).bit7 

/*
#define  bit_LdTmpBufRam7(adr,bitnm)  bitnm==0  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit0  : bitnm==1  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit1  : bitnm == 2  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit2  : bitnm==3  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit3  : bitnm==4  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit4  : bitnm==5  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit5  : bitnm==6  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit6  : GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit7 
#define  bit_LdTmpBufRam15(adr,bitnm) bitnm==8  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit8  : bitnm==9  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit9  : bitnm == 10 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit10 : bitnm==11 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit11 : bitnm==12 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit12 : bitnm==13 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit13 : bitnm==14 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit14 : GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit15 
#define  bit_LdTmpBufRam23(adr,bitnm) bitnm==16 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit16 : bitnm==17 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit17 : bitnm == 18 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit18 : bitnm==19 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit19 : bitnm==20 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit20 : bitnm==21 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit21 : bitnm==22 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit22 : GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit23 
#define  bit_LdTmpBufRam31(adr,bitnm) bitnm==24 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit24 : bitnm==25 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit25 : bitnm == 26 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit26 : bitnm==27 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit27 : bitnm==28 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit28 : bitnm==29 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit29 : bitnm==30 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit30 : GET_LONGFIELD(&UserPgmConfigBuf[(adr % 64)/4]).bitx.bit31 

#define  bit_LdTmpBufRam(adr,bitnm) bitnm <= 7 ? bit_LdTmpBufRam7(adr,bitnm) : bitnm <= 15 ? bit_LdTmpBufRam15(adr,bitnm) : bitnm <= 23 ? bit_LdTmpBufRam23(adr,bitnm) : bit_LdTmpBufRam31(adr,bitnm)  
*/




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  Set_Long_Flash_Buf(adr)         GET_LONGFIELD(&UserPgmConfigBuf[((adr+0)/4) % 16])		.long_data
#define  Set_Integer_Flash_Buf(adr)      GET_LONGFIELD(&UserPgmConfigBuf[((adr+0)/4) % 16]) 	.intger[(adr/2)%2]
#define  Set_Byte_Flash_Buf(adr)         GET_LONGFIELD(&UserPgmConfigBuf[((adr+0)/4) % 16])		.byte[(adr % 4)]


#define  Set_Bit_Flash_Buf_07(adr,bitnm) bitnm==0  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit0  : bitnm==1  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit1  : bitnm == 2  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit2  : bitnm==3  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit3  : bitnm==4  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit4  : bitnm==5  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit5  : bitnm==6  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit6  : GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit7 
#define  Set_Bit_Flash_Buf_15(adr,bitnm) bitnm==8  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit8  : bitnm==9  ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit9  : bitnm == 10 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit10 : bitnm==11 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit11 : bitnm==12 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit12 : bitnm==13 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit13 : bitnm==14 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit14 : GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit15 
#define  Set_Bit_Flash_Buf_23(adr,bitnm) bitnm==16 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit16 : bitnm==17 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit17 : bitnm == 18 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit18 : bitnm==19 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit19 : bitnm==20 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit20 : bitnm==21 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit21 : bitnm==22 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit22 : GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit23 
#define  Set_Bit_Flash_Buf_31(adr,bitnm) bitnm==24 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit24 : bitnm==25 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit25 : bitnm == 26 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit26 : bitnm==27 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit27 : bitnm==28 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit28 : bitnm==29 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit29 : bitnm==30 ? GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit30 : GET_LONGFIELD(&UserPgmConfigBuf[(adr/4) % 16]).bitx.bit31 

#define  Set_Bit_Flash_Buf(adr,bitnm)	 bitnm <= 7 ? Set_Bit_Flash_Buf_07(adr,bitnm) : bitnm <= 15 ? Set_Bit_Flash_Buf_15(adr,bitnm) : bitnm <= 23 ? Set_Bit_Flash_Buf_23(adr,bitnm) : Set_Bit_Flash_Buf_31(adr,bitnm)  
#define  Set_BitNm_Flash_Buf(bitnm) 	(bitnm % 32) <= 7 ?  Set_Bit_Flash_Buf_07((bitnm / 32),(bitnm % 32)) : (bitnm % 32) <= 15 ? Set_Bit_Flash_Buf_15((bitnm / 32),(bitnm % 32)) : (bitnm % 32) <= 23 ? Set_Bit_Flash_Buf_23((bitnm / 32),(bitnm % 32)) : Set_Bit_Flash_Buf_31((bitnm / 32),(bitnm % 32))  

		


#define  Get_Long_Flash(adr)		GET_LONGFIELD(&UserPgmConfig[((adr+0)/4)])	.long_data
#define  Get_Integer_Flash(adr)		GET_LONGFIELD(&UserPgmConfig[((adr+0)/4)])	.intger[((adr/2)%2)]
#define  Get_Byte_Flash(adr)		GET_LONGFIELD(&UserPgmConfig[((adr+0)/4)])	.byte[((adr+0)%4)]


#define  Get_Bit_Flash07(adr,bitnm) bitnm==0  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit0  : bitnm==1  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit1  : bitnm == 2  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit2  : bitnm==3  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit3  : bitnm==4  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit4  : bitnm==5  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit5  : bitnm==6  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit6  : GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit7 
#define  Get_Bit_Flash15(adr,bitnm) bitnm==8  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit8  : bitnm==9  ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit9  : bitnm == 10 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit10 : bitnm==11 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit11 : bitnm==12 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit12 : bitnm==13 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit13 : bitnm==14 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit14 : GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit15 
#define  Get_Bit_Flash23(adr,bitnm) bitnm==16 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit16 : bitnm==17 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit17 : bitnm == 18 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit18 : bitnm==19 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit19 : bitnm==20 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit20 : bitnm==21 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit21 : bitnm==22 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit22 : GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit23 
#define  Get_Bit_Flash31(adr,bitnm) bitnm==24 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit24 : bitnm==25 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit25 : bitnm == 26 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit26 : bitnm==27 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit27 : bitnm==28 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit28 : bitnm==29 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit29 : bitnm==30 ? GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit30 : GET_LONGFIELD(&UserPgmConfig[((adr+0) / 4)]).bitx.bit31 

#define  Get_Bit_Flash(adr,bitnm) 	bitnm <= 7 ?  Get_Bit_Flash07(adr,bitnm) : bitnm <= 15 ? Get_Bit_Flash15(adr,bitnm) : bitnm <= 23 ? Get_Bit_Flash23(adr,bitnm) : Get_Bit_Flash31(adr,bitnm)  

#define  Get_BitNm_Flash(bitnm) 	(bitnm % 32) <= 7 ?  Get_Bit_Flash07((bitnm / 32),(bitnm % 32)) : (bitnm % 32) <= 15 ? Get_Bit_Flash15((bitnm / 32),(bitnm % 32)) : (bitnm % 32) <= 23 ? Get_Bit_Flash23((bitnm / 32),(bitnm % 32)) : Get_Bit_Flash31((bitnm / 32),(bitnm % 32))  



/*
/////////////////////////block0////////////////////////////////////
//bit group
#define  	F_BLOCK_B               0	
																										
#define  	bit_X0_A             			F_BLOCK_B+0             //1     16
#define  	bit_X1             				F_BLOCK_B+1             //1     16
#define  	bit_X2             				F_BLOCK_B+2             //1     16
#define  	bit_X3             				F_BLOCK_B+3             //1     16
#define  	bit_X4_A           				F_BLOCK_B+4             //1     16
#define  	bit_X5             				F_BLOCK_B+5             //1     16
#define  	bit_X6             				F_BLOCK_B+6             //1     16
#define  	b_X7             				F_BLOCK_B+7             //1     16
#define  	b_X8             				F_BLOCK_B+8             //1     16
#define  	b_X9             				F_BLOCK_B+9             //1     16
#define  	b_X10            				F_BLOCK_B+10             //1     16
#define  	b_X11            				F_BLOCK_B+11             //1     16
#define  	b_X12            				F_BLOCK_B+12             //1     16
#define  	b_X13            				F_BLOCK_B+13             //1     16
#define  	b_X14            				F_BLOCK_B+14             //1     16
#define  	b_X15            				F_BLOCK_B+15             //1     16
#define  	b_X16            				F_BLOCK_B+16             //1     16
#define  	b_X17            				F_BLOCK_B+17             //1     16
#define  	b_X18            				F_BLOCK_B+18             //1     16
#define  	b_X19            				F_BLOCK_B+19             //1     16
#define  	b_X20            				F_BLOCK_B+20             //1     16
#define  	b_X21            				F_BLOCK_B+21             //1     16
#define  	b_X22            				F_BLOCK_B+22             //1     16
#define  	b_X23            				F_BLOCK_B+23             //1     16
#define  	b_X24            				F_BLOCK_B+24             //1     16
#define  	b_X25            				F_BLOCK_B+25             //1     16
#define  	b_X26            				F_BLOCK_B+26             //1     16
#define  	b_X27            				F_BLOCK_B+27             //1     16
#define  	b_X28            				F_BLOCK_B+28             //1     16
#define  	b_X29            				F_BLOCK_B+29             //1     16
#define  	b_X30            				F_BLOCK_B+30             //1     16
#define  	b_X31            				F_BLOCK_B+31             //1     16
#define  	b_X32             				F_BLOCK_B+32             //1     16
#define  	b_X33             				F_BLOCK_B+33             //1     16
#define  	b_X34             				F_BLOCK_B+34             //1     16
#define  	b_X35             				F_BLOCK_B+35             //1     16
#define  	b_X36             				F_BLOCK_B+36             //1     16
#define  	b_X37             				F_BLOCK_B+37             //1     16
#define  	b_X38             				F_BLOCK_B+38             //1     16
#define  	b_X39             				F_BLOCK_B+39             //1     16
#define  	b_X40             				F_BLOCK_B+40             //1     16
#define  	b_X41             				F_BLOCK_B+41             //1     16
#define  	b_X42            				F_BLOCK_B+42             //1     16
#define  	b_X43            				F_BLOCK_B+43             //1     16
#define  	b_X44            				F_BLOCK_B+44             //1     16
#define  	b_X45            				F_BLOCK_B+45             //1     16
#define  	b_X46            				F_BLOCK_B+46             //1     16
#define  	b_X47            				F_BLOCK_B+47             //1     16
#define  	b_X48            				F_BLOCK_B+48             //1     16
#define  	b_X49            				F_BLOCK_B+49             //1     16
#define  	b_X50            				F_BLOCK_B+50             //1     16
#define  	b_X51            				F_BLOCK_B+51             //1     16
#define  	b_X52            				F_BLOCK_B+52             //1     16
#define  	b_X53            				F_BLOCK_B+53             //1     16
#define  	b_X54            				F_BLOCK_B+54             //1     16
#define  	b_X55            				F_BLOCK_B+55             //1     16
#define  	b_X56            				F_BLOCK_B+56             //1     16
#define  	b_X57            				F_BLOCK_B+57             //1     16
#define  	b_X58            				F_BLOCK_B+58             //1     16
#define  	b_X59            				F_BLOCK_B+59             //1     16
#define  	b_X60            				F_BLOCK_B+60             //1     16
#define  	b_X61            				F_BLOCK_B+61             //1     16
#define  	b_X62            				F_BLOCK_B+62             //1     16
#define  	b_X63            				F_BLOCK_B+63             //1     16

*/
 



#define  	BLK0             				0

#define  	cF_0000_A             		BLK0+0          	//1     16
#define  	cF_0001_A					BLK0+1             	//1     16
#define  	cF_0002_A					BLK0+2             	//1     16
#define  	cF_0003_A             		BLK0+3             	//1     16
#define  	cF_0004_A					BLK0+4             	//1     17
#define  	cF_0005_A					BLK0+5             	//1     17
#define  	cF_0006_A             		BLK0+6             	//1     17
#define  	cF_0007_A					BLK0+7             	//1     17
#define  	cF_0008_A					BLK0+8             	//1     16
#define  	cF_0009_A             		BLK0+9             	//1     16
#define  	cF_0010_A					BLK0+10             //1     16
#define  	cF_0011_A					BLK0+11             //1     16
#define  	cF_0012_A             		BLK0+12             //1     16
#define  	cF_0013_A					BLK0+13             //1     16
#define  	cF_0014_A					BLK0+14            	//1     16
#define  	cF_0015_A             		BLK0+15            	//1     16
#define  	cF_0016_A					BLK0+16            	//1     16
#define  	cF_0017_A					BLK0+17            	//1     16
#define  	cF_0018_A             		BLK0+18             //1     16
#define  	cF_0019_A					BLK0+19             //1     16
#define  	cF_0020_A					BLK0+20             //1     16
#define  	cF_0021_A             		BLK0+21             //1     16
#define  	cF_0022_A					BLK0+22            	//1     16
#define  	cF_0023_A					BLK0+23            	//1     16
#define  	cF_0024_A             		BLK0+24            	//1     16
#define  	cF_0025_A					BLK0+25            	//1     16
#define  	cF_0026_A					BLK0+26            	//1     16
#define  	cF_0027_A             		BLK0+27            	//1     16
#define  	cF_0028_A					BLK0+28             //1     16
#define  	cF_0029_A					BLK0+29             //1     16
#define  	cF_0030_A             		BLK0+30            	//1     16
#define  	cF_0031_A					BLK0+31            	//1     16
#define  	cF_0032_A					BLK0+32            	//1     16
#define  	cF_0033_A             		BLK0+33            	//1     16
#define  	cF_0034_A					BLK0+34            	//1     16
#define  	cF_0035_A					BLK0+35            	//1     16
#define  	cF_0036_A             		BLK0+36            	//1     16
#define  	cF_0037_A					BLK0+37            	//1     16
#define  	cF_0038_A					BLK0+38            	//1     16
#define  	cF_0039_A             		BLK0+39            	//1     16
#define  	cF_0040_A					BLK0+40            	//1     16
#define  	cF_0041_A					BLK0+41             //1     16
#define  	cF_0042_A             		BLK0+42             //1     16
#define  	cF_0043_A					BLK0+43            	//1     16
#define  	cF_0044_A					BLK0+44             //1     16
#define  	cF_0045_A             		BLK0+45             //1     16
#define  	cF_0046_A					BLK0+46             //1     16
#define  	cF_0047_A					BLK0+47            	//1     16
#define  	cF_0048_A             		BLK0+48            	//1     16
#define  	cF_0049_A					BLK0+49             //1     16
#define  	cF_0050_A					BLK0+50             //1     16
#define  	cF_0051_A             		BLK0+51             //1     16
#define  	cF_0052_A					BLK0+52            	//1     16
#define  	cF_0053_A					BLK0+53             //1     16
#define  	cF_0054_A             		BLK0+54             //1     16
#define  	cF_0055_A					BLK0+55            	//1     16
#define  	cF_0056_A					BLK0+56             //1     16
#define  	cF_0057_A             		BLK0+57            	//1     16
#define  	cF_0058_A					BLK0+58            	//1     16
#define  	cF_0059_A					BLK0+59            	//1     16
#define  	cF_0060_A             		BLK0+60            	//1     16
#define  	cF_0061_A					BLK0+61            	//1     16
#define  	cF_0062_A					BLK0+62            	//1     16
#define  	cF_0063_A					BLK0+63            	//1     16

#define  	cF_0064_A             		BLK0+64          	//1     16
#define  	cF_0065_A					BLK0+65             //1     16
#define  	cF_0066_A					BLK0+66             //1     16
#define  	cF_0067_A             		BLK0+67             //1     16
#define  	cF_0068_A					BLK0+68             //1     16
#define  	cF_0069_A					BLK0+69             //1     16
#define  	cF_0070_A             		BLK0+70             //1     16
#define  	cF_0071_A					BLK0+71             //1     16
#define  	cF_0072_A					BLK0+72             //1     16
#define  	cF_0073_A             		BLK0+73             //1     16
#define  	cF_0074_A					BLK0+74             //1     16
#define  	cF_0075_A					BLK0+75             //1     16
#define  	cF_0076_A             		BLK0+76             //1     16
#define  	cF_0077_A					BLK0+77             //1     16
#define  	cF_0078_A					BLK0+78            	//1     16
#define  	cF_0079_A             		BLK0+79            	//1     16
#define  	cF_0080_A					BLK0+80            	//1     16
#define  	cF_0081_A					BLK0+81            	//1     16
#define  	cF_0082_A             		BLK0+82             //1     16
#define  	cF_0083_A					BLK0+83             //1     16
#define  	cF_0084_A					BLK0+84             //1     16
#define  	cF_0085_A             		BLK0+85             //1     16
#define  	cF_0086_A					BLK0+86            	//1     16
#define  	cF_0087_A					BLK0+87            	//1     16
#define  	cF_0088_A             		BLK0+88            	//1     16
#define  	cF_0089_A					BLK0+89            	//1     16
#define  	cF_0090_A					BLK0+90            	//1     16
#define  	cF_0091_A             		BLK0+91            	//1     16
#define  	cF_0092_A					BLK0+92             //1     16
#define  	cF_0093_A					BLK0+93             //1     16
#define  	cF_0094_A             		BLK0+94            	//1     16
#define  	cF_0095_A					BLK0+95            	//1     16
#define  	cF_0096_A					BLK0+96            	//1     16
#define  	cF_0097_A             		BLK0+97            	//1     16
#define  	cF_0098_A					BLK0+98            	//1     16
#define  	cF_0099_A					BLK0+99            	//1     16
#define  	cF_0100_A             		BLK0+100            //1     16
#define  	cF_0101_A					BLK0+101            //1     16
#define  	cF_0102_A					BLK0+102            //1     16
#define  	cF_0103_A             		BLK0+103            //1     16
#define  	cF_0104_A					BLK0+104            //1     16
#define  	cF_0105_A					BLK0+105            //1     16
#define  	cF_0106_A             		BLK0+106            //1     16
#define  	cF_0107_A					BLK0+107            //1     16
#define  	cF_0108_A					BLK0+108            //1     16
#define  	cF_0109_A             		BLK0+109            //1     16
#define  	cF_0110_A					BLK0+110            //1     16
#define  	cF_0111_A					BLK0+111            //1     16
#define  	cF_0112_A             		BLK0+112            //1     16
#define  	cF_0113_A					BLK0+113            //1     16
#define  	cF_0114_A					BLK0+114            //1     16
#define  	cF_0115_A             		BLK0+115            //1     16
#define  	cF_0116_A					BLK0+116            //1     16
#define  	cF_0117_A					BLK0+117            //1     16
#define  	cF_0118_A             		BLK0+118            //1     16
#define  	cF_0119_A					BLK0+119            //1     16
#define  	cF_0120_A					BLK0+120            //1     16
#define  	cF_0121_A             		BLK0+121            //1     16
#define  	cF_0122_A					BLK0+122            //1     16
#define  	cF_0123_A					BLK0+123            //1     16
#define  	cF_0124_A             		BLK0+124            //1     16
#define  	cF_0125_A					BLK0+125            //1     16
#define  	cF_0126_A					BLK0+126            //1     16
#define  	cF_0127_A					BLK0+127            //1     16

#define  	cF_0128_A             		BLK0+128     		//1     16
#define  	cF_0129_A					BLK0+129            //1     16
#define  	cF_0130_A					BLK0+130            //1     16
#define  	cF_0131_A             		BLK0+131            //1     16
#define  	cF_0132_A					BLK0+132            //1     16
#define  	cF_0133_A					BLK0+133            //1     16
#define  	cF_0134_A             		BLK0+134            //1     16
#define  	cF_0135_A					BLK0+135            //1     16
#define  	cF_0136_A					BLK0+136            //1     16
#define  	cF_0137_A             		BLK0+137            //1     16
#define  	cF_0138_A					BLK0+138            //1     16
#define  	cF_0139_A					BLK0+139            //1     16
#define  	cF_0140_A             		BLK0+140            //1     16
#define  	cF_0141_A					BLK0+141            //1     16
#define  	cF_0142_A					BLK0+142            //1     16
#define  	cF_0143_A             		BLK0+143            //1     16
#define  	cF_0144_A					BLK0+144            //1     16
#define  	cF_0145_A					BLK0+145            //1     16
#define  	cF_0146_A             		BLK0+146            //1     16
#define  	cF_0147_A					BLK0+147            //1     16
#define  	cF_0148_A					BLK0+148            //1     16
#define  	cF_0149_A             		BLK0+149            //1     16
#define  	cF_0150_A					BLK0+150            //1     16
#define  	cF_0151_A					BLK0+151            //1     16
#define  	cF_0152_A             		BLK0+152            //1     16
#define  	cF_0153_A					BLK0+153            //1     16
#define  	cF_0154_A					BLK0+154            //1     16
#define  	cF_0155_A             		BLK0+155            //1     16
#define  	cF_0156_A					BLK0+156            //1     16
#define  	cF_0157_A					BLK0+157            //1     16
#define  	cF_0158_A             		BLK0+158            //1     16
#define  	cF_0159_A					BLK0+159            //1     16
#define  	cF_0160_A					BLK0+160            //1     16
#define  	cF_0161_A             		BLK0+161            //1     16
#define  	cF_0162_A					BLK0+162            //1     16
#define  	cF_0163_A					BLK0+163            //1     16
#define  	cF_0164_A             		BLK0+164            //1     16
#define  	cF_0165_A					BLK0+165            //1     16
#define  	cF_0166_A					BLK0+166            //1     16
#define  	cF_0167_A             		BLK0+167            //1     16
#define  	cF_0168_A					BLK0+168            //1     16
#define  	cF_0169_A					BLK0+169            //1     16
#define  	cF_0170_A             		BLK0+170            //1     16
#define  	cF_0171_A					BLK0+171            //1     16
#define  	cF_0172_A					BLK0+172            //1     16
#define  	cF_0173_A             		BLK0+173            //1     16
#define  	cF_0174_A					BLK0+174            //1     16
#define  	cF_0175_A					BLK0+175            //1     16
#define  	cF_0176_A             		BLK0+176            //1     16
#define  	cF_0177_A					BLK0+177            //1     16
#define  	cF_0178_A					BLK0+178            //1     16
#define  	cF_0179_A             		BLK0+179            //1     16
#define  	cF_0180_A					BLK0+180            //1     16
#define  	cF_0181_A					BLK0+181            //1     16
#define  	cF_0182_A             		BLK0+182            //1     16
#define  	cF_0183_A					BLK0+183            //1     16
#define  	cF_0184_A					BLK0+184            //1     16
#define  	cF_0185_A             		BLK0+185            //1     16
#define  	cF_0186_A					BLK0+186            //1     16
#define  	cF_0187_A					BLK0+87            	//1     16
#define  	cF_0188_A             		BLK0+188            //1     16
#define  	cF_0189_A					BLK0+189            //1     16
#define  	cF_0190_A					BLK0+190            //1     16
#define  	cF_0191_A					BLK0+191            //1     16

#define  	cF_0192_A             		BLK0+192          	//1     16
#define  	cF_0193_A					BLK0+193            //1     16
#define  	cF_0194_A					BLK0+194            //1     16
#define  	cF_0195_A             		BLK0+195            //1     16
#define  	cF_0196_A					BLK0+196            //1     16
#define  	cF_0197_A					BLK0+197            //1     16
#define  	cF_0198_A             		BLK0+198            //1     16
#define  	cF_0199_A					BLK0+199            //1     16
#define  	cF_0200_A					BLK0+200            //1     16
#define  	cF_0201_A             		BLK0+201            //1     16
#define  	cF_0202_A					BLK0+202            //1     16
#define  	cF_0203_A					BLK0+203            //1     16
#define  	cF_0204_A             		BLK0+204            //1     16
#define  	cF_0205_A					BLK0+205            //1     16
#define  	cF_0206_A					BLK0+206            //1     16
#define  	cF_0207_A             		BLK0+207            //1     16
#define  	cF_0208_A					BLK0+208            //1     16
#define  	cF_0209_A					BLK0+209            //1     16
#define  	cF_0210_A             		BLK0+210            //1     16
#define  	cF_0211_A					BLK0+211            //1     16
#define  	cF_0212_A					BLK0+212            //1     16
#define  	cF_0213_A             		BLK0+213            //1     16
#define  	cF_0214_A					BLK0+214            //1     16
#define  	cF_0215_A					BLK0+215            //1     16
#define  	cF_0216_A             		BLK0+216            //1     16
#define  	cF_0217_A					BLK0+217            //1     16
#define  	cF_0218_A					BLK0+218            //1     16
#define  	cF_0219_A             		BLK0+219            //1     16
#define  	cF_0220_A					BLK0+220            //1     16
#define  	cF_0221_A					BLK0+221            //1     16
#define  	cF_0222_A             		BLK0+222            //1     16
#define  	cF_0223_A					BLK0+223            //1     16
#define  	cF_0224_A					BLK0+224            //1     16
#define  	cF_0225_A             		BLK0+225            //1     16
#define  	cF_0226_A					BLK0+226            //1     16
#define  	cF_0227_A					BLK0+227            //1     16
#define  	cF_0228_A             		BLK0+228            //1     16
#define  	cF_0229_A					BLK0+229            //1     16
#define  	cF_0230_A					BLK0+230            //1     16
#define  	cF_0231_A             		BLK0+231            //1     16
#define  	cF_0232_A					BLK0+232            //1     16
#define  	cF_0233_A					BLK0+233            //1     16
#define  	cF_0234_A             		BLK0+234            //1     16
#define  	cF_0235_A					BLK0+235            //1     16
#define  	cF_0236_A					BLK0+236            //1     16
#define  	cF_0237_A             		BLK0+237            //1     16
#define  	cF_0238_A					BLK0+238            //1     16
#define  	cF_0239_A					BLK0+239            //1     16
#define  	cF_0240_A             		BLK0+240            //1     16
#define  	cF_0241_A					BLK0+241            //1     16
#define  	cF_0242_A					BLK0+242            //1     16
#define  	cF_0243_A             		BLK0+243            //1     16
#define  	cF_0244_A					BLK0+244            //1     16
#define  	cF_0245_A					BLK0+245            //1     16
#define  	cF_0246_A             		BLK0+246            //1     16
#define  	cF_0247_A					BLK0+247            //1     16
#define  	cF_0248_A					BLK0+248            //1     16
#define  	cF_0249_A             		BLK0+249            //1     16
#define  	cF_0250_A					BLK0+250            //1     16
#define  	cF_0251_A					BLK0+251            //1     16
#define  	cF_0252_A             		BLK0+252            //1     16
#define  	cF_0253_A					BLK0+253            //1     16
#define  	cF_0254_A					BLK0+254            //1     16
#define  	cF_0255_A					BLK0+255            //1     16



#define  	iF_0000_A					BLK0+0            	//1     16
#define  	iF_0002_A             		BLK0+2            	//1     16
#define  	iF_0004_A					BLK0+4            	//1     16
#define  	iF_0006_A					BLK0+6            	//1     16
#define  	iF_0008_A             		BLK0+8            	//1     16
#define  	iF_0010_A					BLK0+10           	//1     16
#define  	iF_0012_A					BLK0+12            	//1     16
#define  	iF_0014_A             		BLK0+14           	//1     16
#define  	iF_0016_A					BLK0+16           	//1     16
#define  	iF_0018_A					BLK0+18           	//1     16
#define  	iF_0020_A             		BLK0+20             //1     16
#define  	iF_0022_A					BLK0+22            	//1     16
#define  	iF_0024_A					BLK0+24            	//1     16
#define  	iF_0026_A             		BLK0+26            	//1     16
#define  	iF_0028_A					BLK0+28             //1     16
#define  	iF_0030_A					BLK0+30            	//1     16
#define  	iF_0032_A             		BLK0+32            	//1     16
#define  	iF_0034_A					BLK0+34            	//1     16
#define  	iF_0036_A					BLK0+36            	//1     16
#define  	iF_0038_A             		BLK0+38            	//1     16
#define  	iF_0040_A					BLK0+40             //1     16
#define  	iF_0042_A					BLK0+42            	//1     16
#define  	iF_0044_A             		BLK0+44             //1     16
#define  	iF_0046_A					BLK0+46            	//1     16
#define  	iF_0048_A					BLK0+48             //1     16
#define  	iF_0050_A             		BLK0+50             //1     16
#define  	iF_0052_A					BLK0+52             //1     16
#define  	iF_0054_A					BLK0+54            	//1     16
#define  	iF_0056_A             		BLK0+56            	//1     16
#define  	iF_0058_A					BLK0+58            	//1     16
#define  	iF_0060_A					BLK0+60            	//1     16
#define  	iF_0062_A					BLK0+62            	//1     16

#define  	iF_0064_A					BLK0+64          	//1     16
#define  	iF_0066_A             		BLK0+66             //1     16
#define  	iF_0068_A					BLK0+68             //1     16
#define  	iF_0070_A					BLK0+70             //1     16
#define  	iF_0072_A             		BLK0+72             //1     16
#define  	iF_0074_A					BLK0+74             //1     16
#define  	iF_0076_A					BLK0+76             //1     16
#define  	iF_0078_A             		BLK0+78            	//1     16
#define  	iF_0080_A					BLK0+80            	//1     16
#define  	iF_0082_A					BLK0+82             //1     16
#define  	iF_0084_A             		BLK0+84             //1     16
#define  	iF_0086_A					BLK0+86            	//1     16
#define  	iF_0088_A					BLK0+88            	//1     16
#define  	iF_0090_A             		BLK0+90            	//1     16
#define  	iF_0092_A					BLK0+92             //1     16
#define  	iF_0094_A					BLK0+94            	//1     16
#define  	iF_0096_A             		BLK0+96            	//1     16
#define  	iF_0098_A					BLK0+98            	//1     16
#define  	iF_0100_A					BLK0+100            //1     16
#define  	iF_0102_A             		BLK0+102            //1     16
#define  	iF_0104_A					BLK0+104            //1     16
#define  	iF_0106_A					BLK0+106            //1     16
#define  	iF_0108_A             		BLK0+108            //1     16
#define  	iF_0110_A					BLK0+110            //1     16
#define  	iF_0112_A					BLK0+112            //1     16
#define  	iF_0114_A             		BLK0+114            //1     16
#define  	iF_0116_A					BLK0+116            //1     16
#define  	iF_0118_A					BLK0+118            //1     16
#define  	iF_0120_A             		BLK0+120            //1     16
#define  	iF_0122_A					BLK0+122            //1     16
#define  	iF_0124_A					BLK0+124            //1     16
#define  	iF_0126_A					BLK0+126            //1     16

#define  	iF_0128_A					BLK0+128            //1     16
#define  	iF_0130_A             		BLK0+130            //1     16
#define  	iF_0132_A					BLK0+132            //1     16
#define  	iF_0134_A					BLK0+134            //1     16
#define  	iF_0136_A             		BLK0+136            //1     16
#define  	iF_0138_A					BLK0+138            //1     16
#define  	iF_0140_A					BLK0+140            //1     16
#define  	iF_0142_A             		BLK0+142            //1     16
#define  	iF_0144_A					BLK0+144            //1     16
#define  	iF_0146_A					BLK0+146            //1     16
#define  	iF_0148_A             		BLK0+148            //1     16
#define  	iF_0150_A					BLK0+150            //1     16
#define  	iF_0152_A					BLK0+152            //1     16
#define  	iF_0154_A             		BLK0+154            //1     16
#define  	iF_0156_A					BLK0+156            //1     16
#define  	iF_0158_A					BLK0+158            //1     16
#define  	iF_0160_A             		BLK0+160            //1     16
#define  	iF_0162_A					BLK0+162            //1     16
#define  	iF_0164_A					BLK0+164            //1     16
#define  	iF_0168_A             		BLK0+168            //1     16
#define  	iF_0170_A					BLK0+170            //1     16
#define  	iF_0172_A					BLK0+172            //1     16
#define  	iF_0174_A             		BLK0+174            //1     16
#define  	iF_0176_A					BLK0+176            //1     16
#define  	iF_0178_A					BLK0+178            //1     16
#define  	iF_0180_A             		BLK0+180            //1     16
#define  	iF_0182_A					BLK0+182            //1     16
#define  	iF_0184_A					BLK0+184            //1     16
#define  	iF_0186_A             		BLK0+186            //1     16
#define  	iF_0188_A					BLK0+188            //1     16
#define  	iF_0190_A					BLK0+190            //1     16

#define  	iF_0192_A					BLK0+192            //1     16
#define  	iF_0194_A					BLK0+194            //1     16
#define  	iF_0196_A             		BLK0+196            //1     16
#define  	iF_0198_A					BLK0+198            //1     16
#define  	iF_0200_A					BLK0+200            //1     16
#define  	iF_0202_A             		BLK0+202            //1     16
#define  	iF_0204_A					BLK0+204            //1     16
#define  	iF_0206_A					BLK0+206            //1     16
#define  	iF_0208_A             		BLK0+208            //1     16
#define  	iF_0210_A					BLK0+210            //1     16
#define  	iF_0212_A					BLK0+212            //1     16
#define  	iF_0214_A             		BLK0+214            //1     16
#define  	iF_0216_A					BLK0+216            //1     16
#define  	iF_0218_A					BLK0+218            //1     16
#define  	iF_0220_A             		BLK0+220            //1     16
#define  	iF_0222_A					BLK0+222            //1     16
#define  	iF_0224_A					BLK0+224            //1     16
#define  	iF_0226_A             		BLK0+226            //1     16
#define  	iF_0228_A					BLK0+228            //1     16
#define  	iF_0230_A					BLK0+230            //1     16
#define  	iF_0232_A             		BLK0+232            //1     16
#define  	iF_0234_A					BLK0+234            //1     16
#define  	iF_0236_A					BLK0+236            //1     16
#define  	iF_0238_A             		BLK0+238            //1     16
#define  	iF_0240_A					BLK0+240            //1     16
#define  	iF_0242_A					BLK0+242            //1     16
#define  	iF_0244_A             		BLK0+244            //1     16
#define  	iF_0246_A					BLK0+246            //1     16
#define  	iF_0248_A					BLK0+248            //1     16
#define  	iF_0250_A             		BLK0+250            //1     16
#define  	iF_0252_A					BLK0+252            //1     16
#define  	iF_0254_A					BLK0+254            //1     16


///////////////////////////////////////////////////////////////////////
#define  	lF_0000_A					BLK0+0            	//1     16
#define  	lF_0004_A					BLK0+4             	//1     16
#define  	lF_0008_A             		BLK0+8            	//1     16
#define  	lF_0012_A					BLK0+12            	//1     16
#define  	lF_0016_A					BLK0+16           	//1     16
#define  	lF_0020_A             		BLK0+20             //1     16
#define  	lF_0024_A					BLK0+24            	//1     16
#define  	lF_0028_A					BLK0+28             //1     16
#define  	lF_0032_A             		BLK0+32            	//1     16
#define  	lF_0036_A					BLK0+36            	//1     16
#define  	lF_0040_A					BLK0+40             //1     16
#define  	lF_0044_A             		BLK0+44             //1     16
#define  	lF_0048_A					BLK0+48             //1     16
#define  	lF_0052_A					BLK0+52             //1     16
#define  	lF_0056_A             		BLK0+56            	//1     16
#define  	lF_0060_A					BLK0+60            	//1     16

#define  	lF_0064_A					BLK0+64             //1     16
#define  	lF_0068_A					BLK0+68             //1     16
#define  	lF_0072_A             		BLK0+72             //1     16
#define  	lF_0076_A					BLK0+76             //1     16
#define  	lF_0080_A					BLK0+80            	//1     16
#define  	lF_0084_A             		BLK0+84             //1     16
#define  	lF_0088_A					BLK0+88            	//1     16
#define  	lF_0092_A					BLK0+92             //1     16
#define  	lF_0096_A             		BLK0+96            	//1     16
#define  	lF_0100_A					BLK0+100            //1     16
#define  	lF_0104_A					BLK0+104            //1     16
#define  	lF_0108_A             		BLK0+108            //1     16
#define  	lF_0112_A					BLK0+112            //1     16
#define  	lF_0116_A					BLK0+116            //1     16
#define  	lF_0120_A             		BLK0+120            //1     16
#define  	lF_0124_A					BLK0+124            //1     16

#define  	lF_0128_A					BLK0+128            //1     16
#define  	lF_0132_A					BLK0+132            //1     16
#define  	lF_0136_A             		BLK0+136            //1     16
#define  	lF_0140_A					BLK0+140            //1     16
#define  	lF_0144_A					BLK0+144            //1     16
#define  	lF_0148_A             		BLK0+148            //1     16
#define  	lF_0152_A					BLK0+152            //1     16
#define  	lF_0156_A					BLK0+156            //1     16
#define  	lF_0160_A             		BLK0+160            //1     16
#define  	lF_0164_A					BLK0+164            //1     16
#define  	lF_0168_A             		BLK0+168            //1     16
#define  	lF_0172_A					BLK0+172            //1     16
#define  	lF_0176_A					BLK0+176            //1     16
#define  	lF_0180_A             		BLK0+180            //1     16
#define  	lF_0184_A					BLK0+184            //1     16
#define  	lF_0188_A					BLK0+188            //1     16

#define  	lF_0192_A					BLK0+192            //1     16
#define  	lF_0196_A             		BLK0+196            //1     16
#define  	lF_0200_A					BLK0+200            //1     16
#define  	lF_0204_A					BLK0+204            //1     16
#define  	lF_0208_A             		BLK0+208            //1     16
#define  	lF_0212_A					BLK0+212            //1     16
#define  	lF_0216_A					BLK0+216            //1     16
#define  	lF_0220_A             		BLK0+220            //1     16
#define  	lF_0224_A					BLK0+224            //1     16
#define  	lF_0228_A					BLK0+228            //1     16
#define  	lF_0232_A             		BLK0+232            //1     16
#define  	lF_0236_A					BLK0+236            //1     16
#define  	lF_0240_A					BLK0+240            //1     16
#define  	lF_0244_A             		BLK0+244            //1     16
#define  	lF_0248_A					BLK0+248            //1     16
#define  	lF_0252_A					BLK0+252            //1     16


////////////////////////////////////////////////////////////////////////////////////////

#define  	bit_X0_A					cF_0000_A
#define  	bit_X1_A					cF_0001_A
#define  	bit_X2_A					cF_0002_A
#define  	bit_X3_A					cF_0003_A
#define  	bit_X4_A					cF_0004_A



#define  	cF_Version_A				cF_0005_A
#define  	cF_SetMyProductIdValue_A	cF_0006_A
#define  	cF_SetMyAddr1Value_A		cF_0007_A
#define  	cF_SetMyAddr2Value_A		cF_0008_A
#define  	cF_SetMyAddr3Value_A		cF_0009_A
#define  	cF_SetMyAddr4Value_A		cF_0010_A
#define 	cF_Com1MyProductIdValue_A	cF_0011_A
#define 	cF_Com1MyAddr1Value_A		cF_0012_A
#define 	cF_Com1MyAddr2Value_A		cF_0013_A
#define 	cF_Com1MyAddr3Value_A		cF_0014_A
#define 	cF_Com1MyAddr4Value_A		cF_0015_A
#define 	cF_Com2MyProductIdValue_A	cF_0016_A
#define 	cF_Com2MyAddr1Value_A		cF_0017_A
#define 	cF_Com2MyAddr2Value_A		cF_0018_A
#define 	cF_Com2MyAddr3Value_A		cF_0019_A
#define 	cF_Com2MyAddr4Value_A		cF_0020_A

#define 	cF_Can1MyGroupAddrValue_A	cF_0021_A
#define 	cF_Can1MyLocalAddrValue_A	cF_0022_A
#define 	cF_Can1MyProductIdValue_A	cF_0023_A
#define 	cF_Can2MyGroupAddrValue_A	cF_0024_A
#define 	cF_Can2MyLocalAddrValue_A	cF_0025_A
#define 	cF_Can2MyProductIdValue_A	cF_0026_A

#define 	cF_AirconAdd1_A				cF_0060_A
#define 	cF_AirconAdd2_A				cF_0061_A
#define 	cF_AirconAdd3_A				cF_0062_A
#define 	cF_AirconAdd4_A				cF_0063_A




#define 	iF_Com1Baudrate_A			iF_0064_A
#define 	iF_Com2Baudrate_A			iF_0066_A
#define 	iF_Can1Baudrate_A			iF_0068_A
#define 	iF_Can2Baudrate_A			iF_0070_A
#define 	iF_Can1RxSidMask_A			iF_0072_A
#define 	iF_Can1RxSidFilter_A		iF_0074_A
#define 	iF_Can2RxSidMask_A			iF_0076_A
#define 	iF_Can2RxSidFilter_A		iF_0078_A


#define 	lF_Can1RxEidMask_A			lF_0128_A
#define 	lF_Can1RxEidFilter_A		lF_0132_A
#define 	lF_Can2RxEidMask_A			lF_0136_A
#define 	lF_Can2RxEidFilter_A		lF_0140_A





//////////////////////////////////////////////////////////////////////////////////
#define  	bitF_HibProtocolCom1_Buf		Set_Bit_Flash_Buf((bit_X0_A),0)
#define  	bitF_HibProtocolCom2_Buf		Set_Bit_Flash_Buf((bit_X0_A),1)
#define  	bitF_HibProtocolCan1_Buf		Set_Bit_Flash_Buf((bit_X0_A),2)
#define  	bitF_HibProtocolCan2_Buf		Set_Bit_Flash_Buf((bit_X0_A),3)
#define  	bitF_HibProtocolSet_Buf			Set_Bit_Flash_Buf((bit_X0_A),4)
#define  	bitF_VirtualHostSet_Buf			Set_Bit_Flash_Buf((bit_X0_A),5)
#define  	bitF_IamRepeater_Buf       		Set_Bit_Flash_Buf((bit_X0_A),6) 
#define  	bitF_Sid_Eid_Dl_Buf            	Set_Bit_Flash_Buf((bit_X0_A),7) 

#define  	bitF_X0_08_Buf					Set_Bit_Flash_Buf((bit_X0_A),8)
#define  	bitF_X0_09_Buf					Set_Bit_Flash_Buf((bit_X0_A),9)
#define  	bitF_X0_10_Buf					Set_Bit_Flash_Buf((bit_X0_A),10)
#define  	bitF_X0_11_Buf					Set_Bit_Flash_Buf((bit_X0_A),11)
#define  	bitF_X0_12_Buf					Set_Bit_Flash_Buf((bit_X0_A),12)
#define  	bitF_X0_13_Buf					Set_Bit_Flash_Buf((bit_X0_A),13)
#define  	bitF_X0_14_Buf					Set_Bit_Flash_Buf((bit_X0_A),14)
#define  	bitF_X0_15_Buf					Set_Bit_Flash_Buf((bit_X0_A),15)

#define  	bitF_X0_16_Buf					Set_Bit_Flash_Buf((bit_X0_A),16)
#define  	bitF_X0_17_Buf					Set_Bit_Flash_Buf((bit_X0_A),17)
#define  	bitF_X0_18_Buf					Set_Bit_Flash_Buf((bit_X0_A),18)
#define  	bitF_X0_19_Buf					Set_Bit_Flash_Buf((bit_X0_A),19)
#define  	bitF_X0_20_Buf					Set_Bit_Flash_Buf((bit_X0_A),20)
#define  	bitF_X0_21_Buf					Set_Bit_Flash_Buf((bit_X0_A),21)
#define  	bitF_X0_22_Buf					Set_Bit_Flash_Buf((bit_X0_A),22)
#define  	bitF_X0_23_Buf					Set_Bit_Flash_Buf((bit_X0_A),23)

#define  	bitF_X0_24_Buf					Set_Bit_Flash_Buf((bit_X0_A),24)
#define  	bitF_X0_25_Buf					Set_Bit_Flash_Buf((bit_X0_A),25)
#define  	bitF_X0_26_Buf					Set_Bit_Flash_Buf((bit_X0_A),26)
#define  	bitF_X0_27_Buf					Set_Bit_Flash_Buf((bit_X0_A),27)
#define  	bitF_X0_28_Buf					Set_Bit_Flash_Buf((bit_X0_A),28)
#define  	bitF_X0_29_Buf					Set_Bit_Flash_Buf((bit_X0_A),29)
#define  	bitF_X0_30_Buf					Set_Bit_Flash_Buf((bit_X0_A),30)
#define  	bitF_X0_31_Buf					Set_Bit_Flash_Buf((bit_X0_A),31)



#define  	bitF_X4_00_Buf					Set_Bit_Flash_Buf((bit_X4_A),0)
#define  	bitF_X4_01_Buf					Set_Bit_Flash_Buf((bit_X4_A),1)
#define  	bitF_X4_02_Buf					Set_Bit_Flash_Buf((bit_X4_A),2)
#define  	bitF_X4_03_Buf					Set_Bit_Flash_Buf((bit_X4_A),3)
#define  	bitF_X4_04_Buf					Set_Bit_Flash_Buf((bit_X4_A),4)
#define  	bitF_X4_05_Buf					Set_Bit_Flash_Buf((bit_X4_A),5)
#define  	bitF_X4_06_Buf            		Set_Bit_Flash_Buf((bit_X4_A),6) 
#define  	bitF_X4_07_Buf            		Set_Bit_Flash_Buf((bit_X4_A),7) 


#define  	bitF_X4_24_Buf					Set_Bit_Flash_Buf((bit_X4_A),24)
#define  	bitF_X4_25_Buf					Set_Bit_Flash_Buf((bit_X4_A),25)
#define  	bitF_X4_26_Buf					Set_Bit_Flash_Buf((bit_X4_A),26)
#define  	bitF_X4_27_Buf					Set_Bit_Flash_Buf((bit_X4_A),27)
#define  	bitF_X4_28_Buf					Set_Bit_Flash_Buf((bit_X4_A),28)
#define  	bitF_X4_29_Buf					Set_Bit_Flash_Buf((bit_X4_A),29)
#define  	bitF_X4_30_Buf					Set_Bit_Flash_Buf((bit_X4_A),30)
#define  	bitF_X4_31_Buf					Set_Bit_Flash_Buf((bit_X4_A),31)





#define  	bitF_HibProtocolCom1    		(Get_Bit_Flash((bit_X0_A),0)  & 0x01)
#define  	bitF_HibProtocolCom2    		(Get_Bit_Flash((bit_X0_A),1)  & 0x01)
#define  	bitF_HibProtocolCan1   			(Get_Bit_Flash((bit_X0_A),2)  & 0x01)
#define  	bitF_HibProtocolCan2   			(Get_Bit_Flash((bit_X0_A),3)  & 0x01)
#define  	bitF_HibProtocolSet         	(Get_Bit_Flash((bit_X0_A),4)  & 0x01)
#define  	bitF_VirtualHostSet         	(Get_Bit_Flash((bit_X0_A),5)  & 0x01) 
#define  	bitF_IamRepeater           		(Get_Bit_Flash((bit_X0_A),6)  & 0x01) 
#define  	bitF_Sid_Eid_Dl            		(Get_Bit_Flash((bit_X0_A),7)  & 0x01) 


#define  	bitF_HibProtocolCom1x    		(Get_Bit_Flash((bit_X4_A),0)   & 0x01)
#define  	bitF_HibProtocolCom2x    		(Get_Bit_Flash((bit_X4_A),1)   & 0x01)
#define  	bitF_HibProtocolCan1x   		(Get_Bit_Flash((bit_X4_A),2)   & 0x01)
#define  	bitF_HibProtocolCan2x   		(Get_Bit_Flash((bit_X4_A),3)   & 0x01)
#define  	bitF_HibProtocolSetx        	(Get_Bit_Flash((bit_X4_A),4)   & 0x01)
#define  	bitF_VirtualHostSetx        	(Get_Bit_Flash((bit_X4_A),5)   & 0x01)
#define  	bitF_X4_06            			(Get_Bit_Flash((bit_X4_A),6)   & 0x01)
#define  	bitF_X4_07            			(Get_Bit_Flash((bit_X4_A),7)   & 0x01)
/////////////////////////////////////////////////////////////////////////////////


#define		cF_Version         			Get_Byte_Flash(cF_Version_A)
#define 	cF_SetMyProductIdValue		Get_Byte_Flash(cF_SetMyProductIdValue_A)
#define 	cF_SetMyAddr1Value			Get_Byte_Flash(cF_SetMyAddr1Value_A)
#define 	cF_SetMyAddr2Value			Get_Byte_Flash(cF_SetMyAddr2Value_A)
#define 	cF_SetMyAddr3Value			Get_Byte_Flash(cF_SetMyAddr3Value_A)
#define 	cF_SetMyAddr4Value			Get_Byte_Flash(cF_SetMyAddr4Value_A)

#define 	cF_Com1MyProductIdValue		Get_Byte_Flash(cF_Com1MyProductIdValue_A)
#define 	cF_Com1MyAddr1Value			Get_Byte_Flash(cF_Com1MyAddr1Value_A)
#define 	cF_Com1MyAddr2Value			Get_Byte_Flash(cF_Com1MyAddr2Value_A)
#define 	cF_Com1MyAddr3Value			Get_Byte_Flash(cF_Com1MyAddr3Value_A)
#define 	cF_Com1MyAddr4Value			Get_Byte_Flash(cF_Com1MyAddr4Value_A)

#define 	cF_Com2MyProductIdValue		Get_Byte_Flash(cF_Com2MyProductIdValue_A)
#define 	cF_Com2MyAddr1Value			Get_Byte_Flash(cF_Com2MyAddr1Value_A)
#define 	cF_Com2MyAddr2Value			Get_Byte_Flash(cF_Com2MyAddr2Value_A)
#define 	cF_Com2MyAddr3Value			Get_Byte_Flash(cF_Com2MyAddr3Value_A)
#define 	cF_Com2MyAddr4Value			Get_Byte_Flash(cF_Com2MyAddr4Value_A)


#define 	cF_Can1MyGroupAddrValue		Get_Byte_Flash(cF_Can1MyGroupAddrValue_A)
#define 	cF_Can1MyLocalAddrValue		Get_Byte_Flash(cF_Can1MyLocalAddrValue_A)
#define 	cF_Can1MyProductIdValue		Get_Byte_Flash(cF_Can1MyProductIdValue_A)
#define 	cF_Can2MyGroupAddrValue		Get_Byte_Flash(cF_Can2MyGroupAddrValue_A)
#define 	cF_Can2MyLocalAddrValue		Get_Byte_Flash(cF_Can2MyLocalAddrValue_A)
#define 	cF_Can2MyProductIdValue		Get_Byte_Flash(cF_Can2MyProductIdValue_A)


#define 	cF_AirconAdd1				Get_Byte_Flash(cF_AirconAdd1_A)
#define 	cF_AirconAdd2				Get_Byte_Flash(cF_AirconAdd2_A)
#define 	cF_AirconAdd3				Get_Byte_Flash(cF_AirconAdd3_A)
#define 	cF_AirconAdd4				Get_Byte_Flash(cF_AirconAdd4_A)





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
										    
#define 	iF_Com1Baudrate				Get_Integer_Flash(iF_Com1Baudrate_A)
#define 	iF_Com2Baudrate				Get_Integer_Flash(iF_Com2Baudrate_A)
#define 	iF_Can1Baudrate				Get_Integer_Flash(iF_Can1Baudrate_A)
#define 	iF_Can2Baudrate				Get_Integer_Flash(iF_Can2Baudrate_A)
#define 	iF_Can1RxSidMask			Get_Integer_Flash(iF_Can1RxSidMask_A)
#define 	iF_Can1RxSidFilter			Get_Integer_Flash(iF_Can1RxSidFilter_A)
#define 	iF_Can2RxSidMask			Get_Integer_Flash(iF_Can2RxSidMask_A)
#define 	iF_Can2RxSidFilter			Get_Integer_Flash(iF_Can2RxSidFilter_A)




#define 	lF_Can1RxEidMask			Get_Long_Flash(lF_Can1RxEidMask_A)
#define 	lF_Can1RxEidFilter			Get_Long_Flash(lF_Can1RxEidFilter_A)
#define 	lF_Can2RxEidMask			Get_Long_Flash(lF_Can2RxEidMask_A)
#define 	lF_Can2RxEidFilter			Get_Long_Flash(lF_Can2RxEidFilter_A)




///////////////////////////////////////////////////////////////////////////////////////////////









//#define  	bitF_GetBit(adr,bitnm) bitnm==0 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit0 : bitnm==1 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit1 : bitnm == 2 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit2 : bitnm==3 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit3 : bitnm==4 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit4 : bitnm==5 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit5 : bitnm==6 ? GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit6 : GET_BITFIELD(&UserPgmConfig[(adr % 64)/8]).bit7 
//#define  	bitF_GetBit(adr,bitnm) bitnm==0 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit0 : bitnm==1 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit1 : bitnm == 2 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit2 : bitnm==3 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit3 : bitnm==4 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit4 : bitnm==5 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit5 : bitnm==6 ? GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit6 : GET_LONGFIELD(&UserPgmConfig[(adr % 64)/8]).bitx.bit7 
//#define 	cF_GetByte(adr)		GET_LONGFIELD(&(UserPgmConfig[((adr)/4)]))			.byte[((adr)%4)]
//#define 	lF_GetByte(adr)		GET_LONGFIELD(&(UserPgmConfig[adr]))			.long_data




/*
#define  StartPulse             GET_LONGFIELD(&parameter_mirror[START_PULSE])		.long_data
#define  bxyz             		GET_BITFIELD(&UserPgmConfig[F_BrkStTm10/4])			.bit0  
#define  cF_TOPFLR              GET_LONGFIELD(&UserPgmConfig[F_TopFlr/4])  			.byte[F_TopFlr%4]
#define  iF_F_BrkStTm10         GET_LONGFIELD(&UserPgmConfig[F_BrkStTm10/4])      	.intger[(F_BrkStTm10%4)/2]
#define  FLOOR_COUNT(adr)       GET_LONGFIELD(&UserPgmConfig[(adr+FLR_01_PULSE)])	.long_data
*/


