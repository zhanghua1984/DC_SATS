#include "stdafx.h"
#include ".\serial.h"

CSerial::CSerial(void)
{
	m_nBaudRate=9600;
	m_nParity=NOPARITY;
	m_nByteSize=8;
	m_nStopBits=ONESTOPBIT;

	m_hSerial=NULL;
	m_hTermEvent=0;

	strcpy(m_szSerialName,"COM1");
	memset(&m_oWrite,0,sizeof(OVERLAPPED));
	memset(&m_oRead,0,sizeof(OVERLAPPED));
	m_oRead.hEvent=CreateEvent(NULL,true,FALSE,NULL);
	m_oWrite.hEvent=CreateEvent(NULL,true,FALSE,NULL);

	memset(m_szReadList,0,sizeof(m_szReadList));
	m_nHead=0;
	m_nTail=0;
	m_hMutex=CreateMutex(NULL,FALSE,NULL);

	m_pThread=NULL;
	m_bContinue=1;
}
CSerial::~CSerial(void)
{
	m_bContinue=0;

	CloseSerial();
}
int CSerial::ReadSerial(CSerial *This)
{	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength=0;

	ClearCommError( This->m_hSerial, &dwErrorFlags, &ComStat );
	dwLength = min(	1,ComStat.cbInQue );	

	if (dwLength > 0)
	{
		if (ReadFile(This->m_hSerial,This->m_szReadBuf+This->m_oRead.Offset,dwLength,&dwLength,&This->m_oRead)==0)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				if(WaitForSingleObject(This->m_oRead.hEvent,-1))
				dwLength=0;
			}
			else
			{
				GetOverlappedResult(This->m_hSerial,&This->m_oRead,&dwLength,FALSE);
				This->m_oRead.Offset+=dwLength;
			}
		}
		else
		{
			GetOverlappedResult(This->m_hSerial,&This->m_oRead,&dwLength,FALSE);
			This->m_oRead.Offset+=dwLength;
		}
	}

	return dwLength;
} 
int CSerial::GetReceived(char *sReceived)
{	
	memcpy(sReceived,m_szReadBuf,m_oRead.Offset);

	ResetEvent(m_oRead.hEvent);

	return m_oRead.Offset;
}

bool CSerial::ReadPush(CSerial *This,unsigned char *s,int nLen)
{
	WaitForSingleObject(This->m_hMutex,INFINITE);

	int n=0;
	if (This->m_nTail>=This->m_nHead)
		n=READ_LIST_BUF_MAX-(This->m_nTail-This->m_nHead)-1;
	else
		n=This->m_nHead-This->m_nTail;

	bool bSuccess=(n>=nLen);
	if (bSuccess)
	{
		if (This->m_nTail>=This->m_nHead)
		{

			if (READ_LIST_BUF_MAX-This->m_nTail-1>=nLen)
			{
				memcpy(&This->m_szReadList[This->m_nTail],s,nLen);
				This->m_nTail+=nLen;
			}
			else
			{
				n=READ_LIST_BUF_MAX-This->m_nTail-1;
				memcpy(&This->m_szReadList[This->m_nTail],&s[0],n);
				memcpy(&This->m_szReadList[0],&s[n],nLen-n);
				This->m_nTail=nLen-n;
			}
		}
		else
		{
			memcpy(&This->m_szReadList[This->m_nTail],s,nLen);
			This->m_nTail+=nLen;
		}
	}

	ReleaseMutex(This->m_hMutex);

	return bSuccess;
}
int CSerial::Read(CSerial *This,unsigned char *s)
{
	WaitForSingleObject(This->m_hMutex,INFINITE);
	
	int n=0;
	if (This->m_nHead!=This->m_nTail) 
	{
		if (This->m_nHead<This->m_nTail)
		{
			n=This->m_nTail-This->m_nHead;
			memcpy(s,&This->m_szReadList[This->m_nHead],n);
		}
		else
		{
			n=READ_LIST_BUF_MAX-This->m_nHead-1;
			memcpy(s,&This->m_szReadList[This->m_nHead],n);
			memcpy(&s[n],&This->m_szReadList[0],This->m_nHead);
			n+=This->m_nHead;
		}
	}
	s[n]=0;
	This->m_nHead=This->m_nTail=0;
	ReleaseMutex(This->m_hMutex);
	return n;
}
bool CSerial::StartSerialThread(CSerial *This)
{
	if(OpenSerial(This))
	{
		This->m_pThread=AfxBeginThread((AFX_THREADPROC)ReadSerialThread,This,THREAD_PRIORITY_BELOW_NORMAL,0, CREATE_SUSPENDED);
		This->m_pThread->m_bAutoDelete=FALSE;
		This->m_pThread->ResumeThread();
		return true;
	}
	return false;
}

