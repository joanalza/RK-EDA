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
	LinearCooling(double maximumT, double minimumT, int maximumGenerations);
	double getNewTemperature(int currentGen);

	double maximumTemperature;
	double minimumTemperature;
	double currentTemp;
	int m_gens;

private:

};



#endif /* AZIZIADAPTATIVECOOLING_H_ */
