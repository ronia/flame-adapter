#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "AssortedxADL_SimGen.h"

namespace XTEAM {

	extern std::string serialize_GUID(BON::Object &object);

	XTEAM_Assorted_Sim_Generator::XTEAM_Assorted_Sim_Generator(std::string version_dir, std::string simulation_code_dir) {
		// clears the messages
		energy_msg					= "";
		latency_msg					= "";
		latency_out_interface_count	= 0;
		memory_msg					= "";

		// sets the version path
		version_path				= version_dir;
		sim_code_path				= simulation_code_dir;
	}

	void XTEAM_Assorted_Sim_Generator::write_warning_messages () {
		// stores the energy consumption analysis messages
		std::string energy_path = version_path + "//analysis_messages_energy.list";
		std::ofstream energy_file (energy_path.c_str());
		energy_file << energy_msg;
		energy_file.close();

		// stores the memory usage analysis messages
		std::string memory_path = version_path + "//analysis_messages_memory.list";
		std::ofstream memory_file (memory_path.c_str());
		memory_file << memory_msg;
		memory_file.close();

		// stores the memory usage analysis messages
		std::string latency_path = version_path + "//analysis_messages_latency.list";
		std::ofstream latency_file (latency_path.c_str());
		if(latency_out_interface_count == 0) {
			latency_file << "No interface direction has been set as out" << std::endl;
		}
		latency_file.close();
	}

	void XTEAM_Assorted_Sim_Generator::initialize (Architecture arch) {
		

		// initializes
		std::set<Host> hosts = arch->getHost ();
		for (std::set<Host>::iterator host_iter = hosts.begin (); host_iter != hosts.end (); host_iter++) {
			Host next_host (*host_iter);
			ASSERT (next_host);
			std::set<Element> host_elements = next_host->getElements (0);
			for (std::set<Element>::iterator element_iter = host_elements.begin (); element_iter != host_elements.end (); element_iter++) {
				Element next_element (*element_iter);
				ASSERT (next_element);
				this->set_element_host (next_element, next_host);
			}
		}
	}

	void XTEAM_Assorted_Sim_Generator::generate (Architecture arch) {
		//////////////////////////////////////////////////////////////////
		// from power sim gen
		//////////////////////////////////////////////////////////////////

		// Generate the header for the energy logger class.
		this->generate_energy_logger_header (arch);

		// Generate the implementation for the energy logger class.
		this->generate_energy_logger_implementation (arch);

		//////////////////////////////////////////////////////////////////
		// from micro sim gen
		//////////////////////////////////////////////////////////////////

		// Generate the header for the memory logger class.
		this->generate_memory_logger_header (arch);

		// Generate the implementation for the memory logger class.
		this->generate_memory_logger_implementation (arch);

		//////////////////////////////////////////////////////////////////
		// from fast sim gen
		//////////////////////////////////////////////////////////////////

		// Generate the header for the latency logger class.
		this->generate_latency_logger_header ();

		// Generate the implementation for the latency logger class.
		this->generate_latency_logger_implementation (arch);
	}

	void XTEAM_Assorted_Sim_Generator::element_include (std::ofstream& file) {
		file << "#include \"EnergyLogger.h\"\n";
		file << "#include \"MemoryLogger.h\"\n";
		file << "#include \"LatencyLogger.h\"\n";
	}

	void XTEAM_Assorted_Sim_Generator::element_state (std::ofstream& file) {
		file << "    std::vector<double> thread_memory_;\n";
	}

	void XTEAM_Assorted_Sim_Generator::architecture_include (std::ofstream& file) {
		file << "#include \"EnergyLogger.h\"\n";
		file << "#include \"MemoryLogger.h\"\n";
		file << "#include \"LatencyLogger.h\"\n";
	}

	bool XTEAM_Assorted_Sim_Generator::hasElementInHostAssignments(Element &element) {
		// finds the element in the host assignments map
		std::map<XTEAM::Element, XTEAM::Host>::const_iterator it = this->host_assignments_.find(element);

		// checks if the element exists in the map
		bool found = it != host_assignments_.end();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Element has not been assigned to any Host
		///////////////////////////////////////////////////////////////
		if(!found) {
			std::stringstream ss;
			ss	<< element->getName() 
				<< " [" << serialize_GUID(element)
				<< "/" << element->getObjectMeta().name() << "]"
				<< " has not been assigned to any Host" << std::endl;
			energy_msg += ss.str();
		}

		return found;
	}

