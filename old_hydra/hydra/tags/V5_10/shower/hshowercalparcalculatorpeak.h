#ifndef HShowerCalParCalculatorPeak_H
#define HShowerCalParCalculatorPeak_H

#include "hshowercalparcalculator.h"

class HShowerCalParCalculatorPeak1 : public HShowerCalParCalculator {
public:
	HShowerCalParCalculatorPeak1()
             : HShowerCalParCalculator(1){}
        HShowerCalParCalculatorPeak1(Text_t *name,Text_t *title) 
             : HShowerCalParCalculator(name, title, 1){}

        ClassDef(HShowerCalParCalculatorPeak1,1) //ROOT extension
};

class HShowerCalParCalculatorPeak2 : public HShowerCalParCalculator {
public:
        HShowerCalParCalculatorPeak2()
             : HShowerCalParCalculator(2){}
        HShowerCalParCalculatorPeak2(Text_t *name,Text_t *title)
             : HShowerCalParCalculator(name, title, 2){}

        ClassDef(HShowerCalParCalculatorPeak2,1) //ROOT extension
};

#endif
