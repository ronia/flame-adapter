//###############################################################################################################################################
//
//	Meta and Builder Object Network V2.0 for GME
//	BON2Component.cpp
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

#include "stdafx.h"
#include <Console.h>
#include "BON2Component.h"

#include "iframe/XTEAM_IFrame.h"
#include "AssortedxADL/AssortedxADL_SimGen.h"

#include <direct.h>

#include <iomanip>


// connect() retry times
#define		CONN_TRY		5

// connect() retry delay time
#define		CONN_DELAY_SEC	1

namespace BON
{

//###############################################################################################################################################
//
// 	C L A S S : BON::Component
//
//###############################################################################################################################################

Component::Component()
	: m_bIsInteractive( false )
{
	// NULLify the socket
	m_pClientSocket				= NULL;

	// initialize event queues
	m_pEventQueue				= new EventQueue();

	// initialize the Connected mutex
	hConnectedMutex				= CreateMutex(NULL, FALSE, NULL);

	// initialize the Disconnected mutex
	hDisconnectedMutex			= CreateMutex(NULL, FALSE, NULL);
	
	// initialize the Set mutex
	hSetMutex					= CreateMutex(NULL, FALSE, NULL);

	// initialize the Reference mutex
	hReferenceMutex				= CreateMutex(NULL, FALSE, NULL);

	// initialize the connection/disconnection info structure
	m_pConnectionInfo			= NULL;
	m_pDisconnectionInfo		= NULL;
	m_pSetInfo					= NULL;

	// CoDesign Adaptor Switch
	m_bCoDesignAdaptorSwitch	= false;

	// XTEAM Engine flag
	m_bIsXTEAMEngine			= false;
}

Component::~Component()
{
	// close the socket
	if(m_pClientSocket)
		m_pClientSocket->Close();

	// delete the queues
	if (m_pEventQueue)
		delete m_pEventQueue;

	// close the Connected mutex handle
	CloseHandle(hConnectedMutex);

	// close the Disconnected mutex handle
	CloseHandle(hDisconnectedMutex);

	// close the Set mutex
	CloseHandle(hSetMutex);

	// close the Reference mutex
	CloseHandle(hReferenceMutex);

	// BON Default destruction code
	if ( m_project ) {
		m_project->finalizeObjects();
		finalize( m_project );
		m_project = NULL;
	}
}

// ====================================================
// Mutex getters and losers

// CONNECTED Mutex
void Component::get_connected_mutex() {
	WaitForSingleObject(hConnectedMutex, INFINITE);
}

void Component::lose_connected_mutex() {
	ReleaseMutex(hConnectedMutex);
}

// DISCONNECTED Mutex
void Component::get_disconnected_mutex() {
	WaitForSingleObject(hDisconnectedMutex, INFINITE);
}

void Component::lose_disconnected_mutex() {
	ReleaseMutex(hDisconnectedMutex);
}

// SETINCLUDED Mutex
void Component::get_set_mutex() {
	WaitForSingleObject(hSetMutex, INFINITE);
}

void Component::lose_set_mutex() {
	ReleaseMutex(hSetMutex);
}

// REFERENCED Mutex
void Component::get_reference_mutex() {
	WaitForSingleObject(hReferenceMutex, INFINITE);
}

void Component::lose_reference_mutex() {
	ReleaseMutex(hReferenceMutex);
}

// ====================================================
// This method is called after all the generic initialization is done
// This should be empty unless application-specific initialization is needed

void Component::initialize( Project& project )
{
	// ======================
	// Insert application specific code here
#ifdef GME_ADDON
	project->setAutoCommit( false);
#endif

	// initializes winsock
	openutils::CWinSock::Initialize();

	int nCoWarePort	= 0;
	char *buffer	= new char[MSG_BUFF_SZ];
	memset(buffer, NULL, MSG_BUFF_SZ);

	//////////////////////////////////
	// connects to PortNumberTracker
	//////////////////////////////////

	// the return message
	string	msg_PNT;

	// connects!
	m_pPNTSocket = iterativeConnect("127.0.0.1", TRACKER_PORT, msg_PNT);
	
	// if the connection was successfully established
	if(m_pPNTSocket != NULL) {
		try {
			// sends a port number request "REQUEST"
			m_pPNTSocket->Send("REQUEST");

			// receives the port number (comes in as a stringified decimal)
			if (m_pPNTSocket->Read(buffer, (MSG_BUFF_SZ-1)) != -1) {
				nCoWarePort = atoi(buffer);
			} else {
				m_pPNTSocket->Close();
			}

			// sends an ACK "ACK"
			m_pPNTSocket->Send("ACK");
		} catch (openutils::CSocketException *cse) {
			// switches off the synchronization
			m_bCoDesignAdaptorSwitch = false;

			// prints out an error message
			std::stringstream ss;
			ss << "Communication with PortNumberTracker failed:\n  " << cse->GetMessageA() << "\nCoDesign synchronization stops.";
			AfxMessageBox(ss.str().c_str());

			return;
		}
	}
	// if the connection was failed
	else {
		// switches off the synchronization
		m_bCoDesignAdaptorSwitch = false;

		// prints out an error message
		std::stringstream ss;
		ss << "Connection to PortNumberTracker failed:\n  " << msg_PNT << "\nCoDesign synchronization stops.";
		AfxMessageBox(ss.str().c_str());

		return;
	}

	/////////////////////////////////////////
	// begins connection to CoWare
	/////////////////////////////////////////
	
	// the return message
	string	msg_CoWare;

	// connects!
	m_pClientSocket = iterativeConnect("127.0.0.1", nCoWarePort, msg_CoWare);

	// if the connection was successfully established
	if(m_pClientSocket != NULL) {
		// creates an attributes package for the main iteration thread
		ATTRPKG *attr	= new ATTRPKG;
		attr->intf		= m_pCOMIntf;
		attr->socket	= m_pClientSocket;
		attr->bon		= this;
		
		// switches on the synchronization
		m_bCoDesignAdaptorSwitch = true;

		// begins main iteration thread
		AfxBeginThread(inboxThread, attr, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	}
	// if the connection was failed
	else {
		// switches off the synchronization
		m_bCoDesignAdaptorSwitch = false;

		// prints out an error message
		std::stringstream ss;
		ss << "Connection to CoWare failed:\n  " << msg_CoWare << "\nCoDesign synchronization stops.";
		AfxMessageBox(ss.str().c_str());

		return;
	}
}

// iteratively tries to connect
openutils::CSocket* Component::iterativeConnect(const char* host_name, int port, std::string &msg) {
	
	// new socket initialization
	openutils::CSocket *pSocket = new openutils::CSocket();

	int		ret;							// the return value
	bool	isConnected			= false;	// connection flag
	
	// iterates until the connection succeeds
	for (int i=1; i <= CONN_TRY; i++) {
		// tries connection
		if((ret = pSocket->Connect(host_name, port, msg)) == SOCKET_OP_SUCCESS) {
			// sets the connected flag as true
			isConnected = true;
			break;
		} 
			
		// closes the socket
		pSocket->Close();
		
		// delays before it retries to reconnect
		if(i != CONN_TRY) {
			Sleep (100 * CONN_DELAY_SEC);
		}
	}

	// returns the socket only if the connection has been successfully established
	if (isConnected) {
		return pSocket;
	} else {
		delete pSocket;
		return NULL;
	}
}

// ====================================================
// This method is called before the whole BON2 project released and disposed
// This should be empty unless application-specific finalization is needed

void Component::finalize( Project& project )
{
	// ======================
	// Insert application specific code here

	// close and destroy the socket
	if(m_pClientSocket != NULL)
		m_pClientSocket->Close();

	openutils::CWinSock::Finalize();

	if(m_pClientSocket)
		delete m_pClientSocket;

	// delete the queues
	if (m_pEventQueue)
		delete m_pEventQueue;

	// close the Connected mutex handle
	CloseHandle(hConnectedMutex);

	// close the Disconnected mutex handle
	CloseHandle(hDisconnectedMutex);

	AfxMessageBox("CoDesign is turned off.");
}

// ====================================================
// This is the obsolete component interface
// This present implementation either tries to call InvokeEx, or does nothing except of a notification

void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
{
	#ifdef SUPPORT_OLD_INVOKE
		Object focus;
		invokeEx( project, focus, setModels, lParam );
	#else
		if ( m_bIsInteractive )
			AfxMessageBox(_T("This BON2 Component does not support the obsolete invoke mechanism!"));
	#endif
}

// ====================================================
// This is the main component method for Interpereters and Plugins.
// May also be used in case of invokeable Add-Ons

void Component::invokeEx( Project& project, FCO& currentFCO, const std::set<FCO>& setSelectedFCOs, long lParam )
{
#ifdef GME_ADDON
	project->setAutoCommit( false);
#endif
	using namespace GMEConsole;
	//Console::Out::WriteLine("Interpreter started...");
	// ======================
	// TODO: Insert application specific code here

	// apply all the events in the event queue to the local model
	handleEvents();


	//Console::Out::WriteLine("Interpreter completed...");
}

// ====================================================
// GME currently does not use this function
// You only need to implement it if other invokation mechanisms are used

void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
	{
		if ( m_bIsInteractive )
			AfxMessageBox(_T("This BON2 Component does not support objectInvokeEx method!"));
	}

// ====================================================
// Implement application specific parameter-mechanism in these functions

Util::Variant Component::getParameter( const std::string& strName )
{
	// ======================
	// Insert application specific code here

	return Util::Variant();
}

void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
{
	// ======================
	// Insert application specific code here
}

//////////////////////////////////////////////////////
// handleEvents() pulls an event from the event queue 
// and applies it to the model until no event is left
//////////////////////////////////////////////////////

void Component::handleEvents()
{	
	// if the switch is off, do nothing
	if(!m_bCoDesignAdaptorSwitch)
		return;

	// Get the queue mutex, blocked while waiting for the condition signal
	m_pEventQueue->get_mutex();

	while(true)
	{
		// Take the first event in the event queue and parse the string
		EventMessage *head = m_pEventQueue->get_head();

		// if the queue is empty, break out of the iteration
		if(head == NULL)
			break;

		std::string type = head->get_event_type();

		// Apply the event to the local model
		if( head->get_event_type() == "CREATED" ) {

			// CREATED can be either an object or a Connection
			
			// if it is a Connection
			if (head->get_object_info().substr(0, head->get_object_info().find_first_of(' ')) == "MON::Connection")
			{
				// create connection
				if(!m_project->create_connection(	head->get_GUID_1(),
													head->get_GUID_2(),
													head->get_GUID_3(),
													head->get_GUID_4(),
													head->get_object_type(),
													head->get_object_info(),
													head->get_connection_GUID1_1(),
													head->get_connection_GUID1_2(),
													head->get_connection_GUID1_3(),
													head->get_connection_GUID1_4(),
													head->get_connection_object_type1(),
													head->get_connection_object_info1(),
													head->get_connection_GUID2_1(),
													head->get_connection_GUID2_2(),
													head->get_connection_GUID2_3(),
													head->get_connection_GUID2_4(),
													head->get_connection_object_type2(),
													head->get_connection_object_info2(),
													head->get_parent_GUID_1(),
													head->get_parent_GUID_2(),
													head->get_parent_GUID_3(),
													head->get_parent_GUID_4(),
													head->get_parent_type(),
													head->get_parent_info())) 
				{
					AfxMessageBox("CoDesign Error: Cannot create a Connection");
				}
			}
			// if it's an object or a reference
			else
			{
				// create object
				if(!m_project->create_object(		head->get_GUID_1(),
													head->get_GUID_2(),
													head->get_GUID_3(),
													head->get_GUID_4(),
													head->get_object_type(),
													head->get_object_info(),
													head->get_parent_GUID_1(),
													head->get_parent_GUID_2(),
													head->get_parent_GUID_3(),
													head->get_parent_GUID_4(),
													head->get_parent_type(),
													head->get_parent_info()))
				{
					AfxMessageBox("CoDesign Error: Cannot create an Object");
				}

				// update the object's name
				if(!m_project->update_name(			head->get_GUID_1(),
													head->get_GUID_2(),
													head->get_GUID_3(),
													head->get_GUID_4(),	
													head->get_object_name(),
													head->get_object_type())) 
				{
					AfxMessageBox("CoDesign Error: Cannot update a name");
				}

				// update the object's location
				if(head->get_location_registry().size() != 0) 
				{
					m_project->update_location_registry(	head->get_GUID_1(),
															head->get_GUID_2(),
															head->get_GUID_3(),
															head->get_GUID_4(), 
															head->get_object_type(), 
															head->get_location_registry() );
				}

				// update additional attributes if it has any
				if(head->get_additional_attributes().size() != 0)
				{
					m_project->update_additional_attributes(	head->get_GUID_1(),
																head->get_GUID_2(),
																head->get_GUID_3(),
																head->get_GUID_4(), 
																head->get_object_type(),
																head->get_additional_attributes());
				}

				// if it is a reference, add the reference
				if (head->get_object_info().substr(0, head->get_object_info().find_first_of(' ')) == "MON::Reference")
				{
					m_project->add_ref(			head->get_GUID_1(),
												head->get_GUID_2(),
												head->get_GUID_3(),
												head->get_GUID_4(), 
												head->get_object_type(), 
												head->get_object_info(),
												head->get_ref_GUID_1(),
												head->get_ref_GUID_2(),
												head->get_ref_GUID_3(),
												head->get_ref_GUID_4(),
												head->get_ref_object_type(),
												head->get_ref_object_info());
				}

			}
			

		} else if( head->get_event_type() == "XTEAM" ) {
			m_bIsXTEAMEngine = true;
			
		} else if( head->get_event_type() == "Snapshot" ) {
			// do nothing
		} else if( head->get_event_type() == "DESTROYED" ) {
			m_project->destroy_object(					head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(),
														head->get_object_name(),
														head->get_object_type(),
														head->get_object_info());
		} else if( head->get_event_type() == "REGISTRY") {
			if(	head->get_object_info().substr(0, head->get_object_info().find_first_of(' ')) != "MON::Connection" ) {
				// if it is a non-connection element
				m_project->update_location_registry(	head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(), 
														head->get_object_type(), 
														head->get_location_registry() );
			} else {
				// if it is a Connection
				m_project->update_location(				head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(), 
														head->get_object_type(),
														head->get_position_string() );
			}
		} else if( head->get_event_type() == "ATTR") {
			m_project->update_additional_attributes(	head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(), 
														head->get_object_type(),
														head->get_additional_attributes());
		} else if( head->get_event_type() == "PROPERTIES" ) {
			m_project->update_name(						head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(),
														head->get_object_name(),
														head->get_object_type());
		} else if( head->get_event_type() == "SETINCLUDED" ) {
			m_project->add_set_member(					head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(),
														head->get_object_type(),
														head->get_object_info(),
														head->get_new_member_GUID_1(),
														head->get_new_member_GUID_2(),
														head->get_new_member_GUID_3(),
														head->get_new_member_GUID_4(),
														head->get_new_member_object_type(),
														head->get_new_member_object_info());
											

		} else if( head->get_event_type() == "SETEXCLUDED" ) {
			m_project->remove_set_member(				head->get_GUID_1(),
														head->get_GUID_2(),
														head->get_GUID_3(),
														head->get_GUID_4(),
														head->get_object_type(),
														head->get_object_info(),
														head->get_new_member_GUID_1(),
														head->get_new_member_GUID_2(),
														head->get_new_member_GUID_3(),
														head->get_new_member_GUID_4(),
														head->get_new_member_object_type(),
														head->get_new_member_object_info());

		} else {
			//std::string msg = "CoDesign Adaptor is not supposed to receive: " + std::string(head->get_event_type());
			//AfxMessageBox(msg.c_str());
		}

		
		// Runs the XTEAM execution only when the switch is on
		if(head->isXTEAMAnalysisOn())
		{
			// execute model to compute the energy threshold
			std::string simDirectory = executeModel();

			// get the current working directory
			TCHAR cwd[2000];
			GetCurrentDirectory(2000, cwd);

			// manipulate the Event value
			stringstream ss;
			ss << "X"	<< DELIMITER_ATTR << "-1" 
						<< DELIMITER_ATTR << "XTEAM" 
						<< DELIMITER_ATTR << cwd << "\\" << simDirectory 
						<< DELIMITER_ATTR 
						<< DELIMITER_ATTR 
						<< DELIMITER_ATTR;
			m_pClientSocket->Send(ss.str().c_str());
		}
	}
	
	// lose mutex
	m_pEventQueue->lose_mutex();
}



std::string Component::executeModel()
{
	// find the model and invoke the interpreter with the model
	std::set<BON::Object> objs;
	
	if(!m_project->get_models(objs))
	{
		AfxMessageBox("There is no Model in the project. Unable to run XTEAM interpreter.");
	}
	else
	{
		//AfxMessageBox("There is one or more Model in the project. Launching the XTEAM interpreter.");

		std::set<BON::Object>::iterator it;

		int cnt = 1;

		for(it = objs.begin(); it != objs.end(); it ++) {	
			// check if it's a model
			BON::Model model (*it);
			if(model)
			{
				if(objs.size() > 1)
				{
					std::stringstream ss;
					ss << "Running interpreter for [" << model->getName() << "] [" << cnt++ << "/" << objs.size() << "]";
					AfxMessageBox(ss.str().c_str());
				}

				// invoke the target interpreter
				return invokeInterpreter(m_project, model);
			}
		}
	}

	return NULL;
}

std::string Component::invokeInterpreter(Project& project, FCO& currentFCO)
{
	
	time_t timer;
	struct tm * timeinfo;

	// have the version name
	time(&timer);

	// convert the time to human readable format
	timeinfo = localtime (&timer);
	
	stringstream ss;
	ss	<< timeinfo->tm_year + 1900 
		<< std::setfill ('0') << std::setw (2) << timeinfo->tm_mon+1 
		<< std::setfill ('0') << std::setw (2) << timeinfo->tm_mday << "_" 
		<< std::setfill ('0') << std::setw (2) << timeinfo->tm_hour 
		<< std::setfill ('0') << std::setw (2) << timeinfo->tm_min << "_" 
		<< std::setfill ('0') << std::setw (2) << timeinfo->tm_sec;

	string version = ss.str();

	bool version_directory_error = true;
	bool error = true;
	std::string directory_name = version;

	// create a directory for this version -- if exists, put a number at the end
	for(int i=0; i < 100; i++) {
		if(_mkdir(directory_name.c_str()) == 0) {
			version_directory_error = false;
			break;
		} else {
			std::stringstream ss_dn;
			ss_dn << version << "_" << std::setfill ('0') << std::setw (2) << i;
			directory_name = ss_dn.str();
		}
	}

	// only if the version directory was successfully created, create the following paths
	if(version_directory_error == false) {
		if(_mkdir((directory_name + "\\XTEAM_Simulation").c_str()) == 0) {
			if(_mkdir((directory_name + SIMCODEPATH).c_str()) == 0) {
				// error flag be set as false only if all directory creation succeeds
				error = false;
			}
		}
	}

	// if there was any error, stop the simulation generation
	if (error) {
		stringstream ssErr;
		ssErr << "Directory for simulation version [" << directory_name << "] cannot be created.\nStopping generating the simulation code ...";
		AfxMessageBox(ssErr.str().c_str());
		return NULL;
	}

	XTEAM::XTEAM_Assorted_Sim_Generator * sim_generator = new XTEAM::XTEAM_Assorted_Sim_Generator (directory_name, directory_name + SIMCODEPATH);
	XTEAM::XTEAM_IFrame::sim_gen (sim_generator);
	XTEAM::XTEAM_IFrame::generate (project, currentFCO, directory_name, 2500);
	sim_generator->write_warning_messages();
	
	delete sim_generator;

	return directory_name;
}

#ifdef GME_ADDON

// ====================================================
// If the component is an Add-On, then this method is called for every Global Event

void Component::globalEventPerformed( globalevent_enum event )
{
	// ======================
	// Insert application specific code here
}

// ====================================================
// If the component is an Add-On, then this method is called for every Object Event

void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
{
	// ======================
	// Insert application specific code here

	// if the switch is off, do nothing
	if(!m_bCoDesignAdaptorSwitch)
		return;

	if(object)
		object->getProject()->setAutoCommit(false);

	// event type variable
	std::string eventType;

	// translate a GME object event into a CoDesign event
	if ( event & OBJEVENT_CREATED )
		eventType = "CREATED";
	else if ( event & OBJEVENT_ATTR )
		eventType = "ATTR";
	else if ( event & OBJEVENT_REGISTRY ) 
		eventType = "REGISTRY";
	else if ( event & OBJEVENT_RELATION ) 
		eventType = "RELATION";
	else if ( event & OBJEVENT_PROPERTIES ) 
		eventType = "PROPERTIES";
	else if ( event & OBJEVENT_SUBT_INST ) 
		eventType = "SUBT_INST";
	else if ( event & OBJEVENT_PARENT ) 
		eventType = "PARENT";
	else if ( event & OBJEVENT_LOSTCHILD ) 
		return;
	else if ( event & OBJEVENT_REFERENCED ) 
		eventType = "REFERENCED";
	else if ( event & OBJEVENT_CONNECTED ) 
		eventType = "CONNECTED";
	else if ( event & OBJEVENT_SETINCLUDED ) 
		eventType = "SETINCLUDED";
	else if ( event & OBJEVENT_REFRELEASED ) 
		return;
	else if ( event & OBJEVENT_DISCONNECTED ) 
		return;
	else if ( event & OBJEVENT_SETEXCLUDED ) 
		eventType = "SETEXCLUDED";
	else if ( event & OBJEVENT_MARKEDRO ) 
		eventType = "MARKEDRO";
	else if ( event & OBJEVENT_MARKEDRW ) 
		eventType = "MARKEDRW";
	else if ( event & OBJEVENT_OPENMODEL ) 
		eventType = "OPENMODEL";
	else if ( event & OBJEVENT_SELECT ) 
		eventType = "SELECT";
	else if ( event & OBJEVENT_DESELECT ) 
		eventType = "DESELECT";
	else if ( event & OBJEVENT_MOUSEOVER ) 
		eventType = "MOUSEOVER";
	else if ( event & OBJEVENT_CLOSEMODEL ) 
		eventType = "CLOSEMODEL";
	else if ( event & OBJEVENT_PRE_DESTROYED )
		eventType = "DESTROYED";
	else if ( event & OBJEVENT_DESTROYED ) 
		return;											
	else if ( event * OBJEVENT_NEWCHILD ) 
		return;												
	else {
		// it is impossible that GME gives an event type 
		// that is not specified above, but just in case ...

		stringstream ss;
		ss << "Unknown event type: " << hex << event;

		// pop up a message box for a critical error
		AfxMessageBox(ss.str().c_str());
		return;
	}

	//////////////////////////////////////////////////////
	// if the event was a SETINCLUDED or SETEXCLUDED
	//////////////////////////////////////////////////////

	if ( eventType == "SETINCLUDED" || eventType == "SETEXCLUDED") {
		// get the set mutex
		get_set_mutex();

		// get the GUID of the end object
		long guid[4];
		object->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);

		/*
			###################### before 10/29/2013
			As both the SETINCLUDED and SETEXCLUDED event comes along with RELATION event,
			the SETINCLUDED event has to be stored and wait for the following RELATION event.
			The mutex should be preventing two or more SETINCLUDED events happening at the same time,
			but just in case ...

			###################### after 10/29/2013
			If the user do set operations (SETINCLUDED or SETEXCLUDED) in a normal way, this would 
			work alright, but if the user removed a component that has been already included in a
			set or a set to which already has some elements assigned, it would fail.

			## Removing a Host that has elements
			DESTROYED
			SETEXCLUDED

			## Unassigning a Component from a Host
			SETEXCLUDED
			RELATION

			## Assigning a Component to a Host
			SETINCLUDED
			RELATION

			## Removing a Component that is assigned to a Host
			DESTROYED
			RELATION

			Just ignore the previous SETINCLUDED/SETEXCLUDED messages if a new one arrives.
		*/
		/*
		if(m_pSetInfo != NULL) { 
			std::stringstream ss;
			ss << "A new " << eventType << " message cannot be initiated while another ";
			if (m_pSetInfo->is_this_set_included())
				ss << "SETINCLUDED";
			else
				ss << "SETEXCLUDED";
			ss << " is being processed";
			AfxMessageBox(ss.str().c_str());
			return; 
		}
		*/

		if(m_pSetInfo != NULL) {
			delete m_pSetInfo;
		}

		// initiate a new Set inclusion/exclusion
		m_pSetInfo = new SetInfo();

		// set the SETINCLUDED flag; true if SETINCLUDED or false if SETEXCLUDED
		if(eventType == "SETINCLUDED")	{
			m_pSetInfo->this_is_set_included();
		} else {
			m_pSetInfo->this_is_set_excluded();
		}

		// if the add_object() returns -1, that means there is already a SET operation stored previously
		m_pSetInfo->add_object(		guid[0], guid[1], guid[2], guid[3],
									object->getObjectMeta().name(),
									object->getObjectMeta().infoString());

		return;
	}

	//////////////////////////////////////////////////////
	// if the event was a CONNECTED
	//////////////////////////////////////////////////////
	else if ( eventType == "CONNECTED") {
		// check if there is an open Connection
		if(m_pConnectionInfo) {

			// a connection consists of two end objects
			// get the GUID of the end object
			long guid[4];
			object->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);

			// add the end object to the ConnectionInfo
			int nEnds = m_pConnectionInfo->add_object(	guid[0], guid[1], guid[2], guid[3],
														object->getObjectMeta().name(),
														object->getObjectMeta().infoString());

			// if two objects are ready, send an event
			if (nEnds == 2) {
				// retrieve the original CREATED event of the connection
				EventMessage * evt = m_pConnectionInfo->get_event();

				if(evt)
				// set the two end objects of the connection
				evt->set_connection(	m_pConnectionInfo->get_GUID1_1(),
										m_pConnectionInfo->get_GUID1_2(),
										m_pConnectionInfo->get_GUID1_3(),
										m_pConnectionInfo->get_GUID1_4(),
										m_pConnectionInfo->get_connection_object_type1(),
										m_pConnectionInfo->get_connection_object_info1(),
										m_pConnectionInfo->get_GUID2_1(),
										m_pConnectionInfo->get_GUID2_2(),
										m_pConnectionInfo->get_GUID2_3(),
										m_pConnectionInfo->get_GUID2_4(),
										m_pConnectionInfo->get_connection_object_type2(),
										m_pConnectionInfo->get_connection_object_info2());

				// send it over
				if(m_pClientSocket != NULL)
					m_pClientSocket->Send(evt->create_payload().c_str());
				else
					AfxMessageBox("Socket connection to CoWareClient is unavailable");

				// free Message as it is no longer used
				delete evt;
				
				// lose the Connection mutex
				lose_connected_mutex();

				// free the memory -- somehow crashes -_-;
				delete m_pConnectionInfo;
				m_pConnectionInfo = NULL;

				return;
			}
		} else {
			AfxMessageBox("CONNECTED event is missing the prior CREATED event");
			return;
		}
	}

	
	//////////////////////////////////////////////////////
	// if the event was a REFERENCED
	//////////////////////////////////////////////////////

