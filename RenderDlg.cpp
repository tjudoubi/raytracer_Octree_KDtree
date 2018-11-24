// RenderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RayTracer.h"
#include "RenderDlg.h"
#include "afxdialogex.h"


static char bitmapbuffer[sizeof( BITMAPINFO ) + 16];
static BITMAPINFO* bh;

// CRenderDlg 对话框


CRenderDlg::CRenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRenderDlg::IDD, pParent)
{
	m_pBuffer = 0;
	m_iSize = 0;
	m_iDepth = 0;
	m_pTracer = new Tracer();


	//----------------------------- your code here --------------------------------
	
	m_pCamera = new Camera(Point(6.0, 5.0, 12.0), Point(-1.0, 0.0, -1.0), Vector3D(0.0, 1.0, 0.0));
	m_pCamera->fovy = 40.0;
	m_pCamera->aspect = 1;

	m_pScene = new Scene();
	m_pScene->InitScene();

	//----------------------------- end of your code ------------------------------
}

CRenderDlg::~CRenderDlg()
{
	if(m_pBuffer)
		delete[] m_pBuffer;

	delete m_pScene;
	delete m_pCamera;
	delete m_pTracer;

}

void CRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRenderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CRenderDlg 消息处理程序
BOOL CRenderDlg::Create(int cx,int cy, CWnd* pParentWnd,bool FullScreen)
{
	BOOL re;
	re=CDialog::Create(IDD, pParentWnd);
	if(re)
	{
		CRect clientrect;
        GetClientRect(&clientrect);
        CRect windowrect;
        GetWindowRect(&windowrect);

        int width=cx+windowrect.Width()-clientrect.Width();
        int height=cy+windowrect.Height()-clientrect.Height();

        SetWindowPos(NULL,0,0,width,height,SWP_NOZORDER|SWP_NOMOVE);

	}
	return re;
}
BOOL CRenderDlg::DestroyWindow()
{
	return CDialog::DestroyWindow();
}
BOOL CRenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void CRenderDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	CDC dcMem;
	CBitmap bmp;
	dcMem.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	dcMem.SelectObject(&bmp);


	StretchDIBits(dcMem.GetSafeHdc(), 0, 0, m_iSize, m_iSize, 0, 0, m_iSize, m_iSize, m_pBuffer, bh, DIB_RGB_COLORS, SRCCOPY);


	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.DeleteDC();	// 删除DC
	bmp.DeleteObject();

	CDialog::OnPaint();
}

void CRenderDlg::InitScene(int size, int depth, bool acceleration, bool antialias)
{
	m_iSize = size;
	m_iDepth = depth;
	m_bAcceleration = acceleration;
	m_bAntialias = antialias;
	m_bStop = false;

	CRect clientrect;
    GetClientRect(&clientrect);
    CRect windowrect;
    GetWindowRect(&windowrect);

    int width = m_iSize + windowrect.Width() - clientrect.Width();
    int height = m_iSize + windowrect.Height() - clientrect.Height();

	SetWindowPos(NULL, 160, 90, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);

	if(m_pBuffer)
		delete[] m_pBuffer;

	m_pBuffer = new Pixel[m_iSize*m_iSize];
	memset(m_pBuffer, 0, sizeof(Pixel) * m_iSize * m_iSize);

	memset(bitmapbuffer, 0, sizeof(BITMAPINFOHEADER) + 16);
	bh = (BITMAPINFO *)&bitmapbuffer;
	bh->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bh->bmiHeader.biPlanes = 1;
	bh->bmiHeader.biBitCount = 32;
	bh->bmiHeader.biCompression = BI_BITFIELDS;
	bh->bmiHeader.biWidth = m_iSize, bh->bmiHeader.biHeight = -m_iSize;
	((unsigned long*)bh->bmiColors)[0] = 255 << 16;
	((unsigned long*)bh->bmiColors)[1] = 255 << 8;
	((unsigned long*)bh->bmiColors)[2] = 255;

	m_pTracer->SetTarget(m_pCamera, m_pScene, m_pBuffer, m_iSize, m_iSize, m_iDepth);

}

DWORD WINAPI CRenderDlg::Run(LPVOID pt)
{
	CRenderDlg* rDlg = (CRenderDlg*)pt;
	CString str;

	while (!rDlg->m_bStop && !rDlg->m_pTracer->RayCast(rDlg->m_bAntialias, rDlg->m_bAcceleration)) 
	{
		{
			MSG message;
			HACCEL haccel = 0;
			rDlg->Invalidate();
			rDlg->SendMessage(WM_PAINT, 0, 0 );
			while (PeekMessage( &message, rDlg->m_hWnd, 0, 0, PM_REMOVE ))
			{
				if (TranslateAccelerator( rDlg->m_hWnd, haccel, &message ) == 0)
				{
					TranslateMessage( &message );
					DispatchMessage( &message );
				}
			}
			Sleep( 0 );
		}

		str.Format(_T("%d%% Rendered image."), rDlg->m_pTracer->GetRLine() * 100 / rDlg->m_iSize);
		rDlg->SetWindowText(str);
	}

	rDlg->Invalidate();
	rDlg->SetWindowText(_T("Rendered image."));

	return 0;		
}

void CRenderDlg::StopRender()
{
	m_bStop = true;
}

void CRenderDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

}


void CRenderDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bStop = true;
	ShowWindow(SW_HIDE);
}


BOOL CRenderDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}

