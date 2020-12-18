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
	, top(0)
	, z_near(0)
	, z_far(0)
	, fovy(0)
	, aspect(0)
	, rot_order_index(0)
	, proj_radio_index(0)
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
	DDX_Text(pDX, IDC_EDIT_TOP, top);
	DDX_Text(pDX, IDC_EDIT_Z_NEAR, z_near);
	DDX_Text(pDX, IDC_EDIT_Z_FAR, z_far);
	DDX_Text(pDX, IDC_EDIT_FOVY, fovy);
	DDX_Text(pDX, IDC_EDIT_ASPECT, aspect);
	DDX_Control(pDX, IDC_RADIO_ORTHO, ortho_radio);
	DDX_Control(pDX, IDC_RADIO_FRUSTUM, frustum_radio);
	DDX_Control(pDX, IDC_RADIO_PERSPECTIVE, perspective_radio);
	DDX_Control(pDX, IDC_COMBO_ORDER, rot_order);
	DDX_Control(pDX, IDC_EDIT_LEFT, left_editbox);
	DDX_Control(pDX, IDC_EDIT_RIGHT, right_editbox);
	DDX_Control(pDX, IDC_EDIT_BOTTOM, bottom_editbox);
	DDX_Control(pDX, IDC_EDIT_TOP, top_editbox);
	DDX_Control(pDX, IDC_EDIT_Z_NEAR, z_near_editbox);
	DDX_Control(pDX, IDC_EDIT_Z_FAR, z_far_editbox);
	DDX_Control(pDX, IDC_EDIT_FOVY, fovy_editbox);
	DDX_Control(pDX, IDC_EDIT_ASPECT, aspect_editbox);
	initOrderCombo();
	ortho_radio.SetCheck(TRUE);
}


BEGIN_MESSAGE_MAP(CDlgEditCam, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgEditCam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ORTHO, &CDlgEditCam::OnBnClickedRadioOrtho)
	ON_BN_CLICKED(IDC_RADIO_FRUSTUM, &CDlgEditCam::OnBnClickedRadioFrustum)
	ON_BN_CLICKED(IDC_RADIO_PERSPECTIVE, &CDlgEditCam::OnBnClickedRadioPerspective)
	ON_CBN_SELCHANGE(IDC_COMBO_ORDER, &CDlgEditCam::OnCbnSelchangeComboOrder)
END_MESSAGE_MAP()


// CDlgNewCam message handlers


void CDlgEditCam::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CDlgEditCam::OnBnClickedRadioOrtho()
{
	left_editbox.EnableWindow(TRUE);
	right_editbox.EnableWindow(TRUE);
	top_editbox.EnableWindow(TRUE);
	bottom_editbox.EnableWindow(TRUE);
	fovy_editbox.EnableWindow(FALSE);
	aspect_editbox.EnableWindow(FALSE);
	proj_radio_index = 0;
}


void CDlgEditCam::OnBnClickedRadioFrustum()
{
	left_editbox.EnableWindow(TRUE);
	right_editbox.EnableWindow(TRUE);
	top_editbox.EnableWindow(TRUE);
	bottom_editbox.EnableWindow(TRUE);
	fovy_editbox.EnableWindow(FALSE);
	aspect_editbox.EnableWindow(FALSE);
	proj_radio_index = 1;
}


void CDlgEditCam::OnBnClickedRadioPerspective()
{
	left_editbox.EnableWindow(FALSE);
	right_editbox.EnableWindow(FALSE);
	top_editbox.EnableWindow(FALSE);
	bottom_editbox.EnableWindow(FALSE);
	fovy_editbox.EnableWindow(TRUE);
	aspect_editbox.EnableWindow(TRUE);
	proj_radio_index = 2;
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

void CDlgEditCam::OnCbnSelchangeComboOrder()
{
	rot_order_index = rot_order.GetCurSel();
	printf("%d\n", rot_order_index);
}
