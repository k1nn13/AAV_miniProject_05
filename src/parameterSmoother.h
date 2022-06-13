#pragma once
#include "ofMain.h"

class parameterSmoother
{

public:
	parameterSmoother(float smoothTimeMS, int sampleRate);
	float process(float input);


private:
	float a;
	float b;
	float z;


};

struct smoothValue {
	float targetValue;
	float currentValue;
	parameterSmoother* smoother;
};