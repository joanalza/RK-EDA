#ifndef _DTSP_H__
#define _DTSP_H__

#include "Tools.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <stdio.h>

#include "DynPermP.h"
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using namespace std;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::string;

class DTSP : public DynPermP
{
	
public:
	
    /*
     * Matrix of distances between the cities.
     */
	int ** m_distance_matrix;
	
	/*
	 * The number of cities.
	 */
	int m_size;

	int *m_aux;
	
	/*
     * The constructor.
     */
	DTSP();
	
    /*
     * The destructor.
     */
    virtual ~DTSP();
	
	/*
	 * Read TSP instance file that belongs to the TSPLIB library.
	 */
	int ReadTSPLIB(string filename);

    /*
	 * Read TSP instance file.
	 */
	int ReadInstance(string filename);
    
	/*
	 * This function evaluates the m_fitness of the solution for the TSP problem.
	 */
	double Evaluate(int * genes);

    /*
	 * This function evaluates the inverted solution of the given solution for the TSP problem.
	 */
	double EvaluateInv(int * genes);

	double CalculateGEODistance(double latitudeX, double latitudeY, double longitudeX, double longitudeY);
    
    /*
     * Returns the size of the problem.
     */
    int GetProblemSize();
private:
	
};
#endif