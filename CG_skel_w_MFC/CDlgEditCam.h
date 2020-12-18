#pragma once


// CDlgEditCam dialog
#include "afxdialogex.h"

class CDlgEditCam : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditCam)

public:
	CDlgEditCam(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgEditCam();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT_ACTIVE_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float translate_x;
	float translate_y;
	float translate_z;
	float rotate_x;
	float rotate_y;
	float rotate_z;
	float left;
	float right;
	float bottom;
	float top;
	float z_near;
	float z_far;
	float fovy;
	float aspect;
	int rot_order_index;
	int proj_radio_index;
	CButton ortho_radio;
	CButton frustum_radio;
	CButton perspective_radio;
	CComboBox rot_order;
	CEdit left_editbox;
	CEdit right_editbox;
	CEdit bottom_editbox;
	CEdit top_editbox;
	CEdit z_near_editbox;
	CEdit z_far_editbox;
	CEdit fovy_editbox;
	CEdit aspect_editbox;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioOrtho();
	afx_msg void OnBnClickedRadioFrustum();
	afx_msg void OnBnClickedRadioPerspective();
	afx_msg void OnCbnSelchangeComboOrder();
	void initOrderCombo();
};
