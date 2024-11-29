/* Copyright (C) 2012-2025 Stefan-Mihai MOGA
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

#pragma once

#include "WndResizer.h"

// CEnumProgramsDlg dialog

class CSortStringArray : public CStringArray
{
public:
	void Sort();
private:
	BOOL CompareAndSwap(int pos);
};

class CEnumProgramsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnumProgramsDlg)

public:
	CEnumProgramsDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CEnumProgramsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMPROGRAMS_DIALOG };
#endif
	CWndResizer m_pWindowResizer;
	CSortStringArray m_arrPrograms;
	CListCtrl m_ctrlPrograms;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedVersion();
	afx_msg void OnClickedRefresh();

	DECLARE_MESSAGE_MAP()
public:
};
