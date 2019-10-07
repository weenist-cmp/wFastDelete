#ifndef wFastDelete_private_H
#define wFastDelete_private_H

#define V_Major	  2019
#define V_Minor	  10
#define V_Release	3
#define V_Build 	1

#define _STR(n) #n
#define STR(n) _STR(n)

#define V_String STR(V_Major) "." STR(V_Minor) "." STR(V_Release) "." STR(V_Build)

#define P_Name        "wFastDelete"
#define F_Description "Fast!! Remove Dir & Delete File"
#define P_FileName    P_Name ".exe"

#define P_Copyright   "weenist"

#define ID_MainDialog   1000
#define ID_Message      1001

#define ID_cAutoClose   1011
#define ID_cSilence     1012
#define ID_cListMode    1013

#define ID_cShowWhileWorking    1014
#define ID_cShowWhileRemove     1015

#define ID_cbListMode   1021

#define ID_pbFF         1031

#define ID_b1           1091
#define ID_b2           1092

#define def_M_ShellMenu1    "폴더삭제"
#define def_M_ShellMenu2    "폴더정보"
#define def_M_RegisterMode  "등록"
#define def_M_RemoveMode    "삭제"
#define def_M_InfoMode      "정보"

#define def_M_Register1   "[%s]를 등록하려면 [%s]을 클릭해주세요"
#define def_M_Register2   "[%s]를 등록해제하려면 [%s]를 클릭해주세요"
#define def_M_Register3   "종료하려면 빈곳을 더블클릭해주세요"

#define def_M_Waring1     "모든 파일과 폴더가 삭제됩니다!!"
#define def_M_Waring2     "실행할까요?"

#define def_M_Result1     "폴더갯수: %s"
#define def_M_Result2     "파일갯수: %s"
#define def_M_Result3     "전체크기: %s"
#define def_M_Result4     "실행시간: %s(밀리초)"
#define def_M_Result5     "성공"
#define def_M_Result6     "실패"

#define def_M_AutoClose   "자동닫기"
#define def_M_Silence     "메세지 비표시"

#define def_B_Register    "등록"
#define def_B_UnRegister  "해제"
#define def_B_Yes         "예"
#define def_B_No          "아니오"
#define def_B_Close       "닫기"

#endif
