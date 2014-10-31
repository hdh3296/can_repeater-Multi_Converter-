
#include <p30f6011a.h>            
#include <float.h>            

#include  "..\..\multi_converter_common\host_io.h"            
#include  "..\..\multi_converter_common\30f6010_io.h"
#include  "..\..\multi_converter_common\you_can1.h"
#include  "..\..\multi_converter_common\you_can2.h"
#include  "..\..\multi_converter_common\ComPort1.h"
#include  "..\..\multi_converter_common\ComPort2.h"
#include  "..\..\multi_converter_common\memory_map.h"
#include  "..\..\multi_converter_common\multi_converter.h"
#include  "..\..\multi_converter_common\WorkingRam.h"
#include  "..\..\multi_converter_common\my_util.h"
//#include  "prj_app.h"


#include	"myCall_Power_crt.h"


#define	POLLING_MAX_WAIT_TIME	20
#define	BIT_AREA	56


#define	HI_TENSION_1		0	//고압반1
#define	HI_TENSION_2		1	//고압반2
#define	RECTIFIER			2	// 정류기반
#define	GENERATOR			3   // 발전기반 
#define	LOW_TENSION_1		4	//저압반1-PL1
#define	LOW_TENSION_2		5	//저압반2-PL4
#define	LOW_TENSION_3		6	//저압반3-PL6
#define	LOW_TENSION_4		7	//저압반4-PL9
#define	LOW_TENSION_5		8	//저압반5-PL11


unsigned int 	Com2Crc;
unsigned char 	Com2ThisTarget;




unsigned char abc[15];

unsigned int   __attribute__((section(".usercode"))) f_to_string(void)
{

	typedef	union tt_tmpFloat{
        unsigned char	aa[4];
		float			fdata;
	}xtmpFloat;

	volatile xtmpFloat ftos;


	ftos.aa[0]=0x3f;
	ftos.aa[1]=0xb4;
	ftos.aa[2]=0x96;
	ftos.aa[3]=0x49;

////ftos.fdata=1234567.891; 

	sprintf(abc,"%.2f",ftos.fdata);

	return(0);
}




unsigned int   __attribute__((section(".usercode"))) DataSwap(unsigned char *srcpt,unsigned char *destpt)
{
	*(destpt+0)=*(srcpt+1);
	*(destpt+1)=*(srcpt+0);
	*(destpt+2)=*(srcpt+3);
	*(destpt+3)=*(srcpt+2);

	return(0);
}



unsigned int   __attribute__((section(".usercode"))) VIDER_DC1_IO_Resetting(unsigned char pt)
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);


	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);


	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB1   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR1   == OFF

	if(bufh & 0x0010)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x02);			// ACB2   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x02);		// ACB2   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x20);			// OCR2   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x20);		// OCR2   == OFF


	if(bufh & 0x0080)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);		// PL2   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);	// PL2   == OFF

	if(bufh & 0x0100)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);	// PL3   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);// PL3   == OFF

	return(0);
}

unsigned int   __attribute__((section(".usercode"))) VIDER_DC2_IO_Resetting(unsigned char pt)  // pl4
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);


	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);


	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB1   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR1   == OFF

	if(bufh & 0x0010)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);			// PL4   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);		// PL4   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);			// PL5   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);		// PL6   == OFF


	return(0);
}


unsigned int   __attribute__((section(".usercode"))) VIDER_DC3_IO_Resetting(unsigned char pt)  // PL6
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);


	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);


	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB1   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR1   == OFF

	if(bufh & 0x0010)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x02);			// ACB2   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x02);		// ACB2   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x20);			// OCR2   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x20);		// OCR2   == OFF


	if(bufh & 0x0080)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);		// PL7   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);	// PL7   == OFF

	if(bufh & 0x0100)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);	// PL8   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);// PL8   == OFF

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) VIDER_DC4_IO_Resetting(unsigned char pt) //pl9
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);

	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);


	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB1   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR1   == OFF

	if(bufh & 0x0010)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);			// PL9   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);		// PL9   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);			// PL10   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);		// PL10   == OFF


	return(0);
}



