// File: hconstant.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/04/04 15:19:41
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

