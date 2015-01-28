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


  if (testres)
    std::cout << std::endl << COLOR_GREEN << "SUCCESSFULL !!" << COLOR_DEFAULT << std::endl;
  else
    std::cout << std::endl << COLOR_RED << "FAILED !!" << COLOR_DEFAULT << std::endl;
}

void
CardiacTestsMaterial::computeQpProperties()
{
}
