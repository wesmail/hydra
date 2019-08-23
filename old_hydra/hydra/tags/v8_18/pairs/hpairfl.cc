// $Id: hpairfl.cc,v 1.11 2008-05-09 16:14:32 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-03-28 18:31:49
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairFL                                                                     
//                                                                           
// Static function library of module "pairs"
// 
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <TFile.h>
#include <TH1F.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TString.h>
#include <TVector3.h>
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hcategory.h>
#include <hevent.h>
#include <hiterator.h>
#include <hlinearcategory.h>
#include <hgeantkine.h>
#include <hgeomvector.h>
#include <hgeomvertexfit.h>
// ---------------------------------------------------------------------------
#include "hpidparticle.h"
#include "pairsdef.h"
// ---------------------------------------------------------------------------
#include "hpairfl.h"
// ---------------------------------------------------------------------------
ClassImp(HPairFL)
// ---------------------------------------------------------------------------


HPairFL::HPairFL(void) 
{

}

// ---------------------------------------------------------------------------
HPairFL::~HPairFL(void) 
{

}

// ---------------------------------------------------------------------------
HCategory* HPairFL::buildLinearCategory(Cat_t cat, Text_t* catName) 
{
    HCategory *pCat = 0;
    if (gHades && gHades->getCurrentEvent())
    {
	if ((pCat = ((HEvent*)(gHades->getCurrentEvent()))->getCategory(cat))) 
	    return pCat;
	else
	{
	    pCat = new HLinearCategory(catName); 
	    if (!pCat) ::Error("HPairFL::buildLinearCategory",
			     "Cannot create new category");
	} 
    }
    else 
    {
	::Error("HPairFL::buildLinearCategory","Cannot access current event");
	return pCat;
    }
    return pCat;

}

