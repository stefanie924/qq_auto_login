// qq_auto_loginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "qq_auto_login.h"
#include "qq_auto_loginDlg.h"
#include ".\qq_auto_logindlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cqq_auto_loginDlg 对话框

Cqq_auto_loginDlg::Cqq_auto_loginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cqq_auto_loginDlg::IDD, pParent)
	, m_nLoginQQNum(5)
	, m_nCurLoginQQIndex(0)
	, m_nPauseTime(10)
	, m_bStartAutoMode(TRUE)
	, m_pDlgTip(NULL)
	, m_nQQ2013(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	string qq_exe_path, qq_account_path;
	theConfig.GetString("PATH", "QQ_EXE_PATH", qq_exe_path, "config.ini");
	theConfig.GetString("PATH", "QQ_ACCOUNT_PATH", qq_account_path, "config.ini");
	theConfig.GetInt("QQ_NUM", "NUM", m_nLoginQQNum, "config.ini");
	theConfig.GetInt("TIMER",  "ELAPSE", m_nPauseTime, "config.ini");
	theConfig.GetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
	theConfig.GetInt("QQ_NUM", "QQ2013", m_nQQ2013, "config.ini");

	m_strQQExePath = qq_exe_path.c_str();
	m_strQQAccountPath = qq_account_path.c_str();
}

Cqq_auto_loginDlg::~Cqq_auto_loginDlg()
{
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");

	if (NULL != m_pDlgTip)
	{
		delete m_pDlgTip;
	}
};

void Cqq_auto_loginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nLoginQQNum);
	DDV_MinMaxInt(pDX, m_nLoginQQNum, 1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_nPauseTime);
	DDV_MinMaxInt(pDX, m_nPauseTime, 1, INT_MAX);
	DDX_Text(pDX, IDC_EDIT3, m_strQQExePath);
	DDX_Text(pDX, IDC_EDIT4, m_strQQAccountPath);
}

BEGIN_MESSAGE_MAP(Cqq_auto_loginDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
END_MESSAGE_MAP()


// Cqq_auto_loginDlg 消息处理程序

BOOL Cqq_auto_loginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	m_pDlgTip=new CExpToolTip;
	m_pDlgTip->SetTitleColor(RGB(128,128,255));
	m_pDlgTip->Create(this);//创建信息提示框
		
	if (1 == m_nQQ2013)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void Cqq_auto_loginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cqq_auto_loginDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR Cqq_auto_loginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//手工模式
void Cqq_auto_loginDlg::OnBnClickedButton1()	
{
	//先结束当前所有QQ进程
	Fun_TerminateProcess("qq.exe");

	UpdateData();
	if (m_strQQAccountPath == "" || m_strQQExePath == "")
	{
		MessageBox("请先设置QQ2012主程序路径以及QQ帐号密码所在路径");
		return ;
	}

	//读取QQ帐号
	ReadQQAccount(m_strQQAccountPath,  m_mapQQ);
	if (m_mapQQ.size() <= 0)
	{
		MessageBox("读取不到QQ帐号与密码（可能格式不对，格式以QQ号码和密码以|号分隔，如下： 123456|11111");
		return ;
	}
	
	//如果文件里的QQ帐号小于设置要运行的QQ数目，则设置要运行的QQ数目等于QQ帐号数
	if (m_nLoginQQNum > m_mapQQ.size())
	{
		m_nLoginQQNum = m_mapQQ.size();
		UpdateData(FALSE);
	}

	//先定位到扫描到的QQ号码
	if (m_nCurLoginQQIndex >= m_mapQQ.size())
	{
		m_nCurLoginQQIndex = 0;			
	}
	vector_type::iterator it = m_mapQQ.begin();
	for (int i=0; i<m_nCurLoginQQIndex; i++, it++);
	
	//运行指定个数目的QQ
	int nPreLoginQQNum = 0;
	while (it != m_mapQQ.end())
	{
		if (nPreLoginQQNum < m_nLoginQQNum )
		{
			AotoLoginQQ(m_nQQ2013,  m_strQQExePath, it->first.c_str(), it->second.c_str());
		}
		else
		{
			break;
		}
		m_nCurLoginQQIndex++;
		nPreLoginQQNum++;
		it++;
	}
	
	//保存配置 
	theConfig.SetInt("QQ_NUM", "NUM", m_nLoginQQNum, "config.ini");
	theConfig.SetInt("TIMER",  "ELAPSE", m_nPauseTime, "config.ini");
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");	
	theConfig.SetInt("QQ_NUM", "QQ2013", m_nQQ2013, "config.ini");
}

HBRUSH Cqq_auto_loginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIP)
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void Cqq_auto_loginDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1000)
	{
		//结束所有QQ进程
		Fun_TerminateProcess("qq.exe");

		//先定位到扫描到的QQ号码
		if (m_nCurLoginQQIndex >= m_mapQQ.size())
		{
			m_nCurLoginQQIndex = 0;			
		}
		vector_type::iterator it = m_mapQQ.begin();
		for (int i=0; i<m_nCurLoginQQIndex; i++, it++);
		
		int nPreLoginQQNum = 0;
		while (it != m_mapQQ.end())
		{
			if (nPreLoginQQNum < m_nLoginQQNum )
			{
				AotoLoginQQ(m_nQQ2013,  m_strQQExePath, it->first.c_str(), it->second.c_str());
			}
			else
			{
				break;
			}
			m_nCurLoginQQIndex++;
			nPreLoginQQNum++;
			it++;
			if (m_nCurLoginQQIndex >= m_mapQQ.size())
			{
				m_nCurLoginQQIndex = 0;
				it = m_mapQQ.begin();
			}
		}

		theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
	}
	CDialog::OnTimer(nIDEvent);
}

