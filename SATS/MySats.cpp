#include "StdAfx.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include ".\mysats.h"

CMySats::CMySats(CListBox *pListMsg,CListBox *pListAlarm)
{
	m_pListMsg=pListMsg;
	m_pListAlarm=pListAlarm;

	m_nCurDevice=-1;

	m_dDistance=0.0;
	m_dSpeed=0.0;
	m_dSpeed=0.0;
	m_dAcc=0.0;
	m_nGap=0;
	m_bSimRun=false;

	m_nTechoDir=1;

	m_nTrackCar=0;
	m_nTrackCarRecord=0;
	m_nTrackMod=0;
	m_nTrackModRecord=0;

	memset(m_szSpeedCodeName,0,sizeof(m_szSpeedCodeName));
	strcpy(m_szSpeedCodeName[0],"N");
	strcpy(m_szSpeedCodeName[1],"44N");
	strcpy(m_szSpeedCodeName[2],"33N");
	strcpy(m_szSpeedCodeName[3],"33S");
	strcpy(m_szSpeedCodeName[4],"22N");
	strcpy(m_szSpeedCodeName[5],"22R1");
	strcpy(m_szSpeedCodeName[6],"21R2");
	strcpy(m_szSpeedCodeName[7],"21N");
	strcpy(m_szSpeedCodeName[8],"22NS");
	strcpy(m_szSpeedCodeName[9],"11NS");
	strcpy(m_szSpeedCodeName[10],"10R1");
	strcpy(m_szSpeedCodeName[11],"10N");
	strcpy(m_szSpeedCodeName[12],"11AS");
	strcpy(m_szSpeedCodeName[13],"11N");
	strcpy(m_szSpeedCodeName[14],"00N");

	m_bRenewGap=false;

	m_pSerial1=NULL;
	m_pSerial2=NULL;

	memset(m_szModulationNames,0,sizeof(m_szModulationNames));

	m_bDataFileOk=false;
	m_bAutoSendGap=false;

	m_nCurTrack=-1;
	m_bTest=false;

	m_dbSatsPort=0;
	m_dbSatsPort2=0;
	m_nSatsVersion=0;
	m_nSatsLastVersion=-1;

	m_dbSatsLastPort=-1;
	m_dbSatsLastPort2=-1;
	m_dbSatsLastPort3=-1;
	m_dbSatsLastPort4=-1;

	m_nAccRate=100;

	memset(&m_ucAtoLoopData,0,sizeof(m_ucAtoLoopData));

	m_bInAtoMode=false;
	m_bTrainHeadRight=false;

	m_dAtoAntennaOffset=0.0;
	m_dAtpAntennaOffset=0.0;
	m_dAprReaderOffset=0.0;
	m_nAtoLoopEnable=0;

	m_nDirPara=0;
}

