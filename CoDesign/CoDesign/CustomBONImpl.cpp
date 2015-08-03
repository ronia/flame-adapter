#include "stdafx.h"
#include "BONImpl.h"
#include "CoDesign.h"
#include <string>
#include <sstream>

namespace BON {

////////////////////////////////////////////////////////////////////////////////////////
//
//	Custom Methods: intergrating XTEAM												////
//
////////////////////////////////////////////////////////////////////////////////////////

	// find all Models
	bool ProjectImpl::get_models(std::set<BON::Object> &objs)
	{
		objs = findByKind("Architecture");

		if(objs.size() == 0)
			return false;

		return true;
	}

	// add a SET member
	bool ProjectImpl::add_set_member(			long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4,  
												std::string strObjectType, std::string strObjectInfo,
												long nGUID1_1, long nGUID1_2, long nGUID1_3, long nGUID1_4,
												std::string strObjectType1, std::string strObjectInfo1)
	{
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj)) {
			AfxMessageBox("Adding a new Set member: cannot find the object!");
			return false;
		}

		// if obj is a set, set will not be NULL
		BON::Set set (obj);
		if(set) {

			// find new member with the GUID. returns false if failed.
			Object obj1;
			
			bool flag = find_object_by_GUID(nGUID1_1, nGUID1_2, nGUID1_3, nGUID1_4, strObjectType1, obj1);
			if(!flag)
				return false;
			BON::FCO fco1 (obj1);

			// if the new member is not an FCO (not sure ever possible), return false
			if(fco1) {
				set->addMember(fco1);
			} else {
				AfxMessageBox("Cannot find the new member object");
				return false;
			}
		} else {
			AfxMessageBox("Adding a new Set member: the object is not a Set");
			return false;
		}

		return true;
	}

	// remove a SET member
	bool ProjectImpl::remove_set_member(		long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4,  
												std::string strObjectType, std::string strObjectInfo,
												long nGUID1_1, long nGUID1_2, long nGUID1_3, long nGUID1_4,
												std::string strObjectType1, std::string strObjectInfo1) 
	{
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj)) {
			AfxMessageBox("Removing a new Set member: cannot find the object!");
			return false;
		}

		// if obj is a set, set will not be NULL
		BON::Set set (obj);
		if(set) {

			// find new member with the GUID. returns false if failed.
			Object obj1;
			bool flag = find_object_by_GUID(nGUID1_1, nGUID1_2, nGUID1_3, nGUID1_4, strObjectType1, obj1);
			if(!flag)
				return false;
			BON::FCO fco1 (obj1);

			// if the new member is not an FCO (not sure ever possible), return false
			if(fco1) {
				set->removeMember(fco1);
			} else {
				AfxMessageBox("Cannot find the member object being removed");
				return false;
			}
		} else {
			AfxMessageBox("Removing a new Set member: the object is not a Set");
			return false;
		}

		return true;
	}

	// add a reference
	bool ProjectImpl::add_ref(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, 
								std::string strObjectType, std::string strObjectInfo,
								long nGUID1_1, long nGUID1_2, long nGUID1_3, long nGUID1_4,
								std::string strObjectType1, std::string strObjectInfo1)
	{
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj)) {
			AfxMessageBox("Adding a Reference: cannot find the Referencer object!");
			return false;
		}

		// if obj is a Reference, ref will not be NULL
		BON::Reference ref (obj);
		if(ref) {
			// find new member with the GUID. returns false if failed.
			Object obj1;
			bool flag = find_object_by_GUID(nGUID1_1, nGUID1_2, nGUID1_3, nGUID1_4, strObjectType1, obj1);
			if(!flag)
				return false;
			BON::FCO fco1 (obj1);

			// if the referencee is not an FCO (not sure ever possible), return false
			if(fco1) {
				ref->setReferred(fco1);
			} else {
				AfxMessageBox("Adding a Reference: cannot find the Referencee object!");
				return false;
			}
		} else {
			AfxMessageBox("Adding a Reference: the Referencer object is not a Reference");
			return false;
		}

		return true;
	}