void Cqq_auto_loginDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, "exe", "QQ.exe",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,"可执行文件(*.exe)|*.exe||", this);
 	if (dlg.DoModal() == IDOK)
	{
		m_strQQExePath = dlg.GetPathName();

		theConfig.SetString("PATH", "QQ_EXE_PATH", string(m_strQQExePath), "config.ini");
		UpdateData(FALSE);
	}
}

void Cqq_auto_loginDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, "exe", "QQ.txt",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,"文本文件(*.txt)|*.txt||", this);
	if (dlg.DoModal() == IDOK)
	{
		m_strQQAccountPath = dlg.GetPathName();
		theConfig.SetString("PATH", "QQ_ACCOUNT_PATH", string(m_strQQAccountPath), "config.ini");
		UpdateData(FALSE);
	}
}

void Cqq_auto_loginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bStartAutoMode)
	{		
		OnBnClickedButton1();
		SetTimer(1000, 1000*60*m_nPauseTime, NULL);
		SetDlgItemText(IDC_BUTTON2, "停止自动模式");
	}
	else
	{
		KillTimer(1000);
		SetDlgItemText(IDC_BUTTON2, "启动自动模式");		
	}

	m_bStartAutoMode = !m_bStartAutoMode;
}

void Cqq_auto_loginDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	//结束所有QQ进程
	Fun_TerminateProcess("qq.exe");
}

void Cqq_auto_loginDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurLoginQQIndex = 0;
	theConfig.SetInt("QQ_NUM", "CUR_INDEX", m_nCurLoginQQIndex, "config.ini");
}

BOOL Cqq_auto_loginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	CRect rc1, rc2, rc3, rc4;
	GetDlgItem(IDC_BUTTON1)->GetWindowRect(rc1);
	GetDlgItem(IDC_BUTTON2)->GetWindowRect(rc2);
	GetDlgItem(IDC_BUTTON5)->GetWindowRect(rc3);
	GetDlgItem(IDC_BUTTON6)->GetWindowRect(rc4);

	ScreenToClient(rc1);
	ScreenToClient(rc2);
	ScreenToClient(rc3);
	ScreenToClient(rc4);
	CString strManalMode, strAutoMode, strCloseAllQQ, strClearQQMemory;
	strManalMode.Format("结束当前电脑运行的所有QQ进程，然后按照已记忆的顺序重新启动%d个", m_nLoginQQNum);
	strAutoMode.Format("结束当前电脑运行的所有QQ进程，然后按照已记忆的顺序重新启动%d个\n再暂停%d分钟，然后结束当前所有QQ进程，再启动，如此循环，直到点击停止自动模式", m_nLoginQQNum, m_nPauseTime);
	strCloseAllQQ.Format("结束当前电脑上运行的所有QQ进程");
	strClearQQMemory.Format("点此按钮后，下次登录的QQ从设置的文件中的第一个重新开始");
	
	if (WM_MOUSEMOVE == pMsg->message)
	{
		CPoint point(pMsg->pt);
		ScreenToClient(&point);
		//判断其位置在范围内,并且不在标头上
		if (rc1.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "原理", strManalMode );
		}

		if (rc2.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "原理", strAutoMode );
		}

		if (rc3.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "原理", strCloseAllQQ);
		}

		if (rc4.PtInRect(point) )
		{
			m_pDlgTip->RelayEvent(pMsg->pt, "原理", strClearQQMemory );
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void Cqq_auto_loginDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_nQQ2013 = 1;
}

void Cqq_auto_loginDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_nQQ2013 = 0;
}
