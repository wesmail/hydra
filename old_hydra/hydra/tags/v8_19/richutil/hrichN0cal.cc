
// File: hrichN0cal.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/10/10 13:11:25
//
#include "hrichN0cal.h"
#include "hmatrixcatiter.h"
#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "richdef.h"
#include "hrichlocalmaxheader.h"
#include "hrichlocal.h"
ClassImp(HRichN0Cal);

HRichN0Cal::HRichN0Cal(Text_t *name, Text_t *title,Text_t *outName,Char_t *typ):
HReconstructor(name, title) {

    outFile = new TFile(outName,"RECREATE");
    for(Int_t i =0;i<6;i++){
	
	    countEvt[i]=0;
    }

    Char_t hname3[40];
    Char_t hname4[40];
    Char_t hname1[40];

    histo = new TH1F**[6];
    for (Int_t i =0; i< 6 ; i++){
        histo[i] = new TH1F*[2];
	secFlag[i] = 0;
    }
    for (Int_t i =0; i< 6 ; i++){
	//sprintf(hname3,"N0SiO2Sec%dSim",i);
	sprintf(hname3,"N0SiO2Sec%d",i);
	strcat(hname3,typ);
	sprintf(hname4,"N0MgF2Sec%d",i);
	//sprintf(hname4,"N0MgF2Sec%dSim",i);
	strcat(hname4,typ);
	histo[i][0]=new TH1F(hname3,hname3,50,120,250);
	histo[i][1]=new TH1F(hname4,hname4,50,120,250);
    }
    thetaH = new TH1F*[6];
    //multiplicity cuts on the different sectors
    for (Int_t i =0; i< 6 ; i++){
	sprintf(hname1,"ThetaSec%d",i);
	thetaH[i]  =new TH1F(hname1,hname1,100,0.5,100.5);
    }
    slopeBG[0] = 1.5;
    offSetBG[0] = -10.8;
    slopeBG[1]= 1.4;
    offSetBG[1]= -5.5;
    slopeBG[2]= 1.26;
    offSetBG[2]= -6.38;
    slopeBG[5] = 1.8;
    offSetBG[5]= -16.;

     cutSecD[0] = 45;// apr02 exp
    cutSecD[1] = 44;// apr02 exp
    cutSecD[2] = 48;// apr02 exp
    cutSecD[5]  =60;// apr02 exp


    //cutSecD[0] = 0; // 
    // cutSecD[1] = 0; //
    //cutSecD[2] = 0; //
    cutSecD[3] = 0;//
    cutSecD[4] = 0;//
    //cutSecD[5] = 0;//

    //cutSecD[4] = -1;//nov00
    //cutSecD[0] = -1;//nov00
    //  cutSecU[0] = 103;// apr02 exp
    //cutSecU[1] = 105;// apr02 exp
    //c/utSecU[2] = 109;// apr02 exp
   
    //  cutSecU[5] = 136;// apr02 exp

    // cutSecU[0] = 160;// aug01 exp
    //cutSecU[1] = 160;// aug01 exp
    //cutSecU[2] = 160;// aug01 exp

          cutSecU[0] = 300;
       cutSecU[1] = 300;
       cutSecU[2] = 300;
       cutSecU[3] = 300; 
       cutSecU[4] = 300; 
       cutSecU[5] = 300;
   

       //ring1ThMin = 35; //apr02 exp
       //ring1ThMax = 55;//apr02 exp
       ring1ThMin = 32;//nov00 exp
       ring1ThMax = 52;//nov00 exp
       ring2ThMin = 52;//nov00 exp
       //       ring2ThMin = 55;//apr02 exp
       ring2ThMax = 90;//apr02 exp
    secOk = 0;
}
HRichN0Cal::~HRichN0Cal(){
    delete outFile;
    delete thetaH;
    for(Int_t i =0; i< 6 ; i++){
	delete histo[i][0];
	delete histo[i][1];
    }
    for(Int_t i =0; i< 6 ; i++) delete histo[i];
    delete histo; 
}
Bool_t HRichN0Cal::init(){
 printf("initialization of rich photon\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup()
	-> getDetector("Rich");

    fLocalCatHr=gHades->getCurrentEvent()->getCategory(catRichLocalMaxHeader);
    if (!fLocalCatHr) {
      fLocalCatHr=pRichDet->buildCategory(catRichLocalMaxHeader);

      if (!fLocalCatHr) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichLocalMaxHeader,fLocalCatHr, "Rich");
    }
    fIter2 = (HIterator*) getLocalHrCat() -> MakeIterator();
   
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal, 
							fLocalCat, "Rich");
    }
    fIter1 = (HIterator*) getLocalCat() -> MakeIterator();

    return 0;
}

