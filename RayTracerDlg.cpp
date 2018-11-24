
// RayTracerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RayTracer.h"
#include "RayTracerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRayTracerDlg �Ի���

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


// CRayTracerDlg ��Ϣ�������

BOOL CRayTracerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_ctrlDepth.SetRange(0, 10);
	m_ctrlDepth.SetPos(0);

	m_ctrlSize.SetRange(64, 512);
	m_ctrlSize.SetPos(64);

	m_dlgOpenGL.Create(64, 64, this);
	m_dlgOpenGL.ShowWindow(SW_HIDE);

	m_ctrlAcceleration.SetCheck(1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRayTracerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRayTracerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRayTracerDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CRayTracerDlg::OnCustomdrawSliderDepth(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_strDepth.Format(_T("%d"), m_ctrlDepth.GetPos());
	UpdateData(false);

	*pResult = 0;
}


void CRayTracerDlg::OnCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	m_strSize.Format(_T("%d"), m_ctrlSize.GetPos());
	UpdateData(false);

	*pResult = 0;
}


void CRayTracerDlg::OnBnClickedButtonRender()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	m_dlgOpenGL.StopRender();
	m_dlgOpenGL.InitScene(m_ctrlSize.GetPos(), m_ctrlDepth.GetPos() + 1, m_ctrlAcceleration.GetCheck(), m_ctrlAntialias.GetCheck());

	DWORD dwThreadID; 
	HANDLE  FThread=CreateThread(NULL, 0, CRenderDlg::Run, &m_dlgOpenGL, CREATE_SUSPENDED, &dwThreadID);
	ResumeThread(FThread);
}

void CRayTracerDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlgOpenGL.StopRender();
}
