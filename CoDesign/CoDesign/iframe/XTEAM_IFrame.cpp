#include "stdafx.h"
#include "XTEAM_IFrame.h"
#include <sstream>
#include <iostream>
#include <iomanip>

namespace XTEAM
{

/////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////

/**
	gets the GUID and returns a string version of it
*/
std::string serialize_GUID(BON::Object &object) {
	std::stringstream	ss;
	long				guid[4];
	
	// gets the GUID
	object->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);
	
	// serialize the GUID
	ss << std::uppercase << std::hex << guid[0] << "-" << guid[1] << "-" << guid[2] << "-" << guid[3];

	return ss.str();
}

/*
Object GUID comparison function
*/
bool guid_comp (BON::Object &lhs, BON::Object &rhs)
{
	bool ret = false;
	long lhs_guid[4];
	long rhs_guid[4];

	// gets the GUIDs of the two objects
	lhs->getObjectI()->GetGuid(&lhs_guid[0], &lhs_guid[1], &lhs_guid[2], &lhs_guid[3]);
	rhs->getObjectI()->GetGuid(&rhs_guid[0], &rhs_guid[1], &rhs_guid[2], &rhs_guid[3]);

	for(int i=0; i < 4; i++) {
		if(lhs_guid[i] > rhs_guid[i]) {
			ret = false;
			break;
		} else if(lhs_guid[i] < rhs_guid[i]) {
			ret = true;
			break;
		}
	}

	return ret;
}

/*
Object GUID equality function
*/
bool guid_equal (BON::Object &lhs, BON::Object &rhs)
{
	long lhs_guid[4];
	long rhs_guid[4];

	// gets the GUIDs of the two objects
	lhs->getObjectI()->GetGuid(&lhs_guid[0], &lhs_guid[1], &lhs_guid[2], &lhs_guid[3]);
	rhs->getObjectI()->GetGuid(&rhs_guid[0], &rhs_guid[1], &rhs_guid[2], &rhs_guid[3]);

	for(int i=0; i < 4; i++) {
		if(lhs_guid[i] != rhs_guid[i]) {
			return false;
		} 
	}

	return true;
}



/////////////////////////////////////////////////////////
// XTEAM!
/////////////////////////////////////////////////////////


// global version path
static std::string			version_path;

// global syntax error message
static std::string			err_msg;

// global target time for simulation
static int					target_time;

// detects if there is any duplicate port name in the interface
void detect_duplicates (std::vector<BON::Object> &ordered_objects) 
{
	int size = ordered_objects.size();

	std::sort(ordered_objects.begin(), ordered_objects.end(), guid_comp);
	
	// For Input names, checks for duplicate names
	for (int i=0; i < size-1; i++) {
		// Object 1
		BON::Object obj_1 (ordered_objects.at(i));
		ASSERT (obj_1);
		std::string name_1 = obj_1->getName();

		// iterates through the ports
		for (int j=i; j < size; j++) {
			// Port name 2
			BON::Object obj_2 (ordered_objects.at(j));
			ASSERT (obj_2);
			std::string name_2 = obj_2->getName();

			// when input 1 and 2 are different, checks for equal name
			if(!guid_equal (obj_1, obj_2) && name_1 == name_2) {
				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Two ports cannot have the same name
				///////////////////////////////////////////////////////////////
				std::stringstream ss;
				ss	<< obj_1->getName() 
					<< " [" << serialize_GUID(obj_1)
					<< "/" << obj_1->getObjectMeta().name() << "]"
					<< " in "
					<< obj_1->getParent()->getName()
					<< " and "
					<< obj_2->getName() 
					<< " [" << serialize_GUID(obj_2)
					<< "/" << obj_2->getObjectMeta().name() << "]"
					<< " in "
					<< obj_2->getParent()->getName()
					<< " have identical name" << std::endl;
				err_msg += ss.str();	
			}
		}
	}
}

void XTEAM_IFrame::generate (BON::Project& project, BON::FCO& currentFCO, std::string version, int time)
{
	// stores the version name
	version_path				= version + SIMCODEPATH;
	target_time					= time;

	// clears the messages
	err_msg						= "";

	Architecture current_architecture (currentFCO);
	if  (!current_architecture){
		AfxMessageBox ("You must begin interpretation from a Architecture model.");
	} else {
		current_architecture->generate_top_level_architecture ();
	}

	BON::Folder root_folder = project->getRootFolder ();
	std::set<BON::Folder> arch_folders = root_folder->getChildFolders ();
	std::vector<BON::Object> datums;
	for (std::set<BON::Folder>::iterator arch_folder_iter = arch_folders.begin (); arch_folder_iter != arch_folders.end (); arch_folder_iter++)
	{
		ArchitectureFolder next_arch_folder (*arch_folder_iter);
		ASSERT (next_arch_folder);

		std::set<DataModelsFolder> data_folders = next_arch_folder->getDataModelsFolder ();
		for (std::set<DataModelsFolder>::iterator data_folder_iter = data_folders.begin (); data_folder_iter != data_folders.end (); data_folder_iter++)
		{
			DataModelsFolder next_data_folder (*data_folder_iter);
			ASSERT (next_data_folder);

			std::set<DataModel> data_models = next_data_folder->getDataModel ();
			for (std::set<DataModel>::iterator data_model_iter = data_models.begin (); data_model_iter != data_models.end (); data_model_iter++)
			{
				DataModel next_data_model (*data_model_iter);
				ASSERT (next_data_model);

				std::set<Datum> data_objects = next_data_model->getDatum ();
				for (std::set<Datum>::iterator datum_iter = data_objects.begin (); datum_iter != data_objects.end (); datum_iter++)
				{
					Datum next_datum (*datum_iter);
					ASSERT (next_datum);

					next_datum->generate ();

					// checks if the name is "value"
					if(next_datum->getName() == "value")
					{
						///////////////////////////////////////////////////////////////
						// CoDesign Modification
						// Datum's name cannot be "value" as it interferes with compilation
						///////////////////////////////////////////////////////////////
						std::stringstream ss;
						ss	<< next_datum->getName() 
							<< " [" << serialize_GUID(next_datum) 
							<< "/" << next_datum->getObjectMeta().name() << "]"
							<< " in "
							<< next_data_model->getName()
							<< " [" << serialize_GUID(next_data_model)
							<< "/" << next_data_model->getObjectMeta().name() << "]"
							<< " has a prohibited name \"value\"" << std::endl;
						err_msg += ss.str();	
					}
					
					// adds the datum to the vector for the duplicate name checking
					datums.push_back(next_datum);
				}
			}
		}
	}

	// detects duplicate names among datums
	detect_duplicates(datums);

	// stores the syntax errors
	std::string err_path = version + "//syntax_errors.list";
	std::ofstream err_file (err_path.c_str());
	err_file << err_msg;
	err_file.close();
}

void XTEAM_IFrame::sim_gen (XTEAM_Sim_Generator * sim_gen)
{
	sim_gen_ = sim_gen;
}

XTEAM_Sim_Generator * XTEAM_IFrame::sim_gen ()
{
	return sim_gen_;
}

XTEAM_IFrame::XTEAM_IFrame ()
{
}

XTEAM_Sim_Generator * XTEAM_IFrame::sim_gen_ = 0;

/*
Main entry point for the simulation file generator. Invoked on the architecture being interpreted.
*/
void ArchitectureImpl::generate_top_level_architecture ()
{
	// Initialize the simulation generator.
	XTEAM_IFrame::sim_gen ()->initialize (this);
	
	// Recursively provide each element with a reference to its group (thread pool).
	this->set_element_groups ();

	// Recursively generate the header and implementation of this architecture and all
	// sub-elements of this architecture.
	this->generate ();

	// Generate any supplemental files needed for analysis.
	XTEAM_IFrame::sim_gen ()->generate (this);

	// Generate the main () application entry point function.
	this->generate_main_file ();
}

/*
Generates the header and implementation files for this architecture and invokes the generate () method
on all sub-elements.
*/
void ArchitectureImpl::generate ()
{
	// Initialize all the private data members of the ArchitectureImpl class.
	this->initialize_architecture ();


	int cnt = 0;

	// Generate the headers and implementations for all sub-elements.
	for (std::set<Element>::iterator element_iter = this->elements_.begin (); element_iter != this->elements_.end (); element_iter++)
	{
		Element next_element (*element_iter);

		ASSERT (next_element);
		next_element->generate ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// An Element has no assigned Group
		///////////////////////////////////////////////////////////////
		Group group (next_element->get_element_group());
		if(!group) {
			std::stringstream ss;
			ss	<< next_element->getName() 
				<< " [" << serialize_GUID(next_element)
				<< "/" << next_element->getObjectMeta().name() << "]"
				<< " has not been assigned to any Group" << std::endl;
			err_msg += ss.str();
		}

		cnt++;
	}

	// Generate the class header file.
	this->generate_header ();

	// Generate the class implementation.
	this->generate_implementation ();
}

/*
Initializes all the private data members of the ArchitectureImpl class.
*/
void ArchitectureImpl::initialize_architecture ()
{
	std::string architecture_name;
	std::string architecture_id;

	BON::Object architecture_parent = this->getParent ();
	ArchitectureFolder parent_folder (architecture_parent);
	if (parent_folder)
		{
			// Use the name of the architecture for the architecture name.
			architecture_name = this->getName ();
			architecture_id = (this->getID ()).substr (8, 15);
		}
	else
		{
			// Use the name of the parent element for the architecture name.
			architecture_name = this->getParent ()->getName ();
			architecture_id = (this->getParent ()->getID ()).substr (8, 15);
		}

	this->name_ = architecture_name + "_" + architecture_id;
	this->instance_name_ = this->name_ + "_instance";
	this->elements_ = this->getElement ();
	this->interfaces_ = this->getInterface ();
	this->groups_ = this->getGroup ();

	// sets group pointer of every element
	for (std::set<Group>::iterator group_iter = groups_.begin (); group_iter != groups_.end (); group_iter++)
	{
		Group next_group (*group_iter);
		ASSERT (next_group);
		std::set<Element> group_elements = next_group->getElements (0);
		for (std::set<Element>::iterator element_iter = group_elements.begin (); element_iter != group_elements.end (); element_iter++)
		{
			Element next_element (*element_iter);
			ASSERT (next_element);
			next_element->set_element_group (next_group);
		}
	}

	// Insert all the architecture's inputs and outputs into the event_inputs_ and event_outputs_ maps.
	// The port numbers for each input and output will be initialized to 0 (zero) and corrected later.
	for (std::set<Interface>::iterator interface_iter = this->interfaces_.begin (); interface_iter != this->interfaces_.end (); interface_iter++)
		{
			Interface next_interface (*interface_iter);
			ASSERT (next_interface);

			// Insert all the inputs into the input map.
			std::set<Input> inputs = next_interface->getInput ();
			for (std::set<Input>::iterator input_iter = inputs.begin (); input_iter != inputs.end (); input_iter++)
				{
					Input next_input (*input_iter);
					ASSERT (next_input);
					this->event_inputs_[next_input] = 0;
				}

			// Insert all the outputs into the output map.
			std::set<Output> outputs = next_interface->getOutput ();
			for (std::set<Output>::iterator output_iter = outputs.begin (); output_iter != outputs.end (); output_iter++)
				{
					Output next_output (*output_iter);
					ASSERT (next_output);
					this->event_outputs_[next_output] = 0;
				}
		}
}

/*
Generates the header file for the architecture (static digraph) class.
*/
void ArchitectureImpl::generate_header ()
{
	// Open the header file stream.
	std::string header_file_name = version_path + this->name_ + ".h";
	std::ofstream header_file (header_file_name.c_str ());

	// Create a compile variable flag to prevent multiple includes from causing compilation errors.
	std::string define_var_name = this->name_ + "_H";
	std::transform (define_var_name.begin (), define_var_name.end (), define_var_name.begin (), toupper);

	// Write the compilation directives, the class declaration, and the class constructor and destructor.
	header_file << "#ifndef " << define_var_name << "\n"
				<< "#define " << define_var_name << "\n"
				<< "\n"
				////////////////////////////////////////////////////////////
				//
				// pre-compilation 
				//
				////////////////////////////////////////////////////////////
				//<< "#include \"adevs.h\"\n"
				<< "\n"
				<< "class " << this->name_ << ": public staticDigraph\n"
				<< "{\n"
				<< "  public:\n"
				<< "    " << this->name_ << " (std::string name, std::ofstream& event_log);\n"
				<< "    ~" << this->name_ << " ();\n";

	// vector that contains all Ports for duplicate name checking
	std::vector<BON::Object> ports;

	// Create port_t static data members corresponding to each port (input or output) of the architecture.
	for (std::set<Interface>::iterator interface_iter = this->interfaces_.begin (); interface_iter != this->interfaces_.end (); interface_iter++)
	{
		Interface next_interface (*interface_iter);
		ASSERT (next_interface);

		// gets the Input ports
		std::set<Input> inputs = next_interface->getInput ();

		// gets the Output ports
		std::set<Output> outputs = next_interface->getOutput ();

		// Create ports for the inputs.
		for (std::set<Input>::iterator input_iter = inputs.begin (); input_iter != inputs.end (); input_iter++)
		{
			// validation
			Input next_input (*input_iter);
			ASSERT (next_input);

			// adding the port to the vector
			ports.push_back(*input_iter);

			std::string input_name = next_input->getName ();
			header_file << "    static const port_t " << input_name << ";\n";
		}

		// Create ports for the outputs.
		for (std::set<Output>::iterator output_iter = outputs.begin (); output_iter != outputs.end (); output_iter++)
		{
			// validation
			Output next_output (*output_iter);
			ASSERT (next_output);

			// adding the port to the vector
			ports.push_back(*output_iter);

			std::string output_name = next_output->getName ();
			header_file << "    static const port_t " << output_name << ";\n";
		}
	}

	detect_duplicates(ports);

	// Create a private data member to reference the output file stream for the event log. This
	// will need to be passed to all elements (atomics) contained in the architecture (static digraph).
	header_file << "  private:\n"
				<< "    std::ofstream * event_log_;\n"
				<< "};\n"
				<< "\n"
				<< "#endif\n";

	// Close the header file stream.
	header_file.close ();
}

