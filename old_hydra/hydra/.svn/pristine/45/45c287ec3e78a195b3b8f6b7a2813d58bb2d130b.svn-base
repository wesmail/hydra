// File: htboxchan.cc
// created 2/3/00 RMS 
//

#include "htboxchan.h"

ClassImp(HTBoxChan)

HTBoxChan::HTBoxChan(void) {
	for(int i=0;i<SCALER_MAX;i++){
		scaler[i]=0;
	}
}
void HTBoxChan::clear(void){
	chanNum=0; 
	for(int i=0;i<SCALER_MAX;i++){
		 scaler[i]=0;
	}
}

void HTBoxChan::set(Int_t i0,Int_t i1,Int_t i2,Int_t i3,Int_t i4,Int_t nScalerNr){
	chanNum=i0;
	scaler[0+nScalerNr*4]=i1;
	scaler[1+nScalerNr*4]=i2;
	scaler[2+nScalerNr*4]=i3;
	scaler[3+nScalerNr*4]=i4;

}

void HTBoxChan::get(Int_t& i0,Int_t& i1,Int_t& i2,Int_t& i3,Int_t& i4){
	i0=chanNum;
	i1=scaler[0];
	i2=scaler[1];
	i3=scaler[2];
	i4=scaler[3];
}

void HTBoxChan::print(){

	printf("TBoxCh %2.1d:\n",chanNum);
	for(int i=0;i<SCALER_MAX;i++){
		printf(" %8.1d",scaler[i]);
	}
	printf("\n");
}


