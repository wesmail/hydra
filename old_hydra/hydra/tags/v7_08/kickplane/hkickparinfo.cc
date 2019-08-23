#include "hkickparinfo.h"

void HKickParInfo::print(void) {
  cout << "Date:    " << fDate.Data() << endl;
  cout << "Author:  " << fAuthor.Data() << endl;
  cout << "Comment: " << fComment.Data() << endl;
  cout << "Mode:    " << fMode.Data() << endl;
  cout << "Field:   " << fFieldStrength << endl;
  cout << "Vertex:  (";
  cout << fVertex.getX() << ", ";
  cout << fVertex.getY() << ", ";
  cout << fVertex.getZ() << ", )" << endl;
}

ClassImp(HKickParInfo)
