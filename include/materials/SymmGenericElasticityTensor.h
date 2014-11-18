#ifndef SymmGenericElasticityTensor_H
#define SymmGenericElasticityTensor_H

#include "SymmElasticityTensor.h"


/**
 * More Generic version of SymmElasticityTensor that allows access to
 * individual tensor elements and provides some common
 * contraction operations
 */
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
    return _val[convert_indices(i,j)];
  }

  inline Real & operator () (const unsigned int i,
                             const unsigned int j)
  {
    mooseAssert(i>=0 && i<=5, "SymmGenericElasticityTensor::operator () : Invalid value for i.");
    mooseAssert(j>=0 && j<=5, "SymmGenericElasticityTensor::operator () : Invalid value for j.");
    return _val[convert_indices(i,j)];
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
    return _val[convert_indices(m, n, o, p)];
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
    return _val[convert_indices(m, n, o, p)];
  }

  /// computes \f$\sum_{M,N,P,Q} t_{M,N,P,Q} v1_{M} v2_{N} v3_{P} v4_{Q} \f$
  inline Real fullContraction(const RealVectorValue & v1,
                              const RealVectorValue & v2,
                              const RealVectorValue & v3,
                              const RealVectorValue & v4) const
  {
    Real res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            res += _val[convert_indices(M,N,P,Q)]*v1(M)*v2(N)*v3(P)*v4(Q);
    return res;
  }

  /// computes \f$\sum_{M,N,P,Q} t_{M,N,P,Q} v1_{M} v2_{N} \frac{1}{2}\left(v3_{P} v4_{Q} + v3_{Q} v4_{P}\right) \f$
  inline Real doubleLeftSymmDoubleRightContraction(const RealVectorValue & v1,
                                                   const RealVectorValue & v2,
                                                   const RealVectorValue & v3,
                                                   const RealVectorValue & v4) const
  {
    Real res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            res += _val[convert_indices(M,N,P,Q)] * v1(M) * v2(N) * 0.5*(v3(P)*v4(Q)+v3(Q)*v4(P));
    return res;
  }

  /// computes \f$\sum_{M,N,P,Q} t_{M,N,P,Q} v1_{M} v2_{N} r_{P,Q} \f$
  inline Real doubleLeftSymmDoubleRightContraction(const RealVectorValue & v1,
                                                   const RealVectorValue & v2,
                                                   const SymmTensor & r) const
  {
    Real res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            res += _val[convert_indices(M,N,P,Q)] * v1(M) * v2(N) * r(P,Q);
    return res;
  }

  /// computes \f$\sum_{T,U} (R^t)_{M,T} t_{T,U,P,Q} R_{U,N} = \sum_{T,U} (R)_{T,M} R_{U,N} t_{T,U,P,Q}\f$
  inline SymmGenericElasticityTensor doubleLeftProduct(const RealTensorValue & R) const
  {
    SymmGenericElasticityTensor res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            for (unsigned int T=0;T<3;T++)
              for (unsigned int U=0;U<3;U++)
                res(M,N,P,Q) += _val[convert_indices(T,U,P,Q)] * R(T,M) * R(U,N);
    return res;
  }

  /// computes \f$\sum_{m,n,p,q} (R^t)_{M,m} (R^t)_{P,p} t_{m,n,p,q} R_{n,N} R_{q,Q} = \sum_{m,n,p,q} R_{m,M} R_{p,P} R_{n,N} R_{q,Q} t_{m,n,p,q} \f$
  inline SymmGenericElasticityTensor doubleLeftdoubleRightProduct(const RealTensorValue & R) const
  {
    SymmGenericElasticityTensor res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            for (unsigned int m=0;m<3;m++)
              for (unsigned int n=0;n<3;n++)
                for (unsigned int p=0;p<3;p++)
                  for (unsigned int q=0;q<3;q++)
                    res(M,N,P,Q) += _val[convert_indices(m,n,p,q)] * R(m,M) * R(n,N) * R(p,P) * R(q,Q);
    return res;
  }

protected:

  virtual void calculateEntries(unsigned int qp) {};

  /// Converts indices i, j (slightly modified Voigt Notation, see SymmElasticityTensor.h)
  /// to a single index into the internal _val[] array
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

  /// Converts indices M,N,P,Q to a single index into the internal _val[] array
  inline int convert_indices(const unsigned int m,
                             const unsigned int n,
                             const unsigned int o,
                             const unsigned int p) const
  {
    const unsigned int i(convert_indices(m,n));
    const unsigned int j(convert_indices(o,p));
    const unsigned int row( i < j ? i : j);
    const unsigned int col( i > j ? i : j);
    // great index magic :-)
    return col + (row*(11-row)) / 2;
  }
};

#endif //SymmGenericElasticityTensor_H