	else if ( eventType == "REFERENCED" ) { 
		// check if there is an open ReferenceInfo
		if( m_pReferenceInfo) {

			// get the GUID of the referencee object
			long guid[4];
			object->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);

			// retrieve the original CREATED event that has the referencer information
			EventMessage * evt = m_pReferenceInfo->get_event();

			// set the referencee information
			evt->set_ref(	guid[0], guid[1], guid[2], guid[3], 
							object->getObjectMeta().name(),
							object->getObjectMeta().infoString());

			// send it over
			if(m_pClientSocket != NULL)
				m_pClientSocket->Send(evt->create_payload().c_str());
			else
				AfxMessageBox("Socket connection to CoWareClient is unavailable");
				
			// free Message as it is no longer used
			delete evt;

			// lose the Reference mutex
			lose_reference_mutex();

			// free the memory
			delete m_pReferenceInfo;
			m_pReferenceInfo = NULL;

			return;
		} else {
			AfxMessageBox("REFERENCED event is missing the prior CREATED event");
			return;
		}
	}

	//////////////////////////////////////////////////////
	// if the event was 
	// CREATED, DESTROYED, PROPERTIES, REGISTRY, or ATTR
	//////////////////////////////////////////////////////

	else
	{
		/////////////////////////////////////
		// get the GUID of the object
		/////////////////////////////////////

		long guid[4];
		object->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);

		// Create a new event message
		EventMessage *msg = new EventMessage(	guid[0], guid[1], guid[2], guid[3],
												eventType,
												object->getName(),
												object->getObjectMeta().name(),
												object->getObjectMeta().infoString() );


		/////////////////////////////////////
		// If it's an Atom, a Set, a Reference, or a Model, retrieve location registry
		/////////////////////////////////////

		// if obj is an atom, atom will not be NULL
		// if obj is a set, setObj will not be NULL
		// if obj is a Connection, conn will not be NULL
		// if obj is a Model, model will not be NULL
		Atom atom (object);
		Set setObj (object);
		Connection conn (object);
		Model model (object);
		Reference ref (object);

		// the additional attributes set
		std::set<BON::Attribute> attr_set;
		if (atom){
			attr_set = atom->getAttributes();			// retrieve all additional attributes the atom has
		} else if (setObj) {
			attr_set = setObj->getAttributes();			// retrieve all additional attributes the set has
		} else if (model) {
			attr_set = model->getAttributes();			// retrieve all additional attributes the model has
		} else if (ref) {
			attr_set = ref->getAttributes();			// retrieve all additional attributes the reference has
		} else if (conn) {
			// setting up position
			msg->set_position(conn->getRegistry()->getChild("customConnectionData")->getValue());

			// retrieve all additional attributes the Connection has
			attr_set = conn->getAttributes();
		} 

		// add the location registry information
		if(atom || setObj || model || ref) {

			// retrieve the PartRegs, which stores the (1) position and (2) size information
			set<BON::RegistryNode> reg_set_per_aspect = object->getRegistry()->getChild("PartRegs")->getChildren();
			
			// debuging message
			if(reg_set_per_aspect.size() > 1)
				AfxMessageBox("There are more than one PartRegs info. Let Jae know of it please.");

			for( set<BON::RegistryNode>::iterator pos = reg_set_per_aspect.begin(); pos != reg_set_per_aspect.end(); ++pos ) 
			{
				std::string registry_name	= (*pos)->getName(); // get the aspect name
				
				// add the Aspect "key=value" pair to the vector
				std::stringstream ss_aspect_key_value;
				ss_aspect_key_value		<< "Aspect" 
										<< DELIMITER_ADDITIONAL_ATTR_KEY_VALUE
										<< registry_name;
				msg->add_location_registry(ss_aspect_key_value.str());

				// retrieve the registry nodes (e.g. Position and Size)
				set<BON::RegistryNode> reg_set = (*pos)->getChildren();

				// add all registry node key=value pairs
				for (set<BON::RegistryNode>::iterator pos_reg = reg_set.begin(); pos_reg != reg_set.end(); ++pos_reg)
				{
					// add the "key=value" pair to the vector
					msg->add_location_registry(	(*pos_reg)->getName() +
												DELIMITER_ADDITIONAL_ATTR_KEY_VALUE +
												(*pos_reg)->getValue());
				}
			}
		}
		
		// add the additional attributes to the vector storage
		if(atom || setObj || conn || ref || model) 
		{
			// iterate through the set elements (Attribute), and put them into the EventMessage
			for( set<BON::Attribute>::iterator pos = attr_set.begin(); pos != attr_set.end(); ++pos )
			{
				// add the "key=value" pair to the vector
				msg->add_additional_attribute(	(*pos)->getAttributeMeta().name() + 
												DELIMITER_ADDITIONAL_ATTR_KEY_VALUE +
												(*pos)->getStringValue() );
			}
		}
		

		// retrieve parent information
		long parentGUID[4];
		object->getParent()->getObjectI()->GetGuid(&parentGUID[0], &parentGUID[1], &parentGUID[2], &parentGUID[3]);
		msg->set_parent(	parentGUID[0],
							parentGUID[1],
							parentGUID[2],
							parentGUID[3],
							object->getParent()->getObjectMeta().name(),
							object->getParent()->getObjectMeta().infoString() );
	
		// if it is CREATED, 
		if(eventType == "CREATED"){
			// check if it's a creation of a Connection
			if(msg->get_object_info().substr(0, msg->get_object_info().find_first_of(' ')) == "MON::Connection")
			{
				// get the Connection mutex
				get_connected_mutex();

				// initiate a ConnectionInfo
				m_pConnectionInfo = new ConnectionInfo(msg);
				return;
			}

			// check if it's a creation of a Reference
			if(msg->get_object_info().substr(0, msg->get_object_info().find_first_of(' ')) == "MON::Reference")
			{
				// get the Reference mutex
				get_reference_mutex();

				// initiate a ReferenceInfo
				m_pReferenceInfo = new ReferenceInfo(msg);
				return;
			}
		}

		//////////////////////////////////////////////////////
		// if the event was a RELATION following a SETINCLUDED or a SETEXCLUDED
		//////////////////////////////////////////////////////

		// RELATION comes along with a prior SETINCLUDED or a SETEXCLUDE
		if( eventType == "RELATION") {

			// if the set info does not exist, just ignore the RELATION event
			if(m_pSetInfo == NULL) {
				/*
				if(!m_bIsXTEAMEngine)
					AfxMessageBox("RELATION is lacking a prior SETINCLUDED or SETEXCLUDED message");
				*/
				return;
			}

			// change the event type to SETINCLUDED or SETEXCLUDED
			if(m_pSetInfo->is_this_set_included())
				msg->set_eventType("SETINCLUDED");
			else
				msg->set_eventType("SETEXCLUDED");

			// add the new member information
			msg->set_new_member(	m_pSetInfo->get_GUID1_1(),
									m_pSetInfo->get_GUID1_2(),
									m_pSetInfo->get_GUID1_3(),
									m_pSetInfo->get_GUID1_4(),
									m_pSetInfo->get_connection_object_type1(),
									m_pSetInfo->get_connection_object_info1());

			// lose SET mutex
			lose_set_mutex();

			// free the memory
			delete m_pSetInfo;
			m_pSetInfo = NULL;
		}


		// filter some messages not to spam debugging
		if( eventType != "SELECT" &&
			eventType != "DESELECT" &&
			eventType != "OPENMODEL" &&
			eventType != "CLOSEMODEL")
		{

			// send it over!
			try
			{
				// send message to CoWare Server
				if(m_pClientSocket != NULL)
					m_pClientSocket->Send(msg->create_payload().c_str());
			} catch ( Exception e ) {
				AfxMessageBox(e.getErrorMessage().c_str());
			}
		}

		// free msg if it is no more used
		delete msg;
	}
}

