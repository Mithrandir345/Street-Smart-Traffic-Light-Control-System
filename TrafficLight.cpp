#include "TrafficLight.h"

TrafficLight::TrafficLight()
{
}

TrafficLight::~TrafficLight()
{

}
void TrafficLight::setLightPinOut(uint8_t mode, int redPin, int greenPin)
{
	this->redPin = redPin;
	this->greenPin = greenPin;
	pinMode(redPin, mode);
	pinMode(greenPin, mode);
}

void TrafficLight::setLightColor(int redValue, int greenValue) {
	digitalWrite(redPin, redValue);
	analogWrite(greenPin, greenValue);
}

void TrafficLight::TrafficLightControl(unsigned long currentTime, unsigned long flashingInterval)
{
	switch (directionState)
	{
	case NORTH:
		switch (lightState)
		{
		case GREEN:
			setLightColor(LOW, 255);
			break;
		case YELLOW:
			setLightColor(HIGH, 60);
			break;
		case RED:
			setLightColor(HIGH, 0);
			break;
		case FLASHINGRED:
			flashingRed(currentTime, flashingInterval);
			break;
		default:
			break;
		}
		break;
	case EAST:
		switch (lightState)
		{
		case GREEN:
			setLightColor(LOW, 255);
			break;
		case YELLOW:
			setLightColor(HIGH, 60);
			break;
		case RED:
			setLightColor(HIGH, 0);
			break;
		case FLASHINGRED:
			flashingRed(currentTime, flashingInterval);
			break;
		default:
			break;
		}
		break;
	case SOUTH:
		switch (lightState)
		{
		case GREEN:
			setLightColor(LOW, 255);
			break;
		case YELLOW:
			setLightColor(HIGH, 60);
			break;
		case RED:
			setLightColor(HIGH, 0);
			break;
		case FLASHINGRED:
			flashingRed(currentTime, flashingInterval);
			break;
		default:
			break;
		}
		break;
	case WEST:
		switch (lightState)
		{
		case GREEN:
			setLightColor(LOW, 255);
			break;
		case YELLOW:
			setLightColor(HIGH, 60);
			break;
		case RED:
			setLightColor(HIGH, 0);
			break;
		case FLASHINGRED:
			flashingRed(currentTime, flashingInterval);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void TrafficLight::flashingRed(unsigned long currentTime,  unsigned long flashingInterval)
{
	if (currentTime - previousTime > flashingInterval)
	{
		this->previousTime = currentTime;
		if (isLedOn)
		{
			setLightColor(HIGH, 0);
			isLedOn = false;
		}
		else
		{
			setLightColor(LOW, 0);
			isLedOn = true;
		}
	}

}


