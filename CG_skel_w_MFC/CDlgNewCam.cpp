// CDlgNewCam.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgNewCam.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgNewCam dialog

IMPLEMENT_DYNAMIC(CDlgNewCam, CDialog)

CDlgNewCam::CDlgNewCam(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_NEW_CAMERA, pParent)
	, pos_x(0)
	, pos_y(0)
	, pos_z(0)
	, lookat_x(0)
	, lookat_y(0)
	, lookat_z(0)
	, up_x(0)
	, up_y(0)
	, up_z(0)
{

}

CDlgNewCam::~CDlgNewCam()
{
}

void CDlgNewCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS_X, pos_x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, pos_y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, pos_z);
	DDX_Text(pDX, IDC_EDIT_LOOKAT_X, lookat_x);
	DDX_Text(pDX, IDC_EDIT_LOOKAT_Y, lookat_y);
	DDX_Text(pDX, IDC_EDIT_LOOKAT_Z, lookat_z);
	DDX_Text(pDX, IDC_EDIT_UP_X, up_x);
	DDX_Text(pDX, IDC_EDIT_UP_Y, up_y);
	DDX_Text(pDX, IDC_EDIT_UP_Z, up_z);
}


BEGIN_MESSAGE_MAP(CDlgNewCam, CDialog)
END_MESSAGE_MAP()


// CDlgNewCam message handlers
