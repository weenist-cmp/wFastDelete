//===========================================================================
//wFastDelete
//2016/02/16...Start, FileFind, FileSize 0, Delete
//2016/02/17...Recursive DeleteDirectory(), RemoveDirectory, MessageBox, Kor or Eng
//2016/02/20...Non-Ansi Character (TCHAR)
//2019/09/13...레지스트리 다이알로그 메세지등 필요한 루틴 테스트
//2019/09/14...Auto Registry, Language Ini File, silently_remove_directory
//2019/09/15...FileFind
//2019/09/16...Non-Ansi Path처리 마무리
/*
mbstowcs로 변환하면 이름이 외국문자인 일부 폴더에서 바로 실행할때 오류가 난다 '?' 이런외국문자코드가 들어있을때 바꾸면 제대로 바뀌지 않아 Path이름에서 오류가 발생한다.
전체 코드를 유니코드로 만드는건 너무 힘들고 WinMain함수의 LPSTR lpCmdLine 자체가 유니코드를 지원하지 않아서 wWinMain만 새로 만들어도 안된다.
해결한 방법은 GetCommandLineW, CommandLineToArgvW,lstrcpyW 이렇게 메인에서 처리하고
보여줄때 잠깐 mbs로 바꿔서 출력하고 FindFile과 RemoveDirectory함수는 wcs로 처리했다.
*/
//20190916...InfoMode DirectoryInfoMode FileInfoMode setlocale
//20190918...알아보기 쉽게 코드 재작성, 계단식메뉴, 목록저장추가
//20190920...MakeTestDirectory
//20190921...Wait until Remove, ShowWhileWorking, ShowWhileRemove
//20190924...ProgressBar, comma number string
//20191001...ShowWhileWorking,ShowWhileRemove Time
//20191004...자릿수 모자라서 오류로 강제 종료되는 문제 해결
//===========================================================================
#include <stdio.h>
//#include <stdlib.h>
#include <windows.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <Shlwapi.h>
#include <locale.h>
//===========================================================================
#include "wFastDelete_module.h"
#include "wFastDelete_FileFind.h"
#include "wFastDelete_private.h"
#include "wFastDelete_main.h"
//===========================================================================
#define RM_Register       0
#define RM_Remove         1
#define RM_Info           2
#define RM_List           3

#define RM_S  RM_Register
#define RM_E  RM_List

#define RM_ListDirectory  31
#define RM_ListFile       32
#define RM_ListFileSize   33

#define RM_LS RM_ListDirectory
#define RM_LE RM_ListFileSize
//===========================================================================
int RunMode,CheckReg,AutoCloseMillisecond,AutoClose,Silence,ListMode,ShowWhileWorking,ShowWhileRemove,CommaCount;
HWND hwm;
const char *fnfINI = "%s\\%s.ini";
const char *RN_Remove="Remove" , *RN_Info="Info" , *RN_List="List", *RN_Command="Command",
  *RN_MUIVerb="MUIVerb", *RN_StartDirectoryName="StartDirectoryName",
  *VN_Language="Language", *VN_Locale="Locale",
  *VN_MainMenu="MainMenu", *VN_MainMenuPosition="MainMenuPosition",
  *RN_icon="icon", *VN_iconM="iconM", *VN_icon1="icon1", *VN_icon2="icon2",
  *RN_SubCommands="SubCommands",*RN_Position="Position",
  *VN_AutoClose="AutoClose", *VN_AutoCloseMillisecond="AutoCloseMillisecond",
  *VN_Silence="Silence", *VN_ListMode="ListMode", *VN_ListModeStart="ListModeStart", *VN_ShowWhileWorking="ShowWhileWorking", *VN_ShowWhileRemove="ShowWhileRemove", *VN_CommaCount="CommaCount";
