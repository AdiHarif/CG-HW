#pragma once


// CDlgEditModelColor dialog
#include "afxdialogex.h"
#include "PrimMeshModel.h"
#include "NonUniformMeshModel.h"
#include "MeshModel.h"

class CDlgEditModelColor : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditModelColor)

public:
	CDlgEditModelColor(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgEditModelColor();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT_MODEL_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float ambient_r;
	float ambient_g;
	float ambient_b;
	float diffuse_r;
	float diffuse_g;
	float diffuse_b;
	float specular_r;
	float specular_g;
	float specular_b;
	float emit_r;
	float emit_g;
	float emit_b;
	CEdit ambient_r_editbox;
	CEdit ambient_g_editbox;
	CEdit ambient_b_editbox;
	CEdit diffuse_r_editbox;
	CEdit diffuse_g_editbox;
	CEdit diffuse_b_editbox;
	CEdit specular_r_editbox;
	CEdit specular_g_editbox;
	CEdit specular_b_editbox;
	CEdit emit_r_editbox;
	CEdit emit_g_editbox;
	CEdit emit_b_editbox;

	Model* model;

	void updateEditBoxes();
	void setModel(Model* m);
};
