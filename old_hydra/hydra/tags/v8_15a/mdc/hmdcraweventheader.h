#ifndef HMDCRAWEVENTHEADER_H
#define HMDCRAWEVENTHEADER_H

#include <TObject.h>
#include <stdio.h>

class HMdcRawEventHeader : public TObject {
protected:
  int errorWord;   // error word of each ROC
  int module;      // module number [0-3]
  int roc;         // read out controler (ROC) number
  int sector;      // sector number [0-5]
  int subEvtId;    // subevent id 
  int subEvtSize;  // subevent size
  int triggerType; // trigger type

public:
    HMdcRawEventHeader(void) { clear(); }
    ~HMdcRawEventHeader(void){;}
    void clear(void) {
	// reset data members to default values

	errorWord=module=roc=sector=subEvtId=subEvtSize=triggerType=-999;
    }

    void print(void)
      {
        // print the data member to the screen
	printf("%s: ",this->ClassName());
	printf("subEvtId: %4i "  ,subEvtId);
	printf("subEvtSize: %4i ",subEvtSize);
	printf("trigType: %2i "  ,triggerType);
	printf("roc: %2i "       ,roc);
	printf("errorWord: %2i " ,errorWord);
	printf("module: %2i "    ,module);
	printf("sector: %2i "    ,sector);
	printf("\n");
      }

    void set( int err,   int mod,   int ro, 
	      int sec,   int subId, int subSize, 
	      int trigType)
      { 
	errorWord=err;
	module=mod;
	roc=ro;
	sector=sec;
	subEvtId=subId;
	subEvtSize=subSize;
	triggerType=trigType;
      };

    void setErrorWord  (const int i){errorWord=i;};
    void setModule     (const int i){module=i;};
    void setRoc        (const int i){roc=i;};
    void setSector     (const int i){sector=i;};
    void setSubEvtId   (const int i){subEvtId=i;};
    void setSubEvtSize (const int i){subEvtSize=i;};
    void setTriggerType(const int i){triggerType=i;};

    int getErrorWord  (void){return errorWord;};
    int getModule     (void){return module;};
    int getRoc        (void){return roc;};
    int getSector     (void){return sector;};
    int getSubEvtId   (void){return subEvtId;};
    int getSubEvtSize (void){return subEvtSize;};
    int getTriggerType(void){return triggerType;};

    ClassDef(HMdcRawEventHeader,1) // subevent header of MDC
};

#endif /* !HMDCRAWEVTHEADER_H */
