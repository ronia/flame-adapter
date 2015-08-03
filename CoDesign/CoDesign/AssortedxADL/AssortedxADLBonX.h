#ifndef ASSORTEDXADLBONX_H
#define ASSORTEDXADLBONX_H

#include "BON.h"
#include "BONImpl.h"
#include "Extensions.h"

///BUP
#include <iostream>
#include <fstream>
#include <algorithm>
///EUP

namespace XTEAM
{

DECLARE_BONEXTENSION( BON::Folder, ArchitectureFolderImpl, ArchitectureFolder );
DECLARE_BONEXTENSION( BON::Folder, DataModelsFolderImpl, DataModelsFolder );
DECLARE_BONEXTENSION( BON::Folder, ElementsFolderImpl, ElementsFolder );
DECLARE_BONEXTENSION( BON::Folder, InterfacesFolderImpl, InterfacesFolder );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, ElementImpl, Element );
DECLARE_BONEXTENSION( Element, ComponentImpl, Component );
DECLARE_BONEXTENSION( Element, ConnectorImpl, Connector );
DECLARE_ABSTRACT_BONEXTENSION( BON::Model, PortImpl, Port );
DECLARE_BONEXTENSION( Port, InputImpl, Input );
DECLARE_BONEXTENSION( Port, OutputImpl, Output );
DECLARE_BONEXTENSION( BON::Model, ArchitectureImpl, Architecture );
DECLARE_BONEXTENSION( BON::Model, DataModelImpl, DataModel );
DECLARE_BONEXTENSION( BON::Model, InterfaceImpl, Interface );
DECLARE_BONEXTENSION( BON::Model, ProcessImpl, Process );
DECLARE_BONEXTENSION( BON::Model, ResourceImpl, Resource );
DECLARE_BONEXTENSION( BON::Model, TaskImpl, Task );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, BranchEndImpl, BranchEnd );
DECLARE_ABSTRACT_BONEXTENSION( BON::FCO, InitiatorImpl, Initiator );
DECLARE_ABSTRACT_BONEXTENSION( Initiator, TransitionEndImpl, TransitionEnd );
DECLARE_BONEXTENSION2( BON::Atom, Initiator, ChoiceImpl, Choice );
DECLARE_BONEXTENSION2( BON::Reference, Initiator, EventImpl, Event );
DECLARE_BONEXTENSION2( BON::Atom, TransitionEnd, ConditionalImpl, Conditional );
DECLARE_BONEXTENSION3( BON::Atom, BranchEnd, TransitionEnd, StopImpl, Stop );
DECLARE_BONEXTENSION3( BON::Reference, BranchEnd, TransitionEnd, ProcessTransitionImpl, ProcessTransition );
DECLARE_BONEXTENSION( BON::Atom, DatumImpl, Datum );
DECLARE_BONEXTENSION( BON::Atom, InheritanceImpl, Inheritance );
DECLARE_BONEXTENSION( BON::Atom, StartImpl, Start );
DECLARE_BONEXTENSION( BON::Connection, BaseInheritanceImpl, BaseInheritance );
DECLARE_BONEXTENSION( BON::Connection, ChooseImpl, Choose );
DECLARE_BONEXTENSION( BON::Connection, ContainmentImpl, Containment );
DECLARE_BONEXTENSION( BON::Connection, ContinueImpl, Continue );
DECLARE_BONEXTENSION( BON::Connection, DerivedInheritanceImpl, DerivedInheritance );
DECLARE_BONEXTENSION( BON::Connection, ElseBranchImpl, ElseBranch );
DECLARE_BONEXTENSION( BON::Connection, IfBranchImpl, IfBranch );
DECLARE_BONEXTENSION( BON::Connection, InitiateImpl, Initiate );
DECLARE_BONEXTENSION( BON::Connection, InterfaceMappingImpl, InterfaceMapping );
DECLARE_BONEXTENSION( BON::Connection, LinkImpl, Link );
DECLARE_BONEXTENSION( BON::Connection, ReadImpl, Read );
DECLARE_BONEXTENSION( BON::Connection, TransitionImpl, Transition );
DECLARE_BONEXTENSION( BON::Connection, WriteImpl, Write );
DECLARE_BONEXTENSION( BON::Set, GroupImpl, Group );
DECLARE_BONEXTENSION( BON::Set, HostImpl, Host );
DECLARE_BONEXTENSION( BON::Reference, PortTypeImpl, PortType );
DECLARE_BONEXTENSION( BON::Reference, ResourceTypeImpl, ResourceType );
DECLARE_BONEXTENSION( BON::Reference, ResourceUsageImpl, ResourceUsage );