wchar_t WorkingDirectoryName[9999];
char fnINI[9999],StartDirectoryName[9999],Language[9999],CT_Locale[9999],//fnList[9999],
MainMenuName[9999],Position[9999],iconM[9999],icon1[9999],icon2[9999],
M_ShellMenu1[9999],M_ShellMenu2[9999],M_RegisterMode[9999],M_RemoveMode[9999],M_InfoMode[9999],
  M_Register1[9999],M_Register2[9999],M_Register3[9999],M_Waring1[9999],M_Waring2[9999],M_Result1[9999],M_Result2[9999],M_Result3[9999],M_Result4[9999],M_Result5[9999],M_Result6[9999],
  M_AutoClose[9999],M_Silence[9999],B_Register[9999],B_UnRegister[9999],B_Yes[9999],B_No[9999],B_Close[9999];
//===========================================================================
int Register(void)
{
  int cv;
  char ks[9999], str[9999];

//MainMenu
  sprintf(ks,"%s",P_Name);
  if(wwrSetString(ks, NULL, RN_MUIVerb, MainMenuName)<0) return -1;
  if(wwrSetString(ks, NULL, RN_icon, iconM)<0) return -2;
  if(wwrSetString(ks, NULL, RN_StartDirectoryName, StartDirectoryName)<0) return -3;
  if(wwrSetString(ks, NULL, RN_SubCommands, "")<0) return -4;
  if(wwrSetString(ks, NULL, RN_Position, "")<0) return -5;

//Remove
  sprintf(ks,"%s\\shell\\%s",P_Name,RN_Remove);
  if(wwrSetString(ks, NULL, RN_MUIVerb, M_ShellMenu1)<0) return -11;
  if(wwrSetString(ks, NULL, RN_icon, icon1)<0) return -12;
  sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_Remove,RN_Command);
  sprintf(str,"%s\\\\%s %d \"%%1\"",StartDirectoryName,P_FileName,RM_Remove);
  if(wwrSetString(ks, NULL, "", str)<0) return -13;

//Info
  sprintf(ks,"%s\\shell\\%s",P_Name,RN_Info);
  if(wwrSetString(ks, NULL, RN_MUIVerb, M_ShellMenu2)<0) return -21;
  if(wwrSetString(ks, NULL, RN_icon, icon2)<0) return -22;
  sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_Info,RN_Command);
  sprintf(str,"%s\\\\%s %d \"%%1\"",StartDirectoryName,P_FileName,RM_Info);
  if(wwrSetString(ks, NULL, "", str)<0) return -23;

//List
  if(SendMessage(GetDlgItem(hwm,ID_cListMode),BM_GETCHECK,0,0)==1){
    sprintf(ks,"%s\\shell\\%s",P_Name,RN_List);
    sprintf(str,"%s (LM)",M_ShellMenu2);
    if(wwrSetString(ks, NULL, RN_MUIVerb, str)<0) return -31;
    if(wwrSetString(ks, NULL, RN_icon, icon2)<0) return -32;
    sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_List,RN_Command);
    sprintf(str,"%s\\\\%s %d \"%%1\"",StartDirectoryName,P_FileName,RM_List);
    if(wwrSetString(ks, NULL, "", str)<0) return -33;
  }else{ // Unchecked List 삭제
    sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_List,RN_Command);
    wwrDeleteKey(ks, NULL);
    sprintf(ks,"%s\\shell\\%s",P_Name,RN_List);
    wwrDeleteKey(ks, NULL);
  }

//AutoClose
  cv=SendMessage(GetDlgItem(hwm,ID_cAutoClose),BM_GETCHECK,0,0);
  sprintf(str,"%d",(cv==1?1:0));
  if(wwiWriteString(fnINI, P_Name, VN_AutoClose, str)<0) return -41;

//Silence
  cv=SendMessage(GetDlgItem(hwm,ID_cSilence),BM_GETCHECK,0,0);
  sprintf(str,"%d",(cv==1?1:0));
  if(wwiWriteString(fnINI, P_Name, VN_Silence, str)<0) return -42;

//ListMode
  cv=SendMessage(GetDlgItem(hwm,ID_cListMode),BM_GETCHECK,0,0);
  sprintf(str,"%d",(cv==1?1:0));
  if(wwiWriteString(fnINI, P_Name, VN_ListMode, str)<0) return -43;

