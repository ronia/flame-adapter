#pragma once

#include <afxtempl.h>
#include <unknwn.h>
#include <atlbase.h>
#include <comdef.h>

// --------------------------- MACROS

inline void COMVERIFY(HRESULT hr)
{
	VERIFY(SUCCEEDED(hr));
}

inline void COMASSERT(HRESULT hr)
{
	ASSERT(SUCCEEDED(hr));
}

// --------------------------- CBstr

class CBstr
{
public:
	CBstr() : s(NULL) { }
	CBstr(const CString &str) : s(str.AllocSysString()) { }
	~CBstr() { Free(); }

	void operator = (const CString &str) { Free(); s = str.AllocSysString(); }

	void Free() { SysFreeString(s); }

	BSTR Send() { return s; }
	operator BSTR* () { return &s; }
	operator CString () { return s; }

private:
	BSTR s;
};

class CBstrRef
{
public:
	CBstrRef(BSTR str) : s(str) { }

	void operator = (const CString& str) { Free(); s = str.AllocSysString(); }

	void Free() { SysFreeString(s); }

	operator BSTR () { return s; }
	operator CString () { return s; }

private:
	BSTR &s;
};

class CBstrIn
{
public:
	CBstrIn(const CString &str) : s(str.AllocSysString()) { }
	~CBstrIn() { SysFreeString(s); }

	operator BSTR () { return s; }
	operator _bstr_t() { return _bstr_t(s); }

private:
	BSTR s;
};

