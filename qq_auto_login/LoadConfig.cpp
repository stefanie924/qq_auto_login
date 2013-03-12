#include "StdAfx.h"
#include ".\loadconfig.h"
#include "Global.h"

CLoadConfig::CLoadConfig(void)
{
	char cBuf[MAX_PATH] = { NULL };
	GetCurrDir(MAX_PATH, cBuf); 
	m_strPath  = cBuf;
	m_strPath += "\\";
}

CLoadConfig::~CLoadConfig(void)
{
}

void CLoadConfig::GetString( const char *pAppName, const char *pKeyName, std::string &strValue, const char *pFileName )
{
	std::string strPath = m_strPath;
	strPath += pFileName;

	char pBuf[20480] = { NULL };
	::GetPrivateProfileString( pAppName ,pKeyName, strValue.c_str(), pBuf, 20480, strPath.c_str());
	strValue = pBuf;
}

void CLoadConfig::GetInt( const char *pAppName, const char *pKeyName, int &nValue, const char *pFileName )
{
	std::string strPath = m_strPath;
	strPath += pFileName;

	nValue = ::GetPrivateProfileInt( pAppName ,pKeyName, nValue, strPath.c_str());
}

void CLoadConfig::SetString( const char *pAppName, const char *pKeyName, std::string &strValue, const char *pFileName )
{
	std::string strPath = m_strPath;
	strPath += pFileName;

	::WritePrivateProfileString( pAppName ,pKeyName,  strValue.c_str(), strPath.c_str());
}

void CLoadConfig::SetInt( const char *pAppName, const char *pKeyName, int nValue, const char *pFileName )
{
	std::string strPath = m_strPath;
	strPath += pFileName;

	char pBuf[50] = {NULL};
	sprintf(pBuf, "%d", nValue );
	std::string strValue = pBuf;

	::WritePrivateProfileString( pAppName ,pKeyName, strValue.c_str(),  strPath.c_str());
}

CLoadConfig theConfig;


