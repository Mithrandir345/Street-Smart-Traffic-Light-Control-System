#include "TrafficLight.h"

TrafficLight::TrafficLight()
{
}

TrafficLight::~TrafficLight()
{

}
void TrafficLight::setLightPinOut(uint8_t mode, int redPin, int greenPin, int bluePin)
{
	this->redPin = redPin;
	this->greenPin = greenPin;
	this->bluePin = bluePin;
	pinMode(redPin, mode);
	pinMode(greenPin, mode);
	pinMode(bluePin, mode);
}

void TrafficLight::setLightColor(int redValue, int greenValue, int blueValue) {
	analogWrite(redPin, redValue);
	analogWrite(greenPin, greenValue);
	analogWrite(bluePin, blueValue);
}

void TrafficLight::TrafficLightControl(unsigned long currentTime, unsigned long flashingInterval)
{
	switch (directionState)
	{
	case NORTH:
		switch (lightState)
		{
		case GREEN:
			setLightColor(0, 255, 0);
			break;
		case YELLOW:
			setLightColor(255, 60, 0);
			break;
		case RED:
			setLightColor(255, 0, 0);
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
			break;
		case YELLOW:
			break;
		case RED:
			break;
		case FLASHINGRED:
			break;
		default:
			break;
		}
		break;
	case SOUTH:
		switch (lightState)
		{
		case GREEN:
			break;
		case YELLOW:
			break;
		case RED:
			break;
		case FLASHINGRED:
			break;
		default:
			break;
		}
		break;
	case WEST:
		switch (lightState)
		{
		case GREEN:
			break;
		case YELLOW:
			break;
		case RED:
			break;
		case FLASHINGRED:
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
			setLightColor(255, 0, 0);
			isLedOn = false;
		}
		else
		{
			setLightColor(0, 0, 0);
			isLedOn = true;
		}
	}

}


