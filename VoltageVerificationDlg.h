
// VoltageVerificationDlg.h : ͷ�ļ�
//

#pragma once
#include "Serial.h"
#include "ModbusSerial.h"
#include "afxwin.h"

// CVoltageVerificationDlg �Ի���
class CVoltageVerificationDlg : public CDialogEx
{
// ����
public:
	CVoltageVerificationDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VOLTAGEVERIFICATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
