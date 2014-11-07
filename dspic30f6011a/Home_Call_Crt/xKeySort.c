
#include <p30f6011a.h>            

//#include        "iomap.h"
#include        "xiodef.h"
/////////////////#include        "fontout.h"
#include        "xcomdata.h"
#include        "xesdots.h"
#include        "xkeysort.h"
#include        "xsetup.h"

#include        "memory_map.h"
//#include        "..\can_4480\you_can.h"

#include  		"you_can1.h"
#include  		"WorkingRam.h"






unsigned int  xIdPt;
unsigned char xtmpadr;
unsigned char xTmpCurFloor;
unsigned char xtmptop;
unsigned char xtmpstart;
unsigned char xtmpnext;
unsigned char xtmphigh;
unsigned char xtmplow;
unsigned char xdestflr;
unsigned char xyoufloor;
unsigned char Priority;



unsigned char maxlowflrbuf[MAX_ELEV];
unsigned char maxtopflrbuf[MAX_ELEV];
unsigned char maxtopflr;
unsigned char maxlowflr;


unsigned char Equal[MAX_ELEV];

//#define		SELECTED		7
#define		FIRST_OFFSET	    8
#define		SECOND_OFFSET	    10
#define		THIRD_OFFSET		12

#define     CalCulate(a,b,c,d,e)		((a-b) + (c-d) + e)                     		  





unsigned char  GetElevPointer(unsigned int id)
{
    xtmpadr=MyAddress-1;         
	xIdPt=IsBufferPt(id);
    xTmpCurFloor=RcvBuf[xIdPt]-1;
    xtmptop=RcvBuf[xIdPt+SL_mTopFlr];
    xtmpstart=RcvBuf[xIdPt+SL_mStartFloor];
    xtmpnext=RcvBuf[xIdPt+SL_mMostLongDst];
    xtmphigh=RcvBuf[xIdPt+mHighFloor];
    xtmplow=RcvBuf[xIdPt+mLowFloor];
    xdestflr=RcvBuf[xIdPt+DEST_FLR];
	xyoufloor=RcvBuf[xIdPt+DEST_FLR];


	if(IsCarUpWard(xIdPt)){
		if(xtmphigh & UPDN_CAR_READY){
			xyoufloor=xtmphigh;
		}
	}
	else if(IsCarDnWard(xIdPt)){
		if(xtmplow & UPDN_CAR_READY){
			xyoufloor=xtmplow;
		}
	}
	else{
		xyoufloor=xTmpCurFloor;
	}

	return(0);
}


/*
unsigned char  SyncButtonCheck(void)
{
	unsigned  int   IdPt; 

	IdPt=IsBufferPt(SrcAddress);
	if(IsMyFamilyService(IdPt))	return(0);

	if(b_MyFamily){
		if(SrcAddress  != LocalNumber)	return(0);
	}		
	return(1);		
}
*/


unsigned int  UpDnKeyFlrDsbCheck(unsigned char buf_pt,unsigned char bit_val,unsigned char sfl,unsigned char efl)
{
    unsigned char DsbFlr[4];

    DsbFlr[0]=RcvBuf[xIdPt + FLR_ON_OFF0];
    DsbFlr[1]=RcvBuf[xIdPt + FLR_ON_OFF1];
    DsbFlr[2]=RcvBuf[xIdPt + FLR_ON_OFF2];
    DsbFlr[3]=RcvBuf[xIdPt + FLR_ON_OFF3];

	for(;1;){   
	    if(DsbFlr[buf_pt] & bit_val){	
	        return(0);
	    }

		bit_val=(bit_val << 1);
		if(bit_val==0x0){
			if(buf_pt==3)	return(1);
			else{
				buf_pt++;
				bit_val=0x01;
			}
		}
		if(sfl >= efl)	return(1);		//3
		sfl++;
	}

	return(1);
}


unsigned int  SelElevUpKeyFlrDsbCheck(unsigned int id)
{
    unsigned char buf_pt,bit_val,sfl,efl;
	unsigned char t_off;
	
	if(ButtonType==0)	t_off=0;
	else				t_off=1;

	efl=(xtmptop + t_off);
	if( xtmpadr >= efl)	return(1);

	if(ButtonType==0)	t_off=1;
	else				t_off=0;

	sfl=(xtmpadr+t_off);
	buf_pt=GetMode(sfl);
	bit_val=GetBitPoint(sfl);

//	sfl=xtmpadr+t_off;

	buf_pt=UpDnKeyFlrDsbCheck(buf_pt,bit_val,sfl,efl);
	return(buf_pt);
}




