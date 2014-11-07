

extern unsigned int     SelElevNoUseCheck(unsigned int id);
extern unsigned int     SelElevFlrDsbCheck(unsigned int id);
extern unsigned int     SelElevDsbCheck(unsigned int id);
extern unsigned char    UpDnKeyLoad(unsigned int id);
extern unsigned char    UpKeyCalcElev(unsigned int id);
extern unsigned char    DnKeyCalcElev(unsigned int id);
extern unsigned char  	GetElevPointer(unsigned int id);
extern unsigned char  	UpKeyReadyChk(unsigned int id);
extern unsigned char  	DnKeyReadyChk(unsigned int id);

extern void	DownButtonClear(void);
extern void	UpButtonClear(void);


extern void	UpDownKey(void);


//#define     IsDestFlrEqualReadyFlr(pt)         ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) == (RcvBuf[pt+tmp2] & ONLY_FLR))    ? (1) : (0)

//#define     IsDestFlrHighMe(pt,meFlr)          ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) >  meFlr)    ? (1) : (0)
//#define     IsDestFlrLowMe(pt,meFlr)           ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) <  meFlr)    ? (1) : (0)

//#define     IsDestFlrEqualMe(pt,meFlr)         ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) == meFlr)    ? (1) : (0)
//#define     IsDestFlrHighOrEqualMe(pt,meFlr)   ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) >= meFlr)    ? (1) : (0)
//#define     IsDestFlrLowOrEqualMe(pt,meFlr)    ((RcvBuf[pt+DEST_FLR] & ONLY_FLR) <= meFlr)    ? (1) : (0)


//#define     IsDestFlrNoneWard(pt)                 ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == 0)             ? (1) : (0)
//#define     IsDestFlrOnlyUp(pt)                   ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == UP_READY)      ? (1) : (0)
//#define     IsDestFlrOnlyDn(pt)                   ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == DN_READY)      ? (1) : (0)
//#define     IsDestFlrOnlyCar(pt)                  ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == CAR_READY)     ? (1) : (0)
//#define     IsDestFlrCarAndUp(pt)                 ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == UP_CAR_READY)  ? (1) : (0)
//#define     IsDestFlrCarAndDn(pt)                 ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY) == DN_CAR_READY)  ? (1) : (0)
//#define     IsDestFlrCarOrUp(pt)                  ((RcvBuf[pt+DEST_FLR] & UP_CAR_READY))                    ? (1) : (0)
//#define     IsDestFlrCarOrDn(pt)                  ((RcvBuf[pt+DEST_FLR] & DN_CAR_READY))                    ? (1) : (0)
//#define     IsDestFlrUp(pt)                       ((RcvBuf[pt+DEST_FLR] & UP_READY))                        ? (1) : (0)
//#define     IsDestFlrDn(pt)                       ((RcvBuf[pt+DEST_FLR] & DN_READY))                        ? (1) : (0)
//#define     IsDestFlrCarOrUpOrDn(pt)              ((RcvBuf[pt+DEST_FLR] & UPDN_CAR_READY))                  ? (1) : (0)

                                                                                                                                                                                                

/*
#define     IsReadyFlrNoneWard(pt)                  ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == 0)               ? (1) : (0)
#define     IsReadyFlrOnlyUp(pt)                    ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == UP_READY)        ? (1) : (0)
#define     IsReadyFlrOnlyDn(pt)                    ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == DN_READY)        ? (1) : (0)
#define     IsReadyFlrOnlyCar(pt)                   ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == CAR_READY)       ? (1) : (0)
#define     IsReadyFlrCarAndUp(pt)                  ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == UP_CAR_READY)    ? (1) : (0)
#define     IsReadyFlrCarAndDn(pt)                  ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY) == DN_CAR_READY)    ? (1) : (0)
#define     IsReadyFlrCarOrUp(pt)                   ((RcvBuf[pt+SL_mMostLongDst] & UP_CAR_READY))                      ? (1) : (0)
#define     IsReadyFlrCarOrDn(pt)                   ((RcvBuf[pt+SL_mMostLongDst] & DN_CAR_READY))                      ? (1) : (0)
#define     IsReadyFlrUp(pt)                        ((RcvBuf[pt+SL_mMostLongDst] & UP_READY))                          ? (1) : (0)
#define     IsReadyFlrDn(pt)                        ((RcvBuf[pt+SL_mMostLongDst] & DN_READY))                          ? (1) : (0)
#define     IsReadyFlrCarOrUpOrDn(pt)               ((RcvBuf[pt+SL_mMostLongDst] & UPDN_CAR_READY))                    ? (1) : (0)
*/

