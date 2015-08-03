#include "stdafx.h"
#include <string>
#include <sstream>
#include <vector>
#include <set> 

// maximum message buffer size. An EventMessage can grow up to this size.
#define MSG_BUFF_SZ		2000

// maximum number of parameters an EventMessage can have
#define MSG_MAX_VALUE	100

// maximum length of an object GUID
#define GUID_MAX_LENGTH	60

// maximum length of an additional attribute string
#define ADDITIONAL_ATTRIBUTE_MAX_LENGTH 1000


namespace BON {

//###############################################################################################################################################
//
// 	Delimiters
//
//###############################################################################################################################################

#define DELIMITER_ADDITIONAL_ATTR			'&'		// to separate additional attribute strings
#define DELIMITER_ADDITIONAL_ATTR_KEY_VALUE	'='		// to separate key and value of an additional attribute string
#define DELIMITER_ATTR						'`'		// to divide between each attribute of an EventMessage
#define DELIMITER_EOL						'|'		// to represent an end-of-line

//###############################################################################################################################################
//
// 	Options
//
//###############################################################################################################################################

#define MSG_POSITION_NO			0		// POSITION is not declared
#define MSG_POSITION_REGISTRY	1		// POSITION is given as an X & Y coordinate
#define MSG_POSITION_STRING		2		// POSITION is given as a string

//###############################################################################################################################################
//
// 	the ordering of a CoDesign EventMessage parameters
//
//###############################################################################################################################################

#define MSG_XTEAM		0			// O or X for running XTEAM analysis
#define MSG_GUID		1			// GUID consists of 4 longs, devided by commas
#define MSG_EVENT_TYPE	2
#define MSG_OBJ_NAME	3
#define MSG_OBJ_TYPE	4
#define MSG_INFOSTRING	5

#define MSG_POSITION	6			// position consists of 2 integers, devided by a comma

#define MSG_PARENT_GUID	7			// GUID consists of 4 longs, devided by commas
#define MSG_PARENT_TYPE	8
#define MSG_PARENT_INFO	9

#define MSG_ADDATTR		10

#define MSG_CONN_GUID_1	11			// GUID consists of 4 longs, devided by commas
#define MSG_CONN_TYPE_1	12
#define MSG_CONN_INFO_1	13
#define MSG_CONN_GUID_2	14			// GUID consists of 4 longs, devided by commas
#define MSG_CONN_TYPE_2	15
#define MSG_CONN_INFO_2	16

#define MSG_NEW_MEMBER_GUID 17		// GUID consists of 4 longs, devided by commas
#define MSG_NEW_MEMBER_TYPE 18
#define MSG_NEW_MEMBER_INFO 19

#define MSG_REF_GUID 20				// GUID consists of 4 longs, devided by commas 
#define MSG_REF_TYPE 21
#define MSG_REF_INFO 22

//###############################################################################################################################################
//
// 	C L A S S : BON::EventMessage
//
//###############################################################################################################################################

class EventMessage
{
public:
	// constructor, used when a new CoDesign event is created by a designer
	EventMessage(	long nGUID_1,
					long nGUID_2,
					long nGUID_3,
					long nGUID_4,
					std::string strEventType,
					std::string strObjectName,
					std::string strObjectType,
					std::string strObjectInfo );

	// constructor, used when a CoDesign event is received. parses the message automatically
	EventMessage(	std::string msg );

	///////////////////////////////////////////////
	// setters
	///////////////////////////////////////////////
	
	void set_eventType(				std::string strEventType) { m_strEventType = strEventType; };
	void set_attributes(			long nGUID_1,
									long nGUID_2,
									long nGUID_3,
									long nGUID_4,
									std::string strEventType,
									std::string strObjectName,
									std::string strObjectType,
									std::string strObjectInfo );
	void set_parent(				long nParentGUID_1,
									long nParentGUID_2,
									long nParentGUID_3,
									long nParentGUID_4,
									std::string strParentType,
									std::string strParentInfo );

	// location registry information for atoms, models ... 
	// ... things that are not connections
	void add_location_registry(		std::string strRegistry );

