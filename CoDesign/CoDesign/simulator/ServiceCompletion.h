#ifndef SERVICECOMPLETION_H
#define SERVICECOMPLETION_H

#include <string>
#include "adevs.h"

using namespace std;

class ServiceCompletion: public object
{
  public:

    ServiceCompletion ():
      object ()
    {
    }

    ServiceCompletion (std::string element_id, int thread_id, int worker_number):
	  object (),
	  element_id_ (element_id),
	  thread_id_ (thread_id),
	  worker_number_ (worker_number)
    {
    }

	/// Copy constructor.
	ServiceCompletion (const ServiceCompletion& src):
	  object (),
	  element_id_ (src.element_id_),
	  thread_id_ (src.thread_id_),
	  worker_number_ (src.worker_number_)
	{
	}

	/// Assignment operator.
	const ServiceCompletion& operator= (const ServiceCompletion& src)
	{
		element_id_ = src.element_id_;
		thread_id_ = src.thread_id_;
		worker_number_ = src.worker_number_;
		return *this;
	}

    object * clone () const
    {
        return new ServiceCompletion (*this);
    }

	std::string element_id_;
	int thread_id_;
	int worker_number_;

};

#endif
