
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
	DDX_Control(pDX, addrLineEdit, m_addrLineEdit);
	DDX_Control(pDX, moduleBtlLineEdit, m_BaudLineEdit);
}

BEGIN_MESSAGE_MAP(CVoltageVerificationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(openSerialButton, &CVoltageVerificationDlg::OnBnClickedopenserialbutton)
	ON_BN_CLICKED(closeSerialButton, &CVoltageVerificationDlg::OnBnClickedcloseserialbutton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CVoltageVerificationDlg::OnBnClickedStartVerify)
	ON_BN_CLICKED(setModbusAddrButton, &CVoltageVerificationDlg::OnBnClickedsetmodbusaddrbutton)
	ON_BN_CLICKED(setBaudButton, &CVoltageVerificationDlg::OnBnClickedsetbaudbutton)
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
	m_serialPortComb.SetCurSel(5);

	// 波特率
	m_serialBtlComb.InsertString(0, _T("9600"));
	m_serialBtlComb.SetItemData(0, (DWORD)9600);
	m_serialBtlComb.InsertString(1, _T("19200"));
	m_serialBtlComb.SetItemData(1, (DWORD)19200);
	m_serialBtlComb.InsertString(2, _T("115200"));
	m_serialBtlComb.SetItemData(2, (DWORD)115200);

	m_serialBtlComb.SetCurSel(0);

	m_closeSerialButton.EnableWindow(false);

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
		m_openSerialButton.EnableWindow(false);
		m_closeSerialButton.EnableWindow(true);
		MessageBox(_T("串口打开"), _T("提示信息"),
			MB_ICONQUESTION | MB_OK);

		OnBnClickedreadmoduleinfobutton();

		SetTimer(1, 500, NULL);

	}
}


void CVoltageVerificationDlg::OnBnClickedcloseserialbutton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (m_serial)
		if (m_serial->IsOpened())
		{
			if (m_serial->Close())
			{
				KillTimer(1);

				m_openSerialButton.EnableWindow(true);
				m_closeSerialButton.EnableWindow(false);
				MessageBox(_T("串口关闭"), _T("提示信息"),
					MB_ICONASTERISK | MB_OK);

			}
		}
}


void CVoltageVerificationDlg::OnBnClickedreadmoduleinfobutton()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!pModbus || !m_serial->IsOpened())
	{
		MessageBox(_T("串口未打开"), _T("提示信息"),
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}


	for (int i = 0; i < 3; i++)
	{
		Sleep(100);

		unsigned char czData[16] = { '\0' };

		if (pModbus->Read03Data(0xff, 40050, 4, czData))
		{
			int addr = 0, moduleName = 0, baud = 0;
			addr = (int)pModbus->getUInt16(czData, 0);
			moduleName = (int)pModbus->getUInt16(czData, 2);
			baud = (int)pModbus->getUInt16(czData, 6);

			nAddr = addr;

			CString value;
			value.Format(_T("%d"), addr);
			GetDlgItem(addrLineEdit)->SetWindowTextW(value);

			value.Format(_T("%d"), moduleName);
			GetDlgItem(nameLineEdit)->SetWindowTextW(value);

			switch (baud)
			{
			case 0:
				GetDlgItem(moduleBtlLineEdit)->SetWindowTextW(_T("9600"));
				break;
			case 1:
				GetDlgItem(moduleBtlLineEdit)->SetWindowTextW(_T("19200"));
				break;
			case 2:
				GetDlgItem(moduleBtlLineEdit)->SetWindowTextW(_T("115200"));
				break;
			default:
				break;
			}

			break;
		}
	}

}


void CVoltageVerificationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{

		if (!pModbus || !m_serial->IsOpened())
		{
			MessageBox(_T("串口未打开"), _T("提示信息"),
				MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		//OnBnClickedreadmoduleinfobutton();

		unsigned char czData[16] = { '\0' };
		if (pModbus->Read03Data(nAddr, 40052, 6, czData))
		{
			int powerStatus = 0; float adapterVoltage = 0.0, batteryVoltage = 0.0;
			CString powerSupplier, chargeStatus, value;
			powerStatus = (int)pModbus->getUInt16(czData, 0);
			adapterVoltage = pModbus->getFloat(czData, 4);
			batteryVoltage = pModbus->getFloat(czData, 8);

			switch (powerStatus)
			{
			case 1:
				powerSupplier.Format(_T("电池组"));
				chargeStatus.Format(_T("放电"));
				break;
			case 2:
				powerSupplier.Format(_T("适配器"));
				chargeStatus.Format(_T("充满"));
				break;
			case 3:
				powerSupplier.Format(_T("适配器"));
				chargeStatus.Format(_T("充电"));
				break;
			default:
				break;
			}
			GetDlgItem(powerStatusLineEdit)->SetWindowTextW(powerSupplier);
			GetDlgItem(chargeStatusLineEdit)->SetWindowTextW(chargeStatus);

			value.Format(_T("%f"), adapterVoltage);
			GetDlgItem(adapterVoltageLineEdit)->SetWindowTextW(value);


			value.Format(_T("%f"), batteryVoltage);
			GetDlgItem(batteryVoltageLineEdit)->SetWindowTextW(value);

		}
	}


}

void CVoltageVerificationDlg::OnBnClickedStartVerify()
{
	clearVoltageData();

	// 校准

	if (!pModbus || !m_serial->IsOpened())
	{
		MessageBox(_T("串口未打开"), _T("提示信息"),
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	unsigned char czData[2] = { '\0' };
	unsigned short iCommand = 320;
	memcpy(czData, &iCommand, 2);
	unsigned char m_czData[2] = { '\0' };
	m_czData[0] = czData[1];
	m_czData[1] = czData[0];


	for (int i = 0; i < 5; i++) //多几次确保发出
	{
		Sleep(100); //每次查询前需延时
		if (pModbus->Write10Data(nAddr, 40001, 1, m_czData))
		{
			MessageBox(_T("校准命令已发出"), _T("提示信息"),
				MB_ICONEXCLAMATION | MB_OK);
			SetTimer(1, 500, NULL); // 继续刷新
			break;
		}
	}
}


void CVoltageVerificationDlg::clearVoltageData()
{
	// TODO:  在此添加控件通知处理程序代码
	KillTimer(1);

	GetDlgItem(powerStatusLineEdit)->SetWindowTextW(_T(""));
	GetDlgItem(chargeStatusLineEdit)->SetWindowTextW(_T(""));

	GetDlgItem(adapterVoltageLineEdit)->SetWindowTextW(_T(""));
	GetDlgItem(batteryVoltageLineEdit)->SetWindowTextW(_T(""));

	//GetDlgItem(addrLineEdit)->SetWindowTextW(_T(""));
	//GetDlgItem(nameLineEdit)->SetWindowTextW(_T(""));
	//GetDlgItem(moduleBtlLineEdit)->SetWindowTextW(_T(""));

}


void CVoltageVerificationDlg::OnBnClickedsetmodbusaddrbutton()
{
	//clearVoltageData();

	if (!pModbus || !m_serial->IsOpened())
	{
		MessageBox(_T("串口未打开"), _T("提示信息"),
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	unsigned char czData[2] = { '\0' };
	unsigned char czDataValue[2] = { '\0' };

	unsigned short iCommand = 1;
	memcpy(czData, &iCommand, 2);

	CString value;
	m_addrLineEdit.GetWindowTextW(value);

	unsigned short iValue = (unsigned short)_tstoi(value);
	memcpy(czDataValue, &iValue, 2);

	unsigned char m_czData[4] = { '\0' };
	m_czData[0] = czData[1];
	m_czData[1] = czData[0];
	m_czData[2] = czDataValue[1];
	m_czData[3] = czDataValue[0];


	for (int i = 0; i < 5; i++) //多几次确保发出
	{
		Sleep(100); //每次查询前需延时
		if (pModbus->Write10Data(nAddr, 40001, 2, m_czData))
		{
			MessageBox(_T("地址设置成功"), _T("提示信息"),
				MB_ICONEXCLAMATION | MB_OK);

			OnBnClickedreadmoduleinfobutton();
			SetTimer(1, 500, NULL); // 继续刷新
			break;
		}
	}
}


void CVoltageVerificationDlg::OnBnClickedsetbaudbutton()
{
	//clearVoltageData();

	if (!pModbus || !m_serial->IsOpened())
	{
		MessageBox(_T("串口未打开"), _T("提示信息"),
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	unsigned char czData[2] = { '\0' };
	unsigned char czDataValue[2] = { '\0' };

	unsigned short iCommand = 3;
	memcpy(czData, &iCommand, 2);

	CString value;
	GetDlgItem(moduleBtlLineEdit)->GetWindowTextW(value);
	int iValue = (unsigned short)_tstoi(value);
	int baud = -1;
	switch (iValue)
	{
	case 9600:
		baud = 0;
		break;
	case 19200:
		baud = 1;
		break;
	case 115200:
		baud = 2;
		break;
	default:
		MessageBox(_T("只能设置9600， 19200， 115200三个波特率"), _T("提示信息"),
			MB_ICONEXCLAMATION | MB_OK);
		return;
		break;
	}

	memcpy(czDataValue, &baud, 2);

	unsigned char m_czData[4] = { '\0' };
	m_czData[0] = czData[1];
	m_czData[1] = czData[0];
	m_czData[2] = czDataValue[1];
	m_czData[3] = czDataValue[0];


	for (int i = 0; i < 5; i++) //多几次确保发出
	{
		Sleep(100); //每次查询前需延时
		if (pModbus->Write10Data(nAddr, 40001, 2, m_czData))
		{
			MessageBox(_T("波特率设置成功，请等待模块重启"), _T("提示信息"),
				MB_ICONEXCLAMATION | MB_OK);

			OnBnClickedreadmoduleinfobutton();
			SetTimer(1, 500, NULL); // 继续刷新
			break;
		}
	}
}


void CVoltageVerificationDlg::OnOK()
{
	return;
}