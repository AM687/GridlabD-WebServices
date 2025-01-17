// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#include <memory>
#include <staff/utils/SharedPtr.h>
#include <staff/utils/Log.h>
#include <staff/utils/fromstring.h>
#include <staff/utils/tostring.h>
#include <staff/utils/HexBinary.h>
#include <staff/utils/Base64Binary.h>
#include <staff/common/DataObject.h>
#include <staff/common/Attribute.h>
#include <staff/common/Operation.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#include <staff/client/IProxyAllocator.h>
#include <staff/client/ICallback.h>
#include <staff/client/Options.h>
#include "wheaterProxy.h"

namespace staff
{

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// classes implementation



// proxy allocator
class wheaterProxyAllocator: public staff::IProxyAllocator
{
public:
  wheaterProxyAllocator()
  {
    try
    {
      staff::ServiceFactory::Inst().RegisterProxyAllocator(typeid(wheater).name(), *this);
    }
    STAFF_CATCH_ALL_DESCR("Failed to register proxy allocator wheater");
  }

  virtual staff::IService* AllocateProxy(const std::string& sServiceUri,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    std::auto_ptr<wheaterProxy> tpProxy(new wheaterProxy);
    tpProxy->Init(sServiceUri, sSessionId, sInstanceId);
    return tpProxy.release();
  }

  virtual staff::IService* AllocateProxy(const std::string& sBaseUri,
                                         const std::string& sServiceName,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    std::auto_ptr<wheaterProxy> tpProxy(new wheaterProxy);
    tpProxy->Init(sBaseUri + (sServiceName.empty() ? "wheater" : sServiceName),
                  sSessionId, sInstanceId);
    return tpProxy.release();
  }
};

wheaterProxyAllocator twheaterProxyAllocatorInitializer;

// asynch proxies


// service proxy
wheaterProxy::wheaterProxy()
{
}

wheaterProxy::~wheaterProxy()
{
  try
  {
    Deinit();
  }
  STAFF_CATCH_ALL;
}

void wheaterProxy::Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId)
{
  staff::IService::Init("wheater", sSessionId, sInstanceId);
  m_sServiceUri = !sServiceUri.empty() ? sServiceUri : "http://10.96.12.46:9090/axis2/services/wheater";
  m_tClient.Init(m_sServiceUri);
  staff::Options& rOptions = m_tClient.GetOptions();
  rOptions.SetDefaultNamespace("http://tempui.org/", "");
  rOptions.SetSoapVersion(staff::Options::Soap11);

  rOptions.SetSessionId(sSessionId);

  if (!sInstanceId.empty())
  {
    staff::Operation tOperation("CreateInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(sInstanceId);
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                             tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
      STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " +
                  tOperation.GetResponse().ToString()); // other fault
    }
    rOptions.SetInstanceId(sInstanceId);
  }
}

void wheaterProxy::Deinit()
{
  if (!staff::IService::GetInstanceId().empty())
  {
    staff::Operation tOperation("FreeInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(staff::IService::GetInstanceId());
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                             tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
      STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " +
                  tOperation.GetResponse().ToString()); // other fault
    }
  }
}

staff::ServiceClient* wheaterProxy::GetClient()
{
  return &m_tClient;
}


int wheaterProxy::WHcreate()
{
  staff::Operation tOperation("WHcreate", "WHcreateResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

int wheaterProxy::WHinit()
{
  staff::Operation tOperation("WHinit", "WHinitResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

int wheaterProxy::WHsync(int t0, int t1, double heating_element_capacity, double temperature, double tank_setpoint, double tank_UA, double water_demand, double tank_volume, double thermostat_deadband)
{
  staff::Operation tOperation("WHsync", "WHsyncResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("t0", t0);
  rdoRequest.CreateChild("t1", t1);
  rdoRequest.CreateChild("heating_element_capacity", heating_element_capacity);
  rdoRequest.CreateChild("temperature", temperature);
  rdoRequest.CreateChild("tank_setpoint", tank_setpoint);
  rdoRequest.CreateChild("tank_UA", tank_UA);
  rdoRequest.CreateChild("water_demand", water_demand);
  rdoRequest.CreateChild("tank_volume", tank_volume);
  rdoRequest.CreateChild("thermostat_deadband", thermostat_deadband);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

int wheaterProxy::WHcommit()
{
  staff::Operation tOperation("WHcommit", "WHcommitResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

int wheaterProxy::WHtank_state()
{
  staff::Operation tOperation("WHtank_state", "WHtank_stateResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

int wheaterProxy::WHpostsync()
{
  staff::Operation tOperation("WHpostsync", "WHpostsyncResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

void wheaterProxy::WHset_time_to_transition()
{
  staff::Operation tOperation("WHset_time_to_transition", "WHset_time_to_transitionResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }
}

int wheaterProxy::WHset_current_model_and_load_state()
{
  staff::Operation tOperation("WHset_current_model_and_load_state", "WHset_current_model_and_load_stateResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  int tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHdhdt(double h)
{
  staff::Operation tOperation("WHdhdt", "WHdhdtResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("h", h);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHactual_kW()
{
  staff::Operation tOperation("WHactual_kW", "WHactual_kWResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHnew_time_1node(double T0, double T1)
{
  staff::Operation tOperation("WHnew_time_1node", "WHnew_time_1nodeResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("T0", T0);
  rdoRequest.CreateChild("T1", T1);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHnew_temp_1node(double T0, double delta_t)
{
  staff::Operation tOperation("WHnew_temp_1node", "WHnew_temp_1nodeResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("T0", T0);
  rdoRequest.CreateChild("delta_t", delta_t);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHnew_time_2zone(double h0, double h1)
{
  staff::Operation tOperation("WHnew_time_2zone", "WHnew_time_2zoneResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("h0", h0);
  rdoRequest.CreateChild("h1", h1);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHnew_h_2zone(double h0, double delta_t)
{
  staff::Operation tOperation("WHnew_h_2zone", "WHnew_h_2zoneResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  rdoRequest.CreateChild("h0", h0);
  rdoRequest.CreateChild("delta_t", delta_t);

  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}

double wheaterProxy::WHget_Tambient()
{
  staff::Operation tOperation("WHget_Tambient", "WHget_TambientResult");
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("http://tempui.org/");
  // synchronous call
  tOperation.SetResponse(m_tClient.Invoke(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
  double tReturn = 0;
  rdoResult.GetValue(tReturn);
  return tReturn;
}


