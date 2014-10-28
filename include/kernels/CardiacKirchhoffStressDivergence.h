#ifndef CardiacKirchhoffStressDivergence_H
#define CardiacKirchhoffStressDivergence_H

#include "Kernel.h"
#include "SymmGenericElasticityTensor.h"

//Forward Declarations
class ColumnMajorMatrix;
class CardiacKirchhoffStressDivergence;
class SymmElasticityTensor;
class SymmTensor;

template<>
InputParameters validParams<CardiacKirchhoffStressDivergence>();

class CardiacKirchhoffStressDivergence : public Kernel
{
public:

  CardiacKirchhoffStressDivergence(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  MaterialProperty<RealTensorValue> & _stress;
  MaterialProperty<SymmGenericElasticityTensor> & _stress_derivative;

  /// computes   SUM(M,N) [ t(M,N) v1(M) v2(N) ]
  Real fullContraction(const RealTensorValue & t,
                       const RealVectorValue & v1,
                       const RealVectorValue & v2) const;
  /// computes   SUM(M,N,P,Q) [ t(M,N,P,Q) v1(M) v2(N) v3(P) v4(Q) ]
  Real fullContraction(const SymmGenericElasticityTensor & t,
                       const RealVectorValue & v1,
                       const RealVectorValue & v2,
                       const RealVectorValue & v3,
                       const RealVectorValue & v4) const;
  /// computes   SUM(M,N,P,Q) [ t(M,N,P,Q) v1(M) v2(N)  0.5{ v3(P)v4(Q) + v3(Q)v4(P) } ]
  Real doubleLeftSymmDoubleRightContraction(const SymmGenericElasticityTensor & t,
                                            const RealVectorValue & v1,
                                            const RealVectorValue & v2,
                                            const RealVectorValue & v3,
                                            const RealVectorValue & v4) const;
private:
  const unsigned int _component;

  const unsigned int _xdisp_var;
  const unsigned int _ydisp_var;
  const unsigned int _zdisp_var;
};
#endif //CardiacKirchhoffStressDivergence_H
