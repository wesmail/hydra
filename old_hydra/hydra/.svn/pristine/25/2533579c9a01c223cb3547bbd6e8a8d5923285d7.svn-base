#pragma implementation
#include <hcompositecategory.h>
#include <hdebug.h>

ClassImp(HCompositeCategory)

HCompositeCategory::HCompositeCategory(void)
{
  //Default constructor 
}

HCompositeCategory::~HCompositeCategory(void) {
  //Destructor. 

}

void HCompositeCategory::Streamer(TBuffer &R__b)
{

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HCategory::Streamer(R__b);
   } else {
      R__b.WriteVersion(HCompositeCategory::IsA());
      HCategory::Streamer(R__b);
   }
}

