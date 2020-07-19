#include "PID.h"


PID::PID()
{
	targetTimeValue = 10000; // 10 seconds
	targetCarCount = 5;
	proportionalTimeGain = 0.1; // 10%
	integrateTimeMax = 10000;
	integrateTimeMin = 6000;
	integrateTimeError = 0;
	derivativeTimeGain = 0.1; // 10%
	
}

PID::~PID()
{

}

void PID::calculateTimeError(unsigned long currentDetectedTime, unsigned long previousDetectTime)
{
	timeElapsed += (currentDetectedTime - previousDetectTime);
	timeError = targetTimeValue - timeElapsed;
}

void PID::proportionalTimeError()
{
	proportionalTimeValue = proportionalTimeGain * timeError;
}

void PID::derivativeTimeError()
{
	derivativeTimeValue = (timeError - previousTimeError) / (timeElapsed);
}

void PID::integrateTimeErrorValue()
{
	integrateTimeError += timeError;
}

void PID::clampIntegratorTimeError()
{
	if (integrateTimeError > integrateTimeMax)
	{
		integrateTimeError = integrateTimeMax;
	}
	else if (integrateTimeError < integrateTimeMin)
	{
		integrateTimeError = integrateTimeMin;
	}
	integrateTimeValue = (integrateTimeGain * integrateTimeError);
}

unsigned int PID::getTargetCarCount()
{
	return targetCarCount;
}

unsigned long PID::getTargetTimeValue()
{
	return targetTimeValue;
}

void PID::UpdateTimePID()
{
	targetTimeValue =  proportionalTimeValue + derivativeTimeValue + integrateTimeValue; // update the setpoint time value
}
void PID::UpdateCarPID()
{
	targetCarCount = proportionalCarCount + derivativeCarCount + integrateCarCount; // update the setpoint car count
}
