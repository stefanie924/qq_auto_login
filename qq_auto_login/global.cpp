#include "stdafx.h"
#include "global.h"
#include <winable.h>
#include <fstream>
//#include <Psapi.h>
//#pragma comment(lib, "Psapi.lib")

#include <TLHELP32.H>
void SendAscii(wchar_t data, BOOL shift)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		SendInput(1, input, sizeof(INPUT));
	}

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = data;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(2, input, sizeof(INPUT));

	if (shift)
	{
		input[0].type = INPUT_KEYBOARD;
		input[0].ki.wVk = VK_SHIFT;
		input[0].ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, input, sizeof(INPUT));  
	}
}


void SendUnicode(wchar_t data)
{
	INPUT input[2];
	memset(input, 0, 2 * sizeof(INPUT));

	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = 0;
	input[0].ki.wScan = data;
	input[0].ki.dwFlags = 0x4;//KEYEVENTF_UNICODE;

	input[1].type = INPUT_KEYBOARD;
	input[1].ki.wVk = 0;
	input[1].ki.wScan = data;
	input[1].ki.dwFlags = KEYEVENTF_KEYUP | 0x4;//KEYEVENTF_UNICODE;

	SendInput(2, input, sizeof(INPUT));
}

//为方便使用，下面这个函数包装了前两个函数。
void SendKeys(CString msg)
{
	short vk;
	BOOL shift;

	USES_CONVERSION;
	wchar_t* data = T2W(msg.GetBuffer(0));
	int len = wcslen(data);

	for(int i=0;i<len;i++)
	{
		if (data[i]>=0 && data[i]<256) //ascii字符
		{
			vk = VkKeyScanW(data[i]);

			if (vk == -1)
			{
				SendUnicode(data[i]);
			}
			else
			{
				if (vk < 0)
				{
					vk = ~vk + 0x1;
				}

				shift = vk >> 8 & 0x1;

				if (GetKeyState(VK_CAPITAL) & 0x1)
				{
					if (data[i]>='a' && data[i]<='z' || data[i]>='A' && data[i]<='Z')
					{
						shift = !shift;
					}
				}

				SendAscii(vk & 0xFF, shift);
			}
		}
		else //unicode字符
		{
			SendUnicode(data[i]);
		}
	}
}

void PressShiftAndTab()
{
	::keybd_event(VK_SHIFT, 0, 0, 0);
	::keybd_event(VK_TAB, 0, 0, 0);
	::keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
}


void PressKey(char key)
{
	::keybd_event(key, 0, 0, 0);
	::keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
}

void ClearPwd()
{
	::keybd_event(VK_HOME, 0, 0, 0);
	for (int i=0; i<16; i++)
	{
		::keybd_event(VK_DELETE, 0, 0, 0);
		::keybd_event(VK_DELETE, 0, KEYEVENTF_KEYUP, 0);
	}
}


void InputCode(CString strCode)
{
	for (int i=0; i<strCode.GetLength(); i++)
	{
		::keybd_event(strCode.GetAt(i), 0, 0, 0);
		::keybd_event(strCode.GetAt(i), 0, KEYEVENTF_KEYUP, 0);
	}	
}


void ReadQQAccount(CString strPath, vector_type &vQQ  )
{
	FILE *file = fopen(strPath, "r");
	if (NULL == file)
	{
		return ;
	}

	ifstream fin(strPath);  
	string   line;  
	while( getline(fin,line) )
	{    		
		int nIndex = line.find('|');
		if (-1 != nIndex)
		{
			string code = line.substr(0, nIndex);
			string pwd  = line.substr(nIndex+1);
			if (code.length() > 0 && pwd.length() > 0)
			{
				vQQ.push_back(spair__(code, pwd));
			}			
		}
	}
 
}

bool AotoLoginQQ(int nType, CString strPath, CString strCode, CString strPwd)
{
	if (1 == nType)
	{
		return	AutoLoginQQ2013(strPath, strCode, strPwd);
	}
	
	return	AutoLoginQQ2012(strPath, strCode, strPwd);
}

