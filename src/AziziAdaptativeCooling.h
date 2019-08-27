/*
 * coolingScheme.h
 *
 *  Created on: 4 Apr 2018
 *      Author: 1715818
 */

#ifndef AZIZIADAPTATIVECOOLING_H_
#define AZIZIADAPTATIVECOOLING_H_

class AziziAdaptativeCooling {
public:
	AziziAdaptativeCooling();
	AziziAdaptativeCooling(double minTemp, double heating);
	virtual ~AziziAdaptativeCooling();
	double getNewTemperature(int currentGen);

	double currentTemp;
	double minTemperature;
	double heatingCoefficient;

private:

};



#endif /* AZIZIADAPTATIVECOOLING_H_ */
