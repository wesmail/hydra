//*-- AUTHOR : Ingo Froehlich
//*-- Created : 30/11/2005
//*-- Modified : 

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//
//  HPidAlgHardCutsPar
//
//  Parameter container for any 2dim graphical cut
//
///////////////////////////////////////////////////////////////////////////

#include "hpidalghardcutspar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

ClassImp(HPidAlgHardCutsPar)

HPidAlgHardCutsPar::HPidAlgHardCutsPar(const Char_t *name, const Char_t *title, const Char_t *context)
  :HParCond(name, title, context)
{
  // constructor
  clear();
}

HPidAlgHardCutsPar::~HPidAlgHardCutsPar(void) 
{
  for (Int_t i = 0; i < numofparams; i++) {
    delete HardCutID[i];
    delete HardCutValue[i];
  }
  numofparams=0;
}

Bool_t HPidAlgHardCutsPar::init(HParIo * inp, Int_t * set)
{
  // initializes the container from an input
  HDetParIo *input = inp->getDetParIo("HCondParIo");

  if (input)
    return (input->init(this, set));
  return kFALSE;
}

Int_t HPidAlgHardCutsPar::write(HParIo * output)
{
  // writes the container to an output
  HDetParIo *out = output->getDetParIo("HCondParIo");

  if (out)
    return out->write(this);
  return kFALSE;
}

void HPidAlgHardCutsPar::clear(void)
{
  // all parameters are initialized with 0.
  numofparams = 0;
  for (Int_t i = 0; i < HARDCUTSPAR_MAX; i++) {
    HardCutValue[i]=NULL;
    HardCutID[i]=NULL;
    x[i]=new TArrayF();
    y[i]=new TArrayF();
  }
}

void HPidAlgHardCutsPar::registerCut(Char_t * cutID) {
  //register in init(), that we can load in reinit()
  TCutG * dummy = NULL;
  setCut(cutID,dummy);
}


TCutG * HPidAlgHardCutsPar::getCut(Char_t * cutID)
{
  //searches in the list for the cutID and returns the value
  for (Int_t i = 0; i < numofparams; i++)
    if (strcmp(HardCutID[i]->Data(),cutID) == 0) {
      return HardCutValue[i];
    }
  return NULL;
}

void HPidAlgHardCutsPar::setCut(Char_t * cutID, TCutG *  cutValue)
{
  //set the cut with the cutID
  //checks first, if value is already in the list
  //if yes, overwrite
  //if no, append

  //searches in the list for the cutID
  //cout << cutID <<":"<< numofparams << endl;
  for (Int_t i = 0; i < numofparams; i++)
    if (HardCutID[i]->CompareTo(cutID)==0) {
      delete HardCutValue[i];
      if (cutValue) {
	HardCutValue[i] = new TCutG(HardCutID[i]->Data(),cutValue->GetN(),cutValue->GetX(),cutValue->GetY());
      }
      else {
	HardCutValue[i] = NULL;
      }
      return;
    }

  numofparams++;

  //start appending, if there is space
  if (numofparams == HARDCUTSPAR_MAX) {
    std::cout << "HPidAlgHardCutsPar::setCut: HARDCUTSPAR_MAX reached" << std::endl;
    return;
  }
  HardCutID[numofparams-1] = new TString(cutID);
  if (cutValue) {
    gROOT->GetListOfSpecials()->Remove(cutValue); //this is needed sincs ctor calls dtor
    HardCutValue[numofparams-1] = new TCutG(HardCutID[numofparams-1]->Data(),cutValue->GetN(),cutValue->GetX(),cutValue->GetY());
  }
  else {
    HardCutValue[numofparams-1] = NULL;
  }
}

