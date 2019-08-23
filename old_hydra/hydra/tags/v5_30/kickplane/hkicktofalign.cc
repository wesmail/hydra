#include "hkicktofalign.h"

void HKickTofAlign::evaluate(HKickImpact *imp, HTofHit *hit, 
			     HKickCandidate &out) {
  transform(hit,fFarPos);
  Double_t bx = imp->direction().getX() / imp->direction().getZ();
  Double_t by = imp->direction().getY() / imp->direction().getZ();
  Double_t z = fFarPos.getZ();
//   out.setPosition1(imp->position().getX() + bx * (z - imp->position().getZ())-
// 		   fFarPos.getX(),
// 		   imp->position().getY() + by * (z - imp->position().getZ())-
// 		   fFarPos.getY(),
// 		   z);
  out.setSystem(1);
  out.markAssigned();
  fControl->Fill(imp->position().getX() + bx * (z - imp->position().getZ())-
 		   fFarPos.getX(),
 		   imp->position().getY() + by * (z - imp->position().getZ())-
 		   fFarPos.getY(),
 		   z);
}

Bool_t HKickTofAlign::init(void) {
  fOut = new TFile("ali.root","RECREATE");
  fControl = new TNtuple("con","control","x:y:z");
  return kTRUE;
}

Bool_t HKickTofAlign::finalize(void) {
  if (fOut) {
    fOut->cd();
    fOut->Write();
    fOut->Close();
  }
  return kTRUE;
}

ClassImp(HKickTofAlign)