unsigned int  SelElevDnKeyFlrDsbCheck(unsigned int id)
{
    unsigned char buf_pt,bit_val,sfl,efl;

	unsigned char t_off;	
	if(ButtonType==0)	t_off=0;
	else				t_off=1;	  

	if((xtmpadr+t_off) <= (xtmpstart))    return(1); 

	sfl=xtmpstart;

	if(ButtonType==0)	t_off=1;
	else				t_off=0;

	efl=(xtmpadr-t_off);

	buf_pt=GetMode(sfl);
	bit_val=GetBitPoint(sfl);

	buf_pt=UpDnKeyFlrDsbCheck(buf_pt,bit_val,sfl,efl);

	return(buf_pt);
}


/*
unsigned int  SelElevDnKeyFlrDsbCheck(unsigned int id)
{
    unsigned char buf_pt,bit_val,sfl,efl;

	unsigned char t_off;	
	if(ButtonType==0)	t_off=0;
	else				t_off=1;	  

	if((xtmpadr+t_off) <= (xtmpstart))    return(1); 



	if(ButtonType==0)	t_off=1;
	else				t_off=0;

	buf_pt=0;
	bit_val=0x01;

	efl=(xtmpadr-t_off);
	sfl=xtmpstart+t_off;

//	buf_pt=GetMode(sfl);
//	bit_val=GetBitPoint(sfl);

	buf_pt=UpDnKeyFlrDsbCheck(buf_pt,bit_val,sfl,efl);

	return(buf_pt);
}
*/


unsigned int  SelElevFlrDsbCheck(unsigned int id)
{
    unsigned char i,j,k;
    unsigned char DsbFlr[4];
    unsigned int  IdPt;

	IdPt=IsBufferPt(id); 
    
    DsbFlr[0]=RcvBuf[IdPt + FLR_ON_OFF0];
    DsbFlr[1]=RcvBuf[IdPt + FLR_ON_OFF1];
    DsbFlr[2]=RcvBuf[IdPt + FLR_ON_OFF2];
    DsbFlr[3]=RcvBuf[IdPt + FLR_ON_OFF3];



	j=(MyAddress-1);
	i=GetMode(j);
	k=GetBitPoint(j);

   
    if(!(DsbFlr[i] & k)){	
        return(1);
    }

    return(0);
}


unsigned int SelElevNoUseCheck(unsigned int id)
{
    unsigned int IdPt;

    maxtopflrbuf[id]=0x00;
    maxlowflrbuf[id]=0xff;

	IdPt=IsBufferPt(id);

    if(IsCarVip(IdPt))              return(1);
    if(IsCarManual(IdPt))           return(1);
    if(IsCarEmg(IdPt))              return(1);
    if(IsCarParking(IdPt))          return(1);
    if(IsCarFire(IdPt))             return(1); 
	if(IsCarExtButClr(IdPt))        return(1);

	
	if(IsMyFamilyService(IdPt)){
		if(id != LocalNumber)		return(1); 	//10,05
	}

	if(b_MyFamily){								//10,05
		if(id != LocalNumber)		return(1); 	//10,05
	}											//10,05


#ifdef  VIP_TEMP
    SelHostAdr=LocalNumber;    
    if(id != LocalNumber)           return(1);    
#endif


  	maxtopflrbuf[id]=RcvBuf[IdPt + SL_mTopFlr];		//10,05
  	maxlowflrbuf[id]=RcvBuf[IdPt + SL_mStartFloor];	//10,05

	return(0);
}



unsigned int SelElevDsbCheck(unsigned int id)
{
    
    if(SelElevNoUseCheck(id)){
		MoveTime[id]=0;
       	return(1);
	}


	if(IsCarMove(xIdPt)){
		MoveTime[id]=0;
	}

	if(IsYouFlrNoneWard(xtmphigh)){
		if(IsYouFlrNoneWard(xtmplow)){
			MoveTime[id]=0;
		}
	}

    if(MoveTime[id] > MAX_CALL_TIME){
		if( !IsYouFlrEqualMe(xTmpCurFloor,xtmpadr)){
	        MoveTime[id]= MAX_CALL_TIME + 1;
			return(1);
		}
    }
 
    return(0);
}