/*
Generates the implementation file for the architecture (static digraph) class.
*/
void ArchitectureImpl::generate_implementation ()
{
	// Open the implentation file stream.
	std::string impl_file_name = version_path + this->name_ + ".cpp";
	std::ofstream impl_file (impl_file_name.c_str ());

	////////////////////////////////////////////////////////////
	//
	// pre-compilation 
	//
	////////////////////////////////////////////////////////////

	// Include the stdafx.h file.
	impl_file << "#include \"stdafx.h\"\n";

	// Include the class header file.
	impl_file << "#include \"" << this->name_ << ".h\"\n";

	// Include the header files of all the sub-elements (atomics).
	for (std::set<Element>::iterator element_iter = this->elements_.begin (); element_iter != this->elements_.end (); element_iter++)
		{
			Element next_element (*element_iter);
			ASSERT (next_element);
			impl_file << "#include \"" << next_element->get_element_name () << ".h\"\n";
		}

	// Include the header files of all the groups (atomics).
	for (std::set<Group>::iterator group_iter = this->groups_.begin (); group_iter != this->groups_.end (); group_iter++)
		{
			Group next_group (*group_iter);
			ASSERT (next_group);
			next_group->initialize_group();
			impl_file << "#include \"" << next_group->get_group_name () << ".h\"\n";
		}

		impl_file << "#include \"Thread.h\"\n"
				  << "\n";

	// Insert the real port number for each input into the event_inputs_ map, and write the code to
	// initialize the port static data members.
	int port_index = 0;
	for (std::map<Input, int>::iterator event_input_iter = this->event_inputs_.begin (); event_input_iter != this->event_inputs_.end (); event_input_iter++)
		{
			std::pair<Input, int> next_event_input (*event_input_iter);
			Input port (next_event_input.first);
			ASSERT (port);
			
			// Assign the port number to the input.
			this->event_inputs_[port] = port_index;

            // Write the code to initialize the port.
			impl_file << "const port_t " << this->name_ << "::" << port->getName () << " = " << port_index << ";\n";
			port_index++;
		}

	// Insert the real port number for each output into the event_outputs_ map, and write the code to
	// initialize the port static data members.
	for (std::map<Output, int>::iterator event_output_iter = this->event_outputs_.begin (); event_output_iter != this->event_outputs_.end (); event_output_iter++)
		{
			std::pair<Output, int> next_event_output (*event_output_iter);
			Output port (next_event_output.first);
			ASSERT (port);

			// Assign the port number to the output.
			this->event_outputs_[port] = port_index;

			// Write the code to initialize the port.
			impl_file << "const port_t " << this->name_ << "::" << port->getName () << " = " << port_index << ";\n";
			port_index++;
		}

	// Write the constructor.
	impl_file << "\n"
			  << this->name_ << "::" << this->name_ << " (std::string name, std::ofstream& event_log):\n"
			  << "staticDigraph()\n"
			  << "{\n"
			  << "  this->event_log_ = &event_log;\n";

	// Write the code to create an atomic for each element and add it to the static digraph.
	for (std::set<Element>::iterator element_iter = this->elements_.begin (); element_iter != this->elements_.end (); element_iter++)
		{
			Element next_element (*element_iter);
			ASSERT (next_element);
			std::string element_instance_name = next_element->get_element_name () + "_instance";
			impl_file << "  " << next_element->get_element_name () << " * " << element_instance_name << " = new " << next_element->get_element_name () << " (\"" << next_element->get_element_name () << "\", *this->event_log_);\n"
					  << "  this->add (" << element_instance_name << ");\n";
		}

	// Write the code to create an atomic(s) for each group and add them to the static digraph.
	for (std::set<Group>::iterator group_iter = this->groups_.begin (); group_iter != this->groups_.end (); group_iter++)
		{
			Group next_group (*group_iter);
			ASSERT (next_group);
			std::string group_instance_name = next_group->get_group_name () + "_instance";

			impl_file << "  " << next_group->get_group_name () << " * " << group_instance_name << " = new " << next_group->get_group_name () << " (\"" << next_group->get_group_name () << "\", *this->event_log_);\n"
					  << "  this->add (" << group_instance_name << ");\n";

			int thread_pool_size = next_group->getthreadPoolSize ();
			for (int i = 0; i < thread_pool_size; i++)
				{
					impl_file << "  Thread * " << group_instance_name << "_thread" << i << " = new Thread (\"" << group_instance_name << "_thread" << i << "\", " << i << ", *this->event_log_);\n"
							  << "  this->add (" << group_instance_name << "_thread" << i << ");\n";
				}
		}

	// Write the code to couple the appropriate ports of the element atomics.
	std::set<Link> links = this->getLink ();
	for (std::set<Link>::iterator link_iter = links.begin (); link_iter != links.end (); link_iter++)
	{
		Link next_link (*link_iter);
		ASSERT (next_link);

		// Determine if the source of the link is mapped to a sub-interface.
		Interface source = next_link->getSrc ();
		ASSERT (source);
		Interface final_source = source->get_mapped_interface ();
		ASSERT (final_source);

		// Determine if the destination of the link is mapped to a sub-interface.
		Interface destination = next_link->getDst ();
		ASSERT (destination);
		Interface final_destination = destination->get_mapped_interface ();
		ASSERT (final_destination);

		// Get the element (atomic) or architecture (static digraph) that contains the source
		// of the link.
		BON::Model source_parent = final_source->getParent ();
		Element source_element (source_parent);
		if (!source_element)
		{
			Architecture source_architecture (source_parent);
			ASSERT (source_architecture);
			source_element = source_architecture->getParent ();
		}
		ASSERT (source_element);

		// Get the element (atomic) or architecture (static digraph) that contains the destination
		// of the link.
		BON::Model destination_parent = final_destination->getParent ();
		Element destination_element (destination_parent);
		if (!destination_element)
		{
			Architecture destination_architecture (destination_parent);
			ASSERT (destination_architecture);
			destination_element = destination_architecture->getParent ();
		}
		ASSERT (destination_element);

		// Write the invocation of the couple method and the source parameters.
		impl_file << "  this->couple (" << source_element->get_element_name () << "_instance, " << source_element->get_element_name () << "_instance->";

		// Find the output port name of the source atomic.
		std::set<Output> output_ports = final_source->getOutput ();
		for (std::set<Output>::iterator output_port_iter = output_ports.begin (); output_port_iter != output_ports.end (); output_port_iter++)
		{
			Output next_output_port (*output_port_iter);
			ASSERT (next_output_port);

			// Write the name of the port to the parameter list.
			impl_file << next_output_port->getName ();
		}

		// Write the destination parameters of the couple method.
		impl_file << ", " << destination_element->get_element_name () << "_instance, " << destination_element->get_element_name () << "_instance->";

		// Find the input port name of the destination atomic.
		std::set<Input> input_ports = final_destination->getInput ();
		for (std::set<Input>::iterator input_port_iter = input_ports.begin (); input_port_iter != input_ports.end (); input_port_iter++)
		{
			Input next_input_port (*input_port_iter);
			ASSERT (next_input_port);

			// Write the name of the port to the parameter list.
			impl_file << next_input_port->getName ();
		}

		// End the parameter list for the couple method.
		impl_file << ");\n";
	}


	// Write the code to couple the ports of the group atomics.
	for (std::set<Group>::iterator group_iter = this->groups_.begin (); group_iter != this->groups_.end (); group_iter++)
		{
			Group next_group (*group_iter);
			ASSERT (next_group);
			std::string group_instance_name = next_group->get_group_name () + "_instance";

			int thread_pool_size = next_group->getthreadPoolSize ();
			for (int i = 0; i < thread_pool_size; i++)
				{
					impl_file << "  this->couple (" << group_instance_name << ", " << group_instance_name << "->ServiceDemandOutput[" << i << "], " << group_instance_name << "_thread" << i << ", " << group_instance_name << "_thread" << i << "->ServiceDemandInput);\n";
					impl_file << "  this->couple (" << group_instance_name << "_thread" << i << ", " << group_instance_name << "_thread" << i << "->ServiceCompletionOutput, " << group_instance_name << ", " << group_instance_name << "->ServiceCompletionInput[" << i << "]);\n";
				}
		}

	// Write the code to couple the elements to the groups.
	for (std::set<Group>::iterator group_iter = this->groups_.begin (); group_iter != this->groups_.end (); group_iter++)
		{
			Group next_group (*group_iter);
			ASSERT (next_group);
			std::string group_instance_name = next_group->get_group_name () + "_instance";

			std::set<Element> group_elements = next_group->getElements (0);
			for (std::set<Element>::iterator element_iter = group_elements.begin (); element_iter != group_elements.end (); element_iter++)
				{
					Element next_element (*element_iter);
					ASSERT (next_element);

					std::string element_instance_name = next_element->get_element_name () + "_instance";
					
					impl_file << "  this->couple (" << group_instance_name << ", " << group_instance_name << "->ServiceCompletionOutput, " << element_instance_name << ", " << element_instance_name << "->ServiceCompletionInput);\n";
					impl_file << "  this->couple (" << element_instance_name << ", " << element_instance_name << "->ServiceDemandOutput, " << group_instance_name << ", " << group_instance_name << "->ServiceDemandInput);\n";
				}
		}

	// Write the code to couple the appropriate ports of the element atomics to the ports of the static digraph.
	std::set<InterfaceMapping> mappings = this->getInterfaceMapping ();
	for (std::set<InterfaceMapping>::iterator mapping_iter = mappings.begin (); mapping_iter != mappings.end (); mapping_iter++)
		{
			InterfaceMapping next_mapping (*mapping_iter);
			ASSERT (next_mapping);

			// Get the mapped interfaces.
			Interface source_intf = next_mapping->getSrc ();
			ASSERT (source_intf);

			Interface dest_intf = next_mapping->getDst ();
			ASSERT (dest_intf);

			Interface internal_intf;
			Interface external_intf;

			BON::Model source_parent = source_intf->getParent ();
			Element internal_element (source_parent);
			if (internal_element)
				{
					internal_intf = source_intf;
					external_intf = dest_intf;
				}
			else
				{
					internal_intf = dest_intf;
					external_intf = source_intf;
					internal_element = dest_intf->getParent ();
				}

			ASSERT (internal_element);
			ASSERT (internal_intf);
			ASSERT (external_intf);

			if ((internal_intf->getOutput ()).size () > 0)
				{
					// Write the invocation of the couple method and the source parameters.
					impl_file << "  this->couple (" << internal_element->get_element_name () << "_instance, " << internal_element->get_element_name () << "_instance->";

					// Find the output port name of the source atomic.
					std::set<Output> internal_output_ports = internal_intf->getOutput ();
					for (std::set<Output>::iterator internal_output_port_iter = internal_output_ports.begin (); internal_output_port_iter != internal_output_ports.end (); internal_output_port_iter++)
						{
							Output next_internal_output_port (*internal_output_port_iter);
							ASSERT (next_internal_output_port);
							impl_file << next_internal_output_port->getName ();
						}

					// Write the destination parameters of the couple method.
					impl_file << ", this, this->";

					// Find the input port name of the static digraph.
					std::set<Output> external_output_ports = external_intf->getOutput ();
					for (std::set<Output>::iterator external_output_port_iter = external_output_ports.begin (); external_output_port_iter != external_output_ports.end (); external_output_port_iter++)
						{
							Output next_external_output_port (*external_output_port_iter);
							ASSERT (next_external_output_port);
							impl_file << next_external_output_port->getName ();
						}

					// End the parameter list for the couple method.
					impl_file << ");\n";
				}

			if ((internal_intf->getInput ()).size () > 0)
				{
					// Begin a new invocation to couple in the opposite direction.
					impl_file << "  this->couple (this, this->";

					std::set<Input> external_input_ports = external_intf->getInput ();
					for (std::set<Input>::iterator external_input_port_iter = external_input_ports.begin (); external_input_port_iter != external_input_ports.end (); external_input_port_iter++)
						{
							Input next_external_input_port (*external_input_port_iter);
							ASSERT (next_external_input_port);
							impl_file << next_external_input_port->getName ();
						}

					impl_file << ", " << internal_element->get_element_name () << "_instance, " << internal_element->get_element_name () << "_instance->";

					std::set<Input> internal_input_ports = internal_intf->getInput ();
					for (std::set<Input>::iterator internal_input_port_iter = internal_input_ports.begin (); internal_input_port_iter != internal_input_ports.end (); internal_input_port_iter++)
						{
							Input next_internal_input_port (*internal_input_port_iter);
							ASSERT (next_internal_input_port);
							impl_file << next_internal_input_port->getName ();
						}

					// End the parameter list for the couple method.
					impl_file << ");\n";
				}
		}

	// End the constructor and write the destructor.
	impl_file << "}\n"
			  << "\n"
			  << this->name_ << "::~" << this->name_ << " ()\n"
			  << "{\n"
			  << "}\n";

}

/*
Generates the main application entry point file.
*/
void ArchitectureImpl::generate_main_file ()
{
	// Open the main file stream.
	std::string main_file_name = version_path + this->name_ + "_main.cpp";
	std::ofstream main_file (main_file_name.c_str ());

	////////////////////////////////////////////////////////////
	//
	//
	//	pre-compilation work
	//
	//
	////////////////////////////////////////////////////////////

	// Write the main file.
	main_file << "#include \"stdafx.h\"\n"
			  << "#include \"NewRandom.h\"\n"
			  << "#include \"" << this->name_ << ".h\"\n";

	// Include any custom analysis headers.
	XTEAM_IFrame::sim_gen ()->architecture_include (main_file);

	// Initialize the random number generator, open the event log, and instantiate the architecture.
	main_file << "\n"
			  << "int main(int argc, char** argv)\n"
			  << "{\n"
			  << "  Random::Set ((double) time (NULL) / INT_MAX);\n"
			  //<< "	// if the target output directory is given, set the current working dir\n"
			  //<< "	if(argc > 1) {\n"
			  //<< "		SetCurrentDirectory(argv[1]);\n"
			  //<< "	}\n"
			  << "  std::ofstream event_log (\"" << this->name_ << "_Event_Log.txt\");\n"
			  << "  " << this->name_ << " " << this->instance_name_  << " (\"" << this->instance_name_ << "\", event_log);\n"
			  << "  devssim sim (&" << this->instance_name_ << ");\n";

	// Perform any pre-simulation analysis functions.
	XTEAM_IFrame::sim_gen ()->on_sim_start (this, main_file);

	// Run and time the simulation.
	main_file << "  clock_t start = clock ();\n"
			  << "  sim.run (" << target_time << ");\n"
			  << "  clock_t end = clock ();\n"
			  << "  //std::cout << \"Simulation completed in \" << (double)(end - start) / CLOCKS_PER_SEC << \" seconds.\" << endl;\n";

	// Perform any post-simulation analysis functions.
	XTEAM_IFrame::sim_gen ()->on_sim_end (this, main_file);

	// Close the event log and return.
	main_file << "  event_log.close ();\n"
		<< "	std::ofstream completion_tag (\"Simulation_Completion.txt\");\n"
		<< "	completion_tag << \"Simulation completed in \" << (double)(end - start) / CLOCKS_PER_SEC << \" seconds.\" << endl;\n"
		<< "	completion_tag.close ();\n"
			  << "  return 0;\n"
			  << "}\n";

	// Close the main file stream.
	main_file.close ();
}