////////////////////////////////////////////////////////////////////////////////////////
//
//	Custom Methods																	////
//
////////////////////////////////////////////////////////////////////////////////////////

	// find an object by GUID
	bool ProjectImpl::find_object_by_GUID(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType, Object &obj)
	{
		std::set<BON::Object> objs = findByKind(strObjectType);
		std::set<BON::Object>::iterator it;

		long g1, g2, g3, g4;
		bool found = false;

		for(it = objs.begin(); it != objs.end(); it ++) {
			(*it)->getObjectI()->GetGuid(&g1, &g2, &g3, &g4);

			// when this object is the object we want!
			if(	nGUID_1 == g1 &&
				nGUID_2 == g2 &&
				nGUID_3 == g3 &&
				nGUID_4 == g4) {
				obj = *it;
				return true;
			}
		}
		std::stringstream ss;
		ss << "Could not find [" << std::hex <<	nGUID_1 << ", " <<
											nGUID_2 << ", " <<
											nGUID_3 << ", " <<
											nGUID_4 << "]";
		// AfxMessageBox(ss.str().c_str());

		return false;
	}

	// change the border color of an object to red
	// made for demonstration purpose
	bool ProjectImpl::update_color_to_red(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType) {
		
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
			return false;

		// if it has position registry, overwrite it
		// if obj is an atom, atom will not be NULL
		// if obj is a set, setObj will not be NULL
		BON::Atom	atom	(obj);
		BON::Set	setObj	(obj);

		// in case obj is an atom
		if(atom)
		{
			atom->getRegistry()->setColor(0xFF0000);
			atom->getRegistry()->setNameColor(0xFF0000);
		}
		
		// in case obj is a set
		else if(setObj)
		{	
			setObj->getRegistry()->setColor(0xFF0000);
			setObj->getRegistry()->setNameColor(0xFF0000);
		}

		return true;
	}

	// update location registry of an atom/model/set
	bool ProjectImpl::update_location_registry(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType, std::vector<std::string> vectorLocationRegistry)
	{
		// compute the length of the coordinate string
		int len = vectorLocationRegistry.size();

		// if the vector is not empty
		if( len != 0 )
		{
			// find object with the GUID. returns false if failed.
			Object obj;
			if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
				return false;

			// if it has Attributes registry, overwrite it
			// if obj is an atom, atom will not be NULL
			// if obj is a set, setObj will not be NULL
			// if obj is a Model, model will not be NULL
			// if obj is a Reference, ref will not be NULL
			BON::Atom		atom	(obj);
			BON::Set		setObj	(obj);
			BON::Model		model	(obj);
			BON::Reference	ref		(obj);

			// get the aspect name first
			bool aspect_name_found = false;
			std::string aspect_name;
			for(int i=0; i < vectorLocationRegistry.size(); i++) {
				std::string original = vectorLocationRegistry.at(i);
				std::string key, value;
			
				// break down into key and value pair
				int ampersand	= original.find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE);
				value			= original.substr(ampersand+1, original.length()).c_str();
				key				= original.substr(0, ampersand).c_str();
				
				if(key == "Aspect") {
					aspect_name = value;
					aspect_name_found = true;
				}
			}

			// if the aspect name is not found, error
			if(!aspect_name_found) {
				AfxMessageBox("Received REGISTRY message does not carry the Aspect name");
				return false;
			}

			// iterator for each attribute string
			for(int i=0; i < vectorLocationRegistry.size(); i++) {
				std::string original = vectorLocationRegistry.at(i);
				std::string key, value;
			
				// break down into key and value pair
				int ampersand	= original.find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE);
				value			= original.substr(ampersand+1, original.length()).c_str();
				key				= original.substr(0, ampersand).c_str();
				
				if(key != "Aspect") {
					BON::RegistryNode regNode (obj->getRegistry()->getChild("PartRegs")->getChild(aspect_name)->getChild(key));
					if(regNode) {
						regNode->setValue(value);
					} else {
						// the attribute does not exist
						std::stringstream ss_msg;
						ss_msg	<< "Received REGISTRY message error\n[Aspect:" 
								<< aspect_name << "] [Object:" 
								<< obj->getName() << "] does not have key [" 
								<< key << "]";
						AfxMessageBox(ss_msg.str().c_str());
						return false;
					}
				}
			}
		} else {
			return false;
		}

		return true;
	}

	// update location of a connection
	bool ProjectImpl::update_location(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType, std::string strObjectPosition) {
		// compute the length of the coordinate string
		int len = strObjectPosition.length();

		// if the string is not empty
		if( len != 0 )
		{
			// find object with the GUID. returns false if failed.
			Object obj;
			if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
				return false;

			// if obj is a Connection, conn will not be NULL
			BON::Connection conn (obj);
			BON::RegistryNode regNode (obj->getRegistry()->getChild("customConnectionData"));
			if(regNode)
			{
				obj->getRegistry()->getChild("customConnectionData")->setValue(strObjectPosition);
				return true;
			}
				
			/*
			if(conn) {
				

				conn->getRegistry()->getChild("customConnectionData")->setValue(strObjectPosition);
				return true;
			} else {
				return false;
			}
			*/
		}

		return false;
	}
	
	// update Attributes of a class
	bool ProjectImpl::update_additional_attributes(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType, std::vector<std::string> vectorAdditionalAttributes) {
	
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
			return false;

		// if it has Attributes registry, overwrite it
		// if obj is an atom, atom will not be NULL
		// if obj is a set, setObj will not be NULL
		// if obj is a Connection, conn will not be NULL
		// if obj is a Reference, ref will not be NULL
		BON::Atom		atom	(obj);
		BON::Set		setObj	(obj);
		BON::Connection	conn	(obj);
		BON::Reference	ref		(obj);
		BON::Model		model	(obj);
		
		// iterator for each attribute string
		for(int i=0; i < vectorAdditionalAttributes.size(); i++) {
			
			std::string original = vectorAdditionalAttributes.at(i);
			std::string key, value;
			
			// break down into key and value pair
			int ampersand	= original.find_first_of(DELIMITER_ADDITIONAL_ATTR_KEY_VALUE);
			value			= original.substr(ampersand+1, original.length()).c_str();
			key				= original.substr(0, ampersand).c_str();
			
			// find the attribute
			if (atom) 
			{
				BON::Attribute attribute (atom->getAttribute(key));
				if(attribute) 
				{
					// find out the type of the attribute
					switch(attribute->getAttributeMeta().valueType())
					{
						// boolean
						case 4:
							// if the value is FALSE
							if(value == "false" || value == "False") {
								attribute->setBooleanValue(false);
							} 
							// if the value is TRUE
							else {
								attribute->setBooleanValue(true);
							}
							break;

						// the rest (only strings for UML)
						default:
							// set the attribute
							attribute->setStringValue(value);
							break;
					}
					
				} else {
					// the attribute does not exist
					return false;
				}

			} else if (setObj) {

				BON::Attribute attribute (setObj->getAttribute(key));
				if(attribute) {
					// find out the type of the attribute
					switch(attribute->getAttributeMeta().valueType())
					{
						// boolean
						case 4:
							// if the value is FALSE
							if(value == "false" || value == "False") {
								attribute->setBooleanValue(false);
							} 
							// if the value is TRUE
							else {
								attribute->setBooleanValue(true);
							}
							break;

						// the rest (only strings for UML)
						default:
							// set the attribute
							attribute->setStringValue(value);
							break;
					}
				} else {
					// the attribute does not exist
					return false;
				}
				
			} else if (conn) {

				BON::Attribute attribute (conn->getAttribute(key));
				if(attribute) {
					// find out the type of the attribute
					switch(attribute->getAttributeMeta().valueType())
					{
						// boolean
						case 4:
							// if the value is FALSE
							if(value == "false" || value == "False") {
								attribute->setBooleanValue(false);
							} 
							// if the value is TRUE
							else {
								attribute->setBooleanValue(true);
							}
							break;

						// the rest (only strings for UML)
						default:
							// set the attribute
							attribute->setStringValue(value);
							break;
					}
				} else {
					// the attribute does not exist
					return false;
				}

			} else if (ref) {

				BON::Attribute attribute (ref->getAttribute(key));
				if(attribute) {
					// find out the type of the attribute
					switch(attribute->getAttributeMeta().valueType())
					{
						// boolean
						case 4:
							// if the value is FALSE
							if(value == "false" || value == "False") {
								attribute->setBooleanValue(false);
							} 
							// if the value is TRUE
							else {
								attribute->setBooleanValue(true);
							}
							break;

						// the rest (only strings for UML)
						default:
							// set the attribute
							attribute->setStringValue(value);
							break;
					}
				} else {
					// the attribute does not exist
					return false;
				}

			} else if (model) {

				BON::Attribute attribute (model->getAttribute(key));
				if(attribute) {
					// find out the type of the attribute
					switch(attribute->getAttributeMeta().valueType())
					{
						// boolean
						case 4:
							// if the value is FALSE
							if(value == "false" || value == "False") {
								attribute->setBooleanValue(false);
							} 
							// if the value is TRUE
							else {
								attribute->setBooleanValue(true);
							}
							break;

						// the rest (only strings for UML)
						default:
							// set the attribute
							attribute->setStringValue(value);
							break;
					}
				} else {
					// the attribute does not exist
					return false;
				}

			} 
			
			else {
				return false;
			}
		}

		return true;
	}

	// update name of an object
	bool ProjectImpl::update_name(long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectName, std::string strObjectType) {
		
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
			return false;

		// set name of the object
		obj->setName(std::string(strObjectName));
		
		return true;
	}

	// get what kind of object it is by the object information string
	std::string ProjectImpl::get_kind_by_info(std::string strObjectInfo)
	{
		std::string kind;

		kind = strObjectInfo.substr(5, strObjectInfo.find_first_of(" ")-5);
		return kind;
	}

	// create an object, and update its parent
	bool ProjectImpl::create_object(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4,  
										std::string strObjectType, 
										std::string strObjectInfo,
										long nParentGUID_1, long nParentGUID_2, long nParentGUID_3, long nParentGUID_4, 
										std::string strParentType, 
										std::string strParentInfo)
	{

		// find the parent object with the GUID. returns false if failed.
		Object parent;
		if(!find_object_by_GUID(nParentGUID_1, nParentGUID_2, nParentGUID_3, nParentGUID_4, strParentType, parent))
			return false;

		// if the parent is a model, m will not be NULL
		Model	m (parent);
		Folder	f (parent);

		// get what kind of object it is: either be an atom or a set
		std::string kind = get_kind_by_info(strObjectInfo);

		Object* obj = NULL;
		if(m) {
			if(kind == "Atom") {
				obj = new Atom (Atom::create(m, strObjectType));
			} else if (kind == "Set") {
				obj = new Set (Set::create(m, strObjectType));
			} else if (kind == "Model") {
				obj = new Model (Model::create(m, strObjectType));
			} else if (kind == "Reference") {
				obj = new Reference (Reference::create(m, strObjectType));
			} else if (kind == "Folder") {
				obj = new Folder (Folder::create(m, strObjectType));
			} else {
				return false;
			}
		} else if(f) {
			if(kind == "Atom") {
				obj = new Atom (Atom::create(f, strObjectType));
			} else if (kind == "Set") {
				obj = new Set (Set::create(f, strObjectType));
			} else if (kind == "Model") {
				obj = new Model (Model::create(f, strObjectType));
			} else if (kind == "Reference") {
				obj = new Reference (Reference::create(f, strObjectType));
			} else if (kind == "Folder") {
				obj = new Folder (Folder::create(f, strObjectType));
				(*obj)->setName("temp");
			} else {
				return false;
			}
		} else {
			return false;
		}

		// put GUID on the new Object
		(*obj)->getObjectI()->PutGuid(nGUID_1, nGUID_2, nGUID_3, nGUID_4);

		
		// if no error so far
		return true;
	}

	// create a connection (association or composition) between two objects, and update its parent
	bool ProjectImpl::create_connection(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4,  
											std::string strObjectType, 
											std::string strObjectInfo,
											long nGUID1_1, long nGUID1_2, long nGUID1_3, long nGUID1_4, 
											std::string strObjectType1, 
											std::string strObjectInfo1,
											long nGUID2_1, long nGUID2_2, long nGUID2_3, long nGUID2_4, 
											std::string strObjectType2, 
											std::string strObjectInfo2,
											long nParentGUID_1, long nParentGUID_2, long nParentGUID_3, long nParentGUID_4, 
											std::string strParentType, 
											std::string strParentInfo)
	{
	
		// find the parent object with the GUID. returns false if failed.
		Object parent;
		if(!find_object_by_GUID(nParentGUID_1, nParentGUID_2, nParentGUID_3, nParentGUID_4, strParentType, parent))
			return false;

		// if the parent is a model, m will not be NULL
		Model m (parent);
		if(m) {

			// find object 1 with the GUID. returns false if failed.
			Object obj1;
			if(!find_object_by_GUID(nGUID1_1, nGUID1_2, nGUID1_3, nGUID1_4, strObjectType1, obj1))
				return false;
			BON::Atom			atom1	(obj1);
			BON::Model			model1	(obj1);
			BON::Reference		ref1	(obj1);

			// find object 2 with the GUID. returns false if failed.
			Object obj2;
			if(!find_object_by_GUID(nGUID2_1, nGUID2_2, nGUID2_3, nGUID2_4, strObjectType2, obj2))
				return false;
			BON::Atom			atom2	(obj2);
			BON::Model			model2	(obj2);
			BON::Reference		ref2	(obj2);

			// create the ConnectionEnds
			BON::ConnectionEnd* end1 = NULL;
			BON::ConnectionEnd* end2 = NULL;

			if(atom1) {
				end1 = new BON::ConnectionEnd (atom1);
			} else if (model1) {
				end1 = new BON::ConnectionEnd (model1);
			} else if (ref1) {
				end1 = new BON::ConnectionEnd (ref1);
			} else {
				return false;
			}

			if(atom2) {
				end2 = new BON::ConnectionEnd (atom2);
			} else if (model2) {
				end2 = new BON::ConnectionEnd (model2);
			} else if (ref2) {
				end2 = new BON::ConnectionEnd (ref2);
			} else {
				return false;
			}

			// create the connection
			Connection conn (Connection::create(m, *end1, *end2, strObjectType));
			
			// put the GUID
			conn->getObjectI()->PutGuid(nGUID_1, nGUID_2, nGUID_3, nGUID_4);

			// put the name
			conn->setName(strObjectType);

			return true;
		}

		return false;
	}
	

	bool ProjectImpl::destroy_object(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4,
										std::string strObjectName,
										std::string strObjectType,
										std::string strObjectInfo )
	{
		// find object with the GUID. returns false if failed.
		Object obj;
		if(!find_object_by_GUID(nGUID_1, nGUID_2, nGUID_3, nGUID_4, strObjectType, obj))
			return false;
		
		// destroy it!
		obj->destroy();

		return true;
	}

	bool ProjectImpl::destroy_connections(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4)
	{
		bool hasError = false;

		if(!destroy_connections(nGUID_1, nGUID_2, nGUID_3, nGUID_4, "Composition"))
			hasError = true;

		if(!destroy_connections(nGUID_1, nGUID_2, nGUID_3, nGUID_4, "Association"))
			hasError = true;

		return !hasError;
	}

	bool ProjectImpl::destroy_connections(	long nGUID_1, long nGUID_2, long nGUID_3, long nGUID_4, std::string strObjectType)
	{
		AfxMessageBox("destroy_connections begins");

		// retrieve all connections
		std::set<BON::Object> objs = findByKind(strObjectType);
		std::set<BON::Object>::iterator it;

		for(it = objs.begin(); it != objs.end(); it ++) {
			Connection conn (*it);

			// if the object is a Connection
			if(conn)
			{
				AfxMessageBox("Checking out a Connection [SRC]");

				std::multiset<BON::ConnectionEnd> cendsSrc = conn->getSrc()->getConnEnds();
				check_out_conn_ends(nGUID_1, nGUID_2, nGUID_3, nGUID_4, cendsSrc);

				AfxMessageBox("Checking out a Connection [DST]");
				
				std::multiset<BON::ConnectionEnd> cendsDst = conn->getDst()->getConnEnds();
				check_out_conn_ends(nGUID_1, nGUID_2, nGUID_3, nGUID_4, cendsDst);
			}
		}

		return true;
	}

	void ProjectImpl::check_out_conn_ends(	long nGUID_1, 
											long nGUID_2, 
											long nGUID_3, 
											long nGUID_4,
											std::multiset<BON::ConnectionEnd>& cends)
	{
		// get the connection ends
		std::multiset<BON::ConnectionEnd>::const_iterator it;
		for(it = cends.begin(); it != cends.end(); ++it)
		{
			FCO connend (*it);
			if(connend)
			{
				AfxMessageBox(connend->getName().c_str());

				// get the GUID of the connection end
				long guid1, guid2, guid3, guid4;
				connend->getObjectI()->GetGuid(&guid1, &guid2, &guid3, &guid4);

				if(	nGUID_1 == guid1 &&
					nGUID_2 == guid2 &&
					nGUID_3 == guid3 &&
					nGUID_4 == guid4 )
				{
					AfxMessageBox(connend->getName().c_str());
				}
			}
		}
	}
}


