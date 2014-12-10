#ifndef TensorHelpers_h
#define TensorHelpers_h

#include "libmesh/tensor_value.h"
#include "SymmTensor.h"

namespace TensorHelpers {
  /// Converts a SymmTensor to a RealTensorValue with the respective symmetric elements.
  const inline RealTensorValue STtoRTV(const SymmTensor & A) {
    RealTensorValue B;
    B(0,0) = A(0,0);
    B(1,1) = A(1,1);
    B(2,2) = A(2,2);
    B(0,1) = B(1,0) = A(0,1);
    B(0,2) = B(2,0) = A(0,2);
    B(1,2) = B(2,1) = A(1,2);
    return B;
  }

  /// Converts a SymmTensor \f$A\f$ to a SymmGenericElasticityTensor \f$C_{MNPQ}\f$.
  /// assuming that \f$C_{MN**} = C_{**MN} = A_{MN}\f$.
  const inline SymmGenericElasticityTensor STtoSGET(const SymmTensor & A) {
    SymmGenericElasticityTensor B;
    B(0,0,0,0) = A(0,0);
    B(1,1,1,1) = A(1,1);
    B(2,2,2,2) = A(2,2);
    B(1,2,1,2) = A(1,2);
    B(0,2,0,2) = A(0,2);
    B(0,1,0,1) = A(0,1);
    return B;
  }

  /// Computes \f$\mathrm{outer}^\mathrm{T}\,\mathrm{inner}\,\mathrm{outer}\f$.
  const inline SymmTensor symmProd(const RealTensorValue & outer, const SymmTensor & inner) {
    /** @todo TODO: this would be much more efficient:
    SymmTensor res;
    for (unsigned int i=0;i<3;i++)
      for (unsigned int j=i;j<3;j++) {
        Real s(0);
        for (unsigned int k=0;k<3;k++)
          s += outer(k,i) * ( inner(k,0)*outer(0,j) + inner(k,1)*outer(1,j) + inner(k,2)*outer(2,j) );
        res(i,j) = s;
      }
    */
    RealTensorValue r(outer.transpose() * STtoRTV(inner) * outer);
    return SymmTensor(r(0,0), r(1,1), r(2,2), r(0,1), r(1,2), r(0,2) );
  }

  /// Computes the symmetric product \f$A^\mathrm{T}A\f$.
  const inline SymmTensor symmProd(const RealTensorValue & A) {
                      /* i j        i      j        i      j        i      j */
    return SymmTensor(/* 0 0 */ A(0,0)*A(0,0) + A(1,0)*A(1,0) + A(2,0)*A(2,0),
                      /* 1 1 */ A(0,1)*A(0,1) + A(1,1)*A(1,1) + A(2,1)*A(2,1),
                      /* 2 2 */ A(0,2)*A(0,2) + A(1,2)*A(1,2) + A(2,2)*A(2,2),
                      /* 0 1 */ A(0,0)*A(0,1) + A(1,0)*A(1,1) + A(2,0)*A(2,1),
                      /* 1 2 */ A(0,1)*A(0,2) + A(1,1)*A(1,2) + A(2,1)*A(2,2),
                      /* 0 2 */ A(0,0)*A(0,2) + A(1,0)*A(1,2) + A(2,0)*A(2,2));
  }

  /// Computes \f$C^{-1}\f$ using if \f$\mathrm{det}C\f$ is already known.
  const inline SymmTensor symmInv(const SymmTensor & C, const Real det) {
    SymmTensor Cinv(/* 00 */ C(0,0)*C(1,1)-C(1,2)*C(1,2),
                    /* 11 */ C(0,0)*C(0,0)-C(0,2)*C(0,2),
                    /* 22 */ C(0,0)*C(1,1)-C(0,1)*C(0,1),
                    /* 01 */ C(0,2)*C(1,2)-C(0,0)*C(0,1),
                    /* 12 */ C(0,1)*C(0,2)-C(0,0)*C(1,2),
                    /* 02 */ C(0,1)*C(1,2)-C(0,2)*C(1,1));
    return Cinv * (1./det);
  }

  /// Computes \f$\mathbf{A}\cdot\mathbf{B}\f$ for symmetric tensors (result is not necessarily symmetric any more !)
  const inline RealTensorValue prod(const SymmTensor & A, const SymmTensor & B) {
    RealTensorValue res;
    for (int M=0;M<3;M++)
      for (int N=0;N<3;N++)
        res(M,N) = A(M,0)*B(0,N) + A(M,1)*B(1,N) + A(M,2)*B(2,N);
    return res;
  }

  /// Computes \f$\mathbf{A}\cdot\mathbf{A}\f$ for symmetric tensors
  const inline SymmTensor square(const SymmTensor & A) {
    SymmTensor res;
    for (int M=0;M<3;M++)
      for (int N=M;N<3;N++)
        res(M,N) = A(M,0)*A(0,N) + A(M,1)*A(1,N) + A(M,2)*A(2,N);
    return res;
  }

  /// Computes \f$\vec{a}\otimes\vec{b}=\vec{a}\cdot\vec{b}^\mathrm{T}\f$
  const inline RealTensorValue kron(const RealVectorValue &a, const RealVectorValue &b) {
    RealTensorValue res;
    for (int M=0;M<3;M++)
      for (int N=0;N<3;N++)
        res(M,N) = a(M)*b(N);
    return res;
  }

  /// Computes \f$\vec{a}\otimes\vec{a}=\vec{a}\cdot\vec{a}^\mathrm{T}\f$
  const inline SymmTensor kron(const RealVectorValue &a) {
    SymmTensor res;
    for (int M=0;M<3;M++)
      for (int N=M;N<3;N++)
        res(M,N) = a(M)*a(N);
    return res;
  }

  /// Computes \f$\vec{a}\otimes\vec{b} + \vec{b}\otimes\vec{a} = \vec{a}\cdot\vec{b}^\mathrm{T} + \vec{b}\cdot\vec{a}^\mathrm{T}\f$
  const inline SymmTensor kronSym(const RealVectorValue &a, const RealVectorValue &b) {
    SymmTensor res;
    for (int M=0;M<3;M++)
      for (int N=M;N<3;N++)
        res(M,N) = a(M)*b(N) + a(N)*b(M);
    return res;
  }

  /// Prepares a scales identity matrix, i.e. \f$a\cdot\mathbf{I}\f$
  const inline SymmTensor scaledID(const Real &a) {
    return SymmTensor(a,a,a,0,0,0);
  }

}

#endif
