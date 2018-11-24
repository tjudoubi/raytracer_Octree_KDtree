
// RayTracerDlg.h : 头文件
//

#pragma once

#include "RenderDlg.h"

// CRayTracerDlg 对话框
class CRayTracerDlg : public CDialogEx
{
// 构造
public:
	CRayTracerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PARAMETERS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	CSliderCtrl m_ctrlDepth;
	CSliderCtrl m_ctrlSize;
	CString m_strDepth;
	CString m_strSize;
	CRenderDlg m_dlgOpenGL;

	// 生成的消息映射函数
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
