#if !defined(AFX_WIZARDDLG_H__158AAD15_5E49_4EAE_B3F1_38C3C102DC1D__INCLUDED_)
#define AFX_WIZARDDLG_H__158AAD15_5E49_4EAE_B3F1_38C3C102DC1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWizardDlg dialog

class CWizardDlg : public CDialog
{
// Construction
public:
	CWizardDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWizardDlg)
	enum { IDD = IDD_WIZARD_DIALOG };
	BOOL	display;
	int		select;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizardDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIZARDDLG_H__158AAD15_5E49_4EAE_B3F1_38C3C102DC1D__INCLUDED_)
