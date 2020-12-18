#pragma once


// CDlgTransformModel dialog
#include "afxdialogex.h"

class CDlgTransformModel : public CDialog
{
	DECLARE_DYNAMIC(CDlgTransformModel)

public:
	CDlgTransformModel(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgTransformModel();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRANSFORM_ACTIVE_MODEL };
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
	float scale_x;
	float scale_y;
	float scale_z;
	int rot_order_index;
	CComboBox rot_order;

	afx_msg void OnCbnSelchangeComboOrder();
	void initOrderCombo();
};