/*
Recursively provides each primitive element (atomic) with a reference to its element group (thread pool).
*/
void ArchitectureImpl::set_element_groups ()
{
	// For each group in the architecture, provide a reference to all elements in the group.
	std::set<Group> groups = this->getGroup ();
	for (std::set<Group>::iterator group_iter = groups.begin (); group_iter != groups.end (); group_iter++)
	{
		Group next_group (*group_iter);
		ASSERT (next_group);
		std::set<Element> group_elements = next_group->getElements (0);
		for (std::set<Element>::iterator element_iter = group_elements.begin (); element_iter != group_elements.end (); element_iter++)
		{
			Element next_element (*element_iter);
			ASSERT (next_element);
			next_element->set_element_group (next_group);
		}
		next_group->generate ();
	}

	// For each element in the architecture, recursively call this function on any sub-architecture.
	std::set<Element> elements = this->getElement ();
	for (std::set<Element>::iterator element_iter = elements.begin (); element_iter != elements.end (); element_iter++)
	{
		Element next_element (*element_iter);
		ASSERT (next_element);

		std::set<Architecture> sub_architectures = next_element->getArchitecture ();
		for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++)
		{
			Architecture next_architecture (*architecture_iter);
			ASSERT (next_architecture);
			next_architecture->set_element_groups ();
		}
	}
}

/*
Returns the interface of a sub-architecture to which this interface is mapped. Only navigates through one level of the hierarchy.
*/
Interface InterfaceImpl::get_mapped_interface ()
{
	std::multiset<Interface> mapped_interfaces = this->getInterfaceMappingEnds ();
	for (std::multiset<Interface>::iterator mapped_interface_iter = mapped_interfaces.begin (); mapped_interface_iter != mapped_interfaces.end (); mapped_interface_iter++)
		{
			Interface mapped_interface = (*mapped_interface_iter);
			ASSERT (mapped_interface);

			if (mapped_interface->getParent () == this->getParent ()->getParent ())
				{
					return mapped_interface;
				}
		}
	return this;
}

/*
Returns the interface of a higher level element to which this interface is mapped. Navigates through all levels of the hierarchy.
*/
Interface InterfaceImpl::get_exposed_interface ()
{
	Interface mapped_interface = this->get_mapped_interface ();
	std::multiset<Interface> exposed_interfaces = mapped_interface->getInterfaceMappingEnds ();
	for (std::multiset<Interface>::iterator exposed_interface_iter = exposed_interfaces.begin (); exposed_interface_iter != exposed_interfaces.end (); exposed_interface_iter++)
		{
			Interface exposed_interface = (*exposed_interface_iter);
			ASSERT (exposed_interface);

			if (exposed_interface->getParent () == this->getParent ()->getParent ()->getParent ())
				{
					return exposed_interface->get_exposed_interface ();
				}
		}
	return this;
}

/*
Returns the group name.
*/
std::string GroupImpl::get_group_name ()
{
	return this->name_;
}

/*
Initializes the member variables for the group, then generates the required files.
*/
void GroupImpl::generate ()
{
	// Initialize the member variables for the group.
	this->initialize_group ();

	// Generate the header file for the group class.
	this->generate_header ();

	// Generate the implementation file for the group class.
	this->generate_implementation ();
}

/*
Initializes all the private data members of the group class.
*/
void GroupImpl::initialize_group ()
{
	std::string group_name = this->getName ();
	std::string group_id = (this->getID ()).substr (8, 15);
	this->name_ = group_name + "_" + group_id;
}

/*
Generates the header file that declares the group (thread pool) class.
*/
void GroupImpl::generate_header ()
{
	std::string thread_pool_file_name = version_path + this->name_ + ".h";
	std::ofstream thread_pool_file (thread_pool_file_name.c_str ());

	// Define a compilation flag to prevent multiple includes from causing compilation errors.
	std::string define_var_name = this->name_ + "_H";
	std::transform (define_var_name.begin (), define_var_name.end (), define_var_name.begin (), toupper);

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Number of thread is 0
	///////////////////////////////////////////////////////////////
	if(this->getthreadPoolSize() == 0)
	{
		std::stringstream ss;
		BON::Object obj (this);
		ss	<< this->getName() 
			<< " [" << serialize_GUID(obj)
			<< "/" << this->getObjectMeta().name() << "]"
			<< " has 0 number of thread, which must be 1 or more" << std::endl;
		err_msg += ss.str();
		return;
	}

	// Write the compilation directives and the group class declaration.
	thread_pool_file << "#ifndef " << define_var_name << "\n"
					 << "#define " << define_var_name << "\n"
					 << "\n"

					////////////////////////////////////////////////////////////
					//
					// pre-compilation 
					//
					////////////////////////////////////////////////////////////

					 //<< "#include <string>\n"
					 //<< "#include <fstream>\n"
					 //<< "#include <iostream>\n"
					 //<< "#include <list>\n"
					 //<< "#include \"adevs.h\"\n"
 					 << "#include \"ServiceDemand.h\"\n"
					 << "#include \"ServiceCompletion.h\"\n"
					 << "\n"
					 << "class " << this->name_ << ": public atomic\n"
					 << "{\n"
					 << "  public:\n"
					 << "\n"
					 << "    " << this->name_ << " (std::string element_id, std::ofstream& event_log);\n"
					 << "    void init ();\n"
					 << "    void delta_int ();\n"
					 << "    void delta_ext (stime_t e, const adevs_bag<PortValue>& x);\n"
					 << "    void delta_conf (const adevs_bag<PortValue>& x);\n"
					 << "    void output_func (adevs_bag<PortValue>& y);\n"
					 << "    void gc_output (adevs_bag<PortValue>& g);\n"
					 << "    ~" << this->name_ << " ();\n"
					 << "    static const int NUM_THREADS = " << this->getthreadPoolSize () << ";\n"
					 << "    static const port_t ServiceDemandInput;\n"
					 << "    static const port_t ServiceCompletionOutput;\n"
					 << "    static const port_t ServiceCompletionInput[NUM_THREADS];\n"
					 << "    static const port_t ServiceDemandOutput[NUM_THREADS];\n"
					 << "\n"
					 << "  private:\n"
					 << "\n"
					 << "    std::string element_id_;\n"
					 << "    std::ofstream * event_log_;\n"
					 << "    std::list<int> available_threads_;\n"
					 << "    std::list<ServiceDemand *> demand_queue_;\n"
					 << "    std::list<ServiceCompletion *> completion_queue_;\n"
					 << "    std::list<PortValue> output_queue_;\n"
					 << "    };\n"
					 << "\n"
					 << "#endif\n";

	thread_pool_file.close ();

}

/*
Generates the implementation file for the group (thread pool) class.
*/
void GroupImpl::generate_implementation ()
{
	// Open the thread pool file.
	std::string thread_pool_file_name = version_path + this->name_ + ".cpp";
	std::ofstream thread_pool_file (thread_pool_file_name.c_str ());

	// Get the number of threads in the thread pool.
	int thread_pool_size = this->getthreadPoolSize ();

	////////////////////////////////////////////////////////////
	//
	// pre-compilation 
	//
	////////////////////////////////////////////////////////////
	thread_pool_file << "#include \"stdafx.h\"\n";

	// Declare and initialize the ports of the thread pool atomic.
	thread_pool_file << "#include \"" << this->name_ << ".h\"\n"
					 << "\n"
					 << "const port_t " << this->name_ << "::ServiceDemandInput = " << this->name_ << "::NUM_THREADS * 2;\n"
					 << "const port_t " << this->name_ << "::ServiceCompletionOutput = " << this->name_ << "::NUM_THREADS * 2 + 1;\n"
					 << "const port_t " << this->name_ << "::ServiceDemandOutput[" << this->name_ << "::NUM_THREADS] = {0";
	
	// Create an output port to send service demands to each thread atomic.
	for (int i = 1; i < thread_pool_size; i++)
		{
			thread_pool_file << ", " << i;
		}

	// Create an input port to receive service completions from each thread atomic.
	thread_pool_file << "};\n"
					 << "const port_t " << this->name_ << "::ServiceCompletionInput[" << this->name_ << "::NUM_THREADS] = {" << thread_pool_size;

	for (int i = 1; i < thread_pool_size; i++)
		{
			thread_pool_file << ", " << i + thread_pool_size;
		}

	// Finish the thread pool implementation file.
	thread_pool_file << "};\n"
					 << "\n"
					 << this->name_ << "::" << this->name_ << " (std::string element_id, std::ofstream& event_log):\n"
                     << "atomic ()\n"
					 << "{\n"
					 << "  this->element_id_ = element_id;\n"
					 << "  this->event_log_ = &event_log;\n"
					 << "  for (int i = 0; i < NUM_THREADS; i++)\n"
					 << "    {\n"
					 << "      available_threads_.push_back (i);\n"
					 << "    }\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::init ()\n"
					 << "{\n"
					 << "#if VERBOSITY >= 3\n"
					 << "  *this->event_log_ << \"" << this->name_ << "::init ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  passivate ();\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::delta_ext (double e, const adevs_bag<PortValue>& x)\n"
					 << "{\n"
 					 << "#if VERBOSITY >= 3\n"
 					 << "  *this->event_log_ << \"" << this->name_ << "::delta_ext ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  for (int i = 0; i < x.getSize (); i++)\n"
					 << "    {\n"
					 << "      port_t arrival_port = x.get(i).port;\n"
					 << "      if (arrival_port == ServiceDemandInput)\n"
					 << "        {\n"
					 << "          ServiceDemand * new_service_demand = (ServiceDemand *)x.get(i).value;\n"
					 << "          if (new_service_demand->service_demand_ == 0)\n"
					 << "            {\n"
					 << "              ServiceCompletion * next_service_completion = new ServiceCompletion (new_service_demand->element_id_, new_service_demand->thread_id_, -1);\n"
					 << "              PortValue new_output;\n"
					 << "              new_output.port = ServiceCompletionOutput;\n"
					 << "              new_output.value = next_service_completion;\n"
					 << "              this->output_queue_.push_back (new_output);\n"
					 << "            }\n"
					 << "          else\n"
					 << "            {\n"
					 << "              ServiceDemand * next_service_demand = new ServiceDemand (*new_service_demand);\n"
					 << "              demand_queue_.push_back (next_service_demand);\n"
					 << "            }\n"
					 << "        }\n"
					 << "      else\n"
					 << "        {\n"
					 << "          ServiceCompletion * new_service_completion = new ServiceCompletion (*(ServiceCompletion *)x.get(i).value);\n"
					 << "          completion_queue_.push_back (new_service_completion);\n"
					 << "        }\n"
					 << "    }\n"
					 << "  while (!completion_queue_.empty ())\n"
					 << "    {\n"
					 << "      ServiceCompletion * next_service_completion = this->completion_queue_.front ();\n"
					 << "      this->completion_queue_.pop_front ();\n"
					 << "      this->available_threads_.push_back (next_service_completion->worker_number_);\n"
					 << "      PortValue new_output;\n"
					 << "      new_output.port = ServiceCompletionOutput;\n"
					 << "      new_output.value = next_service_completion;\n"
					 << "      this->output_queue_.push_back (new_output);\n"
					 << "    }\n"
					 << "  while (!available_threads_.empty () && !demand_queue_.empty ())\n"
					 << "    {\n"
					 << "      ServiceDemand * next_service_demand = this->demand_queue_.front ();\n"
					 << "      this->demand_queue_.pop_front ();\n"
					 << "      PortValue new_output;\n"
					 << "      new_output.port = this->available_threads_.front ();\n"
					 << "      this->available_threads_.pop_front ();\n"
					 << "      new_output.value = next_service_demand;\n"
					 << "      this->output_queue_.push_back (new_output);\n"
					 << "    }\n"
					 << "  hold (0);\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::delta_int ()\n"
					 << "{\n"
					 << "#if VERBOSITY >= 3\n"
 					 << "  *this->event_log_ << \"" << this->name_ << "::delta_int ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  passivate ();\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::delta_conf (const adevs_bag<PortValue>& x)\n"
					 << "{\n"
					 << "#if VERBOSITY >= 3\n"
					 << "  *this->event_log_ << \"" << this->name_ << "::delta_conf ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  delta_ext (0.0, x);\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::output_func (adevs_bag<PortValue>& y)\n"
					 << "{\n"
 					 << "#if VERBOSITY >= 3\n"
					 << "  *this->event_log_ << \"" << this->name_ << "::output_func ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  while (!this->output_queue_.empty ())\n"
					 << "    {\n"
					 << "      output (this->output_queue_.front ().port, this->output_queue_.front ().value, y);\n"
					 << "      this->output_queue_.pop_front ();\n"
					 << "    }\n"
					 << "}\n"
					 << "\n"
					 << "void " << this->name_ << "::gc_output (adevs_bag<PortValue>& g)\n"
					 << "{\n"
  					 << "#if VERBOSITY >= 3\n"
					 << "  *this->event_log_ << \"" << this->name_ << "::gc_output ()\\n\";\n"
					 << "  this->event_log_->flush ();\n"
					 << "#endif\n"
					 << "  for (int i = 0; i < g.getSize (); i++)\n"
					 << "    {\n"
					 << "      delete g[i].value;\n"
					 << "    }\n"
					 << "}\n"
					 << "\n"
					 << this->name_ << "::~" << this->name_ << " ()\n"
					 << "{\n"
					 << "}\n"
					 << "\n";
}

/*
Data member accessor functions.
*/
std::string ElementImpl::get_element_name ()
{
	return this->name_;
}

std::map<Input, int>& ElementImpl::get_event_inputs ()
{
	return this->event_inputs_;
}

std::map<Output, int>& ElementImpl::get_event_outputs ()
{
	return this->event_outputs_;
}

std::set<Datum>& ElementImpl::get_data_types ()
{
	return this->data_types_;
}

Group ElementImpl::get_element_group ()
{
	return this->element_group_;
}

std::map<Port, Element>& ElementImpl::get_linked_elements ()
{
	return this->linked_elements_;
}

void ElementImpl::set_element_group (Group& element_group)
{
	this->element_group_ = element_group;
}

/*
Generate the files for an element. If the element contains an architecture, generate a static digraph for it.
Otherwise, generate an atomic for this element.
*/
void ElementImpl::generate ()
{
	// Determine if the element contains a sub-architecture.
	std::set<Architecture> sub_architectures = this->getArchitecture ();
	if (sub_architectures.size () > 0)
		{
			// Generate a static digraph.
			for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++)
				{
					Architecture next_architecture (*architecture_iter);
					ASSERT (next_architecture);
					std::string element_name = this->getName ();
					std::string element_id = (this->getID ()).substr (8, 15);
					this->name_ = element_name + "_" + element_id;
					next_architecture->generate ();
				}
		}
	else
		{	
			// Generate an atomic.
			this->initialize_element ();
			this->generate_header ();
			this->generate_implementation ();
		}
}