unsigned char   UpKeyTopDsbCheck(unsigned int id)
{
	unsigned char t_off;	
	if(ButtonType==0)	t_off=0;
	else				t_off=1;

	GetElevPointer(id);
    if(xtmpadr >= (xtmptop+ t_off))	return(1); 
    else               				return(0); 
}



unsigned char   DnKeyTopDsbCheck(unsigned int id)
{
	GetElevPointer(id);
    if(xtmpadr > xtmptop)    	return(1); 
    else                  		return(0); 
}



unsigned char   UpKeyLowDsbCheck(unsigned int id)
{
	GetElevPointer(id);
    if(xtmpadr < xtmpstart)   	return(1); 
    else                     	return(0); 
}


unsigned char   DnKeyLowDsbCheck(unsigned int id)
{
	unsigned char t_off;	
	if(ButtonType==0)	t_off=0;
	else				t_off=1;

	GetElevPointer(id);
    if((xtmpadr+t_off) <= (xtmpstart))    return(1); 
    else                  				  return(0); 
}



unsigned char	DsbChk(unsigned int id)
{
    if(SelElevFlrDsbCheck(id))  return(1);
    if(SelElevDsbCheck(id))     return(1);
  
    if(IsCarFull(xIdPt))        return(1);
	if(IsCarMostService(xIdPt))	return(1);

	return(0);
}




unsigned char  CarKeyCheck(unsigned char startcur,unsigned char stopcur)
{

    unsigned char i,j,k,cnt;
    unsigned char tmpcarkey[4];
    unsigned int  zz;

    if(startcur < xtmpstart)	return(0);
    if(startcur > xtmptop)		return(0);

    if(stopcur > xtmptop)		return(0);
    if(stopcur < xtmpstart)		return(0);

    if(startcur > stopcur)		return(0);
         
	cnt=0;

    tmpcarkey[0]=RcvBuf[xIdPt + SL_mCarKey1];
    tmpcarkey[1]=RcvBuf[xIdPt + SL_mCarKey9];
    tmpcarkey[2]=RcvBuf[xIdPt + SL_mCarKey17];
    tmpcarkey[3]=RcvBuf[xIdPt + SL_mCarKey25];
			
	zz=(unsigned int)(startcur);
    i=(unsigned char)(zz / 8);   //1
    j=(unsigned char)(zz % 8);   //2
    k=(0x01 << j);

	for(;startcur <= stopcur;startcur++){
		if(tmpcarkey[i] & k){
			cnt++;
		}

		k=(k<<1);
		if(k==0){
			k=0x01;
   			i++;
		}	
	}		

    return(cnt);

}



unsigned char  UpKeyCarKeyDsbCheck(void)
{
	unsigned char ret,lfl,tfl;

	ret=1;
    if(IsCarUpWard(xIdPt)){
		if(xtmpadr > xTmpCurFloor){
			tfl = xtmpadr-1;
			lfl = xTmpCurFloor + 1;
			if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;				
		}
		else if(xtmpadr < xTmpCurFloor){
			tfl = xtmptop;
			lfl = xTmpCurFloor + 1;
			if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;				
		}
	}
    else if(IsCarDnWard(xIdPt)){
		tfl=xTmpCurFloor-1;
		lfl=xtmpstart;			

		if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;				
	}
	return(ret);
}


unsigned char  DnKeyCarKeyDsbCheck(void)
{
	unsigned char ret,lfl,tfl;

	ret=1;
    if(IsCarUpWard(xIdPt)){
		lfl=xTmpCurFloor+1;
		tfl=xtmptop;			

		if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;				
	}

    else if(IsCarDnWard(xIdPt)){
		if(xtmpadr < xTmpCurFloor){
			tfl = xTmpCurFloor-1;
			lfl = xtmpadr + 1;			
			if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;  				
		}
		else if(xtmpadr > xTmpCurFloor){
			tfl=xTmpCurFloor-1;
			lfl=xtmpstart;			

			if(CarKeyCheck(lfl,tfl) >= 3)	ret=0;				
		}
	}
    return(ret); 
}




