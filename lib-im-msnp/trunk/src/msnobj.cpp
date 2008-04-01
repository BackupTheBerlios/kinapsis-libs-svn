/*
  Copyright (c) 2006 by Pedro Arnedo <vaticano666@gmail.com>
  This file is part of the lib-im-msnp library.
  https://developer.berlios.de/projects/kinapsis-libs/

  This software is distributed under a license. The full license
  agreement can be found in the file LICENSE in this distribution.
  This software may not be copied, modified, sold or distributed
  other than expressed in the named license agreement.

  This software is distributed without any warranty.
*/

#include <QCryptographicHash>
#include "msnobj.h"
namespace libimmsnp {
MSNOBJ::MSNOBJ () {
}
MSNOBJ::~MSNOBJ() {}
QByteArray MSNOBJ::getMsnObj(){
	//<msnobj Creator="buddy@hotmail.com" Size="24539" Type="3" Location="TFR2C.tmp" Friendly="AAA=" SHA1D="trC8SlFx2sWQxZMIBAWSEnXc8oQ=" SHA1C="U32o6bosZzluJq82eAtMpx5dIEI="/>
	QByteArray obj1 = "<msnobj Creator=\"" + m_creator + "\"  Size=\"" + QByteArray::number(m_size) + "\"  Type=\"" + QByteArray::number(m_type) + "\" Location=\"" + m_location + "\" Friendly=\"" + m_friendly + "\"";

	m_SHA1D = QCryptographicHash::hash(obj1,QCryptographicHash::Sha1);

	QByteArray obj2 = obj1 + " SHA1D=\"" + m_SHA1D + "\"";

	m_SHA1C = QCryptographicHash::hash(obj2,QCryptographicHash::Sha1);

	QByteArray obj3 = obj2 + " SHA1C=\"" + m_SHA1C + "\"";

	return obj3;
};
}
