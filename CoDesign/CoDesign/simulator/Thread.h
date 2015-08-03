#ifndef THREAD_H
#define THREAD_H

#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include "adevs.h"

class Thread: public atomic 
{
	public:
		/// Constructor.
		Thread (std::string worker_name, int worker_number, std::ofstream& event_log);
		/// State initialization function.
		void init ();
		/// Internal transition function.
		void delta_int ();
		/// External transition function.
		void delta_ext (stime_t e, const adevs_bag<PortValue>& x);
		/// Confluent transition function.
		void delta_conf (const adevs_bag<PortValue>& x);
		/// Output function.  
		void output_func (adevs_bag<PortValue>& y);
		/// Output value garbage collection.
		void gc_output (adevs_bag<PortValue>& g);
		/// Destructor.
		~Thread ();

		static const port_t ServiceDemandInput;
		static const port_t ServiceCompletionOutput;

	private:
		std::string worker_name_;
		int worker_number_;
		std::ofstream * event_log_;
		std::string current_element_id_;
		int current_thread_id_;
};

#endif
