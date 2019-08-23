// File: showermap.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 17:32:12
//

static const int rowOffset = 8;	// ??

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

Map mapExp[24] = {
  {0, pre}, {4, pre}, {1, pre}, {5, pre},
  {2, pre}, {6, pre}, {3, pre}, {7, pre},
  {4, pre}, {4, empty}, {5, pre}, {5, empty},
  {6, pre}, {6, empty}, {7, pre}, {7, empty},
  {0, pre}, {3, empty}, {1, pre}, {1, empty},
  {2, pre}, {2, empty}, {3, pre}, {0, empty} };
