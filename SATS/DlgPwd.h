#pragma once
#include "afxwin.h"


// CDlgPwd �Ի���

class CDlgPwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPwd)

public:
	CDlgPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPwd();

// �Ի�������
	enum { IDD = IDD_DIALOG_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editPwd;
	CString m_strPwd;
};
