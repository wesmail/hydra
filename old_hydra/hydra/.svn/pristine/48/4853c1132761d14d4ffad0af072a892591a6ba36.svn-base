#ifndef HDATASOURCE_H
#define HDATASOURCE_H

#include <TObject.h>
class HEvent;

enum EDsState {kDsOk=0,kDsEndFile=1,kDsEndData=2,kDsError=3};

class HDataSource : public TObject {
protected:
   HEvent **fEventAddr; //! Address of the event to fill
public:
   void setEventAddress(HEvent **ev);
   virtual EDsState getNextEvent(void)=0;
   virtual Bool_t init(void)=0;
   ClassDef(HDataSource,1) //Event's input data manager.
};

#endif /* !HDATASOURCE_H */
