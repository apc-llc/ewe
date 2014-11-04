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
  enum IionmodelSelection {BERNUS};
  
  //! @todo docu
  //! @param[in] choice Integer indicating the model to use.
  //! @param[in] gates @todo complete
  //! @param[out] Iionmodel* A pointer to an object of the subclass implementing the selected model.
  static Iionmodel * factory(IionmodelSelection choice, std::vector<double>* gates, std::vector<double>* gates_dt) {
    
    switch(choice) {
      case BERNUS :
        return bernus::factory(gates, gates_dt);
        break;
      default:
        throw std::runtime_error("IIonmodelFactory: No model available for selected value of choice");
        
    }
  }
  
};

#endif // IIONMODELFACTORY_HPP