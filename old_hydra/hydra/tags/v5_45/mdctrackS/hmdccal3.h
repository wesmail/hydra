# ifndef HMDCCAL3_H
# define HMDCCAL3_H

# include <fstream.h>
# include <TObject.h>

class HMdcCalLink;

class HMdcCal3 : public TObject{
protected:
  Double_t pos;             // hit coordinate in the local reference system
  Double_t error;           // coordinate error
  Int_t nHits;              // number of hits the cal3 belongs to     
  UInt_t fAddress;          // address: sector, module, layer, cell, hit number

  HMdcCalLink* link;        // !corresponding object in linked list

public:                                      
  HMdcCal3(void);
  HMdcCal3(HMdcCal3* copy);

  ~HMdcCal3(void);
  void setPos(Double_t theta){pos = theta;} 
  void setError(Double_t err){error = err;}
  void setFlag(Int_t n){nHits = n;}     // sets flag to n
  void incrFlag(void){nHits++;}       // increments flag
  void decrFlag(void){nHits--;}       // decrements flag
  Double_t getPos(void){return pos;}    
  Double_t getError(void){return error;}
  Int_t getFlag(void){return nHits;}

  HMdcCalLink* getLink(void){return link;}
  void setLink(HMdcCalLink* calLink){link = calLink;}

  void setAddress(UInt_t address) {fAddress=address;}
  void setAddress(Char_t sector, Char_t module, Char_t layer, Short_t cell, Char_t hitNumber);
  void setHitNumber(Char_t h) {fAddress= (fAddress & 0x0FFFFFF) | (h<<24);}
  void setSector(Char_t s)    {fAddress= (fAddress & 0x00FFFFF) | (s<<20);}
  void setModule(Char_t m)    {fAddress= (fAddress & 0x0F0FFFF) | (m<<16);}
  void setLayer(Char_t l)     {fAddress= (fAddress & 0x0FF0FFF) | (l<<12);}
  void setCell(Short_t c)     {fAddress= (fAddress & 0x0FFF000) | c;}
  UInt_t getAddress(void) {return fAddress;}
  Int_t getHitNumber(void) {return (fAddress>>24 & 0xF);}
  Int_t getSector(void)    {return (fAddress>>20 & 0xF);}
  Int_t getModule(void)    {return (fAddress>>16 & 0xF);}
  Int_t getLayer(void)     {return (fAddress>>12 & 0xF);}
  Int_t getCell(void)      {return (fAddress & 0xFFF);}

  virtual void print(void);
  virtual void print(ostream &file);

  ClassDef(HMdcCal3,1)
};


# endif         /* !HMDCCAL3_H*/






