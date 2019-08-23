/****************************************************************************
** TGo4MainUserPanel meta object code from reading C++ file 'TGo4MainUserPanel.h'
**
** Created: Tue Oct 29 11:30:59 2002
**      by: The Qt MOC ($Id: moc_TGo4MainUserPanel.cpp,v 1.1 2002-10-30 18:19:52 kempter Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "TGo4MainUserPanel.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TGo4MainUserPanel::className() const
{
    return "TGo4MainUserPanel";
}

QMetaObject *TGo4MainUserPanel::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TGo4MainUserPanel;

#ifndef QT_NO_TRANSLATION
QString TGo4MainUserPanel::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TGo4MainUserPanel", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TGo4MainUserPanel::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TGo4MainUserPanel", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TGo4MainUserPanel::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"destroy", 0, 0 };
    static const QUMethod slot_1 = {"init", 0, 0 };
    static const QUMethod slot_2 = {"openHadesMon", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "destroy()", &slot_0, QMetaData::Public },
	{ "init()", &slot_1, QMetaData::Public },
	{ "openHadesMon()", &slot_2, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"TGo4MainUserPanel", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TGo4MainUserPanel.setMetaObject( metaObj );
    return metaObj;
}

void* TGo4MainUserPanel::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TGo4MainUserPanel" ) ) return (TGo4MainUserPanel*)this;
    return QWidget::qt_cast( clname );
}

bool TGo4MainUserPanel::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: destroy(); break;
    case 1: init(); break;
    case 2: openHadesMon(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool TGo4MainUserPanel::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TGo4MainUserPanel::qt_property( int _id, int _f, QVariant* _v)
{
    return QWidget::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
