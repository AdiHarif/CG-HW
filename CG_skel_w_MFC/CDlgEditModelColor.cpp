// CDlgEditModelColor.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgEditModelColor.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgEditModelColor dialog

IMPLEMENT_DYNAMIC(CDlgEditModelColor, CDialog)

CDlgEditModelColor::CDlgEditModelColor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_EDIT_MODEL_COLOR, pParent)
{

}

CDlgEditModelColor::~CDlgEditModelColor()
{
}

void CDlgEditModelColor::DoDataExchange(CDataExchange* pDX)
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
	DDX_Text(pDX, IDC_EDIT_EMIT_R, emit_r);
	DDX_Text(pDX, IDC_EDIT_EMIT_G, emit_g);
	DDX_Text(pDX, IDC_EDIT_EMIT_B, emit_b);
	DDX_Control(pDX, IDC_EDIT_AMBIENT_R, ambient_r_editbox);
	DDX_Control(pDX, IDC_EDIT_AMBIENT_G, ambient_g_editbox);
	DDX_Control(pDX, IDC_EDIT_AMBIENT_B, ambient_b_editbox);
	DDX_Control(pDX, IDC_EDIT_DIFFUSE_R, diffuse_r_editbox);
	DDX_Control(pDX, IDC_EDIT_DIFFUSE_G, diffuse_g_editbox);
	DDX_Control(pDX, IDC_EDIT_DIFFUSE_B, diffuse_b_editbox);
	DDX_Control(pDX, IDC_EDIT_SPECULAR_R, specular_r_editbox);
	DDX_Control(pDX, IDC_EDIT_SPECULAR_G, specular_g_editbox);
	DDX_Control(pDX, IDC_EDIT_SPECULAR_B, specular_b_editbox);
	DDX_Control(pDX, IDC_EDIT_EMIT_R, emit_r_editbox);
	DDX_Control(pDX, IDC_EDIT_EMIT_G, emit_g_editbox);
	DDX_Control(pDX, IDC_EDIT_EMIT_B, emit_b_editbox);
	updateEditBoxes();
}


BEGIN_MESSAGE_MAP(CDlgEditModelColor, CDialog)
END_MESSAGE_MAP()


// CDlgEditModelColor message handlers

void CDlgEditModelColor::updateEditBoxes() {
	Color ambient_color = model->getAmbientColor();
	Color diffuse_color = model->getDiffuseColor();
	Color specular_color = model->getSpecularColor();
	Color emit_color = model->getEmitColor();

	CString str;
	str.Format("%f", ambient_color.r);
	ambient_r_editbox.SetWindowText(str);
	str.Format("%f", ambient_color.g);
	ambient_g_editbox.SetWindowText(str);
	str.Format("%f", ambient_color.b);
	ambient_b_editbox.SetWindowText(str);
	str.Format("%f", diffuse_color.r);
	diffuse_r_editbox.SetWindowText(str);
	str.Format("%f", diffuse_color.g);
	diffuse_g_editbox.SetWindowText(str);
	str.Format("%f", diffuse_color.b);
	diffuse_b_editbox.SetWindowText(str);
	str.Format("%f", specular_color.r);
	specular_r_editbox.SetWindowText(str);
	str.Format("%f", specular_color.g);
	specular_g_editbox.SetWindowText(str);
	str.Format("%f", specular_color.b);
	specular_b_editbox.SetWindowText(str);
	str.Format("%f", emit_color.r);
	emit_r_editbox.SetWindowText(str);
	str.Format("%f", emit_color.g);
	emit_g_editbox.SetWindowText(str);
	str.Format("%f", emit_color.b);
	emit_b_editbox.SetWindowText(str);
}

void CDlgEditModelColor::setModel(Model* m) {
	model = m;
}