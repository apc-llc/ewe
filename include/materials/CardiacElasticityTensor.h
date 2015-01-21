#ifndef CardiacElasticityTensor_H
#define CardiacElasticityTensor_H

#include "SymmTensor.h"


/**
 * Fourth order tensor that contains all symmetries that ar relevant for us,
 * i.e. 
 *  * minor symmetries \f$C_{ijkl} = C_{jikl} and C_{ijkl} = C_{ijlk}\f$
 *  * major symmetry   \f$C_{ijkl} = C_{klij}\f$
 * For performance reasons, index conversion is performed via appropriate lookup tables
 */
class CardiacElasticityTensor
{
public:
  /// default contructor: all elements are zero
  CardiacElasticityTensor() {
    this->zero();
  }

  /// default destructor
  virtual ~CardiacElasticityTensor() {}

  /// read access by linear index
  inline const Real & operator () (const unsigned v) const
  {
    return _val[v];
  }

  /// read/write access by linear index
  inline Real & operator () (const unsigned int v)
  {
    return _val[v];
  }

  /// read access by major block number
  inline const Real & operator () (const unsigned int C,
                                   const unsigned int D) const
  {
    return _val[idx(C, D)];
  }

  /// read/write access by major block number
  inline Real & operator () (const unsigned int C,
                             const unsigned int D)
  {
    return _val[idx(C, D)];
  }

  /// read access by minor index
  inline const Real & operator () (const unsigned int i,
                                   const unsigned int j,
                                   const unsigned int k,
                                   const unsigned int l) const
  {
    return _val[idx(i, j, k, l)];
  }

  /// read/write access by minor index
  inline Real & operator () (const unsigned int i,
                             const unsigned int j,
                             const unsigned int k,
                             const unsigned int l)
  {
    return _val[idx(i, j, k, l)];
  }


  friend std::ostream & operator<<(std::ostream & stream, const CardiacElasticityTensor & obj);

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
            res += _val[idx(M,N,P,Q)]*v1(M)*v2(N)*v3(P)*v4(Q);
    return res;
  }

  /// computes \f$\sum_{M,N,P,Q} t_{M,N,P,Q} v1_{M} v2_{N} r_{P,Q} \f$
  /// \todo Make use of symmetry of r for efficiency
  inline Real doubleLeftSymmDoubleRightContraction(const RealVectorValue & v1,
                                                   const RealVectorValue & v2,
                                                   const SymmTensor & r) const
  {
    Real res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=0;N<3;N++)
        for (unsigned int P=0;P<3;P++)
          for (unsigned int Q=0;Q<3;Q++)
            res += _val[idx(M,N,P,Q)] * v1(M) * v2(N) * r(P,Q);
    return res;
  }


  CardiacElasticityTensor operator+(const CardiacElasticityTensor & rhs) const
  {
    CardiacElasticityTensor t = *this;
    t += rhs;
    return t;
  }

  void operator+=( const CardiacElasticityTensor & rhs )
  {
    for (unsigned i(0); i < 21; ++i)
      _val[i] += rhs._val[i];
  }

  void operator-=( const CardiacElasticityTensor & rhs )
  {
    for (unsigned i(0); i < 21; ++i)
      _val[i] -= rhs._val[i];
  }

  void operator*=( Real rhs )
  {
    for (unsigned i(0); i < 21; ++i)
      _val[i] *= rhs;
  }

  void operator/=( Real rhs )
  {
    for (unsigned i(0); i < 21; ++i)
      _val[i] /= rhs;
  }

  void zero() {
    for (unsigned i(0); i < 21; ++i)
      _val[i] = 0;
  }
