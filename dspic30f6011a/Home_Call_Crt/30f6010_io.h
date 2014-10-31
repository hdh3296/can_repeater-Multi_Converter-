
#include  <p30f6011a.h>            


#define    _U1RXDA     U1STAbits.URXDA       //0
#define    _U1OERR     U1STAbits.OERR        //1
#define    _U1FERR     U1STAbits.FERR        //2
#define    _U1PERR     U1STAbits.PERR        //3
#define    _U1RIDLE    U1STAbits.RIDLE       //4
#define    _U1ADDEN    U1STAbits.ADDEN       //5
#define    _U1RXISEL0  U1STAbits.URXISEL0    //6
#define    _U1RXISEL1  U1STAbits.URXISEL1    //7   
#define    _U1TRMT     U1STAbits.TRMT        //8   
#define    _U1TXBF     U1STAbits.UTXBF       //9   
#define    _U1TXEN     U1STAbits.UTXEN       //10   
#define    _U1TXBRK    U1STAbits.UTXBRK      //11   
#define    _U1TXISEL   U1STAbits.UTXISEL     //15   
			

#define    _U1UARTEN   U1MODEbits.UARTEN      //15 
#define    _U1USIDL    U1MODEbits.USIDL       //13 
#define    _U1ALTIO    U1MODEbits.ALTIO       //10 
#define    _U1WAKE     U1MODEbits.WAKE        //7 
#define    _U1LPBACK   U1MODEbits.LPBACK      //6 
#define    _U1ABAUD    U1MODEbits.ABAUD       //5 
#define    _U1PDSEL    U1MODEbits.PDSEL       //2 
//#define    _U1PDSEL1   U1MODEbits.PDSEL1      //2 
//#define    _U1PDSEL0   U1MODEbits.PDSEL0      //1
#define    _U1STSEL    U1MODEbits.STSEL       //0 



#define    _U2RXDA     U2STAbits.URXDA       //0
#define    _U2OERR     U2STAbits.OERR        //1
#define    _U2FERR     U2STAbits.FERR        //2
#define    _U2PERR     U2STAbits.PERR        //3
#define    _U2RIDLE    U2STAbits.RIDLE       //4
#define    _U2ADDEN    U2STAbits.ADDEN       //5
#define    _U2RXISEL0  U2STAbits.URXISEL0    //6
#define    _U2RXISEL1  U2STAbits.URXISEL1    //7   
#define    _U2TRMT     U2STAbits.TRMT        //8   
#define    _U2TXBF     U2STAbits.UTXBF       //9   
#define    _U2TXEN     U2STAbits.UTXEN       //10   
#define    _U2TXBRK    U2STAbits.UTXBRK      //11   
#define    _U2TXISEL   U2STAbits.UTXISEL     //15   
			

#define    _U2UARTEN   U2MODEbits.UARTEN      //15 
#define    _U2USIDL    U2MODEbits.USIDL       //13 
#define    _U2ALTIO    U2MODEbits.ALTIO       //10 
#define    _U2WAKE     U2MODEbits.WAKE        //7 
#define    _U2LPBACK   U2MODEbits.LPBACK      //6 
#define    _U2ABAUD    U2MODEbits.ABAUD       //5 
#define    _U2PDSEL    U2MODEbits.PDSEL       //2 
//#define    _U2PDSEL1   U2MODEbits.PDSEL1      //2 
//#define    _U2PDSEL0   U2MODEbits.PDSEL0      //1
#define    _U2STSEL    U2MODEbits.STSEL       //0 

   

/*
#define    INT0IF      IFS0bits.INT0IF          //0 
#define    IC1IF       IFS0bits.IC1IF           //1 
#define    OC1IF       IFS0bits.OC1IF           //2
#define    T1IF        IFS0bits.T1IF            //3 
#define    IC2IF       IFS0bits.IC2IF           //4 
#define    OC2IF       IFS0bits.OC2IF           //5 
#define    T2IF        IFS0bits.T2IF            //6 
#define    T3IF        IFS0bits.T3IF            //7 
#define    SPI1IF      IFS0bits.SPI1IF          //8 
//#define    U1RXIF      IFS0bits.U1RXIF          //9 
//#define    U1TXIF      IFS0bits.U1TXIF          //10 
#define    ADIF        IFS0bits.ADIF            //11 
#define    NVMIF       IFS0bits.NVMIF           //12 
#define    SI2CIF      IFS0bits.SI2CIF          //13 
#define    MI2CIF      IFS0bits.MI2CIF          //14 
#define    CNIF        IFS0bits.CNIF            //15 
*/


/*
#define    INT0IE    IEC0bits.INT0IE      //0
#define    IC1IE     IEC0bits.IC1IE       //1
#define    OC1IE     IEC0bits.OC1IE       //2
#define    T1IE      IEC0bits.T1IE        //3
#define    IC2IE     IEC0bits.IC2IE       //4
#define    OC2IE     IEC0bits.OC2IE       //5
#define    T2IE      IEC0bits.T2IE        //6
#define    T3IE      IEC0bits.T3IE        //7


#define    SPI1IE    IEC0bits.SPI1IE      //8
//#define    U1RXIE    IEC0bits.U1RXIE      //9
//#define    U1TXIE    IEC0bits.U1TXIE      //10
#define    ADIE      IEC0bits.ADIE        //11
#define    NVMIE     IEC0bits.NVMIE       //12
#define    SI2CIE    IEC0bits.SI2CIE      //13
#define    MI2CIE    IEC0bits.MI2CIE      //14
#define    CNIE      IEC0bits.CNIE        //15
*/



#define    _T1ON    T1CONbits.TON
#define    _T1SIDL  T1CONbits.TSIDL

//#define    T1IE    IEC0bits.T1IE