unsigned int   __attribute__((section(".usercode"))) VIDER_DC5_IO_Resetting(unsigned char pt)  // pl11
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);

	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);

	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR   == OFF

	if(bufh & 0x0010)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x02);			// ATS1   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x02);		// ATS1   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x04);			// ATS2   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x04);		// ATS2   == OFF


	if(bufh & 0x0040)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);		// PL11   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);	// PL11   == OFF

	if(bufh & 0x0080)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);		// PL12   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);	// PL12   == OFF

	if(bufh & 0x0100)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x04);		// PL13   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x04);	// PL13   == OFF

	if(bufh & 0x0200)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x08);		// PL14   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x08);	// PL14   == OFF

	if(bufh & 0x0400)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x10);		// PL15   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x10);	// PL15   == OFF

	if(bufh & 0x0800)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x20);		// PL16   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x20);	// PL16   == OFF

	if(bufh & 0x1000)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x40);		// PL17   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x40);	// PL17   == OFF

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) VIPAM_3000_IO_Resetting(unsigned char pt)  // pl11
{
	unsigned int    idpt;
	unsigned int	bufh,bufl;
	unsigned int	ActivBit=0;


	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	idpt=(idpt + BIT_AREA);

	bufh=(unsigned int)(Com2RxBuffer[pt+1]);
	bufh=(unsigned int)(bufh << 8);
	bufl=(unsigned int)(Com2RxBuffer[pt]);
	bufh=(bufh | bufl);

	if(bufh & 0x0001)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x01);			// ACB   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x01);		// ACB   == OFF

	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x02);			// LBS   == ON
	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x02);		// LBS   == OFF

//	if(bufh & 0x0004)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR   == ON
//	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR   == OFF


//	if(bufh & 0x0020)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x04);			// ATS2   == ON
//	else				RcvBuf[idpt]=(RcvBuf[idpt] & ~0x04);		// ATS2   == OFF


	if(bufh & 0x0010)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x01);		// OVH1   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x01);	// OVH1   == OFF

	if(bufh & 0x0020)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x02);		// OVH2   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x02);	// OVH2   == OFF

	if(bufh & 0x0040)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x04);		// OVH3   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x04);	// OVH3   == OFF

	if(bufh & 0x0080)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x08);		// OVH4   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x08);	// OVH5   == OFF

	if(bufh & 0x0100)	RcvBuf[idpt+1]=(RcvBuf[idpt+1] | 0x10);		// OVH5   == ON
	else				RcvBuf[idpt+1]=(RcvBuf[idpt+1] & ~0x10);	// OVH5   == OFF


///////////////////////////////////////////////////////////////////////////////////////
/////////////////OCR,OCGR,UVR,OVR//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

	ActivBit=0;
	if(Com2RxBuffer[pt+7] & 0x80)	ActivBit=1;
	if(Com2RxBuffer[pt+8] & 0x01)	ActivBit=1;
	if(Com2RxBuffer[pt+8] & 0x02)	ActivBit=1;
	if(Com2RxBuffer[pt+8] & 0x10)	ActivBit=1;
	if(Com2RxBuffer[pt+8] & 0x20)	ActivBit=1;

	if(ActivBit)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x10);			// OCR   == ON
	else			RcvBuf[idpt]=(RcvBuf[idpt] & ~0x10);		// OCR   == OFF


	ActivBit=0;
	if(Com2RxBuffer[pt+7] & 0x40)	ActivBit=1;
	if(Com2RxBuffer[pt+8] & 0x04)	ActivBit=1;

	if(ActivBit)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x20);			// OCGR   == ON
	else			RcvBuf[idpt]=(RcvBuf[idpt] & ~0x20);		// OCGR   == OFF


	ActivBit=0;
	if(Com2RxBuffer[pt+6] & 0x10)	ActivBit=1;
	if(Com2RxBuffer[pt+6] & 0x20)	ActivBit=1;
	if(Com2RxBuffer[pt+6] & 0x40)	ActivBit=1;

	if(ActivBit)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x40);			// UVR   == ON
	else			RcvBuf[idpt]=(RcvBuf[idpt] & ~0x40);		// UVR   == OFF


	ActivBit=0;
	if(Com2RxBuffer[pt+7] & 0x01)	ActivBit=1;
	if(Com2RxBuffer[pt+7] & 0x02)	ActivBit=1;
	if(Com2RxBuffer[pt+7] & 0x04)	ActivBit=1;

	if(ActivBit)	RcvBuf[idpt]=(RcvBuf[idpt] | 0x80);			// OVR   == ON
	else			RcvBuf[idpt]=(RcvBuf[idpt] & ~0x80);		// OVR   == OFF

	return(0);
}




