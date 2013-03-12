#pragma once


// CExpToolTip

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ExpToolTip.h 
工具提示类             
--*/

class CExpToolTip : public CWnd
{
	DECLARE_DYNAMIC(CExpToolTip)

public:
	CExpToolTip();
	virtual ~CExpToolTip();

public:
	BOOL Create(CWnd* pParentWnd);                                     // 创建TIP窗体

	void RelayEvent(const CPoint ptMove, const CString strTitle, const CString strClient); // 延时显示
	void RelayEventForOrg(const CPoint ptMove, const CString strTitle, const CString strClient);

	// -------------------------------------------------------------------------------
	// 设置标题颜色
	inline void SetTitleColor(const COLORREF clrTitle) { m_clrTitleFontColor=clrTitle;}
	// 设置客户区颜色
	inline void SetClientColor(const COLORREF clrClient) { m_clrClient=clrClient;}  
	// 设置字体颜色
	inline void SetFontColor(const COLORREF clrTipFontColor) { m_clrTipFontColor=clrTipFontColor;} 
	// 设置提示文本
	inline void SetTipText(const CString strTitle,const CString strClient)
	{
		m_strTitle  = strTitle;
		m_strClient = strClient;
	}

	inline COLORREF GetTitleColor() const { return m_clrTitleFontColor;}        // 获得标题颜色
	inline COLORREF GetClientColor() const { return m_clrClient;}      // 获得客户区的颜色 
	inline COLORREF GetFontColor() const { return m_clrTipFontColor;}  // 获得字体颜色

	inline CSize    GetWindowSize() const { return m_SizeWindow; }     // 获得窗体尺寸

	// -------------------------------------------------------------------------------
public:

	// ---------------------------------------
	COLORREF   m_clrTitleFontColor;// 标题颜色
	COLORREF   m_clrClient;           // 客户区颜色
	COLORREF   m_clrTipFontColor;  // 字体颜色

	CString    m_strTitle;           // 标题文本
	CString    m_strClient;           // 客户文本

	CSize      m_SizeWindow;       // 窗体尺寸
	CPoint     m_Point;            // 点的坐标
	CPoint     m_oldPoint;         // 老的坐标
	UINT	   m_nOrgTimer;
	UINT       m_nIDTimer;         // 定时器
	UINT       m_nIDTimer2;        // 用于检测鼠标移动的定时器
	BOOL       m_bCapture;         // 是否捕获 

	CFont*     m_pTitleFont;       // 标题字体
	CFont*     m_pContentFont;     // 文本字体 

protected:                             
	// ---------------------------------------
	int  CalcWindowsSize(CSize& rzWindow) ; // 计算窗体尺寸
	// ---------------------------------------

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
};




