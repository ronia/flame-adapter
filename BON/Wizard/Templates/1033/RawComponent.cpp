///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include <ComHelp.h>
#include <GMECOM.h>
#include <Console.h>
#include "ComponentConfig.h"
#ifndef __INTELLISENSE__
#import "progid:Mga.MgaMetaFolder" no_implementation auto_search no_namespace no_search_namespace
#import "progid:Mga.MgaFolders" no_implementation auto_search no_namespace no_search_namespace
#import "progid:Mga.MgaMetaFolder" implementation_only auto_search no_namespace no_search_namespace
#import "progid:Mga.MgaFolders" implementation_only auto_search no_namespace no_search_namespace
#else
// IntelliSense has a known issue with the above lines.
//  http://connect.microsoft.com/VisualStudio/feedback/details/533526/vc-2010-intellisense-import-directive-using-libid-does-not-work
#ifdef _DEBUG
// If IntelliSense reports "Cannot open source file", compile then reopen the project
#include "Debug\Meta.tlh"
#include "Debug\Mga.tlh"
#else
#include "Release\Meta.tlh"
#include "Release\Mga.tlh"
#endif
#endif

#include "RawComponent.h"

// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
	return S_OK;
}

// this is the obsolete component interface
// this present implementation either tries to call InvokeEx, or returns an error;
STDMETHODIMP RawComponent::Invoke(IMgaProject* gme, IMgaFCOs *models, long param) {
#ifdef SUPPORT_OLD_INVOKE
	CComPtr<IMgaFCO> focus;
	CComVariant parval = param;
	return InvokeEx(gme, focus, selected, parvar);
#else
	if(interactive) {
		AfxMessageBox(_T("This component does not support the obsolete invoke mechanism"));
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}


// This is the main component method for interpereters and plugins. 
// May also be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) {
	COMTRY {
		_bstr_t focusname = "<nothing>";
		IMgaTerritoryPtr terr;
		project->CreateTerritory(NULL, &terr, NULL);
		project->BeginTransaction(terr, TRANSACTION_GENERAL);
		try {
			if (currentobj)
				focusname = currentobj->Name;
			using namespace GMEConsole;
			Console::Out::WriteLine("Interpreter started...");
			AfxMessageBox(_T("RAW Com Component --- Plugin!!!! Sample (project: ") + project->Name +
						_T(", focus: ") + focusname);
			project->CommitTransaction();
			Console::Out::WriteLine(_T("Interpreter completed..."));
		}	catch(...) { project->AbortTransaction(); throw; }
	} COMCATCH(;);
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox(_T("The ObjectsInvoke method is not implemented"));
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	return S_OK;
}


#ifdef GME_ADDON

// these two functions are the main 
STDMETHODIMP RawComponent::GlobalEvent(globalevent_enum event) { 
	if(event == GLOBALEVENT_UNDO) {
		AfxMessageBox(_T("Undo));
	}
	return S_OK; 
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	if(eventmask & OBJEVENT_CREATED) {
		AfxMessageBox(_T("Object created: ObjID=") + obj->ID); 
	}		
	return S_OK;
}

#endif
