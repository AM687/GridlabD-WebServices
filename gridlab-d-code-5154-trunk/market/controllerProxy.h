// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifndef _controllerProxy_h_
#define _controllerProxy_h_

#include <staff/client/ServiceClient.h>
#include "controller.h"


//! Proxy for component service controller
class controllerProxy: public controller
{
public:
  controllerProxy();
  virtual ~controllerProxy();
  void Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId);
  void Deinit();
  virtual staff::ServiceClient* GetClient();
  int COinit(double market_init_price, int pstate, int market_period, int control_mode, double psetpoint, double base_setpoint, double heating_base_setpoint, double cooling_base_setpoint);
  int COpresync(double base_setpoint, double psetpoint, int control_mode);
  double COsync(double load, double demand, int pstate, double total, double target, double pAvg, double pStd, double base_setpoint, int control_mode, double market_current_frameclearing_price);
  double COpostsync(int control_mode, double base_setpoint);
private:
  mutable staff::ServiceClient m_tClient;
  std::string m_sServiceUri;
};


namespace staff
{
  class DataObject;
}

#endif