int CSerial::ReadSerialThread(void *pVoid)
{
	DWORD dwEventMask,dwTransfer;
	DWORD nLength=0;
	CString str;
	CSerial *This=(CSerial*)pVoid;

 	while(This->m_bContinue)
	{
		dwEventMask=0;
		if(!WaitCommEvent(This->m_hSerial,&dwEventMask,&This->m_oRead))
		{
			if(ERROR_IO_PENDING==GetLastError())
			{
				GetOverlappedResult(This->m_hSerial,&This->m_oRead,&dwTransfer,true);
			}
		}
		if((dwEventMask & EV_RXCHAR)==EV_RXCHAR)
		{
			bool bHaveData=FALSE;
			memset(This->m_szReadBuf,0,sizeof(This->m_szReadBuf));
			This->m_oRead.Offset=0;
			while(nLength = ReadSerial(This))
			{
				bHaveData=true;
			}
			if (bHaveData)
			{
				str.Format("%s SRecv %s",This->m_szSerialName,This->m_szReadBuf);
				WaitForSingleObject(This->m_hMutex,INFINITE);
				ReadPush(This,This->m_szReadBuf,This->m_oRead.Offset);
				ReleaseMutex(This->m_hMutex);
				SetEvent(This->m_hTermEvent);
			}
		}
	}
	return nLength;
}
void CSerial::CloseSerial(void)
{
	if (m_hSerial==0)
		return;
	SetCommMask(m_hSerial,0);
	SetEvent(NULL);
	Sleep(500);
	WaitForSingleObject(m_pThread,WAIT_TIMEOUT);
	EscapeCommFunction(m_hSerial,CLRDTR);
	PurgeComm(m_hSerial,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	CloseHandle(m_hSerial);
}
int CSerial::OpenSerial(CSerial *This)
{
	if ((This->m_hSerial=CreateFile(m_szSerialName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL))==NULL) return 1;
	SetCommMask(This->m_hSerial,EV_RXCHAR);
	SetupComm(This->m_hSerial,2048,4096);

	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout=0xffffffff;
	cto.ReadTotalTimeoutMultiplier=0;
	cto.ReadTotalTimeoutConstant=0;
	cto.WriteTotalTimeoutMultiplier=0;
	cto.WriteTotalTimeoutConstant=5000;
	SetCommTimeouts(This->m_hSerial,&cto);


	DCB dcb;
	dcb.DCBlength=sizeof(DCB);
	if (!GetCommState(This->m_hSerial,&dcb)) return 0;
//	BOOL COM1: baud=1200 parity=N data=8 stop=1 
	BOOL b=BuildCommDCB(This->m_szSerialInit,&dcb);
	//dcb.BaudRate=This->m_nBaudRate;
	//dcb.ByteSize=This->m_nByteSize;
	//dcb.Parity=This->m_nParity;
	//dcb.StopBits=This->m_nStopBits;
	//dcb.BaudRate=9600;
	//dcb.ByteSize=8;
	//dcb.Parity=0;
	//dcb.StopBits=0;
	if (!SetCommState(This->m_hSerial,&dcb)) return 0;

	char chs[100];
//	sprintf(str,"AT:%s %d %d %d %d",This->m_szSerialName,This->m_nBaudRate,This->m_nParity,This->m_nByteSize,This->m_nStopBits);
	sprintf(chs,"AT:%s",This->m_szSerialInit);
	return 1;
}
void CSerial::WriteSerial(unsigned char *szSendBuf,int nLen)
{
	DWORD   dwWriten=0;
    DWORD   dwErrorFlags;
	COMSTAT ComStat ;

	if(m_hSerial==NULL) return;

    ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
	EscapeCommFunction(m_hSerial,SETRTS);

	if (WriteFile(m_hSerial,szSendBuf,nLen,&dwWriten,&m_oWrite)==0)
	{
		DWORD dwError=GetLastError();
		switch(dwError)
		{
			case ERROR_IO_PENDING:
 					WaitForSingleObject(m_oWrite.hEvent,INFINITE);
					break;
			case ERROR_IO_INCOMPLETE:
 					break;
			default:
					if (GetOverlappedResult(m_hSerial,&m_oWrite,&dwWriten,FALSE)) m_oWrite.Offset+=dwWriten;
					break;
		}
	}
}
int CSerial::Create(HANDLE hTermEvent,char *szInit)
{	m_hTermEvent=hTermEvent;

	char chs[100];
	strcpy(chs,szInit);
	char *p=strstr(chs,":");
	if (p)
	{
		*p=0;
		strcpy(m_szSerialName,chs);
	}
	strcpy(m_szSerialInit,szInit);

	bool bRtn=StartSerialThread(this);

	if (bRtn)
		sprintf(chs,"Create Serial %s success!",m_szSerialName);
	else
		sprintf(chs,"Create Serial %s failure!",m_szSerialName);

	return bRtn;
}
