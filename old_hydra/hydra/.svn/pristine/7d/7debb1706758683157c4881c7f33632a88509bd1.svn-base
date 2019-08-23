//*-- AUTHOR : Ingo Froehlich
//*-- Created : 13/07/2005
//*-- Modified : 

///////////////////////////////////////////////////////////////////////////
//
//  HHypRecPar
//
//  Parameter container for the HHypReconstructor and it's algorithms
//
///////////////////////////////////////////////////////////////////////////

#include "hhyprecpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

ClassImp(HHypRecPar)

HHypRecPar::HHypRecPar(const char *name, const char *title, const char *context):HParCond(name, title,
         context)
{
  // constructor
  clear();
}

HHypRecPar::~HHypRecPar(void) 
{
  for (int i = 0; i < numofparams; i++) delete hypCutID[i];
  numofparams=0;
}

Bool_t HHypRecPar::init(HParIo * inp, Int_t * set)
{
  // initializes the container from an input
  HDetParIo *input = inp->getDetParIo("HCondParIo");

  if (input)
    return (input->init(this, set));
  return kFALSE;
}

Int_t HHypRecPar::write(HParIo * output)
{
  // writes the container to an output
  HDetParIo *out = output->getDetParIo("HCondParIo");

  if (out)
    return out->write(this);
  return kFALSE;
}

void HHypRecPar::clear(void)
{
  // all parameters are initialized with 0.
  numofparams = 0;
  hypCutValue.Set(numofparams);
}

void HHypRecPar::registerCut(char * cutID) {
  //register in init(), that we can load in reinit()
  setCut(cutID,0);
}


Bool_t HHypRecPar::getCut(char * cutID, Float_t & cutValue)
{
  //searches in the list for the cutID and returns the value
  for (int i = 0; i < numofparams; i++)
    if (strcmp(hypCutID[i]->Data(),cutID) == 0) {
      // cout << "requested: " << cutID << ":" << hypCutID[i]->Data() << " found: " << hypCutValue[i] << endl;
      cutValue = hypCutValue[i];
      return kTRUE;
    }
  return kFALSE;
}

void HHypRecPar::setCut(char * cutID, Float_t cutValue)
{
  //set the cut with the cutID (see hypinfodef.h)
  //checks first, if value is already in the list
  //if yes, overwrite
  //if no, append

  //searches in the list for the cutID
  //cout << cutID <<":"<< numofparams << endl;
  for (int i = 0; i < numofparams; i++)
    if (hypCutID[i]->CompareTo(cutID)==0) {
      hypCutValue[i] = cutValue;
      return;
    }

  numofparams++;
  hypCutValue.Set(numofparams);


  //start appending, if there is space
  if (numofparams == MAX_PARAM_VALUES) {
    std::cout << "HHypRecPar::setCut: MAX_PARAM_VALUES reached" << std::endl;
    return;
  }
  hypCutID[numofparams-1] = new TString(cutID);
  hypCutValue[numofparams-1] = cutValue;

}

void HHypRecPar::putParams(HParamList * l)
{
  // puts all parameters to the parameter list, which is used by the io
  if (!l)
    return;
  for (int i = 0; i < numofparams; i++) l->add(hypCutID[i]->Data(), hypCutValue[i]);
}

void HHypRecPar::print(void) 
{
  HParSet::print();
  for (int i = 0; i < numofparams; i++) cout << hypCutID[i]->Data() << ":" << hypCutValue[i] << endl;
}

Bool_t HHypRecPar::getParams(HParamList * l)
{
  // gets all parameters from the parameter list, which is used by the io
  
  if (!l)
    return kFALSE;

  //each registerd cut has to be found!!!
  for (int i = 0; i < numofparams; i++) {
    if (!(l->fill(hypCutID[i]->Data(),&hypCutValue[i]))) {
      cout << "parameter " << hypCutID[i]->Data() << " registered but not found in param source" << endl;
      return kFALSE;
    }
  }

  return kTRUE;
}

#if 1
void HHypRecPar::Streamer(TBuffer & R__b) 
{
  if (R__b.IsReading())         // read 
  {
    Version_t R__v = R__b.ReadVersion();

    if (R__v != 2) {
      cout << "Version 1 not supported" << endl;
      return;
    }
    TObject::Streamer(R__b);
    HParCond::Streamer(R__b);
    Int_t tmp_numofparams;
    R__b >> tmp_numofparams;
    
    for (int i=0; i<tmp_numofparams; i++) {
      TString tmp_hypCutID;
      Float_t tmp_hypCutValue;
      R__b >> tmp_hypCutValue;
      R__b >> tmp_hypCutID;
      setCut((char*)tmp_hypCutID.Data(),tmp_hypCutValue);
    }
  } else {                       // write    
    //cout << "Write STREAMER" << endl;
    R__b.WriteVersion(HHypRecPar::IsA());
    TObject::Streamer(R__b);
    HParCond::Streamer(R__b);
    R__b << numofparams;

    //cout << "numofparams" << numofparams << endl;
    for (int i=0; i<numofparams; i++) {
      R__b << (Float_t)hypCutValue[i];
      R__b << *hypCutID[i];
      //cout << hypCutID[i]->Data() << ":" << hypCutValue[i] << endl;
    }
  }
}
#endif