//;     NVMCON : Non Volatile Memory Control Register
//;------------------------------------------------------------------------------

#define    PROGOP0    NVMCONbits.PROGOP0     //0
#define    PROGOP1    NVMCONbits.PROGOP1     //1
#define    PROGOP2    NVMCONbits.PROGOP2     //2
#define    PROGOP3    NVMCONbits.PROGOP3     //3
#define    PROGOP4    NVMCONbits.PROGOP4     //4
#define    PROGOP5    NVMCONbits.PROGOP5     //5
#define    PROGOP6    NVMCONbits.PROGOP6     //6

#define    TWRI       NVMCONbits.TWRI        //8
#define    WRERR      NVMCONbits.WRERR       //D
#define    WREN_ON    NVMCONbits.WREN        //E
#define    WR_START   NVMCONbits.WR          //F



/*;------------------------------------------------------------------------------
;     QEICON : Quadrature Encoder Interface Control Register
;------------------------------------------------------------------------------*/
#define    UPDN_SRC    QEICONbits.UPDN_SRC    //0x00
#define    TQCS        QEICONbits.TQCS        //0x01
#define    POSRES      QEICONbits.POSRES      //0x02
#define    TQCKPS0     QEICONbits.TQCKPS0     //0x03
#define    TQCKPS1     QEICONbits.TQCKPS1     //0x04
#define    TQGATE      QEICONbits.TQGATE      //0x05
#define    PCDOUT      QEICONbits.PCDOUT      //0x06
#define    SWPAB       QEICONbits.SWPAB       //0x07

#define    QEIM0       QEICONbits.QEIM0       //0x08
#define    QEIM1       QEICONbits.QEIM1       //0x09
#define    QEIM2       QEICONbits.QEIM2       //0x0a
#define    UPDN        QEICONbits.UPDN        //0x0b
#define    INDX        QEICONbits.INDX        //0x0c 
#define    QEISIDL     QEICONbits.QEISIDL     //0x0d 
#define    CNTERR      QEICONbits.CNTERR      //0x0f 

/*------------------------------------------------------------------------------
;     DFLTCON : Digital Filter Control Register
;------------------------------------------------------------------------------*/

#define    INDCK0      DFLTCONbits.INDCK0     //0x00
#define    INDCK1      DFLTCONbits.INDCK1     //0x01
#define    INDCK2      DFLTCONbits.INDCK2     //0x02
#define    INDOUT      DFLTCONbits.INDOUT     //0x03
#define    QECK0       DFLTCONbits.QECK0      //0x04
#define    QECK1       DFLTCONbits.QECK1      //0x05
#define    QECK2       DFLTCONbits.QECK2      //0x06
#define    QEOUT       DFLTCONbits.QEOUT      //0x07
#define    CEID        DFLTCONbits.CEID       //0x08



/*------------------------------------------------------------------------------
;     IFS2 : Interrupt Flag Status Register 2
;------------------------------------------------------------------------------*/
#define    OC5IF        IFS2bits.OC5IF       //0x00
#define    OC6IF        IFS2bits.OC6IF       //0x01
#define    OC7IF        IFS2bits.OC7IF       //0x02
#define    OC8IF        IFS2bits.OC8IF       //0x03
#define    INT3IF       IFS2bits.INT3IF      //0x04
#define    INT4IF       IFS2bits.INT4IF      //0x05
#define    C2IF         IFS2bits.C2IF        //0x06
#define    PWMIF        IFS2bits.PWMIF       //0x07
#define    QEIIF        IFS2bits.QEIIF       //0x0a
#define    LVDIF        IFS2bits.LVDIF       //0x0b
#define    FLTAIF       IFS2bits.FLTAIF      //0x0c


/*------------------------------------------------------------------------------
;     IEC2 : Interrupt Enable Control Register 2
;------------------------------------------------------------------------------*/
#define    OC5IE       IEC2bits.OC5IE      //0x00
#define    OC6IE       IEC2bits.OC6IE      //0x01
#define    OC7IE       IEC2bits.OC7IE      //0x02
#define    OC8IE       IEC2bits.OC8IE      //0x03
#define    INT3IE      IEC2bits.INT3IE     //0x04
#define    INT4IE      IEC2bits.INT4IE     //0x05
#define    C2IE        IEC2bits.C2IE       //0x06
#define    PWMIE       IEC2bits.PWMIE      //0x07
#define    QEIIE       IEC2bits.QEIIE      //0x08
#define    LVDIE       IEC2bits.LVDIE      //0x0a
#define    FLTAIE      IEC2bits.FLTAIE     //0x0b
#define    FLTBIE      IEC2bits.FLTBIE     //0x0c


/*------------------------------------------------------------------------------
;     RCON
;------------------------------------------------------------------------------*/
#define    POR      RCONbits.POR        //0x0
#define    BOR      RCONbits.BOR        //0x0
#define    IDLE     RCONbits.IDLE       //0x0
#define    SLEEP    RCONbits.SLEEP      //0x0

#define    WDTO     RCONbits.WDTO       //0x0
#define    SWDTEN   RCONbits.SWDTEN     //0x0
#define    SWR      RCONbits.SWR        //0x0
#define    EXTR     RCONbits.EXTR       //0x0

#define    LVDL0    RCONbits.LVDL0      //0x0
#define    LVDL1    RCONbits.LVDL1      //0x0
#define    LVDL2    RCONbits.LVDL2      //0x0
#define    LVDL3    RCONbits.LVDL3      //0x0

#define    LVDEN    RCONbits.LVDEN      //0x0
#define    BGST     RCONbits.BGST       //0x0
#define    IOPUWR   RCONbits.IOPUWR     //0x0
#define    TRAPR    RCONbits.TRAPR      //0x0