CMySats::~CMySats(void)
{
	if (m_pSerial1)
	{
		delete m_pSerial1;
	}

	if (m_pSerial2)
	{
		delete m_pSerial2;
	}

}
void CMySats::SatsCreate(void)
{
	m_dPosition=0.0;

	m_dMaxAcc=1.2;
	m_dMinAcc=-1.2;
	m_dStartPos=0.0;
	m_dEndPos=0.0;
	m_nRecvSum=0;
	m_dDirValue=1.0;
	m_dDistanceLast=0.0;
	m_bRunToRight=true;
	m_bRunBack=false;
	m_dPositionLast=0.0;
	m_nTrackOutputCar=0;	//输出轨道载频
	m_nTrackOutputMod=0; //输出轨道调制
	m_bJustChangeDir=false;
	m_nAprOutputPos=0;
	m_nAtoLoopOutputPos=0;
	m_bSendGap=false;
	m_bRebootSystem=true;

	memset(&m_stuDevs,0,sizeof(m_stuDevs));

	InitFile();		//wmz 2007.01.23

	LoadConfig();
	LoadData();
	LoadData_Map();
	InitSerial();

	m_nX0=30;
	m_nY0=400;
	m_dScale=900.0/(m_dEndPos-m_dStartPos);
}
void CMySats::LoadConfig(void)
{	CString strFilePath="C:\\SATS\\sats.ini";

	int nPort=GetPrivateProfileInt("COM_DEVICE","COM_PORT",1,LPCSTR(strFilePath));
	int nSpeed=GetPrivateProfileInt("COM_DEVICE","COM_SPEED",9600,LPCSTR(strFilePath));
	
	char szVerify[100];
	GetPrivateProfileString("COM_DEVICE","COM_VERIFY","N",szVerify,100,LPCSTR(strFilePath));
	
	int nDataBit=GetPrivateProfileInt("COM_DEVICE","COM_DATA_BIT",8,LPCSTR(strFilePath));
	int nStopBit=GetPrivateProfileInt("COM_DEVICE","COM_STOP_BIT",1,LPCSTR(strFilePath));

	sprintf(m_szCommInit1,"COM%d: baud=%d parity=%s data=%d stop=%d",nPort,nSpeed,szVerify,nDataBit,nStopBit);

	nPort=GetPrivateProfileInt("COM_DEVICE1","COM_PORT",1,LPCSTR(strFilePath));
	nSpeed=GetPrivateProfileInt("COM_DEVICE1","COM_SPEED",9600,LPCSTR(strFilePath));
	
	
	GetPrivateProfileString("COM_DEVICE1","COM_VERIFY","N",szVerify,100,LPCSTR(strFilePath));
	
	nDataBit=GetPrivateProfileInt("COM_DEVICE1","COM_DATA_BIT",8,LPCSTR(strFilePath));
	nStopBit=GetPrivateProfileInt("COM_DEVICE1","COM_STOP_BIT",1,LPCSTR(strFilePath));


	sprintf(m_szCommInit2,"COM%d: baud=%d parity=%s data=%d stop=%d",nPort,nSpeed,szVerify,nDataBit,nStopBit);
}
bool CMySats::IsArp(void)
{
	if (m_nCurDevice==-1) 
	{
		return false;
	}

	if (m_stuDevs.stuDevs[m_nCurDevice].nType==eDevTypeApr)
	{
		return true;
	}

	return false;
}
bool CMySats::IsTrack(void)
{
	if (m_nCurDevice==-1) 
	{
		return false;
	}

	if (m_stuDevs.stuDevs[m_nCurDevice].nType==eDevTypeTrack)
	{
		return true;
	}

	return false;
}
int CMySats::SearchDevice(int x,int y)
{	int x0=(int)((x-m_nX0)/m_dScale);
	int y0=(int)(y-m_nY0);

	int i=0;
	for(i=0;i<m_stuDevs.nDevSum;i++)
	{
		if (fabs(m_stuDevs.stuDevs[i].dPara[0]-x0)<5) break;
	}

	if (i==m_stuDevs.nDevSum) 
	{
		m_nCurDevice=-1;
		return -1;
	}
	
	m_nCurDevice=i;

 	return i;
}
void CMySats::LoadData_Map(void)
{
	CString strFilePath="C:\\SATS\\MAPID\\*.*";		

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strFilePath);

	CStdioFile sf;
	CString strPathName;
	CString strFileName;
	CString strLine;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.MatchesMask(FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE))//FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM))
		{
			strPathName=finder.GetFilePath();	
			strFileName=finder.GetFileName();
			strFileName.Delete(8,4);
			BOOL b=sf.Open(strPathName,CFile::modeReadWrite);
			if (b)
			{
				if (sf.ReadString(strLine))
				{
					m_strMapData+=strFileName;
					m_strMapData+=strLine;
				}
			}
			sf.Close();
		}
	}
}
void CMySats::AddModulationName(char *szValues)
{	CString str,str1;

	int i=0;
	int n=0;

	str=szValues;

	do 
	{	str.TrimLeft();

		if (str.GetLength()==0) 
		{
			break;
		}

		//int n=str.Find(" ");			//very important  if n define before and redefine inside the loop  that the loop inport n is undefined!!!
		n=str.Find(" ");

		if (n==-1) 
		{
			str1=str;
			str="";
		}
		else 
		{
			str1=str.Mid(0,n);
			str.Delete(0,n);
		}
		str1.TrimLeft();
		str1.TrimRight();

		sscanf(str1.GetBuffer(),"%s", &(m_szModulationNames[i++]));
	}
	while(1);

}
void CMySats::AddModulation(struct CMySats::_stuDev *pstuDev,char *szValue)
{	CString str,str1;

	str=szValue;
	int i=0;

	do 
	{	
		str.TrimLeft();
		int n=str.GetLength();

		if (str.GetLength()==0) break;
		
		n=str.Find(" ");
		if (n==-1) 
		{
			str1=str;
			str="";
		}
		else 
		{
			str1=str.Mid(0,n);
			str.Delete(0,n);
		}

		sscanf(str1.GetBuffer(),"%d", &(pstuDev->nModulation[i++]));
	}
	while(1);

}
void CMySats::LoadData(void)
{	CString strFilePath="C:\\SATS\\sats.ini";
	char chs[100];
	char szDevType[100];

	CString strDevNum;
	CString strDevType;
	CString strDevName;
	CString str;

	int nDevSum=GetPrivateProfileInt("DEVMAP","DEVICE_SUM",0,LPCSTR(strFilePath));

	m_dStartPos=GetPrivateProfileInt("DEVMAP","START_POS",0,LPCSTR(strFilePath));	
	if (m_dStartPos>10000000) m_dStartPos=(m_dStartPos-10000000)*(-1.0);
	m_dStartPos/=10.0;

	m_nDirPara=GetPrivateProfileInt("DEVMAP","DIR_REVERSE_PARA",0,LPCSTR(strFilePath));	


	m_dEndPos=GetPrivateProfileInt("DEVMAP","END_POS",0,LPCSTR(strFilePath));	
	if (m_dEndPos>10000000) m_dEndPos=(m_dEndPos-10000000)*(-1.0);
	m_dEndPos/=10.0;

	m_nModulationSum=GetPrivateProfileInt("DEVMAP","MODSUM",0,LPCSTR(strFilePath));	

	GetPrivateProfileString("DEVMAP","MODNAME","",chs,100,LPCSTR(strFilePath));

	m_nAccRate=GetPrivateProfileInt("DEVMAP","ACCRATE",100,LPCSTR(strFilePath));	

	m_dAprReaderOffset=GetPrivateProfileInt("DEVMAP","APR_READER_OFFSET",0,LPCSTR(strFilePath))/1000.0;
	m_dAtoAntennaOffset=GetPrivateProfileInt("DEVMAP","ATO_ANTENNA_OFFSET",0,LPCSTR(strFilePath))/1000.0;
	m_dAtpAntennaOffset=GetPrivateProfileInt("DEVMAP","ATP_ANTENNA_OFFSET",0,LPCSTR(strFilePath))/1000.0;
	m_nAtoCoe=GetPrivateProfileInt("DEVMAP","ATO_COE",100,LPCSTR(strFilePath));

	//if (m_nAccRate<100)
	//{
	//	m_nAccRate=100;
	//}

	AddModulationName(chs);

	struct _stuDev *pCurDev;
	for(int i=0;i<nDevSum;i++)
	{
		pCurDev=&m_stuDevs.stuDevs[m_stuDevs.nDevSum];

		strDevNum.Format("DEV%03d",i+1);
		GetPrivateProfileString(strDevNum,"TYPE","NONE",szDevType,100,LPCSTR(strFilePath));
		strDevType=szDevType;
		if (strDevType=="TRACK")
		{	pCurDev->nType=eDevTypeTrack;

			GetPrivateProfileString(strDevNum,"NAME","NONE",pCurDev->szName,100,LPCSTR(strFilePath));
			pCurDev->dPara[0]=GetPrivateProfileInt(strDevNum,"START",0,LPCSTR(strFilePath));
			if (pCurDev->dPara[0]>10000000) pCurDev->dPara[0]=(pCurDev->dPara[0]-10000000)*(-1.0);
			pCurDev->dPara[0]=pCurDev->dPara[0]/10.0-m_dStartPos;	

			pCurDev->dPara[1]=GetPrivateProfileInt(strDevNum,"END",0,LPCSTR(strFilePath));	
			if (pCurDev->dPara[1]>10000000) pCurDev->dPara[1]=(pCurDev->dPara[1]-10000000)*(-1.0);
			pCurDev->dPara[1]=pCurDev->dPara[1]/10.0-m_dStartPos;	

			pCurDev->nPara[0]=GetPrivateProfileInt(strDevNum,"CARRIER",0,LPCSTR(strFilePath));

			GetPrivateProfileString(strDevNum,"MODULATION","",chs,100,LPCSTR(strFilePath));

			AddModulation(&m_stuDevs.stuDevs[m_stuDevs.nDevSum],chs);

			//MODULATION=1 
//			pCurDev->nPara[eTrackIntModSet]=1;	
			pCurDev->nPara[eTrackIntMod]=1;	
			m_stuDevs.nDevSum++;
		}
		else if (strDevType=="APR")
		{
			pCurDev->nType=eDevTypeApr;
			GetPrivateProfileString(strDevNum,"NAME","NONE",pCurDev->szName,100,LPCSTR(strFilePath));
			pCurDev->dPara[0]=GetPrivateProfileInt(strDevNum,"START",0,LPCSTR(strFilePath));
			if (pCurDev->dPara[0]>10000000) pCurDev->dPara[0]=(pCurDev->dPara[0]-10000000)*(-1.0);
			pCurDev->dPara[0]=pCurDev->dPara[0]/10.0-m_dStartPos;	

			pCurDev->dPara[1]=pCurDev->dPara[0];
			m_stuDevs.nDevSum++;
		}
		else if (strDevType=="ATOLOOP")
		{
			pCurDev->nType=eDevTypeAtoLoop;
			GetPrivateProfileString(strDevNum,"NAME","NONE",pCurDev->szName,100,LPCSTR(strFilePath));
			pCurDev->dPara[0]=GetPrivateProfileInt(strDevNum,"START",0,LPCSTR(strFilePath));
			if (pCurDev->dPara[0]>100000000) pCurDev->dPara[0]=(pCurDev->dPara[0]-100000000)*(-1.0);
			pCurDev->dPara[0]=pCurDev->dPara[0]/100.0-m_dStartPos;	
			pCurDev->dPara[1]=pCurDev->dPara[0];
			pCurDev->nDir=GetPrivateProfileInt(strDevNum,"DIR",0,LPCSTR(strFilePath));
			pCurDev->nInOut=GetPrivateProfileInt(strDevNum,"INOUT",0,LPCSTR(strFilePath));
			char chs[100],chs1[100];
			GetPrivateProfileString(strDevNum,"VALUE","00000000000000",chs,100,LPCSTR(strFilePath));
			int nLen=strlen(chs);
			memset(pCurDev->ucValue,0,sizeof(pCurDev->ucValue));
			for(int i=0;i<nLen;i+=2)
			{
				chs1[0]=chs[i];
				chs1[1]=chs[i+1];
				chs1[2]=0;
				sscanf(chs1,"%02X",&pCurDev->ucValue[i/2]);
			}
			m_stuDevs.nDevSum++;
		}
	}

	nDevSum=nDevSum;

	m_dEndPos-=m_dStartPos;

	m_dStartPosBak=m_dStartPos;
	m_dStartPos=0.0;
}
void CMySats::InitFile(void)
{
	CTime ct(CTime::GetCurrentTime());
	CString str;
	str.Format("C:\\SATS\\SATS-DATA-(%s).TXT",ct.Format("%Y-%m-%d %H-%M-%S"));

	BOOL b=m_sf.Open(str,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate|CFile::shareDenyWrite);
	
	if (b)
	{
		m_bDataFileOk=true;
	}
}
void *CMySats::CreateFont(int nSize,char *szFacename,int nWeight,bool bItalic,bool bUnderline)
{
	CFont *pFont=new CFont;
	pFont->CreateFont( nSize,					       // nHeight
							0,                         // nWidth
							0,                         // nEscapement
							0,                         // nOrientation
							nWeight,		           // nWeight
							bItalic,                   // bItalic
							bUnderline,                // bUnderline
							0,                         // cStrikeOut
							ANSI_CHARSET,              // nCharSet
							OUT_DEFAULT_PRECIS,        // nOutPrecision
							CLIP_DEFAULT_PRECIS,       // nClipPrecision
							DEFAULT_QUALITY,           // nQuality
							DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
							szFacename);               // lpszFacename
	return (void *)pFont;
}
bool CMySats::IsRunToRight(void)
{
	return m_bRunToRight;
}
bool CMySats::IsRunBack(void)
{
	return m_bRunBack;
}
void CMySats::InitSerial(void)
{	CString str;

	m_pSerial1=new CSerial;
	int nRtn=m_pSerial1->Create(NULL,m_szCommInit1);
	if (nRtn)
	{
		str.Format("Create Comm Sats %s Success!",m_szCommInit1);
		AddMsg(str);
	}
	else
	{
		str.Format("Failure to Create Comm Sats %s !",m_szCommInit1);
		AddMsg(str);
	}

	m_pSerial2=new CSerial;
	nRtn=m_pSerial2->Create(NULL,m_szCommInit2);
	if (nRtn)
	{
		str.Format("Create Comm DM %s Success!",m_szCommInit2);
		AddMsg(str);
	}
	else
	{
		str.Format("Failure to Create Comm DM %s !",m_szCommInit2);
		AddMsg(str);
	}
}
void CMySats::AddMsg(CString strMsg,bool bShow)
{
	CTime ct(CTime::GetCurrentTime());
	
	CString str;

	SYSTEMTIME lSysTime;
	GetSystemTime(&lSysTime);

	str.Format("[%05d]%s[%s:%03d]",m_pListMsg->GetCount()+1,strMsg,ct.Format("%Y-%m-%d %H:%M:%S"),lSysTime.wMilliseconds);

	if (bShow)
	{
		m_pListMsg->AddString(str);
		m_pListMsg->SetCurSel(m_pListMsg->GetCount()-1);
	}

	char chs[1024];
	strcpy(chs,str.GetBuffer());

	if (m_bDataFileOk)
	{
		m_sf.WriteString(str);
		m_sf.WriteString("\n");
		m_sf.Flush();
	}

	if (m_pSerial2)
	{
		Encrypt(chs,str.GetLength()-2,3);
		int n=(int)strlen(chs);
        chs[n]=0x0d;
		chs[n+1]=0x0a;
		chs[n+2]=0x00;
		m_pSerial2->WriteSerial((unsigned char *)chs,(int)strlen(chs));
	}
}
void CMySats::AddAlarm(CString strMsg)
{
	CTime ct(CTime::GetCurrentTime());
	
	CString str;

	SYSTEMTIME lSysTime;
	GetSystemTime(&lSysTime);

	str.Format("[%05d]%s[%s:%03d]",m_pListAlarm->GetCount()+1,strMsg,ct.Format("%Y-%m-%d %H:%M:%S"),lSysTime.wMilliseconds);
	
	m_pListAlarm->AddString(str);

	m_pListAlarm->SetCurSel(m_pListAlarm->GetCount()-1);

	char chs[1024];
	strcpy(chs,str.GetBuffer());

	if (m_pSerial2)
	{
		Encrypt(chs,str.GetLength()-2,3);
		int n=(int)strlen(chs);
        chs[n]=0x0d;
		chs[n+1]=0x0a;
		chs[n+2]=0x00;
		m_pSerial2->WriteSerial((unsigned char *)chs,(int)strlen(chs));
	}
}
void CMySats::MyDraw(CPaintDC &dc)
{	CString str;

	CPen penBlue2(PS_SOLID,2,RGB(0,0,255));
	CPen penWhite1(PS_SOLID,1,RGB(255,255,255));
	CPen penBlack1(PS_SOLID,1,RGB(0,0,0));
	CPen penRed1(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen1(PS_SOLID,1,RGB(0,0,0));
	CPen penKhaki(PS_SOLID,1,RGB(255,128,0));
	CPen bruCyan1(PS_SOLID,1,RGB(0,128,128));

	CBrush bruCyan(RGB(0,128,128));
	CBrush bruBlack(RGB(0,0,0));
	CBrush bruWhite(RGB(255,255,255));

	CFont *pFont9=(CFont *)CreateFont(9);
	CFont *pFont11=(CFont *)CreateFont(11);
	CFont *pFont12=(CFont *)CreateFont(12);

	dc.SetBkColor(RGB(212,208,200));

	for(int i=0;i<m_stuDevs.nDevSum;i++)
	{
		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
		{
			dc.SelectObject((CPen * )&penBlue2);
			dc.SelectObject(pFont12);
			int x1=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[0]*m_dScale);
			int x2=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[1]*m_dScale);
			dc.MoveTo(x1,m_nY0);
			dc.LineTo(x2,m_nY0);
			dc.MoveTo(x1,m_nY0-5);
			dc.LineTo(x1,m_nY0+5);
			dc.MoveTo(x2,m_nY0-5);
			dc.LineTo(x2,m_nY0+5);
			
			str.Format("%s",m_stuDevs.stuDevs[i].szName);
			dc.SetTextColor(RGB(0,0,0));
			dc.TextOut((x1+x2)/2-15,m_nY0-50,str);
			if (m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]==0)
			{
				dc.SetTextColor(RGB(0,0,0));
				str.Format("F%d",m_stuDevs.stuDevs[i].nPara[eTrackIntCar]);
			}
			else 
			{
				dc.SetTextColor(RGB(255,0,0));
				if (m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]==8)
				{
					str.Format("F%d",m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]+1);
				}
				else if (m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]==10)
				{
					str.Format("F0");
				}
				else
				{
					str.Format("F%d",m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet]);
				}
			}
			dc.TextOut((x1+x2)/2-10,m_nY0-37,str);

			
			if (m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]==0)
			{
				dc.SetTextColor(RGB(0,0,0));
				str.Format("M%d-%s",m_stuDevs.stuDevs[i].nPara[eTrackIntMod],m_szSpeedCodeName[m_stuDevs.stuDevs[i].nPara[eTrackIntMod]]);
			}
			else
			{
				dc.SetTextColor(RGB(0,0,255));
				str.Format("M%d-%s",m_stuDevs.stuDevs[i].nPara[eTrackIntModSet],m_szSpeedCodeName[m_stuDevs.stuDevs[i].nPara[eTrackIntModSet]]);
			}
			dc.TextOut((x1+x2)/2-15,m_nY0-26,str);

		}
		else if (m_stuDevs.stuDevs[i].nType==eDevTypeApr)
		{	
			dc.SelectObject((CPen * )&penKhaki);
			int x1=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[0]*m_dScale);
			int x2=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[1]*m_dScale);
			dc.SelectObject(pFont12);
		
			if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing])
			{
				dc.SetTextColor(RGB(255,0,0));
			}
			else
			{
				dc.SetTextColor(RGB(0,0,0));
			}
			dc.TextOut(x1,m_nY0+10,"A");
			dc.SetTextColor(RGB(0,0,0));
			dc.SelectObject(pFont11);
			dc.TextOut(x1-8,m_nY0+23,m_stuDevs.stuDevs[i].szName);
		}
		else if (m_stuDevs.stuDevs[i].nType==eDevTypeAtoLoop)
		{	
			dc.SelectObject((CPen * )&penKhaki);
			int x1=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[0]*m_dScale);
			int x2=(int)(m_nX0+m_stuDevs.stuDevs[i].dPara[1]*m_dScale);
			dc.SelectObject(pFont12);
		
			if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing])
			{
				dc.SetTextColor(RGB(255,0,0));
			}
			else 
			{
				dc.SetTextColor(RGB(0,0,0));
			}
			dc.TextOut(x1,m_nY0-5,"∞");
			dc.SetTextColor(RGB(0,0,0));
			dc.SelectObject(pFont11);
			if (m_stuDevs.stuDevs[i].nDir==0)
			{
				dc.SetTextColor(RGB(255,0,0));
			}
			else
			{
				dc.SetTextColor(RGB(0,0,255));
			}
			dc.TextOut(x1,m_nY0+3,m_stuDevs.stuDevs[i].szName);
		}
	}

	int x1=(int)(m_nX0+m_dPosition*m_dScale);
	int y1=(int)(m_nY0-12);
	
	if (IsRunToRight() && !IsRunBack())
	{
		x1-=30;
	}
	else if (!IsRunToRight() && IsRunBack())
	{
		x1-=30;
	}

	dc.SelectObject((CPen * )&bruCyan1);
	dc.SelectObject((CBrush * )&bruCyan);
	dc.Ellipse(x1,y1,x1+10,y1+10);
	dc.Ellipse(x1+20,y1+0,x1+30,y1+10);
	dc.Rectangle(x1+5,y1+0,x1+25,y1+10);

	dc.SelectObject((CPen * )&penKhaki);
	dc.SelectObject((CBrush * )&bruBlack);

	dc.Ellipse(x1+2,y1+5,x1+7,y1+12);
	dc.Ellipse(x1+6,y1+5,x1+11,y1+12);
	dc.Ellipse(x1+23,y1+5,x1+28,y1+12);
	dc.Ellipse(x1+19,y1+5,x1+24,y1+12);
	

	dc.SelectObject((CPen * )&penWhite1);
	dc.SelectObject((CBrush * )&bruWhite);
	for(int i=0;i<5;i++)
	{
		dc.Rectangle(x1+4+5*i,y1+1,x1+6+5*i,y1+5);
	}

	dc.SelectObject((CPen * )&penBlack1);
	

	if (m_bTrainHeadRight)
	{
		dc.SelectObject((CPen * )&penBlack1);
		dc.Rectangle(x1-3,y1+5,x1,y1+8);
		dc.SelectObject((CPen * )&penRed1);
		dc.Rectangle(x1+29,y1+5,x1+32,y1+8);
	}
	else
	{
		dc.SelectObject((CPen * )&penRed1);
		dc.Rectangle(x1-3,y1+5,x1,y1+8);
		dc.SelectObject((CPen * )&penBlack1);
		dc.Rectangle(x1+29,y1+5,x1+32,y1+8);
	}

	delete pFont12;
	delete pFont11;
	delete pFont9;
}
void CMySats::Encrypt(char *pSource,int nLen,int nPwd)
{
	for(int i=0;i<nLen;i++)
	{
		pSource[i]+=(i%nPwd);
	}
}
int CMySats::ReadSerial(void)
{
	int nLen=m_pSerial1->Read(m_pSerial1,&m_szRecvBuf[m_nRecvSum]);


	m_nRecvSum+=nLen;
	

	return m_nRecvSum;
}
void CMySats::DoWithRecvMsg(void)
{	int i=0;

	for(i=0;i<m_nRecvSum-1;i++)
	{
		if (m_szRecvBuf[i]==MSG_HEAD_1 && m_szRecvBuf[i+1]==MSG_HEAD_2)
		{
			break;
		}
	}
	for(int j=i;j<m_nRecvSum;j++)
	{
		m_szRecvBuf[j-i]=m_szRecvBuf[j];
	}
	m_nRecvSum-=i;

//	if (m_nRecvSum<7) return;
	if (m_nRecvSum<8) return;			//20070206 for add other byte of sats input port
	if (m_nRecvSum<10) return;			//20070227 for add other byte of sats other input port and sw version
	if (m_nRecvSum<14) return;			//20070516 for add other byte of sats ato speed serial msg count

	int nDistance=0;
	nDistance+=(m_szRecvBuf[5]<<0) & 0xff;
	nDistance+=(m_szRecvBuf[4]<<8) & 0xff00;
	nDistance+=(m_szRecvBuf[3]<<16) & 0xff0000;
	nDistance+=(m_szRecvBuf[2]<<24) & 0xff000000;
	if (nDistance/100.0<m_dDistance)
	{
		m_dDistanceLast=nDistance/100.0;
		m_dPositionLast=m_dPosition;
	}
	m_dDistance=nDistance/100.0;
	
	m_dbSatsPort=m_szRecvBuf[6];		//20070206 for add other byte of sats input port
	m_dbSatsPort2=m_szRecvBuf[7];		//20070206 for add other byte of sats input port
	m_nSatsVersion=m_szRecvBuf[8];
	m_nAtoSpeed=m_szRecvBuf[10]<<8;		//20070516 Ato speed receive from sats
	m_nAtoSpeed+=m_szRecvBuf[9];
	m_dbSatsPort3=m_szRecvBuf[11];		//20070516 Serial port message count or analog value two bytes
	m_dbSatsPort4=m_szRecvBuf[12];		//20070516 Serial port message count or analog value two bytes

	m_dbSatsPort5=m_szRecvBuf[13];		//20070516 Serial port message count or analog value two bytes
	m_dbSatsPort6=m_szRecvBuf[14];		//20070516 Serial port message count or analog value two bytes

	if (m_bRebootSystem)
	{
		m_bRebootSystem=false;
		m_dDistanceLast=m_dDistance;
	}
	
	CString str;
	str.Format("%2X %2X %2X %2X %2X",m_szRecvBuf[7],m_szRecvBuf[11],m_szRecvBuf[12],m_szRecvBuf[13],m_szRecvBuf[14]);
	AddMsg(str,false);

	m_nRecvSum=0;
	memset(m_szRecvBuf,0,sizeof(m_szRecvBuf));
}
void CMySats::MakeSendBuf_Apr(void)
{	int i;
	CString str;
	memset(m_szSendBuf,0,sizeof(m_szSendBuf));

	i=0;
	m_szSendBuf[i++]=0x68;
	m_szSendBuf[i++]=0x17;

	int nDistance=(int)(fabs(m_dPosition-m_stuDevs.stuDevs[m_nAprOutputPos].dPara[0])+m_dDistance+m_dAprReaderOffset)*100; //20070812 add m_dAprReaderOffset

	m_szSendBuf[i++]=(nDistance & 0xff000000)>>24;
	m_szSendBuf[i++]=(nDistance & 0x00ff0000)>>16;
	m_szSendBuf[i++]=(nDistance & 0x0000ff00)>>8;
	m_szSendBuf[i++]=(nDistance & 0x000000ff)>>0;

	int n=m_strMapData.Find(m_stuDevs.stuDevs[m_nAprOutputPos].szName);
	if (n!=-1)
	{
		memcpy(&m_szSendBuf[i],m_strMapData.Mid(n+4,55),55);			
	}
	i+=55;
	m_szSendBuf[i++]=0x0d;
	m_szSendBuf[i++]=0x0a;
	m_szSendBuf[i]=0;
}
void CMySats::MakeSendBuf_AtoLoop(void)
{	int i;
	CString str;
	memset(m_szSendBuf,0,sizeof(m_szSendBuf));

	i=0;
	m_szSendBuf[i++]=0x68;
	m_szSendBuf[i++]=0x18;
	
	if (m_bInAtoMode)
	{
		m_szSendBuf[i++]=0x01;		//ATO/ATP TEST
	}
	else
	{
		m_szSendBuf[i++]=0x00;		//ATO/ATP TEST
	}

	m_nAtoLoopEnable=2;

	int nDistance=(int)(fabs(m_dPosition-m_stuDevs.stuDevs[m_nAtoLoopOutputPos].dPara[0])+m_dDistance+m_dAtoAntennaOffset)*100;
	m_szSendBuf[i++]=(nDistance & 0xff000000)>>24;
	m_szSendBuf[i++]=(nDistance & 0x00ff0000)>>16;
	m_szSendBuf[i++]=(nDistance & 0x0000ff00)>>8;
	m_szSendBuf[i++]=(nDistance & 0x000000ff)>>0;

	m_szSendBuf[i++]=m_stuDevs.stuDevs[m_nAtoLoopOutputPos].nInOut;
	m_szSendBuf[i++]=(BYTE)(m_nAtoCoe);
	m_szSendBuf[i++]=0;
	m_szSendBuf[i++]=0;
	m_szSendBuf[i++]=0x00;
	for(int j=0;j<i;j++)
	{
		m_szSendBuf[i]+=m_szSendBuf[j];
	}
}
void CMySats::MakeSendBuf_Speed(void)
{	
	int i;
	static int k=0,nLaskTrack=0;

	CString str;
	memset(m_szSendBuf,0,sizeof(m_szSendBuf));

	i=0;
	m_szSendBuf[i++]=0x68;
	m_szSendBuf[i++]=0x16;
	if (m_nTrackOutputCar==0) 
	{
		m_nTrackOutputCar=4;
	}
	if (m_nTrackOutputMod==0)
	{
		m_nTrackOutputMod=1;
	}

	m_szSendBuf[i++]=m_nTrackOutputCar;
	m_szSendBuf[i++]=m_nTrackOutputMod;
	
	if (m_nCurTrack!=-1 && m_stuDevs.stuDevs[m_nCurTrack].nPara[eTrackIntCarSet]==10)
	{
		m_bSendGap=true;
		m_bAutoSendGap=true;
		m_nGap=5000;
		k++;
		if (k==1)
		{
			str.Format("Stop %s track code!",m_stuDevs.stuDevs[m_nCurTrack].szName);
			nLaskTrack=m_nCurTrack;
			AddMsg(str);
		}
	}
	else 
	{
		m_bAutoSendGap=false;
		if (k)
		{
			str.Format("Restore %s track code!",m_stuDevs.stuDevs[nLaskTrack].szName);
			AddMsg(str);
			m_nGap=0;
		}
		k=0;
	}

	if (m_bSendGap || m_bRenewGap || m_bAutoSendGap)
	{
		m_szSendBuf[i++]=(m_nGap>>8) & 0xff;
		m_szSendBuf[i++]=m_nGap & 0xff;

		m_bSendGap=false;
		
		if ((m_bSendGap || m_bRenewGap) && !m_bAutoSendGap)
		{
			str.Format("Send gap %d mS.",m_nGap);
			AddMsg(str);
		}
	
		m_bRenewGap=false;
	}
	else
	{
		m_szSendBuf[i++]=0;
		m_szSendBuf[i++]=0;
	}
	
	int nSpeed=(int)(m_dSpeed*10.0);
	m_szSendBuf[i++]=(nSpeed >> 8) & 0xff;
	m_szSendBuf[i++]=nSpeed & 0xff;

	m_szSendBuf[i++]=m_nTechoDir;
//	ASSERT(m_nTechoDir==1);

//	m_szSendBuf[i++]=1;
	static int nLastSpeed=0;
	static int nCount=0;
	if (nLastSpeed==nSpeed)
	{
		nCount++;
	}
	else
	{
		ASSERT(m_bTest==false);
		nLastSpeed=nSpeed;
		nCount=0;
	}

	for(int k=0;k<7;k++)
	{
		m_szSendBuf[i++]=m_ucAtoLoopData[k];
	}

	//use dopplar bit to send ato apply cmd
	if (m_nAtoLoopEnable)
	{
		m_nAtoLoopEnable--;
		m_szSendBuf[i++]=0x01;		//ATO/ATP TEST
	}
	else
	{
		m_szSendBuf[i++]=0x00;		//ATO/ATP TEST
	}
	
	m_szSendBuf[i++]=0;

	m_szSendBuf[i++]=m_nDmPosition+1;

	unsigned ucSum=0;
	for(int j=0;j<i;j++)
	{
		ucSum+=m_szSendBuf[j];
	}
	m_szSendBuf[i++]=ucSum;
}