//ListModeStart
  cv=SendDlgItemMessage(hwm,ID_cbListMode,CB_GETCURSEL,0,0);
  if(cv<0||cv>(RM_LE-RM_LS)) cv=RM_LE-RM_LS;
  sprintf(str,"%d",cv+RM_LS);
  if(wwiWriteString(fnINI, P_Name, VN_ListModeStart, str)<0) return -44;

//ShowWhileWorking
  cv=SendMessage(GetDlgItem(hwm,ID_cShowWhileWorking),BM_GETCHECK,0,0);
  sprintf(str,"%d",(cv==1?1:0));
  if(wwiWriteString(fnINI, P_Name, VN_ShowWhileWorking, str)<0) return -45;

//ShowWhileRemove
  cv=SendMessage(GetDlgItem(hwm,ID_cShowWhileRemove),BM_GETCHECK,0,0);
  sprintf(str,"%d",(cv==1?1:0));
  if(wwiWriteString(fnINI, P_Name, VN_ShowWhileRemove, str)<0) return -46;

  return 1;
}
//===========================================================================
int UnRegister(void)
{
  char ks[9999];

  sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_Remove,RN_Command);
  wwrDeleteKey(ks, NULL);
  sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_Info,RN_Command);
  wwrDeleteKey(ks, NULL);
  sprintf(ks,"%s\\shell\\%s\\%s",P_Name,RN_List,RN_Command);
  wwrDeleteKey(ks, NULL);

  sprintf(ks,"%s\\shell\\%s",P_Name,RN_Remove);
  wwrDeleteKey(ks, NULL);
  sprintf(ks,"%s\\shell\\%s",P_Name,RN_Info);
  wwrDeleteKey(ks, NULL);
  sprintf(ks,"%s\\shell\\%s",P_Name,RN_List);
  wwrDeleteKey(ks, NULL);

  sprintf(ks,"%s\\shell",P_Name);
  wwrDeleteKey(ks, NULL);

  sprintf(ks,"%s\\",P_Name);
  return wwrDeleteKey(ks, NULL);
}
//===========================================================================
//191004...자릿수 모자라서 오류로 강제 종료되는 문제 해결
void FFCB_ShowWhileWorking(wchar_t *dn, DWORD cdct)
{
  char str[9999],s1[9999],s2[9999],s3[9999],s5[9999];

  sprintf(s1,M_Result1,CommaString(str,CommaCount,FF_DirectoryCount));
  if(RunMode==RM_Remove){
    s2[0]=0;s3[0]=0;
  }else{
    sprintf(s2,M_Result2,CommaString(str,CommaCount,FF_FileCount));
    sprintf(s3,M_Result3,CommaString64(str,CommaCount,FF_FileSize));
  }
  wcstombs(s5, dn, 9999); sprintf(str,"%s\n%s\n%s\n\n%s",s1,s2,s3,s5);

  SendDlgItemMessage(hwm,ID_Message,WM_SETTEXT,0,(LPARAM)str);
  SendMessage(GetDlgItem(hwm,ID_pbFF),PBM_SETPOS,cdct,0);
}
//===========================================================================
int RunInfo(void)
{
  int mdc,sww; // sww...191001
  char str[9999],s1[9999],s2[9999],s3[9999],s4[9999],s5[9999],s41[9999];//s41...191001
  DWORD wtcs,wtce,wtms,wtms1; //wtms1...191001

  if(RunMode>=RM_LS && RunMode<=RM_LE){
    OPENFILENAME OFN; char wfn[9999]="";
    memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner=hwm; OFN.lpstrFilter="Text File (*.txt)\0*.txt\0Any File (*.*)\0*.*\0";
		OFN.lpstrFile=wfn; OFN.nMaxFile=MAX_PATH; OFN.lpstrInitialDir="";	OFN.Flags=OFN_OVERWRITEPROMPT;
/* Flags List
OFN_ALLOWMULTISELECT //복수개의 파일 선택 가능.
이 플래그를 OFN_EXPLORER없이 사용하면 구형 대화상자가 열린다.
OFN_CREATEPROMPT // '파일을 생성할까요?' 대화상자 표시 여부
OFN_ENABLEHOOK // 훅 프로시저 사용
OFN_EXTENSIONDEFFERENT // 사용자가 입력한 확장자가 디폴트 확장자와 다를때 이 플래그가 셋팅된다.
OFN_FILEMUSTEXIST //존재하는 파일만 선택 가능
OFN_NOTESTFILECREATE //대화상자가 닫히기 전에 파일이 생성되지 않도록 한다. (무슨말인지?)
OFN_OVERWRITEPROMPT // 파일이 이미 존재할 경우 덮어쓰기 대화상자 표시
OFN_PATHMUSTEXIST //존재하는 경로와 파일만 선택 가능
*/

    if(GetSaveFileName(&OFN)==TRUE){
      FFFP = fopen(OFN.lpstrFile,"wt");
      setlocale(LC_ALL, CT_Locale);
      if(FFFP==INVALID_HANDLE_VALUE) return -1;
    }else return 0;
  }
  wtcs = GetTickCount();

  if( (RunMode==RM_Info || (RunMode>=RM_LS && RunMode<=RM_LE)) && ShowWhileWorking==1){
    sww=1;
    mdc = GetDirectoryCount(WorkingDirectoryName);
    wtce = GetTickCount(); wtms1 = (wtcs<=wtce?wtce-wtcs:wtce);
    SendMessage(GetDlgItem(hwm,ID_pbFF),PBM_SETRANGE32,0,(LPARAM)mdc);
  }else sww=0;

  FFM=RunMode; FFM_ShowWhileWorking=ShowWhileWorking;
  FileFind(WorkingDirectoryName,FFCB_Directory,FFCB_File,FFCB_ShowWhileWorking);
  wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);

  sprintf(s1,M_Result1,CommaString(str,CommaCount,FF_DirectoryCount)); sprintf(s2,M_Result2,CommaString(str,CommaCount,FF_FileCount)); sprintf(s3,M_Result3,CommaString64(str,CommaCount,FF_FileSize)); sprintf(s4,M_Result4,CommaString(str,CommaCount,wtms));
  wcstombs(s5, WorkingDirectoryName, 9999);
  if(sww==1){
    sprintf(s41,M_Result4,CommaString(str,CommaCount,wtms1));
    sprintf(str,"%s\n%s\n%s\n%s\n%s\n\n%s",s1,s2,s3,s41,s4,s5);
  }else sprintf(str,"%s\n%s\n%s\n%s\n\n%s",s1,s2,s3,s4,s5);

  if(RunMode>=RM_LS && RunMode<=RM_LE){
    fprintf(FFFP,"\n\n=====\n");
    fprintf(FFFP,str);
    fclose(FFFP);
  }

  SendDlgItemMessage(hwm,ID_Message,WM_SETTEXT,0,(LPARAM)str);
  return 1;
}
//===========================================================================
#define ReadString(ms) if(wwiReadString(fnINI, Language, STR(ms), str, 9999)>0) strcpy(ms,str); else strcpy(ms,def_ ## ms)
int Init(void)
{
  int cv;
  char str[9999],s1[9999],s2[9999];

//  sprintf(fnINI,fnfINI,StartDirectoryName,P_Name);
  if(wwiReadString(fnINI, P_Name, VN_Language, str, 9999)>0) strcpy(Language,str); else strcpy(Language,"기본한국어");
  if(wwiReadString(fnINI, P_Name, VN_Locale, str, 9999)>0) strcpy(CT_Locale,str); else strcpy(CT_Locale,"");

  if(wwiReadString(fnINI, P_Name, VN_MainMenu, str, 9999)>0) strcpy(MainMenuName,str); else strcpy(MainMenuName,P_Name);

//MessageBox(NULL,MainMenuName,MainMenuName,MB_OK);

  if(wwiReadString(fnINI, P_Name, VN_MainMenuPosition, str, 9999)>0) strcpy(Position,str); else strcpy(Position,"");

  if(wwiReadString(fnINI, P_Name, VN_iconM, str, 9999)>0) strcpy(iconM,str); else strcpy(iconM,"");
  if(wwiReadString(fnINI, P_Name, VN_icon1, str, 9999)>0) strcpy(icon1,str); else strcpy(icon1,"");
  if(wwiReadString(fnINI, P_Name, VN_icon2, str, 9999)>0) strcpy(icon2,str); else strcpy(icon2,"");

//MessageBox(NULL,icon2,icon2,MB_OK);

  ReadString(M_ShellMenu1); ReadString(M_ShellMenu2); ReadString(M_RegisterMode); ReadString(M_RemoveMode); ReadString(M_InfoMode);
  ReadString(M_Register1); ReadString(M_Register2); ReadString(M_Register3); ReadString(M_Waring1); ReadString(M_Waring2);
  ReadString(M_Result1); ReadString(M_Result2); ReadString(M_Result3); ReadString(M_Result4); ReadString(M_Result5); ReadString(M_Result6);
  ReadString(M_AutoClose); ReadString(M_Silence);
  ReadString(B_Register); ReadString(B_UnRegister);
  ReadString(B_Yes); ReadString(B_No); ReadString(B_Close);

//ShowWhileWorking
  if(wwiReadString(fnINI, P_Name, VN_ShowWhileWorking, str, 9999)>0){
    ShowWhileWorking=(atoi(str)==0 ? 0 : 1);
  }else ShowWhileWorking=1;

//ShowWhileRemove
  if(wwiReadString(fnINI, P_Name, VN_ShowWhileRemove, str, 9999)>0){
    ShowWhileRemove=(atoi(str)==1 ? 1 : 0);
  }else ShowWhileRemove=0;

//AutoCloseMillisecond
  if(wwiReadString(fnINI, P_Name, VN_AutoCloseMillisecond, str, 9999)>0){
    cv=atoi(str);
    if (cv<100||cv>60000) cv=5000;
  }else cv=5000;
  AutoCloseMillisecond=cv;

//CommaCount
  if(wwiReadString(fnINI, P_Name, VN_CommaCount, str, 9999)>0){
    cv=atoi(str);
    if (cv<0||cv>10) cv=0;
  }else cv=0;
  CommaCount=cv;

  SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)B_Close);

