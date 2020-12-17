// CDlgEditCam.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgEditCam.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgEditCam dialog

IMPLEMENT_DYNAMIC(CDlgEditCam, CDialog)

CDlgEditCam::CDlgEditCam(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EDIT_ACTIVE_CAMERA, pParent)
	, translate_x(0)
	, translate_y(0)
	, translate_z(0)
	, rotate_x(0)
	, rotate_y(0)
	, rotate_z(0)
	, left(0)
	, right(0)
	, bottom(0)
	, up(0)
	, z_near(0)
	, z_far(0)
	, fovy(0)
	, aspect(0)
{

}

CDlgEditCam::~CDlgEditCam()
{
}

void CDlgEditCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_X, translate_x);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_Y, translate_y);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_Z, translate_z);
	DDX_Text(pDX, IDC_EDIT_ROTATE_X, rotate_x);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Y, rotate_y);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Z, rotate_z);
	DDX_Text(pDX, IDC_EDIT_LEFT, left);
	DDX_Text(pDX, IDC_EDIT_RIGHT, right);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, bottom);
	DDX_Text(pDX, IDC_EDIT_UP, up);
	DDX_Text(pDX, IDC_EDIT_Z_NEAR, z_near);
	DDX_Text(pDX, IDC_EDIT_Z_FAR, z_far);
	DDX_Text(pDX, IDC_EDIT_FOVY, fovy);
	DDX_Text(pDX, IDC_EDIT_ASPECT, aspect);
}


BEGIN_MESSAGE_MAP(CDlgEditCam, CDialog)
END_MESSAGE_MAP()


// CDlgNewCam message handlers