void HPidAlgHardCutsPar::putParams(HParamList * l)
{
  // puts all parameters to the parameter list, which is used by the io
  if (!l)
    return;
  for (Int_t i = 0; i < numofparams; i++) {
    //Have to reconstruct x any y names
    TString xname(*HardCutID[i] + TString("_x"));
    TString yname(*HardCutID[i] + TString("_y"));

    //making dummies to convert TArrayD ind F
    TArrayD xd(HardCutValue[i]->GetN(), HardCutValue[i]->GetX());
    TArrayD yd(HardCutValue[i]->GetN(), HardCutValue[i]->GetY());
    TArrayF xf;
    TArrayF yf;
    xf.Set(HardCutValue[i]->GetN());
    yf.Set(HardCutValue[i]->GetN());
    for (Int_t j=0;j<HardCutValue[i]->GetN();j++) {
      xf[j]=(Float_t) xd[j];
      yf[j]=(Float_t) yd[j];
    }

    //maybe delete the old shit
    if (x[i]) delete x[i];
    if (y[i]) delete y[i];
    x[i] = new TArrayF(xf);
    y[i] = new TArrayF(yf);
    l->add(xname, * x[i]);
    l->add(yname, * y[i]);
  }
}

void HPidAlgHardCutsPar::print(void) 
{
  HParSet::print();
  for (Int_t i = 0; i < numofparams; i++) {
    cout << HardCutID[i]->Data() << " is:" << endl;
    HardCutValue[i]->Print();
  }
}

Bool_t HPidAlgHardCutsPar::getParams(HParamList * l)
{
  // gets all parameters from the parameter list, which is used by the io
  
  if (!l)
    return kFALSE;

  //each registerd cut has to be found!!!
  for (Int_t i = 0; i < numofparams; i++) {
    //Have to reconstruct x any y names
    TString xname(*HardCutID[i] + TString("_x"));
    TString yname(*HardCutID[i] + TString("_y"));

    //look if registered cut was found
    if (!(l->fill(xname, x[i])))
      Error("getParams", "parameter %s (x) registered but not found in param source",HardCutID[i]->Data());
    if (!(l->fill(yname, y[i])))
      Error("getParams", "parameter %s (y) registered but not found in param source",HardCutID[i]->Data());
    if (x[i]->GetSize() != y[i]->GetSize())
      Error("getParams", "parameter %s: N does not match ",HardCutID[i]->Data());
   
    //some debug info
    cout << "Array with " << y[i]->GetSize()<< " elements for " << HardCutID[i]->Data() << " found." << endl;
 
    //if worked, set
    TCutG dummy(HardCutID[i]->Data(),y[i]->GetSize(),x[i]->GetArray(),y[i]->GetArray());    
 

    gROOT->GetListOfSpecials()->Remove(&dummy); //this is needed sincs ctor calls dtor
    setCut((Char_t *)HardCutID[i]->Data(),&dummy);
    
  }

  return kTRUE;
}

#if 1
void HPidAlgHardCutsPar::Streamer(TBuffer & R__b) 
{
  if (R__b.IsReading())         // read 
  {
    Version_t R__v = R__b.ReadVersion();

    if (R__v != 1) {
      cout << "Version >1 not supported" << endl;
      return;
    }
    TObject::Streamer(R__b);
    HParCond::Streamer(R__b);
    Int_t tmp_numofparams;
    R__b >> tmp_numofparams;
    
    for (Int_t i=0; i<tmp_numofparams; i++) {
      TString tmp_HardCutID;
      TCutG tmp_HardCutValue;
      tmp_HardCutValue.Streamer(R__b);
      R__b >> tmp_HardCutID;
      setCut((Char_t*)tmp_HardCutID.Data(),&tmp_HardCutValue);
    }
  } else {                       // write    
    //cout << "Write STREAMER" << endl;
    R__b.WriteVersion(HPidAlgHardCutsPar::IsA());
    TObject::Streamer(R__b);
    HParCond::Streamer(R__b);
    R__b << numofparams;

    //cout << "numofparams" << numofparams << endl;
    for (Int_t i=0; i<numofparams; i++) {
      HardCutValue[i]->Streamer(R__b);
      R__b << *HardCutID[i];
    }
  }
}
#endif
