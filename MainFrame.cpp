/* Copyright (C) 2012-2024 Stefan-Mihai MOGA
This file is part of IntelliTask application developed by Stefan-Mihai MOGA.
IntelliTask is an alternative Windows version to the famous Task Manager!

IntelliTask is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Open
Source Initiative, either version 3 of the License, or any later version.

IntelliTask is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
IntelliTask. If not, see <http://www.opensource.org/licenses/gpl-3.0.html>*/

// MainFrame.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "IntelliTask.h"

#include "MainFrame.h"
#include "EnumDevicesDlg.h"
#include "EnumProgramsDlg.h"
#include "WebBrowserDlg.h"
#include "CheckForUpdatesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, &CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, &CFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, &CFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, &CFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_VIEW_CAPTION_BAR, &CMainFrame::OnViewCaptionBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
	ON_COMMAND(ID_TOOLS_OPTIONS, &CMainFrame::OnOptions)
	ON_COMMAND(ID_PROPERTIES, &CMainFrame::OnProperties)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES, &CMainFrame::OnUpdateProperties)
	ON_COMMAND(ID_KILL_PROCESS, &CMainFrame::OnKillProcess)
	ON_UPDATE_COMMAND_UI(ID_KILL_PROCESS, &CMainFrame::OnUpdateKillProcess)
	ON_COMMAND(IDC_DEVICES, &CMainFrame::OnDevices)
	ON_COMMAND(IDC_PROGRAMS, &CMainFrame::OnPrograms)
	ON_COMMAND(IDC_TWITTER, &CMainFrame::OnTwitter)
	ON_COMMAND(IDC_LINKEDIN, &CMainFrame::OnLinkedin)
	ON_COMMAND(IDC_FACEBOOK, &CMainFrame::OnFacebook)
	ON_COMMAND(IDC_INSTAGRAM, &CMainFrame::OnInstagram)
	ON_COMMAND(IDC_ISSUES, &CMainFrame::OnIssues)
	ON_COMMAND(IDC_DISCUSSIONS, &CMainFrame::OnDiscussions)
	ON_COMMAND(IDC_WIKI, &CMainFrame::OnWiki)
	ON_COMMAND(IDC_USER_MANUAL, &CMainFrame::OnUserManual)
	ON_COMMAND(IDC_CHECK_FOR_UPDATES, &CMainFrame::OnCheckForUpdates)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_MainButton = nullptr;
	m_pProcessView = nullptr;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	m_MainButton = new CMFCRibbonApplicationButton;
	m_MainButton->SetVisible(FALSE);
	m_wndRibbonBar.SetApplicationButton(m_MainButton, CSize());

	/* if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	bool bNameValid;
	CString strTitlePane;
	bNameValid = strTitlePane.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(
		ID_STATUSBAR_PANE1, strTitlePane, TRUE, nullptr,
		_T("012345678901234567890123456789012345678901234567890123456789")), strTitlePane); */

	/* Create a caption bar :
	if (!CreateCaptionBar())
	{
		TRACE0("Failed to create caption bar\n");
		return -1;      // fail to create
	} */

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CCreateContext pProcessContext;
	pProcessContext.m_pCurrentDoc = nullptr;
	pProcessContext.m_pCurrentFrame = this;
	pProcessContext.m_pLastView = nullptr;
	pProcessContext.m_pNewDocTemplate = nullptr;
	pProcessContext.m_pNewViewClass = RUNTIME_CLASS(CProcessView);

	if ((m_pProcessView = (CProcessView*) CreateView(&pProcessContext, AFX_IDW_PANE_FIRST)) != nullptr)
	{
		m_pProcessView->ShowWindow(SW_SHOW);
		m_pProcessView->OnInitialUpdate();

		m_pProcessView->m_pMainFrame = this;
	}

	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

bool CMainFrame::CreateCaptionBar()
{
	if (!m_wndCaptionBar.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, ID_VIEW_CAPTION_BAR, -1, TRUE))
	{
		TRACE0("Failed to create caption bar\n");
		return false;
	}

	bool bNameValid;
	CString strTemp, strTemp2;
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetButtonToolTip(strTemp);

	bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

	m_wndCaptionBar.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
	bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
	ASSERT(bNameValid);
	bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
	ASSERT(bNameValid);
	m_wndCaptionBar.SetImageToolTip(strTemp, strTemp2);

	return true;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_pProcessView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_pProcessView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewCaptionBar()
{
	m_wndCaptionBar.ShowWindow(m_wndCaptionBar.IsVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndCaptionBar.IsVisible());
}

