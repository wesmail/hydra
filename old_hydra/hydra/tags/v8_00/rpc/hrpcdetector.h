#ifndef HRPCDETECTOR_H
#define HRPCDETECTOR_H
                                                                                
#include "hdetector.h"
#include "haddef.h"
#include <TArrayI.h>
                                                                                
class HRpcDetector : public HDetector {
                                                                                
public:
  HRpcDetector(void);
  ~HRpcDetector(void);
  HCategory *buildCategory(Cat_t cat);
  HCategory *buildMatrixCategory(Text_t *,Float_t );
  HCategory *buildLinearCategory(Text_t *,Float_t );
  Bool_t init(void) {return kTRUE;}
  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);
                                                                                
  ClassDef(HRpcDetector,1) // RPC detector class
};
                                                                                
                                                                                
#endif /* !HRPCDETECTOR_H */

