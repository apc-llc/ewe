#ifndef MaterialSymmTensorAux_H
#define MaterialSymmTensorAux_H

// MOOSE includes
#include "MaterialAuxBase.h"
#include "SymmTensor.h"

// Forward declerations
class MaterialSymmTensorAux;

template<>
InputParameters validParams<MaterialSymmTensorAux>();

/**
 * AuxKernel for outputting a SymmTensor material property component to an AuxVariable
 */
class MaterialSymmTensorAux : public MaterialAuxBase<SymmTensor>
{
public:

  /**
   * Class constructor
   * @param name The name of the AuxKernel
   * @param parameters The input parameters for this AuxKernel
   */
  MaterialSymmTensorAux(const std::string & name, InputParameters parameters);

  /**
   * Class destructor
   */
  virtual ~MaterialSymmTensorAux();

protected:

  /**
   * Computes the component of the tensor for output
   */
  virtual Real computeValue();

  /// The row index to output
  unsigned int _row;

  /// The column index to output
  unsigned int _col;
};

#endif //MaterialSymmTensorAux_H
