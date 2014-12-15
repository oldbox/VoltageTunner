
// VoltageVerificationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VoltageVerification.h"
#include "VoltageVerificationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVoltageVerificationDlg �Ի���



CVoltageVerificationDlg::CVoltageVerificationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVoltageVerificationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CVoltageVerificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, serialPortComb, m_serialPortComb);
	DDX_Control(pDX, serialBtlComb, m_serialBtlComb);
	DDX_Control(pDX, openSerialButton, m_openSerialButton);
	DDX_Control(pDX, closeSerialButton, m_closeSerialButton);
}

BEGIN_MESSAGE_MAP(CVoltageVerificationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(openSerialButton, &CVoltageVerificationDlg::OnBnClickedopenserialbutton)
END_MESSAGE_MAP()


// CVoltageVerificationDlg ��Ϣ�������

BOOL CVoltageVerificationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	ModifyStyle(WS_MAXIMIZEBOX, 0);
	initView();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CVoltageVerificationDlg::OnDestroy()
{
	delete m_serial;
	delete pModbus;

	return true;
}

void CVoltageVerificationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVoltageVerificationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVoltageVerificationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVoltageVerificationDlg::OnBnClickedopenserialbutton()
{
	// TODO: Add your control notification handler code here
	int nPort = (int)m_serialPortComb.GetItemData(m_serialPortComb.GetCurSel());
	int nBaud = (int)m_serialBtlComb.GetItemData(m_serialBtlComb.GetCurSel());

	m_serial = new CSerial();
	if (m_serial->Open(nPort, nBaud))
	{
		pModbus = new CModbusSerial(m_serial);

	}
}


void CVoltageVerificationDlg::initView()
{

	// �˿�
	for (int i = 0; i < 10; i++)
	{
		CString portName;
		portName.Format(_T("�˿�%d"), i);
		m_serialPortComb.InsertString(i, portName);
		m_serialPortComb.SetItemData(i, (DWORD)i);
	}
	m_serialPortComb.SetCurSel(6);

	// ������
	m_serialBtlComb.InsertString(0,_T("9600"));
	m_serialBtlComb.SetItemData(0, (DWORD)9600);
	m_serialBtlComb.InsertString(1, _T("115200"));
	m_serialBtlComb.SetItemData(1, (DWORD)115200);

	m_serialBtlComb.SetCurSel(0);

}