	void XTEAM_Assorted_Sim_Generator::on_component_input (	Component component, 
															Process process, 
															Interface intf, 
															Input input, 
															std::ofstream& file) {
		// from power sim gen
		std::string energy_cost = intf->getinvocationEnergyCost ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Interface has no invocation energy cost
		///////////////////////////////////////////////////////////////
		if (energy_cost.length() == 0)
		{
			std::stringstream ss;
			ss	<< intf->getName() 
				<< " [" << serialize_GUID(intf)
				<< "/" << intf->getObjectMeta().name() << "]"
				<< " in " << process->getName()
				<< " in " << component->getName()
				<< " has no invocation energy cost" << std::endl;
			energy_msg += ss.str();
		}

		// sets a temporary value if the energy cost is missing
		if(energy_cost.length() == 0) {
			energy_cost = "0";
		}

		file << "          double computational_energy_cost = " << energy_cost << ";\n";

		
		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Port has no source Element
		///////////////////////////////////////////////////////////////
		
		// finds the element in the port-destination_element map
		std::map<XTEAM::Port, XTEAM::Element>::const_iterator it = component->get_linked_elements().find(input);

		// checks if the element exists in the map
		bool found = it != component->get_linked_elements().end();

		// in case it is not found == there is a port that has no destination Element
		if(!found) 
		{
			std::stringstream ss;
			ss	<< input->getName() 
				<< " [" << serialize_GUID(input)
				<< "/" << input->getObjectMeta().name() << "]"
				<< " in " << intf->getName()
				<< " in " << component->getName()
				<< " has no link to any source" << std::endl;
			energy_msg += ss.str();

			file	<< "          double communication_energy_cost = 0;\n";
		}


		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Component has no assigned host
		///////////////////////////////////////////////////////////////
		else 
		{
			if(		hasElementInHostAssignments(component) && 
					hasElementInHostAssignments(component->get_linked_elements()[input])) 
			{
				if (this->host_assignments_[component] != this->host_assignments_[component->get_linked_elements ()[input]]) 
				{
					std::set<PortType> port_types = input->getPortType ();
					PortType port_type = *(port_types.begin ());
					ASSERT (port_type);

					Datum data_type (port_type->getDatum ());
					ASSERT (data_type);

					file	<< "          double communication_energy_cost = "
							<< input->getName () << "->size () * "
							<< this->host_assignments_[component]->getreceiveEnergyCost () << " + "
							<< this->host_assignments_[component]->getreceiveEnergyOverhead () << ";\n";
				} else {
					file	<< "          double communication_energy_cost = 0;\n";
				}
			} else {
				/* 
					If any one of the component (this one or the one connected to this one)
					does not exist in the host_assignments_ map, assuming the warning message
					has been written, just put a temporary value (0) and proceed.
				*/
				file	<< "          double communication_energy_cost = 0;\n";
			}
		}
		

		file	<< "          EnergyLogger::instance ()->record_energy_usage (this->element_id_, computational_energy_cost + communication_energy_cost, timeCurrent ());\n";

		// from fast sim gen
		if (intf->getdirection () == InterfaceImpl::out_direction_Type) {
			std::string interface_instance_id = (intf->getID ()).substr (8, 15);
			std::string interface_instance_name = intf->getName () + "_" + interface_instance_id;
			file << "          LatencyLogger::instance ()->record_response (\"" << interface_instance_name << "\", timeCurrent ());\n";
		}
	}

	void XTEAM_Assorted_Sim_Generator::on_connector_input (	Connector connector, 
															Process process, 
															Interface intf, 
															Input input, 
															std::ofstream& file) {
		// from power sim gen
		std::string energy_cost = intf->getinvocationEnergyCost ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Interface has no invocation energy cost
		///////////////////////////////////////////////////////////////
		if (energy_cost.length() == 0)
		{
			std::stringstream ss;
			ss	<< intf->getName() 
				<< " [" << serialize_GUID(intf)
				<< "/" << intf->getObjectMeta().name() << "]"
				<< " in " << process->getName()
				<< " in " << connector->getName()
				<< " has no invocation energy cost" << std::endl;
			energy_msg += ss.str();
		}

		// sets a temporary value if the energy cost is missing
		if(energy_cost.length() == 0)
		{
			energy_cost = "0";
		}

		file << "          double computational_energy_cost = " << energy_cost << ";\n";

		
		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Port has no source Element
		///////////////////////////////////////////////////////////////
		
		// finds the element in the port-destination_element map
		std::map<XTEAM::Port, XTEAM::Element>::const_iterator it = connector->get_linked_elements().find(input);

		// checks if the element exists in the map
		bool found = it != connector->get_linked_elements().end();

		// in case it is not found == there is a port that has no destination Element
		if(!found) 
		{
			std::stringstream ss;
			ss	<< input->getName() 
				<< " [" << serialize_GUID(input)
				<< "/" << input->getObjectMeta().name() << "]"
				<< " in " << intf->getName()
				<< " in " << connector->getName()
				<< " has no link to any source" << std::endl;
			energy_msg += ss.str();

			file	<< "          double communication_energy_cost = 0;\n";
		}


		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Connector has no assigned host
		///////////////////////////////////////////////////////////////
		else
		{
			if(		hasElementInHostAssignments(connector) && 
					hasElementInHostAssignments(connector->get_linked_elements()[input])) {
				if (this->host_assignments_[connector] != this->host_assignments_[connector->get_linked_elements ()[input]]) {
					std::set<PortType> port_types = input->getPortType ();
					PortType port_type = *(port_types.begin ());
					ASSERT (port_type);

					Datum data_type (port_type->getDatum ());
					ASSERT (data_type);

					file	<< "          double communication_energy_cost = "
							<< input->getName () << "->size () * "
							<< this->host_assignments_[connector]->getreceiveEnergyCost () << " + "
							<< this->host_assignments_[connector]->getreceiveEnergyOverhead () << ";\n";

				} else {
					file << "          double communication_energy_cost = 0;\n";
				}
			} else {
				/* 
					If any one of the connector (this one or the one connected to this one)
					does not exist in the host_assignments_ map, assuming the warning message
					has been written, just put a temporary value (0) and proceed.
				*/
				file	<< "          double communication_energy_cost = 0;\n";
			}
		}
		

		file << "              EnergyLogger::instance ()->record_energy_usage (this->element_id_, computational_energy_cost + communication_energy_cost, timeCurrent ());\n";

		// from fast sim gen
		if (intf->getdirection () == InterfaceImpl::out_direction_Type) {
			std::string interface_instance_id = (intf->getID ()).substr (8, 15);
			std::string interface_instance_name = intf->getName () + "_" + interface_instance_id;
			file	<< "          LatencyLogger::instance ()->record_response (\""
					<< interface_instance_name << "\", timeCurrent ());\n";
		}
	}

