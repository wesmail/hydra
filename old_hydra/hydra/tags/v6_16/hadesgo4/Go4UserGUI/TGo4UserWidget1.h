/****************************************************************************
** Form interface generated from reading ui file 'TGo4UserWidget1.ui'
**
** Created: Tue Oct 29 11:30:41 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef TGO4USERWIDGET1_H
#define TGO4USERWIDGET1_H

#include <qvariant.h>
#include <qwidget.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QLCDNumber;
class QLabel;
class TQRootCanvas;

class TGo4UserWidget1 : public QWidget
{ 
    Q_OBJECT

public:
    TGo4UserWidget1( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~TGo4UserWidget1();

    QLabel* TextLabel1;
    TQRootCanvas* RootCanvas4;
    QLCDNumber* LCDNumber2;
    QLCDNumber* LCDNumber1;


protected:
    QGridLayout* TGo4UserWidget1Layout;
};

#endif // TGO4USERWIDGET1_H
