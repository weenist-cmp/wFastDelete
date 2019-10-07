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

#define def_M_ShellMenu1    "��������"
#define def_M_ShellMenu2    "��������"
#define def_M_RegisterMode  "���"
#define def_M_RemoveMode    "����"
#define def_M_InfoMode      "����"

#define def_M_Register1   "[%s]�� ����Ϸ��� [%s]�� Ŭ�����ּ���"
#define def_M_Register2   "[%s]�� ��������Ϸ��� [%s]�� Ŭ�����ּ���"
#define def_M_Register3   "�����Ϸ��� ����� ����Ŭ�����ּ���"

#define def_M_Waring1     "��� ���ϰ� ������ �����˴ϴ�!!"
#define def_M_Waring2     "�����ұ��?"

#define def_M_Result1     "��������: %s"
#define def_M_Result2     "���ϰ���: %s"
#define def_M_Result3     "��üũ��: %s"
#define def_M_Result4     "����ð�: %s(�и���)"
#define def_M_Result5     "����"
#define def_M_Result6     "����"

#define def_M_AutoClose   "�ڵ��ݱ�"
#define def_M_Silence     "�޼��� ��ǥ��"

#define def_B_Register    "���"
#define def_B_UnRegister  "����"
#define def_B_Yes         "��"
#define def_B_No          "�ƴϿ�"
#define def_B_Close       "�ݱ�"

#endif
