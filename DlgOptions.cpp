// DlgOptions.cpp : implementation file
//

#include "pch.h"
#include "Life-MFC.h"
#include "DlgOptions.h"
#include "afxdialogex.h"
#include "Map.h"
#include "MainFrm.h"
extern Map map;

// DlgOptions dialog
DlgOptions theDlg;

IMPLEMENT_DYNAMIC(DlgOptions, CDialogEx)


DlgOptions::DlgOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{
	m_hAccel = NULL;
}

DlgOptions::~DlgOptions()
{
}

void DlgOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgOptions, CDialogEx)
	ON_STN_CLICKED(IDC_PREVIEW, &DlgOptions::OnStnClickedPreview)
	ON_EN_CHANGE(IDC_BUILTIN, &DlgOptions::OnChangeBuiltin)
	ON_EN_CHANGE(IDC_DIRECTION, &DlgOptions::OnChangeDirection)
	ON_EN_CHANGE(IDC_TIMER, &DlgOptions::OnChangeTimer)
	ON_EN_CHANGE(IDC_SCALE, &DlgOptions::OnChangeScale)
	ON_BN_CLICKED(IDC_STARTSTOP, &DlgOptions::OnBnClickedStartstop)
	ON_BN_CLICKED(IDC_RESET, &DlgOptions::OnBnClickedReset)
	ON_BN_CLICKED(IDC_UP, &DlgOptions::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &DlgOptions::OnBnClickedDown)
	ON_BN_CLICKED(IDC_LEFT, &DlgOptions::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &DlgOptions::OnBnClickedRight)
	ON_EN_CHANGE(IDC_XPIVOT, &DlgOptions::OnChangeXpivot)
	ON_EN_CHANGE(IDC_YPIVOT, &DlgOptions::OnChangeYpivot)
	ON_BN_CLICKED(IDC_RADIOCLICK, &DlgOptions::OnBnClickedRadioclick)
	ON_BN_CLICKED(IDC_RADIOPEN, &DlgOptions::OnBnClickedRadiopen)
	ON_BN_CLICKED(IDC_RADIOERASER, &DlgOptions::OnBnClickedRadioeraser)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIODRAG, &DlgOptions::OnBnClickedRadiodrag)
END_MESSAGE_MAP()


// DlgOptions message handlers


void DlgOptions::OnStnClickedPreview()
{
	// TODO: Add your control notification handler code here
}


BOOL DlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	CWnd* pBuiltin = GetDlgItem(IDC_BUILTIN);
	pBuiltin->SendMessage(EM_SETLIMITTEXT, 1, 0);
	pBuiltin->SendMessage(WM_SETTEXT, 0, (LPARAM)L"0");

	CWnd* pDirection = GetDlgItem(IDC_DIRECTION);
	pDirection->SendMessage(EM_SETLIMITTEXT, 1, 0);
	pDirection->SendMessage(WM_SETTEXT, 0, (LPARAM)L"0");

	CWnd* pTimer = GetDlgItem(IDC_TIMER);
	pTimer->SendMessage(EM_SETLIMITTEXT, 5, 0);
	pTimer->SendMessage(WM_SETTEXT, 0, (LPARAM)L"50");

	CWnd* pScale = GetDlgItem(IDC_SCALE);
	pScale->SendMessage(EM_SETLIMITTEXT, 3, 0);
	pScale->SendMessage(WM_SETTEXT, 0, (LPARAM)L"10");

	CWnd* pSetX = GetDlgItem(IDC_XPIVOT);
	pSetX->SendMessage(EM_SETLIMITTEXT, 10, 0);
	pSetX->SendMessage(WM_SETTEXT, 0, (LPARAM)L"08000000");

	CWnd* pSetY = GetDlgItem(IDC_YPIVOT);
	pSetY->SendMessage(EM_SETLIMITTEXT, 10, 0);
	pSetY->SendMessage(WM_SETTEXT, 0, (LPARAM)L"08000000");

	CWnd* pClick = GetDlgItem(IDC_RADIOCLICK);
	pClick->SendMessage(WM_LBUTTONDOWN, 0, 0);
	pClick->SendMessage(WM_LBUTTONUP, 0, 0);

#ifdef REALTIME_NEW
	SetDlgItemText(IDC_HEADPOOL_SIZE, L"1");
	SetDlgItemText(IDC_NODEPOOL_SIZE, L"1");
