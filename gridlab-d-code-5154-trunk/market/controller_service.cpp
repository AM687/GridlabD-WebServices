/** $Id: controller_service.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2009 Battelle Memorial Institute
	@file auction.cpp
	@defgroup controller_service Transactive controller_service, OlyPen experiment style
	@ingroup market

 **/
#include "controller_service.h"

#include <memory>
#include <staff/utils/Log.h>
#include <staff/common/logoperators.h>
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>

#include "controller_server.h"

std::auto_ptr< ::controller_server > pcontroller_server(::staff::ServiceFactory::Inst().GetService< ::controller_server >());

CLASS* controller_service::oclass = NULL;

controller_service::controller_service(MODULE *module){
	if (oclass==NULL)
	{
		oclass = gl_register_class(module,"controller_service",sizeof(controller_service),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class controller_service";
		else
			oclass->trl = TRL_QUALIFIED;

		if (gl_publish_variable(oclass,
			PT_enumeration, "simple_mode", PADDR(simplemode),
				PT_KEYWORD, "NONE", (enumeration)SM_NONE,
				PT_KEYWORD, "HOUSE_HEAT", (enumeration)SM_HOUSE_HEAT,
				PT_KEYWORD, "HOUSE_COOL", (enumeration)SM_HOUSE_COOL,
				PT_KEYWORD, "HOUSE_PREHEAT", (enumeration)SM_HOUSE_PREHEAT,
				PT_KEYWORD, "HOUSE_PRECOOL", (enumeration)SM_HOUSE_PRECOOL,
				PT_KEYWORD, "WATERHEATER", (enumeration)SM_WATERHEATER,
				PT_KEYWORD, "DOUBLE_RAMP", (enumeration)SM_DOUBLE_RAMP,
			PT_enumeration, "bid_mode", PADDR(bidmode),
				PT_KEYWORD, "ON", (enumeration)BM_ON,
				PT_KEYWORD, "OFF", (enumeration)BM_OFF,
			PT_enumeration, "use_override", PADDR(use_override),
				PT_KEYWORD, "OFF", (enumeration)OU_OFF,
				PT_KEYWORD, "ON", (enumeration)OU_ON,
			PT_double, "ramp_low[degF]", PADDR(ramp_low), PT_DESCRIPTION, "the comfort response below the setpoint",
			PT_double, "ramp_high[degF]", PADDR(ramp_high), PT_DESCRIPTION, "the comfort response above the setpoint",
			PT_double, "range_low", PADDR(range_low), PT_DESCRIPTION, "the setpoint limit on the low side",
			PT_double, "range_high", PADDR(range_high), PT_DESCRIPTION, "the setpoint limit on the high side",
			PT_char32, "target", PADDR(target), PT_DESCRIPTION, "the observed property (e.g., air temperature)",
			PT_char32, "setpoint", PADDR(setpoint), PT_DESCRIPTION, "the controlled property (e.g., heating setpoint)",
			PT_char32, "demand", PADDR(demand), PT_DESCRIPTION, "the controlled load when on",
			PT_char32, "load", PADDR(load), PT_DESCRIPTION, "the current controlled load",
			PT_char32, "total", PADDR(total), PT_DESCRIPTION, "the uncontrolled load (if any)",
			PT_object, "market", PADDR(pMarket), PT_DESCRIPTION, "the market to bid into",
			PT_char32, "state", PADDR(state), PT_DESCRIPTION, "the state property of the controlled load",
			PT_char32, "avg_target", PADDR(avg_target),
			PT_char32, "std_target", PADDR(std_target),
			PT_double, "bid_price", PADDR(last_p), PT_ACCESS, PA_REFERENCE, PT_DESCRIPTION, "the bid price",
			PT_double, "bid_quantity", PADDR(last_q), PT_ACCESS, PA_REFERENCE, PT_DESCRIPTION, "the bid quantity",
			PT_double, "set_temp[degF]", PADDR(set_temp), PT_ACCESS, PA_REFERENCE, PT_DESCRIPTION, "the reset value",
			PT_double, "base_setpoint[degF]", PADDR(setpoint0),
			// new stuff
			PT_double, "market_price", PADDR(clear_price), PT_DESCRIPTION, "the current market clearing price seen by the controller_service.",
			PT_double, "period[s]", PADDR(dPeriod), PT_DESCRIPTION, "interval of time between market clearings",
			PT_enumeration, "control_mode", PADDR(control_mode),
				PT_KEYWORD, "RAMP", (enumeration)CN_RAMP,
				PT_KEYWORD, "DOUBLE_RAMP", (enumeration)CN_DOUBLE_RAMP,
			PT_enumeration, "resolve_mode", PADDR(resolve_mode),
				PT_KEYWORD, "DEADBAND", (enumeration)RM_DEADBAND,
				PT_KEYWORD, "SLIDING", (enumeration)RM_SLIDING,
			PT_double, "slider_setting",PADDR(slider_setting),
			PT_double, "slider_setting_heat", PADDR(slider_setting_heat),
			PT_double, "slider_setting_cool", PADDR(slider_setting_cool),
			PT_char32, "override", PADDR(re_override),
			// double ramp
			PT_double, "heating_range_high[degF]", PADDR(heat_range_high),
			PT_double, "heating_range_low[degF]", PADDR(heat_range_low),
			PT_double, "heating_ramp_high", PADDR(heat_ramp_high),
			PT_double, "heating_ramp_low", PADDR(heat_ramp_low),
			PT_double, "cooling_range_high[degF]", PADDR(cool_range_high),
			PT_double, "cooling_range_low[degF]", PADDR(cool_range_low),
			PT_double, "cooling_ramp_high", PADDR(cool_ramp_high),
			PT_double, "cooling_ramp_low", PADDR(cool_ramp_low),
			PT_double, "heating_base_setpoint[degF]", PADDR(heating_setpoint0),
			PT_double, "cooling_base_setpoint[degF]", PADDR(cooling_setpoint0),
			PT_char32, "deadband", PADDR(deadband),
			PT_char32, "heating_setpoint", PADDR(heating_setpoint),
			PT_char32, "heating_demand", PADDR(heating_demand),
			PT_char32, "cooling_setpoint", PADDR(cooling_setpoint),
			PT_char32, "cooling_demand", PADDR(cooling_demand),
			PT_double, "sliding_time_delay[s]", PADDR(sliding_time_delay), PT_DESCRIPTION, "time interval desired for the sliding resolve mode to change from cooling or heating to off",
			PT_bool, "use_predictive_bidding", PADDR(use_predictive_bidding),
			// redefinitions
			PT_char32, "average_target", PADDR(avg_target),
			PT_char32, "standard_deviation_target", PADDR(std_target),
#ifdef _DEBUG
			PT_enumeration, "current_mode", PADDR(thermostat_mode),
				PT_KEYWORD, "INVALID", (enumeration)TM_INVALID,
				PT_KEYWORD, "OFF", (enumeration)TM_OFF,
				PT_KEYWORD, "HEAT", (enumeration)TM_HEAT,
				PT_KEYWORD, "COOL", (enumeration)TM_COOL,
			PT_enumeration, "dominant_mode", PADDR(last_mode),
				PT_KEYWORD, "INVALID", (enumeration)TM_INVALID,
				PT_KEYWORD, "OFF", (enumeration)TM_OFF,
				PT_KEYWORD, "HEAT", (enumeration)TM_HEAT,
				PT_KEYWORD, "COOL", (enumeration)TM_COOL,
			PT_enumeration, "previous_mode", PADDR(previous_mode),
				PT_KEYWORD, "INVALID", TM_INVALID,
				PT_KEYWORD, "OFF", (enumeration)TM_OFF,
				PT_KEYWORD, "HEAT", (enumeration)TM_HEAT,
				PT_KEYWORD, "COOL", (enumeration)TM_COOL,
			PT_double, "heat_max", PADDR(heat_max),
			PT_double, "cool_min", PADDR(cool_min),
#endif
			PT_int32, "bid_delay", PADDR(bid_delay),
			NULL)<1) GL_THROW("unable to publish properties in %s",__FILE__);
		memset(this,0,sizeof(controller_service));
	}
}

int controller_service::create(){
	
	return 1;
}

/** provides some easy default inputs for the transactive controller_service,
	 and some examples of what various configurations would look like.
 **/
void controller_service::cheat(int simplemode){
	switch(simplemode){
		case SM_NONE:
			break;
		case SM_HOUSE_HEAT:
			sprintf(target, "air_temperature");
			sprintf(setpoint, "heating_setpoint");
			sprintf(demand, "heating_demand");
			sprintf(total, "total_load");
			sprintf(load, "hvac_load");
			sprintf(state, "power_state");
			ramp_low = -2;
			ramp_high = -2;
			range_low = -5;
			range_high = 0;
			dir = -1;
			break;
		case SM_HOUSE_COOL:
			sprintf(target, "air_temperature");
			sprintf(setpoint, "cooling_setpoint");
			sprintf(demand, "cooling_demand");
			sprintf(total, "total_load");
			sprintf(load, "hvac_load");
			sprintf(state, "power_state");
			ramp_low = 2;
			ramp_high = 2;
			range_low = 0;
			range_high = 5;
			dir = 1;
			break;
		case SM_HOUSE_PREHEAT:
			sprintf(target, "air_temperature");
			sprintf(setpoint, "heating_setpoint");
			sprintf(demand, "heating_demand");
			sprintf(total, "total_load");
			sprintf(load, "hvac_load");
			sprintf(state, "power_state");
			ramp_low = -2;
			ramp_high = -2;
			range_low = -5;
			range_high = 3;
			dir = -1;
			break;
		case SM_HOUSE_PRECOOL:
			sprintf(target, "air_temperature");
			sprintf(setpoint, "cooling_setpoint");
			sprintf(demand, "cooling_demand");
			sprintf(total, "total_load");
			sprintf(load, "hvac_load");
			sprintf(state, "power_state");
			ramp_low = 2;
			ramp_high = 2;
			range_low = -3;
			range_high = 5;
			dir = 1;
			break;
		case SM_WATERHEATER:
			sprintf(target, "temperature");
			sprintf(setpoint, "tank_setpoint");
			sprintf(demand, "heating_element_capacity");
			sprintf(total, "actual_load");
			sprintf(load, "actual_load");
			sprintf(state, "power_state");
			ramp_low = -2;
			ramp_high = -2;
			range_low = 0;
			range_high = 10;
			break;
		case SM_DOUBLE_RAMP:
			sprintf(target, "air_temperature");
			sprintf(heating_setpoint.get_string(), "heating_setpoint");
			sprintf(heating_demand.get_string(), "heating_demand");
			sprintf(heating_total.get_string(), "total_load");		// using total instead of heating_total
			sprintf(heating_load.get_string(), "hvac_load");			// using load instead of heating_load
			sprintf(cooling_setpoint.get_string(), "cooling_setpoint");
			sprintf(cooling_demand.get_string(), "cooling_demand");
			sprintf(cooling_total.get_string(), "total_load");		// using total instead of cooling_total
			sprintf(cooling_load.get_string(), "hvac_load");			// using load instead of cooling_load
			sprintf(deadband.get_string(), "thermostat_deadband");
			sprintf(load, "hvac_load");
			sprintf(total, "total_load");
			heat_ramp_low = -2;
			heat_ramp_high = -2;
			heat_range_low = -5;
			heat_range_high = 5;
			cool_ramp_low = 2;
			cool_ramp_high = 2;
			cool_range_low = 5;
			cool_range_high = 5;
			break;
		default:
			break;
	}
}


/** convenience shorthand
 **/
void controller_service::fetch(double **prop, char *name, OBJECT *parent){
	OBJECT *hdr = OBJECTHDR(this);
	*prop = gl_get_double_by_name(parent, name);
	if(*prop == NULL){
		char tname[32];
		char *namestr = (hdr->name ? hdr->name : tname);
		char msg[256];
		sprintf(tname, "controller_service:%i", hdr->id);
		if(*name == NULL)
			sprintf(msg, "%s: controller_service unable to find property: name is NULL", namestr);
		else
			sprintf(msg, "%s: controller_service unable to find %s", namestr, name);
		throw(msg);
	}
}

/** initialization process
 **/
int controller_service::init(OBJECT *parent){

	OBJECT *hdr = OBJECTHDR(this);
	char tname[32];
	char *namestr = (hdr->name ? hdr->name : tname);
//	double high, low;

	sprintf(tname, "controller_service:%i", hdr->id);

	cheat(5);

	if(parent == NULL){
		gl_error("%s: controller_service has no parent, therefore nothing to control", namestr);
		return 0;
	}

	if(pMarket == NULL){
		gl_error("%s: controller_service has no market, therefore no price signals", namestr);
		return 0;
	}

	if(gl_object_isa(pMarket, "auction")){
		gl_set_dependent(hdr, pMarket);
		market = OBJECTDATA(pMarket, auction);
	} else {
		gl_error("controller_services only work when attached to an 'auction' object");
		return 0;
	}

	if(dPeriod == 0.0){
		if((pMarket->flags & OF_INIT) != OF_INIT){
			char objname[256];
			gl_verbose("controller_service::init(): deferring initialization on %s", gl_name(pMarket, objname, 255));
			return 2; // defer
		}
		period = market->period;
	} else {
		period = (TIMESTAMP)floor(dPeriod + 0.5);
	}



	if(bid_delay < 0){
		bid_delay = -bid_delay;
	}
	if(bid_delay > period){
		gl_warning("Bid delay is greater than the controller_service period. Resetting bid delay to 0.");
		bid_delay = 0;
	}

	if(target[0] == 0){
		GL_THROW("controller_service: %i, target property not specified", hdr->id);
	}
	if(setpoint[0] == 0 && control_mode == CN_RAMP){
		GL_THROW("controller_service: %i, setpoint property not specified", hdr->id);;
	}
	if(demand[0] == 0 && control_mode == CN_RAMP){
		GL_THROW("controller_service: %i, demand property not specified", hdr->id);
	}
	if(deadband[0] == 0 && use_predictive_bidding == TRUE && control_mode == CN_RAMP){
		GL_THROW("controller_service: %i, deadband property not specified", hdr->id);
	}
	if(total[0] == 0){
		GL_THROW("controller_service: %i, total property not specified", hdr->id);
	}
	if(load[0] == 0){
		GL_THROW("controller_service: %i, load property not specified", hdr->id);
	}

	if(heating_setpoint[0] == 0 && control_mode == CN_DOUBLE_RAMP){
		GL_THROW("controller_service: %i, heating_setpoint property not specified", hdr->id);;
	}
	if(heating_demand[0] == 0 && control_mode == CN_DOUBLE_RAMP){
		GL_THROW("controller_service: %i, heating_demand property not specified", hdr->id);
	}

	if(cooling_setpoint[0] == 0 && control_mode == CN_DOUBLE_RAMP){
		GL_THROW("controller_service: %i, cooling_setpoint property not specified", hdr->id);;
	}
	if(cooling_demand[0] == 0 && control_mode == CN_DOUBLE_RAMP){
		GL_THROW("controller_service: %i, cooling_demand property not specified", hdr->id);
	}

	if(deadband[0] == 0 && control_mode == CN_DOUBLE_RAMP){
		GL_THROW("controller_service: %i, deadband property not specified", hdr->id);
	}

		if(state[0] != 0){
		// grab state pointer
		pState = gl_get_enum_by_name(parent, state);
		last_pState = 0;
		if(pState == 0){
			gl_error("state property name \'%s\' is not published by parent class", state);
			return 0;
		}
	}



	fetch(&pMonitor, target, parent);
	if(control_mode == 0){
		fetch(&pSetpoint, setpoint, parent);
		fetch(&pDemand, demand, parent);
		fetch(&pTotal, total, parent);
		fetch(&pLoad, load, parent);
		if(use_predictive_bidding == TRUE){
			fetch(&pDeadband, deadband.get_string(), parent);
		}
fetch(&pAvg, avg_target.get_string(), pMarket);
	fetch(&pStd, std_target.get_string(), pMarket);
	}
		
		//CALL INTI METHOD OF WEB SERIVCE
	//input parameters
	//market_init_price= market->init_price, 
	//pstate=pState;
	//market_period=market->period, 
	//control_mode=control_mode;
	//psetpoint=*pSetpoint,
	//base_setpoint=setpoint0,
	//heating_base_setpoint=heating_setpoint0,
	//cooling_base_setpoint=cooling_setpoint0
	//return value on last_p
	double market_init_price = market->init_price;
    int pstate = 1;
    int market_period = market->period;
    int control_mode = 1;
    double psetpoint = *pSetpoint;
    double base_setpoint = setpoint0;
    double heating_base_setpoint = heating_setpoint0;
    double cooling_base_setpoint = cooling_setpoint0;
    int COinitResult = pcontroller_server->COinit(market_init_price, pstate, market_period, control_mode, psetpoint, base_setpoint, heating_base_setpoint, cooling_base_setpoint);
    // staff::LogInfo() << "COinit result: " << COinitResult;
	printf("COinitResult = %d \n", COinitResult);





	return 1;
}


int controller_service::isa(char *classname)
{
	return strcmp(classname,"controller_service")==0;
}


TIMESTAMP controller_service::presync(TIMESTAMP t0, TIMESTAMP t1){

	//CALL PRESYNC METHOD FOR WEB SERVICE
	//input parameters
	//base_setpoint=setpoint0;
	//control_mode=control_mode;
	//psetpoint=*pSetpoint;
	double base_setpoint = setpoint0;
    double psetpoint = *pSetpoint;
    int control_mode = 1;
    int COpresyncResult = pcontroller_server->COpresync(base_setpoint, psetpoint, control_mode);
    //staff::LogInfo() << "COpresync result: " << COpresyncResult;
	printf("COpresyncResult = %d \n", COpresyncResult);

	

	return TS_NEVER;
}

TIMESTAMP controller_service::sync(TIMESTAMP t0, TIMESTAMP t1){



	//CALL SYNC METHOD OF WEB SERVICE
	//INPUT PARAMETERS
	
	//load=*pLoad
	//demand=*pDemand;
	//pstate=pState,
	//total=*pTotal
	//target=*pMonitor
	//pAvg=*pAvg
	//pStd=*pStd
	//base_setpoint=*pSetpoint
	//control_mode=control_mode
	//market_current_frameclearing_price=market->current_frame.clearing_price
	//return value on last_p variable
	double load = *pLoad;
    double demand = *pDemand;
    int pstate = *pState;
    double total = *pTotal;
    double target = *pMonitor;
    double pAvg2 = *pAvg;
    double pStd2 = *pStd;
    double base_setpoint = *pSetpoint;
    int control_mode = 1;
    double market_current_frameclearing_price = market->current_frame.clearing_price;
    double COsyncResult = pcontroller_server->COsync(load, demand, pstate, total, target, pAvg2, pStd2, base_setpoint, control_mode, market_current_frameclearing_price);
    // staff::LogInfo() << "COsync result: " << COsyncResult;
	printf("COsyncResult = %f \n", COsyncResult);
	//test return
	last_p = COsyncResult;
	
	KEY bid = (KEY)(lastmkt_id == market->market_id ? lastbid_id : -1);
				lastbid_id = submit_bid(this->pMarket, OBJECTHDR(this), -last_q, last_p, bid);
	return TS_NEVER;
}

TIMESTAMP controller_service::postsync(TIMESTAMP t0, TIMESTAMP t1){


	
	TIMESTAMP rv = next_run - bid_delay;


	//CALL POSTYNC METHOD of web service 
	//input parameters
	//control_mode=control_mode;
	//base_setpount=setpoint0;
	int control_mode = 1;
    double base_setpoint = setpoint0;
    double COpostsyncResult = pcontroller_server->COpostsync(control_mode, base_setpoint);
    // staff::LogInfo() << "COpostsync result: " << COpostsyncResult;
	printf("COpostsyncResult = %f \n", COpostsyncResult);


	if(resolve_mode == RM_SLIDING){
		if(*pHeatState == 1 || *pAuxState == 1){
			thermostat_mode = TM_HEAT;
			if(last_mode == TM_OFF)
				time_off = TS_NEVER;
		} else if(*pCoolState == 1){
			thermostat_mode = TM_COOL;
			if(last_mode == TM_OFF)
				time_off = TS_NEVER;
		} else if(*pHeatState == 0 && *pAuxState == 0 && *pCoolState == 0){
			thermostat_mode = TM_OFF;
			if(previous_mode != TM_OFF)
				time_off = t1 + dtime_delay;
		} else {
			gl_error("The HVAC is in two or more modes at once. This is impossible");
			if(resolve_mode == RM_SLIDING)
				return TS_INVALID; // If the HVAC is in two modes at once then the sliding resolve mode will have conflicting state input so stop the simulation.
		}
	}

	if (t1 - next_run < bid_delay){
		rv = next_run;
	}

	if(next_run == t1){
		next_run += (TIMESTAMP)(this->period);
		rv = next_run - bid_delay;
	}

	return rv;
}

//////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF CORE LINKAGE
//////////////////////////////////////////////////////////////////////////

EXPORT int create_controller_service(OBJECT **obj, OBJECT *parent)
{
	try
	{
		*obj = gl_create_object(controller_service::oclass);
		if (*obj!=NULL)
		{
			controller_service *my = OBJECTDATA(*obj,controller_service);
			gl_set_parent(*obj,parent);
			return my->create();
		}
		else
			return 0;
	}
	CREATE_CATCHALL(controller_service);
}

EXPORT int init_controller_service(OBJECT *obj, OBJECT *parent)
{
	try
	{
		if (obj!=NULL)
		{
			return OBJECTDATA(obj,controller_service)->init(parent);
		}
		else
			return 0;
	}
	INIT_CATCHALL(controller_service);
}

EXPORT int isa_controller_service(OBJECT *obj, char *classname)
{
	if(obj != 0 && classname != 0){
		return OBJECTDATA(obj,controller_service)->isa(classname);
	} else {
		return 0;
	}
}

EXPORT TIMESTAMP sync_controller_service(OBJECT *obj, TIMESTAMP t1, PASSCONFIG pass)
{
	TIMESTAMP t2 = TS_NEVER;
	controller_service *my = OBJECTDATA(obj,controller_service);
	try
	{
		switch (pass) {
		case PC_PRETOPDOWN:
			t2 = my->presync(obj->clock,t1);
			//obj->clock = t1;
			break;
		case PC_BOTTOMUP:
			t2 = my->sync(obj->clock, t1);
			//obj->clock = t1;
			break;
		case PC_POSTTOPDOWN:
			t2 = my->postsync(obj->clock,t1);
			obj->clock = t1;
			break;
		default:
			gl_error("invalid pass request (%d)", pass);
			return TS_INVALID;
			break;
		}
		return t2;
	}
	SYNC_CATCHALL(controller_service);
}

// EOF
