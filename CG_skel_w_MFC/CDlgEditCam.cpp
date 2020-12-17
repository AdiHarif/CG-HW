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
{

}

CDlgEditCam::~CDlgEditCam()
{
}

void CDlgEditCam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEditCam, CDialog)
END_MESSAGE_MAP()


// CDlgEditCam message handlers