Int_t HRichN0Cal::execute(){
    HRichLocalMaxHeader *localHr = NULL;
    Float_t multSec[6];
    for (Int_t i =0; i< 6 ; i++){
	secFlag[i] = 0;
	multSec[i]=0;
    }
    fIter2 -> Reset();
    while ((localHr=(HRichLocalMaxHeader*)fIter2 -> Next())) {
	localHr->getSecMult(multSec);
	
    }
    //cout<<" multSec "<<multSec[0]<<"  "<<multSec[1]<<"  "<<multSec[2]<<"  "<<multSec[3]<<"  "<<multSec[4]<<"  "<<multSec[5]<<"  "<<endl;
    secOk = 0;
    for(Int_t i = 0;i<6;i++){
	secOk +=filterSector(i,multSec[i]);
	//cout<<" sector flag "<<secFlag[i]<<endl;
    }
    if(secOk==6&&(fLocalCat->getEntries())!=0){
	for(Int_t i = 0;i<6;i++){
	    countEvt[i]++;
	}
    }
    else return kSkipEvent;
    //    cout<<"secOK  "<<secOk<<endl;
    HRichLocal *local = NULL;
    fIter1 -> Reset();
    while ((local=(HRichLocal*)fIter1 -> Next())) {
	//if (secFlag[local->getSector()]){
	if (secOk==6){
	    Int_t sec =local->getSector();
	    Float_t theta =local->getLocalTheta();	
	    Float_t energy =  local->getLocalEnergy();
	    Int_t t = (Int_t)theta;
			
		thetaH[sec]->Fill(t);
		//cout<<" energy "<<energy<<endl;
		//	    cout<<" theta "<<t<<endl;
		if(energy){ 
		    Float_t lambda = 1239/energy;  
		    //cout<<" lambda "<<lambda<<endl;
		    if(theta>ring1ThMin&&theta<ring1ThMax) {
			//  cout<<" sec "<<local->getSector()<<endl;
			histo[local->getSector()][1]->Fill(lambda);
			// countEvt[local->getSector()][1]++;
			//cout<<" mgF2 how many evt  "<<countEvt[local->getSector()][1]<<endl;
		    }
		    else if(theta>ring2ThMin&&theta<ring2ThMax) {
			//cout<<" sec "<<local->getSector()<<endl;
			histo[local->getSector()][0]->Fill(lambda);
			// countEvt[local->getSector()][0]++;
			//cout<<" siO2 how many evt  "<<countEvt[local->getSector()][0]<<endl;
		    }
		}
	
	}
	
    }
    return 0;
}
Int_t HRichN0Cal::filterSector(Int_t sec,Float_t multSec){
    //cout<<" sector "<<sec<<"  multSec  "<<multSec<<endl;
    if(multSec>cutSecD[sec]&&multSec< cutSecU[sec]) {
	secFlag[sec]=1;
	//countEvt[sec]++;
	return 1;
    }
    else {
	secFlag[sec] =0;
	return 0;
    }
}
Bool_t HRichN0Cal::finalize(){
 outFile->cd(); 

 for(Int_t i = 0;i<6;i++){
     thetaH[i]->Write();
     cout<<" sec  "<<i<<" evt Nr  "<<countEvt[i]<<endl;
     if(countEvt[i]>0) {
	 histo[i][0]->Scale(1./countEvt[i]);
	 histo[i][1]->Scale(1./countEvt[i]);
	 histo[i][0]->Write();
	 histo[i][1]->Write();
     }	
 }
 outFile->Close();  
 return 0;
}












