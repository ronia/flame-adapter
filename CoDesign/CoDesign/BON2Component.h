//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.h
//
//###############################################################################################################################################

/*
	Copyright (c) Vanderbilt University, 2000-2004
	ALL RIGHTS RESERVED

	Vanderbilt University disclaims all warranties with regard to this
	software, including all implied warranties of merchantability
	and fitness.  In no event shall Vanderbilt University be liable for
	any special, indirect or consequential damages or any damages
	whatsoever resulting from loss of use, data or profits, whether
	in an action of contract, negligence or other tortious action,
	arising out of or in connection with the use or performance of
	this software.
*/

#ifndef BON2Component_h
#define BON2Component_h

#include	"BON.h"
#include	"BONImpl.h"
#include	"sock.h"
#include	"CoDesign.h"
#include	<ComponentConfig.h>


/*
	This is the port number of PortNumberTracker. CoDesign Adaptor first 
	connects to this port to get the port number of the CoWare Component
	(either CoWareXTEAMClient or XTEAMEngine) to which it connects.
*/
#define		TRACKER_PORT	52530

/*
	CoDesign Adaptor connects to two components: (1) PortNumberTracker 
	first to get the port number of the CoWare component to which it 
	connects.

	Upon connecting, in case the connection fails, CoDesign Adaptor
	tries multiple times with a certain amount of delay in between.

	CONN_TRY is the number if trials it attemps, and CONN_DELAY_SEC
	is the delay in seconds it waits between the attempts.
*/


namespace BON
{

//###############################################################################################################################################
//
// 	C L A S S : BON::Component
//
//###############################################################################################################################################

class Component
{
	//==============================================================
	// IMPLEMENTOR SPECIFIC PART
	// Insert application specific members and method deifinitions here

	public:

		//////////////////////////////////////////////////////
		// iterative connection function
		//////////////////////////////////////////////////////
		openutils::CSocket* iterativeConnect(	const char* host_name,
												int port, 
												std::string &msg);

		//////////////////////////////////////////////////////
		// handleEvents() pulls an event from the event queue 
		// and applies it to the model until no event is left
		//////////////////////////////////////////////////////
		void handleEvents();

		//////////////////////////////////////////////////////
		// excuteModel() handles the process of executing model
		//////////////////////////////////////////////////////
		std::string executeModel();

		//////////////////////////////////////////////////////
		// invokeInterpreter() invokes the target interpreter
		// (e.g. PowerxADL)
		//////////////////////////////////////////////////////
		std::string invokeInterpreter(Project& project, FCO& currentFCO);

		//////////////////////////////////////////////////////
		// pass the COM interface handle to invoke the model  
		// update after application of new events
		//////////////////////////////////////////////////////
		void setCOMIntf(IMgaComponentEx * intf) { m_pCOMIntf = intf; };
		
		// a WINSOCK socket that connects CoDesign Adaptor and CoWare Client
		openutils::CSocket			*m_pClientSocket;

		// a WINSOCK socket that connects CoDesign Adaptor and PortNumberTracker
		openutils::CSocket			*m_pPNTSocket;

		// the COM interface
		CComPtr<IMgaComponentEx>	m_pCOMIntf;

		// a queue that stores all incoming events
		EventQueue					*m_pEventQueue;

		// a data structure that temporarily stores two end objects of a new connection
		ConnectionInfo				*m_pConnectionInfo;

		// a data structure that temporarily stores two end objects of a removed connection
		ConnectionInfo				*m_pDisconnectionInfo;

		// a data structure that temporarily stores the set included information
		SetInfo						*m_pSetInfo;

		// a data structure that temporarily stores the reference information
		ReferenceInfo				*m_pReferenceInfo;

		// CoDesign Adaptor switch
		bool						m_bCoDesignAdaptorSwitch;

		// XTEAM Engine flag -- true if it is connected to an XTEAMEngine
		bool						m_bIsXTEAMEngine;
		
		//////////////////////////////////////////////////////
		// 
		/*

			When a Connection is created, three ObjectEvents are called.
				(1) CREATED for the connection
				(2) CONNECTED for one side of the connection
				(3) CONNECTED for the other side of the connection

			Since the process is not a single process, and the ObjectEvent handling 
			function has to be invoked multiple times, there is possibility that this
			process can be obstructed by another Connection creation process.

			Below are the special mutex for the Connection creation. It is locked when
			the first CREATED for a Connection is called, and it is unlocked when the
			last CONNECTED is called and the Connection event Message is completely formed.

		*/
		// 
		//////////////////////////////////////////////////////

		// CONNECTED MUTEX ahoy!
		HANDLE hConnectedMutex;

		// Get the mutex
		void get_connected_mutex();

		// Lose the mutex
		void lose_connected_mutex();

		// DISCONNECTED MUTEX ahoy!
		HANDLE hDisconnectedMutex;

		// Get the mutex
		void get_disconnected_mutex();

		// Lose the mutex
		void lose_disconnected_mutex();

		// SET MUTEX ahoy!
		HANDLE hSetMutex;

		// Get the mutex
		void get_set_mutex();
	
		// Lose the mutex
		void lose_set_mutex();

		// REFERECE MUTEX ahoy!
		HANDLE hReferenceMutex;

		// Get the mutex
		void get_reference_mutex();

		// Lose the mutex
		void lose_reference_mutex();


	//==============================================================
	// BON2 SPECIFIC PART
	// Do not modify anything below

	// Member variables
	public :
		Project 	m_project;
		bool		m_bIsInteractive;

	public:
		Component();
		~Component();

	public:
		void initialize( Project& project );
		void finalize( Project& project );
		void invoke( Project& project, const std::set<FCO>& setModels, long lParam );
		void invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam );
		void objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam );
		Util::Variant getParameter( const std::string& strName );
		void setParameter( const std::string& strName, const Util::Variant& varValue );

	#ifdef GME_ADDON
		void globalEventPerformed( globalevent_enum event );
		void objectEventPerformed( Object& object, unsigned long event, VARIANT v );
	#endif
};

// custom message reception thread
UINT inboxThread(LPVOID n);

// pointers to pass to the custom message reception thread
typedef struct attrPkg {
	openutils::CSocket		*socket;
	HWND					hwnd;
	Component				*bon;
	IMgaComponentEx			*intf;
} ATTRPKG;


}; // namespace BON

#endif // Bon2Component_H