#pragma once

#include "serial.h"

#define TIMER1		100
#define MSG_HEAD_1	0x68
#define MSG_HEAD_2	0x16
//#define PRE_APR	5				//米
//#define PRE_APR		1				//米	20070814 for after change apr offset , no need this again. and if the two next apr is very near < 10M then 
//perhaps the seconed apr will occpied the first one. cause the first apr can not be send
#define PRE_APR		3				// if the pre value equale 1 meter will cause loose apr when speed is very high
#define PRE_ATOLOOP		10			//米
#define MODULATION_MAX	100
#define ATPLOOP_DATA_MAX	7

#define DEV_SENSITIVITY	5	//设备在5米范围内只能发出一次信息
#define STOP_POINT_ERROR	20

class CMySats
{
public:
	enum eDevType { eDevTypeTrack=1,eDevTypeApr,eDevTypeAtoLoop,eDevTypeMax};
	enum eTrackIntPos { eTrackIntCar=0,eTrackIntMod,eTrackIntCarSet,eTrackIntModSet,eTrackIntMax};
	enum eAprIntPos { eAprIntMissing=0,eAprIntMax };
struct _stuDev
{
	int nType;
	char szName[100];
	double dPara[10];
	int nPara[10];
	int nDir;
	int nInOut;
	unsigned char ucValue[100];
	int nModulation[MODULATION_MAX];
};
struct _stuDevs
{
	int nDevSum;
	struct _stuDev stuDevs[100];
};

public:
	double m_dAtoAntennaOffset;
	double m_dAtpAntennaOffset;
	double m_dAprReaderOffset;


	int m_nAtoCoe;

	bool m_bTrainHeadRight;
	bool m_bRunToRight;
	bool m_bRunBack;

	bool m_bSendGap;
	bool m_bRebootSystem;
	bool m_bJustChangeDir;
	bool m_bSimRun;
	bool m_bRenewGap;
	bool m_bTest;

	int m_nDirPara;
	int m_nX0;
	int m_nY0;
	double m_dScale;
	double m_dEndPos;
	double m_dStartPos;
	double m_dStartPosBak;
	double m_dPosition;
	double m_dMaxAcc;
	double m_dMinAcc;
	double m_dDirValue;		//方向系数
	double m_dDistanceLast;	//折返前距离计数
	double m_dPositionLast;
	double m_dDistance;	
	double m_dDm;
	double m_dAcc;
	double m_dSpeed;
	bool m_bAutoSendGap;
	BYTE m_dbSatsPort;
	BYTE m_dbSatsPort2;
	BYTE m_dbSatsPort3;
	BYTE m_dbSatsPort4;
	BYTE m_dbSatsPort5;
	BYTE m_dbSatsPort6;
	int	m_nAtoSpeed;

	int m_nAtoLoopEnable;
	int m_nSatsVersion;
	int m_nSatsLastVersion;

	BYTE m_dbSatsLastPort;
	BYTE m_dbSatsLastPort2;
	BYTE m_dbSatsLastPort3;
	BYTE m_dbSatsLastPort4;
	BYTE m_dbSatsLastPort5;
	BYTE m_dbSatsLastPort6;

	bool m_bInAtoMode;

	int m_nAccRate;
	CStdioFile m_sf;

	unsigned char m_szSendBuf[100];
	unsigned char m_szRecvBuf[10240];
	
	char m_szSpeedCodeName[20][10];
	int m_nTechoDir;
	int m_nDmPosition;
	int m_nModulationSum;
	char m_szModulationNames[MODULATION_MAX][100];

	int m_nTrackCar;
	int m_nTrackCarRecord;
	int m_nTrackMod;
	int m_nTrackModRecord;

	int m_nGap;
	int m_nRecvSum;
	int m_nTrackOutputCar;	//输出轨道载频
	int m_nTrackOutputMod; //输出轨道调制
	int m_nAprOutputPos;	//当前发送APR的保存位置
	int m_nAtoLoopOutputPos;
	int m_nCurDevice;
	int m_nCurTrack;

	struct _stuDevs m_stuDevs;

	char m_szCommInit1[100];
	char m_szCommInit2[100];

	unsigned char m_ucAtoLoopData[100];

	CString m_strMapData;

	CListBox *m_pListMsg;
	CListBox *m_pListAlarm;

	CSerial *m_pSerial1;
	CSerial *m_pSerial2;

	bool m_bDataFileOk;
public:
	void SendAtoEnabelCmd(bool bEnable);

	void LoadConfig(void);
	bool IsArp(void);
	bool IsTrack(void);
	int SearchDevice(int x,int y);
	void LoadData(void);
	void LoadData_Map(void);
	void *CreateFont(int nSize,char *szFacename="新宋体",int nWeight=FW_NORMAL,bool bItalic=false,bool bUnderline=false);
	bool IsRunToRight(void);
	bool IsRunBack(void);

	void InitSerial(void);
	
	void InitFile(void);

	void AddAlarm(CString strMsg);
	void AddMsg(CString strMsg,bool bShow=true);

	void MyDraw(CPaintDC &dc);
	int ReadSerial(void);
	void DoWithRecvMsg(void);
	void MakeSendBuf_Speed(void);
	void MakeSendBuf_Apr(void);
	void Patrol_Track(void);
	void Patrol_Apr(void);

	void TimerTask(void);
	void SatsCreate(void);
	void Patrol_AtoLoop(void);

	void Encrypt(char *pSource,int nLen,int nPwd);
	void MakeSendBuf_AtoLoop(void);

	void AddModulation(struct CMySats::_stuDev *pstuDev,char *szValue);
	void AddModulationName(char *szValues);

public:

	CMySats(CListBox *pListMsg,CListBox *pListAlarm);

	~CMySats(void);
};