unsigned char  UpKeyReadyChk(unsigned int id)
{
    unsigned char  ret;
    ret=1;
	GetElevPointer(id);
	if(UpKeyCarKeyDsbCheck() == 0)	return(0);			
    return(ret); 
}



unsigned char  DnKeyReadyChk(unsigned int id)
{
    unsigned char  ret;
    ret=1;
	GetElevPointer(id);
	if(DnKeyCarKeyDsbCheck() == 0)						return(0);
    return(ret); 
}



void	CarCurStatus(unsigned int id)
{
	
	Equal[id]=100;

	if(IsYouFlrEqualMe(xyoufloor,xtmpadr))				Equal[id]=0;		
	else if(IsYouFlrEqualMe(xyoufloor,xTmpCurFloor)){
 		if(IsCarMove(xIdPt))		Equal[id]=3;		
		else{
			if(IsCarClose(xIdPt))	Equal[id]=1;
			else					Equal[id]=2;	
		}
	}
	else{
 		Equal[id]=9;		
	}
}






unsigned char  UpKeyCalcElev(unsigned int id)
{
    unsigned char ret=0;

    UpKeyCalu[id] = 0xff;

	GetElevPointer(id);


	if(DsbChk(id))	return(1);

    if(ButtonType==0){
        if(UpKeyTopDsbCheck(id))    	return(1);
        if(UpKeyLowDsbCheck(id))    	return(1);
		if(SelElevUpKeyFlrDsbCheck(id))	return(1);
    }        


	if(UpKeyReadyChk(id)==0){
		UpKeyCalu[id] = 100;
		return(1);
	}

	CarCurStatus(id);

    ret=1;        
    if(Up_Key_Valid || UP_KEY_LAMP){
		ret=0;

                                                 					
        if(xtmpadr < xTmpCurFloor){            
            if(IsCarDnWard(xIdPt)){
				if(IsYouDn(xyoufloor)){
						UpKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);                     		  
				}
				else{
					if(IsYouFlrLowMe(xyoufloor,xtmpadr)){
						UpKeyCalu[id] = CalCulate(xTmpCurFloor,(xyoufloor & ONLY_FLR),xtmpadr,(xyoufloor & ONLY_FLR),THIRD_OFFSET);        
						if(!IsYouFlrLowMe( ((xyoufloor & ONLY_FLR)+ CallOffset),xtmpadr)){
							UpKeyCalu[id] = CalCulate(xtmpadr,(xyoufloor & ONLY_FLR),0,0,FIRST_OFFSET);    //test
						}
					}
					else{
						UpKeyCalu[id]= CalCulate(0,0,xTmpCurFloor,xtmpadr,FIRST_OFFSET);
					}
				}
            }            
            else if(IsCarUpWard(xIdPt)){
				UpKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);
            }            
            else{
				UpKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpadr,0,0,FIRST_OFFSET);                     		  
            }
        }
        else if(xtmpadr > xTmpCurFloor){
            if(IsCarUpWard(xIdPt)){
				UpKeyCalu[id] = CalCulate(xtmpadr,xTmpCurFloor,0,0,FIRST_OFFSET);
			}
            else if(IsCarDnWard(xIdPt)){
				UpKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);
            }            
            else{
				UpKeyCalu[id] = CalCulate(xtmpadr,xTmpCurFloor,0,0,FIRST_OFFSET);  
            }
        } 
        else{
            if(IsCarUpWard(xIdPt)){
                if(IsCarMainOpen(xIdPt) && !bIamXSubDoor){
                    UpKeyCalu[id] = 0;
                }
                else if(IsCarSubOpen(xIdPt) && bIamXSubDoor){
                    UpKeyCalu[id] = 0;
                }
                else if(IsYouFlrEqualMe(xdestflr,xtmpadr)){
					if(IsYouDn(xdestflr)){					
						UpKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);                     		  
					}  
					else{
                    	UpKeyCalu[id] = 1;  
					}
                }
				else{
					UpKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);                     		  
				}

            }
            else if(IsCarDnWard(xIdPt)){
                if(IsYouFlrEqualMe(xdestflr,xtmpadr)){
	                if(IsYouUp(xdestflr)){
	                    UpKeyCalu[id] = 1;  
	                }
                	else{
						UpKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);                     		  
					}
				}
                else{
					UpKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);                     		  
                }
            }
            else{    
                UpKeyCalu[id] = 2; 
            }
		}
    }

    return(ret); 
}




