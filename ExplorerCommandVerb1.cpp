#include "Dll.h"
#include "ExplorerCommands.h"
#include <Windows.h>

static WCHAR const c_szVerbDisplayName[] = L"Exec Notepad";

CExplorerCommandVerb1::CExplorerCommandVerb1(CExplorerCommandVerb& theParent) : _cRef(1), Parent(theParent)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1()");

	DllAddRef();
}

CExplorerCommandVerb1::~CExplorerCommandVerb1()
{
	DllRelease();
}

// IUnknown
IFACEMETHODIMP CExplorerCommandVerb1::QueryInterface(REFIID riid, void** ppv)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::QueryInterface");
	static const QITAB qit[] =
	{
		QITABENT(CExplorerCommandVerb1, IExplorerCommand),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb1::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb1::Release()
{
	long cRef = InterlockedDecrement(&_cRef);
	if (!cRef)
	{
		delete this;
	}
	return cRef;
}

// IExplorerCommand

// compute the visibility of the verb here, respect "fOkToBeSlow" if this is slow (does IO for example)
// when called with fOkToBeSlow == FALSE return E_PENDING and this object will be called
// back on a background thread with fOkToBeSlow == TRUE
IFACEMETHODIMP CExplorerCommandVerb1::GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::GetState");
	UNREFERENCED_PARAMETER(psia);
	UNREFERENCED_PARAMETER(fOkToBeSlow);

	*pCmdState = ECS_ENABLED;
	HRESULT hr = S_OK;
	return hr;
}

IFACEMETHODIMP CExplorerCommandVerb1::GetTitle(IShellItemArray* psia, LPWSTR* ppszName)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::GetTitle");
	// the verb name can be computed here, in this example it is static
	return SHStrDup(c_szVerbDisplayName, ppszName);
}

IFACEMETHODIMP CExplorerCommandVerb1::GetFlags(EXPCMDFLAGS* pFlags)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::GetFlags");
	*pFlags = ECF_DEFAULT;
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb1::GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon)
{
	UNREFERENCED_PARAMETER(psia);
	// the icon ref ("dll,-<resid>") is provided here, in this case none is provided
	*ppszIcon = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb1::GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip)
{
	UNREFERENCED_PARAMETER(psia);
	// tooltip provided here, in this case none is provided
	*ppszInfotip = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb1::GetCanonicalName(GUID* pguidCommandName)
{
	*pguidCommandName = __uuidof(this);
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb1::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::EnumSubCommands");
	*ppEnum = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb1::Invoke(IShellItemArray* psia, IBindCtx* /* pbc */)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb1::Invoke");

	ShellExecute(Parent.GetWndHandle(), NULL, L"notepad.exe", Parent.GetSelection(), NULL, SW_SHOWNORMAL);

	return S_OK;
}
