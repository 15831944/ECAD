
// Gears.h : main header file for the Gears application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "HEWinApp.h"
#include "app_share.h"

//TODO use release/debug builds
#ifdef _DEBUG
#pragma comment(lib, "KiCAD_parsed.lib")
#pragma comment(lib, "BXL_Reader-x86-d.lib")
#else
#pragma comment(lib, "KiCAD_parse.lib")
#pragma comment(lib, "BXL_Reader-x86.lib")
#endif

#pragma comment(lib, "sqlite3.lib")

class CADApp : public HEWinApp
{
public:
	CADApp();

	profile_t profile;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpenbxlfile();
};

extern CADApp theApp;