unsigned char  DnKeyCalcElev(unsigned int id)
{

    unsigned char  ret=0;

	GetElevPointer(id);

    DnKeyCalu[id] = 0xff;

	if(DsbChk(id))	return(1);


    if(ButtonType==0){
        if(DnKeyTopDsbCheck(id))    	return(1);
        if(DnKeyLowDsbCheck(id))    	return(1);
		if(SelElevDnKeyFlrDsbCheck(id))	return(1);
    }        


	if(DnKeyReadyChk(id)==0){
		DnKeyCalu[id] = 100;
		return(1);
	}

	CarCurStatus(id);

    ret=1;

    if(Dn_Key_Valid || DN_KEY_LAMP){
        ret=0;                        
        if(xtmpadr < xTmpCurFloor){
            if(IsCarUpWard(xIdPt)){
				DnKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);
			}
            else if(IsCarDnWard(xIdPt)){
				DnKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpadr,0,0,FIRST_OFFSET);
            }            
            else{				
				DnKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpadr,0,0,FIRST_OFFSET);                      		  
            }
        }
        else if(xtmpadr > xTmpCurFloor){
            if(IsCarUpWard(xIdPt)){
				if(IsYouUp(xyoufloor)){
					DnKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);                     		  
				}
				else{
					if(IsYouFlrHighMe(xyoufloor,xtmpadr)){
						DnKeyCalu[id] = CalCulate((xyoufloor & ONLY_FLR),xTmpCurFloor,(xyoufloor & ONLY_FLR),xtmpadr,THIRD_OFFSET);    
						if(!IsYouFlrHighMe(xyoufloor,xtmpadr+CallOffset)){
							DnKeyCalu[id] = CalCulate((xyoufloor & ONLY_FLR),xtmpadr,0,0,FIRST_OFFSET);    //test
						}
					}
					else{
						DnKeyCalu[id] = CalCulate(xtmpadr,xTmpCurFloor,0,0,FIRST_OFFSET);                     		  
					}
				}
            }            
            else if(IsCarDnWard(xIdPt)){
				DnKeyCalu[id] = CalCulate(xTmpCurFloor,(xyoufloor & ONLY_FLR),xtmptop,(xyoufloor & ONLY_FLR),THIRD_OFFSET);                     		  
            }            
            else{
				DnKeyCalu[id] = CalCulate(xtmpadr,xTmpCurFloor,0,0,FIRST_OFFSET);                    		  
            }
        } 
        else{
            if(IsCarDnWard(xIdPt)){
                if(IsCarMainOpen(xIdPt) && !bIamXSubDoor){
                    DnKeyCalu[id] = 0;   
                }
                else if(IsCarSubOpen(xIdPt) && bIamXSubDoor){
                    DnKeyCalu[id] = 0;  
                }
                else if(IsYouFlrEqualMe(xdestflr,xtmpadr)){
					if(IsYouUp(xdestflr)){					
						DnKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);                     		  
					}
					else{
                    	DnKeyCalu[id] = 1;  
					}
				}
				else{
					DnKeyCalu[id] = CalCulate(xTmpCurFloor,xtmpstart,xtmpadr,xtmpstart,THIRD_OFFSET);                     		  
				}	
            }
            else if(IsCarUpWard(xIdPt)){    
                if(IsYouFlrEqualMe(xdestflr,xtmpadr)){
	                if(IsYouDn(xdestflr)){
	                    DnKeyCalu[id] = 1;  
	                }
					else{	
						DnKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);                     		  
					}
				}
                else{
					DnKeyCalu[id] = CalCulate(xtmptop,xTmpCurFloor,xtmptop,xtmpadr,THIRD_OFFSET);                     		  
                }
            }
            else{    
                DnKeyCalu[id] = 2;  
            }
        }
    }

    return(ret); 
}


void	MoveTimeClear(void)
{
	unsigned char i;
    for(i=0;i<MAX_ELEV;i++){
		MoveTime[i]=0;
	}
}


////////////////////


