#include "stdafx.h"
#include "XTEAM_SimGen.h"

namespace XTEAM
{

void XTEAM_Sim_Generator::initialize (Architecture arch)
{
}

void XTEAM_Sim_Generator::generate (Architecture arch)
{
}

void XTEAM_Sim_Generator::architecture_include (std::ofstream& file)
{
}

void XTEAM_Sim_Generator::element_include (std::ofstream& file)
{
}

void XTEAM_Sim_Generator::element_state (std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_sim_start (Architecture arch, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_sim_end (Architecture arch, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_component_input (Component component, Process process, Interface intf, Input input, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_component_output (Component component, Process process, Interface intf, Output output, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_component_task_begin (Component component, Process process, Task task, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_component_task_end (Component component, Process process, Task task, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_connector_input (Connector connector, Process process, Interface intf, Input input, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_connector_output (Connector connector, Process process, Interface intf, Output output, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_connector_task_begin (Connector connector, Process process, Task task, std::ofstream& file)
{
}

void XTEAM_Sim_Generator::on_connector_task_end (Connector connector, Process process, Task task, std::ofstream& file)
{
}

}; // namespace XTEAM
