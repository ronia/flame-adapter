#ifndef XTEAM_IFRAME_H
#define XTEAM_IFRAME_H

#include "XTEAM_SimGen.h"

#define SIMCODEPATH "\\XTEAM_Simulation\\simulation_code\\"

namespace XTEAM
{

class XTEAM_IFrame
{

public:

	static void generate (BON::Project& project, BON::FCO& currentFCO, std::string version, int time);

	static void sim_gen (XTEAM_Sim_Generator * sim_gen);
	
	static XTEAM_Sim_Generator * sim_gen ();



private:

	XTEAM_IFrame ();

	static XTEAM_Sim_Generator * sim_gen_;

};


}; // namespace XTEAM

#endif // XTEAM_IFRAME_H