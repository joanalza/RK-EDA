/*
 * coolingScheme.cpp
 *
 *  Created on: 4 Apr 2018
 *      Author: 1715818
 */



#include <ctime>
#include <string>
#include <iostream>
#include "LinearCooling.h"
#include "math.h"

LinearCooling::LinearCooling(){ }
LinearCooling::LinearCooling(double initialT, int maximumGenerations){
	initialTemperature = initialT;
	currentTemp = initialT;
	maxGens = maximumGenerations;
}

double LinearCooling::getNewTemperature(int currentGen){
	 double c = (1 - ((double) currentGen / maxGens));
	 currentTemp = initialTemperature * c;
	return currentTemp;
}
