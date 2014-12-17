
// VoltageVerificationDlg.h : 头文件
//

#pragma once
#include "Serial.h"
#include "ModbusSerial.h"
#include "afxwin.h"

// CVoltageVerificationDlg 对话框
class CVoltageVerificationDlg : public CDialogEx
{
// 构造
public:
	CVoltageVerificationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VOLTAGEVERIFICATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnOK();

private:
	CSerial * m_serial;
	CModbusSerial* pModbus;

	void initView();
	int nAddr;
public:
	afx_msg void OnBnClickedopenserialbutton();
	CComboBox m_serialPortComb;
	CComboBox m_serialBtlComb;
	CButton m_openSerialButton;
	CButton m_closeSerialButton;
	afx_msg void OnBnClickedcloseserialbutton();
	afx_msg void OnBnClickedreadmoduleinfobutton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStartVerify();
	void clearVoltageData();
	afx_msg void OnBnClickedsetmodbusaddrbutton();
	afx_msg void OnBnClickedsetbaudbutton();
	CEdit m_addrLineEdit;
	CEdit m_BaudLineEdit;
};
