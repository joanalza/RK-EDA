/* 
 * File:   Tools.h
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:33
 */

#ifndef TOOLS_H
#define	TOOLS_H
#include <vector> 
#include <algorithm>    // std::sort
#include <iostream>
#include <stdio.h>
using namespace std;
class Tools {
public:
    Tools();
    Tools(const Tools& orig);
    virtual ~Tools();
    double Mean(int* array, int num);
    double Variance(int* array, int num);
    double Mean(double* array, int num);
    double Variance(double* array, int num);
    void printarray(int perm[], int length);

    void printarray(double perm[], int length);

    char* double2str(double n, char* s);
    char* perm2str(int* p, int n, char* s);
    char* perm2str(int* p, int n);
    char* perm2str(std::vector<int> p, int n);
    char* rk2str(double* p, int n);
    char* rk2str(std::vector<double> p, int n);
    int* randomKeyToAL(double priorities[], int pSize);
    bool strContains(const string inputStr, const string searchStr);
	vector<string> split(string str);

private:

};


#endif	/* TOOLS_H */

