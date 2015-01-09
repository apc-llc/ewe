#include "PiecewiseFunction.h"

template<>
InputParameters validParams<PiecewiseFunction>()
{
  InputParameters params = validParams<Function>();
  params.addParam<FunctionName>("default_function", "0", "The default function that is used if none of the given intervals matches.");
  params.addParam<std::vector<FunctionName> >("functions", "The functions(i) to be used in the intervals [left(i); right(i)[");
  params.addRequiredParam<std::vector<Real> >("left", "The left limits of the time intervals.");
  params.addRequiredParam<std::vector<Real> >("right", "The right limits of the time intervals.");
  params.addParam<Real>("scale_factor", 1.0, "Scale factor to be applied to the ordinate values");
  return params;
}

PiecewiseFunction::PiecewiseFunction(const std::string & name, InputParameters parameters) :
  Function(name, parameters),
  FunctionInterface( parameters ),
  _scale_factor( getParam<Real>("scale_factor") ),
  _function_names( getParam<std::vector<FunctionName> >("functions") ),
  _nfunctions( _function_names.size() ),
  _default_function( getParam<FunctionName>("default_function") ),
  _left(getParam<std::vector<Real> >("left")),
  _right(getParam<std::vector<Real> >("right"))
{
  if (0 == _nfunctions)
    mooseError( "A PiecewiseFunction must reference at least one other function." );

  // verify interval integrity
  if (_left.size() != _right.size() || _left.size() != _nfunctions)
    mooseError("PiecewiseParsedFunction: numbers of entries in left, right, functions do not match. Check these and make also sure that the individual functions do not contain space characters.");

  // ensure correct ordering of left and right arrays and prevent overlap
  if (_right[0] <= _left[0])
    mooseError("PiecewiseParsedFunction: invalid ordering of interval limits.");

  for (unsigned int i=1; i<_left.size(); i++)
    if (  _left[i]  <= _left[i-1]
      || _right[i] <= _right[i-1]
      || _left[i]  <  _right[i-1]
      || _right[i] <= _left[i] )
      mooseError("PiecewiseParsedFunction: invalid ordering of interval limits.");
}

void
PiecewiseFunction::initialSetup()
{
  // this cannot be done in the constructor because not all required/referenced function objects may have been constructed (depends on order in input file)
  _functions.resize(_nfunctions+1);
  for (unsigned int i(0); i < _nfunctions; ++i)
  {
    if (_name == _function_names[i])
      mooseError( "A PiecewiseFunction must not reference itself" );

    Function * const f = &getFunctionByName( _function_names[i] );
    if (!f)
      mooseError(std::string("Error in composite function " + _name + ".  Function " + _function_names[i] + " referenced but not found."));
    _functions[i] = f;
  }

  // we put the default function to the very end. This simplifies value() and gradient() since we avoid branching there if no interval is found.
  Function * const f = &getFunctionByName( _default_function );
  if (!f)
    mooseError(std::string("Error in composite function " + _name + ".  Function " + _default_function + " referenced but not found."));
  _functions[_nfunctions] = f;
}


PiecewiseFunction::~PiecewiseFunction()
{
}

int
PiecewiseFunction::find_function(Real t)
{
  for (unsigned int i=0; i<_left.size(); i++)
    if (_left[i] <= t && _right[i] > t)
      return i;
  return _left.size();
}

Real
PiecewiseFunction::value(Real t, const Point & p)
{
  return _scale_factor*_functions[find_function(t)]->value( t, p );
}

RealGradient
PiecewiseFunction::gradient(Real t, const Point & p)
{
  return _scale_factor*_functions[find_function(t)]->gradient( t, p );
}

RealVectorValue
PiecewiseFunction::vectorValue(Real /*t*/, const Point & /*p*/)
{
  mooseError("The vectorValue method is not defined in PiecewiseFunction");
}
