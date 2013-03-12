// qq_auto_loginDlg.h : 头文件
//

#pragma once
#include <map>
#include <string>
using namespace std;
#include "ExpToolTip.h"

// Cqq_auto_loginDlg 对话框
class Cqq_auto_loginDlg : public CDialog
{
// 构造
public:
	Cqq_auto_loginDlg(CWnd* pParent = NULL);	// 标准构造函数
	~Cqq_auto_loginDlg();
	
// 对话框数据
	enum { IDD = IDD_QQ_AUTO_LOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int m_nLoginQQNum;
	int m_nPauseTime;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	int m_nCurLoginQQIndex;
public:
	afx_msg void OnTimer(UINT nIDEvent);
	vector_type m_mapQQ;

	CString m_strQQExePath;
	afx_msg void OnBnClickedButton3();
	CString m_strQQAccountPath;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();

private:
	CExpToolTip *m_pDlgTip; 
	BOOL m_bStartAutoMode;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nQQ2013;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
