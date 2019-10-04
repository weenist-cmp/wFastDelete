#ifndef wFastDelete_module_H
#define wFastDelete_module_H
int wwiReadString(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName, LPTSTR lpReturnedString, DWORD nSize);
int wwiWriteString(LPCTSTR lpFileName, LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
int wwrDeleteKey(char *SectionMain, char *SectionSub);
int wwrSetString(char *SectionMain, char *SectionSub, const char *Valuename, char *Value);
int wwrGetString(char *SectionMain, char *SectionSub, const char *Valuename, char *Value, int MaxSize);
int wwrCheckKey(char *SectionMain, char *SectionSub);
#endif
