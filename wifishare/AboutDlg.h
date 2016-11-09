#pragma once
#include "afxwin.h"


// CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_DIALOG };
#endif

private:
	void ChangeState_RestartButton(int state);
	int m_State_RestartButton;

	static UINT RestartICSService(LPVOID p);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	

	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButton1();
	// //重新启动ICS服务按钮
	CButton m_Button_Restart;
	CEdit m_EditBox_Link;
};
