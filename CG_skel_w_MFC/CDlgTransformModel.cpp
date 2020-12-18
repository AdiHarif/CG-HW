// CDlgTransformModel.cpp : implementation file
//

//#include "pch.h"
#include "CG_skel_w_MFC.h"
#include "CDlgTransformModel.h"
#include "afxdialogex.h"
#include "afxwin.h"


// CDlgTransformModel dialog

IMPLEMENT_DYNAMIC(CDlgTransformModel, CDialog)

CDlgTransformModel::CDlgTransformModel(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_TRANSFORM_ACTIVE_MODEL, pParent)
	, translate_x(0)
	, translate_y(0)
	, translate_z(0)
	, rotate_x(0)
	, rotate_y(0)
	, rotate_z(0)
	, scale_x(0)
	, scale_y(0)
	, scale_z(0)
	, rot_order_index(0)

{

}

CDlgTransformModel::~CDlgTransformModel()
{
}

void CDlgTransformModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_X, translate_x);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_Y, translate_y);
	DDX_Text(pDX, IDC_EDIT_TRANSLATE_Z, translate_z);
	DDX_Text(pDX, IDC_EDIT_ROTATE_X, rotate_x);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Y, rotate_y);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Z, rotate_z);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, scale_x);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, scale_y);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, scale_z);
	DDX_Control(pDX, IDC_COMBO_ORDER, rot_order);
	initOrderCombo();
}


BEGIN_MESSAGE_MAP(CDlgTransformModel, CDialog)
END_MESSAGE_MAP()


// CDlgTransformModel message handlers

void CDlgTransformModel::initOrderCombo() {
	rot_order.AddString(_T("x->y->z"));
	rot_order.AddString(_T("x->z->y"));
	rot_order.AddString(_T("y->x->z"));
	rot_order.AddString(_T("y->z->x"));
	rot_order.AddString(_T("z->x->y"));
	rot_order.AddString(_T("z->y->x"));
	rot_order.SetCurSel(0);
}

void CDlgTransformModel::OnCbnSelchangeComboOrder()
{
	rot_order_index = rot_order.GetCurSel();
}

