#pragma once

#define READ_BUF_MAX			4096
#define WRITE_BUF_MAX			4096
#define READ_LIST_BUF_MAX		10240
#define WRITE_LIST_BUF_MAX		5120
#define LINE_LONG_MAX			80
#define SERIAL_SHOW				1

class CSerial
{
private:
	CWinThread *m_pThread;
	HANDLE m_hMutex;
	HANDLE m_hSerial;
	HANDLE m_hTermEvent;
	OVERLAPPED m_oRead;
	OVERLAPPED m_oWrite;
	bool m_bContinue;
	int m_nSerialPort;
	int m_nBaudRate;
	int m_nByteSize;
	int m_nParity;
	int m_nStopBits;
	int  m_nReadHead;
	int  m_nReadTail;
	int  m_nWriteHead;
	int  m_nWriteTail;
	int  m_nHead;
	int  m_nTail;
//	struct timeval tv;
	unsigned char m_szReadBuf[READ_BUF_MAX];
	unsigned char m_szWriteBuf[WRITE_BUF_MAX];
	unsigned char m_szReadList[READ_LIST_BUF_MAX];
	unsigned char m_szWriteList[READ_LIST_BUF_MAX];
	char m_szSerialName[LINE_LONG_MAX];
	char m_szSerialInit[LINE_LONG_MAX];

public:
	static int ReadSerialThread(void *pVoid);
	static int ReadSerial(CSerial *This);
	int GetReceived(char *sReceived);
	int Create(HANDLE hTermEvent,char *szInit);
	static bool ReadPush(CSerial *This,unsigned char *s,int nLen);
	static int Read(CSerial *This,unsigned char *s);
	void WriteSerial(unsigned char *szSendBuf,int nLen);
	void CloseSerial(void);
	int OpenSerial(CSerial *This);
	bool StartSerialThread(CSerial *This);

public:
	CSerial(void);
	~CSerial(void);
};

