
// RayTracerDlg.h : ͷ�ļ�
//

#pragma once

#include "RenderDlg.h"

// CRayTracerDlg �Ի���
class CRayTracerDlg : public CDialogEx
{
// ����
public:
	CRayTracerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PARAMETERS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	CSliderCtrl m_ctrlDepth;
	CSliderCtrl m_ctrlSize;
	CString m_strDepth;
	CString m_strSize;
	CRenderDlg m_dlgOpenGL;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	
	afx_msg void OnCustomdrawSliderDepth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawSliderSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRender();
	CButton m_ctrlAcceleration;
	CButton m_ctrlAntialias;
	CButton m_btnRender;
	CButton m_btnStop;
	afx_msg void OnBnClickedButtonStop();
};
