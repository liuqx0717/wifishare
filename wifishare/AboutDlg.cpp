// AboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wifishare.h"
#include "AboutDlg.h"
#include "afxdialogex.h"
#include "wifishareKernel.h"


// CAboutDlg 对话框

IMPLEMENT_DYNAMIC(CAboutDlg, CDialogEx)

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ABOUT_DIALOG, pParent)
{

}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_Restart);
	DDX_Control(pDX, IDC_EDIT2, m_EditBox_Link);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAboutDlg 消息处理程序


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ChangeState_RestartButton(0);
	m_EditBox_Link.SetWindowTextW(L"https://github.com/liuqx0717/wifishare");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAboutDlg::OnPaint()
{
					   // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
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

HCURSOR CAboutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


UINT CAboutDlg::RestartICSService(LPVOID p)
{
	CAboutDlg *dlg = (CAboutDlg*)p;
	int ret;

	ret = lqx::ChangeServiceStatusW(L"SharedAccess", false, 25);
	if (ret == ERROR_SERVICE_REQUEST_TIMEOUT) {
		AfxMessageBox(L"操作超时。");
		dlg->ChangeState_RestartButton(0);
		return 0;
	}
	else if (ret != ERROR_SUCCESS && ret != ERROR_SERVICE_NOT_ACTIVE) {
		AfxMessageBox(L"操作失败。");
		dlg->ChangeState_RestartButton(0);
		return 0;
	}

	dlg->ChangeState_RestartButton(2);
	ret = lqx::ChangeServiceStatusW(L"SharedAccess", true, 25);
	if (ret == ERROR_SERVICE_REQUEST_TIMEOUT) {
		AfxMessageBox(L"操作超时。");
		dlg->ChangeState_RestartButton(0);
		return 0;
	}
	else if (ret != ERROR_SUCCESS ) {
		AfxMessageBox(L"操作失败。");
		dlg->ChangeState_RestartButton(0);
		return 0;
	}

	dlg->ChangeState_RestartButton(0);
	return 0;

}

void CAboutDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ChangeState_RestartButton(1);
	AfxBeginThread(&RestartICSService, this);

}



//0：显示“重新启动ICS服务”  1：显示“正在停止ICS服务...”  2：显示“正在启动ICS服务...” 
void CAboutDlg::ChangeState_RestartButton(int state)
{

	switch (state)
	{
	case 0:
		m_Button_Restart.SetWindowTextW(L"重新启动ICS服务");
		m_Button_Restart.EnableWindow(TRUE);
		m_State_RestartButton = state;
		break;
	case 1:
		m_Button_Restart.SetWindowTextW(L"正在停止ICS服务...");
		m_Button_Restart.EnableWindow(FALSE);
		m_State_RestartButton = state;
		break;
	case 2:
		m_Button_Restart.SetWindowTextW(L"正在启动ICS服务...");
		m_Button_Restart.EnableWindow(FALSE);
		m_State_RestartButton = state;
		break;

	default:
		break;
	}
}