unsigned int   __attribute__((section(".usercode"))) GIPAM_115FI_IO_ReSetting(unsigned int idpt)
{
	unsigned char	buf;
	unsigned int	xidpt;

	xidpt=(idpt + 56);

	buf=Com2RxBuffer[3+2];

	RcvBuf[xidpt]    =(RcvBuf[xidpt] & 0xf1);			// 00xx xxxx
	RcvBuf[xidpt+1]  =(RcvBuf[xidpt+1] & 0x01);			// 00xx xxxx


	if(buf & 0x02)	RcvBuf[xidpt]=(RcvBuf[xidpt] | 0x01);			// vcb1/acb1   == ON
	else			RcvBuf[xidpt]=(RcvBuf[xidpt] & ~0x01);			// vcb1/acb1   == OFF

	if(buf & 0x08)	RcvBuf[xidpt]=(RcvBuf[xidpt] | 0x10);			// OCR   == ON
	else			RcvBuf[xidpt]=(RcvBuf[xidpt] & ~0x10);			// OCR   == OFF

	if(buf & 0x10)	RcvBuf[xidpt]=(RcvBuf[xidpt] | 0x20);			// OCGR   == ON
	else			RcvBuf[xidpt]=(RcvBuf[xidpt] & ~0x20);			// OCGR   == OFF

	if(buf & 0x20)	RcvBuf[xidpt]=(RcvBuf[xidpt] | 0x80);			// OVR   == ON
	else			RcvBuf[xidpt]=(RcvBuf[xidpt] & ~0x80);			// OVR   == OFF

	if(buf & 0x40)	RcvBuf[xidpt]=(RcvBuf[xidpt] | 0x40);			// UVR   == ON
	else			RcvBuf[xidpt]=(RcvBuf[xidpt] & ~0x40);			// UVR   == OFF

	if(buf & 0x04)	RcvBuf[xidpt+1]=(RcvBuf[xidpt+1] | 0x01);		// alarm1   == ON   // 과충전 과방전 
	else			RcvBuf[xidpt+1]=(RcvBuf[xidpt+1] & ~0x01);		// alarm1   == OFF


	return(0);
}



unsigned int   __attribute__((section(".usercode"))) VIPAM_3000_CaculatePower(void)
{
	typedef	union x_tmpFloat{
        unsigned char	aa[4];
		float			fdata;
	}tmpFloat;

	volatile tmpFloat FLT[2];

	FLT[0].aa[0]=Com2RxBuffer[3+1];
	FLT[0].aa[1]=Com2RxBuffer[3+0];
	FLT[0].aa[2]=Com2RxBuffer[3+3];
	FLT[0].aa[3]=Com2RxBuffer[3+2];

	FLT[1].aa[0]=Com2RxBuffer[3+5];
	FLT[1].aa[1]=Com2RxBuffer[3+4];
	FLT[1].aa[2]=Com2RxBuffer[3+7];
	FLT[1].aa[3]=Com2RxBuffer[3+6];

	FLT[1].fdata= (FLT[1].fdata/(float)(10000));

	FLT[0].fdata=(FLT[0].fdata + FLT[1].fdata);

	Com2RxBuffer[3+0]=FLT[0].aa[1];
	Com2RxBuffer[3+1]=FLT[0].aa[0];
	Com2RxBuffer[3+2]=FLT[0].aa[3];
	Com2RxBuffer[3+3]=FLT[0].aa[2];
 
	return(0);
}



