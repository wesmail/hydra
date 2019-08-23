# ifndef HMDCHITAUXSIM_H
# define HMDCHITAUXSIM_H

# include <fstream.h>
# include "hmdchitaux.h"

class HMdcHitAuxSim : public HMdcHitAux {
private:
  Int_t id;                                    // track identifier
public:
  HMdcHitAuxSim(void);
  ~HMdcHitAuxSim(void);
  void setId(Int_t identifier){id = identifier;}
  Int_t getId(void) {return id;}
  void print(ostream &file);

  ClassDef(HMdcHitAuxSim,1)
};
# endif /* !HMDCHITAUXSIM_H */
