#pragma once
#include <string>
using namespace std;
//º«¬º≈‰÷√–≈œ¢(add by lxm 07-11-14)
class CLoadConfig
{
public:
	CLoadConfig(void);
	~CLoadConfig(void);

public:
	void	GetString( const char *pAppName, const char *pKeyName, std::string &strValue, const char *pFileName );
	void	SetString( const char *pAppName, const char *pKeyName, std::string &strValue, const char *pFileName );
	void	GetInt( const char *pAppName, const char *pKeyName, int &nValue, const char *pFileName );
	void	SetInt( const char *pAppName, const char *pKeyName, int nValue, const char *pFileName );
	
private:
	std::string m_strPath;
};


extern CLoadConfig theConfig;