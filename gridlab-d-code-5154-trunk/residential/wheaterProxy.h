// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifndef _wheaterProxy_h_
#define _wheaterProxy_h_

#include <staff/client/ServiceClient.h>
#include "wheater.h"


//! Proxy for component service wheater
class wheaterProxy: public wheater
{
public:
  wheaterProxy();
  virtual ~wheaterProxy();
  void Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId);
  void Deinit();
  virtual staff::ServiceClient* GetClient();
  int WHcreate();
  int WHinit();
  int WHsync(int t0, int t1, double heating_element_capacity, double temperature, double tank_setpoint, double tank_UA, double water_demand, double tank_volume, double thermostat_deadband);
  int WHcommit();
  int WHtank_state();
  int WHpostsync();
  void WHset_time_to_transition();
  int WHset_current_model_and_load_state();
  double WHdhdt(double h);
  double WHactual_kW();
  double WHnew_time_1node(double T0, double T1);
  double WHnew_temp_1node(double T0, double delta_t);
  double WHnew_time_2zone(double h0, double h1);
  double WHnew_h_2zone(double h0, double delta_t);
  double WHget_Tambient();
private:
  mutable staff::ServiceClient m_tClient;
  std::string m_sServiceUri;
};


namespace staff
{
  class DataObject;
}

#endif

