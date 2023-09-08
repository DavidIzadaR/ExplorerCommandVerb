// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

// ExplorerCommand handlers are an in-proc verb implementation method that can provide
// dynamic behavior including computing the name of the command, its icon and its visibility state.
// only use this verb implementation method if you are implementing a command handler on
// the commands module and need the same functionality on a context menu.
//
// each ExplorerCommand handler needs to have a unique COM object, run UUIDGEN.EXE to
// create new CLSID values for your handler. a handler can implement multiple
// different verbs using the information provided via IInitializeCommand (the verb name).
// your code can switch off those different verb names or the properties provided
// in the property bag

#include "Dll.h"
#include "ExplorerCommands.h"
#include <Windows.h>

static WCHAR const c_szVerbDisplayName[] = L"Custom context menu";
static WCHAR const c_szVerbName[] = L"Sample.ExplorerCommandVerb";

CExplorerCommandVerb::CExplorerCommandVerb() : 
	_cRef(1), 
	_punkSite(NULL), 
	_index(0)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb()");
	OutputDebugString(c_szVerbDisplayName);

	wcscpy_s(_selection, L"");
	TestCmd1 = new CExplorerCommandVerb1(*this);
	TestCmd2 = new CExplorerCommandVerb2(*this);

	DllAddRef();
}

CExplorerCommandVerb::~CExplorerCommandVerb()
{
	OutputDebugString(L"[ContextMenuSample] ~CExplorerCommandVerb()");
	OutputDebugString(c_szVerbDisplayName);

	SafeRelease(&_punkSite);
	DllRelease();
}

// IUnknown
IFACEMETHODIMP CExplorerCommandVerb::QueryInterface(REFIID riid, void** ppv)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::QueryInterface");

	if (ppv == nullptr)
		return E_POINTER;

	static const QITAB qit[] =
	{
		QITABENT(CExplorerCommandVerb, IExplorerCommand),
		QITABENT(CExplorerCommandVerb, IObjectWithSite),
		QITABENT(CExplorerCommandVerb, IEnumExplorerCommand),
		{ 0 },
	};

	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb::Release()
{
	long cRef = InterlockedDecrement(&_cRef);
	if (!cRef)
	{
		delete this;
	}
	return cRef;
}

// IExplorerCommand
IFACEMETHODIMP CExplorerCommandVerb::GetTitle(IShellItemArray* psia, LPWSTR* ppszName)
{
	UNREFERENCED_PARAMETER(psia);
	// the verb name can be computed here, in this example it is static
	return SHStrDup(c_szVerbDisplayName, ppszName);
}

IFACEMETHODIMP CExplorerCommandVerb::GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon)
{
	UNREFERENCED_PARAMETER(psia);
	// the icon ref ("dll,-<resid>") is provided here, in this case none is provided
	*ppszIcon = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb::GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip)
{
	UNREFERENCED_PARAMETER(psia);
	// tooltip provided here, in this case none is provided
	*ppszInfotip = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb::GetCanonicalName(GUID* pguidCommandName)
{
	*pguidCommandName = __uuidof(this);
	return S_OK;
}

// compute the visibility of the verb here, respect "fOkToBeSlow" if this is slow (does IO for example)
// when called with fOkToBeSlow == FALSE return E_PENDING and this object will be called
// back on a background thread with fOkToBeSlow == TRUE
IFACEMETHODIMP CExplorerCommandVerb::GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::GetState");
	UNREFERENCED_PARAMETER(fOkToBeSlow);

	// capture the first selected item
	if (psia)
	{
		DWORD count;
		psia->GetCount(&count);

		IShellItem2* psi;
		PWSTR pszName;
		if (SUCCEEDED(GetItemAt(psia, 0, IID_PPV_ARGS(&psi))))
		{
			if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszName)))
			{
				wcscpy_s(_selection, pszName);

				CoTaskMemFree(pszName);
			}

			psi->Release();
		}
	}

	// the verb is always enabled in this example to show the subcommands
	*pCmdState = ECS_ENABLED;
	HRESULT hr = S_OK;
	return hr;
}