//Title
  sprintf(str,"%s - %s [%s]",P_Name,(RunMode==RM_Register?M_RegisterMode:(RunMode==RM_Remove?M_RemoveMode: M_InfoMode)),Language);
  SetWindowText( hwm, str);

//cbListMode
  if(RunMode==RM_Register||RunMode==RM_List){
    SendDlgItemMessage(hwm,ID_cbListMode,CB_ADDSTRING,0,(LPARAM)"Folder");
    SendDlgItemMessage(hwm,ID_cbListMode,CB_ADDSTRING,0,(LPARAM)"File");
    SendDlgItemMessage(hwm,ID_cbListMode,CB_ADDSTRING,0,(LPARAM)"FileSize");
    if(wwiReadString(fnINI, P_Name, VN_ListModeStart, str, 9999)>0){ cv=atoi(str); if (cv<RM_LS||cv>RM_LE) cv=RM_LE; }else cv=RM_LE;
//    SendDlgItemMessage(hwm,ID_cbListMode,CB_SETCURSEL,(cv-RM_LS),0);
    SendMessage(GetDlgItem(hwm,ID_cbListMode),CB_SETCURSEL,(cv-RM_LS),0);
  }

//CheckBox Check Status
  if(wwiReadString(fnINI, P_Name, VN_AutoClose, str, 9999)>0){ cv=atoi(str); AutoClose=cv; }
  if(wwiReadString(fnINI, P_Name, VN_Silence, str, 9999)>0){ cv=atoi(str); Silence=cv; }
  if(wwiReadString(fnINI, P_Name, VN_ListMode, str, 9999)>0){ cv=atoi(str); ListMode=cv; }
  if(wwiReadString(fnINI, P_Name, VN_ShowWhileWorking, str, 9999)>0){ cv=atoi(str); ShowWhileWorking=cv; }
  if(wwiReadString(fnINI, P_Name, VN_ShowWhileRemove, str, 9999)>0){ cv=atoi(str); ShowWhileRemove=cv; }

