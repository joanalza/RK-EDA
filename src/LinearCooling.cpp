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
	m_maximumTemperature = initialT;
	m_minimumTemperature = minT;
	m_gens = maximumGenerations;
}

LinearCooling::~LinearCooling() { }

double LinearCooling::getNewTemperature(int currentGen){
	 double c = (1 - ((double) currentGen / m_gens));
	 double currentTemp = (m_maximumTemperature - m_minimumTemperature) * c + m_minimumTemperature;
	return currentTemp;
}

void LinearCooling::initialise(double initialT, double minT, int maximumGenerations) {
	m_maximumTemperature = initialT;
	m_minimumTemperature = minT;
	m_gens = maximumGenerations;
}

// Getters and setters
double LinearCooling::getMaximumTemperature(){
	return m_maximumTemperature;
}
double LinearCooling::getMinimumTemperature(){
	return m_minimumTemperature;
}
int LinearCooling::getGenerations(){
	return m_gens;
}
void LinearCooling::setMaximumTemperature(double maxTemp){
	m_maximumTemperature = maxTemp;
}
void LinearCooling::setMinimumTemperature(double minTemp){
	m_minimumTemperature = minTemp;
}
void LinearCooling::setGenerations(int gens){
	m_gens = gens;
}