	// position registry for connections
	void set_position(				std::string strPosition);

	void add_additional_attribute(	std::string strAdditionalAttributes );
	void set_connection(			long nGUID1_1,
									long nGUID1_2,
									long nGUID1_3,
									long nGUID1_4,
									std::string strObjectType1,
									std::string strObjectInfo1,
									long nGUID2_1,
									long nGUID2_2,
									long nGUID2_3,
									long nGUID2_4,
									std::string strObjectType2,
									std::string strObjectInfo2 );

	// set the new member for RELATION following SETINCLUDED or SETEXCLUDED
	void set_new_member(			long nGUID1_1,
									long nGUID1_2,
									long nGUID1_3,
									long nGUID1_4,
									std::string strObjectType1,
									std::string strObjectInfo1);

	// set the reference information for REFERENCED following CREATED
	void set_ref(					long nGUID1_1,
									long nGUID1_2,
									long nGUID1_3,
									long nGUID1_4,
									std::string strObjectType1,
									std::string strObjectInfo1); 

	// deserialize the Message
	bool parse_message(				std::string message);

	// deserialize HEX string GUID into numerical GUID
	bool deserialize_GUID(			std::string strGUID,
									long *nGUID_1,
									long *nGUID_2,
									long *nGUID_3,
									long *nGUID_4 );

	// deserialize additional attributes string
	bool deserialize_addtional_attributes(	
									std::string strAdditionalAttributes,
									std::vector<std::string> *vectorTokenAttributes);

	///////////////////////////////////////////////
	// getters
	///////////////////////////////////////////////

	bool isXTEAMAnalysisOn() {
		if(m_cXTEAM == 'O')	return true;
		else				return false;
	}

	char get_XTEAM_flag() { return m_cXTEAM; }

	void get_GUID (		long *nGUID_1, 
						long *nGUID_2,
						long *nGUID_3,
						long *nGUID_4) {				
		*nGUID_1 = m_nGUID_1; 
		*nGUID_2 = m_nGUID_2;
		*nGUID_3 = m_nGUID_3;
		*nGUID_4 = m_nGUID_4; 
	};

	long get_GUID_1 () { return m_nGUID_1; };
	long get_GUID_2 () { return m_nGUID_2; };
	long get_GUID_3 () { return m_nGUID_3; };
	long get_GUID_4 () { return m_nGUID_4; };

	std::string get_event_type()  { return m_strEventType; };
	std::string get_object_name() { return m_strObjectName; };
	std::string get_object_type() { return m_strObjectType; };
	std::string get_object_info() { return m_strObjectInfo; };

	std::vector<std::string> get_location_registry() { return m_vectorLocationRegistry; };
	std::string get_position_string()	{ return m_strPosition; };
	
	void get_parent_GUID (	long *nGUID_1, 
							long *nGUID_2,
							long *nGUID_3,
							long *nGUID_4) {				
		*nGUID_1 = m_nParentGUID_1; 
		*nGUID_2 = m_nParentGUID_2;
		*nGUID_3 = m_nParentGUID_3;
		*nGUID_4 = m_nParentGUID_4; 
	};

	long get_parent_GUID_1 () { return m_nParentGUID_1; };
	long get_parent_GUID_2 () { return m_nParentGUID_2; };
	long get_parent_GUID_3 () { return m_nParentGUID_3; };
	long get_parent_GUID_4 () { return m_nParentGUID_4; };

	std::string get_parent_type() { return m_strParentType; };
	std::string get_parent_info() { return m_strParentInfo; };

	std::vector<std::string> get_additional_attributes() { return m_vectorAdditionalAttributes; };

	void get_connection_GUID_1 (	long *nGUID_1, 
									long *nGUID_2,
									long *nGUID_3,
									long *nGUID_4) {				
		*nGUID_1 = m_nGUID1_1; 
		*nGUID_2 = m_nGUID1_2;
		*nGUID_3 = m_nGUID1_3;
		*nGUID_4 = m_nGUID1_4; 
	};
	long get_connection_GUID1_1() { return m_nGUID1_1; };
	long get_connection_GUID1_2() { return m_nGUID1_2; };
	long get_connection_GUID1_3() { return m_nGUID1_3; };
	long get_connection_GUID1_4() { return m_nGUID1_4; };

