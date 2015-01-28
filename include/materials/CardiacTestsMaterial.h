#ifndef CardiacTestsMaterial_H
#define CardiacTestsMaterial_H

#include "Material.h"
#include "Iionmodel.h"
#include "IionmodelFactory.h"

class CardiacTestsMaterial;

template<>
InputParameters validParams<CardiacTestsMaterial>();

/**
 * \todo document
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