/*
Initialize all the private data members for the element.
*/
void ElementImpl::initialize_element ()
{
	std::string element_name = this->getName ();
	std::string element_id = (this->getID ()).substr (8, 15);
	this->name_ = element_name + "_" + element_id;
	std::string impl_file_name = version_path + this->name_ + ".cpp";
	this->impl_file_ = new std::ofstream (impl_file_name.c_str ());

	this->processes_ = this->getProcess();

	// copies the processes to vector and sorts them
	for (std::set<Process>::iterator proc_iter = this->processes_.begin(); proc_iter != this->processes_.end(); proc_iter ++) 
	{
		this->ordered_processes_.push_back(*proc_iter);
	}
	std::sort(this->ordered_processes_.begin(), this->ordered_processes_.end(), guid_comp);

	// Populate the event_inputs_, event_outputs_, data_types_, and linked_elements_ members.
	std::set<Interface> interfaces = this->getInterface ();
	for (std::set<Interface>::iterator interface_iter = interfaces.begin (); interface_iter != interfaces.end (); interface_iter++)
		{
			Interface next_interface (*interface_iter);
			ASSERT (next_interface);

			// Get all inputs in the interface.
			std::set<Input> inputs = next_interface->getInput ();
			for (std::set<Input>::iterator input_iter = inputs.begin (); input_iter != inputs.end (); input_iter++)
				{
					Input next_input (*input_iter);
					ASSERT (next_input);

					std::set<PortType> port_types = next_input->getPortType ();

					///////////////////////////////////////////////////////////////
					// CoDesign Modification
					// Input has no port type
					///////////////////////////////////////////////////////////////
					if(port_types.empty())
					{
						std::stringstream ss;
						ss	<< next_input->getName() 
							<< " [" << serialize_GUID(next_input)
							<< "/" << next_input->getObjectMeta().name() << "]"
							<< " in " << next_interface->getName() 
							<< " in " << this->getName()
							<< " has no Port Type" << std::endl;
						err_msg += ss.str();
						continue;
					}

					ASSERT (!port_types.empty ());

					PortType port_type (*port_types.begin ());
					ASSERT (port_type);

					///////////////////////////////////////////////////////////////
					// CoDesign Modification
					// Input port has no Datum (or broken reference)
					///////////////////////////////////////////////////////////////
					Datum input_datum = port_type->getDatum ();
					if(!input_datum)
					{
						std::stringstream ss;
						ss	<< port_type->getName() 
							<< " [" << serialize_GUID(port_type)
							<< "/" << port_type->getObjectMeta().name() << "]"
							<< " in " << next_input->getName()
							<< " in " << this->getName()
							<< " has no Datum (or broken reference)" << std::endl;
						err_msg += ss.str();
						continue;
					}

					ASSERT (input_datum);

					this->event_inputs_[next_input] = 0;
					this->data_types_.insert (input_datum);

					Interface exposed_interface = next_interface->get_exposed_interface ();
					ASSERT (exposed_interface);

					std::multiset<Interface> link_sources = exposed_interface->getLinkSrcs();
					for (std::multiset<Interface>::iterator link_source_iter = link_sources.begin (); link_source_iter != link_sources.end (); link_source_iter++)
						{
							Interface next_link_source (*link_source_iter);
							ASSERT (next_link_source);
							BON::Model source_parent = next_link_source->getParent ();
							Element source_element (source_parent);
							ASSERT (source_element);
							this->linked_elements_[next_input] = source_element;
						}
				}

			// Get all outputs in the interface.
			std::set<Output> outputs = next_interface->getOutput ();
			for (std::set<Output>::iterator output_iter = outputs.begin (); output_iter != outputs.end (); output_iter++)
				{
					Output next_output (*output_iter);
					ASSERT (next_output);

					std::set<PortType> port_types = next_output->getPortType ();

					///////////////////////////////////////////////////////////////
					// CoDesign Modification
					// Output port has no Port Type
					///////////////////////////////////////////////////////////////
					if(port_types.empty())
					{
						std::stringstream ss;
						ss	<< next_output->getName() 
							<< " [" << serialize_GUID(next_output)
							<< "/" << next_output->getObjectMeta().name() << "]"
							<< " in " << next_interface->getName() 
							<< " in " << this->getName()
							<< " has no Port Type" << std::endl;
						err_msg += ss.str();
						continue;
					}

					ASSERT (!port_types.empty ());
					PortType port_type (*port_types.begin ());
					ASSERT (port_type);

					///////////////////////////////////////////////////////////////
					// CoDesign Modification
					// Output port has no Datum
					///////////////////////////////////////////////////////////////
					Datum output_datum = port_type->getDatum ();
					if(!output_datum)
					{
						std::stringstream ss;
						ss	<< port_type->getName() 
							<< " [" << serialize_GUID(port_type)
							<< "/" << port_type->getObjectMeta().name() << "]"
							<< " in " << next_output->getName() 
							<< " in " << this->getName()
							<< " has no Datum (or broken reference)" << std::endl;
						err_msg += ss.str();
						continue;
					}

					ASSERT (output_datum);

					this->event_outputs_[next_output] = 0;
					this->data_types_.insert (output_datum);

					Interface exposed_interface = next_interface->get_exposed_interface ();
					ASSERT (exposed_interface);

					std::multiset<Interface> link_dests = exposed_interface->getLinkDsts();
					for (std::multiset<Interface>::iterator link_dest_iter = link_dests.begin (); link_dest_iter != link_dests.end (); link_dest_iter++)
						{
							Interface next_link_dest (*link_dest_iter);
							ASSERT (next_link_dest);
							BON::Model dest_parent = next_link_dest->getParent ();
							Element dest_element (dest_parent);
							ASSERT (dest_element);
							this->linked_elements_[next_output] = dest_element;
						}
				}
		}

	// Insert all resource data types into the data_types_ member.
	std::set<Resource> resources = this->getResource ();
	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++)
		{
			Resource next_resource (*resource_iter);
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Resource is broken
			///////////////////////////////////////////////////////////////
			if(!next_resource)
			{
				// gets the GUID
				long				guid[4];
				this->getObjectI()->GetGuid(&guid[0], &guid[1], &guid[2], &guid[3]);
				
				// serialize the GUID
				std::stringstream ss_guid;
				ss_guid << std::uppercase << std::hex << guid[0] << "-" << guid[1] << "-" << guid[2] << "-" << guid[3];

				std::stringstream ss;
				ss	<< this->getName() 
					<< " [" << ss_guid
					<< "/" << this->getObjectMeta().name() << "]"
					<< " has a broken Resource that might no longer exist" << std::endl;
				err_msg += ss.str();
				continue;
			}
			ASSERT (next_resource);

			std::set<ResourceType> resource_types = next_resource->getResourceType ();
			
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Resource has no Resource Type
			///////////////////////////////////////////////////////////////
			if (resource_types.empty())
			{
				std::stringstream ss;
				ss	<< next_resource->getName() 
					<< " [" << serialize_GUID(next_resource)
					<< "/" << next_resource->getObjectMeta().name() << "]"
					<< " in " << this->getName()
					<< " has no Resource Type" << std::endl;
				err_msg += ss.str();
				continue;
			}
			ASSERT (!resource_types.empty ());

			ResourceType resource_type (*resource_types.begin ());
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Resource has broken resource type
			///////////////////////////////////////////////////////////////
			if(!resource_type)
			{
				std::stringstream ss;
				ss	<< next_resource->getName() 
					<< " [" << serialize_GUID(next_resource)
					<< "/" << next_resource->getObjectMeta().name() << "]"
					<< " in " << this->getName()
					<< " has a broken Resource Type that might no longer exist" << std::endl;
				err_msg += ss.str();
				continue;
			}
			ASSERT (resource_type);


			Datum resource_datum = resource_type->getDatum ();
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Resource has no Resource Type
			///////////////////////////////////////////////////////////////
			if(!resource_datum)
			{
				std::stringstream ss;
				ss	<< resource_type->getName() 
					<< " [" << serialize_GUID(resource_type)
					<< "/" << resource_type->getObjectMeta().name() << "]"
					<< " in " << next_resource->getName()
					<< " in " << this->getName()
					<< " has a broken Datum that might no longer exist" << std::endl;
				err_msg += ss.str();
				continue;
			}
			ASSERT (resource_datum);

			this->data_types_.insert (resource_datum);
		}

}

/*
Generate the header file for the element (atomic).
*/
void ElementImpl::generate_header ()
{
	// Open the header file stream.
	std::string header_file_name = version_path + this->name_ + ".h";
	std::ofstream header_file (header_file_name.c_str ());

	// Create a compilation directive to prevent multiple includes from causing errors.
	std::string define_var_name = this->name_ + "_H";
	std::transform (define_var_name.begin (), define_var_name.end (), define_var_name.begin (), toupper);

	// Include the needed libraries.
	header_file << "#ifndef " << define_var_name << "\n"
				<< "#define " << define_var_name << "\n"
				<< "\n"
				////////////////////////////////////////////////////////////
				//
				// pre-compilation 
				//
				////////////////////////////////////////////////////////////
				//<< "#include <string>\n"
				//<< "#include <fstream>\n"
				//<< "#include <iostream>\n"
				//<< "#include <list>\n"
				//<< "#include \"adevs.h\"\n"
				;

	// Include the header file for any data types (objects) used.
	for (std::set<Datum>::iterator datum_iter = this->data_types_.begin (); datum_iter != this->data_types_.end (); datum_iter++)
		{
			Datum next_datum (*datum_iter);
			ASSERT (next_datum);

			header_file << "#include \"" << next_datum->getName () << ".h\"\n";
		}

	// Include any additional header files needed for analysis.
	XTEAM_IFrame::sim_gen ()->element_include (header_file);

	// Write the class declaration.
	header_file << "\n"
				<< "class " << this->name_ << ": public atomic\n"
				<< "{\n"
				<< "  public:\n"
				<< "\n"
				<< "    " << this->name_ << " (std::string element_id, std::ofstream& event_log);\n"
				<< "    void init ();\n"
				<< "    void delta_int ();\n"
				<< "    void delta_ext (stime_t e, const adevs_bag<PortValue>& x);\n"
				<< "    void delta_conf (const adevs_bag<PortValue>& x);\n"
				<< "    void output_func (adevs_bag<PortValue>& y);\n"
				<< "    void gc_output (adevs_bag<PortValue>& g);\n"
				<< "    ~" << this->name_ << " ();\n";

	// Create a port corresponding to each input and output.
	std::set<Interface> interfaces = this->getInterface ();
	std::vector<BON::Object> ports;

	for (std::set<Interface>::iterator interface_iter = interfaces.begin (); interface_iter != interfaces.end (); interface_iter++)
	{
		Interface next_interface (*interface_iter);
		ASSERT (next_interface);

		std::set<Input> inputs = next_interface->getInput ();
		std::set<Output> outputs = next_interface->getOutput ();

		for (std::set<Input>::iterator input_iter = inputs.begin (); input_iter != inputs.end (); input_iter++)
		{
			Input next_input (*input_iter);
			ASSERT (next_input);

			ports.push_back(*input_iter);

			std::string input_name = next_input->getName ();
			header_file << "    static const port_t " << input_name << ";\n";
		}
		
		for (std::set<Output>::iterator output_iter = outputs.begin (); output_iter != outputs.end (); output_iter++)
		{
			Output next_output (*output_iter);
			ASSERT (next_output);

			ports.push_back(*output_iter);

			std::string output_name = next_output->getName ();
			header_file << "    static const port_t " << output_name << ";\n";
		}
	}

	detect_duplicates(ports);

	// Create ports to send service demands and receive service completions from the thread pool.
	header_file << "    static const port_t ServiceDemandOutput;\n"
				<< "    static const port_t ServiceCompletionInput;\n";

	// Genearte the private member functions and variables.
	header_file << "\n"
				<< "  private:\n"
				<< "\n"
				<< "    void handle_next_event (int thread_id);\n"
				<< "    void generate_output (int thread_id);\n"
				<< "    void transition_process (int thread_id);\n"
				<< "\n"
				<< "    std::string element_id_;\n"
				<< "    std::ofstream * event_log_;\n"
				<< "    std::list<PortValue> input_queue_;\n"
				<< "    std::list<PortValue> output_queue_;\n"
				<< "    std::list<PortValue> service_queue_;\n"
				<< "    std::string initial_process_;\n"
				<< "    std::vector<std::string> thread_processes_;\n"
				<< "    std::list<int> idle_threads_;\n";

	// Generate a data member corresponding to each resource.
	std::set<Resource> resources = this->getResource ();

	// for duplicate detection
	std::vector<BON::Object> ordered_resources;

	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++) {
		Resource next_resource (*resource_iter);
		ASSERT (next_resource);

		ordered_resources.push_back(next_resource);

		std::set<ResourceType> resource_types = next_resource->getResourceType ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// Resource does not have Resource Type
		///////////////////////////////////////////////////////////////
		if (resource_types.size() == 0) {
			std::stringstream ss;
			ss	<< next_resource->getName() 
				<< " [" << serialize_GUID(next_resource)
				<< "/" << next_resource->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has no Resource Type" << std::endl;
			err_msg += ss.str();
			continue;
		}

		ResourceType next_resource_type = *(resource_types.begin ());
		ASSERT (next_resource_type);

		Datum next_datum = next_resource_type->getDatum ();
		ASSERT (next_datum);

		if (next_resource->getscope () == ResourceImpl::Global_scope_Type) {
			header_file << "    " << next_datum->getName () << " * " << next_resource->getName () << ";\n";
		}
		else if (next_resource->getscope () == ResourceImpl::Local_scope_Type) {
			header_file << "    std::vector<" << next_datum->getName () << " * > " << next_resource->getName () << ";\n";
		}
	}

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Resources cannot have duplicate names
	///////////////////////////////////////////////////////////////
	detect_duplicates(ordered_resources);

	// Add any additional data members needed for analysis.
	XTEAM_IFrame::sim_gen ()->element_state (header_file);

	// End the class declaration.
	header_file << "};\n"
				<< "\n"
				<< "#endif\n";

	header_file.close ();

}