	void get_connection_GUID2 (		long *nGUID_1, 
									long *nGUID_2,
									long *nGUID_3,
									long *nGUID_4) {				
		*nGUID_1 = m_nGUID2_1; 
		*nGUID_2 = m_nGUID2_2;
		*nGUID_3 = m_nGUID2_3;
		*nGUID_4 = m_nGUID2_4; 
	};
	long get_connection_GUID2_1() { return m_nGUID2_1; };
	long get_connection_GUID2_2() { return m_nGUID2_2; };
	long get_connection_GUID2_3() { return m_nGUID2_3; };
	long get_connection_GUID2_4() { return m_nGUID2_4; };

	std::string get_connection_object_type1() { return m_strObjectType1; };
	std::string get_connection_object_info1() { return m_strObjectInfo1; };
	std::string get_connection_object_type2() { return m_strObjectType2; };
	std::string get_connection_object_info2() { return m_strObjectInfo2; };

	void get_new_member_GUID(		long *nGUID_1,
									long *nGUID_2,
									long *nGUID_3,
									long *nGUID_4) {
		*nGUID_1 = m_nGUID_new_member_1; 
		*nGUID_2 = m_nGUID_new_member_2;
		*nGUID_3 = m_nGUID_new_member_3;
		*nGUID_4 = m_nGUID_new_member_4; 
	}

	long get_new_member_GUID_1() { return m_nGUID_new_member_1; };
	long get_new_member_GUID_2() { return m_nGUID_new_member_2; };
	long get_new_member_GUID_3() { return m_nGUID_new_member_3; };
	long get_new_member_GUID_4() { return m_nGUID_new_member_4; };
	
	std::string get_new_member_object_type() { return m_strObjectType_new_member; };
	std::string get_new_member_object_info() { return m_strObjectInfo_new_member; };

	void get_ref_GUID(				long *nGUID_1,
									long *nGUID_2,
									long *nGUID_3,
									long *nGUID_4) {
		*nGUID_1 = m_nGUID_ref_1; 
		*nGUID_2 = m_nGUID_ref_2;
		*nGUID_3 = m_nGUID_ref_3;
		*nGUID_4 = m_nGUID_ref_4; 
	}

	long get_ref_GUID_1() { return m_nGUID_ref_1; };
	long get_ref_GUID_2() { return m_nGUID_ref_2; };
	long get_ref_GUID_3() { return m_nGUID_ref_3; };
	long get_ref_GUID_4() { return m_nGUID_ref_4; };

	std::string get_ref_object_type() { return m_strObjectType_ref; };
	std::string get_ref_object_info() { return m_strObjectInfo_ref; };

	// serialize numerical GUID into HEX string GUID
	void serialize_GUID(		long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string* strGUID);

	// serialize the Message
	std::string create_payload();

	// replace charactor
	std::string replaceStrChar(std::string str, char org, char to);


private:

	// Running XTEAM analysis flag
	char m_cXTEAM;

	// Required Attributes
	long m_nGUID_1;
	long m_nGUID_2;
	long m_nGUID_3;
	long m_nGUID_4;
	std::string m_strEventType;
	std::string m_strObjectName;
	std::string m_strObjectType;
	std::string m_strObjectInfo;

	// Position Attributes
	int	m_nPosition;
	std::vector<std::string> m_vectorLocationRegistry;
	std::string m_strPosition;

	// Parent Information Attributes
	bool m_bParent;
	long m_nParentGUID_1;
	long m_nParentGUID_2;
	long m_nParentGUID_3;
	long m_nParentGUID_4;
	std::string m_strParentType;
	std::string m_strParentInfo;

	// Additional Attributes (defines what attributes a class has)
	bool m_bAdditionalAttributes;
	std::vector<std::string> m_vectorAdditionalAttributes;

