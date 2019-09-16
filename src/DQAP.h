#ifndef _DQAP_H__
#define _DQAP_H__

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
using std::stringstream;
using std::string;

class DQAP : public DynPermP
{
	
public:
	
    /*
     * The matrix of distances between the cities.
     */
	int ** m_distance_matrix;
	
    /*
     * The flow matrix.
     */
	int ** m_flow_matrix;
	
	/*
	 * The number of jobs of the problem.
	 */
	int m_size;

	int *m_aux;
	
	/*
     * The constructor. It initializes a QAP from a file.
     */
	DQAP();
	
    /*
     * The destructor.
     */
    virtual ~DQAP();
	
	/*
	 * Read QAP file.
	 */
	int ReadInstance(string filename);
	
	/*
	 * This function evaluates the individuals for the QAP problem.
	 */
	double Evaluate(int * genes);
    
    /*
     * This function evaluates the inverted solution of the given individual for the QAP problem.
     */
    double EvaluateInv(int * genes);
	
    /*
     * Returns the size of the problem.
     */
    int GetProblemSize();
private:
	
};
#endif
