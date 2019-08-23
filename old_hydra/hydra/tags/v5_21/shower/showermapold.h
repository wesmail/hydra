// File: showermap.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 17:32:12
//

static const int rowOffset = 16;	// ??

enum Module { pre, post, empty };

struct Adc {
  int nr     :16;
  int charge :8;
  int ctrl   :8;
};

struct Map {
  int row;
  enum Module module; 
};

Map map[24] = {
  {0, post}, {4, post}, {1, post}, {5, post},
  {2, post}, {6, post}, {3, post}, {7, post},
  {4, pre}, {4, empty}, {5, pre}, {5, empty},
  {6, pre}, {6, empty}, {7, pre}, {7, empty},
  {0, pre}, {3, empty}, {1, pre}, {1, empty},
  {2, pre}, {2, empty}, {3, pre}, {0, empty} };
