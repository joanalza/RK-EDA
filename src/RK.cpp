//#include "stdafx.h"
#include <iostream>
using namespace std;
#include "RK.h"
#include "Tools.h"



//int* m_permutation= new int[IND_SIZE];

RK::RK(){

}

RK::RK(double randomkeys1[], int psize) {
	m_pSize = psize;
	m_randomkeys = new double[m_pSize];
	for (int i = 0; i < psize; i++) 
		m_randomkeys[i] = randomkeys1[i];
	m_permutation = new int[m_pSize];
	setPermFromRKs();
}

RK::~RK()
{
	delete[] m_randomkeys;
	delete[] m_permutation;

}

RK RK::copyOf() {

	RK rk1(m_randomkeys, m_pSize);
	rk1.setFitness(m_fitness);
	rk1.setPermutation(m_permutation);
	return rk1;
}

RK* RK::Clone()
{
	//RK rk1(m_randomkeys, m_pSize);
	RK*  ind = new RK(m_randomkeys, m_pSize);
	ind->setFitness(m_fitness);
	ind->setPermutation(m_permutation);
	return ind;
}

void RK::deepCopy(RK* obj1)
{
	setProblemSize(obj1->getProblemSize());
	setPermutation(obj1->getPermutation());
	setRandomKeys(obj1->getRandomKeys());
	setFitness(obj1->getFitness());
}


double RK::getFitness() {
	return m_fitness;
}

void RK::setFitness(double fitness1) {
	m_fitness = fitness1;
}

void RK::setProblemSize(int size) {
	m_pSize = size;
}

int RK::getProblemSize() {
	return m_pSize;
}

void RK::setRandomKeys(double *rks) {
	for (int i = 0; i< m_pSize; i++)
		m_randomkeys[i] = rks[i];
}

int* RK::getPermutation() {
	return m_permutation;
}

double* RK::getRandomKeys() {
	return m_randomkeys;
}

void RK::setPermutation(int* perm) {
	for (int i = 0; i< m_pSize; i++)
		m_permutation[i] = perm[i];
}

double* RK::copyGene() {
	double* randomkeys1 = new double[m_pSize];
	for (int i = 0; i < m_pSize; i++) {
		randomkeys1[i] = m_randomkeys[i];
	}
	return randomkeys1;
}

void RK::normalise() {
	for (int i = 0; i < m_pSize; i++) {
		m_randomkeys[i] = (double)((double)m_permutation[i] / (m_pSize - 1));
	}
}

void RK::setRKfromPermutation(int* perm) {
	m_randomkeys = new double[m_pSize];
	for (int i = 0; i < m_pSize; i++) {
		//            AL[i] = (double)(ranks[i]-1)/(size-1);
//		cout << perm[i] << endl;
		m_randomkeys[i] = (double)((double)perm[i] / (m_pSize - 1));
	}
	//m_randomkeys = normaliseRanks(m_permutation);
}
double* RK::getNormalisedRKs()
{
	return m_randomkeys;
}

