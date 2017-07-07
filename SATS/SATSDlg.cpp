// SATSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SATS.h"
#include "SATSDlg.h"
#include  <stdio.h>
#include  <math.h>
#include  <stdlib.h>
#include ".\satsdlg.h"
#include "dlgpwd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:

};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

END_MESSAGE_MAP()


// CSATSDlg �Ի���



CSATSDlg::CSATSDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSATSDlg::IDD, pParent),CMySats(&m_listMsg,&m_listAlarm)
, m_dAutoSpeed(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_dSpeed=0.0;

	m_dAutoSpeed=0.0;

	m_nDmPosition=7;

	m_bSuperUsr=false;
	m_bWaitingToDeacc=false;
	m_bContinueBreakPoin=false;

	m_nStopPos=0;

	m_dBreakPoint=0.0;
	m_dBreakPointBak=0.0;

	m_dSatsVersion=SATS_UVER;

	m_bVersionCheck=false;

	m_nSimDirRecord=0;
	m_nDrvDirRecord=0;

	m_nAtoSpeed=0;

}

void CSATSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSATSDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_ACC, m_sldAcc);
	DDX_Text(pDX, IDC_EDIT_ACC, m_dAcc);
	DDX_Control(pDX, IDC_COMBO_DM, m_cbDm);
	DDX_Control(pDX, IDC_LIST_MSG, m_listMsg);
	DDX_Control(pDX, IDC_COMBO_DIR, m_cbDir);
	DDX_Control(pDX, IDC_COMBO_CAR, m_cbTrackCar);
	DDX_Control(pDX, IDC_COMBO_MOD, m_cbTrackMod);
	DDX_Control(pDX, IDC_COMBO_SIMDIR, m_cbSimDir);
	DDX_Control(pDX, IDC_LIST2, m_listAlarm);
	DDX_Control(pDX, IDC_CHECK_SIMRUN, m_btnSimRun);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition);
	DDX_Control(pDX, IDC_EDIT_DISTANCE, m_editDistance);
	DDX_Control(pDX, IDC_EDIT_START_POS, m_editStartPos);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editSpeed);
	DDX_Control(pDX, IDC_EDIT_GAP, m_editGap);
	DDX_Text(pDX, IDC_EDIT_AUTO_SPEED, m_dAutoSpeed);
	DDV_MinMaxDouble(pDX, m_dAutoSpeed, 0.0, 99.0);
	DDX_Control(pDX, IDC_CHECK_NOTRACK, m_checkNoTrack);
	DDX_Control(pDX, IDC_COMBO_CFG, m_cmbRouteCfg);
	DDX_Control(pDX, IDC_COMBO_ESI, m_cmbEsi);
	DDX_Control(pDX, IDC_EDIT_PORT, m_editPort);
	DDX_Control(pDX, IDC_EDIT_STOP_POS, m_editStopPos);
	DDX_Control(pDX, IDC_COMBO_HEADDIR, m_cmbHeadDir);
	DDX_Control(pDX, IDC_EDIT_VER, m_editSatsVer);
	DDX_Control(pDX, IDC_BTN_OUTTC, m_btnOutTc);
	DDX_Control(pDX, IDC_COMBO_DRVDIR, m_cmbDrvDir);
	DDX_Control(pDX, IDC_BTN_OUTDIR, m_btnOutDir);
	DDX_Control(pDX, IDC_EDIT_SPEED_ATO, m_editAtoSpeed);
	DDX_Control(pDX, IDC_EDIT_ACC_ATO, m_editAccAto);
	DDX_Control(pDX, IDC_CHECK_ATO_MODE, m_checkAtoMode);
	DDX_Control(pDX, IDC_LIST_VALUE, m_listCoreValue);
}

BEGIN_MESSAGE_MAP(CSATSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_SIMDIR, OnCbnSelchangeComboSimdir)
	ON_BN_CLICKED(IDC_BTN_POSSET, OnBnClickedBtnPosset)
	ON_BN_CLICKED(IDC_BTN_GAP, OnBnClickedBtnGap)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_MENU_USER, OnMenuUser)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_DIR, OnCbnSelchangeComboDir)
	ON_CBN_SELCHANGE(IDC_COMBO_DM, OnCbnSelchangeComboDm)
	ON_CBN_SELCHANGE(IDC_COMBO_CAR, OnCbnSelchangeComboCar)
	ON_CBN_SELCHANGE(IDC_COMBO_MOD, OnCbnSelchangeComboMod)
	ON_BN_CLICKED(IDC_CHECK_SIMRUN, OnBnClickedCheckSimrun)
	ON_COMMAND_RANGE(ID_COMMAND_POP_BEGIN,ID_COMMAND_POP_END,OnPopMyCommand)
	ON_BN_CLICKED(IDC_BTN_SET_ZERO_SPEED, OnBnClickedBtnSetZeroSpeed)
	ON_BN_CLICKED(IDC_BTN_AUTO_SPEED, OnBnClickedBtnAutoSpeed)
	ON_BN_CLICKED(IDC_CHECK_NOTRACK, OnBnClickedCheckNotrack)
	ON_CBN_SELCHANGE(IDC_COMBO_CFG, OnCbnSelchangeComboCfg)
	ON_COMMAND(ID_MENU_ESITEST, OnMenuEsitest)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACC, OnNMCustomdrawSliderAcc)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_COMMAND(ID_MENU_SIMRUN, OnMenuSimrun)
	ON_UPDATE_COMMAND_UI(ID_MENU_SIMRUN, OnUpdateMenuSimrun)
	ON_CBN_SELCHANGE(IDC_COMBO_HEADDIR, OnCbnSelchangeComboHeaddir)
	ON_BN_CLICKED(IDC_BTN_OUTTC, OnBnClickedBtnOuttc)
	ON_CBN_SELCHANGE(IDC_COMBO_DRVDIR, OnCbnSelchangeComboDrvdir)
	ON_BN_CLICKED(IDC_BTN_OUTDIR, OnBnClickedBtnOutdir)
	ON_BN_CLICKED(IDC_CHECK_ATO_MODE, OnBnClickedCheckAtoMode)
	ON_BN_CLICKED(IDC_CHECK_TIR, &CSATSDlg::OnBnClickedCheckTir)
	ON_BN_SETFOCUS(IDC_CHECK_TIR, &CSATSDlg::OnBnSetfocusCheckTir)