void	UpDownKeyLoad(void)
{
	unsigned char i,j,value_bit=0x01;
        
	UpKeyBit=0;
    DnKeyBit=0;

	UP_KEY_LAMP=0;
	Up_Key_Valid=0;

	DN_KEY_LAMP=0;
	Dn_Key_Valid=0;


/*
	do{
*/

		i=((MyAddress-1) / 8);
		j=((MyAddress-1) % 8);

		value_bit =(value_bit << j);
	
		if(HomeCallUpKey[i] & value_bit){
	        UpKeyBit=1;
			Up_Key_Valid=1;
		} 
		if(HomeCallDnKey[i] & value_bit){
	        DnKeyBit=1;
			Dn_Key_Valid=1;
		}
 
/*
		if( !UpKeyBit && !DnKeyBit){
			MyAddress++;
			if(MyAddress > 31){
				MyAddress =1;
				break;
			}  
		} 
	}
	while( !UpKeyBit && !DnKeyBit);
*/
 
}



void	ArriveUpDnButtonLampClear(unsigned char updn_arv)
{
	unsigned char i,j,value_bit=0x01;


	i=((MyAddress-1) / 8);
	j=((MyAddress-1) % 8);

	value_bit =(value_bit << j);
	value_bit = ~value_bit;


	if(updn_arv==1)			HomeCallUpKey[i]=(HomeCallUpKey[i] & value_bit); 
	else if(updn_arv==2)	HomeCallDnKey[i]=(HomeCallDnKey[i] & value_bit); 

}



////////////////////////////

void	DownButtonLampOn(void)
{
	DN_KEY_LAMP=1;  
	Dn_Key_Valid=1;
}

void	DownButtonLampClear(void)
{
	DN_KEY_LAMP=0;  
	Dn_Key_Valid=0;
}


void	UpButtonLampOn(void)
{
	UP_KEY_LAMP=1;
	Up_Key_Valid=1;
}

void	UpButtonLampClear(void)
{
	UP_KEY_LAMP=0;
	Up_Key_Valid=0;
}


void	UpLampClearButtonClear(void)
{
	UpButtonLampClear();
	Up_Key_Clear=1;
	Can1Tx1ConfirmCnt=3;    
}


void	UpLampOnButtonOn(void)
{
	
	UpButtonLampOn();
	Up_Key_Clear=0;
	Can1Tx1ConfirmCnt=3;    
}



void	DnLampClearButtonClear(void)
{
	DownButtonLampClear();
	Dn_Key_Clear=1;
	Can1Tx0ConfirmCnt=3;    

}


void	DnLampOnButtonOn(void)
{
	DownButtonLampOn();
	Dn_Key_Clear=0;
	Can1Tx0ConfirmCnt=3;    
}


void	DownButtonClear(void)
{
	DownButtonLampClear();
 
//	DnButtonTime=0;
	Dn_Key_Clear=0;
	Can1Tx0ConfirmCnt=0;    
	TogDnKeyBitSet=0;
	TogDnKeyBitReset=0;

	MoveTimeClear();

}



void	UpButtonClear(void)
{

	UpButtonLampClear();

//	UpButtonTime=0;
	Up_Key_Clear=0;
	Can1Tx1ConfirmCnt=0;    
	TogUpKeyBitReset=0;
	TogUpKeyBitSet=0;
	MoveTimeClear();
}



