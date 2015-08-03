#ifndef XTEAM_SIMGEN_H
#define XTEAM_SIMGEN_H

#include "SimGenInclude.h"

namespace XTEAM
{

class XTEAM_Sim_Generator
{

public:

	virtual void initialize (Architecture arch);

	virtual void generate (Architecture arch);

	virtual void architecture_include (std::ofstream& file);

	virtual void element_include (std::ofstream& file);

	virtual void element_state (std::ofstream& file);

	virtual void on_sim_start (Architecture arch,
							   std::ofstream& file);

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

	virtual void on_component_task_begin (Component component,
										  Process process,
										  Task task,
										  std::ofstream& file);

	virtual void on_component_task_end (Component component,
										Process process,
										Task task,
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

	virtual void on_connector_task_begin (Connector connector,
										  Process process,
										  Task task,
										  std::ofstream& file);

	virtual void on_connector_task_end (Connector connector,
										Process process,
										Task task,
										std::ofstream& file);

};

}; // namespace XTEAM


#endif // XTEAM_SIMGEN_H