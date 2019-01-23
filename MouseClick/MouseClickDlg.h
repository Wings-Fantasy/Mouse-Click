
// MouseClickDlg.h: 头文件
//

#pragma once


// CMouseClickDlg 对话框
class CMouseClickDlg : public CDialogEx
{
// 构造
public:
	CMouseClickDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSECLICK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedFixedMode();
	afx_msg void OnClickedRandomMode();
	afx_msg void OnClickedStart();
	afx_msg void OnClickedStop();
	afx_msg void OnChangeIntervaltime();
};
