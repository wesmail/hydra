// $Id: hconstant.h,v 1.4 2008-09-18 13:05:35 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:48:00
//
#ifndef HCONSTANT_H
#define HCONSTANT_H
#include "TObject.h"
class HConstant : public TObject {

public:
    HConstant();
    ~HConstant();
    static Double_t rad2deg();
private:

ClassDef(HConstant,1)

};

#endif //HCONSTANT_H

