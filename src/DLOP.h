#ifndef _LOP_H__
#define _LOP_H__

#include "Tools.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
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

class DLOP : public DynPermP
{
	
public:
	
    /*
     * Entries matrix of the LOP.
     */
	int ** m_matrix;
    
	/*
	 * The size of the problem.
	 */
	int m_problemsize;

	int* m_aux;
		

    /*
     * The constructor.
     */
	DLOP();
	
    /*
     * The destructor.
     */
    virtual ~DLOP();
	
	/*
	 * Read LOP instance file.
	 */
	int ReadInstance(string filename);
	
	/*
	 * This function evaluates the solution for the LOP problem.
	 */
	double Evaluate(int * genes);
    
    /*
     * This function evaluates the inverted solution of the given solution for the LOP problem.
     */
    double EvaluateInv(int * genes);
	
    /*
     * Returns the size of the problem.
     */
    int GetProblemSize();
    
private:
    
};
#endif





