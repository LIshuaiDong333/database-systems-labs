// WizardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wb.h"
#include "WizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardDlg dialog


CWizardDlg::CWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWizardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWizardDlg)
	display = FALSE;
	select = -1;
	//}}AFX_DATA_INIT
}


void CWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardDlg)
	DDX_Check(pDX, IDC_DISPLAY_CHECK, display);
	DDX_Radio(pDX, IDC_SAMPLE_RADIO, select);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardDlg, CDialog)
	//{{AFX_MSG_MAP(CWizardDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardDlg message handlers

BOOL CWizardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	display = TRUE;
	select = 0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
