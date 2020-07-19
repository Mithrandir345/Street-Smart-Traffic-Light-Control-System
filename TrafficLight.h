#pragma once
#include <Arduino.h>
#include "PID.h"
class TrafficLight : public PID
{
public:
	TrafficLight();
	~TrafficLight();
	void setLightPinOut(uint8_t mode, int redPin, int greenPin);
	void setLightColor(int redValue, int greenValue);
	void TrafficLightControl(unsigned long currentTime, unsigned long flashingInterval);
	enum lightingState { GREEN, YELLOW, RED, FLASHINGRED };
	enum cardinalState { NORTH, EAST, SOUTH, WEST };
	lightingState lightState;
	cardinalState directionState;
	void flashingRed(unsigned long currentTime, unsigned long flashingInterval);
private:
	int redPin, greenPin, bluePin;
	bool isLedOn;
	unsigned long previousTime = 0;
	unsigned int carCount = 0;
	unsigned long currentTime = 0;
	unsigned long timeSinceArduinoTurnedOn = 0;
	unsigned long detectedTime = 0;
	unsigned long previousDetectedTime = 0;
	unsigned long timeSinceGreen = 0;
	unsigned long timeSinceYellow = 0;

	
};