unsigned int   __attribute__((section(".usercode"))) GIPAM_115FI_Data(void)
{			
	unsigned int idpt;

	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	//VOLT
	RcvBuf[idpt+0]=Com2RxBuffer[3+20];
	RcvBuf[idpt+1]=Com2RxBuffer[3+21];
	RcvBuf[idpt+2]=Com2RxBuffer[3+22];
	RcvBuf[idpt+3]=Com2RxBuffer[3+23];

	RcvBuf[idpt+4]=Com2RxBuffer[3+24];
	RcvBuf[idpt+5]=Com2RxBuffer[3+25];
	RcvBuf[idpt+6]=Com2RxBuffer[3+26];
	RcvBuf[idpt+7]=Com2RxBuffer[3+27];

	RcvBuf[idpt+8] =Com2RxBuffer[3+28];
	RcvBuf[idpt+9] =Com2RxBuffer[3+29];
	RcvBuf[idpt+10]=Com2RxBuffer[3+30];
	RcvBuf[idpt+11]=Com2RxBuffer[3+31];

	//CURRENT
	RcvBuf[idpt+12]=Com2RxBuffer[3+36];
	RcvBuf[idpt+13]=Com2RxBuffer[3+37];
	RcvBuf[idpt+14]=Com2RxBuffer[3+38];
	RcvBuf[idpt+15]=Com2RxBuffer[3+39];

	RcvBuf[idpt+16]=Com2RxBuffer[3+40];
	RcvBuf[idpt+17]=Com2RxBuffer[3+41];
	RcvBuf[idpt+18]=Com2RxBuffer[3+42];
	RcvBuf[idpt+19]=Com2RxBuffer[3+43];

	RcvBuf[idpt+20]=Com2RxBuffer[3+44];
	RcvBuf[idpt+21]=Com2RxBuffer[3+45];
	RcvBuf[idpt+22]=Com2RxBuffer[3+46];
	RcvBuf[idpt+23]=Com2RxBuffer[3+47];

	//ACTIVE POWER
	RcvBuf[idpt+24]=Com2RxBuffer[3+56];
	RcvBuf[idpt+25]=Com2RxBuffer[3+57];
	RcvBuf[idpt+26]=Com2RxBuffer[3+58];
	RcvBuf[idpt+27]=Com2RxBuffer[3+59];
	//REACTIVE POWER
	RcvBuf[idpt+28]=Com2RxBuffer[3+60];
	RcvBuf[idpt+29]=Com2RxBuffer[3+61];
	RcvBuf[idpt+30]=Com2RxBuffer[3+62];
	RcvBuf[idpt+31]=Com2RxBuffer[3+63];
	//ACTIVE ENERGY
	RcvBuf[idpt+32]=Com2RxBuffer[3+68];
	RcvBuf[idpt+33]=Com2RxBuffer[3+69];
	RcvBuf[idpt+34]=Com2RxBuffer[3+70];
	RcvBuf[idpt+35]=Com2RxBuffer[3+71];
	//REACTIVE ENERGY
	RcvBuf[idpt+36]=Com2RxBuffer[3+72];
	RcvBuf[idpt+37]=Com2RxBuffer[3+73];
	RcvBuf[idpt+38]=Com2RxBuffer[3+74];
	RcvBuf[idpt+39]=Com2RxBuffer[3+75];
	//POWER FACTOR
	RcvBuf[idpt+40]=Com2RxBuffer[3+64];
	RcvBuf[idpt+41]=Com2RxBuffer[3+65];
	RcvBuf[idpt+42]=Com2RxBuffer[3+66];
	RcvBuf[idpt+43]=Com2RxBuffer[3+67];
	//Frequency
	RcvBuf[idpt+44]=Com2RxBuffer[3+48];
	RcvBuf[idpt+45]=Com2RxBuffer[3+49];
	RcvBuf[idpt+46]=Com2RxBuffer[3+50];
	RcvBuf[idpt+47]=Com2RxBuffer[3+51];

	GIPAM_115FI_IO_ReSetting(idpt);

	return(0);
}


unsigned int   __attribute__((section(".usercode"))) VIDER_Data(void)
{			
	unsigned int idpt;
	unsigned char j;

	j=(Com2RxBuffer[0]-1);
	idpt= ( (unsigned int)(j) * HOST_DATA_RECOD);

	switch(SubRdCmd){
		case	0:	// Volt Read
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+0]);   //rs
			DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+4]);   //st
			DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+8]);   //ts
			break;
		case	1:	// current read
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+12]);  //r
			DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+16]);  //s
			DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+20]);  //t

			break;
		case	2:	// active power total read  // 유효전력
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+24]);
			break;
		case	3:	// reactive power total read  // 무효전력
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+28]);
			break;
		case	4:	// active energy total read  // 유효전력양
			if( (ThisHostSelect == 0) || (ThisHostSelect == 1)){
				VIPAM_3000_CaculatePower();
			}
				DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+32]);
			break;
		case	5:	// reactive energy total read  // 무효전력양
			if( (ThisHostSelect == 0) || (ThisHostSelect == 1)){
				VIPAM_3000_CaculatePower();
			} 
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+36]);
			break;
		case	6:	//PF,Freq read
			if( (ThisHostSelect >= LOW_TENSION_1) && (ThisHostSelect <= LOW_TENSION_5)){
				DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+40]);  // freq
				DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+44]);  // PF
			}
			else{
				DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+40]);  // freq
				DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+44]);  // PF
			}
			break;
		case	7:	//D/I read
			if( (ThisHostSelect >= LOW_TENSION_1) && (ThisHostSelect <= LOW_TENSION_5)){
				switch(ThisHostSelect){
					case	LOW_TENSION_1:
						VIDER_DC1_IO_Resetting(3);
						break;
					case	LOW_TENSION_2:
						VIDER_DC2_IO_Resetting(3);
						break;
					case	LOW_TENSION_3:
						VIDER_DC3_IO_Resetting(3);
						break;
					case	LOW_TENSION_4:
						VIDER_DC4_IO_Resetting(3);
						break;
					case	LOW_TENSION_5:
						VIDER_DC5_IO_Resetting(3);
						break;
				}
				
			}
			else{
				VIPAM_3000_IO_Resetting(3);
			}					
			break;
		case	8:	// D/O read
			break;
		case	9:	// DC VOLT,DC CURRENT,BT CURRENT 
			DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+4]);		// dc volt
			DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+16]);     // dc current
			DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+20]);     // battery current
			break;
		default:
			break;
	};
}



