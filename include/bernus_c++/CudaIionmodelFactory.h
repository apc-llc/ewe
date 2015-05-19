#ifndef CUDAIIONMODELFACTORY_HPP
#define CUDAIIONMODELFACTORY_HPP

#include "CudaIionmodel.h"
#include "CudaBernus.h"
#include <stdexcept>

/**
 * @todo docu
 */
class CudaIionmodelFactory {
  
public:
  
  //! @todo docu
  enum IionmodelSelection {BERNUS};
  
  //! @todo docu
  //! @param[in] choice Integer indicating the model to use.
  //! @param[in] gates @todo complete
  //! @param[out] Iionmodel* A pointer to an object of the subclass implementing the selected model.
  static CudaIionmodel * factory(IionmodelSelection choice) {
    
    switch(choice) {
      case BERNUS :
        return CudaBernus::factory();
        break;
      default:
        throw std::runtime_error("IIonmodelFactory: No model available for selected value of choice");
        
    }
  }
  
};

#endif // CUDAIIONMODELFACTORY_HPP