#endif // GME_ADDON

//////////////////////////////////////////////////////
// this is the running thread that constantly reads
// from the socket, and write to the event queue
//////////////////////////////////////////////////////
UINT inboxThread(LPVOID n) {
	
	// initialize the thread
	COMTHROW(CoInitialize(NULL));

	// receive the parameters
	ATTRPKG *attr						= (ATTRPKG *) n; // static_cast
	openutils::CSocket *clientSocket	= (openutils::CSocket*)(attr->socket);
	Component *bon2comp					= (Component *)(attr->bon);

	// the message buffer
	char *buffer						= new char[MSG_BUFF_SZ];
	memset(buffer, NULL, MSG_BUFF_SZ);
	
	// variables to execute the event application to the local model
	BON::ProjectPtr project = attr->bon->m_project->getProjectI();
	BON::FCOPtr current;
	BON::FCOsPtr selected;

	// sleeps for one second to avoid colliding with GME opening the MGA file
	// It would be much better to be able to check the model opening has done at this point
	Sleep(1000);

	// run while the connection is alive
	while(bon2comp->m_bCoDesignAdaptorSwitch)
	{
		// Block and wait for an incoming message, read when the socket has received something
		if (clientSocket->Read(buffer, (MSG_BUFF_SZ-1)) == -1)
		{
			bon2comp->m_bCoDesignAdaptorSwitch = false;
			if(!bon2comp->m_bIsXTEAMEngine) {
				AfxMessageBox("CoDesign is turned off due to disrupted socket connection");
			}
			break;
		}

		// create an EventMessage out of what's received
		EventMessage * eventmsg = new EventMessage( string(buffer) );

		// get the mutex, blocked waiting for the signal
		bon2comp->m_pEventQueue->get_mutex();

		// add it to the queue
		bon2comp->m_pEventQueue->add_event(string(buffer));

		// lose mutex
		bon2comp->m_pEventQueue->lose_mutex();
		
		// execute
		attr->intf->InvokeEx(project, NULL, NULL, 0);
	}

	return 0;
}

}; // namespace BON