END_MESSAGE_MAP()


// CSATSDlg ��Ϣ�������

BOOL CSATSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SatsCreate();
	
	CString str;

	str.Format("%d",m_nAtoCoe);
	GetDlgItem(IDC_EDIT_ATOCOE)->SetWindowText(str);

	m_editGap.SetWindowText("100");
	str.Format("%d",m_dStartPos/10);
	m_editStartPos.SetWindowText(str);

	m_sldAcc.SetRange(1,13);
	m_sldAcc.SetPos(7);

	m_cbDm.AddString("774.4");
	m_cbDm.AddString("783.2");
	m_cbDm.AddString("791.9");
	m_cbDm.AddString("800.7");
	m_cbDm.AddString("809.0");
	m_cbDm.AddString("818.2");
	m_cbDm.AddString("826.9");
	m_cbDm.AddString("835.7");
	m_cbDm.SetCurSel(7);
	
	m_cbDir.AddString("����");
	m_cbDir.AddString("����");
	m_cbDir.SetCurSel(0);

	m_cmbHeadDir.AddString("��");
	m_cmbHeadDir.AddString("��");
	m_cmbHeadDir.SetCurSel(1);
	m_bTrainHeadRight=true;

	m_cmbDrvDir.AddString("ǰ��");
	m_cmbDrvDir.AddString("����");
	m_cmbDrvDir.SetCurSel(0);

	m_cbTrackCar.AddString("F0");
	m_cbTrackCar.AddString("F1");
	m_cbTrackCar.AddString("F2");
	m_cbTrackCar.AddString("F3");
	m_cbTrackCar.AddString("F4");
	m_cbTrackCar.AddString("F5");
	m_cbTrackCar.AddString("F6");
	m_cbTrackCar.AddString("F7");
	m_cbTrackCar.AddString("F9");
	m_cbTrackCar.SetCurSel(0);


	m_cbTrackMod.AddString("M0");
	m_cbTrackMod.AddString("M1");
	m_cbTrackMod.AddString("M2");
	m_cbTrackMod.AddString("M3");
	m_cbTrackMod.AddString("M4");
	m_cbTrackMod.AddString("M5");
	m_cbTrackMod.AddString("M6");
	m_cbTrackMod.AddString("M7");
	m_cbTrackMod.AddString("M8");
	m_cbTrackMod.AddString("M9");
	m_cbTrackMod.AddString("M10");
	m_cbTrackMod.AddString("M11");
	m_cbTrackMod.AddString("M12");
	m_cbTrackMod.AddString("M13");
	m_cbTrackMod.AddString("M14");
	m_cbTrackMod.SetCurSel(0);

	m_cmbEsi.AddString("1.2");
	m_cmbEsi.AddString("1.1");
	m_cmbEsi.AddString("1.0");
	m_cmbEsi.AddString("0.9");
	m_cmbEsi.AddString("0.8");
	m_cmbEsi.AddString("0.7");
	m_cmbEsi.AddString("0.6");
	m_cmbEsi.AddString("0.5");
	m_cmbEsi.AddString("0.4");
	m_cmbEsi.AddString("0.3");
	m_cmbEsi.AddString("0.2");
	m_cmbEsi.AddString("0.1");
	m_cmbEsi.SetCurSel(0);


	m_cbSimDir.AddString("-->");
	m_cbSimDir.AddString("<--");

	m_cbSimDir.SetCurSel(0);

	SetTimer(1000,3000,NULL);
	SetTimer(100,TIMER1,NULL);

	m_cmbRouteCfg.AddString("Ĭ��");
	for(int i=0;i<m_nModulationSum;i++)
	{
		m_cmbRouteCfg.AddString(m_szModulationNames[i]);
	}
	m_cmbRouteCfg.SetCurSel(0);


	m_hHandCursor=AfxGetApp()->LoadCursor(IDC_CURSOR_HAND);
	m_hHandArror=AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_btnOutTc.EnableWindow(false);
	m_btnOutDir.EnableWindow(false);

	m_editPosition.SetWindowText("");
	m_editAtoSpeed.SetWindowText("");

	AddValueList();

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CSATSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSATSDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		MyDraw(dc);

		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CSATSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSATSDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int n=m_sldAcc.GetPos();
	CString str;

	if (n>7)
	{
		m_dAcc=(n-7)/6.0*m_dMinAcc;
		m_dAcc=((int)(m_dAcc*10.0-0.5))/10.0;
		str.Format("��ʻ�ֱ��ı� %d .",n-7);
		AddMsg(str);
	}
	else 
	{
		m_dAcc=(7-n)/6.0*m_dMaxAcc;
		m_dAcc=((int)(m_dAcc*10.0+0.5))/10.0;
		str.Format("��ʻ�ֱ��ı� %d .",7-n);
		AddMsg(str);
	}


	UpdateData(FALSE);


	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSATSDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static int nCountIn=0;
	static double dLastSpeed=0;
	static double dLastDistance=0;
	static DWORD dwLast=GetTickCount();

	CString str,str1;
	
	if (nIDEvent==1000)
	{
		if (m_checkNoTrack.GetCheck())
		{
			m_bRenewGap=true;
			m_editGap.SetWindowText("5000");
			m_nGap=5000;
		}

		CDialog::OnTimer(nIDEvent);

		return;
	}


	DWORD dwNow=GetTickCount();
	DWORD dwSpan=dwNow-dwLast;
	dwLast=dwNow;