/*
Generate the implementation file for the element (atomic).
*/
void ElementImpl::generate_implementation ()
{

	// Include the needed header files.
	////////////////////////////////////////////////////////////
	//
	// pre-compilation 
	//
	////////////////////////////////////////////////////////////
	*this->impl_file_	<< "#include \"stdafx.h\"\n"
						<< "#include \"NewRandom.h\"\n"
						<< "#include \"" << this->name_ << ".h\"\n"
						<< "#include \"ServiceDemand.h\"\n"
						<< "#include \"ServiceCompletion.h\"\n";

	*this->impl_file_ << "\n";

	// Initialize all the input ports of the atomic.
	int port_index = 0;
	for (std::map<Input, int>::iterator event_input_iter = this->event_inputs_.begin (); event_input_iter != this->event_inputs_.end (); event_input_iter++)
		{
			std::pair<Input, int> next_event_input (*event_input_iter);
			Input port (next_event_input.first);
			ASSERT (port);
			this->event_inputs_[port] = port_index;
			*this->impl_file_ << "const port_t " << this->name_ << "::" << port->getName () << " = " << port_index << ";\n";
			port_index++;
		}

	// Initialize all the output ports of the atomic.
	for (std::map<Output, int>::iterator event_output_iter = this->event_outputs_.begin (); event_output_iter != this->event_outputs_.end (); event_output_iter++)
		{
			std::pair<Output, int> next_event_output (*event_output_iter);
			Output port (next_event_output.first);
			ASSERT (port);
			this->event_inputs_[port] = port_index;
			*this->impl_file_ << "const port_t " << this->name_ << "::" << port->getName () << " = " << port_index << ";\n";
			port_index++;
		}

	// Initialize the thread pool ports of the atomic.
	*this->impl_file_ << "const port_t " << this->name_ << "::ServiceDemandOutput = " << port_index << ";\n";
	port_index++;
	*this->impl_file_ << "const port_t " << this->name_ << "::ServiceCompletionInput = " << port_index << ";\n"
					  << "\n";

	// Generate the constructor.
	*this->impl_file_ << this->name_ << "::" << this->name_ << " (std::string element_id, std::ofstream& event_log):\n"
					  << "atomic ()\n"
					  << "{\n"
					  << "  this->element_id_ = element_id;\n"
					  << "  this->event_log_ = &event_log;\n"
					  << "}\n"
					  << "\n";

	// Generate the state initialization function.
	*this->impl_file_ << "void " << this->name_ << "::init ()\n"
					  << "{\n"
  					  << "#if VERBOSITY >= 2\n"
					  << "  *this->event_log_ << \"" << this->name_ << "::init ()\\n\";\n"
					  << "  this->event_log_->flush ();\n"
					  << "#endif\n";

	this->generate_init_function ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the external transition function.
	*this->impl_file_ << "void " << this->name_ << "::delta_ext (double e, const adevs_bag<PortValue>& x)\n"
					  << "{\n"
					  << "#if VERBOSITY >= 2\n"
					  << "  *this->event_log_ << \"" << this->name_ << "::delta_ext ()\\n\";\n"
					  << "  this->event_log_->flush ();\n"
					  << "#endif\n";

	this->generate_delta_ext ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the internal transition function.
	*this->impl_file_ << "void " << this->name_ << "::delta_int ()\n"
					  << "{\n"
					  << "#if VERBOSITY >= 2\n"
					  << "  *this->event_log_ << \"" << this->name_ << "::delta_int ()\\n\";\n"
					  << "  this->event_log_->flush ();\n"
  					  << "#endif\n";

	this->generate_delta_int ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the confluent transition function.
	*this->impl_file_ << "void " << this->name_ << "::delta_conf (const adevs_bag<PortValue>& x)\n"
					  << "{\n"
  					  << "#if VERBOSITY >= 2\n"
					  << "  *this->event_log_ << \"" << this->name_ << "::delta_conf ()\\n\";\n"
					  << "  this->event_log_->flush ();\n"
  					  << "#endif\n"
					  << "  delta_int ();\n"
					  << "  delta_ext (0.0, x);\n"
					  << "}\n"
					  << "\n";

	// Generate the output function.
	*this->impl_file_ << "void " << this->name_ << "::output_func (adevs_bag<PortValue>& y)\n"
					  << "{\n";
	this->generate_output_func ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the output value garbage collection.
	*this->impl_file_ << "void " << this->name_ << "::gc_output (adevs_bag<PortValue>& g)\n"
					  << "{\n"
  					  << "#if VERBOSITY >= 2\n"
					  << "  *this->event_log_ << \"" << this->name_ << "::gc_output ()\\n\";\n"
					  << "  this->event_log_->flush ();\n"
  					  << "#endif\n"
					  << "  for (int i = 0; i < g.getSize (); i++)\n"
					  << "    {\n"
					  << "      delete g[i].value;\n"
					  << "    }\n"
					  << "}\n"
					  << "\n";

	// Generate the destructor.
	*this->impl_file_ << this->name_ << "::~" << this->name_ << " ()\n"
					  << "{\n"
					  << "}\n"
					  << "\n";

	// Generate the event handler.
	*this->impl_file_ << "void " << this->name_ << "::handle_next_event (int thread_id)\n"
					  << "{\n";
	this->generate_event_handler ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the process transitioner.
	*this->impl_file_ << "void " << this->name_ << "::transition_process (int thread_id)\n"
					  << "{\n";
	this->generate_process_transitioner ();
	*this->impl_file_ << "}\n"
					  << "\n";

	// Generate the output generator.
	*this->impl_file_ << "void " << this->name_ << "::generate_output (int thread_id)\n"
					  << "{\n";
	this->generate_output_generator ();
	*this->impl_file_ << "}\n"
					  << "\n";

	(*this->impl_file_).close ();
	delete this->impl_file_;

}

/*
Generate the initialization function of the atomic.
*/
void ElementImpl::generate_init_function ()
{
	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Element has no process
	///////////////////////////////////////////////////////////////
	if(this->processes_.size() == 0)
	{
		std::stringstream ss;
		BON::Object obj (this);
		ss	<< this->getName()
			<< " [" << serialize_GUID(obj)
			<< "/" << this->getObjectMeta().name() << "]"
			<< " has no Process" << std::endl;
		err_msg	+= ss.str();
		return;
	}

	// a flag that says if this atom has an initial process
	// an atom is supposed to have an initial process
	int initial_processes_size = 0;

	// Find the initial process.
	Process initial_process;
	for (std::set<Process>::iterator process_iter = this->processes_.begin (); process_iter != this->processes_.end (); process_iter++)
	{
		Process next_process (*process_iter);
		ASSERT (next_process);

		if (next_process->isinitialProcess())
		{
			initial_process = next_process;
			initial_processes_size ++;
		}
	}

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Element has no initial Process
	///////////////////////////////////////////////////////////////
	if(initial_processes_size == 0)
	{
		std::stringstream ss;
		BON::Object obj (this);
		ss	<< this->getName() 
			<< " [" << serialize_GUID(obj)
			<< "/" << this->getObjectMeta().name() << "]"
			<< " has no initial Process" << std::endl;
		err_msg += ss.str();
		return;
	} 
	
	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Element has more than one initial process
	///////////////////////////////////////////////////////////////
	else if (initial_processes_size > 1)
	{
		std::stringstream ss;
		BON::Object obj (this);
		ss	<< this->getName() 
			<< " [" << serialize_GUID(obj)
			<< "/" << this->getObjectMeta().name() << "]"
			<< " has more than one initial Process" << std::endl;
		err_msg += ss.str();
		return;
	} 

	// Initialize all resources.
	std::set<Resource> resources = this->getResource ();
	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++)
		{
			Resource next_resource (*resource_iter);
			ASSERT (next_resource);

			//std::set<ResourceType> resource_types = next_resource->getResourceType ();
			//ResourceType next_resource_type = *(resource_types.begin ());
			//ASSERT (next_resource_type);

			//if (next_resource->getscope () == ResourceImpl::Global_scope_Type)
				//{

			*this->impl_file_ << "\n"
							<< "  // Begin initialization.\n"
							<< next_resource->getinitialValue ()
							<< "\n"
							<< "  // End initialization.\n"
							<< "\n";
				//}
		}

	// Get the start node.
	std::set<Start> initial_process_starts = initial_process->getStart();

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Initial Process has no start node
	///////////////////////////////////////////////////////////////
	if(initial_process_starts.size() == 0) {
		std::stringstream ss;
		ss		<< initial_process->getName() 
				<< " [" << serialize_GUID(initial_process)
				<< "/" << initial_process->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has no start node" << std::endl;
		err_msg += ss.str();
		return;
	}
	
	Start initial_process_start = *(initial_process_starts.begin ());
	ASSERT (initial_process_start);

	// Put the atomic in the initial process.
	std::string initial_process_name = initial_process->getName ();
	std::transform (initial_process_name.begin (), initial_process_name.end (), initial_process_name.begin (), toupper);

	*this->impl_file_ << "  this->initial_process_ = \"" << initial_process_name << "\";\n"
					  << "  this->thread_processes_.push_back (this->initial_process_);\n"
					  << "  int thread_id = 0;\n";

	// Initialize all local resources.
	resources = this->getResource ();
	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++)
		{
			Resource next_resource (*resource_iter);
			ASSERT (next_resource);

			//std::set<ResourceType> resource_types = next_resource->getResourceType ();
			//ResourceType next_resource_type = *(resource_types.begin ());
			//ASSERT (next_resource_type);

			if (next_resource->getscope () == ResourceImpl::Local_scope_Type)
				{
					*this->impl_file_ << "  " << next_resource->getName () << ".push_back (NULL);\n";
				}
		}

	// Get the first task.
	std::multiset<Task> initial_process_tasks = initial_process_start->getInitiateDsts();

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Start node has no following task
	///////////////////////////////////////////////////////////////
	if(initial_process_tasks.size() == 0)
	{
		std::stringstream ss;
		ss	<< "Start node in " << initial_process->getName()
			<< " in " << this->getName()
			<< " has no following Task" << std::endl;
		err_msg += ss.str();
		return;
	}

	Task initial_process_task = *(initial_process_tasks.begin ());
	ASSERT (initial_process_task);

	// Perform any analysis for the start of a task.
	this->on_task_begin (initial_process, initial_process_task, *this->impl_file_);

	std::string execution_time = initial_process_task->getexecutionTime();

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Task has no execution time
	///////////////////////////////////////////////////////////////
	if(execution_time.length() == 0)
	{
		std::stringstream ss;
		ss		<< initial_process_task->getName() 
				<< " [" << serialize_GUID(initial_process_task)
				<< "/" << initial_process_task->getObjectMeta().name() << "]"
				<< " in " << initial_process->getName() 
				<< " in " << this->getName() 
				<< " has no execution time" << std::endl;
		err_msg += ss.str();
		return;
	}

	std::string instruction = initial_process_task->gettaskInstruction ();

	// Perform the task instructions.
	*this->impl_file_ << "\n"
						<< "          // Begin task instructions.\n"
						<< instruction
						<< "\n"
						<< "          // End task instructions.\n"
						<< "\n";

	// Send a service demand for the task execution time to the thread pool.
	*this->impl_file_ << "  ServiceDemand * new_service_demand = new ServiceDemand (this->element_id_, thread_id, " << execution_time << ");\n"
					  << "  PortValue new_output_event (ServiceDemandOutput, new_service_demand);\n"
					  << "  this->output_queue_.push_back (new_output_event);\n"
					  << "  hold (0);\n"
  					  << "#if VERBOSITY >= 1\n"
					  << "  *this->event_log_ << \"" << this->name_ << " initialized\\n\";\n"
					  << "  this->event_log_->flush ();\n"
  					  << "#endif\n";

}

/*
Generate the external transition function for the atomic.
*/
void ElementImpl::generate_delta_ext ()
{
	/*
	For each input received, put it in one of two queues. If it is a service completion (signifying that a thread
	has completed a task) put it in the service queue. If it is an input from another element (atomic), put it in
	the input queue.
	*/
	*this->impl_file_ << "  for (int i = 0; i < x.getSize (); i++)\n"
					  << "    {\n"
					  << "      if (x.get(i).port == ServiceCompletionInput)\n"
					  << "        {\n"
					  << "          service_queue_.push_back (x.get(i));\n"
  					  << "          service_queue_.back ().value = service_queue_.back ().value->clone ();\n"
					  << "        }\n"
					  << "      else\n"
					  << "        {\n"
					  << "          input_queue_.push_back (x.get(i));\n"
    				  << "          input_queue_.back ().value = input_queue_.back ().value->clone ();\n"
					  << "        }\n"
					  << "    }\n";

	// For each service completion in the service queue, generate any needed output and transition to the next process.
	*this->impl_file_ << "  while (!this->service_queue_.empty ())\n"
					  << "    {\n"
					  << "      PortValue next_port_value = this->service_queue_.front ();\n"
					  << "      this->service_queue_.pop_front ();\n"
					  << "      ServiceCompletion * new_service_completion = (ServiceCompletion *)(next_port_value.value);\n"
					  << "      if (new_service_completion->element_id_ == this->element_id_)\n"
					  << "        {\n"
					  << "          int thread_id = new_service_completion->thread_id_;\n"
					  << "          this->generate_output (thread_id);\n"
					  << "          this->transition_process (thread_id);\n"
					  << "        }\n"
					  << "    }\n";

	// For each input in the input queue, determine the next task to be performed and send a service demand to the thread pool.
	*this->impl_file_ << "  while (!this->input_queue_.empty ())\n"
					  << "    {\n"
					  << "      int new_thread_id;\n"
					  << "      if (this->idle_threads_.empty ())\n"
					  << "        {\n"
					  << "          new_thread_id = this->thread_processes_.size ();\n"
					  << "          this->thread_processes_.push_back (this->initial_process_);\n";

	// Initialize all local resources.
	std::set<Resource> resources = this->getResource ();
	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++)
		{
			Resource next_resource (*resource_iter);
			ASSERT (next_resource);

			//std::set<ResourceType> resource_types = next_resource->getResourceType ();
			//ResourceType next_resource_type = *(resource_types.begin ());
			//ASSERT (next_resource_type);

			if (next_resource->getscope () == ResourceImpl::Local_scope_Type)
				{
					*this->impl_file_ << "  " << next_resource->getName () << ".push_back (NULL);\n";
				}
		}

	*this->impl_file_ << "        }\n"
					  << "      else\n"
					  << "        {\n"
					  << "          new_thread_id = this->idle_threads_.front ();\n"
					  << "          this->idle_threads_.pop_front ();\n"
					  << "          this->thread_processes_[new_thread_id] = this->initial_process_;\n";

	// Initialize all local resources.
	resources = this->getResource ();
	for (std::set<Resource>::iterator resource_iter = resources.begin (); resource_iter != resources.end (); resource_iter++)
		{
			Resource next_resource (*resource_iter);
			ASSERT (next_resource);

			//std::set<ResourceType> resource_types = next_resource->getResourceType ();
			//ResourceType next_resource_type = *(resource_types.begin ());
			//ASSERT (next_resource_type);

			if (next_resource->getscope () == ResourceImpl::Local_scope_Type)
				{
					*this->impl_file_ << "  " << next_resource->getName () << "[new_thread_id] = NULL;\n";
				}
		}

	*this->impl_file_ << "        }\n"
					  << "      this->handle_next_event (new_thread_id);\n"
					  << "    }\n"
					  << "  hold (0);\n";
}

/*
Generate the internal transition function for the atomic.
*/
void ElementImpl::generate_delta_int ()
{
	*this->impl_file_ << "  passivate ();\n";
}

