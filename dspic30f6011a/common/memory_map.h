
				
#define 	COMPANY          'A'   


#define     CAN_KEY_DOOR    0
#define     CAN_KEY_CLEAR   1
#define     CAN_KEY_READY   2
#define     CAN_KEY_SET     3
#define     CAN_KEY_CONFIRM 4
#define     CAN_VIP_COMMAND 0x81
#define     ALL_KEY_CLEAR   0xfd
#define     CAN_NO_KEY_SET  0xfe
#define     CAN_INVALID_CMD 0xff


#define     PC_COMMAND      0x80
#define     CMD_PARKING     0x01
#define     CMD_EVEN_ODD    0x02
#define     CMD_MORNING     0x03
#define     CMD_FIRE        0x04
#define     CMD_FAMILY_SEV  0x05
#define     CMD_CALL_SEV	0x06

#define 	UP_READY          0x80
#define 	DN_READY          0x40
#define 	CAR_READY         0x20
#define 	UP_CAR_READY      0xA0
#define 	DN_CAR_READY      0x60
#define 	UPDN_READY        0xc0
#define 	UPDN_CAR_READY    0xe0
#define 	ONLY_FLR          0x1f
		
#define  	S1_OPEN           0x01
#define  	S1_CLOSE          0x02
#define  	S1_OVERLOAD       0x04
#define  	S1_EMG            0x08
#define  	S1_BAT            0x10
#define  	S1_AUTO           0x20
#define  	S1_MANUAL         0x40
#define  	S1_UP             0x80

#define  	S2_DN             0x01
#define  	S2_CAR_MOVE       0x02
#define  	S2_EXT_BUT_CLR    0x04
#define  	S2_FLOW           0x08
#define  	S2_FIRE           0x10
#define  	S2_CAR_BUT_CLR    0x20
#define  	S2_OPEN_LAMP_ON   0x40    
#define  	S2_LAMP_USER      0x80

#define  	S3_VIP            0x01
#define  	S3_PARKING        0x02
#define  	S3_SHIFT          0x04
#define  	S3_UPDN_VO        0x08
#define  	S3_OPEN_SUB       0x10
#define  	S3_FULL           0x20
#define  	S3_CURKEY         0x40    
#define  	S3_STOP           0x80

#define  	S4_CAR_MOST_SERVICE    	0x01
#define  	S4_FAMILY_SERVICE		0x02
//#define  	S4_ARRIVE				0x04
#define  	S4_WAIT_FLR_SERVICE		0x08
#define  	S4_PARKING_READY		0x10
#define  	S4_DOOROPEN_WAIT		0x20


#define  	S0_FLOOR          0                       //StatusBuf + 0
#define  	S1_STATE          1                       //StatusBuf + 1 
#define  	S2_STATE          2                       //StatusBuf + 2 
#define  	S3_STATE          3                       //StatusBuf + 3 
#define  	S4_STATE          4                       //StatusBuf + 4 
#define  	DEST_FLR          5                       //StatusBuf + 5
#define  	DSP1              6                       //StatusBuf + 15 - 19
#define  	DSP2              7                       //StatusBuf + 15 - 19
#define  	FLR_ON_OFF0       8                       //StatusBuf + 15 - 19
#define  	FLR_ON_OFF1       9                       //StatusBuf + 15 - 19
#define  	FLR_ON_OFF2       10                      //StatusBuf + 15 - 19
#define  	FLR_ON_OFF3       11                      //StatusBuf + 15 - 19
#define  	mEqualFloor       12
#define  	mHighFloor        13
#define  	mLowFloor         14
#define  	mDoor             15
#define  	tmp2              16
#define  	mcurfloor         17
#define  	mBefcurfloor      18
#define  	mReqStopFloor     19
#define  	mAckStopFloor     20
#define  	mNewAckStopFloor  21
#define  	mCarMoveState     22
#define  	mDoorSeq          23
#define  	mSysStatus        24
#define  	mBefSysStatus     25
#define     mTestData         26
#define  	mBefErrCntPt      27
#define  	LD_INSP           28
#define  	FHM_RUN           29
#define  	FHM_SEQ           30
#define  	Year              31
#define  	Month             32
#define  	Day               33
#define  	Hour              34
#define  	Minuate           35
#define  	sec               36
#define  	O_OP              37
#define  	O_U_W             38
#define  	O_Y_0             39
#define  	I_EMG             40
#define  	I_SU1             41
#define  	I_GR              42
#define     I_FIRE            43
#define  	I_X_0             44
#define  	I_FS0             45
#define  	SelDoorStatus     46
#define  	ExtCmdFlrONOFF0   47
#define  	ExtCmdFlrONOFF1   48
#define  	ExtCmdFlrONOFF2   49
#define  	ExtCmdFlrONOFF3   50
#define  	mFireSeq          51