unsigned int   __attribute__((section(".usercode"))) Com2ElectricDataSave(void)
{
	unsigned char i,j;

	j=(Com2RxBuffer[0]-1);
	i=ReqHost[j].Ho_Host[2];

	switch(i){
		case	VIPAM_3000_FI:
			VIDER_Data();
			break;
		case	VIDER_M20:
			VIDER_Data();
			break;
		case	GIPAM_115:
			GIPAM_115FI_Data();
			break;
		case	VIDER_DC:
			VIDER_Data();
			break;
		default:
			break;
	}



/*
		case	GENERATOR:
			switch(i){
				case	VIPAM_3000_FI:
					VIDER_Data();
					break;
				case	VIDER_M20:
					VIDER_Data();
					break;
				case	GIPAM_115:
					GIPAM_115FI_Data();
					break;
				case	VIDER_DC:
					VIDER_Data();
					break;
				default:
					break;
			}

*/

/*
	idpt= (ThisHostSelect * HOST_DATA_RECOD);

	switch(ThisHostSelect){
		case	GENERATOR:			
			//VOLT
			RcvBuf[idpt+0]=Com2RxBuffer[3+20];
			RcvBuf[idpt+1]=Com2RxBuffer[3+21];
			RcvBuf[idpt+2]=Com2RxBuffer[3+22];
			RcvBuf[idpt+3]=Com2RxBuffer[3+23];

			RcvBuf[idpt+4]=Com2RxBuffer[3+24];
			RcvBuf[idpt+5]=Com2RxBuffer[3+25];
			RcvBuf[idpt+6]=Com2RxBuffer[3+26];
			RcvBuf[idpt+7]=Com2RxBuffer[3+27];

			RcvBuf[idpt+8] =Com2RxBuffer[3+28];
			RcvBuf[idpt+9] =Com2RxBuffer[3+29];
			RcvBuf[idpt+10]=Com2RxBuffer[3+30];
			RcvBuf[idpt+11]=Com2RxBuffer[3+31];

			//CURRENT
			RcvBuf[idpt+12]=Com2RxBuffer[3+36];
			RcvBuf[idpt+13]=Com2RxBuffer[3+37];
			RcvBuf[idpt+14]=Com2RxBuffer[3+38];
			RcvBuf[idpt+15]=Com2RxBuffer[3+39];

			RcvBuf[idpt+16]=Com2RxBuffer[3+40];
			RcvBuf[idpt+17]=Com2RxBuffer[3+41];
			RcvBuf[idpt+18]=Com2RxBuffer[3+42];
			RcvBuf[idpt+19]=Com2RxBuffer[3+43];

			RcvBuf[idpt+20]=Com2RxBuffer[3+44];
			RcvBuf[idpt+21]=Com2RxBuffer[3+45];
			RcvBuf[idpt+22]=Com2RxBuffer[3+46];
			RcvBuf[idpt+23]=Com2RxBuffer[3+47];

			//ACTIVE POWER
			RcvBuf[idpt+24]=Com2RxBuffer[3+56];
			RcvBuf[idpt+25]=Com2RxBuffer[3+57];
			RcvBuf[idpt+26]=Com2RxBuffer[3+58];
			RcvBuf[idpt+27]=Com2RxBuffer[3+59];
			//REACTIVE POWER
			RcvBuf[idpt+28]=Com2RxBuffer[3+60];
			RcvBuf[idpt+29]=Com2RxBuffer[3+61];
			RcvBuf[idpt+30]=Com2RxBuffer[3+62];
			RcvBuf[idpt+31]=Com2RxBuffer[3+63];
			//ACTIVE ENERGY
			RcvBuf[idpt+32]=Com2RxBuffer[3+68];
			RcvBuf[idpt+33]=Com2RxBuffer[3+69];
			RcvBuf[idpt+34]=Com2RxBuffer[3+70];
			RcvBuf[idpt+35]=Com2RxBuffer[3+71];
			//REACTIVE ENERGY
			RcvBuf[idpt+36]=Com2RxBuffer[3+72];
			RcvBuf[idpt+37]=Com2RxBuffer[3+73];
			RcvBuf[idpt+38]=Com2RxBuffer[3+74];
			RcvBuf[idpt+39]=Com2RxBuffer[3+75];
			//POWER FACTOR
			RcvBuf[idpt+40]=Com2RxBuffer[3+64];
			RcvBuf[idpt+41]=Com2RxBuffer[3+65];
			RcvBuf[idpt+42]=Com2RxBuffer[3+66];
			RcvBuf[idpt+43]=Com2RxBuffer[3+67];
			//Frequency
			RcvBuf[idpt+44]=Com2RxBuffer[3+48];
			RcvBuf[idpt+45]=Com2RxBuffer[3+49];
			RcvBuf[idpt+46]=Com2RxBuffer[3+50];
			RcvBuf[idpt+47]=Com2RxBuffer[3+51];

			GIPAM_115FI_IO_ReSetting(idpt);

			break;
		case	HI_TENSION_1:
		case	HI_TENSION_2:
		case	RECTIFIER:
		case	LOW_TENSION_1:
		case	LOW_TENSION_2:
		case	LOW_TENSION_3:
		case	LOW_TENSION_4:
		case	LOW_TENSION_5:

			switch(SubRdCmd){
				case	0:	// Volt Read
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+0]);   //rs
					DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+4]);   //st
					DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+8]);   //ts
					break;
				case	1:	// current read
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+12]);  //r
					DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+16]);  //s
					DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+20]);  //t

					break;
				case	2:	// active power total read  // 유효전력
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+24]);
					break;
				case	3:	// reactive power total read  // 무효전력
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+28]);
					break;
				case	4:	// active energy total read  // 유효전력양
					if( (ThisHostSelect == 0) || (ThisHostSelect == 1)){
						VIPAM_3000_CaculatePower();
					}
 					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+32]);
					break;
				case	5:	// reactive energy total read  // 무효전력양
					if( (ThisHostSelect == 0) || (ThisHostSelect == 1)){
						VIPAM_3000_CaculatePower();
					} 
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+36]);
					break;
				case	6:	//PF,Freq read
					if( (ThisHostSelect >= LOW_TENSION_1) && (ThisHostSelect <= LOW_TENSION_5)){
						DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+40]);  // freq
						DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+44]);  // PF
					}
					else{
						DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+40]);  // freq
						DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+44]);  // PF
					}
					break;
				case	7:	//D/I read
					if( (ThisHostSelect >= LOW_TENSION_1) && (ThisHostSelect <= LOW_TENSION_5)){
						switch(ThisHostSelect){
							case	LOW_TENSION_1:
								VIDER_DC1_IO_Resetting(3);
								break;
							case	LOW_TENSION_2:
								VIDER_DC2_IO_Resetting(3);
								break;
							case	LOW_TENSION_3:
								VIDER_DC3_IO_Resetting(3);
								break;
							case	LOW_TENSION_4:
								VIDER_DC4_IO_Resetting(3);
								break;
							case	LOW_TENSION_5:
								VIDER_DC5_IO_Resetting(3);
								break;
						}
						
					}
					else{
						VIPAM_3000_IO_Resetting(3);
					}					
					break;
				case	8:	// D/O read
					break;
				case	9:	// DC VOLT,DC CURRENT,BT CURRENT 
					DataSwap( &Com2RxBuffer[3+0],&RcvBuf[idpt+4]);		// dc volt
					DataSwap( &Com2RxBuffer[3+4],&RcvBuf[idpt+16]);     // dc current
					DataSwap( &Com2RxBuffer[3+8],&RcvBuf[idpt+20]);     // battery current
					break;
				default:
					break;
			};
			break;
		default:
			break;
	}
 */
   
	Com2RxStatus=STX_CHK;
	Com2SerialTime=POLLING_MAX_WAIT_TIME - 3;

	ReqHost[ThisHostSelect].Ho_Host[4] = (ReqHost[ThisHostSelect].Ho_Host[4] | 0x01);    		

}