/*
Generate the output function for the atomic.
*/
void ElementImpl::generate_output_func ()
{
	// Output each element in the output queue.
	*this->impl_file_ << "  while (!this->output_queue_.empty ())\n"
					  << "    {\n"
					  << "      output (this->output_queue_.front ().port, this->output_queue_.front ().value, y);\n"
					  << "      this->output_queue_.pop_front ();\n"
					  << "    }\n";
}

/*
Write the code transition state and perform the next task based on a conditional branch.
*/
void ElementImpl::write_conditional_state_change (ProcessTransition process_transition, std::string process_name, Event event, std::string indent)
{

	Process next_transition = process_transition->getProcess ();
	ASSERT (next_transition);

	// If we are transitioning to a new process, write it to the event log.
	std::string next_transition_name = next_transition->getName ();
	std::transform (next_transition_name.begin (), next_transition_name.end (), next_transition_name.begin (), toupper);

	if (next_transition_name != process_name)
		{

			*this->impl_file_ << "#if VERBOSITY >= 1\n"
							  << indent << "*this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process " << next_transition_name << " at time \" << timeCurrent () << \"\\n\";\n"
							  << indent << "this->event_log_->flush ();\n"
  							  << "#endif\n";
		}

	// Put the thread in the next process.
	*this->impl_file_ << indent << "this->thread_processes_[thread_id] = \"" << next_transition_name << "\";\n";

	std::set<Task> tasks = next_transition->getTask ();

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Process has no Task
	///////////////////////////////////////////////////////////////
	if(tasks.size() == 0)
	{
		std::stringstream ss;
		ss		<< next_transition->getName() 
				<< " [" << serialize_GUID(next_transition)
				<< "/" << next_transition->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has no Task" << std::endl;
		err_msg += ss.str();
		return;
	}
	Task next_task = *(tasks.begin ());
	ASSERT (next_task);

	// Perform any needed analysis for the beginning of a task.
	this->on_task_begin (next_transition, next_task, *this->impl_file_);

	std::string execution_time = next_task->getexecutionTime();

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// if no execution time, error
	///////////////////////////////////////////////////////////////
	if(execution_time.length() == 0)
	{
		std::stringstream ss;
		ss	<< next_task->getName() 
			<< " [" << serialize_GUID(next_task)
			<< "/" << next_task->getObjectMeta().name() << "]"
			<< " in " << next_transition->getName() 
			<< " in " << this->getName() 
			<< " has no execution time" << std::endl;
		err_msg += ss.str();
		return;
	}

	std::string instruction = next_task->gettaskInstruction ();

	// Perform the task instructions.
	*this->impl_file_ << "\n"
					  << indent << "// Begin task instructions.\n"
					  << instruction
					  << "\n"
					  << indent << "// End task instructions.\n"
					  << "\n";

	// Send a service demand for the task execution time to the thread pool.
	*this->impl_file_ << indent << "ServiceDemand * new_service_demand = new ServiceDemand (this->element_id_, thread_id, " << execution_time << ");\n"
					  << indent << "PortValue new_output_event (ServiceDemandOutput, new_service_demand);\n"
					  << indent << "this->output_queue_.push_back (new_output_event);\n"
                      << indent << "return;\n";

}

/*
Write the code for the handle_next_event () function. This function takes an input from the input queue and handles it
by determining the next task to be performed and sending a service demand for the task execution time to the thread pool.
*/
void ElementImpl::generate_event_handler ()
{

	// Get the next event in the input queue.
	*this->impl_file_ << "  PortValue next_event = this->input_queue_.front ();\n"
					  << "  this->input_queue_.pop_front ();\n"
					  << "  port_t arrival_port = next_event.port;\n"
					  << "\n";

	// For duplicate detection
	std::vector<BON::Object> ordered_processes;

	// Write the code to perform the next task based on the current state of the thread and the type of input received.
	//for (std::set<Process>::iterator process_iter = this->processes_.begin (); process_iter != this->processes_.end (); process_iter++)
	for (std::vector<Process>::iterator process_iter = this->ordered_processes_.begin(); process_iter != this->ordered_processes_.end(); process_iter++)
	{
		Process next_process (*process_iter);
		ASSERT (next_process);

		ordered_processes.push_back(next_process);

		std::string element_name = this->getName();
		std::string process_name = next_process->getName ();
		std::transform (process_name.begin (), process_name.end (), process_name.begin (), toupper);

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// Process cannot have "IDLE" as its name
		///////////////////////////////////////////////////////////////
		if(process_name == "IDLE") {
			std::stringstream ss;
			ss	<< next_process->getName() 
				<< " [" << serialize_GUID(next_process)
				<< "/" << next_process->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has a prohibited name \"IDLE\"" << std::endl;
			err_msg += ss.str();
			continue;
		}

		*this->impl_file_ << "  if (this->thread_processes_[thread_id] == \"" << process_name << "\")\n"
  						  << "    {\n";
		std::set<Event> events = next_process->getEvent ();
		for (std::set<Event>::iterator event_iter = events.begin (); event_iter != events.end (); event_iter++)
		{
			Event next_event (*event_iter);
			ASSERT (next_event);
			Input event_input (next_event->getPort ());
			if (event_input)
			{
				*this->impl_file_ << "      if (arrival_port == " << event_input->getName () << ")\n"
								  << "        {\n";

				BON::Model parent_model = event_input->getParent ();
				Interface parent_interface (parent_model);
				ASSERT (parent_interface);

				std::set<PortType> port_types = event_input->getPortType ();

				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Input has no Port Type
				///////////////////////////////////////////////////////////////
				if(port_types.size() == 0) {
					std::stringstream ss;
					ss	<< event_input->getName() 
						<< " [" << serialize_GUID(event_input)
						<< "/" << event_input->getObjectMeta().name() << "]"
						<< " in " << next_process->getName() 
						<< " in " << this->getName()
						<< " has no Port Type" << std::endl;
					err_msg += ss.str();
					continue;
				}

				PortType port_type = *(port_types.begin ());
				ASSERT (port_type);

				Datum data_type (port_type->getDatum ());
				ASSERT (data_type);

				std::string data_type_name = data_type->getName ();
				*this->impl_file_ << "#if VERBOSITY >= 1\n"
								  << "          *this->event_log_ << \"" << this->name_ << " received " << data_type_name << " at time \" << timeCurrent () << \"\\n\";\n"
								  << "          this->event_log_->flush ();\n"
								  << "#endif\n";


				*this->impl_file_ << "          " << data_type_name << " * " << next_event->getName () << " = (" << data_type_name << " *)(next_event.value);\n";

				std::multiset<ResourceUsage> resource_usages = next_event->getWriteDsts();
				for (std::multiset<ResourceUsage>::iterator resource_usage_iter = resource_usages.begin (); resource_usage_iter != resource_usages.end (); resource_usage_iter++)
				{
					ResourceUsage next_resource_usage (*resource_usage_iter);
					ASSERT (next_resource_usage);

					Resource next_resource = next_resource_usage->getResource ();
					ASSERT (next_resource);

					if (next_resource->getscope () == ResourceImpl::Local_scope_Type)
					{
						*this->impl_file_ << "          this->" << next_resource->getName () << "[thread_id] = (" << data_type_name << " *) " << next_event->getName () << "->clone ();\n";
					}
					else if (next_resource->getscope () == ResourceImpl::Global_scope_Type)
					{
						*this->impl_file_ << "          this->" << next_resource->getName () << " = (" << data_type_name << " *) " << next_event->getName () << "->clone ();\n";
					}
				}

				this->on_input (next_process, parent_interface, event_input, *this->impl_file_);

				*this->impl_file_ << "          delete " << next_event->getName () << ";\n";

				std::multiset<TransitionEnd> transitions = next_event->getTransitionDsts();
				for (std::multiset<TransitionEnd>::iterator transition_iter = transitions.begin (); transition_iter != transitions.end (); transition_iter++)
				{
					ProcessTransition next_transition_ref (*transition_iter);
					if (!next_transition_ref)
					{
						Stop stop_transition (*transition_iter);
						if (stop_transition)
						{
							*this->impl_file_ << "#if VERBOSITY >= 1\n"
											  << "          *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
											  << "          this->event_log_->flush ();\n"
											  << "#endif\n"
											  << "          this->thread_processes_[thread_id] = \"IDLE\";\n"
											  << "          this->idle_threads_.push_back (thread_id);\n"
											  << "          return;\n"
											  << "        }\n";
						}
						else
						{
							// The next transition must be a conditional branch.
							Conditional conditional_transition (*transition_iter);
							ASSERT (conditional_transition);


							///////////////////////////////////////////////////////////////
							// CoDesign Modification
							// Retrieves else and if branches in a customized way
							///////////////////////////////////////////////////////////////
							
							// defines empty sets for else and if branches
							std::set<ElseBranch> else_branches;
							std::set<IfBranch> if_branches;
							
							// gets all branches and iterates through them
							std::set<BON::Connection> branches = conditional_transition->getOutBranchLinks();
							std::set<BON::Connection>::iterator it = branches.begin();
							for( ; it != branches.end(); ++it)
							{
								IfBranch	a(*it);
								ElseBranch	c(*it);

								if (a) {
									if_branches.insert(a);
								} else if (c) {
									else_branches.insert(c);
								}
							}

							///////////////////////////////////////////////////////////////
							// CoDesign Modification
							// Conditional has no if branch
							///////////////////////////////////////////////////////////////
							if(if_branches.size() == 0)
							{
								std::stringstream ss;
								ss	<< next_process->getName() 
									<< " [" << serialize_GUID(next_process)
									<< "/" << next_process->getObjectMeta().name() << "]"
									<< " in " << this->getName()
									<< " has a Conditional with no if branch" << std::endl;
								err_msg += ss.str();
								continue;
							}

							for (std::set<IfBranch>::iterator if_branch_iter = if_branches.begin (); if_branch_iter != if_branches.end (); if_branch_iter++)
							{
								IfBranch next_if_branch (*if_branch_iter);
								ASSERT (next_if_branch);

								if (if_branch_iter == if_branches.begin ())
								{
									*this->impl_file_ << "          if ";
								}
								else
								{
									*this->impl_file_ << "          else if ";
								}

								*this->impl_file_ << "(" << next_if_branch->getexpression () << ")\n"
												  << "            {\n";

								BranchEnd if_branch_end = next_if_branch->getDst ();
								Stop if_stop_branch_end (if_branch_end);
								if (if_stop_branch_end)
								{
									*this->impl_file_ << "#if VERBOSITY >= 1\n"
													  << "              *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
													  << "              this->event_log_->flush ();\n"
													  << "#endif\n"
													  << "              this->thread_processes_[thread_id] = \"IDLE\";\n"
													  << "              this->idle_threads_.push_back (thread_id);\n"
													  << "              return;\n"
													  << "            }\n";
								}
								else
								{
									ProcessTransition process_transition_branch_end (if_branch_end);
									ASSERT (process_transition_branch_end);

									this->write_conditional_state_change (process_transition_branch_end, process_name, next_event, "              ");
									*this->impl_file_ << "            }\n";
								}

							}

							*this->impl_file_ << "          else\n"
											  << "            {\n";


							///////////////////////////////////////////////////////////////
							// CoDesign Modification
							// Conditional has no else branch
							///////////////////////////////////////////////////////////////
							
							if(else_branches.size() == 0)
							{
								std::stringstream ss;
								ss	<< next_process->getName() 
									<< " [" << serialize_GUID(next_process)
									<< "/" << next_process->getObjectMeta().name() << "]" 
									<< " in " << this->getName()
									<< " has a Conditional with no else branch" << std::endl;
								err_msg += ss.str();
								continue;
							}

							///////////////////////////////////////////////////////////////
							// CoDesign Modification
							// Else branch has no destination
							///////////////////////////////////////////////////////////////
							std::multiset<BranchEnd> else_branch_ends = conditional_transition->getElseBranchDsts();
							if(else_branch_ends.size() == 0) 
							{
								std::stringstream ss;
								ss	<< next_process->getName() 
									<< " [" << serialize_GUID(next_process)
									<< "/" << next_process->getObjectMeta().name() << "]"
									<< " in " << this->getName()
									<< " has an else branch with no destination" << std::endl;
								err_msg += ss.str();
								continue;
							}

							Stop else_stop_branch_end = *(else_branch_ends.begin ());
							if (else_stop_branch_end)
							{
								*this->impl_file_ << "#if VERBOSITY >= 1\n"
												  << "              *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
												  << "              this->event_log_->flush ();\n"
												  << "#endif\n"
												  << "              this->thread_processes_[thread_id] = \"IDLE\";\n"
												  << "              this->idle_threads_.push_back (thread_id);\n"
												  << "              return;\n"
												  << "            }\n";
							}
							else
							{
								ProcessTransition process_transition_branch_end = *(else_branch_ends.begin ());
								ASSERT (process_transition_branch_end);

								this->write_conditional_state_change (process_transition_branch_end, process_name, next_event, "              ");
								*this->impl_file_ << "            }\n";
							}

							*this->impl_file_ << "        }\n";
											  
						}
					}
					else
					{
						Process next_transition = next_transition_ref->getProcess ();

						///////////////////////////////////////////////////////////////
						// CoDesign Modification
						// Referenced process cannot be found
						///////////////////////////////////////////////////////////////
						if(!next_transition)
						{
							std::stringstream ss;
							ss	<< next_event->getName()
								<< " [" << serialize_GUID(next_event)
								<< "/" << next_event->getObjectMeta().name() << "]"
								<< " in " << next_process->getName()
								<< " in " << this->getName()
								<< " is connected to a broken reference" << std::endl;
							err_msg += ss.str();
							continue;
						}
						ASSERT (next_transition);

						std::string next_transition_name = next_transition->getName ();
						std::transform (next_transition_name.begin (), next_transition_name.end (), next_transition_name.begin (), toupper);
						if (next_transition_name != process_name)
						{
							*this->impl_file_ << "#if VERBOSITY >= 1\n"
											  << "          *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process " << next_transition_name << " at time \" << timeCurrent () << \"\\n\";\n"
											  << "          this->event_log_->flush ();\n"
											  << "#endif\n";
						}
						*this->impl_file_ << "          this->thread_processes_[thread_id] = \"" << next_transition_name << "\";\n";

						std::set<Task> tasks = next_transition->getTask ();

						///////////////////////////////////////////////////////////////
						// CoDesign Modification
						// Process has no Task
						///////////////////////////////////////////////////////////////
						if(tasks.size() == 0)
						{
							std::stringstream ss;
							ss	<< next_transition->getName()
								<< " [" << serialize_GUID(next_transition)
								<< "/" << next_transition->getObjectMeta().name() << "]"
								<< " in " << this->getName()
								<< " has no Task" << std::endl;
							err_msg += ss.str();
							continue;
						}

						Task next_task = *(tasks.begin ());
						ASSERT (next_task);

						this->on_task_begin (next_transition, next_task, *this->impl_file_);

						std::string execution_time = next_task->getexecutionTime();

						///////////////////////////////////////////////////////////////
						// CoDesign Modification
						// Task has no execution time
						///////////////////////////////////////////////////////////////
						if(execution_time.length() == 0)
						{
							std::stringstream ss;
							ss		<< next_task->getName() 
									<< " [" << serialize_GUID(next_task)
									<< "/" << next_task->getObjectMeta().name() << "]"
									<< " in " << next_transition->getName() 
									<< " in " << this->getName() 
									<< " has no execution time" << std::endl;
							err_msg += ss.str();
							continue;
						}

						std::string instruction = next_task->gettaskInstruction ();

						*this->impl_file_ << "\n"
										  << "          // Begin task instructions.\n"
										  << instruction
										  << "\n"
										  << "          // End task instructions.\n"
										  << "\n";

						*this->impl_file_ << "          ServiceDemand * new_service_demand = new ServiceDemand (this->element_id_, thread_id, " << execution_time << ");\n"
										  << "          PortValue new_output_event (ServiceDemandOutput, new_service_demand);\n"
										  << "          this->output_queue_.push_back (new_output_event);\n"
										  << "          return;\n"
										  << "        }\n";
					}
				}
			}
		}

		*this->impl_file_ << "    }\n";

	}

	///////////////////////////////////////////////////////////////
	// CoDesign Modification
	// Processes cannot have duplicate names
	///////////////////////////////////////////////////////////////
	detect_duplicates(ordered_processes);
}

