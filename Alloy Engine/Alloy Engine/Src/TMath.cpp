#include "TMath.h"

const float TMath::PI = 3.14159265358979323846;

float TMath::Radians(const float _degrees)
{
	return _degrees * PI / 180;
}

float TMath::Degrees(const float _radians)
{
	return _radians * 180 / PI;
}
