// File: hconstant.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/30 13:05:31
//
#ifndef HCONSTANT_H
#define HCONSTANT_H
#include <TObject.h>
class HConstant : public TObject {

public:
    HConstant();
    ~HConstant();
    Double_t rad2deg();
private:

ClassDef(HConstant,1)

};

#endif //HCONSTANT_H