	void XTEAM_Assorted_Sim_Generator::on_component_output (	Component component, 
																Process process, 
																Interface intf, 
																Output output, 
																std::ofstream& file) 
	{
		
		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Port has no destination Element
		///////////////////////////////////////////////////////////////
		
		// finds the element in the port-destination_element map
		std::map<XTEAM::Port, XTEAM::Element>::const_iterator it = component->get_linked_elements().find(output);

		// checks if the element exists in the map
		bool found = it != component->get_linked_elements().end();

		// in case it is not found == there is a port that has no destination Element
		if(!found) 
		{
			std::stringstream ss;
			ss	<< output->getName() 
				<< " [" << serialize_GUID(output)
				<< "/" << output->getObjectMeta().name() << "]"
				<< " in " << intf->getName()
				<< " in " << component->getName()
				<< " has no link to any destination" << std::endl;
			energy_msg += ss.str();

			file	<< "          double communication_energy_cost = 0;\n";
		}

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Component has no assigned host
		///////////////////////////////////////////////////////////////
		else 
		{
			if(		hasElementInHostAssignments(component) && 
				hasElementInHostAssignments(component->get_linked_elements()[output])) {
				if (this->host_assignments_[component] != this->host_assignments_[component->get_linked_elements ()[output]]) {
					std::set<PortType> port_types = output->getPortType ();
					PortType port_type = *(port_types.begin ());
					ASSERT (port_type);

					Datum data_type (port_type->getDatum ());
					ASSERT (data_type);

					file	<< "      double communication_energy_cost = "
							<< "new_event->size () * "
							<< this->host_assignments_[component]->gettransmitEnergyCost () << " + "
							<< this->host_assignments_[component]->gettransmitEnergyOverhead () << ";\n";
				} else {
					file << "      double communication_energy_cost = 0;\n";
				}
			} else {
				/* 
					If any one of the component (this one or the one connected to this one)
					does not exist in the host_assignments_ map, assuming the warning message
					has been written, just put a temporary value (0) and proceed.
				*/
				file	<< "          double communication_energy_cost = 0;\n";
			}
		}
		
		

		file << "      EnergyLogger::instance ()->record_energy_usage (this->element_id_, communication_energy_cost, timeCurrent ());\n";

		// from fast sim gen
		if (intf->getdirection () == InterfaceImpl::out_direction_Type) {
			// increases the interface count
			latency_out_interface_count++;

			std::string interface_instance_id = (intf->getID ()).substr (8, 15);
			std::string interface_instance_name = intf->getName () + "_" + interface_instance_id;
			file << "      LatencyLogger::instance ()->record_request (\"" << interface_instance_name << "\", timeCurrent ());\n";
		}
	}

	void XTEAM_Assorted_Sim_Generator::on_connector_output (	Connector connector, 
																Process process, 
																Interface intf, 
																Output output, 
																std::ofstream& file) 
	{
		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Port has no destination Element
		///////////////////////////////////////////////////////////////
		
		// finds the element in the port-destination_element map
		std::map<XTEAM::Port, XTEAM::Element>::const_iterator it = connector->get_linked_elements().find(output);

		// checks if the element exists in the map
		bool found = it != connector->get_linked_elements().end();

		// in case it is not found == there is a port that has no destination Element
		if(!found) 
		{
			std::stringstream ss;
			ss	<< output->getName() 
				<< " [" << serialize_GUID(output)
				<< "/" << output->getObjectMeta().name() << "]"
				<< " in " << intf->getName()
				<< " in " << connector->getName()
				<< " has no link to any destination" << std::endl;
			energy_msg += ss.str();

			file	<< "          double communication_energy_cost = 0;\n";
		}

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: ENERGY CONSUMPTION ANALYSIS
		// Component has no assigned host
		///////////////////////////////////////////////////////////////
		else 
		{
			if(		hasElementInHostAssignments(connector) && 
					hasElementInHostAssignments(connector->get_linked_elements()[output])) {
				if (this->host_assignments_[connector] != this->host_assignments_[connector->get_linked_elements ()[output]]) {
					std::set<PortType> port_types = output->getPortType ();
					PortType port_type = *(port_types.begin ());
					ASSERT (port_type);

					Datum data_type (port_type->getDatum ());
					ASSERT (data_type);

					file	<< "              double communication_energy_cost = "
							<< "new_event->size () * "
							<< this->host_assignments_[connector]->gettransmitEnergyCost () << " + "
							<< this->host_assignments_[connector]->gettransmitEnergyOverhead () << ";\n";
				} else {
					file	<< "              double communication_energy_cost = 0;\n";
				}
			} else {
				/* 
					If any one of the connector (this one or the one connected to this one)
					does not exist in the host_assignments_ map, assuming the warning message
					has been written, just put a temporary value (0) and proceed.
				*/
				file	<< "          double communication_energy_cost = 0;\n";
			}
		}	

		file	<< "              EnergyLogger::instance ()->record_energy_usage (this->element_id_, communication_energy_cost, timeCurrent ());\n";

		// from fast sim gen
		if (intf->getdirection () == InterfaceImpl::out_direction_Type) {
			// increases the interface count
			latency_out_interface_count++;

			std::string interface_instance_id = (intf->getID ()).substr (8, 15);
			std::string interface_instance_name = intf->getName () + "_" + interface_instance_id;
			file	<< "      LatencyLogger::instance ()->record_request (\"" << interface_instance_name << "\", timeCurrent ());\n";
		}
	}


