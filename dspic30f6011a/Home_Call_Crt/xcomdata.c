


#include <p30f6011a.h>            

#include        "xiodef.h"
//////////////////#include        "fontout.h"
#include        "xesdots.h"
#include        "xkeysort.h"
#include        "xsetup.h"
#include        "memory_map.h"
//#include        "..\can_4480\you_can.h"

#include  		"WorkingRam.h"







unsigned char DnKeyOkCheck(unsigned char id)
{

    if((ButtonType==0) && !DN_KEY_LAMP) return(0);
    else if(ButtonType==0)              return(1);


	GetElevPointer((unsigned int)id);


    if(IsYouFlrEqualMe(xTmpCurFloor,xtmpadr)){
        if(IsCarAuto(xIdPt)){
            if(bIamXSubDoor && IsCarSubOpen(xIdPt)){
                return(0);
            }
            else if( !bIamXSubDoor && IsCarMainOpen(xIdPt)){
                return(0);
            }
    	}
 	}

    return(1);
}        


unsigned char LoadBefCheck(unsigned char id,unsigned char UpDnKey)        
{

    unsigned char i;


	GetElevPointer((unsigned int)id);


    i=2;

    CanCmd=CAN_KEY_SET;

    if(UpDnKey){
        CanKeyValue[1] = (xtmpadr | UP_READY);
        if(ButtonType)   	CanKeyValue[1] = (xtmpadr | CAR_READY);
    }
    else        CanKeyValue[1] = (xtmpadr | DN_READY);


	
    if(bIamXSubDoor)  	CanKeyValue[2] = (CanKeyValue[2] | SUB_DOOR_BIT);
    else            	CanKeyValue[2] = (CanKeyValue[2] & ~SUB_DOOR_BIT);

    
    if(xTmpCurFloor < xtmpadr){
        if(UpDnKey){
            if(IsCarDnWard(xIdPt))                              i=1;
            else if(IsYouFlrNoneWard(xtmphigh))           		i=0;    
            else if(IsYouFlrHighMe(xtmphigh,xtmpadr))  			i=0;    
            else if(IsYouFlrLowMe(xtmphigh,xtmpadr)){
                if(IsYouOnlyDn(xtmphigh))                   	i=0;
                else                                            i=1;
            }
            else if(IsYouFlrEqualMe(xtmphigh,xtmpadr)){                
                if(!IsYouUp(xtmphigh))                      	i=0;
                else                                            i=2;
            }
        }
        else{
            if(IsCarDnWard(xIdPt))                              i=1;
            else if(IsYouFlrNoneWard(xtmphigh))           		i=0;                
            else if(IsYouFlrHighMe(xtmphigh,xtmpadr))  			i=1;
            else if(IsYouFlrLowMe(xtmphigh,xtmpadr)){
                if(IsYouOnlyDn(xtmphigh))                   	i=0;
                else                                            i=1;
            }
            else if(IsYouFlrEqualMe(xtmphigh,xtmpadr)){
                if(IsYouOnlyCar(xtmphigh))                  	i=0;
                else                                            i=2;
            }
        }
    }
    else if(xTmpCurFloor > xtmpadr){
        if(UpDnKey){
            if(IsCarUpWard(xIdPt))                              i=1;
            else if(IsYouFlrNoneWard(xtmplow))            		i=0;    
            else if(IsYouFlrLowMe(xtmplow,xtmpadr)){
                if(ButtonType==2)   i=0;
                else                i=1;    
            }
            else if(IsYouFlrHighMe(xtmplow,xtmpadr)){
                if(IsYouOnlyUp(xtmplow))                    	i=0;
                else                                            i=1;
            }
            else if(IsYouFlrEqualMe(xtmplow,xtmpadr)){
                if(IsYouOnlyCar(xtmplow))                   	i=0;
                else                                            i=2;
            }
        }
        else{
            if(IsCarUpWard(xIdPt))                              i=1;
            else if(IsYouFlrNoneWard(xtmplow))            		i=0;
    
            else if(IsYouFlrLowMe(xtmplow,xtmpadr))    			i=0;
    
            else if(IsYouFlrHighMe(xtmplow,xtmpadr)){
                if(IsYouOnlyUp(xtmplow))                    	i=0;
                else                                            i=1;
            }
            else if(IsYouFlrEqualMe(xtmplow,xtmpadr)){
                if(!IsYouDn(xtmplow))                       	i=0;
                else                                            i=2;
            }
        }
    }
    else{
        if(UpDnKey){
            if(IsCarUpWard(xIdPt))      						i=0;
			if(!bToggleOn){
		    	if(ButtonType){
		        	if(IsCarDnWard(xIdPt))       					i=0;
		        }
			}
        }
        else{
            if(IsCarDnWard(xIdPt))           					i=0;
        }
    }

    
    if(IsCarNoneWard(xIdPt))    								i=0;


    return(i);            
}







