// $Id: hconstant.h,v 1.3 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:48:00
//
#ifndef HCONSTANT_H
#define HCONSTANT_H
#include <TObject.h>
class HConstant : public TObject {

public:
    HConstant();
    ~HConstant();
    static Double_t rad2deg();
private:

ClassDef(HConstant,1)

};

#endif //HCONSTANT_H

