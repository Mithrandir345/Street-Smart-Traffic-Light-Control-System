#include "PID.h"

PID::PID()
{
	targetTimeValue = 60000; // 60 seconds
	targetCarCount = 15;
	proportionalTimeGain = 0.9; // 90%
	integrateTimeMax = 6000;
	integrateTimeMin = 1000;
	integrateTimeError = 0;
	integrateTimeGain = 0.9; // 90%
	derivativeTimeGain = 0.9; // 90%
	timeError = 0; //inital time error equal 0
}

PID::~PID()
{

}

void PID::calculateTimeError(unsigned long currentDetectedTime, unsigned long previousDetectTime)
{
	previousTimeError = timeError;
	timeError = (currentDetectedTime - previousDetectTime);
}

void PID::proportionalTimeError()
{
	proportionalTimeValue = proportionalTimeGain * timeError;
}

void PID::derivativeTimeError()
{
	derivativeTimeValue = signed((timeError - previousTimeError));
}

void PID::integrateTimeErrorValue()
{
	integrateTimeError = integrateTimeError + timeError*integrateTimeGain + previousTimeError*integrateTimeGain;
}

void PID::clampIntegratorTimeError()
{
	if (integrateTimeError > integrateTimeMax)
	{
		integrateTimeError = integrateTimeMax;
	}
	else if (integrateTimeError < integrateTimeMin)
	{
		integrateTimeError = -integrateTimeMin;
	}
	integrateTimeValue =  integrateTimeError;
}

unsigned int PID::getTargetCarCount()
{
	return targetCarCount;
}

unsigned long PID::getTargetTimeValue()
{
	return targetTimeValue;
}
signed long PID::getIntegrateTimeValue()
{
	return integrateTimeError;
}
void PID::setTargetTimeValue(unsigned long timeValue)
{
	targetTimeValue = timeValue;
}

signed long PID::getTimeError()
{
	return timeError;
}

void PID::UpdateTimePID()
{
	proportionalTimeError();
	derivativeTimeError();
	clampIntegratorTimeError();
	targetTimeValue =  (proportionalTimeValue + derivativeTimeValue + integrateTimeValue); // update the setpoint time value
}
void PID::UpdateCarPID()
{
	targetCarCount = proportionalCarCount + derivativeCarCount + integrateCarCount; // update the setpoint car count
}