	// Connection Attributes
	bool m_bConnection;

	long m_nGUID1_1;
	long m_nGUID1_2;
	long m_nGUID1_3;
	long m_nGUID1_4;
	std::string m_strObjectType1;
	std::string m_strObjectInfo1;

	long m_nGUID2_1;
	long m_nGUID2_2;
	long m_nGUID2_3;
	long m_nGUID2_4;
	std::string m_strObjectType2;
	std::string m_strObjectInfo2;

	// new member attributes
	bool m_bNewMember;

	long m_nGUID_new_member_1;
	long m_nGUID_new_member_2;
	long m_nGUID_new_member_3;
	long m_nGUID_new_member_4;
	std::string m_strObjectType_new_member;
	std::string m_strObjectInfo_new_member;

	// reference attributes
	bool m_bRef;
	
	long m_nGUID_ref_1;
	long m_nGUID_ref_2;
	long m_nGUID_ref_3;
	long m_nGUID_ref_4;
	std::string m_strObjectType_ref;
	std::string m_strObjectInfo_ref;

};

//###############################################################################################################################################
//
// 	C L A S S : BON::EventQueue
//
//###############################################################################################################################################

class EventQueue
{
public:
	// constructor
	EventQueue();

	// destructor
	~EventQueue();

	// add an EventMessage to the queue
	void add_event(EventMessage * evt);

	// create an EventMessage, and add it to the queue
	void add_event(std::string evt);

	// check if the EventMessage is a duplicate EventMessage, add only if not
	void add_event_no_repeat(EventMessage * evt);

	// check if the queue is empty (false) or not (true)
	bool has_event();

	// empty the queue
	void clean_up();

	// pop from the queue
	EventMessage * get_head();

	// MUTEX ahoy!
	HANDLE hMutex;

	// Get the mutex
	void get_mutex();

	// Lose the mutex
	void lose_mutex();

private:
	// queue data structure
	std::vector<EventMessage *> *events;

};

//###############################################################################################################################################
//
// 	C L A S S : BON::ConnectionInfo
//
//###############################################################################################################################################

// supposed to be used as a global variable by BON2Component
class ConnectionInfo
{
public:
	// constructor
	ConnectionInfo (			EventMessage * evt );

	// a connection has two end objects. add one object. 
	// returns the number of objects that this connection currently has.
	int add_object (			long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo );

	// return the original CONNECTED event
	EventMessage* get_event(	void);

	// getters
	void get_GUID1(				long *nGUID_1,
								long *nGUID_2,
								long *nGUID_3,
								long *nGUID_4) { 
		*nGUID_1 = m_nGUID1_1;
		*nGUID_2 = m_nGUID1_2;
		*nGUID_3 = m_nGUID1_3;
		*nGUID_4 = m_nGUID1_4;
	};
	long get_GUID1_1() { return m_nGUID1_1; };
	long get_GUID1_2() { return m_nGUID1_2; };
	long get_GUID1_3() { return m_nGUID1_3; };
	long get_GUID1_4() { return m_nGUID1_4; };
	
	void get_GUID2(				long *nGUID_1,
								long *nGUID_2,
								long *nGUID_3,
								long *nGUID_4) { 
		*nGUID_1 = m_nGUID2_1;
		*nGUID_2 = m_nGUID2_2;
		*nGUID_3 = m_nGUID2_3;
		*nGUID_4 = m_nGUID2_4;
	};
	long get_GUID2_1() { return m_nGUID2_1; };
	long get_GUID2_2() { return m_nGUID2_2; };
	long get_GUID2_3() { return m_nGUID2_3; };
	long get_GUID2_4() { return m_nGUID2_4; };

	std::string get_connection_object_type1() { return m_strObjectType1; };
	std::string get_connection_object_info1() { return m_strObjectInfo1; };
	std::string get_connection_object_type2() { return m_strObjectType2; };
	std::string get_connection_object_info2() { return m_strObjectInfo2; };
	
private:
	// the original CONNECTED event storage
	EventMessage *	connEvent;

