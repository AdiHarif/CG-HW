// CDlgNewLight.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgNewLight.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgNewLight dialog

IMPLEMENT_DYNAMIC(CDlgNewLight, CDialog)

CDlgNewLight::CDlgNewLight(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_NEW_LIGHT, pParent)
	, color_r(0)
	, color_g(0)
	, color_b(0)
	, dir_x(0)
	, dir_y(0)
	, dir_z(0)
	, pos_x(0)
	, pos_y(0)
	, pos_z(0)
{

}

CDlgNewLight::~CDlgNewLight()
{
}

void CDlgNewLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, name);
	DDX_Text(pDX, IDC_EDIT_COLOR_R, color_r);
	DDX_Text(pDX, IDC_EDIT_COLOR_G, color_g);
	DDX_Text(pDX, IDC_EDIT_COLOR_B, color_b);
	DDX_Text(pDX, IDC_EDIT_DIR_X, dir_x);
	DDX_Text(pDX, IDC_EDIT_DIR_Y, dir_y);
	DDX_Text(pDX, IDC_EDIT_DIR_Z, dir_z);
	DDX_Text(pDX, IDC_EDIT_POS_X, pos_x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, pos_y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, pos_z);
	DDX_Control(pDX, IDC_RADIO_PARALLEL, parallel_radio);
	DDX_Control(pDX, IDC_RADIO_POINT, point_radio);
	DDX_Control(pDX, IDC_EDIT_DIR_X, dir_x_editbox);
	DDX_Control(pDX, IDC_EDIT_DIR_Y, dir_y_editbox);
	DDX_Control(pDX, IDC_EDIT_DIR_Z, dir_z_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_X, pos_x_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_Y, pos_y_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_Z, pos_z_editbox);
	parallel_radio.SetCheck(TRUE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
}


BEGIN_MESSAGE_MAP(CDlgNewLight, CDialog)
	ON_BN_CLICKED(IDC_RADIO_PARALLEL, &CDlgNewLight::OnBnClickedRadioParallel)
	ON_BN_CLICKED(IDC_RADIO_POINT, &CDlgNewLight::OnBnClickedRadioPoint)
	ON_BN_CLICKED(IDOK, &CDlgNewLight::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewLight message handlers

void CDlgNewLight::OnBnClickedRadioParallel()
{
	dir_x_editbox.EnableWindow(TRUE);
	dir_y_editbox.EnableWindow(TRUE);
	dir_z_editbox.EnableWindow(TRUE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
	type_radio_index = 0;
}


void CDlgNewLight::OnBnClickedRadioPoint()
{
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(TRUE);
	pos_y_editbox.EnableWindow(TRUE);
	pos_z_editbox.EnableWindow(TRUE);
	type_radio_index = 1;
}


void CDlgNewLight::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
