#include "stdafx.h"

#include "wifishareCommonLib.h"
#include "wifishareKernel.h"


namespace lqx {


	size_t BytesToHexStringW(const unsigned char *input, size_t inputlen, wchar_t *output, size_t outputlen) {
		wchar_t HEXW[16] = {
			'0', '1', '2', '3',
			'4', '5', '6', '7',
			'8', '9', 'A', 'B',
			'C', 'D', 'E', 'F'
		};
		size_t n = 0;
		if (outputlen < 3) return 0;
		for (size_t i = 0; i < inputlen && n < outputlen - 2; ++i) {
			int t = input[i];
			int a = t / 16;
			int b = t % 16;
			output[n++] = HEXW[a];
			output[n++] = HEXW[b];
		}
		output[n] = 0;
		return n;
	}

	size_t HexStringToBytesW(const wchar_t *input, unsigned char *output, size_t OutputBuffSize)
	{
		size_t n = 0;
		wchar_t tmp[3];
		wchar_t *endpos;
		tmp[2] = 0;
		size_t i, j;
		for (i = 0, j = 0; input[i] && j < OutputBuffSize; ++i) {
			wchar_t c = input[i];
			if (c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f') {
				tmp[n] = c;
				n++;
				if (n == 2) {
					output[j] = (unsigned char)wcstoul(tmp, &endpos, 16);
					j++;
					n = 0;
				}
			}
		}
		if (n != 0) throw L"字符 0~9，a~f，A~F 出现的次数不是偶数。";
		return j;
	}


	wchar_t *trimw(wchar_t *str, wchar_t *Charactors, _Trimtype Trimtype)
	{
		int start = 0, end;
		for (end = 0; str[end]; end++);
		int end_old = end -= 1;
		int i;
		wchar_t c;
		bool f;

		if (Trimtype&TRIM_LEFT) {
			for (; start <= end; start++) {
				f = true;
				for (int i = 0; c = Charactors[i]; i++) {
					if (str[start] == c) {
						f = false;
						break;
					}
				}
				if (f) break;
			}
		}
		if (Trimtype&TRIM_RIGHT) {
			for (; end >= start; end--) {
				f = true;
				for (int i = 0; c = Charactors[i]; i++) {
					if (str[end] == c) {
						f = false;
						break;
					}
				}
				if (f) break;
			}
		}
		if (start == 0 && end == end_old) return str;
		for (i = 0; i <= end - start; i++)
			str[i] = str[start + i];
		str[i] = '\0';

		return str;


	}


	BOOL IsRunasAdmin()
	{
		BOOL bElevated = FALSE;
		HANDLE hToken = NULL;

		// Get current process token  
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
			return FALSE;

		TOKEN_ELEVATION tokenEle;
		DWORD dwRetLen = 0;

		// Retrieve token elevation information  
		if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
		{
			if (dwRetLen == sizeof(tokenEle))
			{
				bElevated = tokenEle.TokenIsElevated;
			}
		}

		CloseHandle(hToken);
		return bElevated;
	}





	ADAPTER_INFO *_wifishareCommonLib_proc1_AdapterInfos;
	size_t _wifishareCommonLib_proc1_MaxCount;
	size_t _wifishareCommonLib_proc1_i;
	
	void wifishareCommonLib_proc1(_NetConnectionInfo *NetConnectionInfo, _SharingType *SharingType, _ConnectingAction *ConnectingAction)
	{
		if (_wifishareCommonLib_proc1_i < _wifishareCommonLib_proc1_MaxCount) {
			_wifishareCommonLib_proc1_AdapterInfos[_wifishareCommonLib_proc1_i].Name.SetString(NetConnectionInfo->Name);
			_wifishareCommonLib_proc1_AdapterInfos[_wifishareCommonLib_proc1_i].DeviceName.SetString(NetConnectionInfo->DeviceName);
			_wifishareCommonLib_proc1_AdapterInfos[_wifishareCommonLib_proc1_i].Status = NetConnectionInfo->Status;
			_wifishareCommonLib_proc1_AdapterInfos[_wifishareCommonLib_proc1_i].SharingType = *SharingType;
		}
		_wifishareCommonLib_proc1_i++;
	}

	void wifishareCommonLib_proc2(PIP_ADAPTER_INFO pIpAdapterInfo)
	{
		wchar_t tmp[256];

		for (size_t i = 0; i < _wifishareCommonLib_proc1_i; i++) {
			MultiByteToWideChar(CP_ACP, 0, pIpAdapterInfo->Description, -1, tmp, 256);
			if (!_wifishareCommonLib_proc1_AdapterInfos[i].DeviceName.Compare(tmp)) {
				IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
				MultiByteToWideChar(CP_ACP, 0, pIpAddrString->IpAddress.String, -1, tmp, 128);
				_wifishareCommonLib_proc1_AdapterInfos[i].IP.SetString(tmp);
			}
			
		}

	}

	size_t GetAllAdaptersInfo(ADAPTER_INFO *AdapterInfos, size_t MaxCount)
	{
		_wifishareCommonLib_proc1_AdapterInfos = AdapterInfos;
		_wifishareCommonLib_proc1_MaxCount = MaxCount;
		_wifishareCommonLib_proc1_i = 0;

		EnumConnections(&wifishareCommonLib_proc1);
		lqx::GetAdaptersInfo(&wifishareCommonLib_proc2);

		return _wifishareCommonLib_proc1_i;

	}





	const wchar_t *_wifishareCommonLib_proc3_DeviceName;
	bool _wifishareCommonLib_proc3_found;

	void wifishareCommonLib_proc3(_NetConnectionInfo *NetConnectionInfo, _SharingType *SharingType, _ConnectingAction *ConnectingAction)
	{

		if (!wcscmp(NetConnectionInfo->DeviceName, _wifishareCommonLib_proc3_DeviceName)) {
			*SharingType = SharingType_Public;
		}
		else if (wcsstr(NetConnectionInfo->DeviceName, HOSTEDNETWORK_DEVICENAME)) {
			if (!_wifishareCommonLib_proc3_found)
			{
				*SharingType = SharingType_Private;
				_wifishareCommonLib_proc3_found = true;
			}
		}

	}

	void StartSharing(const wchar_t *DeviceName)
	{
		StopSharing();

		_wifishareCommonLib_proc3_DeviceName = DeviceName;
		_wifishareCommonLib_proc3_found = false;
		EnumConnections(&wifishareCommonLib_proc3);
		
	}






	void wifishareCommonLib_proc4(_NetConnectionInfo *NetConnectionInfo, _SharingType *SharingType, _ConnectingAction *ConnectingAction)
	{
		*SharingType = lqx::SharingType_None;
	}

	void StopSharing()
	{
		EnumConnections(&wifishareCommonLib_proc4);
	}




	
	const wchar_t *_wifishareCommonLib_proc5_DeviceName;
	bool _wifishareCommonLib_proc5_Connect;

	void wifishareCommonLib_proc5(_NetConnectionInfo *NetConnectionInfo, _SharingType *SharingType, _ConnectingAction *ConnectingAction)
	{
		if (!wcscmp(NetConnectionInfo->DeviceName, _wifishareCommonLib_proc5_DeviceName)) {
			*ConnectingAction = _wifishareCommonLib_proc5_Connect ? ConnectingAction_Connect : ConnectingAction_Disconnect;
		}
	}

	void ChangeAdapterSdate(const wchar_t *DeviceName, bool Connect)
	{
		_wifishareCommonLib_proc5_DeviceName = DeviceName;
		_wifishareCommonLib_proc5_Connect = Connect;

		EnumConnections(&wifishareCommonLib_proc5);
	}




}