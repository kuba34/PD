
class Property
{
	int _reg, _err, _last_err=0;
	float _val, _Kp, _Kd;
	char* _can_device_id;
	
	public:
	Property(float val, float Kp, float Kd, char* can_device_id);
	void change_err(int new_err);
	void calc_reg();
	float get_val() const {return _val;}
	float get_val() {return _val;}
	int get_reg() const {return _reg;}
	void parse_msg(int reg);
};
