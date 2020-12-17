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
};
