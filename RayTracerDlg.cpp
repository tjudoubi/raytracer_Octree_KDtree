
// RayTracerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRayTracerDlg 对话框

CRayTracerDlg::CRayTracerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRayTracerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strDepth = _T("0");
	m_strSize = _T("64");
}

void CRayTracerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_DEPTH, m_ctrlDepth);
	DDX_Control(pDX, IDC_SLIDER_SIZE, m_ctrlSize);
	DDX_Text(pDX, IDC_LABEL_DEPTH, m_strDepth);
	DDX_Text(pDX, IDC_LABEL_SIZE, m_strSize);
	DDX_Control(pDX, IDC_CHECK_ACCELERATION, m_ctrlAcceleration);
	DDX_Control(pDX, IDC_CHECK_ANTIALIAS, m_ctrlAntialias);
	DDX_Control(pDX, IDC_BUTTON_RENDER, m_btnRender);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
}

BEGIN_MESSAGE_MAP(CRayTracerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_DEPTH, &CRayTracerDlg::OnCustomdrawSliderDepth)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SIZE, &CRayTracerDlg::OnCustomdrawSliderSize)
	ON_BN_CLICKED(IDC_BUTTON_RENDER, &CRayTracerDlg::OnBnClickedButtonRender)
ON_BN_CLICKED(IDC_BUTTON_STOP, &CRayTracerDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CRayTracerDlg 消息处理程序

BOOL CRayTracerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_ctrlDepth.SetRange(0, 10);
	m_ctrlDepth.SetPos(0);

	m_ctrlSize.SetRange(64, 512);
	m_ctrlSize.SetPos(64);

	m_dlgOpenGL.Create(64, 64, this);
	m_dlgOpenGL.ShowWindow(SW_HIDE);

	m_ctrlAcceleration.SetCheck(1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRayTracerDlg::OnPaint()
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
HCURSOR CRayTracerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRayTracerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CRayTracerDlg::OnCustomdrawSliderDepth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_strDepth.Format(_T("%d"), m_ctrlDepth.GetPos());
	UpdateData(false);

	*pResult = 0;
}


void CRayTracerDlg::OnCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_strSize.Format(_T("%d"), m_ctrlSize.GetPos());
	UpdateData(false);

	*pResult = 0;
}


void CRayTracerDlg::OnBnClickedButtonRender()
{
	// TODO: 在此添加控件通知处理程序代码

	m_dlgOpenGL.StopRender();
	m_dlgOpenGL.InitScene(m_ctrlSize.GetPos(), m_ctrlDepth.GetPos() + 1, m_ctrlAcceleration.GetCheck(), m_ctrlAntialias.GetCheck());

	DWORD dwThreadID; 
	HANDLE  FThread=CreateThread(NULL, 0, CRenderDlg::Run, &m_dlgOpenGL, CREATE_SUSPENDED, &dwThreadID);
	ResumeThread(FThread);
}

void CRayTracerDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlgOpenGL.StopRender();
}
