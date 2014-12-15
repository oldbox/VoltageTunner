
// VoltageVerificationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VoltageVerification.h"
#include "VoltageVerificationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CVoltageVerificationDlg 对话框



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


// CVoltageVerificationDlg 消息处理程序

BOOL CVoltageVerificationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	ModifyStyle(WS_MAXIMIZEBOX, 0);
	initView();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVoltageVerificationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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

	// 端口
	for (int i = 0; i < 10; i++)
	{
		CString portName;
		portName.Format(_T("端口%d"), i);
		m_serialPortComb.InsertString(i, portName);
		m_serialPortComb.SetItemData(i, (DWORD)i);
	}
	m_serialPortComb.SetCurSel(6);

	// 波特率
	m_serialBtlComb.InsertString(0,_T("9600"));
	m_serialBtlComb.SetItemData(0, (DWORD)9600);
	m_serialBtlComb.InsertString(1, _T("115200"));
	m_serialBtlComb.SetItemData(1, (DWORD)115200);

	m_serialBtlComb.SetCurSel(0);

}