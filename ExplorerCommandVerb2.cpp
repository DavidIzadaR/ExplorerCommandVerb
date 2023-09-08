#include "Dll.h"
#include "ExplorerCommands.h"
#include <Windows.h>

static WCHAR const c_szVerbDisplayName[] = L"Exec Write";

CExplorerCommandVerb2::CExplorerCommandVerb2(CExplorerCommandVerb& theParent) : _cRef(1), Parent(theParent)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2()");

	DllAddRef();
}

CExplorerCommandVerb2::~CExplorerCommandVerb2()
{
	DllRelease();
}

// IUnknown
IFACEMETHODIMP CExplorerCommandVerb2::QueryInterface(REFIID riid, void** ppv)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::QueryInterface");
	static const QITAB qit[] =
	{
		QITABENT(CExplorerCommandVerb2, IExplorerCommand),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb2::AddRef()
{
	return InterlockedIncrement(&_cRef);
}

IFACEMETHODIMP_(ULONG) CExplorerCommandVerb2::Release()
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
IFACEMETHODIMP CExplorerCommandVerb2::GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetState");
	UNREFERENCED_PARAMETER(psia);
	UNREFERENCED_PARAMETER(fOkToBeSlow);

	*pCmdState = ECS_ENABLED;
	HRESULT hr = S_OK;
	return hr;
}

IFACEMETHODIMP CExplorerCommandVerb2::GetTitle(IShellItemArray* psia, LPWSTR* ppszName)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetTitle");
	// the verb name can be computed here, in this example it is static
	return SHStrDup(c_szVerbDisplayName, ppszName);
}

IFACEMETHODIMP CExplorerCommandVerb2::GetFlags(EXPCMDFLAGS* pFlags)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetFlags");
	*pFlags = ECF_DEFAULT;
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb2::GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetIcon");
	// the icon ref ("dll,-<resid>") is provided here, in this case none is provided
	*ppszIcon = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb2::GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetToolTip");
	// tooltip provided here, in this case none is provided
	*ppszInfotip = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb2::GetCanonicalName(GUID* pguidCommandName)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::GetCanonicalName");
	*pguidCommandName = __uuidof(this);
	return S_OK;
}

IFACEMETHODIMP CExplorerCommandVerb2::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::EnumSubCommands");
	*ppEnum = NULL;
	return E_NOTIMPL;
}

IFACEMETHODIMP CExplorerCommandVerb2::Invoke(IShellItemArray* psia, IBindCtx* /* pbc */)
{
	UNREFERENCED_PARAMETER(psia);
	OutputDebugString(L"[ContextMenuSample] CExplorerCommandVerb2::Invoke");

	ShellExecute(Parent.GetWndHandle(), NULL, L"write.exe", Parent.GetSelection(), NULL, SW_SHOWNORMAL);

	return S_OK;
}
