// SATSDlg.h : 头文件
//

#pragma once

#include "serial.h"
#include "afxwin.h"
#include "mysats.h"



//#define  SATS_UVER	1.9
//#define  SATS_UVER	2.0			//2007.05.16
//#define  SATS_UVER	3.0			//2007.11.22
//#define  SATS_UVER	3.1			//2007.12.05
//#define  SATS_UVER	3.2			//2007.12.12
//#define  SATS_UVER	3.3		    //2008.01.17
#define  SATS_UVER	4.0				//2008.03.20


// CSATSDlg 对话框
class CSATSDlg : public CDialog,public CMySats
{

// 构造
public:
	CSATSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SATS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnMenuUser();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();


	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCbnSelchangeComboSimdir();
	afx_msg void OnBnClickedBtnPosset();
	afx_msg void OnBnClickedBtnGap();
	afx_msg void OnAppAbout();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	void OnPopMyCommand(UINT nID);

	CSliderCtrl m_sldAcc;
	CListBox m_listMsg;
	CListBox m_listAlarm;	
	CButton m_btnSimRun;

	CComboBox m_cbDm;
	CComboBox m_cbDir;
	CComboBox m_cbTrackCar;
	CComboBox m_cbTrackMod;
	CComboBox m_cbSim;
	CComboBox m_cbTrack;
	CComboBox m_cbApr;
	CComboBox m_cbStartPos;
	CComboBox m_cbSimDir;

public:
	HCURSOR m_hHandCursor;
	HCURSOR m_hHandArror;



public:
	double m_dSatsVersion;
	int m_nSimDirRecord;
	int m_nDrvDirRecord;

	int m_nStopPos;
	double m_dBreakPoint;
	double m_dBreakPointBak;
	bool m_bVersionCheck;

	bool m_bWaitingToDeacc;
	bool m_bContinueBreakPoin;

	CEdit m_editPosition;
	CEdit m_editDistance;
	afx_msg void OnCbnSelchangeComboDir();
	afx_msg void OnCbnSelchangeComboDm();
	CEdit m_editStartPos;
	afx_msg void OnCbnSelchangeComboCar();
	afx_msg void OnCbnSelchangeComboMod();
	CEdit m_editSpeed;
	CEdit m_editGap;
	afx_msg void OnBnClickedCheckSimrun();

	void OutDir(void);

	void CancelEsi(void);
	void ApplyEsi(void);

	bool m_bSuperUsr;
protected:
	virtual void OnOK();
public:
	void AddValueList(void);
	
	afx_msg void OnBnClickedBtnSetZeroSpeed();
	double m_dAutoSpeed;
	afx_msg void OnBnClickedBtnAutoSpeed();
	afx_msg void OnBnClickedCheckNotrack();
	CButton m_checkNoTrack;
	CComboBox m_cmbRouteCfg;
	afx_msg void OnCbnSelchangeComboCfg();
	CComboBox m_cmbEsi;
	afx_msg void OnBnClickedCheckTest();
	CButton m_btnTest;
	CEdit m_editPort;
	afx_msg void OnMenuEsitest();
	afx_msg void OnNMCustomdrawSliderAcc(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStop();
	CEdit m_editStopPos;
	void VersionCheck(void);

	void OutputTc(void);
	bool IsInAtoMode(void);

	void TimerTask_StartBreakPoint(void);
	void TimerTask_ContinueBreakPoint(void);
	
	afx_msg void OnMenuSimrun();
	afx_msg void OnUpdateMenuSimrun(CCmdUI *pCmdUI);
	CComboBox m_cmbHeadDir;
	afx_msg void OnCbnSelchangeComboHeaddir();
	CEdit m_editSatsVer;
	CButton m_btnOutTc;
	afx_msg void OnBnClickedBtnOuttc();
	CComboBox m_cmbDrvDir;
	afx_msg void OnCbnSelchangeComboDrvdir();
	CButton m_btnOutDir;
	afx_msg void OnBnClickedBtnOutdir();
	CEdit m_editAtoSpeed;
	CEdit m_editAccAto;
	CButton m_checkAtoMode;
	afx_msg void OnBnClickedCheckAtoMode();
	CListBox m_listCoreValue;
public:
	afx_msg void OnBnClickedCheckTir();
public:
	afx_msg void OnBnSetfocusCheckTir();
};
