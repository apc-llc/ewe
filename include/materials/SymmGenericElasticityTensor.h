#ifndef SymmGenericElasticityTensor_H
#define SymmGenericElasticityTensor_H

#include "SymmElasticityTensor.h"

class SymmGenericElasticityTensor : public SymmElasticityTensor
{
public:
  SymmGenericElasticityTensor(const bool constant = true);

  virtual ~SymmGenericElasticityTensor() {}

  inline const Real & operator () (const unsigned int i,
                                   const unsigned int j) const
  {
    mooseAssert(i>=0 && i<=5, "SymmGenericElasticityTensor::operator () : Invalid value for i.");
    mooseAssert(j>=0 && j<=5, "SymmGenericElasticityTensor::operator () : Invalid value for j.");

    const unsigned int row( i < j ? i : j);
    const unsigned int col( i > j ? i : j);
    // great index magic :-)
    return _val[col + (row*(11-row)) / 2];
  }

  inline Real & operator () (const unsigned int i,
                             const unsigned int j)
  {
    mooseAssert(i>=0 && i<=5, "SymmGenericElasticityTensor::operator () : Invalid value for i.");
    mooseAssert(j>=0 && j<=5, "SymmGenericElasticityTensor::operator () : Invalid value for j.");

    const unsigned int row( i < j ? i : j);
    const unsigned int col( i > j ? i : j);
    // great index magic :-)
    return _val[col + (row*(11-row)) / 2];
  }

  inline int convert_indices(const unsigned int m,
                             const unsigned int n) const
  {
    switch(10*m+n) {
      case 00: return 0;
      case 11: return 1;
      case 22: return 2;
      case 01:
      case 10: return 3;
      case 12:
      case 21: return 4;
      case 20:
      case 02: return 5;
    }
    mooseError("convert_indices: invalid index");
  }

  inline const Real & operator () (const unsigned int m,
                                   const unsigned int n,
                                   const unsigned int o,
                                   const unsigned int p) const
  {
    mooseAssert(m>=0 && m<=3, "SymmGenericElasticityTensor::operator () : Invalid value for m.");
    mooseAssert(n>=0 && n<=3, "SymmGenericElasticityTensor::operator () : Invalid value for n.");
    mooseAssert(o>=0 && o<=3, "SymmGenericElasticityTensor::operator () : Invalid value for o.");
    mooseAssert(p>=0 && p<=3, "SymmGenericElasticityTensor::operator () : Invalid value for p.");

    const unsigned int i(convert_indices(m,n));
    const unsigned int j(convert_indices(o,p));
    const unsigned int row( i < j ? i : j);
    const unsigned int col( i > j ? i : j);
    // great index magic :-)
    return _val[col + (row*(11-row)) / 2];
  }

  inline Real & operator () (const unsigned int m,
                             const unsigned int n,
                             const unsigned int o,
                             const unsigned int p)
  {
    mooseAssert(m>=0 && m<=3, "SymmGenericElasticityTensor::operator () : Invalid value for m.");
    mooseAssert(n>=0 && n<=3, "SymmGenericElasticityTensor::operator () : Invalid value for n.");
    mooseAssert(o>=0 && o<=3, "SymmGenericElasticityTensor::operator () : Invalid value for o.");
    mooseAssert(p>=0 && p<=3, "SymmGenericElasticityTensor::operator () : Invalid value for p.");

    const unsigned int i(convert_indices(m,n));
    const unsigned int j(convert_indices(o,p));
    const unsigned int row( i < j ? i : j);
    const unsigned int col( i > j ? i : j);
    // great index magic :-)
    return _val[col + (row*(11-row)) / 2];
  }

protected:

  virtual void calculateEntries(unsigned int qp) {};
};

#endif //SymmGenericElasticityTensor_H
