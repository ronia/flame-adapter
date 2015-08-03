#include "stdafx.h"
#include "Console.h"

#include "../../interfaces/Gme.h"

namespace GMEConsole
{
	CComPtr<IGMEOLEApp> Console::gmeoleapp = 0;

	void Console::SetupConsole(CComPtr<IMgaProject> project)
	{
		CComPtr<IMgaClient> client;	
		CComQIPtr<IDispatch> pDispatch;
		HRESULT s1 = project->GetClientByName(L"GME.Application", &client);

		if ((SUCCEEDED(s1)) && (client != 0))
		{
			HRESULT s2 = client->get_OLEServer(&pDispatch);
			if ((SUCCEEDED(s2)) && (pDispatch != 0) && gmeoleapp == 0)
			{
				COMTHROW(pDispatch->QueryInterface(&gmeoleapp));
			}
		}
	}
	void Console::ReleaseConsole() {
		if (gmeoleapp)
			gmeoleapp.Release();
	}

	void Console::WriteLine(const CString& message, msgtype_enum type)
	{
		if (gmeoleapp == 0) {
			switch (type) {
			case MSG_NORMAL:
			case MSG_INFO:
			case MSG_WARNING:
				_tprintf(_T("%s\n"), message);
				break;
			case MSG_ERROR:
				_ftprintf(stderr, _T("%s\n"), message);
				break;
			}
		}
		else {
			COMTHROW(gmeoleapp->ConsoleMessage( CComBSTR(message.GetLength(),message),type));
		}
	}

	void Console::Clear()
	{
		if (gmeoleapp != 0) {
			CComBSTR empty(L"");
			COMTHROW(gmeoleapp->put_ConsoleContents(empty));
		}
	}

	void Console::SetContents(const CString& contents)
	{
		if (gmeoleapp != 0) {
			COMTHROW(gmeoleapp->put_ConsoleContents( CComBSTR(contents.GetLength(),contents)));
		}
	}

	void Console::NavigateTo(const CString& url)
	{
		if (gmeoleapp != 0) {
			COMTHROW(gmeoleapp->ConsoleNavigateTo(CComBSTR(url.GetLength(), url)));
		}
	}

	void Console::Error::WriteLine(const CString& message)
	{
		Console::WriteLine(message,MSG_ERROR);
	}

	void Console::Out::WriteLine(const CString& message)
	{
		Console::WriteLine(message, MSG_NORMAL);
	}
	void Console::Warning::WriteLine(const CString& message)
	{
		Console::WriteLine(message, MSG_WARNING);
	}
	void Console::Info::writeLine(const CString& message)
	{
		Console::WriteLine(message,MSG_INFO);
	}
	void Console::Info::WriteLine(const CString& message)
	{
		Console::WriteLine(message,MSG_INFO);
	}
}


