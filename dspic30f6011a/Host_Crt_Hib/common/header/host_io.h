

#define DISABLE_INTERRUPTS 	asm("push SR \n bset SR,#7 \n bset SR,#6 \n	bclr	SR,#5")
#define ENABLE_INTERRUPTS	asm("pop SR")





#define    RB0_I		PORTBbits.RB0
#define    RB1_I		PORTBbits.RB1
#define    RB2_I		PORTBbits.RB2
#define    RB3_I		PORTBbits.RB3
#define    RB4_I		PORTBbits.RB4
#define    RB5_I		PORTBbits.RB5
#define    RB8_I		PORTBbits.RB8
#define    RB9_I		PORTBbits.RB9
#define    RB10_I		PORTBbits.RB10
#define    RB11_I		PORTBbits.RB11
#define    RB12_I		PORTBbits.RB12
#define    RB13_I		PORTBbits.RB13
#define    RB14_I		PORTBbits.RB14
#define    RB15_I		PORTBbits.RB15

#define    RC1_I		PORTCbits.RC1
#define    RC2_I   		PORTCbits.RC2
#define    RC3_I		PORTCbits.RC13
#define    RC4_I  		PORTCbits.RC14


#define    RD0_I		PORTDbits.RD0
#define    RD1_I		PORTDbits.RD1
#define    RD2_I		PORTDbits.RD2
#define    RD3_I		PORTDbits.RD3
#define    RD4_I		PORTDbits.RD4
#define    RD5_I		PORTDbits.RD5
#define    RD6_I		PORTDbits.RD6
#define    RD7_I		PORTDbits.RD7
#define    RD8_I		PORTDbits.RD8
#define    RD9_I		PORTDbits.RD9
#define    RD10_I		PORTDbits.RD10
#define    RD11_I		PORTDbits.RD11

#define    RF0_I		PORTFbits.RF0
#define    RF1_I   		PORTFbits.RF1
#define    RF2_I		PORTFbits.RF2
#define    RF3_I  		PORTFbits.RF3
#define    RF4_I		PORTFbits.RF4
#define    RF5_I   		PORTFbits.RF5
#define    RF6_I   		PORTFbits.RF6


#define    RG0_I		PORTGbits.RG0
#define    RG1_I		PORTGbits.RG1
#define    RG2_I		PORTGbits.RG2
#define    RG3_I		PORTGbits.RG3
#define    RG6_I		PORTGbits.RG6
#define    RG7_I		PORTGbits.RG7
#define    RG8_I		PORTGbits.RG8
#define    RG9_I  		PORTGbits.RG9
#define    RG12_I		PORTGbits.RG12
#define    RG13_I		PORTGbits.RG13
#define    RG14_I		PORTGbits.RG14
#define    RG15_I		PORTGbits.RG15
 

//////////////out put////////////////////
#define    RB0_OUT		LATBbits.LATB0
#define    RB1_OUT		LATBbits.LATB1
#define    RB2_OUT		LATBbits.LATB2
#define    RB3_OUT		LATBbits.LATB3
#define    RB4_OUT		LATBbits.LATB4
#define    RB5_OUT		LATBbits.LATB5
#define    RB8_OUT		LATBbits.LATB8
#define    RB9_OUT		LATBbits.LATB9
#define    RB10_OUT		LATBbits.LATB10
#define    RB11_OUT		LATBbits.LATB11
#define    RB12_OUT		LATBbits.LATB12
#define    RB13_OUT		LATBbits.LATB13
#define    RB14_OUT		LATBbits.LATB14
#define    RB15_OUT		LATBbits.LATB15

#define    RC1_OUT		LATCbits.LATC1
#define    RC2_OUT   	LATCbits.LATC2
#define    RC3_OUT		LATCbits.LATC13
#define    RC4_OUT  	LATCbits.LATC14


#define    RD0_OUT		LATDbits.LATD0
#define    RD1_OUT		LATDbits.LATD1
#define    RD2_OUT		LATDbits.LATD2
#define    RD3_OUT		LATDbits.LATD3
#define    RD4_OUT		LATDbits.LATD4
#define    RD5_OUT		LATDbits.LATD5
#define    RD6_OUT		LATDbits.LATD6
#define    RD7_OUT		LATDbits.LATD7
#define    RD8_OUT		LATDbits.LATD8
#define    RD9_OUT		LATDbits.LATD9
#define    RD10_OUT		LATDbits.LATD10
#define    RD11_OUT		LATDbits.LATD11

#define    RF0_OUT		LATFbits.LATF0
#define    RF1_OUT   	LATFbits.LATF1
#define    RF2_OUT		LATFbits.LATF2
#define    RF3_OUT  	LATFbits.LATF3
#define    RF4_OUT		LATFbits.LATF4
#define    RF5_OUT   	LATFbits.LATF5
#define    RF6_OUT   	LATFbits.LATF6