void CMainFrame::OnOptions()
{
	CMFCRibbonCustomizeDialog *pOptionsDlg = new CMFCRibbonCustomizeDialog(this, &m_wndRibbonBar);
	ASSERT(pOptionsDlg != nullptr);

	pOptionsDlg->DoModal();
	delete pOptionsDlg;
}

void CMainFrame::OnProperties()
{
	CProcessView* pProcessView = (CProcessView*)GetActiveView();
	ASSERT_VALID(pProcessView);
	const int nListItem = pProcessView->GetListCtrl().GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
	CProcessData* pProcessData = pProcessView->m_pSystemSnapshot.GetProcessID((int)pProcessView->GetListCtrl().GetItemData(nListItem));
	ASSERT(pProcessData != nullptr);
	CString sPathName{ pProcessData->GetFilePath() };
	if (!sPathName.IsEmpty())
	{
		TRACE(sPathName);
		SHELLEXECUTEINFO sei;
		memset(&sei, 0, sizeof(sei));
		sei.cbSize = sizeof(sei);
		sei.hwnd = AfxGetMainWnd()->GetSafeHwnd();
		sei.nShow = SW_SHOW;
		sei.lpFile = sPathName.GetBuffer(sPathName.GetLength());
		sei.lpVerb = _T("properties");
		sei.fMask = SEE_MASK_INVOKEIDLIST;
#pragma warning(suppress: 26486)
		ShellExecuteEx(&sei);
		sPathName.ReleaseBuffer();
	}
}

void CMainFrame::OnUpdateProperties(CCmdUI *pCmdUI)
{
	CProcessView* pProcessView = (CProcessView*)GetActiveView();
	if ((pProcessView != nullptr) && (pProcessView->GetSafeHwnd() != nullptr))
	{
		const int nListItem = pProcessView->GetListCtrl().GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
		pCmdUI->Enable(nListItem != -1);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnKillProcess()
{
	CProcessView* pProcessView = (CProcessView*)GetActiveView();
	ASSERT_VALID(pProcessView);
	const int nListItem = pProcessView->GetListCtrl().GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
	CProcessData* pProcessData = pProcessView->m_pSystemSnapshot.GetProcessID((int)pProcessView->GetListCtrl().GetItemData(nListItem));
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL  bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, pProcessData->GetProcessID());
	if (hProcess != NULL)
	{
		bool result = TerminateProcess(hProcess, (UINT)-1);
		if (result)
		{
			CString strMessage;
			strMessage.Format(_T("%s has been terminated!"), static_cast<LPCTSTR>(pProcessData->GetFilePath()));
			MessageBox(strMessage, _T("IntelliTask"), MB_OK);
		}
		CloseHandle(hProcess);
	}
}

void CMainFrame::OnUpdateKillProcess(CCmdUI *pCmdUI)
{
	CProcessView* pProcessView = (CProcessView*)GetActiveView();
	if ((pProcessView != nullptr) && (pProcessView->GetSafeHwnd() != nullptr))
	{
		const int nListItem = pProcessView->GetListCtrl().GetNextItem(-1, LVIS_SELECTED | LVIS_FOCUSED);
		pCmdUI->Enable(nListItem != -1);
	}
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnDevices()
{
	CEnumDevicesDlg pEnumDevicesDlg(this);
	pEnumDevicesDlg.DoModal();
}

void CMainFrame::OnPrograms()
{
	CEnumProgramsDlg pEnumProgramsDlg(this);
	pEnumProgramsDlg.DoModal();
}

void CMainFrame::OnTwitter()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://x.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnLinkedin()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.linkedin.com/in/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnFacebook()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.facebook.com/stefanmihaimoga"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnInstagram()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://www.instagram.com/stefanmihaimoga/"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnIssues()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliTask/issues"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnDiscussions()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliTask/discussions"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnWiki()
{
	::ShellExecute(GetSafeHwnd(), _T("open"), _T("https://github.com/mihaimoga/IntelliTask/wiki"), nullptr, nullptr, SW_SHOW);
}

void CMainFrame::OnUserManual()
{
	CWebBrowserDlg dlgWebBrowser(this);
	dlgWebBrowser.DoModal();
}

void CMainFrame::OnCheckForUpdates()
{
	CCheckForUpdatesDlg dlgCheckForUpdates(this);
	dlgCheckForUpdates.DoModal();
}
