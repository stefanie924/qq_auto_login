/***********************************************************************************

单元名称：  工具提示   
模块功能：  延时半秒显示提示

类 名 称：  CExpToolTip 
包含文件：  ExpToolTip.h,ExpToolTip.cpp

开发时间：  2002-10-15
结束时间：  2002-10-22
开发人员：  汪飞
----------------------------------------------------------------------------------
[接口]:
BOOL Create(CWnd* pParentWnd)
[功能]: 创建提示窗体,必须调用

[参数]: 
CWnd* pParentWnd ---- 为父窗体，必须指定

[返回]: 
TRUE             ---- 创建成功
FALSE            ---- 否则失败

--------------------------------------------------------------------

void RelayEvent(CPoint ptMove, CString strTitle, CString strClient)
[功能]: 延时显示文本

[参数]: 
CPoint ptMove     ----- 显示提示窗体的坐标
CString strTitle  ----- 标题文本 
CString strClient ----- 客户区文本,以\n换行，最后一行不加

[返回]: 无

----------------------------------------------------------------------------------
[说明]:

先必须构造一个类的实例，实例最好作为成员变量，在初始化的时候调用
Create(this)创建工具提示,此函数保证当前实例未被撤消时，只创建
一次，多次调用将被忽略。最后在合适的地方调用RelayEvent显示提示

***********************************************************************************/

// ExpToolTip.cpp : 实现文件
//

#include "stdafx.h"
#include "qq_auto_login.h"
#include "ExpToolTip.h"

// CExpToolTip

IMPLEMENT_DYNAMIC(CExpToolTip, CWnd)

CExpToolTip::CExpToolTip() :
m_clrTitleFontColor(RGB(49,106,197)),
m_clrClient(RGB(255,255,228)),
m_clrTipFontColor(RGB(0,0,0)),
m_strTitle(_T("")),
m_strClient(_T("")),
m_nIDTimer(0),
m_SizeWindow(0,0),
m_Point(0,0),
m_oldPoint(0,0),
m_bCapture(TRUE),
m_pTitleFont(NULL),
m_pContentFont(NULL),
m_nIDTimer2(2),
m_nOrgTimer(100)
{

}

CExpToolTip::~CExpToolTip()
{
	// 撤消窗体    
	if (::IsWindow(m_hWnd))
		DestroyWindow(); 
	if(m_pTitleFont)
	{
		m_pTitleFont->DeleteObject();
		delete m_pTitleFont;
	}
	if (m_pContentFont)  
	{
		m_pContentFont->DeleteObject();
		delete m_pContentFont;
	}
}


