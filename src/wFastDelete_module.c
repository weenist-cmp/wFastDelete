#include <windows.h>
#include "wFastDelete_module.h"
//==============================================================================
int wwiReadString(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize)
{
  DWORD rct;
  rct = GetPrivateProfileString(lpAppName,lpKeyName,"",lpReturnedString,nSize,lpFileName);
  if(rct>=0UL) return (int)rct;
  return -1;
}
//==============================================================================
int wwiWriteString(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
  if(WritePrivateProfileString(lpAppName,lpKeyName,lpString,lpFileName)==TRUE) return 1;
  return -1;
}
//==============================================================================
// HKEY_CURRENT_USER\Software
// 1 OK
//-1 SectionMain??
//-2 에러!! 아마도 서브키가 존재
//SectionSub가 NULL이면 SectionMain키가 지워짐
//지워지는 키의 서브키가 존재하면 에러
int wwrDeleteKey(char *SectionMain, char *SectionSub)
{
  int rc;
  HKEY hkr,hkr2,hkm;

  rc=-2;
  if(SectionMain==NULL) return -1;
  if(RegOpenKey(HKEY_CLASSES_ROOT,"Directory",&hkr)==ERROR_SUCCESS){
    if(RegOpenKey(hkr,"shell",&hkr2)==ERROR_SUCCESS){
      if(SectionSub==NULL || RegOpenKey(hkr2,SectionMain,&hkm)==ERROR_SUCCESS){
        if(RegDeleteKey((SectionSub==NULL ? hkr2 : hkm),(SectionSub==NULL ? SectionMain :SectionSub))==ERROR_SUCCESS) rc = 1;
        if(SectionSub!=NULL) RegCloseKey(hkm);
      }
      RegCloseKey(hkr2);
    }
    RegCloseKey(hkr);
  }
  return rc;
}
//==============================================================================
// 1~ OK strlen
//-1 mode?? SectionMain?? SectionSub?? Valuename?? Value??
//-2 Value 에러!!
int _wwrString(int mode, char *SectionMain, char *SectionSub, const char *Valuename, char *Value, int MaxSize)
{
  int rc,ss;
  HKEY hkr,hkr2,hkm,hks;
  DWORD vs,vt;

  rc=-3;
  vs=MaxSize;
  if(mode>2||mode<0||SectionMain==NULL) return -1;
  if(mode<2 && (Valuename==NULL||Value==NULL)) return -2;

  if(mode<2) ss=strlen(Value);
  if(RegOpenKey(HKEY_CLASSES_ROOT,"Directory",&hkr)==ERROR_SUCCESS){
    if(RegOpenKey(hkr,"shell",&hkr2)==ERROR_SUCCESS){
      if(mode==2){
        if(RegOpenKey(hkr2,SectionMain,&hkm)==ERROR_SUCCESS){
          if(SectionSub==NULL||RegOpenKey(hkm,SectionSub,&hks)==ERROR_SUCCESS){
            rc=1;
            if(SectionSub!=NULL) RegCloseKey(hks);
          }
          RegCloseKey(hkm);
        }
      }else{
        if(RegCreateKey(hkr2,SectionMain,&hkm)==ERROR_SUCCESS){
          if(SectionSub==NULL||RegCreateKey(hkm,SectionSub,&hks)==ERROR_SUCCESS){
            if(mode==1 && RegSetValueEx((SectionSub==NULL?hkm:hks),Valuename,0,REG_SZ,(const BYTE *)Value,ss)==ERROR_SUCCESS) rc=ss;
            else if(mode==0 && RegQueryValueEx((SectionSub==NULL?hkm:hks),Valuename,0,&vt,(BYTE *)Value,&vs)==ERROR_SUCCESS && vt==REG_SZ) rc=vs;
            if(SectionSub!=NULL) RegCloseKey(hks);
          }
          RegCloseKey(hkm);
        }
      }
      RegCloseKey(hkr2);
    }
    RegCloseKey(hkr);
  }
  return rc;
}
//==============================================================================
int wwrSetString(char *SectionMain, char *SectionSub, const char *Valuename, char *Value)
{
  return _wwrString(1,SectionMain,SectionSub,Valuename,Value,0);
}
//==============================================================================
int wwrGetString(char *SectionMain, char *SectionSub, const char *Valuename, char *Value, int MaxSize)
{
  return _wwrString(0,SectionMain,SectionSub,Valuename,Value,MaxSize);
}
//==============================================================================
//2019.09.13
int wwrCheckKey(char *SectionMain, char *SectionSub)
{
  return _wwrString(2,SectionMain,SectionSub,NULL,NULL,0);
}
//==============================================================================
