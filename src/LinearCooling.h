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
	virtual ~LinearCooling();
	double getNewTemperature(int currentGen);
	void initialise(double maximumT, double minimumT, int maximumGenerations);
	double getMaximumTemperature();
	double getMinimumTemperature();
	int getGenerations();
	void setMaximumTemperature(double maxTemp);
	void setMinimumTemperature(double minTemp);
	void setGenerations(int gens);

	double m_maximumTemperature;
	double m_minimumTemperature;
	int m_gens;

private:

};



#endif /* AZIZIADAPTATIVECOOLING_H_ */