void ElementImpl::generate_process_transitioner ()
{
	//for (std::set<Process>::iterator process_iter = this->processes_.begin (); process_iter != this->processes_.end (); process_iter++)
	for (std::vector<Process>::iterator process_iter = this->ordered_processes_.begin(); process_iter != this->ordered_processes_.end(); process_iter++)
	{
		Process next_process (*process_iter);
		ASSERT (next_process);

		std::set<Task> tasks = next_process->getTask ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// Process has no Task
		///////////////////////////////////////////////////////////////
		if(tasks.size() == 0)
		{
			std::stringstream ss;
			ss	<< next_process->getName() 
				<< " [" << serialize_GUID(next_process)
				<< "/" << next_process->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has no Task" << std::endl;
			err_msg += ss.str();
			continue;
		}

		Task finished_task = *(tasks.begin ());
		ASSERT (finished_task);

		std::string process_name = next_process->getName ();
		std::transform (process_name.begin (), process_name.end (), process_name.begin (), toupper);
		*this->impl_file_ << "  if (this->thread_processes_[thread_id] == \"" << process_name << "\")\n"
  						  << "    {\n";

		this->on_task_end (next_process, finished_task, *this->impl_file_);

		TransitionEnd next_transition;
		Event next_event;

		////////////////////////////////////////////////////////////////
		// Event atoms handling
		////////////////////////////////////////////////////////////////
		
		std::set<Event> events = next_process->getEvent ();
		if (events.size () > 0)
		{
			next_event = *(events.begin ());
			ASSERT (next_event);
			Input event_input (next_event->getPort ());
			Output event_output (next_event->getPort ());

			// if the Event is an input event
			if (event_input)
			{
				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Only initial process can have an input event
				/////////////////////////////////////////////////////////////// 

				if (!next_process->isinitialProcess()) {
					std::stringstream ss;
					ss	<< next_process->getName() 
						<< " [" << serialize_GUID(next_process)
						<< "/" << next_process->getObjectMeta().name() << "]"
						<< " in " << this->getName()
						<< " is not the initial process, and cannot have an input event" << std::endl;
					err_msg += ss.str();
				}
				
				*this->impl_file_ << "#if VERBOSITY >= 1\n"
								  << "      *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
								  << "      this->event_log_->flush ();\n"
								  << "#endif\n"
								  << "      this->thread_processes_[thread_id] = \"IDLE\";\n"
								  << "      this->idle_threads_.push_back (thread_id);\n"
								  << "      return;\n"
								  << "    }\n";
				
				// Nothing further needed for this process; proceed to next process.
				continue;
			}
			// if the Event is an output event
			else if(event_output)
			{
				std::multiset<TransitionEnd> transitions = next_event->getTransitionDsts ();

				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// if there is no transition destination, it is an error
				///////////////////////////////////////////////////////////////
				if(transitions.size() == 0)
				{
					std::stringstream ss;
					ss	<< next_event->getName() 
						<< " [" << serialize_GUID(next_event)
						<< "/" << next_event->getObjectMeta().name() << "]"
						<< " in " << next_process->getName()
						<< " in " << this->getName()
						<< " has no Transition Destination" << std::endl;
					err_msg += ss.str();
					continue;
				}

				next_transition = *(transitions.begin ());
				ASSERT (next_transition);
			}
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// if there is an event that refers to a port that does not exist
			///////////////////////////////////////////////////////////////
			else
			{
				std::stringstream ss;
				ss	<< next_event->getName() 
					<< " [" << serialize_GUID(next_event)
					<< "/" << next_event->getObjectMeta().name() << "]"
					<< " in " << next_process->getName()
					<< " in " << this->getName()
					<< " is referencing a Port that does not exist" << std::endl;
				err_msg += ss.str();
				continue;
			}

		}

		// if there is no Event atom in the process
		else
		{
			std::multiset<Initiator> initiators = finished_task->getContinueDsts ();

			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Task has no Destination
			///////////////////////////////////////////////////////////////
			if(initiators.size() == 0)
			{
				std::stringstream ss;
				ss		<< finished_task->getName() 
						<< " [" << serialize_GUID(finished_task)
						<< "/" << finished_task->getObjectMeta().name() << "]"
						<< " in " << next_process->getName()
						<< " in " << this->getName()
						<< " has no Destiantion" << std::endl;
				err_msg += ss.str();
				continue;
			}

			next_transition = *(initiators.begin ());
			
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Task destination is open-ended
			///////////////////////////////////////////////////////////////
			if(!next_transition) 
			{
				std::stringstream ss;
				ss		<< finished_task->getName() 
						<< " [" << serialize_GUID(finished_task)
						<< "/" << finished_task->getObjectMeta().name() << "]"
						<< " in " << next_process->getName()
						<< " in " << this->getName()
						<< " has an open-ended destination" << std::endl;
				err_msg += ss.str();
				continue;
			}

			ASSERT (next_transition);
		}

		ProcessTransition transition_process_ref (next_transition);
		if (!transition_process_ref)
		{
			Stop stop_transition (next_transition);
			if (stop_transition)
			{
				*this->impl_file_ << "#if VERBOSITY >= 1\n"
								  << "      *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
								  << "      this->event_log_->flush ();\n"
								  << "#endif\n"
								  << "      this->thread_processes_[thread_id] = \"IDLE\";\n"
								  << "      this->idle_threads_.push_back (thread_id);\n"
								  << "      return;\n";
			}
			else
			{
				// The next transition must be a conditional branch.
				Conditional conditional_transition (next_transition);
				ASSERT (conditional_transition);




				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Retrieves else and if branches in a customized way
				///////////////////////////////////////////////////////////////
				
				// defines empty sets for else and if branches
				std::set<ElseBranch> else_branches;
				std::set<IfBranch> if_branches;
				
				// gets all branches and iterates through them
				std::set<BON::Connection> branches = conditional_transition->getOutBranchLinks();
				std::set<BON::Connection>::iterator it = branches.begin();
				for( ; it != branches.end(); ++it)
				{
					IfBranch	a(*it);
					ElseBranch	c(*it);

					if (a) {
						if_branches.insert(a);
					} else if (c) {
						else_branches.insert(c);
					}
				}

				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Conditional has no if branch
				///////////////////////////////////////////////////////////////
				if(if_branches.size() == 0)
				{
					std::stringstream ss;
					ss	<< next_process->getName() 
						<< " [" << serialize_GUID(next_process)
						<< "/"	<< next_process->getObjectMeta().name() << "]"
						<< " in " << this->getName() 
						<< " has no if branch" << std::endl;
					err_msg += ss.str();
					continue;
				}

				for (std::set<IfBranch>::iterator if_branch_iter = if_branches.begin (); if_branch_iter != if_branches.end (); if_branch_iter++)
				{
					IfBranch next_if_branch (*if_branch_iter);
					ASSERT (next_if_branch);

					if (if_branch_iter == if_branches.begin ())
					{
						*this->impl_file_ << "      if ";
					}
					else
					{
						*this->impl_file_ << "      else if ";
					}

					*this->impl_file_ << "(" << next_if_branch->getexpression () << ")\n"
									  << "        {\n";

					BranchEnd if_branch_end = next_if_branch->getDst ();
					Stop if_stop_branch_end (if_branch_end);
					if (if_stop_branch_end)
					{
						*this->impl_file_ << "#if VERBOSITY >= 1\n"
										  << "          *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
										  << "          this->event_log_->flush ();\n"
										  << "#endif\n"
										  << "          this->thread_processes_[thread_id] = \"IDLE\";\n"
										  << "          this->idle_threads_.push_back (thread_id);\n"
										  << "          return;\n"
										  << "        }\n";
					}
					else
					{
						ProcessTransition process_transition_branch_end (if_branch_end);
						ASSERT (process_transition_branch_end);

						this->write_conditional_state_change (process_transition_branch_end, process_name, next_event, "          ");
						*this->impl_file_ << "        }\n";
					}

				}

				*this->impl_file_ << "      else\n"
								  << "        {\n";

				
				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Conditional has no else branch
				///////////////////////////////////////////////////////////////
				if(else_branches.size() == 0)
				{
					std::stringstream ss;
					ss	<< next_process->getName() 
						<< " [" << serialize_GUID(next_process)
						<< "/"	<< next_process->getObjectMeta().name() << "]"
						<< " in " << this->getName()
						<< " has no else branch" << std::endl;
					err_msg += ss.str();
					continue;
				}

				std::multiset<BranchEnd> else_branch_ends = conditional_transition->getElseBranchDsts();

				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Else branch has no Destination
				///////////////////////////////////////////////////////////////
				if(else_branch_ends.size() == 0)
				{
					std::stringstream ss;
					ss	<< next_process->getName() 
						<< " [" << serialize_GUID(next_process)
						<< "/" << next_process->getObjectMeta().name() << "]"
						<< " in " << this->getName()
						<< " has an else branch with no destination" << std::endl;
					err_msg += ss.str();
					continue;
				}

				Stop else_stop_branch_end = *(else_branch_ends.begin ());
				if (else_stop_branch_end)
				{
					*this->impl_file_ << "#if VERBOSITY >= 1\n"
									  << "          *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process IDLE at time \" << timeCurrent () << \"\\n\";\n"
									  << "          this->event_log_->flush ();\n"
									  << "#endif\n"
									  << "          this->thread_processes_[thread_id] = \"IDLE\";\n"
									  << "          this->idle_threads_.push_back (thread_id);\n"
									  << "          return;\n"
									  << "        }\n";
				}
				else
				{
					ProcessTransition process_transition_branch_end = *(else_branch_ends.begin ());
					ASSERT (process_transition_branch_end);

					this->write_conditional_state_change (process_transition_branch_end, process_name, next_event, "          ");
					*this->impl_file_ << "        }\n";		
				}
	
			}
		}
		else
		{
			Process transition_process = transition_process_ref->getProcess ();
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Process reference broken
			///////////////////////////////////////////////////////////////
			if(!transition_process)
			{
				std::stringstream ss;
				ss		<< transition_process_ref->getName() 
						<< " [" << serialize_GUID(transition_process_ref)
						<< "/" << transition_process_ref->getObjectMeta().name() << "]"
						<< " in " << this->getName()
						<< " is a broken reference" << std::endl;
				err_msg += ss.str();
				continue;
			}
			ASSERT (transition_process);

			std::string transition_process_name = transition_process->getName ();
			std::transform (transition_process_name.begin (), transition_process_name.end (), transition_process_name.begin (), toupper);
			if (transition_process_name != process_name)
				{
					*this->impl_file_ << "#if VERBOSITY >= 1\n"
									  << "      *this->event_log_ << \"" << this->name_ << " : \" << thread_id << \" transitioning to process " << transition_process_name << " at time \" << timeCurrent () << \"\\n\";\n"
									  << "      this->event_log_->flush ();\n"
									  << "#endif\n";

				}
			*this->impl_file_ << "      this->thread_processes_[thread_id] = \"" << transition_process_name << "\";\n";

			std::set<Task> tasks = transition_process->getTask ();

			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Process has no Task
			///////////////////////////////////////////////////////////////
			if(tasks.size() == 0)
			{
				std::stringstream ss;
				ss		<< transition_process->getName() 
						<< " [" << serialize_GUID(transition_process)
						<< "/" << transition_process->getObjectMeta().name() << "]"
						<< " in " << this->getName()
						<< " has no Task" << std::endl;
				err_msg += ss.str();
				continue;
			}

			Task next_task = *(tasks.begin ());
			ASSERT (next_task);

			this->on_task_begin (transition_process, next_task, *this->impl_file_);

			std::string execution_time = next_task->getexecutionTime();

			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// if no execution time, error
			///////////////////////////////////////////////////////////////
			if(execution_time.length() == 0)
			{
				std::stringstream ss;
				ss	<< next_task->getName() 
					<< " [" << serialize_GUID(next_task)
					<< "/" << next_task->getObjectMeta().name() << "]"
					<< " in " << transition_process->getName() 
					<< " in " << this->getName() 
					<< " has no execution time" << std::endl;
				err_msg += ss.str();
				continue;
			}

			std::string instruction = next_task->gettaskInstruction ();

			*this->impl_file_ << "\n"
							  << "      // Begin task instructions.\n"
							  << instruction
							  << "\n"
							  << "      // End task instructions.\n"
							  << "\n";

			*this->impl_file_ << "      ServiceDemand * new_service_demand = new ServiceDemand (this->element_id_, thread_id, " << execution_time << ");\n"
							  << "      PortValue new_output_event (ServiceDemandOutput, new_service_demand);\n"
							  << "      this->output_queue_.push_back (new_output_event);\n"
							  << "      return;\n";
		}
		*this->impl_file_ << "    }\n";
	}
}

