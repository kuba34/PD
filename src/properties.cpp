#include "PD.hpp"


Property::Property(float x, float Kp, float Kd) 
{
	_val=val;
	_Kp=Kp;
	_Kd=Kd;
}

void Property::change_err(int new_err)
{
	_err = new_err;
}

void Property::calc_reg()
{
	_err = center - pos;
	_reg=PD(_err,_Kp,_Kd);
	_last_err = _err;
}
