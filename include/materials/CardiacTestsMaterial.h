#ifndef CardiacTestsMaterial_H
#define CardiacTestsMaterial_H

#include "Material.h"

class CardiacTestsMaterial;

template<>
InputParameters validParams<CardiacTestsMaterial>();

/**
 * This material is solely intended for testing purposes:
 * Using some selected cases, it verifies that CardiacElasticityTensor
 * and TensorHelpers do not compute complete garbage.
 */
class CardiacTestsMaterial : public Material
{
public:
  CardiacTestsMaterial(const std::string & name,
                  InputParameters parameters);

protected:
  virtual void computeQpProperties();
private:

};

#endif //CardiacTestsMaterial_H