int UpKeyLoad(unsigned char Curid)
{
    unsigned int i=1;
    unsigned char id;


    UpHallElev=0xff;
	SelHostAdr=0x0;

/*
    if(b_vip){
        CanCmd=CAN_VIP_COMMAND;
        CanKeyValue[1]= ((MyAddress-1) | CAR_READY);
        return(0);
    }
*/

//    if(!Up_Key_Valid)   return(1);
    

    for(id=0;id<MAX_ELEV;id++){
        UpKeyCalu[id] = 0xff;
        if(!UpKeyCalcElev(id)){
 			i=0;        
		}
    }
  

	SelHostAdr=0x0;
	for(id=0;id<MAX_ELEV;id++){			
		if(UpKeyCalu[SelHostAdr] > UpKeyCalu[id])	SelHostAdr=id;   
		else if(UpKeyCalu[SelHostAdr] == UpKeyCalu[id]){
			if(	Equal[SelHostAdr] >	Equal[id]){		
				SelHostAdr=id;	
			}			
		}
	}
	

    if(i==0){
		UpHallElev=SelHostAdr;
	    if(!Up_Key_Valid)   return(1);

        i=LoadBefCheck(SelHostAdr,1);                      

/*
        if(i==0)        CanCmd=CAN_KEY_SET;

        else if(i==1){
   			i=UpKeyReadyChk(SelHostAdr);
			if(i==1){
				CanCmd=CAN_KEY_READY;
				i=0;
			}
		}
*/			
    }

    return(i);
}



int DnKeyLoad(unsigned char Curid)
{

    unsigned int i=1;
    unsigned char id;

    DnHallElev=0xff;


    if(b_vip){
        CanCmd=CAN_VIP_COMMAND;
        CanKeyValue[1]= ((MyAddress-1) | CAR_READY);
        return(0);
    }


 //   if(!Dn_Key_Valid)	   return(1);

 
    for(id=0;id<MAX_ELEV;id++){
        DnKeyCalu[id] = 0xff;
        if(!DnKeyCalcElev(id)){
  			i=0;
		}
    }



	SelHostAdr=0x0;
	
	for(id=0;id<MAX_ELEV;id++){			
		if(DnKeyCalu[SelHostAdr] > DnKeyCalu[id])     SelHostAdr=id;		
		else if(DnKeyCalu[SelHostAdr] == DnKeyCalu[id]){
			if(	Equal[SelHostAdr] >	Equal[id]){		
				SelHostAdr=id;	
			}			
		}
	}



    if(i==0){
		DnHallElev=SelHostAdr;

	    if(!Dn_Key_Valid)	   return(1);
		 
        i=LoadBefCheck(SelHostAdr,0);             
/*
        if(i==0)        CanCmd=CAN_KEY_SET;

        else if(i==1){
   			i=DnKeyReadyChk(SelHostAdr);
			if(i==1){
				CanCmd=CAN_KEY_READY;
				i=0;
			}
		}
*/
    }
    return(i);
}

unsigned char KeyCancleCheck(unsigned char id)
{
    unsigned char i;

    i=1;

	GetElevPointer((unsigned int)id);

	if( !bToggleOn){
		if(ButtonType==1){
	        if(IsCarMove(xIdPt)){			
	            if(IsYouFlrEqualMe(xdestflr,xtmpadr))   	i=1;
	            else                                		i=0;
	        }
	        else{
	            if(IsYouFlrEqualMe(xTmpCurFloor,xtmpadr))   i=1;
	            else{
	                if(IsCarClose(xIdPt))                   i=1;
	                else                                  	i=0;
	            }            
	        }
	    }
	}	

    return(i);
}        

