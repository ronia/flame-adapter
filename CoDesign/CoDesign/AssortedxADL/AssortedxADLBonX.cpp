#include "stdafx.h"
#include "AssortedxADLBonX.h"
#include <iostream>

namespace BON
{

IMPLEMENT_BONEXTENSION( XTEAM::ArchitectureFolder, "ArchitectureFolder" );
IMPLEMENT_BONEXTENSION( XTEAM::DataModelsFolder, "DataModelsFolder" );
IMPLEMENT_BONEXTENSION( XTEAM::ElementsFolder, "ElementsFolder" );
IMPLEMENT_BONEXTENSION( XTEAM::InterfacesFolder, "InterfacesFolder" );
IMPLEMENT_ABSTRACT_BONEXTENSION( XTEAM::Element );
IMPLEMENT_BONEXTENSION( XTEAM::Component, "Component" );
IMPLEMENT_BONEXTENSION( XTEAM::Connector, "Connector" );
IMPLEMENT_ABSTRACT_BONEXTENSION( XTEAM::Port );
IMPLEMENT_BONEXTENSION( XTEAM::Input, "Input" );
IMPLEMENT_BONEXTENSION( XTEAM::Output, "Output" );
IMPLEMENT_BONEXTENSION( XTEAM::Architecture, "Architecture" );
IMPLEMENT_BONEXTENSION( XTEAM::DataModel, "DataModel" );
IMPLEMENT_BONEXTENSION( XTEAM::Interface, "Interface" );
IMPLEMENT_BONEXTENSION( XTEAM::Process, "Process" );
IMPLEMENT_BONEXTENSION( XTEAM::Resource, "Resource" );
IMPLEMENT_BONEXTENSION( XTEAM::Task, "Task" );
IMPLEMENT_ABSTRACT_BONEXTENSION( XTEAM::BranchEnd );
IMPLEMENT_ABSTRACT_BONEXTENSION( XTEAM::Initiator );
IMPLEMENT_ABSTRACT_BONEXTENSION( XTEAM::TransitionEnd );
IMPLEMENT_BONEXTENSION( XTEAM::Choice, "Choice" );
IMPLEMENT_BONEXTENSION( XTEAM::Event, "Event" );
IMPLEMENT_BONEXTENSION( XTEAM::Conditional, "Conditional" );
IMPLEMENT_BONEXTENSION( XTEAM::Stop, "Stop" );
IMPLEMENT_BONEXTENSION( XTEAM::ProcessTransition, "ProcessTransition" );
IMPLEMENT_BONEXTENSION( XTEAM::Datum, "Datum" );
IMPLEMENT_BONEXTENSION( XTEAM::Inheritance, "Inheritance" );
IMPLEMENT_BONEXTENSION( XTEAM::Start, "Start" );
IMPLEMENT_BONEXTENSION( XTEAM::BaseInheritance, "BaseInheritance" );
IMPLEMENT_BONEXTENSION( XTEAM::Choose, "Choose" );
IMPLEMENT_BONEXTENSION( XTEAM::Containment, "Containment" );
IMPLEMENT_BONEXTENSION( XTEAM::Continue, "Continue" );
IMPLEMENT_BONEXTENSION( XTEAM::DerivedInheritance, "DerivedInheritance" );
IMPLEMENT_BONEXTENSION( XTEAM::ElseBranch, "ElseBranch" );
IMPLEMENT_BONEXTENSION( XTEAM::IfBranch, "IfBranch" );
IMPLEMENT_BONEXTENSION( XTEAM::Initiate, "Initiate" );
IMPLEMENT_BONEXTENSION( XTEAM::InterfaceMapping, "InterfaceMapping" );
IMPLEMENT_BONEXTENSION( XTEAM::Link, "Link" );
IMPLEMENT_BONEXTENSION( XTEAM::Read, "Read" );
IMPLEMENT_BONEXTENSION( XTEAM::Transition, "Transition" );
IMPLEMENT_BONEXTENSION( XTEAM::Write, "Write" );
IMPLEMENT_BONEXTENSION( XTEAM::Group, "Group" );
IMPLEMENT_BONEXTENSION( XTEAM::Host, "Host" );
IMPLEMENT_BONEXTENSION( XTEAM::PortType, "PortType" );
IMPLEMENT_BONEXTENSION( XTEAM::ResourceType, "ResourceType" );
IMPLEMENT_BONEXTENSION( XTEAM::ResourceUsage, "ResourceUsage" );


}; // end namespace BON

