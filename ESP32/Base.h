// Base.h

#ifndef _BASE_h
#define _BASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Base
{
public:
	Base();
	~Base();

	virtual void init() = 0;
	virtual void loop() = 0;
	virtual bool check() = 0;
	bool Enabled;
};


#endif

