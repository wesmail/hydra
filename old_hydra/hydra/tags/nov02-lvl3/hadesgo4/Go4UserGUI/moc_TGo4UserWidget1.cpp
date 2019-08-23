/****************************************************************************
** TGo4UserWidget1 meta object code from reading C++ file 'TGo4UserWidget1.h'
**
** Created: Tue Oct 29 11:31:03 2002
**      by: The Qt MOC ($Id: moc_TGo4UserWidget1.cpp,v 1.1 2002-10-30 18:19:52 kempter Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "TGo4UserWidget1.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *TGo4UserWidget1::className() const
{
    return "TGo4UserWidget1";
}

QMetaObject *TGo4UserWidget1::metaObj = 0;
static QMetaObjectCleanUp cleanUp_TGo4UserWidget1;

#ifndef QT_NO_TRANSLATION
QString TGo4UserWidget1::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TGo4UserWidget1", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString TGo4UserWidget1::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "TGo4UserWidget1", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* TGo4UserWidget1::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"TGo4UserWidget1", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_TGo4UserWidget1.setMetaObject( metaObj );
    return metaObj;
}

void* TGo4UserWidget1::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "TGo4UserWidget1" ) ) return (TGo4UserWidget1*)this;
    return QWidget::qt_cast( clname );
}

bool TGo4UserWidget1::qt_invoke( int _id, QUObject* _o )
{
    return QWidget::qt_invoke(_id,_o);
}

bool TGo4UserWidget1::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool TGo4UserWidget1::qt_property( int _id, int _f, QVariant* _v)
{
    return QWidget::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
