#pragma once


// CDlgNewCam dialog
#include "afxdialogex.h"

class CDlgNewCam : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewCam)

public:
	CDlgNewCam(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNewCam();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NEW_CAMERA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float pos_x;
	float pos_y;
	float pos_z;
	float lookat_x;
	float lookat_y;
	float lookat_z;
	float up_x;
	float up_y;
	float up_z;
};
