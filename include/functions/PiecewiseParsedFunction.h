/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef PiecewiseParsedFunction_H
#define PiecewiseParsedFunction_H

// std includes
#include <iostream>
#include <string>
#include <map>

// MOOSE includes
#include "Function.h"
#include "MooseParsedFunctionBase.h"
#include "MooseParsedFunctionWrapper.h"

//Forward declarations
class PiecewiseParsedFunction;

template<>
InputParameters validParams<PiecewiseParsedFunction>();

/**
 * This class is used to evaluate symbolic equations passed in to Moose through
 * the input file. It supports symbolic variables that you can change by putting
 * a different value in a reference returned by getVarAddr().
 * Furthermore, different expressions can be given for different times.
 *
 * The idea for this class came from MooseParsedFunction
 *
 * Documentation for the Function Parser can be found at:
 * http://warp.povusers.org/FunctionParser/fparser.html
 */
class PiecewiseParsedFunction :
  public Function,
  public MooseParsedFunctionBase
{
public:

  /**
   * Created from MooseSystem via the FunctionFactory.
   * @param name The name of the function
   * @param parameters The input parameters
   */
  PiecewiseParsedFunction(const std::string & name, InputParameters parameters);

  /**
   * Destructor, it cleans up the libMesh::ParsedFunction object
   */
  virtual ~PiecewiseParsedFunction();

  /**
   * Evaluate the equation at the given location. For 1-D and 2-D equations
   * x and y are optional.
   * @param t The evaluation time
   * @param pt The current point (x,y,z)
   * @return The result of evaluating the function
   */
  virtual Real value(Real t, const Point & pt);

  /**
   * Evaluate the gradient of the function. This is computed in libMesh
   * through automatic symbolic differentiation.
   */
  virtual RealGradient gradient(Real t, const Point & p);

  /**
   * Method invalid for ParsedGradFunction
   * @see ParsedVectorFunction
   */
  virtual RealVectorValue vectorValue(Real t, const Point & p);

  /**
   * Nothing to do here.
   */
  virtual void initialSetup();

protected:

  /// The functions defined by the user
  std::vector<std::string> _functions;
  /// Function defined by the user to be in place when no other function applies
  std::string _def_function;
  /// Left limits of the respective intervals for the functions
  std::vector<Real> _left;
  /// Right limits of the repsective intervals for the functions
  std::vector<Real> _right;

  /// Pointers to the wrapper objects for the functions, the dafault function is the very last on. Therefore, this array has one additional entry compared to _left, _right, _functions
  std::vector<std::unique_ptr<MooseParsedFunctionWrapper> > _function_ptrs;

  /// Function for searching for the current interval index. Will return _left.size() if no matching interval has been found.
  int find_function(Real t);
};
#endif //PiecewiseParsedFunction_H