//	m_dSpeed+=m_dAcc/(1000.0/TIMER1)*3.6;								//2007.01.11 changed by wmz for richard require
//	m_dSpeed+=m_dAcc/(1000.0/TIMER1)*3.6*(m_nAccRate/100.0);			//2007.02.08 changed by wmz for acc rate
	m_dSpeed+=m_dAcc*(dwSpan/1000.0)*3.6*(m_nAccRate/100.0);	//2007.06.12 changed by wmz for acc rate

	if (m_dSpeed<0.01)		//change from 0.0 to 0.01 because perhaps the speed is very low and cal by the sats cause the very slow speed increase. 
	{
		m_dSpeed=0.0;
		if (m_dAcc<0.0)
		{
			m_dAcc=0.0;
			SetDlgItemText(IDC_EDIT_ACC,"0.0");
		}
	}

	if (m_bSimRun)
	{
		m_dDistance+=m_dSpeed/3.6*dwSpan/1000.0;
	}


	m_dPosition=m_dPositionLast+(m_dDistance-m_dDistanceLast)*m_dDirValue;


	TimerTask();

	if (m_dSpeed>100.0)
	{
		m_dSpeed=99.0;
		m_dAcc=0.0;
		GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
		((CSliderCtrl *) GetDlgItem(IDC_SLIDER_ACC))->SetPos(8);
	}

	int nAcc=((CSliderCtrl *) GetDlgItem(IDC_SLIDER_ACC))->GetPos();
	//if (m_dSpeed>m_dAutoSpeed && m_dAutoSpeed>1.0)
	//{
	//	m_dAcc=0.0;
	//	GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
	//	((CSliderCtrl *) GetDlgItem(IDC_SLIDER_ACC))->SetPos(8);
	//	m_dSpeed=m_dAutoSpeed;
	//}

	if (m_dSpeed>m_dAutoSpeed && m_dAutoSpeed>1.0)
	{
		m_dAcc=0.0;
		GetDlgItem(IDC_EDIT_ACC)->SetWindowText("0.0");
		((CSliderCtrl *) GetDlgItem(IDC_SLIDER_ACC))->SetPos(7);
		m_dSpeed=m_dAutoSpeed;
	}

	if (m_dSpeed<0.01 && !m_cbSimDir.IsWindowEnabled())
	{
		m_cbSimDir.EnableWindow(TRUE);
		m_cmbDrvDir.EnableWindow(TRUE);
	}
	else if (m_dSpeed>0.1 && m_cbSimDir.IsWindowEnabled())
	{
		m_cbSimDir.EnableWindow(FALSE);
		m_cmbDrvDir.EnableWindow(FALSE);
	}

	str.Format("%.1f",m_dSpeed);
	m_editSpeed.SetWindowText(str);

	str.Format("%.1f",m_dDistance);
	m_editDistance.SetWindowText(str);
	
	if (m_nDirPara)
	{
		str.Format("%.1f",-1*(m_dPosition+m_dStartPosBak));		//wmz changed on 20070208
	}
	else
	{
		str.Format("%.1f",m_dPosition+m_dStartPosBak);		//wmz changed on 20070208
	}
	m_editPosition.SetWindowText(str);

	if (fabs(dLastSpeed-m_dSpeed)>=0.1 || fabs(dLastDistance-m_dPosition)>=1)
	{
		str1.Format("S=%.1f D=%0.f",m_dSpeed,m_dPosition);
		AddAlarm(str1);
		dLastSpeed=m_dSpeed;
		dLastDistance=m_dPosition;
	}

	if (m_dPosition>m_dEndPos)
	{	
		m_bRunToRight=false;
		m_dDirValue=-1.0;
		m_dDistanceLast=m_dDistance;
//		m_dPositionLast=m_dPosition;
		m_dPositionLast=m_dEndPos;
		m_cbSimDir.SetCurSel(1);
		m_cmbDrvDir.SetCurSel(0);
	}

	if (m_dPosition<m_dStartPos)
	{
		m_bRunToRight=true;
		m_dDirValue=1.0;
		m_dDistanceLast=m_dDistance;
//		m_dPositionLast=m_dPosition;
		m_dPositionLast=m_dStartPos;
		m_cbSimDir.SetCurSel(0);
		m_cmbDrvDir.SetCurSel(0);
	}		


	int x1=(int)(m_nX0+m_dPosition*m_dScale);
	InvalidateRect(CRect(x1-40,m_nY0-15,x1+40,m_nY0));


	((CButton*)GetDlgItem(IDC_CHECK_ESI))->GetWindowText(str);
	if (m_dbSatsPort & 0x04)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_ESI))->GetCheck()!=BST_CHECKED)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ESI))->SetCheck(TRUE);
			ApplyEsi();
		}
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_ESI))->GetCheck()==BST_CHECKED)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ESI))->SetCheck(FALSE);
			CancelEsi();
		}
	}

	
	if ((m_dbSatsPort & 0x02)==0)
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_TIR))->GetCheck()!=BST_CHECKED)
		{
			((CButton*)GetDlgItem(IDC_CHECK_TIR))->SetCheck(TRUE);
			str.Format("ʵʩ TIR %3.1f CURSPEED:%3.1f CURPOS:%4.1f",m_dAcc,m_dSpeed,m_dPosition+m_dStartPosBak);
			AddMsg(str);
		}
	}
	else
	{
		if (((CButton*)GetDlgItem(IDC_CHECK_TIR))->GetCheck()==BST_CHECKED)
		{
			((CButton*)GetDlgItem(IDC_CHECK_TIR))->SetCheck(FALSE);
			str.Format("ȡ�� TIR %3.1f CURSPEED:%3.1f CURPOS:%4.1f",m_dAcc,m_dSpeed,m_dPosition+m_dStartPosBak);
			AddMsg(str);
		}
	}

	char chsAto[4]={ 'N','T','B','C'};

	if (nCountIn==0 || 
		m_dbSatsPort!=m_dbSatsLastPort		|| 
		m_dbSatsPort2!=m_dbSatsLastPort2	||
		m_dbSatsPort3!=m_dbSatsLastPort3	||
		m_dbSatsPort4!=m_dbSatsLastPort4	||
		m_dbSatsPort5!=m_dbSatsLastPort5    ||
		m_dbSatsPort6!=m_dbSatsLastPort6)
	{	
		CString str;
		str.Format("%02X %02X %02X %02X",m_dbSatsPort,m_dbSatsPort2,m_dbSatsPort3,m_dbSatsPort4);
		m_editPort.SetWindowText(str);

		if (m_dbSatsPort2&0x01)
		{
			((CButton *)GetDlgItem(IDC_CHECK_FR))->SetCheck(TRUE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CHECK_FR))->SetCheck(FALSE);
		}

		if (m_dbSatsPort2&0x02)
		{
			((CButton *)GetDlgItem(IDC_CHECK_MAR))->SetCheck(TRUE);
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CHECK_MAR))->SetCheck(FALSE);
		}

		m_dbSatsLastPort=m_dbSatsPort;
		m_dbSatsLastPort2=m_dbSatsPort2;
		m_dbSatsLastPort3=m_dbSatsPort3;
		m_dbSatsLastPort4=m_dbSatsPort4;
		m_dbSatsLastPort5=m_dbSatsPort5;
		m_dbSatsLastPort6=m_dbSatsPort6;

		double dAcc=m_dbSatsPort4;
		if (m_dbSatsPort3==2) dAcc*=-1;
		str.Format("%.1f",dAcc/100.0);
		m_editAccAto.SetWindowText(str);
		str.Format("%c %2.1f",chsAto[m_dbSatsLastPort3%4],dAcc/100.0);
		((CEdit *)GetDlgItem(IDC_EDIT_SATS_ACC))->SetWindowText(str);

		dAcc=m_dbSatsPort6;
		if (m_dbSatsPort5==2) dAcc*=-1;
		str.Format("%c %2.1f",chsAto[m_dbSatsLastPort5%4],dAcc/100.0);
		((CEdit *)GetDlgItem(IDC_EDIT_ATO_ACC))->SetWindowText(str);

	}
		
	nCountIn++;


	if (m_bWaitingToDeacc)
	{
		TimerTask_StartBreakPoint();
	}
	if (m_bContinueBreakPoin)
	{
		TimerTask_ContinueBreakPoint();
	}

	if (m_nSatsVersion!=m_nSatsLastVersion)
	{	VersionCheck();
		
		if (m_bVersionCheck)
		{
			str.Format("DVER:%d  UVER:%.2f Pass Version Check",m_nSatsVersion,m_dSatsVersion);
			AddMsg(str);
		}
		else
		{
			str.Format("DVER:%d  UVER:%.2f Fail to version check",m_nSatsVersion,m_dSatsVersion);
			AddMsg(str);
		}
	
		m_editSatsVer.SetWindowText(str);

		m_nSatsLastVersion=m_nSatsVersion;
	}


	m_editAtoSpeed.GetWindowText(str);
	int nSpeed=atof(str.GetBuffer())*10.0;
	if (nSpeed!=m_nAtoSpeed)
	{
		str.Format("%0.1f",	m_nAtoSpeed/10.0);
		m_editAtoSpeed.SetWindowText(str);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSATSDlg::OutDir(void)
{
	m_btnOutDir.EnableWindow(false);

//
	CString str,str1;
	m_cbSimDir.GetWindowText(str1);

	m_bJustChangeDir=true;
	m_dDistanceLast=m_dDistance;
	m_dPositionLast=m_dPosition;
	
	if (m_nSimDirRecord==0)
	{
		m_bRunToRight=true;
		m_dDirValue=1.0;
	}
	else if (m_nSimDirRecord==1)
	{
		m_bRunToRight=false;
		m_dDirValue=-1.0;
	}

	str.Format("�����г����з���-%s",str1);
	AddMsg(str);

//
	m_cmbDrvDir.GetWindowText(str1);
	if (m_nDrvDirRecord==0)
	{	
		m_bRunBack=false;
	}
	else
	{
		m_bRunBack=true;
	}

	str.Format("�����г���ʻ����-%s",str1);
	AddMsg(str);


	OnCbnSelchangeComboHeaddir();


	Invalidate();	
}


void CSATSDlg::OnCbnSelchangeComboSimdir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnOutDir.EnableWindow(true);

	m_nSimDirRecord=m_cbSimDir.GetCurSel();
}


