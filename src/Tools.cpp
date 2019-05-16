/* 
 * File:   Tools.cpp
 * Author: mayowaayodele
 *
 * Created on 24 April 2017, 15:32
 */

#define  _CRT_SECURE_NO_WARNINGS
#include "Tools.h"
#include <cstdlib>
#include <cstring>
using namespace std;


Tools::Tools() {
}

Tools::Tools(const Tools& orig) {
}

Tools::~Tools() {
}

double Tools::Mean(int* array, int num) {
    double result = 0;
    for (int i = 0; i < num; ++i)
        result += array[i];
    return result / num;
}

/*
 * Calculates the variance of first num elements of the int array given.
 */
double Tools::Variance(int* array, int num) {
    double avg = Mean(array, num);
    double var = 0;
    for (int i = 0; i < num; ++i) {
        var += ((array [i] - avg) * (array [i] - avg));
    }
    var /= (num - 1);
    return var;
}

/*
 * Calculates the average of first num elements of the doubles array given.
 */
//double Tools::Mean(double* array, int num) {
//    double result = 0.0;
//    for (int i = 0; i < num; ++i){
//        result += array[i];
//    }
//    return (double)((double)result / (double)num);
//}
double Tools::Mean(double* array, int tSize) {
    double sum = 0.0;
    for (int i = 0; i < tSize; i++){
        sum+= array[i];
        
        cout<<sum<<endl;
       
    }
    sum = sum/(double)tSize;
    return sum;
}

/*
 * Calculates the variance of first num elements of the doubles array given.
 */
double Tools::Variance(double* array, int num) {
    double avg = Mean(array, num);
    double var = 0;
    for (int i = 0; i < num; i++) {
        var += ((array [i] - avg) * (array [i] - avg));
    }
    var /= (num - 1);
    return var;
}

void Tools::printarray(int perm[], int length) {
    for (int n = 0; n < length; ++n)
        cout << perm[n] << ' ';
    cout << '\n';
}

void Tools::printarray(double perm[], int length) {
    for (int n = 0; n < length; ++n)
        cout << perm[n] << ' ';
    cout << '\n';
}

/*
 * Prints in standard output 'length' integer elements of a given array.
 */
void Tools::PrintArray(int* array, int length, string text)
{
	cout<<text;
	for (int i=0;i<length;i++){
		cout<<array[i]<<" ";
	}
	cout<<" "<<endl;
}

char* Tools::perm2str(int* p, int n, char* s) {
    int PERMSTR_SIZE = 2048;
    if (!s)
        s = new char[PERMSTR_SIZE];
    sprintf(s, "%d", p[0]);
    char* ptr;
    for (int i = 1; i < n; i++) {
        ptr = s + strlen(s);
        sprintf(ptr, " %d", p[i]);
    }
    return s;
}

char* Tools::perm2str(int* p, int n) {
    int PERMSTR_SIZE = 2048;
       char*   s = new char[PERMSTR_SIZE];
    sprintf(s, "%d", p[0]);
    char* ptr;
    for (int i = 1; i < n; i++) {
        ptr = s + strlen(s);
        sprintf(ptr, " %d", p[i]);
    }
    return s;
}

char* Tools::perm2str(vector<int> p, int n) {
    int PERMSTR_SIZE = 2048;
       char*   s = new char[PERMSTR_SIZE];
    sprintf(s, "%d", p[0]);
    char* ptr;
    for (int i = 1; i < n; i++) {
        ptr = s + strlen(s);
        sprintf(ptr, " %d", p[i]);
    }
    return s;
}

char* Tools::rk2str(double* p, int n) {
    int PERMSTR_SIZE = 2048;
    
     char*   s = new char[PERMSTR_SIZE];
    sprintf(s, "%.10lf", p[0]);
    char* ptr;
    for (int i = 1; i < n; i++) {
        ptr = s + strlen(s);
        sprintf(ptr, "%.10lf", p[i]);
    }
    return s;
}

char* Tools::rk2str(vector<double> p, int n) {
    int PERMSTR_SIZE = 2048;
    
     char*   s = new char[PERMSTR_SIZE];
    sprintf(s, "%.10lf", p[0]);
    char* ptr;
    for (int i = 1; i < n; i++) {
        ptr = s + strlen(s);
        sprintf(ptr, "%.10lf", p[i]);
    }
    return s;
}
char* Tools::double2str(double n, char* s) {
    if (!s)
        s = new char[32]; //should be enough
    sprintf(s, "%.10lf", n); //10 decimal digits
    int i = strlen(s);
    do {
        --i;
    } while (s[i] == '0' && i > 0);
    if (s[i] == '.')
        s[i] = '\0';
    else
        s[i + 1] = '\0';
    return s;
}


int* Tools::randomKeyToAL(double* priorities, int pSize) {
 
    vector<double> p;
    vector<int> AL; 

//     cout << rk2str(priorities, pSize) << endl;
    for (int i = 0; i < pSize; i++) {
        p.push_back(priorities[i]);
    }
    
    sort(p.begin(),p.end()); 
//     cout << rk2str(p, pSize) << endl;

        for (unsigned int i = 0; i < p.size(); i++) {
            for (unsigned int j = 0; j < p.size(); j++) {
                if (((p[i] == priorities[j]) && (AL.empty()== true))|| ((p[i] == priorities[j]) && ((std::find(AL.begin(), AL.end(), j) != AL.end())==false)) ){
                 
                      AL.push_back(j);  
                }
                if (AL.size() ==p.size()) {
                    goto outer;
                }

            }

        }
     
     outer:
 
// cout << perm2str(AL, pSize) << endl;
    int* AL1 = new int[AL.size()];
    for (unsigned int i = 0; i < AL.size(); i++) {
        AL1[i] = AL[i];
    }
    AL.clear();
    //AL.shrink_to_fit(); // ##C++11
	AL.swap(AL); // ##C++0x
    return AL1;
}

bool Tools::strContains(const string inputStr, const string searchStr)
{
	size_t contains;
	
	contains = inputStr.find(searchStr);
	
	if(contains != string::npos)
		return true;
	else
		return false;
}

vector<string> Tools::split(string str)

{

	vector<string> ret;

	if (str.find(' ') == string::npos) ret.push_back(str);

	else

	{

		for (unsigned i = 0; i < str.size(); i++)

		{

			if (str[i] != ' ')

			{

				string word = str.substr(i, str.find(' ', i));

				ret.push_back(word);

			}

		}

	}



	return ret;

}

/*
 * Inverts a permutation.
 */
void Tools::Invert(int*permu, int n, int* inverted)
{
    int i;
    for(i=0; i<n; i++)
        inverted[permu[i]]=i;
}