void    UpDownKey(void)
{

    unsigned  char  i,upbt,dnbt;
    unsigned  char  valid;
    unsigned  int   IdPt; 


	IdPt=IsBufferPt(LocalNumber);
	if(IsMyFamilyService(IdPt))	b_MyFamily=1;
	else						b_MyFamily=0;



    valid=1;

    i=0;

	maxtopflr =0x0;
	maxlowflr =0xff;
    

	upbt=0;
	dnbt=0;		


    for(i=0;i<MAX_ELEV;i++){
        if(!SelElevNoUseCheck(i)){
            if(!SelElevFlrDsbCheck(i)){
                valid=0;
				GetElevPointer(i);
			    if(!UpKeyTopDsbCheck(i) && !UpKeyLowDsbCheck(i) && !SelElevUpKeyFlrDsbCheck(i))	upbt=1;
				if(!DnKeyTopDsbCheck(i) && !DnKeyLowDsbCheck(i) && !SelElevDnKeyFlrDsbCheck(i))	dnbt=1;
            }
        }   
		else{
			MoveTime[i]=0;
		}

		if(maxtopflr < maxtopflrbuf[i])	maxtopflr = maxtopflrbuf[i];	 
		if(maxlowflr > maxlowflrbuf[i])	maxlowflr = maxlowflrbuf[i];	 
    }


	if(upbt==0){
		UpButtonClear();
	}		
	if(dnbt==0){
		DownButtonClear();
	}		




	if(valid == 0){
		valid = 1;
	    for(i=0;i<MAX_ELEV;i++){
	    	if(KeyCancleCheck(i)) valid=0;  
	    }
	}


    if((valid == 0) && ((MyAddress-1) <= maxtopflr)){   
        if(UpKeyBit && upbt){
//			if((UpButtonTime<BUT_CANLE_T) && (UpButtonTime > 0) ){		
			if(UpButtonTime< 200){		
				if(ButtonType==0)	i=0; //abc
				else				i=1; //abc

                if((MyAddress-1) < (maxtopflr + i)){
					if(bToggleOn){					
	                    if(TogUpKeyBitSet){
	                        if(!UP_KEY_LAMP){                
								UpLampOnButtonOn();
	                        }
	                    }                    
	                    else if(TogUpKeyBitReset){
							UpLampClearButtonClear();
	                    }
	                    else if(!UP_KEY_LAMP){
							UpLampOnButtonOn();
	                        TogUpKeyBitSet=1;
	                    }
	                    else{                
							UpLampClearButtonClear();
	                        TogUpKeyBitReset=1;
	                    }
					}	
					else{
	                    if(!UP_KEY_LAMP){
							UpLampOnButtonOn();
	                    }
					}
                }
            }
        }
        else{
//            UpButtonTime=0;
            TogUpKeyBitSet=0;
            TogUpKeyBitReset=0;
        }

               
        if(DnKeyBit && dnbt){
            if(DnButtonTime<200){
//            if((DnButtonTime<BUT_CANLE_T) && (DnButtonTime > 0) ){
                if(((MyAddress-1) > maxlowflr) || (ButtonType > 0)){
                    if(ButtonType==0){
						if(MyAddress > 1){
							if(bToggleOn){
		                        if(TogDnKeyBitSet){
		                            if(!DN_KEY_LAMP){
										DnLampOnButtonOn();
		                            }
		                        }                    
		                        else if(TogDnKeyBitReset){
									DnLampClearButtonClear();
		                        }
		                        else if(!DN_KEY_LAMP){
									DnLampOnButtonOn();
		                            TogDnKeyBitSet=1;
		                        }
		                        else{                
									DnLampClearButtonClear();
		                            TogDnKeyBitReset=1;
		                        }
							}
							else{
		                        if(!DN_KEY_LAMP){
									DnLampOnButtonOn();
		                        }
							}                    

						}
                    }
                    else{
                        if(!DnKeyOkCheck(LocalNumber)){
							DnLampOnButtonOn();
                        }
                        else{
							DownButtonLampClear();
                            Dn_Key_Clear=0;
							Can1Tx0ConfirmCnt=0;

                        }
                    }
                }
            }
        }
            
        else{
 //           DnButtonTime=0;
            TogDnKeyBitSet=0;
            TogDnKeyBitReset=0;

			if( !bToggleOn){
            	if(ButtonType){
					DownButtonLampClear();
                	Dn_Key_Clear=0;
					Can1Tx0ConfirmCnt=0;
            	}                        
			}	
        }
    }
    else{
		DownButtonClear();
		UpButtonClear();
    }      
}


