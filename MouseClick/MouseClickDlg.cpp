// MouseClickDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "MouseClick.h"
#include "MouseClickDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMouseClickDlg 对话框
CMouseClickDlg::CMouseClickDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(MainFrom, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMouseClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMouseClickDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(FixedMode, &CMouseClickDlg::OnClickedFixedMode)
	ON_BN_CLICKED(RandomMode, &CMouseClickDlg::OnClickedRandomMode)
	ON_BN_CLICKED(Start, &CMouseClickDlg::OnClickedStart)
	ON_BN_CLICKED(Stop, &CMouseClickDlg::OnClickedStop)
	ON_EN_CHANGE(IntervalTime, &CMouseClickDlg::OnChangeIntervaltime)
END_MESSAGE_MAP()

// CMouseClickDlg 消息处理程序
BOOL CMouseClickDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	((CComboBox*)GetDlgItem(Mouse))->SetCurSel(0);
	((CButton*)GetDlgItem(FixedMode))->SetCheck(TRUE);
	GetDlgItem(IntervalTime)->SetWindowTextW(_T("10.0"));
	((CComboBox*)GetDlgItem(HotKey))->SetCurSel(0);
	GetDlgItem(HotKey)->EnableWindow(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CMouseClickDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMouseClickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL Char(char msg, CString number);

BOOL CMouseClickDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC键
			return true;
		case VK_RETURN://Enter键
			return true;
		}
	}
	else if ((GetFocus() == (CEdit*)GetDlgItem(IntervalTime)) && (pMsg->message == WM_CHAR))
	{
		CString number;
		GetDlgItem(IntervalTime)->GetWindowTextW(number);
		if (!Char(pMsg->wParam, number)) return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL Char(char msg, CString number)
{
	//处理输入小数点
	if (msg == '.')
	{
		if (number.Find('.') == -1) return true;
		else return false;
	}
	//处理数字、Backspace和Delete
	else if ((msg >= '0' && msg <= '9') || (msg == 0x08) || (msg == 0x10)) return true;
	else return false;
}

void CMouseClickDlg::OnClickedFixedMode()
{
	GetDlgItem(IntervalTimeTip)->SetWindowTextW(_T("间隔时间："));
}

void CMouseClickDlg::OnClickedRandomMode()
{
	GetDlgItem(IntervalTimeTip)->SetWindowTextW(_T("最大间隔："));
}

void CMouseClickDlg::OnChangeIntervaltime()
{
	UpdateData(true);
	CString timeString;
	GetDlgItem(IntervalTime)->GetWindowTextW(timeString);
	timeString.ReleaseBuffer();
	if (_ttof(timeString) > 60.0)
	{
		GetDlgItem(IntervalTime)->SetWindowTextW(_T("60"));
	}
}

typedef struct MOUSE
{
	bool flag = true;
	int direction = 0;
	bool isRandomDelay = false;
	int delay = 10000;
}MOUSE;

DWORD WINAPI MouseClickThread(LPVOID lpParam);
LPDWORD mouseClickThreadID = NULL;
MOUSE mouse;

void CMouseClickDlg::OnClickedStart()
{
	GetDlgItem(Mouse)->EnableWindow(false);
	GetDlgItem(FixedMode)->EnableWindow(false);
	GetDlgItem(RandomMode)->EnableWindow(false);
	GetDlgItem(IntervalTime)->EnableWindow(false);
	GetDlgItem(HotKey)->EnableWindow(false);
	GetDlgItem(Start)->EnableWindow(false);
	GetDlgItem(Stop)->EnableWindow(true);

	CString delayString;
	GetDlgItem(IntervalTime)->GetWindowTextW(delayString);
	delayString.ReleaseBuffer();

	mouse.flag = true;
	mouse.direction = ((CComboBox*)GetDlgItem(Mouse))->GetCurSel();
	if (((CButton*)GetDlgItem(FixedMode))->GetCheck() == 1) mouse.isRandomDelay = false;
	else mouse.isRandomDelay = true;
	mouse.delay = (int)(_ttof(delayString) * 1000);

	CreateThread(NULL, 0, &MouseClickThread, &mouse, 0, mouseClickThreadID);
}

void CMouseClickDlg::OnClickedStop()
{
	GetDlgItem(Mouse)->EnableWindow(true);
	GetDlgItem(FixedMode)->EnableWindow(true);
	GetDlgItem(RandomMode)->EnableWindow(true);
	GetDlgItem(IntervalTime)->EnableWindow(true);
	//GetDlgItem(HotKey)->EnableWindow(true);
	GetDlgItem(Start)->EnableWindow(true);
	GetDlgItem(Stop)->EnableWindow(false);

	mouse.flag = false;
	ResumeThread(mouseClickThreadID);
}

DWORD WINAPI MouseClickThread(LPVOID lpParam)
{
	MOUSE *mouse = (MOUSE *)lpParam;

	while (mouse->flag)
	{
		switch (mouse->direction)
		{
		case 0://左键点击
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			break;
		case 1://中键点击
			mouse_event(MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
			break;
		case 2://右键点击
			mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			break;
		}
		if (mouse->isRandomDelay)
		{
			srand((unsigned)time(NULL));
			Sleep(1 + rand() % mouse->delay);
		}
		else Sleep(mouse->delay);
	}
	return 0;
}