#else
	SetDlgItemText(IDC_ACTUALHEADPOOLSIZE, map.get_size());
	SetDlgItemText(IDC_ACTUALNODEPOOLSIZE, map.get_size());
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DlgOptions::OnChangeBuiltin()
{
	WCHAR val[16];
	GetDlgItemText(IDC_BUILTIN, val, 16);
	selected_builtin = _wtoi(val);
	map.draw_builtin(this);
}


void DlgOptions::OnChangeDirection()
{
	WCHAR val[16];
	GetDlgItemText(IDC_DIRECTION, val, 16);
	selected_direction = _wtoi(val);
	map.draw_builtin(this);
}


void DlgOptions::OnChangeTimer()
{
	WCHAR val[16];
	GetDlgItemText(IDC_TIMER, val, 16);
	int v = _wtoi(val);
	if (v <= 0) SetDlgItemText(IDC_TIMER, L"1");
	else {
		TIMER = v;
		theApp.m_pMainWnd->SetTimer(1, TIMER, NULL);
	}
}


void DlgOptions::OnChangeScale()
{
	WCHAR val[16];
	GetDlgItemText(IDC_SCALE, val, 16);
	int v = _wtoi(val);
	if (v <= 2) SetDlgItemText(IDC_SCALE, L"3");
	else side_length = v, redraw();
}


void DlgOptions::OnBnClickedStartstop()
{
	started = !started;
	if (!started) map.free_extra();
}


void DlgOptions::OnBnClickedReset()
{
	xpivot = ypivot = 0x08000000;
	started = false;
	map.clear();
}


void DlgOptions::OnBnClickedUp()
{
	ypivot -= 5 * move_length / side_length;
	redraw();
	change_ypivot();
}


void DlgOptions::OnBnClickedDown()
{
	ypivot += 5 * move_length / side_length;
	redraw();
	change_ypivot();
}


void DlgOptions::OnBnClickedLeft()
{
	xpivot -= 5 * move_length / side_length;
	redraw();
	change_xpivot();
}


void DlgOptions::OnBnClickedRight()
{
	xpivot += 5 * move_length / side_length;
	redraw();
	change_xpivot();
}


void DlgOptions::OnChangeXpivot()
{
	WCHAR xstr[16];
	GetDlgItemText(IDC_XPIVOT, xstr, 16);
	int len = lstrlen(xstr);
	for (int i = 0; i < len; i++)
		if (!((xstr[i] >= L'0' && xstr[i] <= L'9') || (xstr[i] >= L'a' && xstr[i] <= L'f'))) {
			MessageBox(L"X Pivot should be a value ranged (0, 0x10000000)", L"Error", MB_OK);
			return change_xpivot();
		}
	unsigned int x = wcstol(xstr, nullptr, 16);
	if (x < 0x10000000 && x > 0) {
		if (xpivot == x && len == 8) return;
		xpivot = x;
	}
	else {
		MessageBox(L"X Pivot should be a value ranged (0, 0x10000000)", L"Error", MB_OK);
	}
	change_xpivot(), redraw();
}


void DlgOptions::OnChangeYpivot()
{
	WCHAR ystr[16];
	GetDlgItemText(IDC_YPIVOT, ystr, 16);
	int len = lstrlen(ystr);
	for (int i = 0; i < len; i++)
		if (!((ystr[i] >= L'0' && ystr[i] <= L'9') || (ystr[i] >= L'a' && ystr[i] <= L'f'))) {
			MessageBox(L"Y Pivot should be a value ranged (0, 0x10000000)", L"Error", MB_OK);
			return change_ypivot();
		}
	unsigned int y = wcstol(ystr, nullptr, 16);
	if (y < 0x10000000 && y > 0) {
		if (ypivot == y && len == 8) return;
		ypivot = y;
	}
	else {
		MessageBox(L"Y Pivot should be a value ranged (0, 0x10000000)", L"Error", MB_OK);
	}
	change_ypivot(), redraw();
}


void DlgOptions::OnBnClickedRadioclick()
{
	mi.state = 0;
}


void DlgOptions::OnBnClickedRadiopen()
{
	mi.state = 1;
}


void DlgOptions::OnBnClickedRadioeraser()
{
	mi.state = 2;
}


void DlgOptions::OnBnClickedRadiodrag()
{
	mi.state = 3;
}


void DlgOptions::OnPaint()
{
	map.draw_builtin(this);
	CDialogEx::OnPaint();
}



BOOL DlgOptions::PreTranslateMessage(MSG* pMsg)
{
	if (::TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

