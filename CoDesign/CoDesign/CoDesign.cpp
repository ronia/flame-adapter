#include "stdafx.h"
#include "CoDesign.h"

namespace BON {

//###############################################################################################################################################
//
// 	C L A S S : BON::EventMessage
//
//###############################################################################################################################################

// constructor, used when a new CoDesign event is created by a designer
EventMessage::EventMessage(	long nGUID_1,
							long nGUID_2,
							long nGUID_3,
							long nGUID_4,
							std::string strEventType,
							std::string strObjectName,
							std::string strObjectType,
							std::string strObjectInfo ) {
		
		// sets the XTEAM analysis flag to false in default
		m_cXTEAM		= 'X';

		// copy all the parameters to the member variables
		m_nGUID_1		= nGUID_1;
		m_nGUID_2		= nGUID_2;
		m_nGUID_3		= nGUID_3;
		m_nGUID_4		= nGUID_4;

		m_strEventType	= strEventType;
		m_strObjectName = strObjectName;
		m_strObjectType = strObjectType;
		m_strObjectInfo = strObjectInfo;

		m_nPosition					= MSG_POSITION_NO;
		m_bParent					= false;
		m_bAdditionalAttributes		= false;
		m_bConnection				= false;
		m_bNewMember				= false;
		m_bRef						= false;
}

// constructor, used when a CoDesign event is received. parses the message automatically
EventMessage::EventMessage( std::string msg ) {
	if(!parse_message(msg)) {
		std::stringstream ss;
		ss << "Parsing a received Event failed:\n" << msg;
		AfxMessageBox(ss.str().c_str());
	}
}

///////////////////////////////////////////////
// setters
///////////////////////////////////////////////

void EventMessage::set_attributes(	long nGUID_1,
									long nGUID_2,
									long nGUID_3,
									long nGUID_4,
									std::string strEventType,
									std::string strObjectName,
									std::string strObjectType,
									std::string strObjectInfo ) {

	m_nGUID_1		= nGUID_1;
	m_nGUID_2		= nGUID_2;
	m_nGUID_3		= nGUID_3;
	m_nGUID_4		= nGUID_4;

	m_strEventType	= strEventType;
	m_strObjectName = strObjectName;
	m_strObjectType = strObjectType;
	m_strObjectInfo = strObjectInfo;
}



void EventMessage::set_parent(	long nParentGUID_1,
								long nParentGUID_2,
								long nParentGUID_3,
								long nParentGUID_4,
								std::string strParentType,
								std::string strParentInfo ) {
	m_bParent = true;

	m_nParentGUID_1	= nParentGUID_1;
	m_nParentGUID_2	= nParentGUID_2;
	m_nParentGUID_3	= nParentGUID_3;
	m_nParentGUID_4	= nParentGUID_4;
	
	m_strParentType = strParentType;
	m_strParentInfo = strParentInfo;
}

void EventMessage::add_location_registry(std::string strLocationRegistry) {
	m_nPosition = MSG_POSITION_REGISTRY;

	for(	std::vector<std::string>::iterator pos = m_vectorLocationRegistry.begin();
			pos != m_vectorLocationRegistry.end();
			++ pos ) {

		// take the key
		std::string strKey = pos->substr(0, pos->find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE));
		std::string strNewKey = strLocationRegistry.substr(0, strLocationRegistry.find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE));

		// compare the key with the new key
		if(strKey.compare(strNewKey) > 0) {
			m_vectorLocationRegistry.insert(pos, strLocationRegistry);
			return;
		}
	}

	m_vectorLocationRegistry.push_back(strLocationRegistry);
}

void EventMessage::set_position(std::string strPosition) {
	m_nPosition = MSG_POSITION_STRING;
	m_strPosition = strPosition;
}


void EventMessage::add_additional_attribute( std::string strAdditionalAttributes ) {
	m_bAdditionalAttributes = true;

	for(	std::vector<std::string>::iterator pos = m_vectorAdditionalAttributes.begin();
			pos != m_vectorAdditionalAttributes.end();
			++ pos ) {
		
		// take the key
		std::string strKey = pos->substr(0, pos->find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE));
		std::string strNewKey = strAdditionalAttributes.substr(0, strAdditionalAttributes.find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE));

		// compare the key with the new key
		if(strKey.compare(strNewKey) > 0) {
			m_vectorAdditionalAttributes.insert(pos, strAdditionalAttributes);
			return;
		}
	}

	m_vectorAdditionalAttributes.push_back(strAdditionalAttributes);
}