unsigned int   __attribute__((section(".usercode"))) Com2ReceiveData(unsigned char rdata)
{
   	unsigned char   buf;
   	unsigned char   temp;

	Com2RxBuffer[Com2RxCnt]=rdata;

   	buf=rdata;

    if(Com2RxStatus != TX_SET){   

		if(Com2SerialTime > 4){
            Com2RxCnt=0;
		}

        Com2SerialTime=0;
        Com2RxBuffer[Com2RxCnt]=buf;

        if(Com2RxCnt < (MAX_COM2_RXBUF-1)){
            Com2RxCnt++;
        }
        else{
            Com2RxCnt=0;
            Com2RxBuffer[Com2RxCnt]=buf;
        }

        switch(Com2RxStatus){
        	case    STX_CHK:
                if(Com2RxCnt == 1){
					if(buf == Com2TxBuffer[0]){
	                    Com2RxBuffer[0]=buf;				

	                    Com2Crc=0xffff;
						Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)buf);
					}
					else{
						Com2RxCnt=0;
					}
                }
                else if(Com2RxCnt == 2){
					if(buf == Com2TxBuffer[1]){
						Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)buf);
					}
					else{
						Com2RxCnt=0;
					}
                }
                else if(Com2RxCnt == 3){
					Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)buf);
					Com2RxStatus=DATA_CHK;
                }                
                else{
					if(buf == Com2TxBuffer[0]){
	                    Com2RxBuffer[0]=buf;	
	                    Com2Crc=0xffff;
						Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)buf);			
						Com2RxCnt=1;
					}
					else{
						Com2RxCnt=0;
					}
                }
        		break;
        	case    DATA_CHK:                                    
	            if(Com2RxCnt >= (Com2RxBuffer[2] + 3) ){
                    Com2RxStatus=CHKSUM_LOW_CHK;
                }
				Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)buf);
                break;
        	case    CHKSUM_LOW_CHK:
        		temp=(unsigned char)(Com2Crc & 0x00ff);
        		if(temp==buf){
        			Com2RxStatus=CHKSUM_HIGH_CHK;
        		}
        		else    Com2RxStatus=RX_ERROR;
        		break;
        	case    CHKSUM_HIGH_CHK:
        		Com2Crc=((Com2Crc >> 8 ) & 0x00ff);
        		temp=(unsigned char)Com2Crc;
        		if(buf==temp){
                    Com2RxStatus=RX_GOOD;
					Com2ElectricDataSave();
        		}
        		else    Com2RxStatus=RX_ERROR;
        		break;
        	case    RX_GOOD:
				break;
        	case    RX_ERROR:
        		Com2RxStatus=STX_CHK;
        		Com2RxCnt=0;
        		break;
        	default:
        		Com2RxStatus=STX_CHK;
        		Com2RxCnt=0;
        		break;
        }
	}	
	return(0);
}



