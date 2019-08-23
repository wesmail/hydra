# ifndef HMDCCAL3SIM_H
# define HMDCCAL3SIM_H

# include "hmdccal3.h"

class HMdcCal3Sim : public HMdcCal3{

private:
     Int_t id;                      // particle ID
public:
     HMdcCal3Sim(void);
     HMdcCal3Sim(HMdcCal3Sim* copy);
     HMdcCal3Sim(Int_t id);
     ~HMdcCal3Sim(void);
     void setId(Int_t identifier){id=identifier;}
     Int_t getId(void){return id;}
     void print(void);
     void print(ostream &file);


  ClassDef(HMdcCal3Sim,1)
}; 

# endif





