#include "MooseError.h"
#include "PiecewiseParsedFunction.h"

template<>
InputParameters validParams<PiecewiseParsedFunction>()
{
  InputParameters params = validParams<Function>();
  params += validParams<MooseParsedFunctionBase>();
  params.addParam<std::string>("default_function", "0", "The default function that is used if non of the given intervals matches.");
  params.addRequiredParam<std::vector<std::string> >("functions", "The functions(i) to be used in the intervals [left(i); right(i)[. Note that individual function definition may not contain spaces as these are used to split distinct functions.");
  params.addRequiredParam<std::vector<Real> >("left", "The left limits of the time intervals.");
  params.addRequiredParam<std::vector<Real> >("right", "The right limits of the time intervals.");
  return params;
}

PiecewiseParsedFunction::PiecewiseParsedFunction(const std::string & name, InputParameters parameters) :
    Function(name, parameters),
    MooseParsedFunctionBase(name, parameters),
    _def_function(verifyFunction(getParam<std::string>("default_function"))),
    _left(getParam<std::vector<Real> >("left")),
    _right(getParam<std::vector<Real> >("right"))
{
   std::vector<std::string> v(getParam<std::vector<std::string> >("functions"));

   for (unsigned int i=0; i<v.size(); i++)
     _functions.push_back(verifyFunction(v[i]));

   if (_left.size() != _right.size() ||
       _left.size() != v.size())
      mooseError("PiecewiseParsedFunction: numbers of entries in left, right, functions do not match. Check these and make also sure that the individual functions do not contain space characters.");

   /// \todo TODO: ensure correct ordering of left and right arrays and prevent overlap

   for (unsigned int i=0; i<_functions.size(); i++)
     _function_ptrs.push_back(std::unique_ptr<MooseParsedFunctionWrapper>(new MooseParsedFunctionWrapper(_pfb_feproblem, _functions[i], _vars, _vals)));
   // we puth the default function to the very end. This simplifies value() and gradient() since we avoid branching there if no interval is found.
   _function_ptrs.push_back(std::unique_ptr<MooseParsedFunctionWrapper>(new MooseParsedFunctionWrapper(_pfb_feproblem, _def_function, _vars, _vals)));
}

PiecewiseParsedFunction::~PiecewiseParsedFunction()
{
}

void
PiecewiseParsedFunction::initialSetup()
{
}

int
PiecewiseParsedFunction::find_function(Real t)
{
  for (unsigned int i=0; i<_left.size(); i++)
    if (_left[i] <= t && _right[i] > t)
      return i;
  return _left.size();
}

Real
PiecewiseParsedFunction::value(Real t, const Point & p)
{
  return _function_ptrs[find_function(t)]->evaluate<Real>(t, p);
}

RealGradient
PiecewiseParsedFunction::gradient(Real t, const Point & p)
{
  return _function_ptrs[find_function(t)]->evaluateGradient(t, p);
}

RealVectorValue
PiecewiseParsedFunction::vectorValue(Real /*t*/, const Point & /*p*/)
{
  mooseError("The vectorValue method is not defined in PiecewiseParsedFunction");
}