/*

  /// computes \f$\sum_{m,n,p,q} A_{Mm} B_{Nn} C_{Pp} D_{Qq} t_{m,n,p,q}\f$
  inline CardiacElasticityTensor quadProduct(RealTensorValue & A, RealTensorValue & B, RealTensorValue & C, RealTensorValue & D) const
  {
    // insane loop to cover all elements of the symmetric 4th order tensor exactly once. The if-construct avoids douple-counting for major-symmetry pairs of MN<->PQ
    CardiacElasticityTensor res(0);
    for (unsigned int M=0;M<3;M++)
      for (unsigned int N=M;N<3;N++)
        for (unsigned int P=M;P<3;P++)
          for (unsigned int Q=P;Q<3;Q++)
            if (P != M || Q >= N)
              res(M,N,P,Q) = fullContraction(A.row(M), B.row(N), C.row(P), D.row(Q));

    return res;
  }
*/

  /// computes \f$\sum_{m,n,p,q} A_{Mm} A_{Nn} A_{Pp} A_{Qq} t_{m,n,p,q}\f$
  inline CardiacElasticityTensor quadProduct(RealTensorValue & A) const
  {
    CardiacElasticityTensor res;
    for (unsigned int v=0;v<21;v++) {
      unsigned int minor[4];
      idx_inv_minor(v, minor);
      res(v) = fullContraction(A.row(minor[0]), A.row(minor[1]), A.row(minor[2]), A.row(minor[3]));
    }
    return res;
  }

protected:
  /// table for converting minor indices to major block indices
  static unsigned int const tbl_minor[3][3];

  /// table for converting major block to linear indices
  static unsigned int const tbl_major[6][6];

  /// table for converting major block indices to minor indices
  static unsigned int const tbl_minor_inv[6][2];

  /// table for converting linear indices to major block indices
  static unsigned int const tbl_major_inv[21][2];

  /// conversion from major indices to linear index
  inline unsigned int idx(const unsigned int C, const unsigned int D) const
  {
    mooseAssert(C>=0 && C<=5, "CardiacElasticityTensor::idx (C, D) : Invalid value for C.");
    mooseAssert(D>=0 && D<=5, "CardiacElasticityTensor::idx (C, D) : Invalid value for D.");
    return tbl_major[C][D];
  }

  /// conversion from minor indices to linear index
  inline unsigned int idx(const unsigned int i, const unsigned int j, const unsigned int k, const unsigned int l) const
  {
    mooseAssert(i>=0 && i<=2, "CardiacElasticityTensor::idx (i, j, k, l) : Invalid value for i.");
    mooseAssert(j>=0 && j<=2, "CardiacElasticityTensor::idx (i, j, k, l) : Invalid value for j.");
    mooseAssert(k>=0 && k<=2, "CardiacElasticityTensor::idx (i, j, k, l) : Invalid value for k.");
    mooseAssert(l>=0 && l<=2, "CardiacElasticityTensor::idx (i, j, k, l) : Invalid value for l.");
    return tbl_major[tbl_minor[i][j]][tbl_minor[k][l]];
  }

  /// conversion from linear index to major indices
  inline void idx_inv_major(const unsigned int v, unsigned int major[2]) const
  {
    mooseAssert(v>=0 && v<=20, "CardiacElasticityTensor::idx_inv (v) : Invalid value for v.");
    major[0] = tbl_major_inv[v][0];
    major[1] = tbl_major_inv[v][1];
  }

  /// conversion from linear index to minor
  inline void idx_inv_minor(const unsigned int v, unsigned int minor[4]) const
  {
    mooseAssert(v>=0 && v<=20, "CardiacElasticityTensor::idx_inv (v) : Invalid value for v.");
    unsigned int major[2];
    idx_inv_major(v, major);

    minor[0] = tbl_minor_inv[major[0]][0];
    minor[1] = tbl_minor_inv[major[0]][1];
    minor[2] = tbl_minor_inv[major[1]][0];
    minor[3] = tbl_minor_inv[major[1]][1];
  }


private:
  /// actual entries, for numbering see the lookup tables above
  Real _val[21];

};

#endif //CardiacElasticityTensor_H
