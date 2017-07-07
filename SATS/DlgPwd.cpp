// DlgPwd.cpp : 实现文件
//

#include "stdafx.h"
#include "SATS.h"
#include "DlgPwd.h"


// CDlgPwd 对话框

IMPLEMENT_DYNAMIC(CDlgPwd, CDialog)
CDlgPwd::CDlgPwd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPwd::IDD, pParent)
	, m_strPwd(_T(""))
{
}

CDlgPwd::~CDlgPwd()
{
}

void CDlgPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PWD, m_editPwd);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
}


BEGIN_MESSAGE_MAP(CDlgPwd, CDialog)
END_MESSAGE_MAP()


// CDlgPwd 消息处理程序