	// connection 1 object information
	long m_nGUID1_1;
	long m_nGUID1_2;
	long m_nGUID1_3;
	long m_nGUID1_4;
	std::string m_strObjectType1;
	std::string m_strObjectInfo1;

	// connection 2 object information
	long m_nGUID2_1;
	long m_nGUID2_2;
	long m_nGUID2_3;
	long m_nGUID2_4;
	std::string m_strObjectType2;
	std::string m_strObjectInfo2;

	// number of objects that have been added to this connection
	// starts at 0, and ends at 2
	int objectCounter;
};

//###############################################################################################################################################
//
// 	C L A S S : BON::SetInfo
//
//###############################################################################################################################################

class SetInfo{
public:
	// constructor
	SetInfo ();

	// A Set inclusion/exclusion has the new member object and the target set; add a member
	// returns the number of objects that this connection currently has.
	int add_object (			long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo );

	// getters
	void get_GUID1(				long *nGUID_1,
								long *nGUID_2,
								long *nGUID_3,
								long *nGUID_4) { 
		*nGUID_1 = m_nGUID1_1;
		*nGUID_2 = m_nGUID1_2;
		*nGUID_3 = m_nGUID1_3;
		*nGUID_4 = m_nGUID1_4;
	};
	long get_GUID1_1() { return m_nGUID1_1; };
	long get_GUID1_2() { return m_nGUID1_2; };
	long get_GUID1_3() { return m_nGUID1_3; };
	long get_GUID1_4() { return m_nGUID1_4; };

	std::string get_connection_object_type1() { return m_strObjectType1; };
	std::string get_connection_object_info1() { return m_strObjectInfo1; };

	void this_is_set_included() { isSetIncluded = true; };
	void this_is_set_excluded() { isSetIncluded = false; };

	bool is_this_set_included() { if(isSetIncluded) return true; else return false; };
	bool is_this_set_excluded() { if(isSetIncluded) return false; else return true; };

private:

	// new member object information
	long m_nGUID1_1;
	long m_nGUID1_2;
	long m_nGUID1_3;
	long m_nGUID1_4;
	std::string m_strObjectType1;
	std::string m_strObjectInfo1;

	// SETINCLUDED or SETEXCLUDED flag; true if SETINCLUDED
	bool isSetIncluded;

	// number of objects that have been added to this connection
	// starts at 0, and ends at 1
	int objectCounter;
};

//###############################################################################################################################################
//
// 	C L A S S : BON::ReferenceInfo
//
//###############################################################################################################################################

// supposed to be used as a global variable by BON2Component
class ReferenceInfo
{
public:
	// constructor
	ReferenceInfo (				EventMessage * evt );

	// a connection has the referencee objects. add one object. 
	// returns the number of objects that this connection currently has.
	int add_object (			long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo );

	// return the original CREATED event
	EventMessage* get_event(	void);

	// getters
	void get_GUID1(				long *nGUID_1,
								long *nGUID_2,
								long *nGUID_3,
								long *nGUID_4) { 
		*nGUID_1 = m_nGUID1_1;
		*nGUID_2 = m_nGUID1_2;
		*nGUID_3 = m_nGUID1_3;
		*nGUID_4 = m_nGUID1_4;
	};
	long get_GUID1_1() { return m_nGUID1_1; };
	long get_GUID1_2() { return m_nGUID1_2; };
	long get_GUID1_3() { return m_nGUID1_3; };
	long get_GUID1_4() { return m_nGUID1_4; };

	std::string get_connection_object_type1() { return m_strObjectType1; };
	std::string get_connection_object_info1() { return m_strObjectInfo1; };

private:
	// the original CONNECTED event storage
	EventMessage *	refEvent;

	// connection 1 object information
	long m_nGUID1_1;
	long m_nGUID1_2;
	long m_nGUID1_3;
	long m_nGUID1_4;
	std::string m_strObjectType1;
	std::string m_strObjectInfo1;

	// number of objects that have been added to this connection
	// starts at 0, and ends at 1
	int objectCounter;
};

} // end of namespace BON
