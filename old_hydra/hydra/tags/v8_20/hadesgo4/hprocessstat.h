#ifndef HPROCESSSTAT_H
#define HPROCESSSTAT_H

#include "TNamed.h"

/** Class to hold numbers for Hades Online Display.
 */
class HProcessStat : public TNamed {
private:

    Int_t pid;
    Int_t uid;
    Char_t name[64]; //!
    Int_t pri, nice;
    ULong_t size, rss;	// in k
    Int_t state;
    ULong_t time;
    Double_t pcpu, wcpu;

inline Char_t * skip_ws(const Char_t *p){while (isspace(*p)) p++; return (Char_t *)p;}
inline Char_t * skip_token(const Char_t *p){while (isspace(*p)) p++; while (*p && !isspace(*p)) p++; return (Char_t *)p;}

public:
    HProcessStat(const Char_t*,const Char_t*);
    HProcessStat();
    ~HProcessStat();
    void   setPid(Int_t pi){pid=pi;}
    Int_t  getPid()        {return pid;}
    void   update();
    void   getCurrentPid(){pid=getpid();}
    void   print();
    ClassDef(HProcessStat,0)
};
#endif  /*!HPROCESSSTAT_H*/