#define  	mCarOpCl          52
#define  	mCarKey1          53
#define  	mCarKey9          54
#define  	mCarKey17         55
#define  	mCarKey25         56
#define  	mUnKnown06        57
#define  	mUnKnown07        58
#define  	mUnKnown08        59
#define  	mUnKnown11        60
#define  	mUnKnown12        61
#define  	mUnKnown13        62
#define  	mUnKnown14        63
#define  	mUnKnown15        64
#define  	mMostLongDst      65
#define  	mStartFloor       66
#define  	mTopFlr           67
#define  	mUnKnown21        68
#define  	mUnKnown22        69
#define  	mUnKnown23        70
#define  	mUnKnown24        71
#define  	mUnKnown25        72
#define  	mUnKnown26        73
#define  	mUnKnown27        74
#define  	mCallMe           75


#define  	mVFlrCnt          76
#define  	mVFlr1            77
#define  	mVFlr2            78
#define  	mx3              79
#define  	mx4              80
#define  	mx5              81
#define  	mx6              82

#define  	EndsRam           83
#define  	TestsRam          84

#define  	END_SRAM          EndsRam+1



#define     SL_mCarOpCl         16
#define     SL_mCarKey1         17
#define     SL_mCarKey9         18
#define     SL_mCarKey17        19
#define     SL_mCarKey25        20
#define     SL_mUnKnown06       21   //status nm
#define     SL_mUnKnown07       22   // o_op
#define     SL_mUnKnown08       23   // o_up_ward
#define     SL_mUnKnown11       24   // o_fan
#define     SL_mUnKnown12       25   // i_emg
#define     SL_mUnKnown13       26   //i_su1
#define     SL_mUnKnown14       27   //i_gr
#define     SL_mUnKnown15       28   //i_fire
#define     SL_mMostLongDst     29   
#define     SL_mStartFloor      30
#define     SL_mTopFlr          31
#define     SL_mUnKnown21       32  //mpm
#define     SL_mUnKnown22       33  //mpm
#define     SL_mUnKnown23       34    
#define     SL_mUnKnown24       35  //i_x0 
#define     SL_mUnKnown25       36	//i_fs0
#define     SL_mUnKnown26       37
#define     SL_mUnKnown27       38
#define     SL_mCallMe          39



#define  LOG_ADR        0   
#define  SLAVE1         1
#define  SLAVE2         2
#define  SLAVE3         3
#define  SLAVE4         4
#define  SLAVE5         5
#define  SLAVE6         6
#define  SLAVE7         7
#define  SLAVE8         8
#define  SLAVE9         9
#define  SLAVE10        10
#define  SLAVE11        11
#define  SLAVE12        12
#define  SLAVE13        13
#define  SLAVE14        14
#define  SLAVE15        15
#define  SLAVE16        16
#define  SLAVE17        17
#define  SLAVE18        18
#define  SLAVE19        19
#define  SLAVE20        20
#define  SLAVE21        21
#define  SLAVE22        22
#define  SLAVE23        23
#define  SLAVE24        24
#define  SLAVE25        25
#define  SLAVE26        26
#define  SLAVE27        27
#define  SLAVE28        28
#define  SLAVE29        29
#define  SLAVE30        30
#define  SLAVE31        31
#define  SLAVE32        32
#define  A_VOI          33
#define  B_VOI          34
#define  C_VOI          35
#define  D_VOI          36
#define  A_CAR1         37
#define  A_CAR2         38
#define  A_CAR3         39
#define  A_CAR4         40
#define  B_CAR1         41
#define  B_CAR2         42
#define  B_CAR3         43
#define  B_CAR4         44
#define  C_CAR1         45
#define  C_CAR2         46
#define  C_CAR3         47
#define  C_CAR4         48
#define  D_CAR1         49
#define  D_CAR2         50
#define  D_CAR3         51
#define  D_CAR4         52
#define  A_XO           53   
#define  A_X1           54
#define  B_XO           55
#define  B_X1           56
#define  C_XO           57
#define  C_X1           58
#define  D_XO           59
#define  D_X1           60   
#define  MAS_A          61
#define  B_M1           62
#define  B_M2           63
#define  C_M1           64
#define  C_M2           65
#define  D_M1           66
#define  D_M2           67
#define  A_LD           68
#define  B_LD           69
#define  C_LD           70
#define  D_LD           71
#define  A_PC           72
#define  B_PC           73
#define  C_PC           74
#define  D_PC           75


#define  BUT_CANLE_T          220
#define  MAX_CALL_TIME        20
