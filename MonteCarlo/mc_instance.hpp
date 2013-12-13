#ifndef MC_INSTANCE
#define MC_INSTANCE

#include "instance_factory.hpp"
#include "monte_carlo.hpp"
#include "ppapi/cpp/var_dictionary.h"
#include "ppapi/cpp/var_array.h"
#include "ppapi/utility/completion_callback_factory.h"
#include "ppapi/utility/threading/simple_thread.h"

// The MonteCarloInstance that stores 
class MonteCarloInstance : public pp::Instance {
  public:
    explicit MonteCarloInstance( PP_Instance instance ) 
      : pp::Instance(instance), callback_factory_(this), sim_thread_(this) {};
    virtual ~MonteCarloInstance( ){ sim_thread_.Join(); };
    virtual void HandleMessage( const pp::Var& );
    virtual bool Init(uint32_t /*argc*/,
        const char * /*argn*/ [],
        const char * /*argv*/ []) {
      sim_thread_.Start();
      sim_thread_.message_loop().PostWork( 
        callback_factory_.NewCallback( &MonteCarloInstance::Version ));
      return true;
    }

  private:
    MonteCarlo mc;
    pp::CompletionCallbackFactory<MonteCarloInstance> callback_factory_;
    pp::SimpleThread sim_thread_; // Thread for MC simulations
    void Simulate(int32_t, unsigned int N); 
    pp::VarDictionary PostResponse( len_t runningTotal, len_t i);
    void Version( int32_t ) {
      PostMessage("Monte Carlo Version 0.1");
    }
};

#endif