//#define     IsReadyFlrHighMe(pt,meFlr)              ((RcvBuf[pt+SL_mMostLongDst] & ONLY_FLR) >  meFlr)                  ? (1) : (0)
//#define     IsReadyFlrLowMe(pt,meFlr)               ((RcvBuf[pt+SL_mMostLongDst] & ONLY_FLR) <  meFlr)                  ? (1) : (0)
//#define     IsReadyFlrEqualMe(pt,meFlr)             ((RcvBuf[pt+SL_mMostLongDst] & ONLY_FLR) == meFlr)                  ? (1) : (0)
//#define     IsReadyFlrHighOrEqualMe(pt,meFlr)       ((RcvBuf[pt+SL_mMostLongDst] & ONLY_FLR) >=  meFlr)                 ? (1) : (0)
//#define     IsReadyFlrLowOrEqualMe(pt,meFlr)        ((RcvBuf[pt+SL_mMostLongDst] & ONLY_FLR) <=  meFlr)                 ? (1) : (0)



//#define     IsNextDestFlrHighMe(pt,meFlr)           ((RcvBuf[pt] & ONLY_FLR) >  meFlr)              ? (1) : (0)
//#define     IsNextDestFlrLowMe(pt,meFlr)            ((RcvBuf[pt] & ONLY_FLR) <  meFlr)              ? (1) : (0)
//#define     IsNextDestFlrEqualMe(pt,meFlr)          ((RcvBuf[pt] & ONLY_FLR) ==  meFlr)             ? (1) : (0)
//#define     IsNextDestFlrHighOrEqualMe(pt,meFlr)    ((RcvBuf[pt] & ONLY_FLR) >= meFlr)              ? (1) : (0)
//#define     IsNextDestFlrLowOrEqualMe(pt,meFlr)     ((RcvBuf[pt] & ONLY_FLR) <= meFlr)              ? (1) : (0)
//#define     IsNextDestFlrNone(pt)                   ((RcvBuf[pt] & UPDN_CAR_READY) == 0)            ? (1) : (0)
//#define     IsHeOnlyUp(pt)                          ((RcvBuf[pt] & UPDN_CAR_READY) == UP_READY)     ? (1) : (0)
//#define     IsHeOnlyDn(pt)                          ((RcvBuf[pt] & UPDN_CAR_READY) == DN_READY)     ? (1) : (0)
//#define     IsHeOnlyCar(pt)                         ((RcvBuf[pt] & UPDN_CAR_READY) == CAR_READY)    ? (1) : (0)
//#define     IsHeUp(pt)                              (RcvBuf[pt] & UP_READY)                         ? (1) : (0)
//#define     IsHeDn(pt)                              (RcvBuf[pt] & DN_READY)                         ? (1) : (0)
//#define     IsHeCarAndUp(pt)                        ((RcvBuf[pt] & UPDN_CAR_READY) == UP_CAR_READY) ? (1) : (0)
//#define     IsHeCarAndDn(pt)                        ((RcvBuf[pt] & UPDN_CAR_READY) == DN_CAR_READY) ? (1) : (0)


#define     IsCarClose(pt)                          (RcvBuf[pt + S1_STATE] & S1_CLOSE)              ? (1) : (0)
#define     IsCarMainOpen(pt)                       (RcvBuf[pt + S1_STATE] & S1_OPEN)               ? (1) : (0)
#define     IsCarSubOpen(pt)                        (RcvBuf[pt + S3_STATE] & S3_OPEN_SUB)           ? (1) : (0)
#define     IsCarUpWard(pt)                         (RcvBuf[pt + S1_STATE] & S1_UP)                 ? (1) : (0)
#define     IsCarDnWard(pt)                         (RcvBuf[pt + S2_STATE] & S2_DN)                 ? (1) : (0)
#define     IsCarAnyWard(pt)                        (IsCarDnWard(pt) || IsCarUpWard(pt))            ? (1) : (0)
#define     IsCarNoneWard(pt)                       (IsCarDnWard(pt) || IsCarUpWard(pt))            ? (0) : (1)


