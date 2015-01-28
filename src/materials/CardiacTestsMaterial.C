#include "CardiacTestsMaterial.h"
#include "CardiacElasticityTensor.h"
#include "SymmTensor.h"
#include "TensorHelpers.h"

template<>
InputParameters validParams<CardiacTestsMaterial>()
{
  InputParameters params = validParams<Material>();
  return params;
}

CardiacTestsMaterial::CardiacTestsMaterial(const std::string & name,
                                 InputParameters parameters) :
  Material(name, parameters){

  std::cout << std::endl << COLOR_YELLOW << "CardiacTestsMaterial performing its tests..." << COLOR_DEFAULT << std::endl;
  bool testres(true);

  if (1) {
    Real res(CardiacElasticityTensor(1).doubleLeftSymmDoubleRightContraction(Point(1,1,1),Point(1,1,1),SymmTensor(1,1,1,1,1,1)));
    Real expected_res(81);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "CardiacElasticityTensor(1).doubleLeftSymmDoubleRightContraction(Point(1,1,1),Point(1,1,1),SymmTensor(1,1,1,1,1,1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    Real res(CardiacElasticityTensor(1).fullContraction(Point(1,1,1),Point(1,1,1),Point(1,1,1),Point(1,1,1)));
    Real expected_res(81);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "CardiacElasticityTensor(1).fullContraction(Point(1,1,1),Point(1,1,1),Point(1,1,1),Point(1,1,1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    Real res(TensorHelpers::fullContraction(SymmTensor(1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1)));
    Real expected_res(9);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::fullContractionSymmTensor(1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1)" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::symmProd(RealTensorValue(1,1,1,1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1)));
    SymmTensor expected_res(9,9,9,9,9,9);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::symmProd(RealTensorValue(1,1,1,1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::symmProd(RealTensorValue(1,1,1,1,1,1,1,1,1)));
    SymmTensor expected_res(3,3,3,3,3,3);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::symmProd(RealTensorValue(1,1,1,1,1,1,1,1,1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    RealTensorValue res(TensorHelpers::prod(SymmTensor(1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1)));
    RealTensorValue expected_res(3,3,3,3,3,3,3,3,3);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::prod(SymmTensor(1,1,1,1,1,1), SymmTensor(1,1,1,1,1,1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor M(20, -4, -16, 11, 2, 1);
    RealTensorValue res(TensorHelpers::prod(M, TensorHelpers::symmInv(M, TensorHelpers::STtoRTV(M).det())));
    RealTensorValue expected_res(1,0,0,0,1,0,0,0,1);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::prod(M, TensorHelpers::symmInv(M, TensorHelpers::STtoRTV(M).det()))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::symmInv(SymmTensor(1,1,1,0,0,0), 1.0));
    SymmTensor expected_res(1,1,1,0,0,0);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::prod(SymmTensor(1,1,1), 1.0)" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    RealTensorValue M(20, -4, -16, 11, 2, 1, 8, 12, -2);
    RealTensorValue res(M*TensorHelpers::mat_inv(M, M.det()));
    RealTensorValue expected_res(1,0,0,0,1,0,0,0,1);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::prod(M, TensorHelpers::mat_inv(M, M.det()))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    RealTensorValue res(TensorHelpers::mat_inv(RealTensorValue(1,0,0,0,1,0,0,0,1), 1.0));
    RealTensorValue expected_res(1,0,0,0,1,0,0,0,1);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::mat_inv(RealTensorValue(1,0,0,0,1,0,0,0,1), 1.0)" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::square(SymmTensor(20, -4, -16, 11, 2, 1)));
    SymmTensor expected_res(522,141,261,178,-29,26);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::square(SymmTensor(20, -4, -16, 11, 2, 1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    RealTensorValue res(TensorHelpers::kron(Point(1,2,3),Point(4,5,6)));
    RealTensorValue expected_res(4,5,6,8,10,12,12,15,18);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::kron(Point(1,2,3),Point(4,5,6))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::kron(Point(1,2,3)));
    SymmTensor expected_res(1,4,9,2,6,3);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::kron(Point(1,2,3))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::kronSym(Point(1,2,3),Point(4,5,6)));
    SymmTensor expected_res(8,20,36,13,27,18);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::kronSymm(Point(1,2,3),Point(4,5,6))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::scaledID(1.23));
    SymmTensor expected_res(1.23,1.23,1.23,0,0,0);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::scaledID(1.23)" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (1) {
    SymmTensor res(TensorHelpers::elementwiseProduct(SymmTensor(20, -4, -16, 11, 2, 1),SymmTensor(1./20, -1./4, -1./16, 1./11, 1./2, 1)));
    SymmTensor expected_res(1,1,1,1,1,1);
    std::cout << (res==expected_res ? COLOR_GREEN:COLOR_RED) << "TensorHelpers::elementwiseProduct(SymmTensor(20, -4, -16, 11, 2, 1),SymmTensor(1./20, -1./4, -1./16, 1./11, 1./2, 1))" << std::endl;
    std::cout << "result: " << res << std::endl;
    std::cout << "expect: " << expected_res << COLOR_DEFAULT << std::endl;
    testres &= (res==expected_res);
  }

  if (testres)
    std::cout << std::endl << COLOR_GREEN << "SUCCESSFULL !!" << COLOR_DEFAULT << std::endl;
  else {
    std::cout << std::endl << COLOR_RED << "FAILED !!" << COLOR_DEFAULT << std::endl;
    mooseError("CardiactestsMaterial: Tests failed.");
  }
}

void
CardiacTestsMaterial::computeQpProperties()
{
}
