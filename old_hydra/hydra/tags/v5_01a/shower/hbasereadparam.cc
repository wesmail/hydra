// File: HBaseReadParam.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/12/02 14:12:11
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "hbasereadparam.h"

ClassImp(HBaseReadParam)

HBaseReadParam::HBaseReadParam() {
  setCalPar(NULL);
}

void HBaseReadParam::setCalPar(HParSet* pCalPar)
{
  m_pCalPar= pCalPar;
}

HParSet* HBaseReadParam::getCalPar()
{
  return m_pCalPar;
}

void HBaseReadParam::setFileName(char* pszName) {
   strncpy(m_szFileName, pszName, sizeof(m_szFileName));
}

Char_t* HBaseReadParam::getFileName() {
   return m_szFileName;
}