void ElementImpl::generate_output_generator ()
{					  
	for (std::vector<Process>::iterator process_iter = this->ordered_processes_.begin (); process_iter != this->ordered_processes_.end (); process_iter++)
	{
		Process next_process (*process_iter);
		ASSERT (next_process);

		std::string process_name = next_process->getName ();
		std::transform (process_name.begin (), process_name.end (), process_name.begin (), toupper);
		*this->impl_file_ << "  if (this->thread_processes_[thread_id] == \"" << process_name << "\")\n"
						  << "    {\n";

		std::set<Event> output_events	= next_process->getOutputEvent();
		std::set<Event> input_events	= next_process->getInputEvent();
		std::set<Event> events			= next_process->getEvent ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// Process could only have Inputs or an Output Event
		///////////////////////////////////////////////////////////////
		if(input_events.size() > 0 && output_events.size() > 0) {
			std::stringstream ss;
			ss	<< next_process->getName() 
				<< " [" << serialize_GUID(next_process)
				<< "/" << next_process->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has both Input and Output Events" << std::endl;
			err_msg += ss.str();
			continue;
		}

		///////////////////////////////////////////////////////////////
		// CoDesign Modification
		// Process could not have more than one Output Event
		///////////////////////////////////////////////////////////////
		if(output_events.size() > 1) {
			std::stringstream ss;
			ss	<< next_process->getName() 
				<< " [" << serialize_GUID(next_process)
				<< "/" << next_process->getObjectMeta().name() << "]"
				<< " in " << this->getName()
				<< " has more than one Output Event" << std::endl;
			err_msg += ss.str();
			continue;
		}

		if (events.size () > 0) {
			Event next_event = *(events.begin ());
			ASSERT (next_event);
			Output event_output (next_event->getPort ());
			if (event_output) {
				std::set<PortType> port_types = event_output->getPortType ();

				///////////////////////////////////////////////////////////////
				// CoDesign Modification
				// Output has no Port Type
				///////////////////////////////////////////////////////////////
				if(port_types.size() == 0) {
					std::stringstream ss;
					ss	<< event_output->getName() 
						<< " [" << serialize_GUID(event_output)
						<< "/" << event_output->getObjectMeta().name() << "]"
						<< " in " << next_process->getName() 
						<< " in " << this->getName()
						<< " has no Port Type" << std::endl;
					err_msg += ss.str();
					continue;
				}

				PortType port_type = *(port_types.begin ());
				ASSERT (port_type);
				Datum data_type (port_type->getDatum ());
				ASSERT (data_type);

				BON::Model parent_model = event_output->getParent ();
				Interface parent_interface (parent_model);
				ASSERT (parent_interface);

				std::multiset<ResourceUsage> resource_usages = next_event->getReadSrcs ();
				if (resource_usages.size () > 0) {
					ResourceUsage next_resource_usage = *(resource_usages.begin ());
					Resource next_resource = next_resource_usage->getResource ();
					ASSERT (next_resource);

					if (next_resource->getscope () == ResourceImpl::Local_scope_Type) {
						*this->impl_file_ << "      " << data_type->getName () << " * new_event = (" << data_type->getName () << " *) this->" << next_resource->getName () << "[thread_id]->clone ();\n";
					} else if (next_resource->getscope () == ResourceImpl::Global_scope_Type) {
						*this->impl_file_ << "      " << data_type->getName () << " * new_event = (" << data_type->getName () << " *) this->" << next_resource->getName () << "->clone ();\n";
					}
				} else {
					*this->impl_file_ << "      " << data_type->getName () << " * new_event = new " << data_type->getName () << ";\n";
				}

				this->on_output (next_process, parent_interface, event_output, *this->impl_file_);

				*this->impl_file_ << "      PortValue new_output (" << event_output->getName () << ", new_event);\n"
								  << "      this->output_queue_.push_back (new_output);\n"
								  << "#if VERBOSITY >= 1\n"
								  << "      *this->event_log_ << \"" << this->name_ << " sent " << data_type->getName () << " at time \" << timeCurrent () << \"\\n\";\n"
								  << "      this->event_log_->flush ();\n"
								  << "#endif\n"
								  << "      return;\n";
			}
		}
		*this->impl_file_ << "    }\n";
	}
}

void ComponentImpl::on_input (Process process, Interface intf, Input input, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_component_input (this, process, intf, input, file);
}

void ConnectorImpl::on_input (Process process, Interface intf, Input input, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_connector_input (this, process, intf, input, file);
}

void ComponentImpl::on_output (Process process, Interface intf, Output output, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_component_output (this, process, intf, output, file);
}

void ConnectorImpl::on_output (Process process, Interface intf, Output output, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_connector_output (this, process, intf, output, file);
}

void ComponentImpl::on_task_begin (Process process, Task task, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_component_task_begin (this, process, task, file);
}

void ConnectorImpl::on_task_begin (Process process, Task task, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_connector_task_begin (this, process, task, file);
}

void ComponentImpl::on_task_end (Process process, Task task, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_component_task_end (this, process, task, file);
}

void ConnectorImpl::on_task_end (Process process, Task task, std::ofstream& file)
{
	XTEAM_IFrame::sim_gen ()->on_connector_task_end (this, process, task, file);
}

void DatumImpl::generate ()
{
	std::string object_name = this->getName ();

	std::string object_file_name = version_path + object_name + ".h";
	std::ofstream object_file (object_file_name.c_str ());

	std::string define_var_name = object_name + "_H";
	std::transform (define_var_name.begin (), define_var_name.end (), define_var_name.begin (), toupper);

	object_file << "#ifndef " << define_var_name << "\n"
				<< "#define " << define_var_name << "\n"
				<< "\n"
				////////////////////////////////////////////////////////////
				//
				// pre-compilation 
				//
				////////////////////////////////////////////////////////////
				//<< "#include <string>\n"
				//<< "#include \"adevs.h\"\n"
				<< "#include \"NewRandom.h\"\n";
				
	std::multiset<Datum> contained_types = this->getContainmentSrcs ();
	for (std::multiset<Datum>::iterator sub_object_iter = contained_types.begin (); sub_object_iter != contained_types.end (); sub_object_iter++)
		{
			Datum next_sub_object (*sub_object_iter);
			ASSERT (next_sub_object);

			object_file << "#include \"" << next_sub_object->getName () << ".h\"\n";
		}

	// Include the headers for all the base types of this class.
	std::multiset<Inheritance> inheritances = this->getDerivedInheritanceSrcs ();
	for (std::multiset<Inheritance>::iterator inheritance_iter = inheritances.begin (); inheritance_iter != inheritances.end (); inheritance_iter++)
		{
			Inheritance next_inheritance (*inheritance_iter);
			ASSERT (next_inheritance);

			std::multiset<Datum> base_types = next_inheritance->getBaseInheritanceSrcs ();
		
			///////////////////////////////////////////////////////////////
			// CoDesign Modification
			// Inheritance has no Datum
			///////////////////////////////////////////////////////////////
			if(base_types.size() == 0)
			{
				std::stringstream ss;
				ss		<< next_inheritance->getName() 
						<< " [" << serialize_GUID(next_inheritance)
						<< "/" << next_inheritance->getObjectMeta().name() << "]"
						<< " in " << this->getName() 
						<< " has no base type Datum" << std::endl;
				err_msg += ss.str();
				continue;
			}

			Datum base_type = *(base_types.begin ());
			ASSERT (base_type);

			object_file << "#include \"" << base_type->getName () << ".h\"\n";
		}

	object_file << "\n"
				<< "class " << object_name << ": ";

	// Insert all the base classes of this class into a set.
	std::set<std::string> base_classes;
	if (inheritances.empty ())
		{
			base_classes.insert ("object");
		}
	else
		{
			for (std::multiset<Inheritance>::iterator inheritance_iter = inheritances.begin (); inheritance_iter != inheritances.end (); inheritance_iter++)
				{
					Inheritance next_inheritance (*inheritance_iter);
					ASSERT (next_inheritance);

					std::multiset<Datum> base_types = next_inheritance->getBaseInheritanceSrcs ();

					///////////////////////////////////////////////////////////////
					// CoDesign Modification
					// Inheritance has no Datum
					///////////////////////////////////////////////////////////////
					if(base_types.size() == 0)
					{
						std::stringstream ss;
						ss	<< next_inheritance->getName() 
							<< " [" << serialize_GUID(next_inheritance)
							<< "/" << next_inheritance->getObjectMeta().name() << "]"
							<< " in " << this->getName() 
							<< " has no base type Datum" << std::endl;
						err_msg += ss.str();
						continue;
					}

					Datum base_type = *(base_types.begin ());
					ASSERT (base_type);

					base_classes.insert (base_type->getName ());
				}
		}

	// Write the classes that this class inherits from.
	for (std::set<std::string>::iterator base_class_iter = base_classes.begin (); base_class_iter != base_classes.end (); base_class_iter++)
		{
			std::string next_base_class (*base_class_iter);
			ASSERT (!next_base_class.empty ());

			if (base_class_iter != base_classes.begin ())
				{
					object_file << ", ";
				}

			object_file << "public " << next_base_class;
		}

	object_file << "\n"
				<< "{\n"
				<< "  public:\n"
				<< "\n";

	// Generate the constructor.
	object_file	<< "    " << object_name << " ():\n";

	for (std::set<std::string>::iterator base_class_iter = base_classes.begin (); base_class_iter != base_classes.end (); base_class_iter++)
		{
			std::string next_base_class (*base_class_iter);
			ASSERT (!next_base_class.empty ());

			if (base_class_iter != base_classes.begin ())
				{
					object_file << ", ";
				}

			object_file << "    " << next_base_class << " ()\n";
		}

	object_file << "      {\n";

	if (!this->getvalue ().empty ())
		{
			object_file << "        " << this->getvalue () << "\n";
		}
	if (!this->getsize ().empty ())
		{
			object_file << "        this->size_ = " << this->getsize () << ";\n";
		}

	object_file << "      }\n"
				<< "\n";

	// Generate the copy constructor.
	object_file << "    " << object_name << " (const " << object_name << "& src):\n";

	std::set<std::string>::iterator copy_class_iter = base_classes.begin ();
	std::string first_base_class (*copy_class_iter);
	ASSERT (!first_base_class.empty ());
	
	object_file << "    " << first_base_class << " (src)";

	copy_class_iter++;
	for (std::set<std::string>::iterator base_class_iter = copy_class_iter; base_class_iter != base_classes.end (); base_class_iter++)
		{
			std::string next_base_class (*base_class_iter);
			ASSERT (!next_base_class.empty ());

			object_file << ",\n"
						<< "    " << next_base_class << " (src)\n";
		}

	for (std::multiset<Datum>::iterator contained_types_iter = contained_types.begin (); contained_types_iter != contained_types.end (); contained_types_iter++)
		{
			Datum next_datum (*contained_types_iter);
			ASSERT (next_datum);

			std::string contained_datum_name = next_datum->getName ();
			object_file << ",\n"
						<< "    " << next_datum->getName () << "_ (src." << next_datum->getName () << "_)";
		}

	if (!this->gettype ().empty ())
		{
			object_file << ",\n"
						<< "    value_ (src.value_)";
		}
	if (!this->getsize ().empty ())
		{
			object_file << ",\n"
						<< "    size_ (src.size_)";
		}
	object_file	<< "\n" 
				<< "      {\n"
				<< "      }\n"
				<< "\n";

	// Generate the assignment operator.
	object_file << "    " << object_name << "& operator= (const " << object_name << "& src)\n"
				<< "      {\n";
				

	object_file << "        " << first_base_class << "::operator= (src);\n";

	for (std::set<std::string>::iterator base_class_iter = copy_class_iter; base_class_iter != base_classes.end (); base_class_iter++)
		{
			std::string next_base_class (*base_class_iter);
			ASSERT (!next_base_class.empty ());

			object_file << "        " << next_base_class << "::operator= (src);\n";
		}

	for (std::multiset<Datum>::iterator contained_types_iter = contained_types.begin (); contained_types_iter != contained_types.end (); contained_types_iter++)
		{
			Datum next_datum (*contained_types_iter);
			ASSERT (next_datum);

			std::string contained_datum_name = next_datum->getName ();
			object_file << "        this->" << next_datum->getName () << "_ = src." << next_datum->getName () << "_;\n";
		}

	if (!this->gettype ().empty ())
		{
			object_file << "        this->value_ = src.value_;\n";
		}
	if (!this->getsize ().empty ())
		{
			object_file << "        this->size_ = src.size_;\n";
		}
	object_file	<< "        return *this;\n"
				<< "      }\n"
				<< "\n";


	// Generate the clone method.
	object_file << "    virtual object * clone () const\n"
				<< "      {\n"
				<< "        return new " << object_name << " (*this);\n"
				<< "      }\n"
				<< "\n";

	// Generate the size method.
	if (this->getsize ().empty ())
		{
			object_file << "    virtual double& size ()\n"
						<< "      {\n"
						<< "        this->size_ = ";

			for (std::multiset<Datum>::iterator contained_types_iter = contained_types.begin (); contained_types_iter != contained_types.end (); contained_types_iter++)
				{
					Datum next_datum (*contained_types_iter);
					ASSERT (next_datum);

					std::string contained_datum_name = next_datum->getName ();
					object_file << "this->" << next_datum->getName () << "_.size () + ";
				}
			if (!inheritances.empty ())
				{
					for (std::set<std::string>::iterator base_class_iter = base_classes.begin (); base_class_iter != base_classes.end (); base_class_iter++)
						{
							std::string next_base_class (*base_class_iter);
							ASSERT (!next_base_class.empty ());

							object_file << next_base_class << "::size () + ";
						}
				}

			object_file << "0;\n"
						<< "        return this->size_;\n";
		}
	else
		{
			object_file << "    virtual double& size ()\n"
						<< "      {\n"
						<< "        return this->size_;\n";
		}

	object_file	<< "      }\n"
				<< "\n";

	// Generate the value method.
	if (!this->gettype ().empty ())
		{
			object_file << "    virtual " << this->gettype () << "& value ()\n"
						<< "      {\n"
						<< "        return this->value_;\n"
						<< "      }\n"
						<< "\n";
		}

	// Generate accessor methods for all fields.

	for (std::multiset<Datum>::iterator contained_types_iter = contained_types.begin (); contained_types_iter != contained_types.end (); contained_types_iter++)
		{
			Datum next_datum (*contained_types_iter);
			ASSERT (next_datum);

			std::string contained_datum_name = next_datum->getName ();
			object_file << "    virtual " << next_datum->getName () << "& get" << next_datum->getName () << " ()\n"
						<< "      {\n"
						<< "        return this->" << next_datum->getName () << "_;\n"
						<< "      }\n"
						<< "\n";
		}

	object_file	<< "  private:\n"
				<< "\n";

	// Generate any fields.

	for (std::multiset<Datum>::iterator contained_types_iter = contained_types.begin (); contained_types_iter != contained_types.end (); contained_types_iter++)
		{
			Datum next_datum (*contained_types_iter);
			ASSERT (next_datum);

			std::string contained_datum_name = next_datum->getName ();
			object_file << "    " << next_datum->getName () << " " << next_datum->getName () << "_;\n";
		}

	if (!this->gettype ().empty ())
		{
			object_file << "    " << this->gettype () << " value_;\n";
		}

	object_file << "    double size_;\n";

	object_file << "};\n"
				<< "\n"
				<< "#endif\n";

	object_file.close ();

}

}; // namespace XTEAM