//*******************************************************************
//   C  L  A  S  S   ArchitectureFolderImpl
//*******************************************************************
class ArchitectureFolderImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Architecture>          getArchitecture();
	virtual std::set<ArchitectureFolder>    getArchitectureFolder();
	virtual std::set<DataModelsFolder>      getDataModelsFolder();
	virtual std::set<ElementsFolder>        getElementsFolder();
	virtual std::set<InterfacesFolder>      getInterfacesFolder();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DataModelsFolderImpl
//*******************************************************************
class DataModelsFolderImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<DataModel>             getDataModel();
	virtual std::set<DataModelsFolder>      getDataModelsFolder();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ElementsFolderImpl
//*******************************************************************
class ElementsFolderImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Component>             getComponent();
	virtual std::set<Connector>             getConnector();
	virtual std::set<Element>               getElement();
	virtual std::set<ElementsFolder>        getElementsFolder();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InterfacesFolderImpl
//*******************************************************************
class InterfacesFolderImpl :
	  public BON::FolderImpl
{
public:
	//
	// kind and subfolder getters
	virtual std::set<Interface>             getInterface();
	virtual std::set<InterfacesFolder>      getInterfacesFolder();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ElementImpl
//*******************************************************************
class ElementImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual void        setdescription( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Architecture>          getArchitecture();
	virtual std::set<Interface>             getInterface();
	virtual std::set<InterfaceMapping>      getInterfaceMapping();
	virtual std::set<Process>               getProcess();
	virtual std::set<Resource>              getResource();

	///BUP
	virtual void generate ();
	virtual std::string get_element_name ();
	virtual std::map<Input, int>& get_event_inputs ();
	virtual std::map<Output, int>& get_event_outputs ();
	virtual std::set<Datum>& get_data_types ();
	virtual Group get_element_group ();
	virtual std::map<Port, Element>& get_linked_elements ();
	virtual void set_element_group (Group& element_group);

private:

	virtual void initialize_element ();
	virtual void generate_header ();
	virtual void generate_implementation ();
	virtual void generate_init_function ();
	virtual void generate_delta_ext ();
	virtual void generate_delta_int ();
	virtual void generate_output_func ();
	virtual void generate_event_handler ();
	virtual void generate_output_generator ();
	virtual void generate_process_transitioner ();
	virtual void write_conditional_state_change (ProcessTransition process_transition, std::string state_name, Event event, std::string indent);
	std::string name_;
	std::ofstream * impl_file_;
	std::set<Process> processes_;
	std::vector<Process> ordered_processes_;
	std::map<Input, int> event_inputs_;
	std::map<Output, int> event_outputs_;
	std::set<Datum> data_types_;
	Group element_group_;
	std::map<Port, Element> linked_elements_;

	virtual void on_input (Process process, Interface intf, Input input, std::ofstream& file) = 0;
	virtual void on_output (Process process, Interface intf, Output output, std::ofstream& file) = 0;
	virtual void on_task_begin (Process process, Task task, std::ofstream& file) = 0;
	virtual void on_task_end (Process process, Task task, std::ofstream& file) = 0;
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ComponentImpl
//*******************************************************************
class ComponentImpl :
	  public ElementImpl
{
public:


	///BUP
	virtual void on_input (Process process, Interface intf, Input input, std::ofstream& file);
	virtual void on_output (Process process, Interface intf, Output output, std::ofstream& file);
	virtual void on_task_begin (Process process, Task task, std::ofstream& file);
	virtual void on_task_end (Process process, Task task, std::ofstream& file);
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConnectorImpl
//*******************************************************************
class ConnectorImpl :
	  public ElementImpl
{
public:


	///BUP
	virtual void on_input (Process process, Interface intf, Input input, std::ofstream& file);
	virtual void on_output (Process process, Interface intf, Output output, std::ofstream& file);
	virtual void on_task_begin (Process process, Task task, std::ofstream& file);
	virtual void on_task_end (Process process, Task task, std::ofstream& file);
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortImpl
//*******************************************************************
class PortImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<PortType>              getPortType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InputImpl
//*******************************************************************
class InputImpl :
	  public PortImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   OutputImpl
//*******************************************************************
class OutputImpl :
	  public PortImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ArchitectureImpl
//*******************************************************************
class ArchitectureImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual void        setdescription( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Component>             getComponent();
	virtual std::set<Connector>             getConnector();
	virtual std::set<Element>               getElement();
	virtual std::set<Group>                 getGroup();
	virtual std::set<Host>                  getHost();
	virtual std::set<Interface>             getInterface();
	virtual std::set<InterfaceMapping>      getInterfaceMapping();
	virtual std::set<Link>                  getLink();

	///BUP
	virtual void generate_top_level_architecture ();
	virtual void generate ();

private:

	virtual void initialize_architecture ();
	virtual void generate_header ();
	virtual void generate_implementation ();
	virtual void generate_main_file ();
	virtual void set_element_groups ();
	std::string name_;
	std::string instance_name_;
	std::set<Element> elements_;
	std::set<Interface> interfaces_;
	std::set<Group> groups_;
	std::map<Input, int> event_inputs_;
	std::map<Output, int> event_outputs_;
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DataModelImpl
//*******************************************************************
class DataModelImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// kind and role getters
	virtual std::set<BaseInheritance>       getBaseInheritance();
	virtual std::set<Containment>           getContainment();
	virtual std::set<Datum>                 getDatum();
	virtual std::set<DerivedInheritance>    getDerivedInheritance();
	virtual std::set<Inheritance>           getInheritance();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InterfaceImpl
//*******************************************************************
class InterfaceImpl :
	  virtual public BON::ModelImpl
{
public:
	typedef enum
	{
		none_direction_Type,
		in_direction_Type,
		out_direction_Type,
		inout_direction_Type
	} direction_Type;

	//
	// connectionEnd getters
	virtual std::set<Link>                  getInLinkLinks();
	virtual std::multiset<Interface>        getInterfaceMappingDsts();
	virtual std::multiset<Interface>        getInterfaceMappingEnds();
	virtual std::set<InterfaceMapping>      getInterfaceMappingLinks();
	virtual std::multiset<Interface>        getInterfaceMappingSrcs();
	virtual std::multiset<Interface>        getLinkDsts();
	virtual std::set<Link>                  getLinkLinks();
	virtual std::multiset<Interface>        getLinkSrcs();
	virtual std::set<Link>                  getOutLinkLinks();
	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual InterfaceImpl::direction_Type   getdirection();
	virtual std::string getinvocationEnergyCost() ;
	virtual void        setdescription( const std::string& val);
	virtual void        setdirection( InterfaceImpl::direction_Type val);
	virtual void        setinvocationEnergyCost( const std::string& val);
	//
	// kind and role getters
	virtual std::set<Input>                 getInput();
	virtual std::set<Output>                getOutput();
	virtual std::set<Port>                  getPort();

	///BUP
	virtual Interface get_mapped_interface ();
	virtual Interface get_exposed_interface ();
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProcessImpl
//*******************************************************************
class ProcessImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// attribute getters and setters
	virtual bool        isinitialProcess() ;
	virtual void        setinitialProcess( bool val);
	//
	// kind and role getters
	virtual std::set<Choice>                getChoice();
	virtual std::set<Choose>                getChoose();
	virtual std::set<Conditional>           getConditional();
	virtual std::set<Continue>              getContinue();
	virtual std::set<ElseBranch>            getElseBranch();
	virtual std::set<Event>                 getEvent();
	virtual std::set<Event>					getOutputEvent();
	virtual std::set<Event>					getInputEvent();
	virtual std::set<IfBranch>              getIfBranch();
	virtual std::set<Initiate>              getInitiate();
	virtual std::set<Initiator>             getInitiator();
	virtual std::set<ProcessTransition>     getProcessTransition();
	virtual std::set<Read>                  getRead();
	virtual std::set<ResourceUsage>         getResourceUsage();
	virtual std::set<Start>                 getStart();
	virtual std::set<Stop>                  getStop();
	virtual std::set<Task>                  getTask();
	virtual std::set<Transition>            getTransition();
	virtual std::set<Write>                 getWrite();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ResourceImpl
//*******************************************************************
class ResourceImpl :
	  virtual public BON::ModelImpl
{
public:
	typedef enum
	{
		Global_scope_Type,
		Local_scope_Type
	} scope_Type;
	//
	// attribute getters and setters
	virtual std::string getinitialValue() ;
	virtual ResourceImpl::scope_Type   getscope();
	virtual void        setinitialValue( const std::string& val);
	virtual void        setscope( ResourceImpl::scope_Type val);
	//
	// kind and role getters
	virtual std::set<ResourceType>          getResourceType();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TaskImpl
//*******************************************************************
class TaskImpl :
	  virtual public BON::ModelImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Initiator>        getContinueDsts();
	virtual std::set<Initiate>              getInInitiateLinks();
	virtual std::multiset<Start>            getInitiateSrcs();
	virtual std::set<Continue>              getOutContinueLinks();
	//
	// attribute getters and setters
	virtual std::string getexecutionTime() ;
	virtual std::string getmemoryUsage() ;
	virtual std::string gettaskInstruction() ;
	virtual void        setexecutionTime( const std::string& val);
	virtual void        setmemoryUsage( const std::string& val);
	virtual void        settaskInstruction( const std::string& val);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BranchEndImpl
//*******************************************************************
class BranchEndImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Conditional>      getElseBranchSrcs();
	virtual std::multiset<Conditional>      getIfBranchSrcs();
	virtual std::set<ElseBranch>            getInElseBranchLinks();
	virtual std::set<IfBranch>              getInIfBranchLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InitiatorImpl
//*******************************************************************
class InitiatorImpl :
	  virtual public BON::FCOImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Task>             getContinueSrcs();
	virtual std::set<Continue>              getInContinueLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TransitionEndImpl
//*******************************************************************
class TransitionEndImpl :
	  public InitiatorImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<Transition>            getInTransitionLinks();
	virtual std::multiset<Event>            getTransitionSrcs();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ChoiceImpl
//*******************************************************************
class ChoiceImpl :
	  virtual public BON::AtomImpl
	, public InitiatorImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Event>            getChooseDsts();
	virtual std::set<Choose>                getOutChooseLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   EventImpl
//*******************************************************************
class EventImpl :
	  virtual public BON::ReferenceImpl
	, public InitiatorImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Choice>           getChooseSrcs();
	virtual std::set<Choose>                getInChooseLinks();
	virtual std::set<Read>                  getInReadLinks();
	virtual std::set<Transition>            getOutTransitionLinks();
	virtual std::set<Write>                 getOutWriteLinks();
	virtual std::multiset<ResourceUsage>    getReadSrcs();
	virtual std::multiset<TransitionEnd>    getTransitionDsts();
	virtual std::multiset<ResourceUsage>    getWriteDsts();
	//
	// ref getters
	virtual Port        getPort();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ConditionalImpl
//*******************************************************************
class ConditionalImpl :
	  virtual public BON::AtomImpl
	, public TransitionEndImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<BranchEnd>        getElseBranchDsts();
	virtual std::multiset<BranchEnd>        getIfBranchDsts();
	virtual std::set<ElseBranch>            getOutElseBranchLinks();
	virtual std::set<IfBranch>              getOutIfBranchLinks();

	virtual std::set<BON::Connection>		getOutBranchLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   StopImpl
//*******************************************************************
class StopImpl :
	  virtual public BON::AtomImpl
	, public BranchEndImpl
	, public TransitionEndImpl
{
public:


	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ProcessTransitionImpl
//*******************************************************************
class ProcessTransitionImpl :
	  virtual public BON::ReferenceImpl
	, public BranchEndImpl
	, public TransitionEndImpl
{
public:

	//
	// ref getters
	virtual Process     getProcess();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DatumImpl
//*******************************************************************
class DatumImpl :
	  virtual public BON::AtomImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Inheritance>      getBaseInheritanceDsts();
	virtual std::multiset<Datum>            getContainmentDsts();
	virtual std::set<Containment>           getContainmentLinks();
	virtual std::multiset<Datum>            getContainmentSrcs();
	virtual std::multiset<Inheritance>      getDerivedInheritanceSrcs();
	virtual std::set<Containment>           getInContainmentLinks();
	virtual std::set<DerivedInheritance>    getInDerivedInheritanceLinks();
	virtual std::set<BaseInheritance>       getOutBaseInheritanceLinks();
	virtual std::set<Containment>           getOutContainmentLinks();
	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual std::string getsize() ;
	virtual std::string gettype() ;
	virtual std::string getvalue() ;
	virtual void        setdescription( const std::string& val);
	virtual void        setsize( const std::string& val);
	virtual void        settype( const std::string& val);
	virtual void        setvalue( const std::string& val);

	///BUP
	virtual void generate ();
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InheritanceImpl
//*******************************************************************
class InheritanceImpl :
	  virtual public BON::AtomImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Datum>            getBaseInheritanceSrcs();
	virtual std::multiset<Datum>            getDerivedInheritanceDsts();
	virtual std::set<BaseInheritance>       getInBaseInheritanceLinks();
	virtual std::set<DerivedInheritance>    getOutDerivedInheritanceLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   StartImpl
//*******************************************************************
class StartImpl :
	  virtual public BON::AtomImpl
{
public:

	//
	// connectionEnd getters
	virtual std::multiset<Task>             getInitiateDsts();
	virtual std::set<Initiate>              getOutInitiateLinks();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   BaseInheritanceImpl
//*******************************************************************
class BaseInheritanceImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Inheritance getDst();
	virtual Datum       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ChooseImpl
//*******************************************************************
class ChooseImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Event       getDst();
	virtual Choice      getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ContainmentImpl
//*******************************************************************
class ContainmentImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Datum       getDst();
	virtual Datum       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ContinueImpl
//*******************************************************************
class ContinueImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Initiator   getDst();
	virtual Task        getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   DerivedInheritanceImpl
//*******************************************************************
class DerivedInheritanceImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Datum       getDst();
	virtual Inheritance getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ElseBranchImpl
//*******************************************************************
class ElseBranchImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual BranchEnd   getDst();
	virtual Conditional getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   IfBranchImpl
//*******************************************************************
class IfBranchImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getexpression() ;
	virtual void        setexpression( const std::string& val);
	//
	// connectionEnd getters
	virtual BranchEnd   getDst();
	virtual Conditional getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InitiateImpl
//*******************************************************************
class InitiateImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Task        getDst();
	virtual Start       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   InterfaceMappingImpl
//*******************************************************************
class InterfaceMappingImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Interface   getDst();
	virtual Interface   getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   LinkImpl
//*******************************************************************
class LinkImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual void        setdescription( const std::string& val);
	//
	// connectionEnd getters
	virtual Interface   getDst();
	virtual Interface   getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ReadImpl
//*******************************************************************
class ReadImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual Event       getDst();
	virtual ResourceUsage                   getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   TransitionImpl
//*******************************************************************
class TransitionImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual TransitionEnd                   getDst();
	virtual Event       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   WriteImpl
//*******************************************************************
class WriteImpl :
	  virtual public BON::ConnectionImpl
{
public:

	//
	// connectionEnd getters
	virtual ResourceUsage                   getDst();
	virtual Event       getSrc();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   GroupImpl
//*******************************************************************
class GroupImpl :
	  virtual public BON::SetImpl
{
public:

	//
	// attribute getters and setters
	virtual std::string getdescription() ;
	virtual long        getthreadPoolSize() ;
	virtual void        setdescription( const std::string& val);
	virtual void        setthreadPoolSize( const long val);
	//
	// set getters
	virtual std::set<Component>             getComponents();
	virtual std::set<Connector>             getConnectors();
	virtual std::set<Element>               getElements( int dummy);

	///BUP
	virtual void generate ();
	virtual std::string get_group_name ();
	virtual void initialize_group ();

private:
	virtual void generate_header ();
	virtual void generate_implementation ();
	std::string name_;
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   HostImpl
//*******************************************************************
class HostImpl :
	  virtual public BON::SetImpl
{
public:

	//
	// attribute getters and setters
	virtual double      getreceiveEnergyCost() ;
	virtual double      getreceiveEnergyOverhead() ;
	virtual double      gettransmitEnergyCost() ;
	virtual double      gettransmitEnergyOverhead() ;
	virtual void        setreceiveEnergyCost( const double val);
	virtual void        setreceiveEnergyOverhead( const double val);
	virtual void        settransmitEnergyCost( const double val);
	virtual void        settransmitEnergyOverhead( const double val);
	//
	// set getters
	virtual std::set<Component>             getComponents();
	virtual std::set<Connector>             getConnectors();
	virtual std::set<Element>               getElements( int dummy);

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   PortTypeImpl
//*******************************************************************
class PortTypeImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Datum       getDatum();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ResourceTypeImpl
//*******************************************************************
class ResourceTypeImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// ref getters
	virtual Datum       getDatum();

	///BUP
	// add your own members here
	///EUP
};


//*******************************************************************
//   C  L  A  S  S   ResourceUsageImpl
//*******************************************************************
class ResourceUsageImpl :
	  virtual public BON::ReferenceImpl
{
public:

	//
	// connectionEnd getters
	virtual std::set<Write>                 getInWriteLinks();
	virtual std::set<Read>                  getOutReadLinks();
	virtual std::multiset<Event>            getReadDsts();
	virtual std::multiset<Event>            getWriteSrcs();
	//
	// ref getters
	virtual Resource    getResource();

	///BUP
	// add your own members here
	///EUP
};



}; // namespace PowerxADL_BON

///BUP
// add your additional class definitions here
///EUP

#endif // XTEAM