void CSATSDlg::OnBnClickedBtnPosset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_editStartPos.GetWindowText(str);
	int nPos=atoi(str.GetBuffer());
	if (m_editStartPos)
	{
		nPos*=-1;
	}

	if (nPos<(int)m_dStartPosBak)
	{
		str.Format("��������λ�� %d �� С�������ļ���ʼλ�� %d �ף�����޷���ɡ�",nPos,(int)m_dStartPosBak);
//		AddMsg(str);
		AfxMessageBox(str,MB_ICONSTOP);
	}
	else 
	{
		m_dDistanceLast=m_dDistance;
		m_dPositionLast=nPos-m_dStartPosBak;
		m_dPosition=m_dPositionLast;

		str.Format("������ʼ�г�λ��:%d",nPos);
		AddMsg(str);
		Invalidate();
	}
}

void CSATSDlg::OnBnClickedBtnGap()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bSendGap=true;

	CString str;
	m_editGap.GetWindowText(str);
	m_nGap=atoi(str.GetBuffer());

	str.Format("�������϶:%0.d ����",m_nGap);
	AddMsg(str);
}


void CSATSDlg::OnAppAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg ad;

	ad.DoModal();
}

void CSATSDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int n=SearchDevice(point.x,point.y);

	if (n!=-1)
	{
		::SetCursor(m_hHandCursor);
	}


	CDialog::OnMouseMove(nFlags, point);
}


void CSATSDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_nCurDevice==-1) 
	{
		return;
	}

	CRect rect1,rect2;
	GetClientRect(&rect1);
	rect2=rect1;
	ClientToScreen(&rect2);
	int x=rect2.left-rect1.left;
	int y=rect2.top-rect1.top;


	UINT nMenuID=0;
	CMenu menu;
	CMenu *pMenu=NULL;

	if (IsArp())
	{
		menu.LoadMenu(IDR_MENU_APR);
	}
	else if (IsTrack())
	{
		menu.LoadMenu(IDR_MENU_TRACK);
	}
	else
	{
		return;
	}

	pMenu=menu.GetSubMenu(0);
//	pMenu->EnableMenuItem(ID_CAD_ADD,MF_GRAYED);
//	pMenu->EnableMenuItem(ID_CAD_EDIT,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	
	CString str,strDevName="��";
	if (m_nCurDevice!=-1)
	{
		strDevName=m_stuDevs.stuDevs[m_nCurDevice].szName;
	}
	str.Format("���ƣ�%s",strDevName);
	pMenu->ModifyMenu(0,MF_BYPOSITION,0,str);

	if (pMenu)
	{
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,	point.x+x, point.y+y,this);
		pMenu->DestroyMenu();
		menu.DestroyMenu();
	}


	CDialog::OnRButtonDown(nFlags, point);
}

void CSATSDlg::OnCbnSelchangeComboDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nTechoDir=(m_cbDir.GetCurSel()+1)%2;

	CString str;
	
	if(m_nTechoDir)
	{
		AddMsg("�˹����ò��ٵ������-����");
	}
	else
	{
		AddMsg("�˹����ò��ٵ������-����");
	}
}

void CSATSDlg::OnCbnSelchangeComboDm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nDmPosition=m_cbDm.GetCurSel();
}