#define     IsCarAuto(pt)                            (RcvBuf[pt + S1_STATE] & S1_AUTO)              ? (1) : (0)
#define     IsCarManual(pt)                          (!(RcvBuf[pt + S1_STATE] & S1_AUTO))           ? (1) : (0)
#define     IsCarEmg(pt)                             (RcvBuf[pt + S1_STATE] & S1_EMG)               ? (1) : (0)
#define     IsCarFire(pt)                            (RcvBuf[pt + S2_STATE] & S2_FIRE)              ? (1) : (0)
#define     IsCarMove(pt)                            (RcvBuf[pt + S2_STATE] & S2_CAR_MOVE)          ? (1) : (0)
#define     IsCarExtButClr(pt)                       (RcvBuf[pt + S2_STATE] & S2_EXT_BUT_CLR)       ? (1) : (0)
#define     IsCarParking(pt)                         (RcvBuf[pt + S3_STATE] & S3_PARKING)           ? (1) : (0)
#define     IsCarVip(pt)                             (RcvBuf[pt + S3_STATE] & S3_VIP)               ? (1) : (0)
#define     IsCarFull(pt)                            (RcvBuf[pt + S3_STATE] & S3_FULL)              ? (1) : (0)
#define     IsMyFamilyService(pt)                    (RcvBuf[pt + S4_STATE] & S4_FAMILY_SERVICE)    ? (1) : (0)
#define     IsCarMostService(pt)                     (RcvBuf[pt + S4_STATE] & S4_CAR_MOST_SERVICE)  ? (1) : (0)
#define     IsCarArrive(pt)                     	 (RcvBuf[pt + S4_STATE] & S4_ARRIVE)  			? (1) : (0)

#define     IsHallLanternUpLamp(pt)                  (RcvBuf[pt + S4_STATE] & S4_HALLLANTERN_UP)  	? (1) : (0)
#define     IsHallLanternDnLamp(pt)                  (RcvBuf[pt + S4_STATE] & S4_HALLLANTERN_DN)  	? (1) : (0)


#define     IsYouFlrHighMe(pt,meFlr)           		((pt & ONLY_FLR) >  meFlr)              		? (1) : (0)
#define     IsYouFlrLowMe(pt,meFlr)            		((pt & ONLY_FLR) <  meFlr)              		? (1) : (0)
#define     IsYouFlrEqualMe(pt,meFlr)          		((pt & ONLY_FLR) ==  meFlr)             		? (1) : (0)
#define     IsYouFlrHighOrEqualMe(pt,meFlr)    		((pt & ONLY_FLR) >= meFlr)              		? (1) : (0)
#define     IsYouFlrLowOrEqualMe(pt,meFlr)     		((pt & ONLY_FLR) <= meFlr)              		? (1) : (0)

#define     IsYouFlrNoneWard(pt)               		((pt & UPDN_CAR_READY) == 0)            		? (1) : (0)
#define     IsYouFlrAnyWard(pt)               		( pt & UPDN_CAR_READY)            				? (1) : (0)
#define     IsYouOnlyUp(pt)                         ((pt & UPDN_CAR_READY) == UP_READY)     		? (1) : (0)
#define     IsYouOnlyDn(pt)                         ((pt & UPDN_CAR_READY) == DN_READY)     		? (1) : (0)
#define     IsYouOnlyCar(pt)                        ((pt & UPDN_CAR_READY) == CAR_READY)    		? (1) : (0)
#define     IsYouUp(pt)                             ( pt & UP_READY)                         		? (1) : (0)
#define     IsYouDn(pt)                             ( pt & DN_READY)                         		? (1) : (0)
#define     IsYouCar(pt)                            ( pt & CAR_READY)                         		? (1) : (0)
#define     IsYouCarAndUp(pt)                       ((pt & UPDN_CAR_READY) == UP_CAR_READY) 		? (1) : (0)
#define     IsYouCarAndDn(pt)                       ((pt & UPDN_CAR_READY) == DN_CAR_READY) 		? (1) : (0)
//#define     IsYouUpAndDn(pt)                        ((pt & UPDN_CAR_READY) == UPDN_READY)    		? (1) : (0)
#define     IsYouCarOrUp(pt)                        ((pt & UP_CAR_READY))                           ? (1) : (0)
#define     IsYouCarOrDn(pt)                        ((pt & DN_CAR_READY))                           ? (1) : (0)
#define     IsYouUpOrDn(pt)                         ((pt & UPDN_READY))                             ? (1) : (0)


#define     IsBufferPt(pt)                     		  ((pt * HOST_DATA_RECOD) + RCV_DATA)


//(RcvBuf[pt + S4_STATE] & S4_CAR_MOST_SERVICE)  ? (1) : (0)
//#define     BASE_OFFSET_CAR                           5

#define     GetMode(t_offset)                   	(t_offset / 8)
#define     GetBitPoint(t_offset)                   (0x01 << (t_offset % 8))


