#include <cstring>
#include "cansend.h"

Property::Property(float val, float Kp, float Kd, char* can_dev_id) 
{
	_val = val;
	_Kp = Kp;
	_Kd = Kd;
	_can_dev_id = can_dev_id;
}

void Property::calc_reg()
{
	_err = center - pos;
  float d = (_err - _prev_err) * _Kd;
	float p = _Kp * _err;
	float reg = d + p;
	_reg = (int)reg;
	_prev_err = _err;
}

int can_send()
{
	char msg[50], reg_c[20];
	
	sprintf(reg_c,"%d",_reg);
	strcpy(msg,_can_dev_id);
	strcat(msg,"#");
	strcat(msg,reg_c);
	
	return cansend(msg);
}
