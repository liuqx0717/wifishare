
// wifishareDlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "AboutDlg.h"
#include "wifishareCommonLib.h"


// CwifishareDlg 对话框
class CwifishareDlg : public CDialogEx
{
	// 构造
public:
	CwifishareDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CwifishareDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WIFISHARE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	bool m_Programatically;
	bool m_IsPasswordHidden;
	lqx::ADAPTER_INFO m_AdapterInfos[MAX_NUMBER_OF_ADAPTERS];
	size_t m_AdapterCount;
	int m_State_StartSharingButton;
	int m_State_StartHostedNetworkButton;
	int m_State_DisableButton;
	CString m_SelectedDeviceName;
	bool m_StartSharing;
	HANDLE m_hMutex_GetAllData;
	CAboutDlg m_AboutDlg;
	bool m_AboutDlgShown;
	bool m_IsPasswordValid;


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void OnOK();
	void GetAllData();
	//0：显示“启用共享”  1：显示“正在启用...”  2：显示“停止共享”  3：显示“正在停止...”  4:按钮失效
	void ChangeState_StartSharingButton(int state);
	//0：显示“开启热点”  1：显示“正在开启...”  2：显示“关闭热点”  3：显示“正在关闭...”  4:按钮失效
	void ChangeState_StartHostedNetworkButton(int state);
	//0：显示“禁用”  1：显示“正在禁用...”  2：显示“启用”  3：显示“正在启用...”  4:按钮失效
	void ChangeState_DisableButton(int state);
	void InitControls();

	static UINT StartHostedNetwork(LPVOID p);
	static UINT StopHostedNetwork(LPVOID p);
	static UINT StartSharing(LPVOID p);
	static UINT StopSharing(LPVOID p);
	static UINT DisableAdapter(LPVOID p);
	static UINT EnableAdapter(LPVOID p);
	static UINT ShowAboutDlg(LPVOID p);

public:


	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	// SSID编辑框
	CEdit m_EditBox_SSID;
	// 承载网络密码
	CEdit m_EditBox_Key;
	// 承载网络状态
	CEdit m_EditBox_State1;
	// 最大连接数
	CEdit m_EditBox_MaxNumberOfPeers;
	// 承载网络IP
	CEdit m_EditBox_IP1;
	// 承载网络连接数
	CEdit m_EditBox_NumberOfPeers;
	// 承载网络认证方式
	CEdit m_EditBox_AuthAlgo;
	// 承载网络加密算法
	CEdit m_EditBox_CipherAlgo;
	// 选择的有线网卡
	CComboBox m_ComboBox_Interface;
	// 有线网卡名称
	CEdit m_EditBox_Name;
	// 有线网卡设备名
	CEdit m_EditBox_DeviceName;
	// 有线网卡IP
	CEdit m_EditBox_IP2;
	// 有线网卡状态
	CEdit m_EditBox_State2;
	// 有线网卡共享状态
	CEdit m_EditBox_SharingState;
	// 显示密码
	CButton m_Button_ShowPassword;
	// 保存修改
	CButton m_Button_Save;
	// 禁用
	CButton m_Button_Disable;
	// 开启热点
	CButton m_Button_StartHostedNetwork;
	// 启用共享
	CButton m_Button_StartSharing;
	// 刷新数据
	CButton m_Button_Refresh;
	// 关闭窗口
	CButton m_Button_Close;
	// 退出程序
	CButton m_Button_Exit;
	// 说明 按钮
	CButton m_Button_About;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton9();
	// 有线网卡IPv6地址
	CEdit m_EditBox_IP3;
	afx_msg void OnBnClickedButton10();
};
