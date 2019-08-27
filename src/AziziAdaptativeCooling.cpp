/*
 * coolingScheme.cpp
 *
 *  Created on: 4 Apr 2018
 *      Author: 1715818
 */

#include <ctime>
#include <string>
#include <iostream>
#include "AziziAdaptativeCooling.h"
#include "math.h"

AziziAdaptativeCooling::AziziAdaptativeCooling(){ }
AziziAdaptativeCooling::AziziAdaptativeCooling(double minTemp, double heating){
	minTemperature = minTemp;
	currentTemp = minTemperature;
	heatingCoefficient = heating;
}
AziziAdaptativeCooling::~AziziAdaptativeCooling() { }

double AziziAdaptativeCooling::getNewTemperature(int noImprovementCounter){
	currentTemp = minTemperature + (heatingCoefficient * log(1+noImprovementCounter));
	return currentTemp;
}