void CSATSDlg::OutputTc(void)
{
	if (m_nTrackCar!=m_nTrackCarRecord)
	{
		m_nTrackCar=m_nTrackCarRecord;

		if (m_nTrackCar)
		{
			for(int i=0;i<m_stuDevs.nDevSum;i++)
			{
				if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
				{	
					m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]=m_nTrackCar;
				}
			}
		}
		else
		{
			for(int i=0;i<m_stuDevs.nDevSum;i++)
			{
				if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
				{	
					m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]=0;
				}
			}
		}

		CString str,str1;
		m_cbTrackCar.GetWindowText(str1);
		str.Format("�ı���Ƶ:%s",str1);
		AddMsg(str);
	}

	if (m_nTrackMod!=m_nTrackModRecord)
	{	m_nTrackMod=m_nTrackModRecord;

		if (m_nTrackMod)
		{
			for(int i=0;i<m_stuDevs.nDevSum;i++)
			{
				if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
				{	
					m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=m_nTrackMod;
				}
			}
		}
		else
		{
			for(int i=0;i<m_stuDevs.nDevSum;i++)
			{
				if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
				{	
					m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=0;
				}
			}
		}

		CString str,str1;
		m_cbTrackMod.GetWindowText(str1);
		str.Format("�ı����Ƶ��:%s",str1);
		AddMsg(str);
	}

	m_btnOutTc.EnableWindow(false);

	Invalidate();
}
void CSATSDlg::OnCbnSelchangeComboCar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnOutTc.EnableWindow(true);
	m_nTrackCarRecord=m_cbTrackCar.GetCurSel();
		
	//m_nTrackCar=m_cbTrackCar.GetCurSel();

	//if (m_nTrackCar)
	//{
	//	for(int i=0;i<m_stuDevs.nDevSum;i++)
	//	{
	//		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
	//		{	
	//			m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]=m_nTrackCar;
	//		}
	//	}
	//}
	//else
	//{
	//	for(int i=0;i<m_stuDevs.nDevSum;i++)
	//	{
	//		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
	//		{	
	//			m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]=0;
	//		}
	//	}
	//}

	//CString str,str1;
	//m_cbTrackMod.GetWindowText(str1);
	//str.Format("�ı���Ƶ:%s",str1);
	//AddMsg(str);

	//Invalidate();
}

void CSATSDlg::OnCbnSelchangeComboMod()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnOutTc.EnableWindow(true);
	m_nTrackModRecord=m_cbTrackMod.GetCurSel();

	//m_nTrackMod=m_cbTrackMod.GetCurSel();

	//if (m_nTrackMod)
	//{
	//	for(int i=0;i<m_stuDevs.nDevSum;i++)
	//	{
	//		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
	//		{	
	//			m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=m_nTrackMod;
	//		}
	//	}
	//}
	//else
	//{
	//	for(int i=0;i<m_stuDevs.nDevSum;i++)
	//	{
	//		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
	//		{	
	//			m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=0;
	//		}
	//	}
	//}

	//CString str,str1;
	//m_cbTrackCar.GetWindowText(str1);
	//str.Format("�ı����Ƶ��:%s",str1);
	//AddMsg(str);

	//Invalidate();
}


