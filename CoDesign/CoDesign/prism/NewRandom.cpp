#include "NewRandom.h"

Uniform * NewRandom::uniform_ = 0;
Exponential * NewRandom::exponential_ = 0;
Cauchy * NewRandom::cauchy_ = 0;
Normal * NewRandom::normal_ = 0;
std::map<int, std::map<double, ChiSq *> > NewRandom::chi_sq_map_;
std::map<double, Gamma *> NewRandom::gamma_map_;
std::map<double, Pareto *> NewRandom::pareto_map_;
std::map<double, Poisson *> NewRandom::poisson_map_;
std::map<int, std::map<double, Binomial *> > NewRandom::binomial_map_;
std::map<double, std::map<double, NegativeBinomial *> > NewRandom::neg_binomial_map_;

double NewRandom::uniform (double min, double max)
{
	if (uniform_ == 0)
		{
			uniform_ = new Uniform ();
		}
	return uniform_->Next () * (max - min) + min;
}

double NewRandom::exponential (double rate)
{
	if (rate < 0)
		{
			return 0;
		}
	if (exponential_ == 0)
		{
			exponential_ = new Exponential ();
		}
	return exponential_->Next () * rate * exp (rate);
}

double NewRandom::cauchy ()
{
	if (cauchy_ == 0)
		{
			cauchy_ = new Cauchy ();
		}
	return max (cauchy_->Next (), 0.0);
}

double NewRandom::normal (double mean, double std_dev)
{
	if (std_dev < 0)
		{
			return 0;
		}
	if (normal_ == 0)
		{
			normal_ = new Normal ();
		}
	return max (normal_->Next () * std_dev + mean, 0.0);
}

double NewRandom::chi_sq (int deg_freedom, double non_centrality)
{
	if (deg_freedom < 1)
		{
			return 0;
		}
	if (chi_sq_map_[deg_freedom][non_centrality] == 0)
		{
			chi_sq_map_[deg_freedom][non_centrality] = new ChiSq (deg_freedom, non_centrality);
		}
	return chi_sq_map_[deg_freedom][non_centrality]->Next ();
}

double NewRandom::gamma (double shape)
{
	if (gamma_map_[shape] == 0)
		{
			gamma_map_[shape] = new Gamma (shape);
		}
	return gamma_map_[shape]->Next ();
}

double NewRandom::pareto (double shape)
{
	if (pareto_map_[shape] == 0)
		{
			pareto_map_[shape] = new Pareto (shape);
		}
	return pareto_map_[shape]->Next ();
}

double NewRandom::poisson (double mean)
{
	if (poisson_map_[mean] == 0)
		{
			poisson_map_[mean] = new Poisson (mean);
		}
	return poisson_map_[mean]->Next ();
}

double NewRandom::binomial (int n, double p)
{
	if (binomial_map_[n][p] == 0)
		{
			binomial_map_[n][p] = new Binomial (n, p);
		}
	return binomial_map_[n][p]->Next ();
}

double NewRandom::neg_binomial (double NX, double PX)
{
	if (neg_binomial_map_[NX][PX] == 0)
		{
			neg_binomial_map_[NX][PX] = new NegativeBinomial (NX, PX);
		}
	return neg_binomial_map_[NX][PX]->Next ();
}
