#include "parameterSmoother.h"



parameterSmoother::parameterSmoother(float smoothingTimeMS, int sampleRate) {
	a = exp(-TWO_PI / (smoothingTimeMS * 0.001 * sampleRate));
	b = 1.0 - a;
	z = 0.0;

}

float parameterSmoother::process(float input) {
	z = (input * b) + (z * a);
	return z;
}