void EventMessage::set_connection(	long nGUID1_1,
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
									std::string strObjectInfo2 )
{	
	m_bConnection = true;

	m_nGUID1_1			= nGUID1_1;
	m_nGUID1_2			= nGUID1_2;
	m_nGUID1_3			= nGUID1_3;
	m_nGUID1_4			= nGUID1_4;

	m_strObjectType1	= strObjectType1;
	m_strObjectInfo1	= strObjectInfo1;

	m_nGUID2_1			= nGUID2_1;
	m_nGUID2_2			= nGUID2_2;
	m_nGUID2_3			= nGUID2_3;
	m_nGUID2_4			= nGUID2_4;

	m_strObjectType2	= strObjectType2;
	m_strObjectInfo2	= strObjectInfo2;
}

void EventMessage::set_new_member(	long nGUID1_1,
									long nGUID1_2,
									long nGUID1_3,
									long nGUID1_4,
									std::string strObjectType1,
									std::string strObjectInfo1)
{
	m_bNewMember = true;

	m_nGUID_new_member_1			= nGUID1_1;
	m_nGUID_new_member_2			= nGUID1_2;
	m_nGUID_new_member_3			= nGUID1_3;
	m_nGUID_new_member_4			= nGUID1_4;

	m_strObjectType_new_member		= strObjectType1;
	m_strObjectInfo_new_member		= strObjectInfo1;

}

void EventMessage::set_ref(			long nGUID1_1,
									long nGUID1_2,
									long nGUID1_3,
									long nGUID1_4,
									std::string strObjectType1,
									std::string strObjectInfo1)
{
	m_bRef = true;

	m_nGUID_ref_1					= nGUID1_1;
	m_nGUID_ref_2					= nGUID1_2;
	m_nGUID_ref_3					= nGUID1_3;
	m_nGUID_ref_4					= nGUID1_4;

	m_strObjectType_ref				= strObjectType1;
	m_strObjectInfo_ref				= strObjectInfo1;
}

