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
LinearCooling::LinearCooling(double initialT, double minT, int maximumGenerations){
	maximumTemperature = initialT;
	minimumTemperature = minT;
	currentTemp = initialT;
	m_gens = maximumGenerations;
}

double LinearCooling::getNewTemperature(int currentGen){
	 double c = (1 - ((double) currentGen / m_gens));
	 currentTemp = (maximumTemperature - minimumTemperature) * c + minimumTemperature;
	return currentTemp;
}