IFACEMETHODIMP CExplorerCommandVerb::GetFlags(EXPCMDFLAGS* pFlags)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::GetFlags");

	// the verb has two subcommands
	*pFlags = ECF_HASSUBCOMMANDS;
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::EnumSubCommands");

	// return an enumerator for the subcommands (itself in this case)
	*ppEnum = this;
	AddRef();
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::Next(ULONG celt, IExplorerCommand** ppUICommand, ULONG* pceltFetched) 
{
	UNREFERENCED_PARAMETER(celt);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::Next");

	if (_index == 0)
		*ppUICommand = TestCmd1;
	else if (_index == 1) 
		*ppUICommand = TestCmd2;
	else
		return S_FALSE;
		
	if (_index < 2)
	{
		// after returning an existing subcommand, report one was fetched
		if (pceltFetched)
			*pceltFetched = 1;
		// in any case, advance the index (to the next subcommand or past the end)
		_index++;
	}
	else
	{
		// no more subcommands, report none were fetched
		if (pceltFetched)
			*pceltFetched = 0;
	}

	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::Skip(ULONG celt)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::Skip");
	// advance the index by the number of subcommands to skip
	// allowing it to go past the end
	_index = (_index + celt) / 3;
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::Reset()
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::Reset");
	// start with the first subcommand
	_index = 0;
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::Clone(IEnumExplorerCommand** ppEnum)
{
	UNREFERENCED_PARAMETER(ppEnum);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::Clone");
	return E_NOTIMPL;
}

WCHAR* CExplorerCommandVerb::GetSelection() const
{
	return (WCHAR*)_selection;
}

// IObjectWithSite
IFACEMETHODIMP CExplorerCommandVerb::SetSite(IUnknown* punkSite)
{
	SetInterface(&_punkSite, punkSite);
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb::GetSite(REFIID riid, void** ppv)
{
	*ppv = NULL;
	return _punkSite ? _punkSite->QueryInterface(riid, ppv) : E_FAIL;
}

// Provides the HWND of the explorer window that invoked this verb
HWND CExplorerCommandVerb::GetWndHandle() const
{
	HWND _hwnd;
	IUnknown_GetWindow(_punkSite, &_hwnd);
	return _hwnd;
}

IFACEMETHODIMP CExplorerCommandVerb::Invoke(IShellItemArray* psia, IBindCtx* /* pbc */)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb::Invoke!!!???");

	// the shell should never call this method on a verb that has subcommands
	return E_NOTIMPL;
}

static WCHAR const c_szProgID[] = L"*";

HRESULT CExplorerCommandVerb_RegisterUnRegister(bool fRegister)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb_RegisterUnRegister");
	CRegisterExtension re(__uuidof(CExplorerCommandVerb));

	HRESULT hr;
	if (fRegister)
	{
		hr = re.RegisterInProcServer(c_szVerbDisplayName, L"Apartment");
		if (SUCCEEDED(hr))
		{
			// register this verb on all files (*) ProgID
			hr = re.RegisterExplorerCommandVerb(c_szProgID, c_szVerbName, c_szVerbDisplayName);
			if (SUCCEEDED(hr))
				hr = re.RegisterVerbAttribute(c_szProgID, c_szVerbName, L"NeverDefault");
		}
	}
	else
	{
		// best effort
		hr = re.UnRegisterVerb(c_szProgID, c_szVerbName);
		hr = re.UnRegisterObject();
	}
	return hr;
}

HRESULT CExplorerCommandVerb_CreateInstance(REFIID riid, void** ppv)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb_CreateInstance");
	*ppv = NULL;
	CExplorerCommandVerb* pVerb = new (std::nothrow) CExplorerCommandVerb();
	HRESULT hr = pVerb ? S_OK : E_OUTOFMEMORY;
	if (SUCCEEDED(hr))
	{
		pVerb->QueryInterface(riid, ppv);
		pVerb->Release();
	}
	return hr;
}
