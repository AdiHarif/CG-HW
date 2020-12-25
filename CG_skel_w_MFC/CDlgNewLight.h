#pragma once


// CDlgNewLight dialog
#include "afxdialogex.h"

class CDlgNewLight : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewLight)

public:
	CDlgNewLight(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNewLight();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEW_LIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	float color_r;
	float color_g;
	float color_b;
	float dir_x;
	float dir_y;
	float dir_z;
	float pos_x;
	float pos_y;
	float pos_z;
	int type_radio_index;
	CButton parallel_radio;
	CButton point_radio;
	CEdit pos_x_editbox;
	CEdit pos_y_editbox;
	CEdit pos_z_editbox;
	CEdit dir_x_editbox;
	CEdit dir_y_editbox;
	CEdit dir_z_editbox;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioParallel();
	afx_msg void OnBnClickedRadioPoint();
};