void CSATSDlg::OnPopMyCommand(UINT nID)
{	CString str;

	switch(nID)
	{
		case ID_TRACK_M0:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=0;
			str.Format("%s ����Ĭ�ϵ���Ƶ��",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M1:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=1;
			str.Format("%s ���õ���Ƶ��M1",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M2:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=2;
			str.Format("%s ���õ���Ƶ��M2",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M3:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=3;
			str.Format("%s ���õ���Ƶ��M3",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M4:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=4;
			str.Format("%s ���õ���Ƶ��M4",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M5:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=5;
			str.Format("%s ���õ���Ƶ��M5",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M6:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=6;
			str.Format("%s ���õ���Ƶ��M6",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M7:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=7;
			str.Format("%s ���õ���Ƶ��M7",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M8:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=8;
			str.Format("%s ���õ���Ƶ��M8",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M9:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=9;
			str.Format("%s ���õ���Ƶ��M9",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M10:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=10;
			str.Format("%s ���õ���Ƶ��M10",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M11:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=11;
			str.Format("%s ���õ���Ƶ��M11",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M12:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=12;
			str.Format("%s ���õ���Ƶ��M12",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M13:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=13;
			str.Format("%s ���õ���Ƶ��M13",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_M14:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntModSet]=14;
			str.Format("%s ���õ���Ƶ��M14",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F1:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=1;
			str.Format("%s ������ƵF1",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F2:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=2;
			str.Format("%s ������ƵF2",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F3:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=3;
			str.Format("%s ������ƵF3",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F4:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=4;
			str.Format("%s ������ƵF4",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F5:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=5;
			str.Format("%s ������ƵF5",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F6:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=6;
			str.Format("%s ������ƵF6",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F7:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=7;
			str.Format("%s ������ƵF7",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F8:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=0;
			str.Format("%s ����Ĭ����Ƶ",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_F9:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=8;
			str.Format("%s ������ƵF9",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_TRACK_DEL:
			m_stuDevs.stuDevs[m_nCurDevice].nPara[eTrackIntCarSet]=10;
			str.Format("%s ȡ����Ƶ���",m_stuDevs.stuDevs[m_nCurDevice].szName);
			AddMsg(str);
			break;
		case ID_APR_MISSING:
			{
				m_stuDevs.stuDevs[m_nCurDevice].nPara[eAprIntMissing]=(m_stuDevs.stuDevs[m_nCurDevice].nPara[eAprIntMissing]+1)%2;
			}
			break;
		default:
			break;
	}

	Invalidate();
}

void CSATSDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

//	__super::OnOK();
}

void CSATSDlg::OnBnClickedBtnSetZeroSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dSpeed=0.0;

	AddMsg("ǿ���ٶ�Ϊ��");
}

void CSATSDlg::OnBnClickedBtnAutoSpeed()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CString str;

	if (m_dAutoSpeed<=0.00001)
	{
		str.Format("ȡ���Զ�����");
	}
	else 
	{
		str.Format("�����Զ�����:%0.1f KM/H",m_dAutoSpeed);
	}

	AddMsg(str);
}

void CSATSDlg::OnBnClickedCheckNotrack()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkNoTrack.GetCheck())
	{
		AddMsg("��ֹ��������");
	}
	else
	{
		AddMsg("�����������");
	}
}

void CSATSDlg::OnCbnSelchangeComboCfg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=m_cmbRouteCfg.GetCurSel();

	if (n)
	{
		for(int i=0;i<m_stuDevs.nDevSum;i++)
		{
			if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
			{	
				m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=m_stuDevs.stuDevs[i].nModulation[n-1];
			}
		}
	}
	else
	{
		for(int i=0;i<m_stuDevs.nDevSum;i++)
		{
			if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
			{	
				m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]=0;
			}
		}
	}

	CString str,str1;
	m_cmbRouteCfg.GetWindowText(str1);
	str.Format("����ѡ��:%s",str1);
	AddMsg(str);

	Invalidate();


}

void CSATSDlg::OnBnClickedCheckTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=m_btnTest.GetCheck();

	if (n)
	{
		m_bTest=true;
	}
	else
	{
		m_bTest=false;
	}
}

void CSATSDlg::OnMenuUser()
{
	// TODO: �ڴ���������������
	CDlgPwd dp;

	if (dp.DoModal()==IDOK)
	{		
		if (dp.m_strPwd=="bdcsrd")
		{
			m_bSuperUsr=true;
			SetDlgItemText(IDC_STATIC_USER,"SUPER");
		}
		else
		{
			SetDlgItemText(IDC_STATIC_USER,"");
			m_bSuperUsr=false;
		}
	}
	else
	{
		SetDlgItemText(IDC_STATIC_USER,"");
		m_bSuperUsr=false;
	}
}

void CSATSDlg::OnMenuEsitest()
{
	// TODO: �ڴ���������������
	if (m_bSuperUsr)
	{
		ApplyEsi();
	}
}
void CSATSDlg::ApplyEsi(void)
{	CString str;

	m_cmbEsi.GetWindowText(str);

	m_dAcc=-1.0*atof(str.GetBuffer());
	

	str.Format("%2.1f",m_dAcc);
	SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());

	m_sldAcc.SetPos(7);

	str.Format("ʵʩ�����ƶ� ESI %3.1f CURSPEED:%3.1f CURPOS:%4.1f",m_dAcc,m_dSpeed,m_dPosition+m_dStartPosBak);

	AddMsg(str);
}
void CSATSDlg::CancelEsi(void)
{	CString str;

	m_dAcc=0.0;
	
	str.Format("%2.1f",m_dAcc);
	SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());

	m_sldAcc.SetPos(7);

	str.Format("ȡ�������ƶ� %2.1f CURSPEED:%3.1f CURPOS:%4.1f",m_dAcc,m_dSpeed,m_dPosition+m_dStartPosBak);
	AddMsg(str);
}
void CSATSDlg::OnNMCustomdrawSliderAcc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CSATSDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int i=0;
	
	CString str;

	m_editStopPos.GetWindowText(str);

	m_nStopPos=atoi(str.GetBuffer());
	if (m_nDirPara)
	{
		m_nStopPos*=-1;
	}
	
	double s=(m_nStopPos-(m_dPosition+m_dStartPosBak)-STOP_POINT_ERROR)*m_dDirValue;	//20070812 -20 
	double a=(m_dSpeed/3.6)*(m_dSpeed/3.6)/(2*s);
	
	if (m_dSpeed<0.1)
	{
		str.Format("�г��Ѿ�ֹͣ������ͣ���޷�ʵ��");
		AfxMessageBox(str,MB_ICONSTOP);		
	}
	else if (a>1.0 || a<0.0)
	{
		str.Format("����ͣ��������ٶ�Ϊ:%2.3f,�޷�ʵ��",a);
		AfxMessageBox(str,MB_ICONSTOP);		
	}
	else
	{
		m_bWaitingToDeacc=true;

		m_dAcc=0.0;
		m_sldAcc.SetPos(7);
		str.Format("%2.1f",m_dAcc);
		SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());
		
		str.Format("����ͣ��������... ֹͣ���� CURACC:%3.1f CURSPEED:%3.1f CURPOS:%4.1f STOP:%d",m_dAcc,m_dSpeed,m_dPosition+m_dStartPosBak,m_nStopPos);
		AddMsg(str);

		m_dBreakPoint=m_nStopPos-((m_dSpeed/3.6)*(m_dSpeed/3.6)/(2*1.0)+STOP_POINT_ERROR)*m_dDirValue;

		str.Format("����ͣ��������... Ԥ���ƶ�λ�� %3.1f",m_dBreakPoint);
		AddMsg(str);
		
		m_bContinueBreakPoin=true;
		//str.Format("����ͣ��,�Զ����ü��ٶ�:%2.1f",a);
		//AddMsg(str);

		//m_dAcc=-1.0*a;
		//str.Format("%2.1f",m_dAcc);
		//SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());

		//m_sldAcc.SetPos(7);
	}
}
void CSATSDlg::TimerTask_StartBreakPoint(void)
{	CString str;

	m_dBreakPoint=m_nStopPos-((m_dSpeed/3.6)*(m_dSpeed/3.6)/(2*1.0)+STOP_POINT_ERROR)*m_dDirValue;
	if (fabs(m_dBreakPoint-m_dBreakPointBak)>0.1)
	{
		m_dBreakPointBak=m_dBreakPoint;
		//str.Format("����ͣ��������... Ԥ���ƶ�λ�� %3.1f",m_dBreakPoint);
		//AddAlarm(str);
	}

	if ((m_dDirValue>0.0 && m_dPosition+m_dStartPosBak>m_dBreakPoint) ||
		(m_dDirValue<0.0 && m_dPosition+m_dStartPosBak<m_dBreakPoint))
	{
		str.Format("ʵʩ����ͣ��,�Զ����ü��ٶ�:1.0");
		AddMsg(str);

		m_dAcc=-1.0;
		str.Format("%2.1f",m_dAcc);
		SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());
		m_sldAcc.SetPos(7);
		m_bWaitingToDeacc=false;
	}
}
void CSATSDlg::TimerTask_ContinueBreakPoint(void)
{	CString str;

	if (m_dSpeed<5.0 && (m_dAcc>0.1 || m_dAcc<-0.1)) 
	{
		m_dAcc=0.0;
		m_sldAcc.SetPos(7);
		str.Format("%2.1f",m_dAcc);
		SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());
	}
	double s=(m_nStopPos-(m_dPosition+m_dStartPosBak))*m_dDirValue;
	if (s<1.0)
	{
		m_dAcc=-1.0;
		str.Format("%2.1f",m_dAcc);
		SetDlgItemText(IDC_EDIT_ACC,str.GetBuffer());
		m_sldAcc.SetPos(7);
		m_bContinueBreakPoin=false;
	}
}
void CSATSDlg::OnBnClickedCheckSimrun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bSimRun=m_btnSimRun.GetCheck() ? true:false;

	if (m_btnSimRun.GetCheck()==0)
	{
		AddMsg("ȡ��ģ���г�");
	}
	else
	{
		AddMsg("��ʼģ���г�");
	}
}
void CSATSDlg::OnMenuSimrun()
{
	// TODO: �ڴ���������������
	static int i=0;

	if (!m_bSuperUsr)
	{
		return;
	}

	if (i==0)
	{
		m_bSimRun=true;
		m_btnSimRun.SetCheck(1);
		i++;
	}
	else if (i==1)
	{
		m_bSimRun=false;
		m_btnSimRun.SetCheck(0);
		i=0;
	}

	int n=m_btnSimRun.GetCheck();

	if (m_btnSimRun.GetCheck()==0)
	{
		AddMsg("ȡ��ģ���г�");
	}
	else
	{
		AddMsg("��ʼģ���г�");
	}
	//ID_MENU_SIMRUN
}
void CSATSDlg::OnUpdateMenuSimrun(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if (m_bSimRun==false)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CSATSDlg::OnCbnSelchangeComboHeaddir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n=m_cmbHeadDir.GetCurSel();

	m_bTrainHeadRight=false;

	if (IsRunToRight() && !IsRunBack())
	{
		m_bTrainHeadRight=true;
	}
	else if (!IsRunToRight() && IsRunBack())
	{
		m_bTrainHeadRight=true;

	}

	if (m_bTrainHeadRight)
	{
		m_cmbHeadDir.SetCurSel(1);
	}
	else
	{
		m_cmbHeadDir.SetCurSel(0);
	}

	Invalidate();
}
void CSATSDlg::VersionCheck(void)
{	m_bVersionCheck=false;


	if (m_dSatsVersion==1.9 && m_nSatsVersion==2)
	{
		m_bVersionCheck=true;
	}
	
	if (m_dSatsVersion>=2.0 &&  m_dSatsVersion<3.0 && m_nSatsVersion==3)
	{
		m_bVersionCheck=true;
	}

	if (m_dSatsVersion>=3.0 &&  m_dSatsVersion<4.0 && m_nSatsVersion==4)
	{
		m_bVersionCheck=true;
	}

	if (m_dSatsVersion>=4.0 &&  m_dSatsVersion<5.0 && m_nSatsVersion==5)
	{
		m_bVersionCheck=true;
	}
}

void CSATSDlg::OnBnClickedBtnOuttc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OutputTc();

}

void CSATSDlg::OnCbnSelchangeComboDrvdir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_btnOutDir.EnableWindow(true);

	m_nDrvDirRecord=m_cmbDrvDir.GetCurSel();

}

void CSATSDlg::OnBnClickedBtnOutdir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OutDir();
}
bool CSATSDlg::IsInAtoMode(void)
{
	return m_checkAtoMode.GetCheck()==1;
}

void CSATSDlg::OnBnClickedCheckAtoMode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_bInAtoMode=(m_checkAtoMode.GetCheck()==1);
	
	SendAtoEnabelCmd(m_checkAtoMode.GetCheck()==1);
}
void CSATSDlg::AddValueList(void)
{	CString str;

	str.Format("ATO_OFFSET:%.2f (m)",m_dAtoAntennaOffset);
	m_listCoreValue.AddString(str);
	
	str.Format("ATP_OFFSET:%.2f (m)",m_dAtpAntennaOffset);
	m_listCoreValue.AddString(str);

	str.Format("APR_OFFSET:%.2f (m)",m_dAprReaderOffset);
	m_listCoreValue.AddString(str);


}
void CSATSDlg::OnBnClickedCheckTir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CSATSDlg::OnBnSetfocusCheckTir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
