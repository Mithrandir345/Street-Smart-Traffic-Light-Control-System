#pragma once
class PID
{
public:
	PID();
	~PID();
	void calculateTimeError(unsigned long currentDetectedTime, unsigned long previousDetectTime);
	void proportionalTimeError();
	void derivativeTimeError();
	void integrateTimeErrorValue();
	void clampIntegratorTimeError();

	

	unsigned int getTargetCarCount();
	unsigned long getTargetTimeValue();

	void UpdateTimePID();
	void UpdateCarPID();
private:

	// Time PID data members
	signed long timeError;
	signed long previousTimeError;
	double proportionalTimeGain;
	unsigned long proportionalTimeValue;
	double integrateTimeGain;
	unsigned long integrateTimeMax;
	signed long integrateTimeMin;
	signed long integrateTimeError;
	signed long integrateTimeValue;
	double derivativeTimeGain;
	unsigned long derivativeTimeValue;
	unsigned long timeElapsed;
	unsigned long targetTimeValue;


	// Car PID data members
	unsigned int targetCarCount;
	signed int errorCarCount;
	signed int previousErrorCarCount;
	signed int proportionalCarCount;
	signed int derivativeCarCount;
	signed int integrateCarCount;

};