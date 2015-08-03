#ifndef SERVICEDEMAND_H
#define SERVICEDEMAND_H

#include <string>
#include "adevs.h"

using namespace std;

class ServiceDemand: public object
{
  public:

    ServiceDemand ():
      object ()
    {
    }

    ServiceDemand (std::string element_id, int thread_id, double service_demand):
      object (),
	  element_id_ (element_id),
	  thread_id_ (thread_id),
	  service_demand_ (service_demand)
    {
    }

	/// Copy constructor.
	ServiceDemand (const ServiceDemand& src):
	  object (),
	  element_id_ (src.element_id_),
	  thread_id_ (src.thread_id_),
	  service_demand_ (src.service_demand_)
	{
	}

	/// Assignment operator.
	const ServiceDemand& operator= (const ServiceDemand& src)
	{
		element_id_ = src.element_id_;
		thread_id_ = src.thread_id_;
		service_demand_ = src.service_demand_;
		return *this;
	}

    object * clone () const
      {
        return new ServiceDemand (*this);
      }

	std::string element_id_;
	int thread_id_;
	double service_demand_;

};

#endif
