#include "PD.hpp"

Property::Property(float val, float Kp, float Kd, char* can_device_id) 
{
	_val = val;
	_Kp = Kp;
	_Kd = Kd;
	_can_device_id = can_device_id
}

void Property::change_err(int new_err)
{
	_err = new_err;
}

void Property::calc_reg()
{
	_err = center - pos;
	_reg = PD(_err,_Kp,_Kd);
	_last_err = _err;
}

void can_send(int reg)
{
	char* reg_c = (char*)
	char* msg = _can_device_id + '#' + (char*)reg;
	cansend(msg);
}
