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

DWORD WINAPI draw_first_builtin(LPVOID Dlg) { Sleep(500); map.draw_builtin((CDialog*)Dlg); return 0; }

DlgOptions::DlgOptions(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPTIONS, pParent)
{
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
END_MESSAGE_MAP()


// DlgOptions message handlers


void DlgOptions::OnStnClickedPreview()
{
	// TODO: Add your control notification handler code here
}


BOOL DlgOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

#ifdef REALTIME_NEW
	SetDlgItemText(IDC_HEADPOOL_SIZE, L"1");
	SetDlgItemText(IDC_NODEPOOL_SIZE, L"1");
#else
	SetDlgItemText(IDC_HEADPOOL_SIZE, map.get_size());
	SetDlgItemText(IDC_NODEPOOL_SIZE, map.get_size());
#endif
	CloseHandle(CreateThread(NULL, 0, draw_first_builtin, this, 0, NULL));

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
	else side_length = v, redraw_erase();
}


void DlgOptions::OnBnClickedStartstop()
{
	started = !started;
	if (!started) {
		map.trial_auto_release();
	}
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
	redraw_erase();
	change_ypivot();
}


void DlgOptions::OnBnClickedDown()
{
	ypivot += 5 * move_length / side_length;
	redraw_erase();
	change_ypivot();
}


void DlgOptions::OnBnClickedLeft()
{
	xpivot -= 5 * move_length / side_length;
	redraw_erase();
	change_xpivot();
}


void DlgOptions::OnBnClickedRight()
{
	xpivot += 5 * move_length / side_length;
	redraw_erase();
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
	change_xpivot(), redraw_erase();
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
	change_ypivot(), redraw_erase();
}
