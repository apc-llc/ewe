#ifndef IIONMODELFACTORY_HPP
#define IIONMODELFACTORY_HPP

#include "Iionmodel.h"
#include "bernus.h"
#include <stdexcept>

/**
 * @todo docu
 */
class IionmodelFactory {
  
public:
  
  //! @todo docu
  static const int bernus = 1;
  
  //! @todo docu
  //! @param[in] choice Integer indicating the model to use.
  //! @param[out] Iionmodel* A pointer to an object of the subclass implementing the selected model.
  static Iionmodel * factory(int choice) {
    if (choice == IionmodelFactory::bernus)
      return bernus::factory();
    else
      throw std::runtime_error("IIonmodelFactory: No model available for selected value of choice");
  }
  
};

#endif // IIONMODELFACTORY_HPP