#define    RG0_OUT		LATGbits.LATG0
#define    RG1_OUT		LATGbits.LATG1
#define    RG2_OUT		LATGbits.LATG2
#define    RG3_OUT		LATGbits.LATG3
#define    RG6_OUT		LATGbits.LATG6
#define    RG7_OUT		LATGbits.LATG7
#define    RG8_OUT		LATGbits.LATG8
#define    RG9_OUT  	LATGbits.LATG9
#define    RG12_OUT		LATGbits.LATG12
#define    RG13_OUT		LATGbits.LATG13
#define    RG14_OUT		LATGbits.LATG14
#define    RG15_OUT		LATGbits.LATG15





#define    TRIS_RB0_IO		   	TRISBbits.TRISB0
#define    TRIS_RB1_IO		   	TRISBbits.TRISB1
#define    TRIS_RB2_IO		   	TRISBbits.TRISB2
#define    TRIS_RB3_IO		   	TRISBbits.TRISB3
#define    TRIS_RB4_IO		   	TRISBbits.TRISB4
#define    TRIS_RB5_IO		   	TRISBbits.TRISB5
#define    TRIS_RB8_IO		   	TRISBbits.TRISB8
#define    TRIS_RB9_IO		   	TRISBbits.TRISB9
#define    TRIS_RB10_IO		   	TRISBbits.TRISB10
#define    TRIS_RB11_IO			TRISBbits.TRISB11
#define    TRIS_RB12_IO			TRISBbits.TRISB12
#define    TRIS_RB13_IO			TRISBbits.TRISB13
#define    TRIS_RB14_IO			TRISBbits.TRISB14
#define    TRIS_RB15_IO			TRISBbits.TRISB15

#define    TRIS_RC1_IO			TRISCbits.TRISC1
#define    TRIS_RC2_IO   		TRISCbits.TRISC2
#define    TRIS_RC13_IO			TRISCbits.TRISC13
#define    TRIS_RC14_IO  		TRISCbits.TRISC14


#define    TRIS_RD0_IO		   	TRISDbits.TRISD0
#define    TRIS_RD1_IO		   	TRISDbits.TRISD1
#define    TRIS_RD2_IO		   	TRISDbits.TRISD2
#define    TRIS_RD3_IO		   	TRISDbits.TRISD3
#define    TRIS_RD4_IO			TRISDbits.TRISD4
#define    TRIS_RD5_IO			TRISDbits.TRISD5
#define    TRIS_RD6_IO			TRISDbits.TRISD6
#define    TRIS_RD7_IO			TRISDbits.TRISD7
#define    TRIS_RD8_IO			TRISDbits.TRISD8
#define    TRIS_RD9_IO			TRISDbits.TRISD9
#define    TRIS_RD10_IO			TRISDbits.TRISD10
#define    TRIS_RD11_IO			TRISDbits.TRISD11

#define    TRIS_RF0_IO			TRISFbits.TRISF0
#define    TRIS_RF1_IO   		TRISFbits.TRISF1
#define    TRIS_RF2_IO			TRISFbits.TRISF2
#define    TRIS_RF3_IO  		TRISFbits.TRISF3
#define    TRIS_RF4_IO			TRISFbits.TRISF4
#define    TRIS_RF5_IO   		TRISFbits.TRISF5
#define    TRIS_RF6_IO   		TRISFbits.TRISF6


#define    TRIS_RG0_IO			TRISGbits.TRISG0
#define    TRIS_RG1_IO			TRISGbits.TRISG1
#define    TRIS_RG2_IO			TRISGbits.TRISG2
#define    TRIS_RG3_IO			TRISGbits.TRISG3
#define    TRIS_RG6_IO		   	TRISGbits.TRISG6
#define    TRIS_RG7_IO		   	TRISGbits.TRISG7
#define    TRIS_RG8_IO		   	TRISGbits.TRISG8
#define    TRIS_RG9_IO 			TRISGbits.TRISG9
#define    TRIS_RG12_IO		   	TRISGbits.TRISG12
#define    TRIS_RG13_IO			TRISGbits.TRISG13
#define    TRIS_RG14_IO			TRISGbits.TRISG14
#define    TRIS_RG15_IO		   	TRISGbits.TRISG15






#define		RUN_LED			RC1_OUT				
#define		TXEN_485		RC2_OUT	

#define		CAN1_RX			RF0_I
#define		CAN1_TX			RF1_OUT
#define		UART1_RX		RF2_I	
#define		UART1_TX		RF3_OUT
#define		UART2_RX		RF4_I
#define		UART2_TX		RF5_OUT
#define		CAN2_RX			RG0_I
#define		CAN2_TX			RG1_OUT


#define    TRIS_RUN_LED		TRISCbits.TRISC1
#define    TRIS_TXEN_485   	TRISCbits.TRISC2
