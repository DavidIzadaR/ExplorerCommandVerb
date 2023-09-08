#pragma once
#include "ShellHelpers.h"

class CExplorerCommandVerb;

class __declspec(uuid("55344254-F3D0-46CB-A207-E13B66C81F61"))
CExplorerCommandVerb1 : public IExplorerCommand
{
private:
	long	_cRef;
	CExplorerCommandVerb&	Parent;
public:
	CExplorerCommandVerb1(CExplorerCommandVerb& theParent);
	CExplorerCommandVerb1::~CExplorerCommandVerb1();
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	IFACEMETHODIMP_(ULONG) AddRef() override;
	IFACEMETHODIMP_(ULONG) Release() override;
	// IExplorerCommand
	IFACEMETHODIMP GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState);
	IFACEMETHODIMP GetTitle(IShellItemArray* psia, LPWSTR* ppszName);
	IFACEMETHODIMP GetFlags(EXPCMDFLAGS* pFlags);
	IFACEMETHODIMP GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon) override;
	IFACEMETHODIMP GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip) override;
	IFACEMETHODIMP GetCanonicalName(GUID* pguidCommandName) override;
	IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand** ppEnum) override;
	IFACEMETHODIMP Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc);
};

class __declspec(uuid("3B39470F-14F1-46BB-8DDA-0EBFCC8C7360"))
CExplorerCommandVerb2: public IExplorerCommand
{
private:
	long	_cRef;
	CExplorerCommandVerb& Parent;
public:
	CExplorerCommandVerb2(CExplorerCommandVerb& theParent);
	~CExplorerCommandVerb2();
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	IFACEMETHODIMP_(ULONG) AddRef() override;
	IFACEMETHODIMP_(ULONG) Release() override;
	// IExplorerCommand
	IFACEMETHODIMP GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState);
	IFACEMETHODIMP GetTitle(IShellItemArray* psia, LPWSTR* ppszName);
	IFACEMETHODIMP GetFlags(EXPCMDFLAGS* pFlags);
	IFACEMETHODIMP GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon) override;
	IFACEMETHODIMP GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip) override;
	IFACEMETHODIMP GetCanonicalName(GUID* pguidCommandName) override;
	IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand** ppEnum) override;
	IFACEMETHODIMP Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc);
};

class __declspec(uuid("CC19E147-7757-483C-B27F-3D81BCEB38FE")) CExplorerCommandVerb :
	public IExplorerCommand, 
	public IObjectWithSite, 
	public IEnumExplorerCommand
{
private:	
	long		_cRef;
	IUnknown*	_punkSite;
	int			_index;
	WCHAR		_selection[128];
	CExplorerCommandVerb1* TestCmd1;
	CExplorerCommandVerb2* TestCmd2;
public:
	CExplorerCommandVerb();
	~CExplorerCommandVerb();
	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv) override;
	IFACEMETHODIMP_(ULONG) AddRef() override;
	IFACEMETHODIMP_(ULONG) Release() override;
	// IExplorerCommand
	IFACEMETHODIMP GetTitle(IShellItemArray* psia, LPWSTR* ppszName) override;
	IFACEMETHODIMP GetState(IShellItemArray* psia, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) override;
	IFACEMETHODIMP GetFlags(EXPCMDFLAGS* pFlags) override;
	IFACEMETHODIMP Invoke(IShellItemArray* psia, IBindCtx* pbc) override;
	IFACEMETHODIMP GetIcon(IShellItemArray* psia, LPWSTR* ppszIcon) override;
	IFACEMETHODIMP GetToolTip(IShellItemArray* psia, LPWSTR* ppszInfotip) override;
	IFACEMETHODIMP GetCanonicalName(GUID* pguidCommandName) override;
	IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand** ppEnum) override;
	// IEnumExplorerCommand
	IFACEMETHODIMP Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched) override;
	IFACEMETHODIMP Skip(ULONG celt) override;
	IFACEMETHODIMP Reset() override;
	IFACEMETHODIMP Clone(IEnumExplorerCommand** ppEnum) override;
	// IObjectWithSite
	IFACEMETHODIMP SetSite(IUnknown* punkSite) override;
	IFACEMETHODIMP GetSite(REFIID riid, void** ppv) override;

	HWND GetWndHandle() const;
	WCHAR* GetSelection() const;
};