void CMySats::Patrol_Track(void)
{	static int nLastTrackId=-1;

	CString str;

	for(int i=0;i<m_stuDevs.nDevSum;i++)
	{
		if (m_stuDevs.stuDevs[i].nType==eDevTypeTrack)
		{	
			if (m_dPosition>m_stuDevs.stuDevs[i].dPara[0]+m_dAtpAntennaOffset && m_dPosition<m_stuDevs.stuDevs[i].dPara[1]+m_dAtpAntennaOffset ||		//20070812 add m_dAtpAntennaOffset
				m_dPosition>m_stuDevs.stuDevs[i].dPara[1]-m_dAtpAntennaOffset && m_dPosition<m_stuDevs.stuDevs[i].dPara[0]-m_dAtpAntennaOffset 	)
			{	
				m_nCurTrack=i;

				int nTemp=m_nTrackOutputCar;
				if (m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet])
				{
					m_nTrackOutputCar=m_stuDevs.stuDevs[i].nPara[eTrackIntCarSet];
				}
				else
				{
					m_nTrackOutputCar=m_stuDevs.stuDevs[i].nPara[eTrackIntCar];
				}
				if (nTemp!=m_nTrackOutputCar)
				{
					str.Format("Change Track %s F%d M%d %s",m_stuDevs.stuDevs[i].szName,m_nTrackOutputCar,m_nTrackOutputMod,m_szSpeedCodeName[m_nTrackOutputMod]);
					AddMsg(str);
				}
				
				nTemp=m_nTrackOutputMod;
				if (m_stuDevs.stuDevs[i].nPara[eTrackIntModSet])
				{
					m_nTrackOutputMod=m_stuDevs.stuDevs[i].nPara[eTrackIntModSet];
				}
				else
				{
					m_nTrackOutputMod=m_stuDevs.stuDevs[i].nPara[eTrackIntMod];
				}
				if (nTemp!=m_nTrackOutputMod)
				{
					str.Format("Change Track %s F%d M%d %s",m_stuDevs.stuDevs[i].szName,m_nTrackOutputCar,m_nTrackOutputMod,m_szSpeedCodeName[m_nTrackOutputMod]);
					AddMsg(str);
				}

/*				if (nLastTrackId!=i)
				{	
					str.Format("Change Track %s F%d M%d %s",m_stuDevs.stuDevs[i].szName,m_nTrackOutputCar,m_nTrackOutputMod,m_szSpeedCodeName[m_nTrackOutputMod]);
					AddMsg(str);
					nLastTrackId=i;
				}
*/
			}
		}
	}

	if (m_nTrackCar!=0)
	{
		m_nTrackOutputCar=m_nTrackCar; //手动优先
	}
	if (m_nTrackMod!=0)
	{
		m_nTrackOutputMod=m_nTrackMod; //手动优先
	}
}
void CMySats::Patrol_Apr(void)
{	static int nLastAprId=-1;

	CString str;
	for(int i=0;i<m_stuDevs.nDevSum;i++)
	{
		if (m_stuDevs.stuDevs[i].nType==eDevTypeApr)
		{				
			bool b=false;
			
			if (IsRunToRight())
			{
				if (m_dPosition>m_stuDevs.stuDevs[i].dPara[0]-PRE_APR &&
					m_dPosition<m_stuDevs.stuDevs[i].dPara[0])
					b=true;
			}
			else
			{
				if (m_dPosition<m_stuDevs.stuDevs[i].dPara[0]+PRE_APR &&
					m_dPosition>m_stuDevs.stuDevs[i].dPara[0])
					b=true;
			}

			if (b && (nLastAprId!=i))
			{
				if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing])
				{
					str.Format("Missing Apr %s",m_stuDevs.stuDevs[i].szName);
				}
				else
				{
					str.Format("Send Apr %s %0.f",m_stuDevs.stuDevs[i].szName,m_stuDevs.stuDevs[i].dPara[0]+m_dStartPosBak);
				}

				AddMsg(str);
				nLastAprId=i;
				m_nAprOutputPos=i;
				//Send by serial
				if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing]==0)
				{
					MakeSendBuf_Apr();
					m_pSerial1->WriteSerial(m_szSendBuf,64);
				}
			}
		}
	}
}
void CMySats::Patrol_AtoLoop(void)
{	static int nLastAtoLoopId=-1;

	CString str;
	for(int i=0;i<m_stuDevs.nDevSum;i++)
	{
		if (m_stuDevs.stuDevs[i].nType!=eDevTypeAtoLoop) continue;

		bool b=false;
		
		if (IsRunToRight())
		{
			if (m_dPosition>m_stuDevs.stuDevs[i].dPara[0]-PRE_ATOLOOP &&
				m_dPosition<m_stuDevs.stuDevs[i].dPara[0]			  &&
				m_stuDevs.stuDevs[i].nDir==0)
				b=true;
		}
		else
		{
			if (m_dPosition<m_stuDevs.stuDevs[i].dPara[0]+PRE_ATOLOOP &&
				m_dPosition>m_stuDevs.stuDevs[i].dPara[0]			  &&
				m_stuDevs.stuDevs[i].nDir==1)
				b=true;
		}

		if (!b || (nLastAtoLoopId==i)) continue;
		//if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing])
		//{
		//	str.Format("Missing Apr %s",m_stuDevs.stuDevs[i].szName);
		//}
		//else
		{
			str.Format("Send AtoLoop Msg %s %0.f",m_stuDevs.stuDevs[i].szName,m_stuDevs.stuDevs[i].dPara[0]+m_dStartPosBak);
		}

		AddMsg(str);
		nLastAtoLoopId=i;
		m_nAtoLoopOutputPos=i;
		//Send by serial
		//if (m_stuDevs.stuDevs[i].nPara[eAprIntMissing]==0)
		{

			memcpy(&m_ucAtoLoopData,&m_stuDevs.stuDevs[i].ucValue,ATPLOOP_DATA_MAX);
			MakeSendBuf_AtoLoop();
			m_pSerial1->WriteSerial(m_szSendBuf,10);
		}
	}
}
void CMySats::SendAtoEnabelCmd(bool bEnable)
{
	memset(m_szSendBuf,0,sizeof(m_szSendBuf));
	
	m_szSendBuf[0]=0x68;
	m_szSendBuf[1]=0x18;
	
	if (bEnable)
		m_szSendBuf[2]=0x02;
	else
		m_szSendBuf[2]=0x00;

	m_szSendBuf[8]=(BYTE)(m_nAtoCoe);

	unsigned ucSum=0;
	for(int j=0;j<11;j++)
	{
		ucSum+=m_szSendBuf[j];
	}
	m_szSendBuf[12]=ucSum;

	m_pSerial1->WriteSerial(m_szSendBuf,10);
}
void CMySats::TimerTask(void)
{	CString str;

	Patrol_Track();

	Patrol_Apr();

	Patrol_AtoLoop();

	MakeSendBuf_Speed();

	m_pSerial1->WriteSerial(m_szSendBuf,20);

	ReadSerial();

	DoWithRecvMsg();
}
