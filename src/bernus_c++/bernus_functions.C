#include "bernus_functions.h"
double const bernus_functions::e_na = (bernus_functions::R*bernus_functions::T/bernus_functions::Fa)*log(bernus_functions::na_e/bernus_functions::na_i);
double const bernus_functions::e_ca = (bernus_functions::R*bernus_functions::T/(2.0*bernus_functions::Fa))*log(bernus_functions::ca_e/bernus_functions::ca_i);
double const bernus_functions::e_to = (bernus_functions::R*bernus_functions::T/bernus_functions::Fa)*log( (0.043*bernus_functions::na_e + bernus_functions::k_e)/(0.043*bernus_functions::na_i + bernus_functions::k_i) );
double const bernus_functions::e_k  = (bernus_functions::R*bernus_functions::T/bernus_functions::Fa)*log(bernus_functions::k_e/bernus_functions::k_i);
