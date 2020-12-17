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
	float up;
	float z_near;
	float z_far;
	float fovy;
	float aspect;
	CButton ortho_radio;
	CButton frustum_radio;
	CButton perspective_radio;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioOrtho();
	afx_msg void OnBnClickedRadioFrustum();
	afx_msg void OnBnClickedRadioPerspective();
	CComboBox rot_order;
	void initOrderCombo();
};