	void XTEAM_Assorted_Sim_Generator::set_element_host (Element element, Host host) {
		std::set<Architecture> sub_architectures = element->getArchitecture ();
		for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++) {

			Architecture next_architecture (*architecture_iter);
			ASSERT (next_architecture);
			std::set<Element> sub_elements = next_architecture->getElement ();

			for (std::set<Element>::iterator element_iter = sub_elements.begin (); element_iter != sub_elements.end (); element_iter++) {
				Element next_element (*element_iter);
				ASSERT (next_element);
				this->set_element_host (next_element, host);
			}
		}

		this->host_assignments_[element] = host;
	}

	void XTEAM_Assorted_Sim_Generator::on_sim_end (Architecture arch, std::ofstream& file) {
		//////////////////////////////////////////////////////////////////
		// from micro sim gen
		//////////////////////////////////////////////////////////////////
		file << "  MemoryLogger::instance ()->persist_memory_usage ();\n";

		//////////////////////////////////////////////////////////////////
		// from fast sim gen
		//////////////////////////////////////////////////////////////////
		file << "  LatencyLogger::instance ()->persist_response_latencies ();\n";
	}

	void XTEAM_Assorted_Sim_Generator::on_component_task_begin (	Component component,
																	Process process,
																	Task task,
																	std::ofstream& file) {

		std::string consume_memory = task->getmemoryUsage ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: MEMORY USAGE ANALYSIS
		// Task has no memory usage attributed
		///////////////////////////////////////////////////////////////
		if(consume_memory.length() == 0)
		{
			std::stringstream ss;
			ss	<< task->getName()
				<< " [" << serialize_GUID(task)
				<< "/" << task->getObjectMeta().name() << "]"
				<< " in " << process->getName()
				<< " in " << component->getName()
				<< " is missing the memory usage value" << std::endl;
			memory_msg += ss.str();
		}
		
		// if the consume memory value is empty, just put 0 as the warning message would have been already added
		if(consume_memory.length() == 0)
		{
			consume_memory = "0";
		}

		file	<< "          double consumed_memory = " << consume_memory << ";\n"
				<< "          if (thread_id >= thread_memory_.size ())\n"
				<< "            {\n"
				<< "              this->thread_memory_.push_back (consumed_memory);\n"
				<< "            }\n"
				<< "          else\n"
				<< "            {\n"
				<< "              this->thread_memory_[thread_id] = consumed_memory;\n"
				<< "            }\n";

		file	<< "          MemoryLogger::instance ()->record_memory_usage (this->element_id_, 0, timeCurrent ());\n"
				<< "          MemoryLogger::instance ()->record_memory_usage (this->element_id_, consumed_memory, timeCurrent ());\n";
	}

	void XTEAM_Assorted_Sim_Generator::on_component_task_end (	Component component,
																Process process,
																Task task,
																std::ofstream& file) {
		file	<< "      double released_memory = this->thread_memory_[thread_id];\n";

		file	<< "      MemoryLogger::instance ()->record_memory_usage (this->element_id_, 0, timeCurrent ());\n"
				<< "      MemoryLogger::instance ()->record_memory_usage (this->element_id_, -released_memory, timeCurrent ());\n";
	}

	void XTEAM_Assorted_Sim_Generator::on_connector_task_begin (	Connector connector,
																	Process process,
																	Task task,
																	std::ofstream& file) {
		std::string consume_memory = task->getmemoryUsage ();

		///////////////////////////////////////////////////////////////
		// CoDesign Modification: MEMORY USAGE ANALYSIS
		// Task has no memory usage attributed
		///////////////////////////////////////////////////////////////
		if(consume_memory.length() == 0)
		{
			std::stringstream ss;
			ss	<< task->getName()
				<< " [" << serialize_GUID(task)
				<< "/" << task->getObjectMeta().name() << "]"
				<< " in " << process->getName()
				<< " in " << connector->getName()
				<< " is missing the memory usage value" << std::endl;
			memory_msg += ss.str();
		}

		// if the consume memory value is empty, just put 0 as the warning message would have been already added
		if(consume_memory.length() == 0)
		{
			consume_memory = "0";
		}

		file	<< "          double consumed_memory = " << consume_memory << ";\n"
				<< "          if (thread_id >= thread_memory_.size ())\n"
				<< "            {\n"
				<< "              this->thread_memory_.push_back (consumed_memory);\n"
				<< "            }\n"
				<< "          else\n"
				<< "            {\n"
				<< "              this->thread_memory_[thread_id] = consumed_memory;\n"
				<< "            }\n";

		file	<< "          MemoryLogger::instance ()->record_memory_usage (this->element_id_, 0, timeCurrent ());\n"
				<< "          MemoryLogger::instance ()->record_memory_usage (this->element_id_, consumed_memory, timeCurrent ());\n";
	}

	void XTEAM_Assorted_Sim_Generator::on_connector_task_end (	Connector connector,
																Process process,
																Task task,
																std::ofstream& file)
	{
		file	<< "      double released_memory = this->thread_memory_[thread_id];\n";

		file	<< "      MemoryLogger::instance ()->record_memory_usage (this->element_id_, 0, timeCurrent ());\n"
				<< "      MemoryLogger::instance ()->record_memory_usage (this->element_id_, -released_memory, timeCurrent ());\n";
	}





	/*
	Generates the header file for the energy logger classes.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_energy_logger_header (Architecture arch) {
		// Open the energy logger header file stream.
		std::string energy_logger_file_name = sim_code_path + "EnergyLogger.h";
		std::ofstream energy_logger_file (energy_logger_file_name.c_str ());

		// Write the compilation directives and and begin the energy logger
		// class declaration.
		energy_logger_file	<< "#ifndef ENERGYLOGGER_H\n"
							<< "#define ENERGYLOGGER_H\n"
							<< "\n"
							////////////////////////////////////////////////////////////
							//
							// pre-compilation 
							//
							////////////////////////////////////////////////////////////
							//<< "#include <iostream>\n"
							//<< "#include <fstream>\n"
							//<< "#include <map>\n"
							//<< "#include <string>\n"
							<< "\n"
							<< "class ElementLog_Energy\n"
							<< "{\n"
							<< "public:\n"
							<< "  void set_name (std::string name);\n"
							<< "  std::string get_name ();\n"
							<< "  void record_energy_usage (double joules, double time);\n"
							<< "\n"
							<< "private:\n"
							<< "  std::string name_;\n"
							<< "  std::ofstream energy_log_file_;\n"
							<< "};\n"
							<< "\n"
							<< "class EnergyLogger\n"
							<< "{\n"
							<< "public:\n"
							<< "  static EnergyLogger * instance ();\n"
							<< "  void record_energy_usage (std::string element_id, double joules, double time);\n"
							<< "\n"
							<< "protected:\n"
							<< "  EnergyLogger ();\n"
							<< "\n"
							<< "private:\n"
							<< "  static EnergyLogger * instance_;\n"
							<< "  std::map<std::string, ElementLog_Energy *> log_assignments_;\n";

		// Close the energy logger class declaration.
		energy_logger_file	<< "};\n"
							<< "\n"
							<< "#endif\n";

		// Close the energy logger header file.
		energy_logger_file.close ();
	}

	/*
	Generate the implementation file for the energy logger classes.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_energy_logger_implementation (Architecture arch) {
		// Open the energy logger implementation file stream.
		std::string energy_logger_file_name = sim_code_path + "EnergyLogger.cpp";
		std::ofstream energy_logger_file (energy_logger_file_name.c_str ());

		////////////////////////////////////////////////////////////
		//
		// pre-compilation 
		//
		////////////////////////////////////////////////////////////
		// Include the energy logger header and write the class implementation.
		energy_logger_file	<< "#include \"stdafx.h\"\n"
							<< "#include \"EnergyLogger.h\"\n"
							<< "\n"
							<< "void ElementLog_Energy::set_name (std::string name)\n"
							<< "{\n"
							<< "  this->name_ = name;\n"
							<< "  std::string energy_log_file_name = this->name_ + \"_Energy_Log.csv\";\n"
							<< "  this->energy_log_file_.open (energy_log_file_name.c_str ());\n"
							<< "  this->energy_log_file_ << \"TIME,ENERGY CONSUMED (J)\\n\";\n"
							<< "}\n"
							<< "\n"
							<< "std::string ElementLog_Energy::get_name ()\n"
							<< "{\n"
							<< "  return this->name_;\n"
							<< "}\n"
							<< "\n"
							<< "void ElementLog_Energy::record_energy_usage (double joules, double time)\n"
							<< "{\n"
							<< "  this->energy_log_file_ << time << \",\" << joules << \"\\n\";\n"
							<< "  this->energy_log_file_.flush();\n"
							<< "}\n"
							<< "\n"
							<< "EnergyLogger * EnergyLogger::instance_ = 0;\n"
							<< "\n"
							<< "EnergyLogger::EnergyLogger ()\n"
							<< "{\n";

		// For each element, insert the element name and corresponding log into the log
		// assignments map. This will be used to determine to which log the energy data will
		// be written.
		std::set<Element> elements = arch->getElement ();
		for (std::set<Element>::iterator element_iter = elements.begin (); element_iter != elements.end (); element_iter++) {
			Element next_element (*element_iter);
			ASSERT (next_element);
			this->write_energy_log_assignment (next_element, energy_logger_file);
		}

		// Write the instance () and record_energy_usage () methods of the energy logger class.
		energy_logger_file	<< "}\n"
							<< "\n"
							<< "EnergyLogger * EnergyLogger::instance ()\n"
							<< "{\n"
							<< "  if (instance_ == 0)\n"
							<< "    {\n"
							<< "      instance_ = new EnergyLogger ();\n"
							<< "    }\n"
							<< "  return instance_;\n"
							<< "}\n"
							<< "\n"
							<< "void EnergyLogger::record_energy_usage (std::string element_id, double joules, double time)\n"
							<< "{\n"
							<< "  this->log_assignments_[element_id]->record_energy_usage (joules, time);\n"
							<< "}\n";

		// Close the energy logger implementation file.
		energy_logger_file.close ();
	}

	void XTEAM_Assorted_Sim_Generator::write_energy_log_assignment (Element element, std::ofstream& energy_logger_file) {
		std::set<Architecture> sub_architectures = element->getArchitecture ();
		if (sub_architectures.size () > 0) {
			for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++) {
				Architecture next_architecture (*architecture_iter);
				ASSERT (next_architecture);
				std::set<Element> sub_elements = next_architecture->getElement ();
				for (std::set<Element>::iterator element_iter = sub_elements.begin (); element_iter != sub_elements.end (); element_iter++) {
					Element next_element (*element_iter);
					ASSERT (next_element);
					this->write_energy_log_assignment (next_element, energy_logger_file);
				}
			}
		} else {
			energy_logger_file	<< "  ElementLog_Energy * " << element->get_element_name () << "_energy_log = new ElementLog_Energy;\n"
								<< "  " << element->get_element_name () << "_energy_log->set_name (\"" << element->get_element_name () << "\");\n"
								<< "  this->log_assignments_[\"" << element->get_element_name () << "\"] = " << element->get_element_name () << "_energy_log;\n";
		}
	}

	void XTEAM_Assorted_Sim_Generator::write_memory_log_assignment (Element element, std::ofstream& memory_logger_file) {
		std::set<Architecture> sub_architectures = element->getArchitecture ();
		if (sub_architectures.size () > 0) {
			for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++) {
				Architecture next_architecture (*architecture_iter);
				ASSERT (next_architecture);
				std::set<Element> sub_elements = next_architecture->getElement ();
				for (std::set<Element>::iterator element_iter = sub_elements.begin (); element_iter != sub_elements.end (); element_iter++) {
					Element next_element (*element_iter);
					ASSERT (next_element);
					this->write_memory_log_assignment (next_element, memory_logger_file);
				}
			}
		} else {
			memory_logger_file	<< "  ElementLog * " << element->get_element_name () << "_memory_log = new ElementLog;\n"
								<< "  " << element->get_element_name () << "_memory_log->set_name (\"" << element->get_element_name () << "\");\n"
								<< "  this->log_assignments_[\"" << element->get_element_name () << "\"] = " << element->get_element_name () << "_memory_log;\n";
		}
	}

	void XTEAM_Assorted_Sim_Generator::write_latency_log_assignment (Element element, std::ofstream& latency_logger_file) {
		std::set<Architecture> sub_architectures = element->getArchitecture ();
		if (sub_architectures.size () > 0) {
			for (std::set<Architecture>::iterator architecture_iter = sub_architectures.begin (); architecture_iter != sub_architectures.end (); architecture_iter++) {
				Architecture next_architecture (*architecture_iter);
				ASSERT (next_architecture);
				std::set<Element> sub_elements = next_architecture->getElement ();
				for (std::set<Element>::iterator element_iter = sub_elements.begin (); element_iter != sub_elements.end (); element_iter++) {
					Element next_element (*element_iter);
					ASSERT (next_element);
					this->write_latency_log_assignment (next_element, latency_logger_file);
				}
			}
		} else {
			std::set<Interface> interfaces = element->getInterface ();
			for (std::set<Interface>::iterator interface_iter = interfaces.begin (); interface_iter != interfaces.end (); interface_iter++) {
				Interface next_interface (*interface_iter);
				ASSERT (next_interface);

				if (next_interface->getdirection () == InterfaceImpl::out_direction_Type || next_interface->getdirection () == InterfaceImpl::inout_direction_Type) {
					std::string interface_instance_id = (next_interface->getID ()).substr (8, 15);
					std::string interface_instance_name = next_interface->getName () + "_" + interface_instance_id;
					latency_logger_file << "  InterfaceLog * " << interface_instance_name << "_latency_log = new InterfaceLog;\n"
										<< "  " << interface_instance_name << "_latency_log->set_name (\"" << interface_instance_name << "\");\n"
										<< "  this->log_assignments_[\"" << interface_instance_name << "\"] = " << interface_instance_name << "_latency_log;\n";
				}
			}
		}
	}


	/*
	Generates the header file for the memory logger class.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_memory_logger_header (Architecture arch) {
		// Open the memory logger header file stream.
		std::string memory_logger_file_name =  sim_code_path + "MemoryLogger.h";
		std::ofstream memory_logger_file (memory_logger_file_name.c_str ());

		// Write the compilation directives and begin the memory logger
		// class declaration.
		memory_logger_file	<< "#ifndef MEMORYLOGGER_H\n"
							<< "#define MEMORYLOGGER_H\n"
							<< "\n"
							////////////////////////////////////////////////////////////
							//
							// pre-compilation 
							//
							////////////////////////////////////////////////////////////
							//<< "#include <iostream>\n"
							//<< "#include <fstream>\n"
							//<< "#include <map>\n"
							//<< "#include <string>\n"
							<< "\n"
							<< "class ElementLog\n"
							<< "{\n"
							<< "public:\n"
							<< "  void set_name (std::string name);\n"
							<< "  std::string get_name ();\n"
							<< "  void record_memory_usage (double kilobytes, double time);\n"
							<< "\n"
							<< "private:\n"
							<< "  std::string name_;\n"
							<< "  double used_memory_;\n"
							<< "  std::ofstream memory_log_file_;\n"
							<< "};\n"
							<< "\n"
							<< "class MemoryLogger\n"
							<< "{\n"
							<< "public:\n"
							<< "  static MemoryLogger * instance ();\n"
							<< "  void record_memory_usage (std::string element_id, double kilobytes, double time);\n"
							<< "  void persist_memory_usage ();\n"
							<< "\n"
							<< "protected:\n"
							<< "  MemoryLogger ();\n"
							<< "\n"
							<< "private:\n"
							<< "  static MemoryLogger * instance_;\n"
							<< "  std::map<std::string, ElementLog *> log_assignments_;\n";

		// Close the memory logger class declaration.
		memory_logger_file	<< "};\n"
							<< "\n"
							<< "#endif\n";

		// Close the memory logger header file.
		memory_logger_file.close ();
	}

	/*
	Generate the implementation file for the memory logger and host classes.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_memory_logger_implementation (Architecture arch) {
		// Open the memory logger implementation file stream.
		std::string memory_logger_file_name =  sim_code_path + "MemoryLogger.cpp";
		std::ofstream memory_logger_file (memory_logger_file_name.c_str ());

		// Include the memory logger header and write the host class implementation.
		////////////////////////////////////////////////////////////
		//
		// pre-compilation 
		//
		////////////////////////////////////////////////////////////
		memory_logger_file	<< "#include \"stdafx.h\"\n"
							<< "#include \"MemoryLogger.h\"\n"
							<< "\n"
							<< "void ElementLog::set_name (std::string name)\n"
							<< "{\n"
							<< "  this->name_ = name;\n"
							<< "  this->used_memory_ = 0;\n"
							<< "  std::string memory_log_file_name = this->name_ + \"_Memory_Log.csv\";\n"
							<< "  this->memory_log_file_.open (memory_log_file_name.c_str ());\n"
							<< "  this->memory_log_file_ << \"TIME,MEMORY IN USE\\n\";\n"
							<< "}\n"
							<< "\n"
							<< "std::string ElementLog::get_name ()\n"
							<< "{\n"
							<< "  return this->name_;\n"
							<< "}\n"
							<< "\n"
							<< "void ElementLog::record_memory_usage (double kilobytes, double time)\n"
							<< "{\n"
							<< "  this->used_memory_ += kilobytes;\n"
							<< "  this->memory_log_file_ << time << \",\" << this->used_memory_ << \"\\n\";\n"
							<< "  this->memory_log_file_.flush();\n"
							<< "}\n"
							<< "\n"
							<< "MemoryLogger * MemoryLogger::instance_ = 0;\n"
							<< "\n"
							<< "MemoryLogger::MemoryLogger ()\n"
							<< "{\n";

		// For each element, insert the element name and corresponding log into the log
		// assignments map. This will be used to determine to which log the memory data will
		// be written.
		std::set<Element> elements = arch->getElement ();
		for (std::set<Element>::iterator element_iter = elements.begin (); element_iter != elements.end (); element_iter++) {
			Element next_element (*element_iter);
			ASSERT (next_element);
			this->write_memory_log_assignment (next_element, memory_logger_file);
		}

		// Write the instance () and record_memory_usage () methods of the memory logger class.
		memory_logger_file	<< "}\n"
							<< "\n"
							<< "MemoryLogger * MemoryLogger::instance ()\n"
							<< "{\n"
							<< "  if (instance_ == 0)\n"
							<< "    {\n"
							<< "      instance_ = new MemoryLogger ();\n"
							<< "    }\n"
							<< "  return instance_;\n"
							<< "}\n"
							<< "\n"
							<< "void MemoryLogger::record_memory_usage (std::string element_id, double kilobytes, double time)\n"
							<< "{\n"
							<< "  this->log_assignments_[element_id]->record_memory_usage (kilobytes, time);\n"
							<< "}\n"
							<< "\n"
							<< "void MemoryLogger::persist_memory_usage ()\n"
							<< "{\n"
							<< "}\n";

		// Close the memory logger implementation file.
		memory_logger_file.close ();
	}

	/*
	Generates the header file for the latency logger class.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_latency_logger_header () {
		// Open the latency logger header file stream.
		std::string latency_logger_file_name =  sim_code_path + "LatencyLogger.h";
		std::ofstream latency_logger_file (latency_logger_file_name.c_str ());

		// Write the compilation directives and the interface log class declaration, and begin the latency logger
		// class declaration.
		latency_logger_file << "#ifndef LATENCYLOGGER_H\n"
							<< "#define LATENCYLOGGER_H\n"
							<< "\n"
							////////////////////////////////////////////////////////////
							//
							// pre-compilation 
							//
							////////////////////////////////////////////////////////////
							//<< "#include <iostream>\n"
							//<< "#include <fstream>\n"
							//<< "#include <map>\n"
							//<< "#include <string>\n"
							//<< "#include <list>\n"
							<< "\n"
							<< "class InterfaceLog\n"
							<< "{\n"
							<< "public:\n"
							<< "  void set_name (std::string name);\n"
							<< "  std::string get_name ();\n"
							<< "  void record_request (double time);\n"
 							<< "  void record_response (double time);\n"
							<< "  void persist_response_latencies ();\n"
							<< "\n"
							<< "private:\n"
							<< "  std::string name_;\n"
							<< "  std::ofstream latency_log_file_;\n"
							<< "  std::list<double> request_times_;\n"
							<< "  std::list<double> response_times_;\n"
							<< "};\n"
							<< "\n"
							<< "class LatencyLogger\n"
							<< "{\n"
							<< "public:\n"
							<< "  static LatencyLogger * instance ();\n"
							<< "  void record_request (std::string interface_id, double time);\n"
 							<< "  void record_response (std::string interface_id, double time);\n"
							<< "  void persist_response_latencies ();\n"
							<< "\n"
							<< "protected:\n"
							<< "  LatencyLogger ();\n"
							<< "\n"
							<< "private:\n"
							<< "  static LatencyLogger * instance_;\n"
							<< "  std::map<std::string, InterfaceLog *> log_assignments_;\n";

		// Close the latency logger class declaration.
		latency_logger_file << "};\n"
							<< "\n"
							<< "#endif\n";

		// Close the latency logger header file.
		latency_logger_file.close ();
	}

	/*
	Generates the implementation file for the latency logger class.
	*/
	void XTEAM_Assorted_Sim_Generator::generate_latency_logger_implementation (Architecture arch) {
		// Open the latency logger implementation file stream.
		std::string latency_logger_file_name =  sim_code_path + "LatencyLogger.cpp";
		std::ofstream latency_logger_file (latency_logger_file_name.c_str ());

		////////////////////////////////////////////////////////////
		//
		// pre-compilation 
		//
		////////////////////////////////////////////////////////////
		// Include the latency logger header and write the interface log class implementation.
		latency_logger_file << "#include \"stdafx.h\"\n"
							<< "#include \"LatencyLogger.h\"\n"
							<< "\n"
							<< "void InterfaceLog::set_name (std::string name)\n"
							<< "{\n"
							<< "  this->name_ = name;\n"
							<< "  std::string latency_log_file_name = this->name_ + \"_Latency_Log.csv\";\n"
							<< "  this->latency_log_file_.open (latency_log_file_name.c_str ());\n"
							<< "}\n"
							<< "\n"
							<< "std::string InterfaceLog::get_name ()\n"
							<< "{\n"
							<< "  return this->name_;\n"
							<< "}\n"
							<< "\n"
							<< "void InterfaceLog::record_request (double time)\n"
							<< "{\n"
							<< "  this->request_times_.push_back (time);\n"
							<< "}\n"
 							<< "\n"
							<< "void InterfaceLog::record_response (double time)\n"
							<< "{\n"
							<< "  this->response_times_.push_back (time);\n"
							<< "}\n"
							<< "\n"
							<< "void InterfaceLog::persist_response_latencies ()\n"
							<< "{\n"
							<< "  this->latency_log_file_ << \"REQUEST TIME,RESPONSE TIME,ROUND-TRIP LATENCY\\n\";\n"
							<< "  while (!this->request_times_.empty ())\n"
							<< "  {\n"
							<< "    this->latency_log_file_ << this->request_times_.front () << \",\";\n"
							<< "    if (!this->response_times_.empty ())\n"
							<< "      {\n"
							<< "        this->latency_log_file_ << this->response_times_.front () << \",\" << this->response_times_.front () - this->request_times_.front () << \"\\n\";\n"
							<< "      }\n"
							<< "    else\n"
							<< "      {\n"
							<< "        this->latency_log_file_ << \"-,-\\n\";\n"
							<< "      }\n"
							<< "    this->request_times_.pop_front ();\n"
							<< "	if(!this->response_times_.empty())\n"
							<< "		this->response_times_.pop_front ();\n"
							<< "  }\n"
							<< "  \n"
							<< "  this->latency_log_file_.flush();\n"
							<< "}\n"
							<< "\n"
							<< "LatencyLogger * LatencyLogger::instance_ = 0;\n"
							<< "\n"
							<< "LatencyLogger::LatencyLogger ()\n"
							<< "{\n";

		// For each element, insert all interface names and corresponding log into the log
		// assignments map. This will be used to determine to which log the latency data will
		// be written.
		std::set<Element> elements = arch->getElement ();
		for (std::set<Element>::iterator element_iter = elements.begin (); element_iter != elements.end (); element_iter++) {
			Element next_element (*element_iter);
			ASSERT (next_element);
			this->write_latency_log_assignment (next_element, latency_logger_file);
		}

		// Write the instance (), record_request (), and record_response () methods of the latency logger class.
		latency_logger_file << "}\n"
							<< "\n"
							<< "LatencyLogger * LatencyLogger::instance ()\n"
							<< "{\n"
							<< "  if (instance_ == 0)\n"
							<< "    {\n"
							<< "      instance_ = new LatencyLogger ();\n"
							<< "    }\n"
							<< "  return instance_;\n"
							<< "}\n"
							<< "\n"
							<< "void LatencyLogger::record_request (std::string interface_id, double time)\n"
							<< "{\n"
							<< "  this->log_assignments_[interface_id]->record_request (time);\n"
							<< "}\n"
							<< "\n"
							<< "void LatencyLogger::record_response (std::string interface_id, double time)\n"
							<< "{\n"
							<< "  this->log_assignments_[interface_id]->record_response (time);\n"
							<< "}\n"
							<< "void LatencyLogger::persist_response_latencies ()\n"
							<< "{\n"
							<< "  for (std::map<std::string, InterfaceLog *>::iterator interface_log_iter = this->log_assignments_.begin (); interface_log_iter != this->log_assignments_.end (); interface_log_iter++)\n"
							<< "    {\n"
							<< "      interface_log_iter->second->persist_response_latencies ();\n"
							<< "    }\n"
							<< "}\n";

		// Close the latency logger implementation file.
		latency_logger_file.close ();
	}

}; // namespace XTEAM
