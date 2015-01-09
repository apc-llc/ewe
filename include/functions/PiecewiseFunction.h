#ifndef PiecewiseFunction_H
#define PiecewiseFunction_H

#include "Function.h"
#include "FunctionInterface.h"

class PiecewiseFunction;

template<>
InputParameters validParams<PiecewiseFunction>();

/**
 * Piecewise Function that combines several functions objects that
 * are defined on different time intervals.
 * The idea for this came from #CompositeFunction
 */
class PiecewiseFunction : public Function, protected FunctionInterface
{
public:
  PiecewiseFunction(const std::string & name, InputParameters parameters);
  virtual ~PiecewiseFunction();

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
   * Setup before initial use but after all constructors have been invoked
   */
  virtual void initialSetup();

private:
  /// global scaling factor
  const Real _scale_factor;
  /// supplied function names
  const std::vector<FunctionName> & _function_names;
  /// number of functions given
  const unsigned int _nfunctions;
  /// dupplied default function name
  const FunctionName & _default_function;
  /// The functions defined by the user. The default function is manually added as the very last one. Therefore, this array has one additional entry compared to _left and _right
  std::vector<Function *> _functions;
  /// Left limits of the respective intervals for the functions
  std::vector<Real> _left;
  /// Right limits of the repsective intervals for the functions
  std::vector<Real> _right;
  /// Function for searching for the current interval index. Will return _left.size() if no matching interval has been found.
  int find_function(Real t);

};

#endif //COMPOSITE_H
