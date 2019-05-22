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
#include <bits/stdc++.h>
using namespace std;
class Tools {
public:
    Tools();
    Tools(const Tools& orig);
    virtual ~Tools();
    static double Mean(int* array, int num);
    static double Variance(int* array, int num);
    static double Mean(double* array, int num);
    static double Variance(double* array, int num);
    static void printarray(int perm[], int length);
    static void printarray(double perm[], int length);
    static void PrintArray(int* array, int length, string text);
    static char* double2str(double n, char* s);
    static char* perm2str(int* p, int n, char* s);
    static char* perm2str(int* p, int n);
    static char* perm2str(std::vector<int> p, int n);
    static char* rk2str(double* p, int n);
    static char* rk2str(std::vector<double> p, int n);
    static int* randomKeyToAL(double priorities[], int pSize);
    static bool strContains(const string inputStr, const string searchStr);
    static vector<string> split(string str);
    static void Invert(int*permu, int n, int* inverted);
    static bool areEqual(double *arr1, double *arr2, int size1, int size2);
    static bool areEqual(int *arr1, int *arr2, int size1, int size2);

private:

};


#endif	/* TOOLS_H */