unsigned char	Com2TxAct(void)
{
	unsigned char i;

    Com2Crc=0xffff;
    for(i=0;i<(Com2TxCnt-2);i++){
		Com2Crc=Common_Crc_Calulate(Com2Crc,(unsigned int)Com2TxBuffer[i]);
    }

    Com2TxBuffer[i]=(unsigned char)(Com2Crc & 0x00ff);
    Com2TxBuffer[i+1]=(unsigned char)((Com2Crc >> 8) & 0x00ff);

	Com2TxThisPt=0;
	Com2SerialTime=0;	
	Com2TxStart();
	return(0);
}



unsigned char	GIPAM_115FI(void)
{
	switch(SubRdCmd){
		case	0: // Total Read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x00;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 38;                  	// read  word  count low   -- 38 word == 76 byte
			Com2TxCnt = 8;
			break;
		default:
			SubRdCmd=0xff;
			break;
	}


	if(SubRdCmd < 0xff){
		Com2TxAct();
		return(0);	
	}
	return(1);
}



unsigned char	VIDER_M20_Read(void)
{

	switch(SubRdCmd){
		case	0: // Volt Read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x08;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	1:	// current read	
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 22;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	2:	// active power total read  // 유효전력
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 76;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	3:	// reactive power total read  // 무효전력
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 84;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	4:	// active energy total read  // 유효전력양
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 100;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                    // read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	5:	// reactive energy total read  // 무효전력양
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 116;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                     // read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	6:	//PF,Freq read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 50;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 4;                     // read  word  count low   -- 4 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	7:			//D/I read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x02;					// read D/I Status
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x00;					// start address low   tr-status
			Com2TxBuffer[4]= 0x00;					// read  byte  count high
			Com2TxBuffer[5]= 16;                    // read  byte  count low   -- 16bit == 2byte
			Com2TxCnt = 8;
			break;
		case	8:			// D/O read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x01;					// read D/O Status
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x00;					// start address low   tr-status
			Com2TxBuffer[4]= 0x00;					// read  byte  count high
			Com2TxBuffer[5]= 16;                    // read  byte  count low   -- 16 == 2byte
			Com2TxCnt = 8;
			break;
		default:
			SubRdCmd=0xff;
			break;
	}

	if(SubRdCmd < 0xff){
		Com2TxAct();
		return(0);	
	}
	return(1);

}