//CheckBox Hide
  if(RunMode!=RM_Register){
    ShowWindow(GetDlgItem(hwm,ID_cAutoClose),SW_HIDE);
    ShowWindow(GetDlgItem(hwm,ID_cSilence),SW_HIDE);
    ShowWindow(GetDlgItem(hwm,ID_cListMode),SW_HIDE);
    ShowWindow(GetDlgItem(hwm,ID_cShowWhileWorking),SW_HIDE);
    ShowWindow(GetDlgItem(hwm,ID_cShowWhileRemove),SW_HIDE);
  }

//pbFF Hide
  if(
     RunMode==RM_Register ||
     ((RunMode==RM_Info || RunMode==RM_List) && ShowWhileWorking==0) ||
     (RunMode==RM_Remove && ShowWhileRemove==0)){
    ShowWindow(GetDlgItem(hwm,ID_pbFF),SW_HIDE);
  }

  if(RunMode==RM_Remove || RunMode==RM_Info){
    ShowWindow(GetDlgItem(hwm,ID_cbListMode),SW_HIDE);
  }

  ShowWindow(hwm,SW_SHOW);

  if(RunMode==RM_Register){
//CheckBox Check
    SendMessage(GetDlgItem(hwm,ID_cAutoClose),BM_SETCHECK,(AutoClose==0?BST_UNCHECKED:BST_CHECKED),0);
    SendMessage(GetDlgItem(hwm,ID_cSilence),BM_SETCHECK,(Silence==0?BST_UNCHECKED:BST_CHECKED),0);
    SendMessage(GetDlgItem(hwm,ID_cListMode),BM_SETCHECK,(ListMode==0?BST_UNCHECKED:BST_CHECKED),0);
    SendMessage(GetDlgItem(hwm,ID_cShowWhileWorking),BM_SETCHECK,(ShowWhileWorking==0?BST_UNCHECKED:BST_CHECKED),0);
    SendMessage(GetDlgItem(hwm,ID_cShowWhileRemove),BM_SETCHECK,(ShowWhileRemove==0?BST_UNCHECKED:BST_CHECKED),0);

//CheckBox String
    SendDlgItemMessage(hwm,ID_cAutoClose,WM_SETTEXT,0,(LPARAM)M_AutoClose);
    SendDlgItemMessage(hwm,ID_cSilence,WM_SETTEXT,0,(LPARAM)M_Silence);

    SendDlgItemMessage(hwm,ID_b1,WM_SETTEXT,0,(LPARAM)B_Register); SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)(CheckReg>0?B_UnRegister:B_Close));

    sprintf(s1,M_Register1,M_ShellMenu1,B_Register); sprintf(s2,M_Register2,M_ShellMenu1,B_UnRegister); sprintf(str,"%s\n\n%s\n\n%s",s1,s2,def_M_Register3);
  }else if(RunMode==RM_Remove){
    SendDlgItemMessage(hwm,ID_b1,WM_SETTEXT,0,(LPARAM)B_Yes);
    SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)B_No);

    wcstombs(s1,WorkingDirectoryName,9999); sprintf(str,"%s\n\n%s\n\n%s",M_Waring1,s1,M_Waring2);
  }else if(RunMode==RM_Info){
    ShowWindow(GetDlgItem(hwm,ID_b1),SW_HIDE);
    SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)B_Close);

    EnableWindow(GetDlgItem(hwm,ID_b2),FALSE);
    RunInfo();
    EnableWindow(GetDlgItem(hwm,ID_b2),TRUE);
