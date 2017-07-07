#pragma once
#include "afxwin.h"


// CDlgPwd 对话框

class CDlgPwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPwd)

public:
	CDlgPwd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPwd();

// 对话框数据
	enum { IDD = IDD_DIALOG_PWD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editPwd;
	CString m_strPwd;
};