unsigned char	VIDER_DC_Read(void)   // 정류기반
{
	switch(SubRdCmd){
		case	0: // Volt Read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x08;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	1:	// current read	
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 22;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	2:	//active power total read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 76;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	3:	// reactive power total read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 84;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	4:	// active energy total read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 100;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                    // read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	5:	// reactive energy total read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 116;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                     // read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	6:	// PF,Freq read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 50;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 4;                     // read  word  count low   -- 4 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	7:	// D/I
		case	8:	// D/O
		case	9:
			SubRdCmd=9;
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// read DC VOLT,DC CURRENT, BATTERY CURRENT 
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 162;					// start address low   tr-status
			Com2TxBuffer[4]= 0x00;					// read  byte  count high
			Com2TxBuffer[5]= 6;                    // read  byte  count low   -- 6 == 12 byte
			Com2TxCnt = 8;
			break;
		default:
			SubRdCmd=0xff;
			break;
	}

	if(SubRdCmd < 0xff){
		Com2TxAct();
		return(0);	
	}
	return(1);
}



unsigned char	VIPAM3000_FI_Read(void)
{
	switch(SubRdCmd){
		case	0: // Volt Read
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x06;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	1:	// current read	
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 14;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 6;                  	// read  word  count low   -- 6 word == 12 byte
			Com2TxCnt = 8;
			break;
		case	2:	// active power total read  // 유효전력
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 26;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	3:	// reactive power total read  // 무효전력
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 28;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 2;                  	// read  word  count low   -- 2 word == 4 byte
			Com2TxCnt = 8;
			break;
		case	4:	// active energy total read  // 유효전력량 //re_arrary and calulate
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 32;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 4;                    // read  word  count low   -- 4 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	5:	// reactive energy total read  // 유효전력량,무효전력량  //re_arrary and calulate
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 40;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 4;                    // read  word  count low   -- 4 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	6:	// Freq,PF read                    // 주파수, 역율 
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x04;					// word read
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 52;					// start address low
			Com2TxBuffer[4]= 0x00;					// read  word  count high
			Com2TxBuffer[5]= 4;                     // read  word  count low   -- 4 word == 8 byte
			Com2TxCnt = 8;
			break;
		case	7:
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x02;					// read D/I Status
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 00;					// start address low   tr-status
			Com2TxBuffer[4]= 0x00;					// read  byte  count high
			Com2TxBuffer[5]= 80;                    // read  byte  count low   -- 80bit == 10 byte
			Com2TxCnt = 8;
			break;
		case	8:
			Com2TxBuffer[0]= (ThisHostSelect+1);
			Com2TxBuffer[1]= 0x01;					// read D/O Status
			Com2TxBuffer[2]= 0x00;					// start address high
			Com2TxBuffer[3]= 0x0;					// start address low   tr-status
			Com2TxBuffer[4]= 0x00;					// read  byte  count high
			Com2TxBuffer[5]= 32;                    // read  byte  count low   -- 32bit == 4 byte
			Com2TxCnt = 8;
			break;
		default:
			SubRdCmd=0xff;
			break;
	}


	if(SubRdCmd < 0xff){
		Com2TxAct();
		return(0);	
	}
	return(1);
}


unsigned char	Machine_name_Sort(void)
{
	unsigned char i;

	SubRdCmd++;
	i=ReqHost[ThisHostSelect].Ho_Host[2];
	switch(i){
		case	VIPAM_3000_FI:
			VIPAM3000_FI_Read();
			break;
		case	VIDER_M20:
			VIDER_M20_Read();
			break;
		case	GIPAM_115:
			GIPAM_115FI();
			break;
		case	VIDER_DC:
			VIDER_DC_Read();
			break;
		default:
			SubRdCmd=0xff;
			break;
	}

	if(SubRdCmd==0xff){
		ThisHostSelect++;
		if(ThisHostSelect >= MAX_ELEV){
			ThisHostSelect=0;
		}
	}

	return(0);
}



unsigned char	Normal_Read(void)
{

	if(Com2SerialTime < POLLING_MAX_WAIT_TIME)	return(1);
	
	switch(ThisHostSelect){
		case	HI_TENSION_1:    // 고압반 1
			Machine_name_Sort();
			break;
		case	HI_TENSION_2:
			Machine_name_Sort();
			break;
		case	RECTIFIER:
			Machine_name_Sort();
			break;
		case	GENERATOR:
			Machine_name_Sort();
			break;
		case	LOW_TENSION_1:
			Machine_name_Sort();
			break;
		case	LOW_TENSION_2:
			Machine_name_Sort();
			break;
		case	LOW_TENSION_3:
			Machine_name_Sort();
			break;
		case	LOW_TENSION_4:
			Machine_name_Sort();
			break;
		case	LOW_TENSION_5:
			Machine_name_Sort();
			break;
		default:
			SubRdCmd=0xff;
			ThisHostSelect=0;
			break;

	}

	return(0);
}


