#ifndef ASSORTEDXADL_SIMGEN_H
#define ASSORTEDXADL_SIMGEN_H

#include "../iframe/XTEAM_SimGen.h"

namespace XTEAM
{

class XTEAM_Assorted_Sim_Generator : public XTEAM_Sim_Generator
{

public:

	XTEAM_Assorted_Sim_Generator(std::string version_dir, std::string simulation_code_dir);

	void write_warning_messages();

	//////////////////////////////////////////////////////////////////
	// from power sim gen
	//////////////////////////////////////////////////////////////////

	virtual void initialize (Architecture arch);

	virtual void generate (Architecture arch);

	virtual void architecture_include (std::ofstream& file);

	virtual void element_include (std::ofstream& file);

	virtual void on_sim_end (Architecture arch,
							 std::ofstream& file);

	virtual void on_component_input (Component component,
									Process process,
									Interface intf,
									Input input,
									std::ofstream& file);

	virtual void on_component_output (Component component,
									  Process process,
									  Interface intf,
									  Output output,
									  std::ofstream& file);

	virtual void on_connector_input (Connector connector,
									 Process process,
									 Interface intf,
									 Input input,
									 std::ofstream& file);

	virtual void on_connector_output (Connector connector,
									  Process process,
									  Interface intf,
									  Output output,
									  std::ofstream& file);

	//////////////////////////////////////////////////////////////////
	// from micro sim gen
	//////////////////////////////////////////////////////////////////

	virtual void element_state (std::ofstream& file);


	virtual void on_component_task_begin (Component component,
										  Process process,
										  Task task,
										  std::ofstream& file);

	virtual void on_component_task_end (Component component,
										Process process,
										Task task,
										std::ofstream& file);

	virtual void on_connector_task_begin (Connector connector,
										  Process process,
										  Task task,
										  std::ofstream& file);

	virtual void on_connector_task_end (Connector connector,
										Process process,
										Task task,
										std::ofstream& file);

private:

	// missing attributes for power analysis message
	std::string			energy_msg;

	// missing attributes for latency analysis message
	std::string			latency_msg;
	int					latency_out_interface_count;

	// missing attributes for memory analysis message
	std::string			memory_msg;

	// version path where the warning messages are written
	std::string version_path;

	// simulation code path where the logger code is written
	std::string sim_code_path;


	//////////////////////////////////////////////////////////////////
	// from power sim gen
	//////////////////////////////////////////////////////////////////

	void set_element_host (Element element, Host host);

	void generate_energy_logger_header (Architecture arch);

	void generate_energy_logger_implementation (Architecture arch);

	void write_energy_log_assignment (Element element, std::ofstream& energy_logger_file);

	std::map<Element, Host> host_assignments_;

	bool hasElementInHostAssignments(Element &element);

	//////////////////////////////////////////////////////////////////
	// from micro sim gen
	//////////////////////////////////////////////////////////////////

	void generate_memory_logger_header (Architecture arch);

	void generate_memory_logger_implementation (Architecture arch);

	void write_memory_log_assignment (Element element, std::ofstream& memory_logger_file);

	//////////////////////////////////////////////////////////////////
	// from fast sim gen
	//////////////////////////////////////////////////////////////////

	void generate_latency_logger_header ();

	void generate_latency_logger_implementation (Architecture arch);

	void write_latency_log_assignment (Element element, std::ofstream& latency_logger_file);

};

}; // namespace XTEAM


#endif // ASSORTEDXADL_SIMGEN_H