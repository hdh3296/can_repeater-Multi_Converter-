

#define     LOCAL_NM        0      // Local number
#define     HIB_HPI         0      // HIB, HPI
#define     SUBDOOR_SEL     0      // 서브 도어,메인 도어 
#define     SENSOR_TYPE     0      // sensor type : 0 = not use,1= reopen, 2= call 
#define     BAGGAGE         0      // button type : 0 = man use,1=call(auto ready),close 2= call(any time),close(화물용)
#define     H_K_C           0      // 4=lamp4((bit2)//2=open on(bit1),//1= hib Button cancle(bit0)   
#define     CALL_OFFSET     0      // call offset
#define     SEARCH4         'Y'
#define     SEARCH5         'S'
#define     NC_END          'E'




////////////////////////////////////////////////////////
/////////////H_K_C Setting//////////////////////////////
////////////////////////////////////////////////////////
//    1                2             4                //
////////////////////////////////////////////////////////
//button cancle , 장애우 Button , lamp 4개 사용시  == 7
////////////////////////////////////////////////////////
//              , 장애우 Button , lamp 4개 사용시  == 6
////////////////////////////////////////////////////////
//button cancle , 	            , lamp 4개 사용시  == 5
////////////////////////////////////////////////////////
//                              , lamp 4개 사용시  == 4
////////////////////////////////////////////////////////
//button cancle , 장애우 Button                    == 3
////////////////////////////////////////////////////////
//              , 장애우 Button                    == 2
////////////////////////////////////////////////////////
//button cancle                                    == 1
////////////////////////////////////////////////////////
//몽땅 사용 안함                                   == 0
//////////////////////////////////////////////////////// 
 


extern  void    TopResetting(void);
extern  void    SetupCheck(void);


/////////extern  void    SetupMode(void);
extern  void    LoadSetupValue(void);
extern  void    CompanyWrite(void);

extern	unsigned char new_value[];

                                    
//extern  unsigned    char    SrcAddress;
extern  unsigned    char    LocalNumber;
//extern  unsigned    char    Model;
//extern  unsigned    char    SubkDoor;
extern  unsigned    char    offset_flr;
//extern  unsigned    char    Company;
extern  unsigned    char    ButtonType;
extern	unsigned    char    CallOffset;

//extern  unsigned    char    TopFloor;
//extern  unsigned    char    DspModel;
extern  unsigned    char    UpButtonTime;
extern  unsigned    char    DnButtonTime;
extern  unsigned    char    MainTimer;
extern  unsigned    char    shiftTime;


/*
extern  bit		UpKeyBit;
extern  bit   	DnKeyBit;
extern  bit   	FDsp;
extern  bit   	CmpSetBit;
extern  bit   	MaskSetBit;
extern  bit		bToggleOn;
extern  bit   	bCurOpenKey;
extern  bit   	bUserLamp4;
extern 	bit   	SetupBit;     
extern	bit   	ChangeSetup;
*/

