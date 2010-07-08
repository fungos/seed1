#ifndef _GETDXVER_H_
#define _GETDXVER_H_

#define INITGUID
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dx8/dxdiag.h>

HRESULT GetDXVersion(DWORD *pdwDirectXVersion, TCHAR *strDirectXVersion, int cchDirectXVersion);
HRESULT GetDirectXVersionViaDxDiag(DWORD *pdwDirectXVersionMajor, DWORD *pdwDirectXVersionMinor, TCHAR *pcDirectXVersionLetter);
HRESULT GetDirectXVerionViaFileVersions(DWORD *pdwDirectXVersionMajor, DWORD *pdwDirectXVersionMinor, TCHAR *pcDirectXVersionLetter);
HRESULT GetFileVersion(TCHAR *szPath, ULARGE_INTEGER *pllFileVersion);
ULARGE_INTEGER MakeInt64(WORD a, WORD b, WORD c, WORD d);
int CompareLargeInts(ULARGE_INTEGER ullParam1, ULARGE_INTEGER ullParam2);

#endif _GETDXVER_H_
