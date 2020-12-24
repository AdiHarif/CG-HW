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
	, ambient_r(0)
	, ambient_g(0)
	, ambient_b(0)
	, diffuse_r(0)
	, diffuse_g(0)
	, diffuse_b(0)
	, specular_r(0)
	, specular_g(0)
	, specular_b(0)
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
	DDX_Text(pDX, IDC_EDIT_AMBIENT_R, ambient_r);
	DDX_Text(pDX, IDC_EDIT_AMBIENT_G, ambient_g);
	DDX_Text(pDX, IDC_EDIT_AMBIENT_B, ambient_b);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_R, diffuse_r);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_G, diffuse_g);
	DDX_Text(pDX, IDC_EDIT_DIFFUSE_B, diffuse_b);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_R, specular_r);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_G, specular_g);
	DDX_Text(pDX, IDC_EDIT_SPECULAR_B, specular_b);
	DDX_Text(pDX, IDC_EDIT_DIR_X, dir_x);
	DDX_Text(pDX, IDC_EDIT_DIR_Y, dir_y);
	DDX_Text(pDX, IDC_EDIT_DIR_Z, dir_z);
	DDX_Text(pDX, IDC_EDIT_POS_X, pos_x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, pos_y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, pos_z);
	DDX_Control(pDX, IDC_RADIO_PARALLEL, parallel_radio);
	DDX_Control(pDX, IDC_RADIO_POINT, point_radio);
	DDX_Control(pDX, IDC_RADIO_AMBIENT, ambient_radio);
	DDX_Control(pDX, IDC_EDIT_DIR_X, dir_x_editbox);
	DDX_Control(pDX, IDC_EDIT_DIR_Y, dir_y_editbox);
	DDX_Control(pDX, IDC_EDIT_DIR_Z, dir_z_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_X, pos_x_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_Y, pos_y_editbox);
	DDX_Control(pDX, IDC_EDIT_POS_Z, pos_z_editbox);
	ambient_radio.SetCheck(TRUE);
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);



}


BEGIN_MESSAGE_MAP(CDlgNewLight, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AMBIENT, &CDlgNewLight::OnBnClickedRadioAmbient)
	ON_BN_CLICKED(IDC_RADIO_PARALLEL, &CDlgNewLight::OnBnClickedRadioParallel)
	ON_BN_CLICKED(IDC_RADIO_POINT, &CDlgNewLight::OnBnClickedRadioPoint)
	ON_BN_CLICKED(IDOK, &CDlgNewLight::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewLight message handlers


void CDlgNewLight::OnBnClickedRadioAmbient()
{
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
	type_radio_index = 0;
}


void CDlgNewLight::OnBnClickedRadioParallel()
{
	dir_x_editbox.EnableWindow(TRUE);
	dir_y_editbox.EnableWindow(TRUE);
	dir_z_editbox.EnableWindow(TRUE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
	type_radio_index = 1;
}


void CDlgNewLight::OnBnClickedRadioPoint()
{
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(TRUE);
	pos_y_editbox.EnableWindow(TRUE);
	pos_z_editbox.EnableWindow(TRUE);
	type_radio_index = 2;
}


void CDlgNewLight::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
