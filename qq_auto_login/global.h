#include <vector>  
#include <string>
using namespace std;

typedef   pair   <std::string,   std::string>   spair__;   
typedef   vector<   spair__   >   vector_type;   

void SendAscii(wchar_t data, BOOL shift);
void SendUnicode(wchar_t data);
void SendKeys(CString msg);
void PressShiftAndTab();
void PressKey(char key);
void ClearPwd();
void InputCode(CString strCode);
void ReadQQAccount(CString strPath,vector_type &vQQ );

bool AotoLoginQQ(int nType, CString strPath, CString strCode, CString strPwd);
bool AutoLoginQQ2012(CString strPath, CString strCode, CString strPwd);
bool AutoLoginQQ2013(CString strPath, CString strCode, CString strPwd);
void GetCurrDir(int nLength, char *lpBuffer);
void Fun_TerminateProcess(CString strProcessName); 