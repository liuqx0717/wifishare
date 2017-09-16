
// wifishare.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "wifishare.h"
#include "wifishareDlg.h"
#include "wifishareKernel.h"
#include "wifishareCommonLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwifishareApp

BEGIN_MESSAGE_MAP(CwifishareApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CwifishareApp 构造

CwifishareApp::CwifishareApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CwifishareApp 对象

CwifishareApp theApp;


// CwifishareApp 初始化

BOOL CwifishareApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("liuqx"));

	if (GetProfileInt(L"Config", L"ShowVirtualAdapters", 0) == 0)
		WriteProfileInt(L"Config", L"ShowVirtualAdapters", 0);


	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_START);
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		SetEvent(hEvent);
		ResetEvent(hEvent);
		return 0;
	}
	else if (!hEvent) {
		MessageBox(NULL, L"CreateEvent调用失败。", L"wifishare", 16);
	}

	CwifishareDlg dlg;
	INT_PTR nResponse;


	if (!lqx::IsRunasAdmin()) {
		//如果没有管理员权限，则请求以管理员身份运行
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
		wchar_t exefile[MAX_PATH];
		wcscpy_s(exefile, MAX_PATH, GetCommandLine());
		sei.lpVerb = TEXT("runas");
		sei.lpFile = lqx::trimw(exefile, L"\" ");     //去除两边的空格和双引号
		sei.nShow = SW_SHOWNORMAL;
		if (!ShellExecuteEx(&sei)) {
			DWORD dwStatus = GetLastError();
			if (dwStatus == ERROR_CANCELLED) {
				MessageBox(NULL, L"请以管理员身份运行。", L"wifishare", 16);
			}
			else if (dwStatus == ERROR_FILE_NOT_FOUND) {
				//printf("所要执行的文件没有找到\n");
			}
		}
		return 0;
	}

	//初始化windock
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		MessageBox(NULL, L"WSAStartup调用失败。", L"wifishare", 16);
		goto finish;
	}




start: //显示主对话框

	   //m_pMainWnd = &dlg;
	try {

		nResponse = dlg.DoModal();

		if (nResponse == DIALOGRESULT_CLOSE)
		{
			// TODO: 在此放置处理何时用
			//  “确定”来关闭对话框的代码
			WaitForSingleObject(hEvent, INFINITE);
			goto start;

		}
		else if (nResponse == DIALOGRESULT_EXIT)
		{
			// TODO: 在此放置处理何时用
			//  “取消”来关闭对话框的代码
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
			TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
		}

	}
	catch (wchar_t *Msg) {
		MessageBox(NULL, Msg, L"wifishare", 16);
	}


finish:

	WSACleanup();

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

