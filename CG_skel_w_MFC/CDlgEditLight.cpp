// CDlgEditLight.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgEditLight.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgEditLight dialog

IMPLEMENT_DYNAMIC(CDlgEditLight, CDialog)

CDlgEditLight::CDlgEditLight(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EDIT_LIGHT, pParent)
	, color_r(0)
	, color_g(0)
	, color_b(0)
	, dir_x(0)
	, dir_y(0)
	, dir_z(0)
	, pos_x(0)
	, pos_y(0)
	, pos_z(0)
	, type_radio_index(0)
	, names_index(-1)
{

}

CDlgEditLight::~CDlgEditLight()
{
}

void CDlgEditLight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NAMES, names);
	DDX_Text(pDX, IDC_EDIT_COLOR_R, color_r);
	DDX_Text(pDX, IDC_EDIT_COLOR_G, color_g);
	DDX_Text(pDX, IDC_EDIT_COLOR_B, color_b);
	DDX_Text(pDX, IDC_EDIT_DIR_X, dir_x);
	DDX_Text(pDX, IDC_EDIT_DIR_Y, dir_y);
	DDX_Text(pDX, IDC_EDIT_DIR_Z, dir_z);
	DDX_Text(pDX, IDC_EDIT_POS_X, pos_x);
	DDX_Text(pDX, IDC_EDIT_POS_Y, pos_y);
	DDX_Text(pDX, IDC_EDIT_POS_Z, pos_z);
	DDX_Control(pDX, IDC_RADIO_AMBIENT, ambient_radio);
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
	updateComboParallel();
}


BEGIN_MESSAGE_MAP(CDlgEditLight, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AMBIENT, &CDlgEditLight::OnBnClickedRadioAmbient)
	ON_BN_CLICKED(IDC_RADIO_PARALLEL, &CDlgEditLight::OnBnClickedRadioParallel)
	ON_BN_CLICKED(IDC_RADIO_POINT, &CDlgEditLight::OnBnClickedRadioPoint)
	ON_CBN_SELCHANGE(IDC_COMBO_NAMES, &CDlgEditLight::OnCbnSelchangeComboName)
	ON_BN_CLICKED(IDOK, &CDlgEditLight::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEditLight message handlers



void CDlgEditLight::OnBnClickedRadioParallel()
{
	dir_x_editbox.EnableWindow(TRUE);
	dir_y_editbox.EnableWindow(TRUE);
	dir_z_editbox.EnableWindow(TRUE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
	type_radio_index = 0;

	names.Clear();
	names.ResetContent();
	updateComboParallel();
}


void CDlgEditLight::OnBnClickedRadioPoint()
{
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(TRUE);
	pos_y_editbox.EnableWindow(TRUE);
	pos_z_editbox.EnableWindow(TRUE);
	type_radio_index = 1;

	names.Clear();
	names.ResetContent();
	updateComboPoint();
}

void CDlgEditLight::OnBnClickedRadioAmbient()
{
	dir_x_editbox.EnableWindow(FALSE);
	dir_y_editbox.EnableWindow(FALSE);
	dir_z_editbox.EnableWindow(FALSE);
	pos_x_editbox.EnableWindow(FALSE);
	pos_y_editbox.EnableWindow(FALSE);
	pos_z_editbox.EnableWindow(FALSE);
	type_radio_index = 2;

	names.Clear();
	names.ResetContent();
}

void CDlgEditLight::OnCbnSelchangeComboName()
{
	names_index = names.GetCurSel();
}


void CDlgEditLight::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CDlgEditLight::setParallelSources(vector<ParallelSource*>* parallel_sources) {
	this->parallel_sources = parallel_sources;
}

void CDlgEditLight::setPointSources(vector<PointSource*>* point_sources) {
	this->point_sources = point_sources;
}


void CDlgEditLight::updateComboParallel() {
	for (vector<ParallelSource*>::iterator i = parallel_sources->begin(); i != parallel_sources->end(); i++) {
		ParallelSource* parallel_s = dynamic_cast<ParallelSource*> ((*i));
		names.AddString(_T(parallel_s->getName()));
	}
}

void CDlgEditLight::updateComboPoint() {
	for (vector<PointSource*>::iterator i = point_sources->begin(); i != point_sources->end(); i++) {
		PointSource* point_s = dynamic_cast<PointSource*> ((*i));
		names.AddString(_T(point_s->getName()));
	}
}