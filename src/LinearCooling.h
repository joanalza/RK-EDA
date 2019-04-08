/*
 * coolingScheme.h
 *
 *  Created on: 4 Apr 2018
 *      Author: 1715818
 */

#ifndef LINEARCOOLING_H_
#define LINEARCOOLING_H_

class LinearCooling {
public:
	LinearCooling();
	LinearCooling(double initialT, int maximumGenerations);
	double getNewTemperature(int currentGen);

	double initialTemperature;
	double currentTemp;
	int maxGens;

private:

};



#endif /* AZIZIADAPTATIVECOOLING_H_ */