// Info 모드에선 자동 닫기 안하는게 좋아서
//    if(AutoClose==1){ Sleep(AutoCloseMillisecond); EndDialog(hwm,0); }
    return 1;
  }else if(RunMode==RM_List){
    SendDlgItemMessage(hwm,ID_b1,WM_SETTEXT,0,(LPARAM)"ListMake");
    SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)B_Close);

    wcstombs(str,WorkingDirectoryName,9999);
  }
//Message
  SendDlgItemMessage(hwm,ID_Message,WM_SETTEXT,0,(LPARAM)str);

  return 1;
}
//===========================================================================
void CheckBoxClick(int cbid)
{
  SendMessage(GetDlgItem(hwm,cbid),BM_SETCHECK,(SendMessage(GetDlgItem(hwm,cbid),BM_GETCHECK,0,0)==1?BST_UNCHECKED:BST_CHECKED),0);
  EnableWindow(GetDlgItem(hwm,ID_cAutoClose),(SendMessage(GetDlgItem(hwm,ID_cSilence),BM_GETCHECK,0,0)==1?FALSE:TRUE));
}
//===========================================================================
void ButtonClick(int btid)
{
  int rc,cv,mdc,swr; //swr...191001
  char str[9999],/*s1[9999],s2[9999],s3[9999],*/s4[9999],s5[9999],ks[9999],s41[9999]; //s41...191001
  DWORD wtcs,wtce,wtms,wtms1;//wtms1...191001

  sprintf(ks,"%s_RM%d",P_Name,RM_Remove);

  rc=0;
  if(RunMode==RM_Register){
    if(btid==ID_b1){
      rc=Register();
      if(rc>0){
        Beep(500,500); EndDialog(hwm,0);
      }else{
        Beep(1000,500); sprintf(str,"%s - Error %d",B_Register,rc);
      }
    }else if(btid==ID_b2){
      if(CheckReg<=0){ EndDialog(hwm,0); return; }
      rc=UnRegister();
      if(rc>=0){
        if(rc>0){ Beep(500,500); EndDialog(hwm,0); }
      }else{
        Beep(1000,500); sprintf(str,"%s - Error %d",B_UnRegister,rc);
      }
    }
    if(rc<0) MessageBox(hwm,str,P_Name,MB_OK);
  }else if(RunMode==RM_Remove){
    if(btid==ID_b1){
      EnableWindow(GetDlgItem(hwm,ID_b1),FALSE); EnableWindow(GetDlgItem(hwm,ID_b2),FALSE);
      if(Silence==1) ShowWindow(hwm,SW_HIDE);
      else{
        wcstombs(s5, WorkingDirectoryName, 9999); sprintf(str,"%s",s5);
        SendDlgItemMessage(hwm,ID_Message,WM_SETTEXT,0,(LPARAM)str);
      }

      FFM=RunMode;
      wtcs = GetTickCount();

      if(ShowWhileRemove==1){
        rc=0; swr=1;
        mdc = GetDirectoryCount(WorkingDirectoryName);
        wtce = GetTickCount(); wtms1 = (wtcs<=wtce?wtce-wtcs:wtce);
        SendMessage(GetDlgItem(hwm,ID_pbFF),PBM_SETRANGE32,0,(LPARAM)mdc);
        FFM=RunMode; FFM_ShowWhileRemove = ShowWhileRemove;
        FileFind(WorkingDirectoryName,FFCB_Directory,FFCB_File,FFCB_ShowWhileWorking);
      }else{
        swr=0;
        rc = cmdRemoveDirectory(WorkingDirectoryName);
        if(rc!=0) Beep(2000,500);
      }
      wtce = GetTickCount(); wtms = (wtcs<=wtce?wtce-wtcs:wtce);
      if(Silence==0){
//        sprintf(s1,M_Result1,FF_DirectoryCount); sprintf(s2,M_Result2,FF_FileCount); sprintf(s3,M_Result3,FF_FileSize);
        sprintf(s4,M_Result4,CommaString(str,CommaCount,wtms));
        wcstombs(s5, WorkingDirectoryName, 9999);
        if(swr==1){
          sprintf(s41,M_Result4,CommaString(str,CommaCount,wtms1));
          sprintf(str,"%s\n\n%s\n%s (%s)",s5,s41,s4,(rc==0 ? M_Result5 : M_Result6));
        }else sprintf(str,"%s\n\n%s (%s)",s5,s4,(rc==0 ? M_Result5 : M_Result6));
        SendDlgItemMessage(hwm,ID_Message,WM_SETTEXT,0,(LPARAM)str); SendDlgItemMessage(hwm,ID_b2,WM_SETTEXT,0,(LPARAM)B_Close);
        if(AutoClose==0) EnableWindow(GetDlgItem(hwm,ID_b2),TRUE);
      }
      if(AutoClose==1||Silence==1){
        if(AutoClose==1&&Silence==0) Sleep(AutoCloseMillisecond);
        EndDialog(hwm,0);
        return;
      }
    }else if(btid==ID_b2) EndDialog(hwm,0);
  }else if(RunMode==RM_Info){
    if(btid==ID_b2) EndDialog(hwm,0);
  }else if(RunMode==RM_List){
    if(btid==ID_b1){
      cv=SendDlgItemMessage(hwm,ID_cbListMode,CB_GETCURSEL,0,0);
      if(cv<0||cv>(RM_LE-RM_LS)) cv=(RM_LE-RM_LS);
      RunMode = RM_LS+cv;
      EnableWindow(GetDlgItem(hwm,ID_b1),FALSE); EnableWindow(GetDlgItem(hwm,ID_b2),FALSE);
      if(RunInfo()>0 && AutoClose==1){ EndDialog(hwm,0); return; }
      RunMode = RM_List;
      EnableWindow(GetDlgItem(hwm,ID_b1),TRUE); EnableWindow(GetDlgItem(hwm,ID_b2),TRUE);
    }else if(btid==ID_b2) EndDialog(hwm,0);
  }
}
//===========================================================================
static BOOL CALLBACK DialogFunc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
  if(GetAsyncKeyState(VK_ESCAPE)){ EndDialog(hwndDlg,0); return TRUE; }

	switch (msg){
    case WM_INITDIALOG:
      hwm = hwndDlg;
      Init();
      return TRUE;
    case WM_LBUTTONDBLCLK:
      EndDialog(hwndDlg,0); return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam)){
        case ID_cAutoClose:
        case ID_cSilence:
        case ID_cListMode:
        case ID_cShowWhileWorking:
        case ID_cShowWhileRemove:
          CheckBoxClick(wParam); break;
        case ID_b1:
        case ID_b2:
          ButtonClick(wParam); break;
      }
      break;
    case WM_CLOSE:
      EndDialog(hwndDlg,0); return TRUE;
  }
	return FALSE;
}
//===========================================================================
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE hinstPrev, LPSTR lpCmdLine, int nCmdShow)
{
  int wac,rmn;
	WNDCLASS wc;
	INITCOMMONCONTROLSEX cc;
  LPWSTR *wcs, wcl;
  char rms[9999];
  wchar_t ws1[9999],ws2[9999];

  setlocale(LC_ALL, "");

  CheckReg=(wwrCheckKey(P_Name, NULL)>0?1:0);
  if(CheckReg>0) wwrGetString(P_Name, NULL, RN_StartDirectoryName, StartDirectoryName, 9999);
  else GetCurrentDirectory(9999,StartDirectoryName);
  WorkingDirectoryName[0]=0;

//fnINI
  sprintf(fnINI,fnfINI,StartDirectoryName,P_Name);

//  MessageBox(NULL,fnINI,fnINI,MB_OK);

  wcl = GetCommandLineW(); wcs = CommandLineToArgvW(wcl,&wac);
  RunMode = RM_Register;
  if(wac>2){
    wcstombs(rms,wcs[1],9999);
    rmn = atoi(rms);
    if(rmn<RM_S||rmn>RM_E) rmn=RM_Info;
    RunMode = rmn;
    lstrcpyW(WorkingDirectoryName,wcs[2]);
    if(PathFileExistsW(WorkingDirectoryName)==FALSE){
      swprintf(ws1,L"Path [%ls] Not Found",WorkingDirectoryName);
      mbstowcs(ws2,P_Name,9999);
      MessageBoxW(NULL,ws1,ws2,MB_OK|MB_ICONINFORMATION);
      RunMode=RM_Register;
    };
  }
  LocalFree(wcs);

	memset(&wc,0,sizeof(wc)); wc.lpfnWndProc = DefDlgProc;wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hInstance = hinst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); wc.lpszClassName = P_Name;
	RegisterClass(&wc);	memset(&cc,0,sizeof(cc));	cc.dwSize = sizeof(cc);	cc.dwICC = 0xffffffff; InitCommonControlsEx(&cc);
	DialogBox(hinst, MAKEINTRESOURCE(ID_MainDialog), NULL, (DLGPROC) DialogFunc);

  return 0;
}
//===========================================================================