// ---------------------------------------------------------------------------
HCategory* HPairFL::getCategory(Cat_t cat, Bool_t bWarning)
{
// Find HCategory if gHades->getCurrentEvent() is accessible

    HEvent    *pEvent;
    HCategory *pCat;
    
    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        if(bWarning)
        {
            ::Error("HPairFL::getCategory",
                            "Cannot access current event");
        }

        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
    {
        if(bWarning)
            ::Error("HPairFL::getCategory", "No category: %d", cat);
    }

    return pCat;
}
// ---------------------------------------------------------------------------
void HPairFL::saveHistos(TFile* pFileOut, TObjArray* pHistArray)
{
    // use this function to save histos every n events 
    // assumes that you have opened a file and that your
    // histograms are stored in a TObjArray
    if(!pFileOut->IsOpen())
    {
	TString filename(pFileOut->GetName());
	pFileOut=TFile::Open(filename.Data(),"UPDATE");
	cout<<"Warning: File "<<pFileOut->GetName()<<" was reopened."<<endl;
	pFileOut->Print();
    }
    pFileOut->cd();
    // write histograms
    for (Int_t i=0;i<(pHistArray->GetLast()+1);i++)
    {
	( (TH1*)(*pHistArray)[i] )->Write();
    }
}
// ---------------------------------------------------------------------------
void HPairFL::printBitField(const Int_t in)
{
    Int_t nBits = sizeof(in) * 8 + 1;
    Char_t* c = new Char_t[nBits];

    for (Int_t k=0; k<=nBits-1 ; k++) c[k]=48; //0
    for (Int_t k=0; k<=nBits-1 ; k++)
        if ( in & (1 << k) ) c[nBits-1-k]=49;//1
    c[nBits-1]=13; // CR
    cout<<"Bit field: "<<in<<" is "<<dec<<c<<endl;
    delete c;
}
// ---------------------------------------------------------------------------
void HPairFL::printBitField(const Char_t in)
{
    Int_t nBits = sizeof(in) * 8 + 1;
    Char_t* c = new Char_t[nBits];

    for (Int_t k=0; k<=nBits-1 ; k++) c[k]=48;
    for (Int_t k=0; k<=nBits-1 ; k++)
        if ( in & (1 << k) ) c[nBits-1-k]=49;
    c[nBits-1]=13; // CR
    cout<<"Bit field: "<<in<<" is "<<dec<<c<<endl;
    delete c;
}
// ---------------------------------------------------------------------------
void HPairFL::setBit(UChar_t pos, Int_t& in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
    }
    Int_t SETTHIS = (1 << pos);
    in |= SETTHIS;
}
// ---------------------------------------------------------------------------
Int_t HPairFL::getBit(UChar_t pos, const Int_t in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
	return -1;
    }
    if ( in & (1 << pos) ) return 1;
    else return 0;
}
// ---------------------------------------------------------------------------
void HPairFL::clearBit(UChar_t pos, Int_t& in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
    }
    Int_t SETTHIS = (Int_t(1) << pos);
    in &= ~SETTHIS;
}
// ---------------------------------------------------------------------------
void HPairFL::setBit(UInt_t pos, Int_t& in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
    }
    Int_t SETTHIS = (1 << pos);
    in |= SETTHIS;
}
// ---------------------------------------------------------------------------
Int_t HPairFL::getBit(UInt_t pos, const Int_t in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
	return -1;
    }
    if ( in & (1 << pos) ) return 1;
    else return 0;
}
// ---------------------------------------------------------------------------
void HPairFL::clearBit(UInt_t pos, Int_t& in)
{
    // start counting from 0 for the position !
    // i.e.: 0,1,2,3,4,5,6, ....
    UInt_t nBits = sizeof(in) * 8;
    if (pos > nBits-1) 
    {
	::Error("HPairFL::setBit","bit position out of range !");
    }
    Int_t SETTHIS = (1 << pos);
    in &= ~SETTHIS;
}
// ---------------------------------------------------------------------------
Int_t HPairFL::countPositiveBits(const Int_t in)
{
    Int_t nBits = sizeof(in) * 8;
    Int_t cnt=0;
    for (Int_t k=0; k<=nBits-1 ; k++) if ( in & (1 << k) ) cnt++;
    return cnt;
}
// ---------------------------------------------------------------------------
/*
// ---------------------------------------------------------------------------
TH1F* HPairFL::createVarBinHisto(TH1* h,Float_t* xbins,Int_t nbins,Bool_t kNorm=kTRUE,Bool_t kErr=kTRUE)
{
    
    // ASSUMPTION: input histo is __NOT__ normalized to bin width !
    TString name(h->GetName());
    name.Append("_rebsig");
    
    //create new histogram with new binning
    TH1F * hn = new TH1F(name.Data(),"inv_mass signal",nbins,xbins);
    //hn->Dump(); 
    hn->Sumw2();
    h->Sumw2();
 
    cout<<"Array input size: "<<h->GetNbinsX()<<endl;
    cout<<"Array output size: "<<hn->GetNbinsX()<<endl;
 
    //init array to store errors
    Float_t *errn = new Float_t[hn->GetNbinsX()+1];
    for (Int_t k=0;k<hn->GetNbinsX()+1;k++) errn[k]=0.;
    cout<<"Error array created with size: "<<hn->GetNbinsX()+1<<endl;
 
 
    // fill rebinned histogram, skip underflow bin
    for (Int_t j=1;j<h->GetNbinsX()+1;j++)
    {
	Int_t i = h->GetBin(j);
	//find center of bin of old histo
	Float_t cc = h->GetBinCenter(i);
	//stop if old bins are beyond new bins
	if (cc > hn->GetBinCenter(hn->GetBin(hn->GetNbinsX()))+
	    hn->GetBinWidth(hn->GetBin(hn->GetNbinsX()))/2. ) break; 
	
	//fill old content in new bin number bin
	Int_t bin = hn->Fill(cc,h->GetBinContent(i));
	
	//check bounds, sum errors quadratic, save in new bin
	if (kErr && bin>=0 && bin<=hn->GetNbinsX()) 
	    errn[bin]+=h->GetBinError(i)*h->GetBinError(i);
	
    }
    
    // Normalization to bin width
    if (kNorm)
    {
	for (Int_t j=1;j<hn->GetNbinsX()+1;j++)
	{
         Int_t bin = hn->GetBin(j);
         hn->SetBinContent( bin, hn->GetBinContent(bin)/ hn->GetBinWidth(bin) );
	}
	
    }
    
    // Set errors, rescale if normalized
    if (kErr)
    {
	for (Int_t j=0;j<hn->GetNbinsX()+1;j++)
	{
	    Int_t bin = hn->GetBin(j);
	    hn->SetBinError(bin,TMath::Sqrt(errn[j]));
	    if (kNorm) hn->SetBinError(bin,hn->GetBinError(bin)/hn->GetBinWidth(bin));
	}
    }
    
    delete [] errn;
    return hn;
}
// ---------------------------------------------------------------------------
TH1F* HPairFL::getSameEventCombinatorialBackground(TH1F *pp, TH1F* nn, Int_t typ)
{
    Char_t name[256];
    sprintf(name,"%s_back",pp->GetName());

    TH1F *bground = new TH1F(name,"comb.backg.",pp->GetNbinsX(),0,pp->GetXaxis()->GetXmax());
    
    bground->Sumw2();
    Double_t massPP, massNN;
    Double_t errPP, errNN;
    Int_t maxBins = pp->GetNbinsX();
    
    for(Int_t i = 1; i < maxBins+1; i++)
    {// skip underflow bin
	massPP  =  pp->GetBinContent(i);
	massNN  =  nn->GetBinContent(i);
	
	errPP   = pp->GetBinError(i);
	errNN   = nn->GetBinError(i);
	
	
	
	if(typ == 0 ) 
	{ 
	    bground->SetBinContent(i,2.*TMath::Sqrt(massPP*massNN));
	    
	    if(massNN>0. && massPP>0. && errPP*errPP*massNN*massPP>=0.)  
                                    bground->SetBinError(i,TMath::Sqrt(  errPP*errPP*massNN/massPP + 
                                                                         errNN*errNN*massPP/massNN 
                                                                         ));
	}
	if(typ == 1) 
	{
	    bground->SetBinContent(i,massPP+massNN);
	    Float_t radix = errPP*errPP + errNN*errNN;
	    if (radix>=0.) bground->SetBinError(i,TMath::Sqrt(radix));
	}       
    }
    
    return bground;
}
// ---------------------------------------------------------------------------
TH1F* HPairFL::subtractHistogram(TH1F *pn, TH1F* backg)
{
    Char_t name[256];
    sprintf(name,"%s_sig",pn->GetName());
    
    TH1F  *invMass = new TH1F(name,"inv_mass_signal",pn->GetNbinsX(),0,pn->GetXaxis()->GetXmax());
    
    invMass->Sumw2();
    invMass->Add(pn,backg,1.,-1.);
    return invMass;
}
// ---------------------------------------------------------------------------
void HPairFL::printHisto(TH1F* h)
{
    Int_t nbXbins = h->GetNbinsX();
    cout<<"Number of bins in histo: "<<nbXbins<<endl;
    for (Int_t j=0;j<nbXbins+1;j++) // loop over bins in X dir
    {
        Int_t i = h->GetBin(j); // return bin, redundant here
        Float_t cc = h->GetBinCenter(i);
        Float_t errn= h->GetBinError(i);
        Float_t width=h->GetBinWidth(i);
        Float_t con=h->GetBinContent(i);
        if (cc > 1000.) break; 
        Float_t relerr=-1.;
        if (con>0.) relerr = errn/con*100.;
        cout<<"Index: "<<j<<" Bin: "<<i<<" Center: "<<cc
	    <<" Content:  "<<con<<" Width: "<<width
	    <<" Error: "<<errn<<" rel. Err.: "<<relerr
	    <<"%"<<" (sqrt content: "<<TMath::Sqrt(con)<<")"<<endl;
    }
}
*/
// ---------------------------------------------------------------------------
Float_t HPairFL::calcInvMass(HPidParticle* p1, HPidParticle* p2)
{
    // use this function only for beta = 1 particles
    TVector3 pvec1 = p1->Vect();
    TVector3 pvec2 = p2->Vect();
    Float_t opang=pvec1.Angle(pvec2);//in radians
    Float_t invMass=2.*TMath::Sin(opang/2.)*TMath::Sqrt(p1->P()*p2->P());
    if (invMass>0.) return invMass;
    else return -1.;
}
// ---------------------------------------------------------------------------
Float_t HPairFL::calcInvMass(TLorentzVector* t1, TLorentzVector* t2)
{
    // use this function only for beta = 1 particles
    TVector3 pvec1 = t1->Vect();
    TVector3 pvec2 = t2->Vect();
    Float_t opang=pvec1.Angle(pvec2);//in radians
    Float_t invMass=2.*TMath::Sin(opang/2.)*TMath::Sqrt(t1->P()*t2->P());
    if (invMass>0.) return invMass;
    else return -1.;
}
// ---------------------------------------------------------------------------
HGeantKine* HPairFL::findParent(HGeantKine *kine,HIterator* iter_kine2)
{
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    if (aPar){
        HGeantKine *kine2=0;
        Int_t aTrackParent,aIDParent;

        //iter_kine2 = (HIterator*)(cat->MakeIterator("native"));
        iter_kine2->Reset();

        while((kine2=(HGeantKine *)iter_kine2->Next())!=0)
        {
            kine2->getParticle(aTrackParent,aIDParent);
            if (aPar == aTrackParent)
            {
		return kine2;
            }
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
HGeantKine* HPairFL::findParent(HGeantKine *kine,HCategory* cat)
{
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    if (aPar){
        HIterator* iter_kine2 = NULL;
        HGeantKine *kine2=0;
        Int_t aTrackParent,aIDParent;

        iter_kine2 = (HIterator*)(cat->MakeIterator("native"));
        iter_kine2->Reset();

        while((kine2=(HGeantKine *)iter_kine2->Next())!=0)
        {
            kine2->getParticle(aTrackParent,aIDParent);
            if (aPar == aTrackParent)
            {
		return kine2;
            }
        }

		// safe delete iter_kine2
		if(iter_kine2)
		{
		   delete iter_kine2;
		   iter_kine2 = NULL;
		}
    }
    return 0;
}
// ---------------------------------------------------------------------------
Float_t HPairFL::calcVertex(HPidParticle *p1, HPidParticle *p2,
	TVector3 *vertex, TVector3 *distance)
{
// calcVertex should return
// 1. the vertex of two tracks (no weights included, so it returns the
//    center of closest approach vector)
// 2. a vector with direction and magnitude of the distance
//    (using stefanos algebra to calculate the magnitude, root cross product
//    to give the direction)

	HGeomVector hoff[2];
	HGeomVector hdir[2];
	HGeomVector hvertex;
	HGeomVertexFit hfitter;
	TVector3 dir[2];
	Float_t dist;
	Float_t det1, det2;

	// extract coordinates from p1, p2, fill them into HGeomVector
	// to use Manuels fitter
	hoff[0].setXYZ(p1->getR()*TMath::Cos(p1->phiDeg()*TMath::DegToRad()
					+ TMath::PiOver2()),
				p1->getR()*TMath::Sin(p1->phiDeg()*TMath::DegToRad()
					 + TMath::PiOver2()),
				p1->getZ());
	hoff[1].setXYZ(p2->getR()*TMath::Cos(p2->phiDeg()*TMath::DegToRad()
					+ TMath::PiOver2()),
				p2->getR()*TMath::Sin(p2->phiDeg()*TMath::DegToRad()
					 + TMath::PiOver2()),
				p2->getZ());

	dir[0] = p1->Vect();
	dir[1] = p2->Vect();
	hdir[0].setXYZ(dir[0].X(),dir[0].Y(),dir[0].Z());
	hdir[1].setXYZ(dir[1].X(),dir[1].Y(),dir[1].Z());

	hfitter.reset();
	for (Int_t i = 0; i < 2; i++) {
		hfitter.addLine(hoff[i],hdir[i]);
	}
	hfitter.getVertex(hvertex);
	vertex->SetXYZ(hvertex.getX(),hvertex.getY(),hvertex.getZ());

	// Function to calculate the distance between two lines in the space
	// c.f. Stefano
	det1 = (
		(hoff[0].getX()-hoff[1].getX()) *
		(hdir[0].getY()*hdir[1].getZ()-hdir[0].getZ()*hdir[1].getY()) -
		(hoff[0].getY()-hoff[1].getY()) *
		(hdir[0].getX()*hdir[1].getZ()-hdir[0].getZ()*hdir[1].getX()) +
		(hoff[0].getZ()-hoff[1].getZ()) *
		(hdir[0].getX()*hdir[1].getY()-hdir[0].getY()*hdir[1].getX())
	);

	det2 = TMath::Sqrt(
		(hdir[0].getX()*hdir[1].getY() - hdir[0].getY()*hdir[1].getX()) *
		(hdir[0].getX()*hdir[1].getY() - hdir[0].getY()*hdir[1].getX()) +
		(hdir[0].getX()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getX()) *
		(hdir[0].getX()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getX()) +
		(hdir[0].getY()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getY()) *
		(hdir[0].getY()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getY())
		);

	// Create a distance vector and scale it with dist
	*distance = dir[0].Cross(dir[1]);

	if (det2==0) {
		dist = -1.;
	} else {
		dist = TMath::Abs(det1/det2);
		distance->SetMag(dist);
	}
	return dist;
}
// ---------------------------------------------------------------------------
void HPairFL::dumpKine(HGeantKine *kine)
{
    if (kine)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ax, ay, az;
	Float_t apx, apy, apz;
	Float_t aInfo, aWeight;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	kine->getVertex(ax,ay,az);
	kine->getMomentum(apx,apy,apz);
	kine->getGenerator(aInfo,aWeight);
	ptot=kine->getTotalMomentum();
	cout<<"----------------------GEANT KINE------------------------"<<endl;
	cout<<"--- GEANT track number: "<<aTrack<<endl;
	cout<<"--- track number of parent particle: "<<aPar<<endl;
	cout<<"--- GEANT particle ID: "<<aID<<endl;
	cout<<"--- GEANT medium of creation: "<<aMed<<endl;
	cout<<"--- GEANT creation mechanism: "<<aMech<<endl;
	cout<<"--- x of track vertex (in mm): "<<ax<<endl;
	cout<<"--- y                        : "<<ay<<endl;
	cout<<"--- z                        : "<<az<<endl;
	cout<<"--- x component of particle momentum (in MeV/c): "<<apx<<endl;
	cout<<"--- y                                          : "<<apy<<endl;
	cout<<"--- z                                          : "<<apz<<endl;
	cout<<"--- total momentum                             : "<<ptot<<endl;
// 	Float_t theta,phi;
// 	calcParticleAnglesKine(kine,theta,phi);
// 	cout<<"--- theta,phi                                  : "<<theta
// 	    <<" "<<phi<<endl;
	cout<<"--- event generator info: "<<aInfo<<endl;
	cout<<"--- associated weight: "<<aWeight<<endl;
	cout<<"--- "<<endl;
	cout<<"--- "<<endl;
	cout<<"--- "<<endl;
	cout<<"--- "<<endl;
	cout<<"--------------------------------------------------------"<<endl;
    }
}
//---------------------------------------------------------------------------- 
Bool_t HPairFL::isNewIndex(Int_t i,Int_t* iarr,Int_t max)
{
    // compare Int_t to content of given array iarr
    // and decide if it is already stored or not
    // if not stored, remember the Int_t 
    Int_t n=0;

    do{
	if(i==iarr[n]) break;//i already stored
	else if(iarr[n]==-2) //new slot
	{
	    iarr[n]=i;//store value i in new slot
	    return kTRUE;
	}
	n++;
    } while(n<max);

    return kFALSE;

}
//---------------------------------------------------------------------------- 
Bool_t HPairFL::isNew2Tuple(Int_t i,Int_t j,Int_t *tuple,Int_t max)
{
    //this function projects (i,j)-->(n) {N2-->N1}
    //according to the function ax+by
    //and decides whether this value is already stored or not
    //the array tuple is assumed to be init to -2

    // N.B.!!   here (i,j) is the same tuple as (j,i) !!!

    //dynamically choose multiplicator
    Int_t base=10;
    Int_t cnt=1;
    while ((max%cnt)<max)
    {
	cnt=cnt*base;
    }
    Int_t f1=cnt*i+j;
    Int_t f2=cnt*j+i;
    //    cout<<"number of entries is: "<<max<<"  and multiplicator is: "<<cnt<<endl;
    Int_t index=-1;
    //  cout<<"Index: ("<<i<<","<<j<<") ==> "<<f1<<" = "<<f2<<endl;
    for (Int_t k=0;k<max;k++)
    {

	if (tuple[k]==-2) 
	{
	    //    cout<<"this is tuple "<<tuple[k]<<endl;
	    index=k;
	    //      cout<<"only default found , break!"<<endl;
	    break;
	}//not found in not default
	if (f1==tuple[k] || f2==tuple[k]) 
	{
	    //cout<<"already stored: "<<tuple[k]<<endl;
	    return kFALSE;//already stored
	}
    }
    //        cout<<index<<" is index"<<endl;
    if (index>-1) 
    {
	tuple[index]=f1;//store new 2-tuple
	//cout<<"newly stored: "<<tuple[index]<<endl;
    }
    else cout<<"(isNew2Tuple,index of new slot not set)"<<endl;
    return kTRUE;
}
//---------------------------------------------------------------------------- 