// deserialize the Message
bool EventMessage::parse_message(std::string message) {
	
	// an array of string to store each parameter of the received EventMessage
	std::string values[MSG_MAX_VALUE];

	// the received message, cloned for manipulation
	std::string payload( message );

	// initialize the tokenizer code
	int leftBound = 0, valCount = 0;
	int rightBound = payload.find_first_of(DELIMITER_ATTR);

	// take one parameter for one iteration
	while( leftBound <= rightBound ) {
		values[valCount++] = payload.substr(leftBound, rightBound-leftBound);
		leftBound = rightBound+1;
		rightBound = payload.find(DELIMITER_ATTR, leftBound);
	}
	values[valCount++] = payload.substr(leftBound);

	///////////////////////////////////////////////
	// MSG_XTEAM
	///////////////////////////////////////////////

	int XTEAMlen = values[MSG_XTEAM].length();
	if( XTEAMlen == 1 ) {
		if(values[MSG_XTEAM] == "O") {
			m_cXTEAM = 'O';
		} else {
			m_cXTEAM = 'X';
		}
	}

	///////////////////////////////////////////////
	// MSG_GUID
	// MSG_EVENT_TYPE
	// MSG_OBJ_NAME
	// MSG_OBJ_TYPE
	// MSG_INFOSTRING
	///////////////////////////////////////////////

	// Convert the GUID value from HEX string to long
	int len = values[MSG_GUID].length();
	if( len != 0 ) {	
		// store numerical guid in here
		long guid[4] = {0, 0, 0, 0};
		
		// deserialize GUID
		if(!deserialize_GUID(values[MSG_GUID], &guid[0], &guid[1], &guid[2], &guid[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}

		// set attributes
		set_attributes( guid[0], guid[1], guid[2], guid[3],
						values[MSG_EVENT_TYPE],
						values[MSG_OBJ_NAME],
						values[MSG_OBJ_TYPE],
						values[MSG_INFOSTRING]);
	}

	///////////////////////////////////////////////
	// MSG_POSITION
	///////////////////////////////////////////////

	// if there is position information, set position
	int posLen = values[MSG_POSITION].length();
	if( posLen != 0 ) {

		// in case the object is not a Connection, the location should be location registry
		if( get_object_type() != "Association" &&
			get_object_type() != "Composition" ) 
		{
			m_nPosition = MSG_POSITION_REGISTRY;

			// reuse the function for the additional attributes
			deserialize_addtional_attributes(values[MSG_POSITION], &m_vectorLocationRegistry);
		}

		// in case the object is a Connection, the location should be a string
		else {
			m_nPosition = MSG_POSITION_STRING;

			set_position(values[MSG_POSITION]);
		}
	}

	///////////////////////////////////////////////
	// MSG_PARENT_GUID
	// MSG_PARENT_TYPE
	// MSG_PARENT_INFO
	///////////////////////////////////////////////

	// if there is parent information, set parent
	int lenParent = values[MSG_PARENT_GUID].length();
	if( lenParent != 0) {
		m_bParent = true;

		// store numerical GUID in here
		long guid[4];

		// deserialize parent GUID
		if(!deserialize_GUID(values[MSG_PARENT_GUID], &guid[0], &guid[1], &guid[2], &guid[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}
		
		// set parent
		set_parent( guid[0], guid[1], guid[2], guid[3],
					values[MSG_PARENT_TYPE],
					values[MSG_PARENT_INFO] );
	}

	///////////////////////////////////////////////
	// MSG_ADDATTR
	///////////////////////////////////////////////

	// if there is additional attribute information, set addAttr
	int lenAdd = values[MSG_ADDATTR].length();
	if (lenAdd != 0) {

		// set the additional attributes flag
		m_bAdditionalAttributes = true;
		
		// deserialize the additinoal attributes string, add each additional attributes to the vector
		deserialize_addtional_attributes(values[MSG_ADDATTR], &m_vectorAdditionalAttributes);
	}

	///////////////////////////////////////////////
	// MSG_CONN_GUID_1
	// MSG_CONN_TYPE_1
	// MSG_CONN_INFO_1
	// MSG_CONN_GUID_2
	// MSG_CONN_TYPE_2
	// MSG_CONN_INFO_2
	///////////////////////////////////////////////

	// if there is Connection attribute information, set Connection
	int lenConn1 = values[MSG_CONN_GUID_1].length();
	int lenType1 = values[MSG_CONN_TYPE_1].length();
	int lenInfo1 = values[MSG_CONN_INFO_1].length();
	int lenConn2 = values[MSG_CONN_GUID_2].length();
	int lenType2 = values[MSG_CONN_TYPE_2].length();
	int lenInfo2 = values[MSG_CONN_INFO_2].length();
	if (	lenConn1 != 0 && lenType1 != 0 && lenInfo1 != 0 && 
			lenConn2 != 0 && lenType2 != 0 && lenInfo2 != 0 ) {
		m_bConnection = true;

		// store numerical GUID in here
		long guid1[4], guid2[4];

		// deserialize the GUID of the first end object
		if(!deserialize_GUID(values[MSG_CONN_GUID_1], &guid1[0], &guid1[1], &guid1[2], &guid1[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}
		
		// deserialize the GUID of the first end object
		if(!deserialize_GUID(values[MSG_CONN_GUID_2], &guid2[0], &guid2[1], &guid2[2], &guid2[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}
		
		// set connection values
		set_connection(		guid1[0], guid1[1], guid1[2], guid1[3],
							values[MSG_CONN_TYPE_1],
							values[MSG_CONN_INFO_1],
							guid2[0], guid2[1], guid2[2], guid2[3],
							values[MSG_CONN_TYPE_2],
							values[MSG_CONN_INFO_2]);
	}

	///////////////////////////////////////////////
	// MSG_NEW_MEMBER_GUID
	// MSG_NEW_MEMBER_TYPE
	// MSG_NEW_MEMBER_INFO
	///////////////////////////////////////////////

	// if there is set member information, set member
	int lenSetGUID = values[MSG_NEW_MEMBER_GUID].length();
	int lenSetType = values[MSG_NEW_MEMBER_TYPE].length();
	int lenSetInfo = values[MSG_NEW_MEMBER_INFO].length();
	if(lenSetGUID != 0 && lenSetType != 0 && lenSetInfo !=0) {
		m_bNewMember = true;

		// store numerical GUID here
		long guid_new_member[4];

		// deserialize the GUID of the new member
		if(!deserialize_GUID(values[MSG_NEW_MEMBER_GUID], &guid_new_member[0], &guid_new_member[1], &guid_new_member[2], &guid_new_member[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}

		// set new member
		set_new_member(		guid_new_member[0],
							guid_new_member[1],
							guid_new_member[2],
							guid_new_member[3],
							values[MSG_NEW_MEMBER_TYPE],
							values[MSG_NEW_MEMBER_INFO]);
	}

	///////////////////////////////////////////////
	// MSG_REF_GUID
	// MSG_REF_TYPE
	// MSG_REF_INFO
	///////////////////////////////////////////////

	// if there is reference information, set reference
	int lenRefGUID = values[MSG_REF_GUID].length();
	int lenRefType = values[MSG_REF_TYPE].length();
	int lenRefInfo = values[MSG_REF_INFO].length();
	if(lenRefGUID != 0 && lenRefType != 0 && lenRefInfo != 0) {
		m_bRef = true;

		// store numerical GUID here
		long guid_ref[4];

		// deserialize the GUID of the reference
		if(!deserialize_GUID(values[MSG_REF_GUID], &guid_ref[0], &guid_ref[1], &guid_ref[2], &guid_ref[3])) {
			// in case the GUID does not consist of four parts
			return false;
		}

		// set reference
		set_ref(			guid_ref[0],
							guid_ref[1],
							guid_ref[2],
							guid_ref[3],
							values[MSG_REF_TYPE],
							values[MSG_REF_INFO]);
	}

	return true;
}

// deserialize HEX string GUID into numerical GUID
bool EventMessage::deserialize_GUID(	std::string strGUID,
										long *nGUID_1,
										long *nGUID_2,
										long *nGUID_3,
										long *nGUID_4 ) {

	// store numerical GUID in here
	long guid[4] = {0, 0, 0, 0};

	// initialize the tokenizer code
	int valCount = 0;

	// create a temp. string to be used for strtok()
	char str[GUID_MAX_LENGTH];
	strcpy(str, strGUID.c_str());
	
	// token pointer, take the first token
	char * token;
	token = strtok (str,",");

	// if no comma is found
	if(token == NULL)
		return false;
	
	// take one GUID for one iteration
	while (token != NULL)
	{
		// convert the token into numeric value
		guid[valCount] = strtoul(token, NULL, 16);

		// if failed to convert
		if(	guid[valCount] == 0 ||
			guid[valCount] == LONG_MAX ||
			guid[valCount] == LONG_MIN)
			return false;
		
		// move on to the next token
		valCount++;
		token = strtok (NULL, ",");
	}

	// copy computed values
	if(valCount == 4) {
		*nGUID_1 = guid[0];
		*nGUID_2 = guid[1];
		*nGUID_3 = guid[2];
		*nGUID_4 = guid[3];
		return true;
	} else {
		return false;
	}
}

// deserialize additional attributes string
bool EventMessage::deserialize_addtional_attributes(	std::string strAdditionalAttributes,
														std::vector<std::string> *vectorTokenAttributes) {
	// create a temp. string to be used for strtok()
	char str[ADDITIONAL_ATTRIBUTE_MAX_LENGTH];
	strcpy(str, strAdditionalAttributes.c_str());
	
	// token pointer, take the first token
	char * token;
	std::stringstream ssDel;
	ssDel << DELIMITER_ADDITIONAL_ATTR;
	std::string delimiter = std::string(ssDel.str());
	token = strtok (str, delimiter.c_str());
	
	// take one additional attribute for one iteration
	while (token != NULL)
	{
		// replace back DELIMITER_EOL with new line charactor
		std::string strToken = replaceStrChar(std::string(token), DELIMITER_EOL, '\n');

		// store the additional attribute into the vector
		vectorTokenAttributes->push_back(strToken);
		
		// move on to the next token
		token = strtok (NULL, delimiter.c_str());
	}

	return true;
}

// serialize numerical GUID into HEX string GUID
void EventMessage::serialize_GUID(		long nGUID_1,
										long nGUID_2,
										long nGUID_3,
										long nGUID_4,
										std::string* strGUID) {
	// deserialize into stringstream
	std::stringstream ss;
	ss << std::hex << nGUID_1 << "," << nGUID_2 << "," << nGUID_3 << "," << nGUID_4;

	// copy the value
	*strGUID = std::string(ss.str());
}

// serialize the EventMessage
std::string EventMessage::create_payload() {

	// stringstream that will carry the whole serialized EventMessage
	std::stringstream ss;

	// delimiter string
	char d = DELIMITER_ATTR;

	// push XTEAM flag into stream
	ss	<< 'X' << d;

	// serialize the GUID
	std::string guid;
	serialize_GUID(m_nGUID_1, m_nGUID_2, m_nGUID_3, m_nGUID_4, &guid);

	// push into the stringstream
	ss	<< guid << d								// GUID 
		<< m_strEventType << d						// event type
		<< m_strObjectName << d						// object name
		<< m_strObjectType << d						// object type
		<< m_strObjectInfo;							// object meta info string

	// push position into the stringstream
	if(m_nPosition == MSG_POSITION_REGISTRY) 
	{	
		// push the delimiter
		ss << d;

		// push each location registry
		for (unsigned int i=0; i < m_vectorLocationRegistry.size(); i++){
			// push the sub delimiter that divides each additional attribute
			if(i != 0) {
				ss << DELIMITER_ADDITIONAL_ATTR;
			}

			// replace new line charactors with DELIMITER_EOL, and push it to the stringstream
			ss << replaceStrChar(m_vectorLocationRegistry.at(i), '\n', DELIMITER_EOL);
		}
	} else if(m_nPosition == MSG_POSITION_STRING) {
		ss << d << m_strPosition;
	} else { 
		ss << d;
	} 
	
	// push parent into the stringstream
	if(m_bParent) {
		// serialize parent GUID
		std::string parentGUID;
		serialize_GUID(m_nParentGUID_1, m_nParentGUID_2, m_nParentGUID_3, m_nParentGUID_4, &parentGUID);

		ss << d << parentGUID << d << m_strParentType << d << m_strParentInfo;
	} else {
		ss << d << d << d;
	}

	// push the additional attributes into the stringstream
	if(m_bAdditionalAttributes) {
		
		// push the delimiter
		ss << d;

		// push each additional attribute
		for (unsigned int i=0; i < m_vectorAdditionalAttributes.size(); i++) {
			// push the sub delimiter that divides each additional attribute
			if(i != 0) {
				ss << DELIMITER_ADDITIONAL_ATTR;
			}

			// replace new line charactors with DELIMITER_EOL, and push it to the stringstream
			ss << replaceStrChar(m_vectorAdditionalAttributes.at(i), '\n', DELIMITER_EOL);
		}
		
	} else {
		ss << d;
	}

	// push the connection into the stringstream
	if(m_bConnection) {
		// serialize first end object GUID
		std::string guid1;
		serialize_GUID(m_nGUID1_1, m_nGUID1_2, m_nGUID1_3, m_nGUID1_4, &guid1);

		// serialize second end object GUID
		std::string guid2;
		serialize_GUID(m_nGUID2_1, m_nGUID2_2, m_nGUID2_3, m_nGUID2_4, &guid2);


		ss	<< d << guid1 << d << m_strObjectType1 << d << m_strObjectInfo1
			<< d << guid2 << d << m_strObjectType2 << d << m_strObjectInfo2;
	} else {
		ss << d << d << d << d << d << d;
	}

	// push the new member info into the stringstream
	if(m_bNewMember) {
		// serialize new member GUID
		std::string guid_new_member;
		serialize_GUID(m_nGUID_new_member_1, m_nGUID_new_member_2, m_nGUID_new_member_3, m_nGUID_new_member_4, &guid_new_member);

		ss << d << guid_new_member << d << m_strObjectType_new_member << d << m_strObjectInfo_new_member;
	} else {
		ss << d << d << d;
	}

	// push the reference info into the stringstream
	if(m_bRef) {
		// serialize reference GUID
		std::string guid_ref;
		serialize_GUID(m_nGUID_ref_1, m_nGUID_ref_2, m_nGUID_ref_3, m_nGUID_ref_4, &guid_ref);

		ss << d << guid_ref << d << m_strObjectType_ref << d << m_strObjectInfo_ref;
	} else {
		ss << d << d << d;
	}

	ss << std::endl;
	return ss.str();
}

// replace charactor
std::string EventMessage::replaceStrChar(std::string str, char org, char to) {
	
	std::string newStr = "";

	for(unsigned int i=0; i<str.length(); i++) {
		if(str[i] != org) {
			newStr += str[i];
		} else {
			newStr += to;
		}
	}

	return newStr;

}

//###############################################################################################################################################
//
// 	C L A S S : BON::EventQueue
//
//###############################################################################################################################################

// constructor
EventQueue::EventQueue() {
	// allocate the queue
	events = new std::vector<EventMessage *>;

	// initialize the queue mutex
	hMutex = CreateMutex(NULL, FALSE, NULL);
}

// destructor
EventQueue::~EventQueue() {
	clean_up();

	delete events;

	// close the mutex handle
	CloseHandle(hMutex);
}

// add an EventMessage to the queue
void EventQueue::add_event(EventMessage *evt) {
	if(events) {
		events->push_back(evt);
	}
}

// create an EventMessage, and add it to the queue
void EventQueue::add_event(std::string evt) {
	if(events) {
		EventMessage * eventmsg = new EventMessage( evt );
		events->push_back(eventmsg);
	}
}

// check if the EventMessage is a duplicate EventMessage, add only if not
void EventQueue::add_event_no_repeat(EventMessage *evt) {
	if(events) {
		std::string pay = evt->create_payload();
		for(unsigned int i=0; i < events->size(); i++) {
			EventMessage * cur = events->at(i);
			if((cur->create_payload().compare(pay)) == 0) {
				// if match
				return;
			} 
		}

		events->push_back(evt);
	}
}

// check if the queue is empty (false) or not (true)
bool EventQueue::has_event() {
	return !events->empty();
}

// empty the queue
void EventQueue::clean_up() {
	// destroy all EventMessages in the vector
	for(unsigned int i=0; i < events->size(); i++) {
		EventMessage* msg = events->at(i);

		if(msg != NULL)
			delete msg;
	}
	
	events->clear();
}

// pop from the queue
EventMessage * EventQueue::get_head() {
	if(events) {
		if(!events->empty()) {
			EventMessage * head = events->front();
			events->erase(events->begin());
			return head;
		}
	}

	return NULL;
}

void EventQueue::get_mutex() {
	WaitForSingleObject(hMutex, INFINITE);
}

void EventQueue::lose_mutex() {
	ReleaseMutex(hMutex);
}


//###############################################################################################################################################
//
// 	C L A S S : BON::ConnectionInfo
//
//###############################################################################################################################################

// constructor
ConnectionInfo::ConnectionInfo(EventMessage * evt)
{
	connEvent = evt;
	objectCounter = 0;
}

// a connection has two end objects. add one object. 
// returns the number of objects that this connection currently has.
int ConnectionInfo::add_object(	long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo)
{
	// still addable
	if(objectCounter == 0){
		m_nGUID1_1 = nGUID_1;
		m_nGUID1_2 = nGUID_2;
		m_nGUID1_3 = nGUID_3;
		m_nGUID1_4 = nGUID_4;
		m_strObjectType1 = strObjectType;
		m_strObjectInfo1 = strObjectInfo;
	} else if (objectCounter == 1) {
		m_nGUID2_1 = nGUID_1;
		m_nGUID2_2 = nGUID_2;
		m_nGUID2_3 = nGUID_3;
		m_nGUID2_4 = nGUID_4;
		m_strObjectType2 = strObjectType;
		m_strObjectInfo2 = strObjectInfo;
	}
	// object cannot be added (already 2 objects)
	else {
		return -1;
	}

	return ++objectCounter;
}

// return the original CREATED event
EventMessage * ConnectionInfo::get_event() {
	return connEvent;
}

//###############################################################################################################################################
//
// 	C L A S S : BON::SetInfo
//
//###############################################################################################################################################

// constructor
SetInfo::SetInfo() { 
	objectCounter = 0;
}

int SetInfo::add_object(		long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo)
{
	// still addable
	if(objectCounter == 0){
		m_nGUID1_1 = nGUID_1;
		m_nGUID1_2 = nGUID_2;
		m_nGUID1_3 = nGUID_3;
		m_nGUID1_4 = nGUID_4;
		m_strObjectType1 = strObjectType;
		m_strObjectInfo1 = strObjectInfo;
	} 
	// object cannot be added (already has an object)
	else {
		return -1;
	}

	return ++objectCounter;
}

//###############################################################################################################################################
//
// 	C L A S S : BON::ReferenceInfo
//
//###############################################################################################################################################

// constructor
ReferenceInfo::ReferenceInfo(EventMessage * evt)
{
	refEvent = evt;
	objectCounter = 0;
}

// a connection has two end objects. add one object. 
// returns the number of objects that this connection currently has.
int ReferenceInfo::add_object(	long nGUID_1,
								long nGUID_2,
								long nGUID_3,
								long nGUID_4,
								std::string strObjectType, 
								std::string strObjectInfo)
{
	// still addable
	if(objectCounter == 0){
		m_nGUID1_1 = nGUID_1;
		m_nGUID1_2 = nGUID_2;
		m_nGUID1_3 = nGUID_3;
		m_nGUID1_4 = nGUID_4;
		m_strObjectType1 = strObjectType;
		m_strObjectInfo1 = strObjectInfo;
	} 
	// object cannot be added (already has an object)
	else {
		return -1;
	}

	return ++objectCounter;
}

// return the original CREATED event
EventMessage * ReferenceInfo::get_event() {
	return refEvent;
}

} // end of namespace