namespace XTEAM
{
//********************************************************************************
// getter for kind "Architecture"
//********************************************************************************
std::set<Architecture> ArchitectureFolderImpl::getArchitecture()
{
	std::set<Architecture> res;
	std::set<BON::Object> kinds = getChildObjects("Architecture");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Architecture elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "ArchitectureFolder"
//********************************************************************************
std::set<ArchitectureFolder> ArchitectureFolderImpl::getArchitectureFolder()
{
	std::set<ArchitectureFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		ArchitectureFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "DataModelsFolder"
//********************************************************************************
std::set<DataModelsFolder> ArchitectureFolderImpl::getDataModelsFolder()
{
	std::set<DataModelsFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		DataModelsFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "ElementsFolder"
//********************************************************************************
std::set<ElementsFolder> ArchitectureFolderImpl::getElementsFolder()
{
	std::set<ElementsFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		ElementsFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "InterfacesFolder"
//********************************************************************************
std::set<InterfacesFolder> ArchitectureFolderImpl::getInterfacesFolder()
{
	std::set<InterfacesFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		InterfacesFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "DataModel"
//********************************************************************************
std::set<DataModel> DataModelsFolderImpl::getDataModel()
{
	std::set<DataModel> res;
	std::set<BON::Object> kinds = getChildObjects("DataModel");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		DataModel elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "DataModelsFolder"
//********************************************************************************
std::set<DataModelsFolder> DataModelsFolderImpl::getDataModelsFolder()
{
	std::set<DataModelsFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		DataModelsFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Component"
//********************************************************************************
std::set<Component> ElementsFolderImpl::getComponent()
{
	std::set<Component> res;
	std::set<BON::Object> kinds = getChildObjects("Component");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Component elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Connector"
//********************************************************************************
std::set<Connector> ElementsFolderImpl::getConnector()
{
	std::set<Connector> res;
	std::set<BON::Object> kinds = getChildObjects("Connector");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Connector elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for kind "Element" and its descendants
//********************************************************************************
std::set<Element> ElementsFolderImpl::getElement()
{
	std::set<Element> res;
	const int len = 2;
	std::set<BON::Object> kinds_vec[ len];
	kinds_vec[0] = getChildObjects("Component");
	kinds_vec[1] = getChildObjects("Connector");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::Object>::iterator i = kinds_vec[k].begin(); i != kinds_vec[k].end(); ++i)
		{
			Element elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for subfolder "ElementsFolder"
//********************************************************************************
std::set<ElementsFolder> ElementsFolderImpl::getElementsFolder()
{
	std::set<ElementsFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		ElementsFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for kind "Interface"
//********************************************************************************
std::set<Interface> InterfacesFolderImpl::getInterface()
{
	std::set<Interface> res;
	std::set<BON::Object> kinds = getChildObjects("Interface");
	for( std::set<BON::Object>::iterator i = kinds.begin(); i != kinds.end(); ++i)
	{
		Interface elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for subfolder "InterfacesFolder"
//********************************************************************************
std::set<InterfacesFolder> InterfacesFolderImpl::getInterfacesFolder()
{
	std::set<InterfacesFolder> res;
	std::set<BON::Folder> l = getChildFolders();
	for( std::set<BON::Folder>::iterator i = l.begin(); i != l.end(); ++i)
	{
		InterfacesFolder elem(*i);
		if (elem)
			res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::string ElementImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void ElementImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// getter for role "Architecture" among "Architecture"s
//********************************************************************************
std::set<Architecture> ElementImpl::getArchitecture()
{
	std::set<Architecture> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Architecture");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Architecture elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Interface" among "Interface"s
//********************************************************************************
std::set<Interface> ElementImpl::getInterface()
{
	std::set<Interface> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Interface");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Interface elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InterfaceMapping" among "InterfaceMapping"s
//********************************************************************************
std::set<InterfaceMapping> ElementImpl::getInterfaceMapping()
{
	std::set<InterfaceMapping> res;
	std::set<BON::FCO> roles = getChildFCOsAs("InterfaceMapping");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InterfaceMapping elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Process" among "Process"s
//********************************************************************************

std::set<Process> ElementImpl::getProcess()
{
	std::set<Process> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Process");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Process elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Resource" among "Resource"s
//********************************************************************************
std::set<Resource> ElementImpl::getResource()
{
	std::set<Resource> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Resource");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Resource elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "PortType" among "PortType"s
//********************************************************************************
std::set<PortType> PortImpl::getPortType()
{
	std::set<PortType> res;
	std::set<BON::FCO> roles = getChildFCOsAs("PortType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		PortType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::string ArchitectureImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void ArchitectureImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// getter for role "Component" among "Element"s and its descendants
//********************************************************************************
std::set<Component> ArchitectureImpl::getComponent()
{
	std::set<Component> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Component");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Component elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Connector" among "Element"s and its descendants
//********************************************************************************
std::set<Connector> ArchitectureImpl::getConnector()
{
	std::set<Connector> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Connector");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Connector elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Element"s and its descendants
//********************************************************************************
std::set<Element> ArchitectureImpl::getElement()
{
	std::set<Element> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Component");
	roles_vec[1] = getChildFCOsAs("Connector");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Element elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "Group" among "Group"s
//********************************************************************************
std::set<Group> ArchitectureImpl::getGroup()
{
	std::set<Group> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Group");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Group elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Host" among "Host"s
//********************************************************************************
std::set<Host> ArchitectureImpl::getHost()
{
	std::set<Host> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Host");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Host elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Interface" among "Interface"s
//********************************************************************************
std::set<Interface> ArchitectureImpl::getInterface()
{
	std::set<Interface> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Interface");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Interface elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "InterfaceMapping" among "InterfaceMapping"s
//********************************************************************************
std::set<InterfaceMapping> ArchitectureImpl::getInterfaceMapping()
{
	std::set<InterfaceMapping> res;
	std::set<BON::FCO> roles = getChildFCOsAs("InterfaceMapping");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		InterfaceMapping elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Link" among "Link"s
//********************************************************************************
std::set<Link> ArchitectureImpl::getLink()
{
	std::set<Link> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Link");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Link elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "BaseInheritance" among "BaseInheritance"s
//********************************************************************************
std::set<BaseInheritance> DataModelImpl::getBaseInheritance()
{
	std::set<BaseInheritance> res;
	std::set<BON::FCO> roles = getChildFCOsAs("BaseInheritance");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		BaseInheritance elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Containment" among "Containment"s
//********************************************************************************
std::set<Containment> DataModelImpl::getContainment()
{
	std::set<Containment> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Containment");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Containment elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Datum" among "Datum"s
//********************************************************************************
std::set<Datum> DataModelImpl::getDatum()
{
	std::set<Datum> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Datum");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Datum elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "DerivedInheritance" among "DerivedInheritance"s
//********************************************************************************
std::set<DerivedInheritance> DataModelImpl::getDerivedInheritance()
{
	std::set<DerivedInheritance> res;
	std::set<BON::FCO> roles = getChildFCOsAs("DerivedInheritance");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		DerivedInheritance elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Inheritance" among "Inheritance"s
//********************************************************************************
std::set<Inheritance> DataModelImpl::getInheritance()
{
	std::set<Inheritance> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Inheritance");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Inheritance elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Link> InterfaceImpl::getInLinkLinks()
{
	std::set<Link> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Link c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Interfaces
//********************************************************************************
std::multiset<Interface> InterfaceImpl::getInterfaceMappingDsts()
{
	std::multiset<Interface> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("InterfaceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Interface dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src and dst Interfaces
//********************************************************************************
std::multiset<Interface> InterfaceImpl::getInterfaceMappingEnds()
{
	std::multiset<Interface> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("InterfaceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Interface dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("InterfaceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cot = out_ends.begin() ; cot != out_ends.end() ; ++cot )
		{
			Interface dst( *cot );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// 
//********************************************************************************
std::set<InterfaceMapping> InterfaceImpl::getInterfaceMappingLinks()
{
	std::set<InterfaceMapping> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		InterfaceMapping c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Interfaces
//********************************************************************************
std::multiset<Interface> InterfaceImpl::getInterfaceMappingSrcs()
{
	std::multiset<Interface> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("InterfaceMapping");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Interface dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Interfaces
//********************************************************************************
std::multiset<Interface> InterfaceImpl::getLinkDsts()
{
	std::multiset<Interface> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Link");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Interface dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Link> InterfaceImpl::getLinkLinks()
{
	std::set<Link> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Link c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Interfaces
//********************************************************************************
std::multiset<Interface> InterfaceImpl::getLinkSrcs()
{
	std::multiset<Interface> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Link");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Interface dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Link> InterfaceImpl::getOutLinkLinks()
{
	std::set<Link> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Link c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::string InterfaceImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
InterfaceImpl::direction_Type InterfaceImpl::getdirection()
{
	std::string val = FCOImpl::getAttribute("direction")->getStringValue();

	if ( val == "none") return none_direction_Type;
	else if ( val == "in") return in_direction_Type;
	else if ( val == "out") return out_direction_Type;
	else if ( val == "inout") return inout_direction_Type;
	else throw("None of the possible items");
}


//********************************************************************************
// 
//********************************************************************************
std::string InterfaceImpl::getinvocationEnergyCost() 
{
	return FCOImpl::getAttribute("invocationEnergyCost")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void InterfaceImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void InterfaceImpl::setdirection( InterfaceImpl::direction_Type val)
{
	std::string str_val = "";

	if ( val == none_direction_Type) str_val = "none";
	else if ( val == in_direction_Type) str_val = "in";
	else if ( val == out_direction_Type) str_val = "out";
	else if ( val == inout_direction_Type) str_val = "inout";
	else throw("None of the possible items");

	FCOImpl::getAttribute("direction")->setStringValue( str_val);
}


//********************************************************************************
// 
//********************************************************************************
void InterfaceImpl::setinvocationEnergyCost( const std::string& val)
{
	FCOImpl::getAttribute("invocationEnergyCost")->setStringValue( val);
}


//********************************************************************************
// getter for role "Input" among "Port"s and its descendants
//********************************************************************************
std::set<Input> InterfaceImpl::getInput()
{
	std::set<Input> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Input");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Input elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Output" among "Port"s and its descendants
//********************************************************************************
std::set<Output> InterfaceImpl::getOutput()
{
	std::set<Output> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Output");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Output elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Port"s and its descendants
//********************************************************************************
std::set<Port> InterfaceImpl::getPort()
{
	std::set<Port> res;
	const int len = 2;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Input");
	roles_vec[1] = getChildFCOsAs("Output");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Port elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
bool ProcessImpl::isinitialProcess() 
{
	return FCOImpl::getAttribute("initialProcess")->getBooleanValue();
}


//********************************************************************************
// 
//********************************************************************************
void ProcessImpl::setinitialProcess( bool val)
{
	FCOImpl::getAttribute("initialProcess")->setBooleanValue( val);
}

//********************************************************************************
// getter for role "Choice" among "Initiator"s and its descendants
//********************************************************************************
std::set<Choice> ProcessImpl::getChoice()
{
	std::set<Choice> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Choice");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Choice elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Choose" among "Choose"s
//********************************************************************************
std::set<Choose> ProcessImpl::getChoose()
{
	std::set<Choose> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Choose");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Choose elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Conditional" among "Initiator"s and its descendants
//********************************************************************************
std::set<Conditional> ProcessImpl::getConditional()
{
	std::set<Conditional> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Conditional");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Conditional elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Continue" among "Continue"s
//********************************************************************************
std::set<Continue> ProcessImpl::getContinue()
{
	std::set<Continue> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Continue");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Continue elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ElseBranch" among "ElseBranch"s
//********************************************************************************
std::set<ElseBranch> ProcessImpl::getElseBranch()
{
	std::set<ElseBranch> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ElseBranch");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ElseBranch elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Event" among "Initiator"s and its descendants
//********************************************************************************
std::set<Event> ProcessImpl::getEvent()
{
	std::set<Event> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Event");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Event elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}

//********************************************************************************
// getter for Output Events
//********************************************************************************
std::set<Event> ProcessImpl::getOutputEvent()
{
	std::set<Event> res; 
	std::set<Event> events = getEvent();
	for( std::set<Event>::iterator it = events.begin(); it != events.end(); ++it)
	{
		Event elem(*it);
		ASSERT(elem);

		// checks if the inside Port is outbound
		Output event_output (elem->getPort ());
		if (event_output) {
			res.insert(elem);
		}
	}
	return res;
}

//********************************************************************************
// getter for Input Events
//********************************************************************************
std::set<Event> ProcessImpl::getInputEvent()
{
	std::set<Event> res; 
	std::set<Event> events = getEvent();
	for( std::set<Event>::iterator it = events.begin(); it != events.end(); ++it)
	{
		Event elem(*it);
		ASSERT(elem);

		// checks if the inside Port is outbound
		Input event_input (elem->getPort ());
		if (event_input) {
			res.insert(elem);
		}
	}
	return res;
}

//********************************************************************************
// getter for role "IfBranch" among "IfBranch"s
//********************************************************************************
std::set<IfBranch> ProcessImpl::getIfBranch()
{
	std::set<IfBranch> res;
	std::set<BON::FCO> roles = getChildFCOsAs("IfBranch");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		IfBranch elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Initiate" among "Initiate"s
//********************************************************************************
std::set<Initiate> ProcessImpl::getInitiate()
{
	std::set<Initiate> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Initiate");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Initiate elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// aggregated getter for role "" among "Initiator"s and its descendants
//********************************************************************************
std::set<Initiator> ProcessImpl::getInitiator()
{
	std::set<Initiator> res;
	const int len = 5;
	std::set<BON::FCO> roles_vec[ len];
	roles_vec[0] = getChildFCOsAs("Choice");
	roles_vec[1] = getChildFCOsAs("Conditional");
	roles_vec[2] = getChildFCOsAs("Stop");
	roles_vec[3] = getChildFCOsAs("Event");
	roles_vec[4] = getChildFCOsAs("ProcessTransition");
	for( int k = 0; k < len; ++k)
		for( std::set<BON::FCO>::iterator i = roles_vec[k].begin(); i != roles_vec[k].end(); ++i)
		{
			Initiator elem(*i);
			ASSERT(elem);
			res.insert(elem);
		}
	return res;
}


//********************************************************************************
// getter for role "ProcessTransition" among "Initiator"s and its descendants
//********************************************************************************
std::set<ProcessTransition> ProcessImpl::getProcessTransition()
{
	std::set<ProcessTransition> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ProcessTransition");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ProcessTransition elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Read" among "Read"s
//********************************************************************************
std::set<Read> ProcessImpl::getRead()
{
	std::set<Read> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Read");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Read elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "ResourceUsage" among "ResourceUsage"s
//********************************************************************************
std::set<ResourceUsage> ProcessImpl::getResourceUsage()
{
	std::set<ResourceUsage> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ResourceUsage");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ResourceUsage elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Start" among "Start"s
//********************************************************************************
std::set<Start> ProcessImpl::getStart()
{
	std::set<Start> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Start");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Start elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Stop" among "Initiator"s and its descendants
//********************************************************************************
std::set<Stop> ProcessImpl::getStop()
{
	std::set<Stop> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Stop");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Stop elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Task" among "Task"s
//********************************************************************************
std::set<Task> ProcessImpl::getTask()
{
	std::set<Task> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Task");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Task elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Transition" among "Transition"s
//********************************************************************************
std::set<Transition> ProcessImpl::getTransition()
{
	std::set<Transition> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Transition");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Transition elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// getter for role "Write" among "Write"s
//********************************************************************************
std::set<Write> ProcessImpl::getWrite()
{
	std::set<Write> res;
	std::set<BON::FCO> roles = getChildFCOsAs("Write");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		Write elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::string ResourceImpl::getinitialValue() 
{
	return FCOImpl::getAttribute("initialValue")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
ResourceImpl::scope_Type ResourceImpl::getscope()
{
	std::string val = FCOImpl::getAttribute("scope")->getStringValue();

	if ( val == "Global") return Global_scope_Type;
	else if ( val == "Local") return Local_scope_Type;
	else throw("None of the possible items");
}


//********************************************************************************
// 
//********************************************************************************
void ResourceImpl::setinitialValue( const std::string& val)
{
	FCOImpl::getAttribute("initialValue")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void ResourceImpl::setscope( ResourceImpl::scope_Type val)
{
	std::string str_val = "";

	if ( val == Global_scope_Type) str_val = "Global";
	else if ( val == Local_scope_Type) str_val = "Local";
	else throw("None of the possible items");

	FCOImpl::getAttribute("scope")->setStringValue( str_val);
}


//********************************************************************************
// getter for role "ResourceType" among "ResourceType"s
//********************************************************************************
std::set<ResourceType> ResourceImpl::getResourceType()
{
	std::set<ResourceType> res;
	std::set<BON::FCO> roles = getChildFCOsAs("ResourceType");
	for( std::set<BON::FCO>::iterator i = roles.begin(); i != roles.end(); ++i)
	{
		ResourceType elem(*i);
		ASSERT(elem);
		res.insert(elem);
	}
	return res;
}


//********************************************************************************
// returns dst Initiators
//********************************************************************************
std::multiset<Initiator> TaskImpl::getContinueDsts()
{
	std::multiset<Initiator> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Continue");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Initiator dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Initiate> TaskImpl::getInInitiateLinks()
{
	std::set<Initiate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Initiate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Starts
//********************************************************************************
std::multiset<Start> TaskImpl::getInitiateSrcs()
{
	std::multiset<Start> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Initiate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Start dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Continue> TaskImpl::getOutContinueLinks()
{
	std::set<Continue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Continue c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::string TaskImpl::getexecutionTime() 
{
	return FCOImpl::getAttribute("executionTime")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string TaskImpl::getmemoryUsage() 
{
	return FCOImpl::getAttribute("memoryUsage")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string TaskImpl::gettaskInstruction() 
{
	return FCOImpl::getAttribute("taskInstruction")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void TaskImpl::setexecutionTime( const std::string& val)
{
	FCOImpl::getAttribute("executionTime")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void TaskImpl::setmemoryUsage( const std::string& val)
{
	FCOImpl::getAttribute("memoryUsage")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void TaskImpl::settaskInstruction( const std::string& val)
{
	FCOImpl::getAttribute("taskInstruction")->setStringValue( val);
}


//********************************************************************************
// returns src Conditionals
//********************************************************************************
std::multiset<Conditional> BranchEndImpl::getElseBranchSrcs()
{
	std::multiset<Conditional> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("ElseBranch");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Conditional dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src Conditionals
//********************************************************************************
std::multiset<Conditional> BranchEndImpl::getIfBranchSrcs()
{
	std::multiset<Conditional> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("IfBranch");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Conditional dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<ElseBranch> BranchEndImpl::getInElseBranchLinks()
{
	std::set<ElseBranch> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ElseBranch c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<IfBranch> BranchEndImpl::getInIfBranchLinks()
{
	std::set<IfBranch> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		IfBranch c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Tasks
//********************************************************************************
std::multiset<Task> InitiatorImpl::getContinueSrcs()
{
	std::multiset<Task> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Continue");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Task dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Continue> InitiatorImpl::getInContinueLinks()
{
	std::set<Continue> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Continue c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Transition> TransitionEndImpl::getInTransitionLinks()
{
	std::set<Transition> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Transition c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Events
//********************************************************************************
std::multiset<Event> TransitionEndImpl::getTransitionSrcs()
{
	std::multiset<Event> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Transition");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Event dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Events
//********************************************************************************
std::multiset<Event> ChoiceImpl::getChooseDsts()
{
	std::multiset<Event> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Choose");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Event dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Choose> ChoiceImpl::getOutChooseLinks()
{
	std::set<Choose> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Choose c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Choices
//********************************************************************************
std::multiset<Choice> EventImpl::getChooseSrcs()
{
	std::multiset<Choice> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Choose");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Choice dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Choose> EventImpl::getInChooseLinks()
{
	std::set<Choose> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Choose c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Read> EventImpl::getInReadLinks()
{
	std::set<Read> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Read c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Transition> EventImpl::getOutTransitionLinks()
{
	std::set<Transition> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Transition c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Write> EventImpl::getOutWriteLinks()
{
	std::set<Write> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Write c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src ResourceUsages
//********************************************************************************
std::multiset<ResourceUsage> EventImpl::getReadSrcs()
{
	std::multiset<ResourceUsage> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Read");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			ResourceUsage dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst TransitionEnds
//********************************************************************************
std::multiset<TransitionEnd> EventImpl::getTransitionDsts()
{
	std::multiset<TransitionEnd> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Transition");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			TransitionEnd dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst ResourceUsages
//********************************************************************************
std::multiset<ResourceUsage> EventImpl::getWriteDsts()
{
	std::multiset<ResourceUsage> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Write");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			ResourceUsage dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
Port EventImpl::getPort()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Port(r);
}


//********************************************************************************
// returns dst BranchEnds
//********************************************************************************
std::multiset<BranchEnd> ConditionalImpl::getElseBranchDsts()
{
	std::multiset<BranchEnd> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("ElseBranch");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BranchEnd dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst BranchEnds
//********************************************************************************
std::multiset<BranchEnd> ConditionalImpl::getIfBranchDsts()
{
	std::multiset<BranchEnd> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("IfBranch");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			BranchEnd dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}

//********************************************************************************
// returns the entire connections
//********************************************************************************
std::set<BON::Connection> ConditionalImpl::getOutBranchLinks()
{
	return ConnectionEndImpl::getOutConnLinks();
}


//********************************************************************************
// 
//********************************************************************************
std::set<ElseBranch> ConditionalImpl::getOutElseBranchLinks()
{
	std::set<ElseBranch> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		ElseBranch c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<IfBranch> ConditionalImpl::getOutIfBranchLinks()
{
	std::set<IfBranch> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		IfBranch c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
Process ProcessTransitionImpl::getProcess()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Process(r);
}


//********************************************************************************
// returns dst Inheritances
//********************************************************************************
std::multiset<Inheritance> DatumImpl::getBaseInheritanceDsts()
{
	std::multiset<Inheritance> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("BaseInheritance");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Inheritance dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Datums
//********************************************************************************
std::multiset<Datum> DatumImpl::getContainmentDsts()
{
	std::multiset<Datum> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Containment");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Datum dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Containment> DatumImpl::getContainmentLinks()
{
	std::set<Containment> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Containment c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns src Datums
//********************************************************************************
std::multiset<Datum> DatumImpl::getContainmentSrcs()
{
	std::multiset<Datum> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Containment");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Datum dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src Inheritances
//********************************************************************************
std::multiset<Inheritance> DatumImpl::getDerivedInheritanceSrcs()
{
	std::multiset<Inheritance> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("DerivedInheritance");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Inheritance dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Containment> DatumImpl::getInContainmentLinks()
{
	std::set<Containment> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Containment c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<DerivedInheritance> DatumImpl::getInDerivedInheritanceLinks()
{
	std::set<DerivedInheritance> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		DerivedInheritance c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<BaseInheritance> DatumImpl::getOutBaseInheritanceLinks()
{
	std::set<BaseInheritance> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		BaseInheritance c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Containment> DatumImpl::getOutContainmentLinks()
{
	std::set<Containment> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Containment c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::string DatumImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string DatumImpl::getsize() 
{
	return FCOImpl::getAttribute("size")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string DatumImpl::gettype() 
{
	return FCOImpl::getAttribute("type")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
std::string DatumImpl::getvalue() 
{
	return FCOImpl::getAttribute("value")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void DatumImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void DatumImpl::setsize( const std::string& val)
{
	FCOImpl::getAttribute("size")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void DatumImpl::settype( const std::string& val)
{
	FCOImpl::getAttribute("type")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void DatumImpl::setvalue( const std::string& val)
{
	FCOImpl::getAttribute("value")->setStringValue( val);
}


//********************************************************************************
// returns src Datums
//********************************************************************************
std::multiset<Datum> InheritanceImpl::getBaseInheritanceSrcs()
{
	std::multiset<Datum> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("BaseInheritance");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Datum dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns dst Datums
//********************************************************************************
std::multiset<Datum> InheritanceImpl::getDerivedInheritanceDsts()
{
	std::multiset<Datum> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("DerivedInheritance");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Datum dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<BaseInheritance> InheritanceImpl::getInBaseInheritanceLinks()
{
	std::set<BaseInheritance> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		BaseInheritance c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<DerivedInheritance> InheritanceImpl::getOutDerivedInheritanceLinks()
{
	std::set<DerivedInheritance> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		DerivedInheritance c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Tasks
//********************************************************************************
std::multiset<Task> StartImpl::getInitiateDsts()
{
	std::multiset<Task> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Initiate");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Task dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Initiate> StartImpl::getOutInitiateLinks()
{
	std::set<Initiate> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Initiate c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Inheritance
//********************************************************************************
Inheritance BaseInheritanceImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Inheritance sp( ce);
	if ( sp)
		return sp;

	Inheritance empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Datum
//********************************************************************************
Datum BaseInheritanceImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Datum sp( ce);
	if ( sp)
		return sp;

	Datum empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Event
//********************************************************************************
Event ChooseImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Event sp( ce);
	if ( sp)
		return sp;

	Event empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Choice
//********************************************************************************
Choice ChooseImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Choice sp( ce);
	if ( sp)
		return sp;

	Choice empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Datum
//********************************************************************************
Datum ContainmentImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Datum sp( ce);
	if ( sp)
		return sp;

	Datum empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Datum
//********************************************************************************
Datum ContainmentImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Datum sp( ce);
	if ( sp)
		return sp;

	Datum empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Initiator
//********************************************************************************
Initiator ContinueImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Initiator sp( ce);
	if ( sp)
		return sp;

	Initiator empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Task
//********************************************************************************
Task ContinueImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Task sp( ce);
	if ( sp)
		return sp;

	Task empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Datum
//********************************************************************************
Datum DerivedInheritanceImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Datum sp( ce);
	if ( sp)
		return sp;

	Datum empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Inheritance
//********************************************************************************
Inheritance DerivedInheritanceImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Inheritance sp( ce);
	if ( sp)
		return sp;

	Inheritance empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BranchEnd
//********************************************************************************
BranchEnd ElseBranchImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BranchEnd sp( ce);
	if ( sp)
		return sp;

	BranchEnd empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Conditional
//********************************************************************************
Conditional ElseBranchImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Conditional sp( ce);
	if ( sp)
		return sp;

	Conditional empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
std::string IfBranchImpl::getexpression() 
{
	return FCOImpl::getAttribute("expression")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void IfBranchImpl::setexpression( const std::string& val)
{
	FCOImpl::getAttribute("expression")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to BranchEnd
//********************************************************************************
BranchEnd IfBranchImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	BranchEnd sp( ce);
	if ( sp)
		return sp;

	BranchEnd empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Conditional
//********************************************************************************
Conditional IfBranchImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Conditional sp( ce);
	if ( sp)
		return sp;

	Conditional empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Task
//********************************************************************************
Task InitiateImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Task sp( ce);
	if ( sp)
		return sp;

	Task empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Start
//********************************************************************************
Start InitiateImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Start sp( ce);
	if ( sp)
		return sp;

	Start empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Interface
//********************************************************************************
Interface InterfaceMappingImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Interface sp( ce);
	if ( sp)
		return sp;

	Interface empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Interface
//********************************************************************************
Interface InterfaceMappingImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Interface sp( ce);
	if ( sp)
		return sp;

	Interface empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
std::string LinkImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
void LinkImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Interface
//********************************************************************************
Interface LinkImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Interface sp( ce);
	if ( sp)
		return sp;

	Interface empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Interface
//********************************************************************************
Interface LinkImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Interface sp( ce);
	if ( sp)
		return sp;

	Interface empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to Event
//********************************************************************************
Event ReadImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	Event sp( ce);
	if ( sp)
		return sp;

	Event empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to ResourceUsage
//********************************************************************************
ResourceUsage ReadImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	ResourceUsage sp( ce);
	if ( sp)
		return sp;

	ResourceUsage empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to TransitionEnd
//********************************************************************************
TransitionEnd TransitionImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	TransitionEnd sp( ce);
	if ( sp)
		return sp;

	TransitionEnd empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Event
//********************************************************************************
Event TransitionImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Event sp( ce);
	if ( sp)
		return sp;

	Event empty;
	return empty;
}


//********************************************************************************
// getDst() return value is a ConnectionEnd casted to ResourceUsage
//********************************************************************************
ResourceUsage WriteImpl::getDst()
{
	BON::ConnectionEnd ce = ConnectionImpl::getDst();
	ResourceUsage sp( ce);
	if ( sp)
		return sp;

	ResourceUsage empty;
	return empty;
}


//********************************************************************************
// getSrc() return value is a ConnectionEnd casted to Event
//********************************************************************************
Event WriteImpl::getSrc()
{
	BON::ConnectionEnd ce = ConnectionImpl::getSrc();
	Event sp( ce);
	if ( sp)
		return sp;

	Event empty;
	return empty;
}


//********************************************************************************
// 
//********************************************************************************
std::string GroupImpl::getdescription() 
{
	return FCOImpl::getAttribute("description")->getStringValue();
}


//********************************************************************************
// 
//********************************************************************************
long GroupImpl::getthreadPoolSize() 
{
	return FCOImpl::getAttribute("threadPoolSize")->getIntegerValue();
}


//********************************************************************************
// 
//********************************************************************************
void GroupImpl::setdescription( const std::string& val)
{
	FCOImpl::getAttribute("description")->setStringValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void GroupImpl::setthreadPoolSize( const long val)
{
	FCOImpl::getAttribute("threadPoolSize")->setIntegerValue( val);
}


//********************************************************************************
// specialized getter for Component setmembers
//********************************************************************************
std::set<Component> GroupImpl::getComponents()
{
	std::set<Component> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Component r( *elem);
		if ( r && r->getObjectMeta().name() == "Component")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for Connector setmembers
//********************************************************************************
std::set<Connector> GroupImpl::getConnectors()
{
	std::set<Connector> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Connector r( *elem);
		if ( r && r->getObjectMeta().name() == "Connector")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// aggregated getter for setmembers
//********************************************************************************
std::set<Element> GroupImpl::getElements( int dummy)
{
	std::set<Element> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Element r( *elem);
		if ( r)
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
double HostImpl::getreceiveEnergyCost() 
{
	return FCOImpl::getAttribute("receiveEnergyCost")->getRealValue();
}


//********************************************************************************
// 
//********************************************************************************
double HostImpl::getreceiveEnergyOverhead() 
{
	return FCOImpl::getAttribute("receiveEnergyOverhead")->getRealValue();
}


//********************************************************************************
// 
//********************************************************************************
double HostImpl::gettransmitEnergyCost() 
{
	return FCOImpl::getAttribute("transmitEnergyCost")->getRealValue();
}


//********************************************************************************
// 
//********************************************************************************
double HostImpl::gettransmitEnergyOverhead() 
{
	return FCOImpl::getAttribute("transmitEnergyOverhead")->getRealValue();
}


//********************************************************************************
// 
//********************************************************************************
void HostImpl::setreceiveEnergyCost( const double val)
{
	FCOImpl::getAttribute("receiveEnergyCost")->setRealValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void HostImpl::setreceiveEnergyOverhead( const double val)
{
	FCOImpl::getAttribute("receiveEnergyOverhead")->setRealValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void HostImpl::settransmitEnergyCost( const double val)
{
	FCOImpl::getAttribute("transmitEnergyCost")->setRealValue( val);
}


//********************************************************************************
// 
//********************************************************************************
void HostImpl::settransmitEnergyOverhead( const double val)
{
	FCOImpl::getAttribute("transmitEnergyOverhead")->setRealValue( val);
}


//********************************************************************************
// specialized getter for Component setmembers
//********************************************************************************
std::set<Component> HostImpl::getComponents()
{
	std::set<Component> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Component r( *elem);
		if ( r && r->getObjectMeta().name() == "Component")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// specialized getter for Connector setmembers
//********************************************************************************
std::set<Connector> HostImpl::getConnectors()
{
	std::set<Connector> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Connector r( *elem);
		if ( r && r->getObjectMeta().name() == "Connector")
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// aggregated getter for setmembers
//********************************************************************************
std::set<Element> HostImpl::getElements( int dummy)
{
	std::set<Element> res;
	std::set<BON::FCO> elems = BON::SetImpl::getMembers();
	std::set<BON::FCO>::iterator elem = elems.begin();
	for( ; elem != elems.end(); ++elem)
	{
		Element r( *elem);
		if ( r)
			res.insert( r);
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
Datum PortTypeImpl::getDatum()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Datum(r);
}


//********************************************************************************
// 
//********************************************************************************
Datum ResourceTypeImpl::getDatum()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Datum(r);
}


//********************************************************************************
// 
//********************************************************************************
std::set<Write> ResourceUsageImpl::getInWriteLinks()
{
	std::set<Write> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getInConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Write c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// 
//********************************************************************************
std::set<Read> ResourceUsageImpl::getOutReadLinks()
{
	std::set<Read> result;
	std::set<BON::Connection> conns = ConnectionEndImpl::getOutConnLinks();
	std::set<BON::Connection>::iterator it = conns.begin();
	for( ; it != conns.end(); ++it)
	{
		Read c( *it);
		if (c)
			result.insert( c);
	}
	return result;
}


//********************************************************************************
// returns dst Events
//********************************************************************************
std::multiset<Event> ResourceUsageImpl::getReadDsts()
{
	std::multiset<Event> res;
	{
		std::multiset<BON::ConnectionEnd> out_ends = BON::ConnectionEndImpl::getOutConnEnds("Read");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = out_ends.begin() ; cit != out_ends.end() ; ++cit )
		{
			Event dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// returns src Events
//********************************************************************************
std::multiset<Event> ResourceUsageImpl::getWriteSrcs()
{
	std::multiset<Event> res;
	{
		std::multiset<BON::ConnectionEnd> in_ends = BON::ConnectionEndImpl::getInConnEnds("Write");
		for ( std::multiset<BON::ConnectionEnd>::iterator cit = in_ends.begin() ; cit != in_ends.end() ; ++cit )
		{
			Event dst( *cit );
			ASSERT(dst);
			res.insert( dst);
		}
	}
	return res;
}


//********************************************************************************
// 
//********************************************************************************
Resource ResourceUsageImpl::getResource()
{
	BON::FCO r = BON::ReferenceImpl::getReferred();
	return Resource(r);
}



}; // namespace XTEAM

