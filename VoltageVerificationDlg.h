
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

private:
	CSerial * m_serial;
	CModbusSerial* pModbus;

	void initView();
public:
	afx_msg void OnBnClickedopenserialbutton();
	CComboBox m_serialPortComb;
	CComboBox m_serialBtlComb;
	CButton m_openSerialButton;
	CButton m_closeSerialButton;
};