BEGIN_MESSAGE_MAP(CExpToolTip, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CExpToolTip 消息处理程序

////////////////////////////////////////////////////////////////////////////////////
BOOL CExpToolTip::Create(CWnd *pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	if(!pParentWnd || ::IsWindow(m_hWnd))
	{
		return FALSE; // 如果实例已经存在，那么不创建
	}

	//  注册窗口类
	LPCTSTR lpszTipClass = AfxRegisterWndClass(CS_CLASSDC | CS_SAVEBITS, // 缺省样式 
		::LoadCursor(NULL, IDC_ARROW),
		::CreateSolidBrush(m_clrClient),// 背景色
		NULL);
	
	//  创建TIP窗体
	BOOL bRet=CWnd::CreateEx(WS_EX_TOOLWINDOW ,  // 工具窗口样式
		lpszTipClass,       // 窗口类
		NULL,
		WS_BORDER | WS_POPUP ,// 边框和弹出样式 
		CW_USEDEFAULT,         // 用户缺省值 
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		CW_USEDEFAULT,
		pParentWnd->GetSafeHwnd(), 
		NULL, 
		NULL);
	if(bRet)
	{
		SetOwner(pParentWnd);     // 设置父窗口     
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////
int CExpToolTip::CalcWindowsSize(CSize &rzWindow) 
{
	TEXTMETRIC tm;
	int        nPos       = 0;
	int        nMax       = 0;
	int        nLineCount = 0;
	CString    strTemp;

	// 标题为空，则窗体长度为空
	if(m_strTitle.IsEmpty())
	{
		rzWindow.cx = 0;
		rzWindow.cy = 0;
		return 0;
	}

	CDC* pDC=GetDC();

	// 保证字体的长宽
	CFont* pOldFont = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
	pDC->GetTextMetrics(&tm);  

	//
	//  求一行最长字符串
	//
	for(int i = 0;i < m_strClient.GetLength();i++)
	{
		if(m_strClient.GetAt(i)=='\n')
		{
			strTemp = m_strClient.Mid(nPos,i-nPos);
			nPos    = i + 1;

			nMax    = nMax < strTemp.GetLength() ? strTemp.GetLength() : nMax;
			nLineCount++;
		}
		else if(i == (m_strClient.GetLength()-1)) // 如果是最后一行
		{
			strTemp = m_strClient.Right(i-nPos+1);

			nMax    = nMax < strTemp.GetLength() ? strTemp.GetLength() : nMax;
			nLineCount++;
		}
	}

	nMax = nMax < m_strTitle.GetLength() ?  m_strTitle.GetLength() : nMax; // 标题的长度
	nLineCount++;

	//
	// 窗体尺寸
	//
	rzWindow.cx = nMax * tm.tmMaxCharWidth / 2 + 5;// 5 为最后预留的距离 
	rzWindow.cy = nLineCount * (tm.tmHeight+tm.tmExternalLeading + 2); // 2为行距

	pDC->SelectObject(pOldFont);

	ReleaseDC(pDC);
	return (tm.tmHeight + tm.tmExternalLeading + 2); // 字符高度+行距
}

///////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::OnPaint()
{
	CPaintDC dc(this); 

	int      nPos       = 0;
	int      nLineCount = 0;
	CRgn     rgn;
	CRect    rtTitle;
	CRect    rtDlg;    
	CSize    rzWindow;
	//    CBrush   bhFrame(::GetSysColor(COLOR_3DDKSHADOW));
	CString  strTemp;

	CFont*   pOldFont=NULL;

	if (NULL == m_pContentFont)
	{
		m_pContentFont = new CFont;
		if (!m_pContentFont->CreateStockObject(DEFAULT_GUI_FONT)) // 取系统字体
			return ;
	}

	if (NULL == m_pTitleFont)
	{
		m_pTitleFont = new CFont;
		LOGFONT        LogFont;
		m_pContentFont->GetLogFont(&LogFont);
		//LogFont.lfWeight = FW_BOLD;
		if (!m_pTitleFont->CreateFontIndirect(&LogFont)) // 创建标题字体
			return ;
	}
	dc.SetBkMode(TRANSPARENT); 

	// 计算窗体尺寸，并返回字符高度
	int  nTitleHeight = CalcWindowsSize(m_SizeWindow);
	if(nTitleHeight == 0)
		return ;    

	//  画标题
	rtTitle.SetRect(0, 0, m_SizeWindow.cx, nTitleHeight);
	pOldFont = dc.SelectObject(m_pTitleFont);
	//dc.SetTextColor(m_clrTitleFontColor);
	dc.SetTextColor(RGB(255,255,255));

	CBrush brush(m_clrTitleFontColor);
	dc.FillRect(&rtTitle,&brush);
	rtTitle.top+=2;

	dc.DrawText(m_strTitle, 
		&rtTitle, 
		DT_CENTER |
		DT_NOPREFIX  |
		DT_EXPANDTABS |  
		DT_SINGLELINE);

	//  客户区输出文本
	dc.SelectObject(m_pContentFont);
	dc.SetTextColor(m_clrTipFontColor);   // 客户区的文本是黑色
	for(int i = 0; i < m_strClient.GetLength();i++)
	{
		if(m_strClient.GetAt(i) =='\n')
		{
			strTemp = m_strClient.Mid(nPos, i-nPos);
			nPos    = i + 1;
			nLineCount++;

			if(nLineCount == 1) // 第一行文本,加上2的行距不于中间的线重合
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
			else
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
		}      
		else if(i == (m_strClient.GetLength() - 1)) // 最后一行文本
		{
			strTemp = m_strClient.Right(i - nPos + 1);
			nLineCount++;

			if(nLineCount == 1) // 只有一行文本
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
			else
			{
				dc.TextOut(1, (nTitleHeight * nLineCount + 2), strTemp);
			}
		}
	}    
	brush.DeleteObject();
	rgn.DeleteObject();

	dc.SelectObject(pOldFont)->DeleteObject();
	// 圆角窗体    
	//GetWindowRect(&rtDlg);
	//rgn.CreateRoundRectRgn(0,0,rtDlg.Width(),rtDlg.Height(),10,10);
	//dc.FrameRgn(&rgn,&bhFrame,1,1);
	//SetWindowRgn((HRGN)rgn,TRUE);    
}

/////////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::RelayEvent(const CPoint ptMove, const CString strTitle, const CString strClient)
{ 
	SetTipText(strTitle, strClient);  // 设置标题
	CalcWindowsSize(m_SizeWindow); // 得到窗体尺寸

	// 两次鼠标位子相同，则忽略
	if(ptMove == m_oldPoint)
		return;
	m_oldPoint = ptMove;  // 记录老的鼠标坐标

	m_Point    = ptMove;   //  鼠标点的偏移
	m_Point.x += 15;
	m_Point.y += 10;

	if(IsWindowVisible())
	{        
		ShowWindow(SW_HIDE);         // 如果它是显示的，那么隐藏
	}

	if(m_nIDTimer != 0)                //   如果上次存在时间，那么结束掉从新开始 
	{
		KillTimer(m_nIDTimer);
	}    
	m_nIDTimer = SetTimer(1, 500, NULL); // 时间间隔为半秒
}

void CExpToolTip::RelayEventForOrg(const CPoint ptMove, const CString strTitle, const CString strClient){
	SetTipText(strTitle, strClient);  // 设置标题
	CalcWindowsSize(m_SizeWindow); // 得到窗体尺寸

	// 两次鼠标位子相同，则忽略
	if(ptMove == m_oldPoint)
		return;
	m_oldPoint = ptMove;  // 记录老的鼠标坐标

	m_Point    = ptMove;   //  鼠标点的偏移
	m_Point.x -= 300;
	m_Point.y += 10;

	if(IsWindowVisible())
	{        
		ShowWindow(SW_HIDE);         // 如果它是显示的，那么隐藏
	}

	if(m_nIDTimer != 0)                //   如果上次存在时间，那么结束掉从新开始 
	{
		KillTimer(m_nIDTimer);
	}    
	m_nIDTimer = SetTimer(1, 500, NULL); // 时间间隔为半秒
}


/////////////////////////////////////////////////////////////////////////////////////
void CExpToolTip::OnTimer(UINT nIDEvent)
{    
	CPoint point;
	::GetCursorPos(&point);
	if(nIDEvent == m_nIDTimer && point==m_oldPoint)
	{  
		KillTimer(m_nIDTimer);        // 结束时间
		m_nIDTimer2 = SetTimer(2, 5, NULL); //用于检测鼠标移动的定时器
		if(!m_strTitle.IsEmpty())
		{          
			SetWindowPos(&this->wndTop,  // 找到关键字则显示窗体
				m_Point.x,
				m_Point.y,
				m_SizeWindow.cx,
				m_SizeWindow.cy,
				SWP_SHOWWINDOW | 
				SWP_NOACTIVATE);
		}
		return;  
	}
	if(nIDEvent == m_nIDTimer2 && point!=m_oldPoint)
	{  
		KillTimer(m_nIDTimer2);        // 结束时间
		if(IsWindowVisible())
		{        
			ShowWindow(SW_HIDE);      // 如果它是显示的，那么隐藏
		}
		return;  
	}
	if(nIDEvent == m_nOrgTimer && point!=m_oldPoint){
		KillTimer(m_nOrgTimer);        // 结束时间
		m_nIDTimer2 = SetTimer(2, 5, NULL); //用于检测鼠标移动的定时器
		if(!m_strTitle.IsEmpty())
		{          
			SetWindowPos(&this->wndTop,  // 找到关键字则显示窗体
				m_Point.x - 100,
				m_Point.y,
				m_SizeWindow.cx,
				m_SizeWindow.cy,
				SWP_SHOWWINDOW | 
				SWP_NOACTIVATE);
		}
		return;  
	}
	CWnd::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////////////////////////

