#ifndef NEWRANDOM_H
#define NEWRANDOM_H

#include <iostream>
#include <map>
#include <math.h>
#include "newran.h"

class NewRandom
{

public:

  static double uniform (double min, double max);
  static double exponential (double rate);
  static double cauchy ();
  static double normal (double mean, double std_dev);
  static double chi_sq (int deg_freedom, double non_centrality);
  static double gamma (double shape);
  static double pareto (double shape);
  static double poisson (double mean);
  static double binomial (int n, double p);
  static double neg_binomial (double NX, double PX);

protected:

  NewRandom ();

private:

  static Uniform * uniform_;
  static Exponential * exponential_;
  static Cauchy * cauchy_;
  static Normal * normal_;
  static std::map<int, std::map<double, ChiSq *> > chi_sq_map_;
  static std::map<double, Gamma *> gamma_map_;
  static std::map<double, Pareto *> pareto_map_;
  static std::map<double, Poisson *> poisson_map_;
  static std::map<int, std::map<double, Binomial *> > binomial_map_;
  static std::map<double, std::map<double, NegativeBinomial *> > neg_binomial_map_;

};

#endif
