#ifndef HRICHHITSELECTION_H
#define HRICHHITSELECTION_H

#include "TObject.h"

class HRichHit;
class HRichAnalysisPar;

class HRichHitSelection : public TObject {

public:

   HRichHitSelection(){m_fAnalysisPar = NULL;}
   HRichHitSelection(HRichAnalysisPar* par){setParams(par);}
       
   Float_t richHitSelection(HRichHit* pHit, Int_t& ret)
              {return richHitSelection(pHit, ret, m_fAnalysisPar);}
   Float_t richHitSelection(HRichHit* pHit, Int_t& ret,
                               HRichAnalysisPar* pParams);
   void setParams(HRichAnalysisPar* par){m_fAnalysisPar = par;}
   HRichAnalysisPar* getParams(){return m_fAnalysisPar;}

   ClassDef(HRichHitSelection,1) //ROOT extension

private:
      
   HRichAnalysisPar* m_fAnalysisPar;
};

#endif // HRICHHITSELECTION_H
