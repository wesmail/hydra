/****************************************************************************
** Form implementation generated from reading ui file 'TGo4UserWidget1.ui'
**
** Created: Tue Oct 29 11:30:50 2002
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "TGo4UserWidget1.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "/u/shower/go4/go41.1/qtroot2.0/tqrootcanvas.h"
static const char* const image0_data[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #00ff00",
"................................",
".............####...............",
"...............##.aa............",
".........####...#a##............",
"........######...a####..........",
".............aa.a...##..........",
"..............a.a#...##.........",
"............##a.a##.............",
"..........###..aa.###...........",
"..........#....a................",
"...............a................",
"...............a................",
"...............a................",
".............aaaaaa......aa.....",
"......aaaa.aaaaaaaaa..aaaaaa....",
"....aaa...aaaaaaaaaaaaa....aa...",
"....aaaaaaaaaaaa....aaaa....aa..",
"...a......a....aa......aa.......",
"..aa...aaaa.....aaa.....aaaaaaa.",
"....aaaa.aa.....aa.aaa...aa.....",
"...aa...a.aa.....aa..aa...aa....",
"..a....aa..aa.....a...a.....aa..",
".....aaa....aa...aa...aa.....a..",
"....aa.....a.a...aa..aaaaaaa....",
"...aa.....a..a..aa...a..a..aa...",
"...a.....aa..a..a.a.....aa..a...",
".aaa....aa.....aa..aa...a..aaa..",
".a.aa..aaa.....a...aaa.....a.a..",
"....a..a.aa...aa.aa..aa...aa.a..",
".....a....aa.aa..a....a..aa..a..",
".....a..........................",
"................................"};


/* 
 *  Constructs a TGo4UserWidget1 which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f'.
 */
TGo4UserWidget1::TGo4UserWidget1( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    QPixmap image0( ( const char** ) image0_data );
    if ( !name )
	setName( "TGo4UserWidget1" );
    resize( 396, 401 ); 
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, sizePolicy().hasHeightForWidth() ) );
    setCaption( trUtf8( "TGo4UserWidget1" ) );
    TGo4UserWidget1Layout = new QGridLayout( this, 1, 1, 11, 6, "TGo4UserWidget1Layout"); 

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, 0, TextLabel1->sizePolicy().hasHeightForWidth() ) );
    TextLabel1->setText( trUtf8( "This is A QWidget with the TQRootCanvas, drag Histograms from the Go4 Browser and drop them to the Canvas below to draw them." ) );
    TextLabel1->setTextFormat( QLabel::RichText );

    TGo4UserWidget1Layout->addMultiCellWidget( TextLabel1, 0, 0, 0, 1 );

    RootCanvas4 = new TQRootCanvas( this, "RootCanvas4" );

    TGo4UserWidget1Layout->addMultiCellWidget( RootCanvas4, 1, 1, 0, 1 );

    LCDNumber2 = new QLCDNumber( this, "LCDNumber2" );

    TGo4UserWidget1Layout->addWidget( LCDNumber2, 2, 1 );

    LCDNumber1 = new QLCDNumber( this, "LCDNumber1" );

    TGo4UserWidget1Layout->addWidget( LCDNumber1, 2, 0 );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
TGo4UserWidget1::~TGo4UserWidget1()
{
    // no need to delete child widgets, Qt does it all for us
}

