#pragma once


// CDlgEditLight dialog
#include "afxdialogex.h"
#include <vector>
#include "Light.h"
#include "vec.h"

using std::vector;


class CDlgEditLight : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditLight)

public:
	CDlgEditLight(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgEditLight();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT_LIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox names;
	float color_r;
	float color_g;
	float color_b;
	float dir_x;
	float dir_y;
	float dir_z;
	float pos_x;
	float pos_y;
	float pos_z;
	int type_radio_index;
	int names_index;
	CButton parallel_radio;
	CButton point_radio;
	CButton ambient_radio;
	CEdit color_r_editbox;
	CEdit color_g_editbox;
	CEdit color_b_editbox;
	CEdit pos_x_editbox;
	CEdit pos_y_editbox;
	CEdit pos_z_editbox;
	CEdit dir_x_editbox;
	CEdit dir_y_editbox;
	CEdit dir_z_editbox;
	vector<ParallelSource*>* parallel_sources;
	vector<PointSource*>* point_sources;

	afx_msg void OnBnClickedRadioAmbient();
	afx_msg void OnBnClickedRadioParallel();
	afx_msg void OnBnClickedRadioPoint();
	afx_msg void OnCbnSelchangeComboName();
	afx_msg void OnBnClickedOk();

	void setParallelSources(vector<ParallelSource*>* parallel_sources);
	void setPointSources(vector<PointSource*>* point_sources);
	void updateComboParallel();
	void updateComboPoint();
	void clearAllEditBoxes();
	ParallelSource* getSelectedParallelSource();
	PointSource* getSelectedPointSource();
};