bool AutoLoginQQ2012(CString strPath, CString strCode, CString strPwd)
{
	if (strPath == "" )
	{
		return false;
	}

	ShellExecute(NULL, "open", strPath, NULL, NULL, true);
	Sleep(2000);

	HWND hWnd = FindWindow(NULL, "QQ2012");
	if (hWnd != NULL )
	{
		HWND hPassword = ::FindWindowEx(hWnd, 0, "Edit", NULL);
		if (NULL == hPassword)
		{
			return false;
		}

		::SetForegroundWindow(hPassword);	//设置前端
		PressShiftAndTab();					//按shift+tab，使焦点从密码框回到帐号输入框
		Sleep(50);
		SendKeys(strCode);					//输入帐号
		Sleep(50);
		PressKey(VK_TAB);					//按住TAB，让焦点定位到密码框
		::SetForegroundWindow(hPassword);	//设置前端
		Sleep(50);
		ClearPwd();							//清空密码输入框
		SendKeys(strPwd);					//输入密码
		PressKey(VK_RETURN);				//回车
		Sleep(3000);						//默认登录一个后3秒后再登录另外一个
		return true;
	}

	AfxMessageBox("QQ2012未启动");
	return false;
}

bool AutoLoginQQ2013(CString strPath, CString strCode, CString strPwd)
{
	if (strPath == "" )
	{
		return false;
	}

	ShellExecute(NULL, "open", strPath, NULL, NULL, true);
	Sleep(3000);

	HWND hWnd = FindWindow("TXGFLayerMask", NULL);
	if (NULL != hWnd)
	{
		hWnd = FindWindowEx(hWnd, NULL, "TXGuiFoundation", NULL);
		if (NULL != hWnd)
		{
			hWnd = FindWindowEx(hWnd, NULL, "Edit", NULL);
			if (NULL == hWnd)
			{
				return false;
			}
		}
	}

	if (NULL == hWnd)
	{
		AfxMessageBox("QQ2013未启动");
		return false;
	}

	::SetForegroundWindow(hWnd);	//设置前端
	for (int i=0; i<12; i++)
	{
		PressKey(VK_TAB);			//qq2013屏蔽掉了shift+tab切换到号码输入框，故使用连续12次TAB按键来切换到帐号输入框
		Sleep(100);
	}
	
	Sleep(50);
	SendKeys(strCode);					//输入帐号
	Sleep(50);
	PressKey(VK_TAB);					//按住TAB，让焦点定位到密码框
	Sleep(50);
	ClearPwd();							//清空密码输入框
	SendKeys(strPwd);					//输入密码
	Sleep(100);
	PressKey(VK_RETURN);				//回车
	return true;
}


void GetCurrDir(int nLength, char *lpBuffer)
{
	::GetModuleFileName(NULL, lpBuffer, nLength);
	CString strPath(lpBuffer);
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	memset(lpBuffer, 0, nLength);
	strcpy(lpBuffer, strPath);
}


void Fun_TerminateProcess(CString strProcessName)
{
	try
	{         
		HANDLE hSnapshot = NULL;

		//获得某一时刻系统的进程、堆（heap）、模块（module）或线程的快照信息
		hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot != NULL)
		{
			PROCESSENTRY32 processListStr;

			processListStr.dwSize = sizeof(PROCESSENTRY32);

			BOOL bReturnValue;

			bReturnValue = Process32First(hSnapshot, &processListStr);

			CString strExeFile = processListStr.szExeFile;

			DWORD dwProcessID = processListStr.th32ProcessID;

			int iThreadCnt = processListStr.cntThreads;

			while (bReturnValue)
			{    
				if (strExeFile.CompareNoCase(strProcessName) == 0)
				{                    
					//先获得该进程句柄，通过进程标识
					HANDLE hProcessHandle;                    
					hProcessHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessID);

					//利用数组ID指定的获得存在的一个进程的句柄
					if (hProcessHandle)
					{
						TerminateProcess(hProcessHandle, 0);
					}

					CloseHandle (hProcessHandle);

					//break;//是否全部查杀还是, 只杀第一个
				} 

				//获得系统进程链表中下一个进程的信息
				bReturnValue = Process32Next(hSnapshot,&processListStr);

				iThreadCnt   = processListStr.cntThreads;            
				dwProcessID  = processListStr.th32ProcessID;            
				strExeFile   = processListStr.szExeFile;        
			}        
			CloseHandle(hSnapshot);
		}
	}
	catch (...)
	{
	}
}