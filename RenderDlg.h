#pragma once

#include "Tracer.h"
// CRenderDlg �Ի���

class CRenderDlg : public CDialogEx
{

public:
	CRenderDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenderDlg();

// �Ի�������
	enum { IDD = IDD_RENDER_DIALOG };

	virtual BOOL Create(int cx,int cy, CWnd* pParentWnd = NULL,bool FullScreen = false);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	void StopRender();
	void InitScene(int size, int depth, bool acceleration, bool antialias);
	static DWORD WINAPI Run(LPVOID rDlg);
private:
	Pixel* m_pBuffer;
	Tracer* m_pTracer;
	Camera* m_pCamera;
	Scene* m_pScene;
	int m_iSize;
	int m_iDepth;
	bool m_bAcceleration;
	bool m_bAntialias;
	bool m_bStop;
	virtual void OnOK();
	virtual void OnCancel();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
