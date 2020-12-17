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
	DDX_Control(pDX, IDC_RADIO_ORTHO, ortho_radio);
	DDX_Control(pDX, IDC_RADIO_FRUSTUM, frustum_radio);
	DDX_Control(pDX, IDC_RADIO_PERSPECTIVE, perspective_radio);
	DDX_Control(pDX, IDC_COMBO_ORDER, rot_order);
	initOrderCombo();
}


BEGIN_MESSAGE_MAP(CDlgEditCam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditCam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ORTHO, &CDlgEditCam::OnBnClickedRadioOrtho)
	ON_BN_CLICKED(IDC_RADIO_FRUSTUM, &CDlgEditCam::OnBnClickedRadioFrustum)
	ON_BN_CLICKED(IDC_RADIO_PERSPECTIVE, &CDlgEditCam::OnBnClickedRadioPerspective)
END_MESSAGE_MAP()


// CDlgNewCam message handlers


void CDlgEditCam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CDlgEditCam::OnBnClickedRadioOrtho()
{
	// TODO: Add your control notification handler code here
}


void CDlgEditCam::OnBnClickedRadioFrustum()
{
	// TODO: Add your control notification handler code here
}


void CDlgEditCam::OnBnClickedRadioPerspective()
{
	// TODO: Add your control notification handler code here
}

void CDlgEditCam::initOrderCombo() {
	rot_order.AddString(_T("x->y->z"));
	rot_order.AddString(_T("x->z->y"));
	rot_order.AddString(_T("y->x->z"));
	rot_order.AddString(_T("y->z->x"));
	rot_order.AddString(_T("z->x->y"));
	rot_order.AddString(_T("z->y->x"));
	rot_order.SetCurSel(0);
}