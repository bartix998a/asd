#include <iostream>

class uinteger
{
public:
	int val;
	uinteger(int x){
		this->val =x;
	};
	bool operator ==(uinteger& x){
		return this->val == x.val;
	}
	bool operator !=(uinteger& x){
		return !(*this == x);
	}
	uinteger operator +(uinteger& x){
		return *(new uinteger((this->val + x.val) %1000000000));
	}
};
