#include "Thread.h"
#include "ServiceDemand.h"
#include "ServiceCompletion.h"

const port_t Thread::ServiceDemandInput = 0;
const port_t Thread::ServiceCompletionOutput = 1;

Thread::Thread (std::string worker_name, int worker_number, std::ofstream& event_log):
  atomic (),
  worker_name_ (worker_name),
  worker_number_ (worker_number),
  event_log_ (&event_log)
{
}

void Thread::init ()
{
  //*this->event_log_ << this->worker_name_ << "::init ()\n";
  this->event_log_->flush ();
  passivate ();
}

void Thread::delta_ext (double e, const adevs_bag<PortValue>& x)
{
  //*this->event_log_ << this->worker_name_ << "::delta_ext ()\n";
  if (x.getSize () > 1)
    {
      *this->event_log_ << "ERROR: " << this->worker_name_ << " received multiple requests at time " << timeCurrent () << "\n";
      this->event_log_->flush ();
    }

  port_t arrival_port = x.get(0).port;
  ServiceDemand * new_service_demand = (ServiceDemand *)(x.get(0).value);
  this->current_element_id_ = new_service_demand->element_id_;
  this->current_thread_id_ = new_service_demand->thread_id_;
  hold (new_service_demand->service_demand_);
}

void Thread::delta_int ()
{
  //*this->event_log_ << this->worker_name_ << "::delta_int ()\n";
  passivate ();
}

void Thread::delta_conf (const adevs_bag<PortValue>& x)
{
  //*this->event_log_ << this->worker_name_ << "::delta_conf ()\n";
  delta_int ();
  delta_ext (0.0, x);
}

void Thread::output_func (adevs_bag<PortValue>& y)
{
  //*this->event_log_ << this->worker_name_ << "::output_func ()\n";
  ServiceCompletion * new_event = new ServiceCompletion (this->current_element_id_, this->current_thread_id_, this->worker_number_);
  output (ServiceCompletionOutput, new_event, y);
}

void Thread::gc_output (adevs_bag<PortValue>& g)
{
  //*this->event_log_ << this->worker_name_ << "::gc_output ()\n";
  for (int i = 0; i < g.getSize (); i++)
    {
      delete g[i].value;
    }
}

Thread::~Thread ()
{
}