/*
////////////////////////////////
void    UpDownBlink(unsigned char id)
{

	unsigned int IdPt;
	unsigned char destfl,destfl1;

	IdPt=IsBufferPt(id);

	destfl=RcvBuf[IdPt+DEST_FLR];
	destfl1=(destfl & ONLY_FLR);

	if(bUserLamp4){
		if(IsHallLanternUpLamp(IdPt)){
			HallLampUpBit=1;
	        bHALL_LAMP_UP=1;   
		}
		else{
			HallLampUpBit=0;
	        bHALL_LAMP_UP=0;   
		}
	
		if(IsHallLanternDnLamp(IdPt)){
			HallLampDnBit=1;
		    HALL_LAMP_DN=1;   
		}
		else{
			HallLampDnBit=0;
		    HALL_LAMP_DN=0;   
		}
	}
	else{
	    BlinkOn=0;
	    HallLampUpBit=0;
	    HallLampDnBit=0;
	
	
	    if((destfl1  == (MyAddress-1)) && (CurFloor  == MyAddress) ){
			if((UpMove || DnMove) && Auto){			
	        	if(Open){
	             	if(UpMove){
	             		HallLampUpBit=1;
	                	BlinkOn=1;  
	             	}                
	             	else{
	             		HallLampDnBit=1;
	                	BlinkOn=1;  
	        		}    				
				}
				else{
					destfl=RcvBuf[IdPt+mEqualFloor];
	  				if(IsYouUp(destfl)){
	                     HallLampUpBit=1;
	                     BlinkOn=1;  
	                 }                
	  				 else if(IsYouDn(destfl)){
	                     HallLampDnBit=1;
	                     BlinkOn=1;  
	                 }        
	        	}
			}
	    }


	    if(BlinkOn){                        
	        BlinkTime=BlinkTime%10;
	        if(HallLampUpBit){
	            if((BlinkTime >= 5))    bHALL_LAMP_UP=0;      
	            else                  	bHALL_LAMP_UP=1;
	   
				HALL_LAMP_DN=0;
				if(DnHallElev == id)	HALL_LAMP_DN=1;				
	
	        }
	        else if(HallLampDnBit){
	            if((BlinkTime >= 5))    HALL_LAMP_DN=0;      
	            else                  	HALL_LAMP_DN=1;
	
	   			bHALL_LAMP_UP=0;
				if(UpHallElev == id)	bHALL_LAMP_UP=1;
	        }
	    }
	
	    else{
	
			if((UpHallElev == id) && (DnHallElev == id)){
		    	if( (CurFloor  < MyAddress) )	HallLampUpBit=1;
		    	if( (CurFloor  > MyAddress) )	HallLampDnBit=1;
	        }
			else if(UpHallElev == id){
	            HallLampUpBit=1;
	        }
	        else if(DnHallElev == id){
	            HallLampDnBit=1;
	        }				
	
			if(HallLampUpBit){
	            bHALL_LAMP_UP=1;   
	            HALL_LAMP_DN=0;
	        }
	        else if(HallLampDnBit){
	            HALL_LAMP_DN=1;   
	            bHALL_LAMP_UP=0;
	        }
	        else{
	            bHALL_LAMP_UP=0;
	            HALL_LAMP_DN=0;
	        }
	        BlinkTime=0;
	    }
	}

#if defined(__TYPE_ES15)
	HALL_LAMP_UP = bHALL_LAMP_UP;
#endif

}

*/




//////////////////////////////////////////////
unsigned char TmpVip(void)
{
    b_vip=0;

#ifdef  VIP_TEMP
    if(!Vip && (Up_Key_Valid || Dn_Key_Valid)){        
        b_vip=1;
        return(1);
    }
#endif
    return(0);            
}



void DoorOpenCheck(unsigned char i)
{

	for(MyAddress=1;MyAddress<32;MyAddress++){
		GetElevPointer((unsigned int)i);
	
	    if(IsYouFlrEqualMe(xTmpCurFloor,xtmpadr) && IsCarAuto(xIdPt)){
		    if( (!bIamXSubDoor && IsCarMainOpen(xIdPt)) || (bIamXSubDoor && IsCarSubOpen(xIdPt)) ){
				if( IsCarUpWard(xIdPt) || (ButtonType)){
		            if(!TmpVip()){			
						UpButtonLampClear();
						Can1Tx0ConfirmCnt=0;
		            }
					ArriveUpDnButtonLampClear(1);
				}
				if(IsCarDnWard(xIdPt)){
		            if(!TmpVip()){        
						DownButtonLampClear();
						Can1Tx0ConfirmCnt=0;
		            }
					ArriveUpDnButtonLampClear(2);
				}
		    }
	    }
	}	
}




void    ButtonLampClrChk(void)
{

#ifdef  VIP_TEMP
       DoorOpenCheck(LocalNumber);
#else            
    unsigned char i;

    for(i=0;i<MAX_ELEV;i++){ 
       DoorOpenCheck(i);
    }
#endif

}                  








