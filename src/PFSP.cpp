#include "PFSP.h"
#include "Tools.h"
#include <fstream>
#include <iostream>
#include <algorithm>
using std::ifstream;
using std::ofstream;

PFSP::PFSP() {
}

PFSP::~PFSP()
{
	for (int i = 0; i < MACHINE_NUM; i++)
	{
		delete[] JOBPROCESSINGMATRIX[i];
	}
	delete[] JOBPROCESSINGMATRIX;
        for (int i = 0; i<MACHINE_NUM; i++)
        {
		delete[] jobsTimeTable_aux[i];
        }
	delete[] jobsTimeTable_aux;
	//delete auxiliary data tables for evaluation.
        delete[] genes_aux;
	delete[] inverted_aux;
	
	

}

int PFSP::ReadInstance(string filename)
{
	//cout << filename << endl;
	bool readMatrix = false;
	bool readDimension = false;
	//double **coordinates;
	char line[2048]; // variable for input value
	string data = "";
	ifstream indata;
	indata.open(filename.c_str(), ios::in);
	//int num=0;
	while (!indata.eof())
	{
		//LEER LA LINEA DEL FICHERO
		indata.getline(line, 2048);
		stringstream ss;
		string sline;
		ss << line;
		ss >> sline;
		if (t.strContains(line, "number of jobs") == true && readMatrix == true)
		{
			break;
		}
		else if (t.strContains(line, "number of jobs") == true)
		{
			readDimension = true;
		}
		else if (readDimension == true)
		{
			//char * pch;
			JOB_NUM = atoi(strtok(line, " "));
			//cout<<"JOB NUM: "<<JOB_NUM<<endl;
			MACHINE_NUM = atoi(strtok(NULL, " "));
			//cout<<"MACHINE NUM: "<<MACHINE_NUM<<endl;
			readDimension = false;

		}
		else if (readMatrix)
		{
			;
			if (data == "")
				data = line;
			else
				data = data + ' ' + line;
		}
		else if (t.strContains(line, "processing times :"))
		{
			readMatrix = true;
		}
	}
	indata.close();

	//BUILD JOB PROCESSING MATRIX
	//cout << "--> BUILDING JOB PROCESSING MATRIX" << endl;
	JOBPROCESSINGMATRIX = new int*[MACHINE_NUM];
	for (int i = 0; i<MACHINE_NUM; i++)
	{
		JOBPROCESSINGMATRIX[i] = new int[JOB_NUM];
	}

	//FILL JOB PROCESSING MATRIX
	//cout << "--> FILLING JOB PROCESSING MATRIX: "<<data << endl;
	istringstream iss(data);
	int i = 0;
	int j = 0;
	do
	{
		string sub;
		iss >> sub;
		if (sub != "")
		{
			//save distance in distances matrix. Save negative distance in order to minimize fitness instead of
			//maximize.
			JOBPROCESSINGMATRIX[i][j] = atoi(sub.c_str());
			if (j == JOB_NUM - 1)
			{
				i++;
				j = 0;
			}
			else
			{
				j++;
			}
		}
		else
		{
			break;
		}
	} while (iss);

	//initialize data structures for fast evaluations.
        jobsTimeTable_aux = (int**)malloc(sizeof(int*)*MACHINE_NUM);
	for (int i = 0; i<MACHINE_NUM; i++)
        {
		jobsTimeTable_aux[i] = (int*)malloc(sizeof(int)*JOB_NUM);
        }

    //        	jobsTimeTable_aux = new int*[MACHINE_NUM];
    //	for (int i = 0; i<MACHINE_NUM; i++)
    //	{
    //		jobsTimeTable_aux[i] = new int[JOB_NUM];
    //	}
                
                
                
	genes_aux = new int[JOB_NUM];
	inverted_aux = new int[JOB_NUM];


	//BUILD JOB PROCESSING MATRIX
	//cout << "--> BUILDING JOB PROCESSING MATRIX" << endl;
	JOBPROCESSINGMATRIX2 = new int*[JOB_NUM];
	for (int i = 0; i<JOB_NUM; i++)
	{
		JOBPROCESSINGMATRIX2[i] = new int[MACHINE_NUM];
	}

	for (int i = 0; i<JOB_NUM; i++)
		for (int j = 0; j<MACHINE_NUM; j++)
			JOBPROCESSINGMATRIX2[i][j] = JOBPROCESSINGMATRIX[j][i];

	return (JOB_NUM);
}


//int PFSP::ReadInstance(string filename)
//{      
//	bool readMatrix=false;
//	bool readDimension=false;
//	//double **coordinates;
//	char line[2048]; // variable for input value
//	string data="";
//	ifstream indata;
//	indata.open(filename.c_str(),ios::in);
//	//int num=0;
//	while (!indata.eof())
//	{
//		//LEER LA LINEA DEL FICHERO
//		indata.getline(line, 2048);
//		stringstream ss;
//		string sline;
//		ss << line;
//		ss >> sline;
//		if (t.strContains(line,"number of jobs")==true && readMatrix==true)
//		{
//			break;
//		}
//		else if (t.strContains(line,"number of jobs")==true)
//		{
//			readDimension=true;
//		}
//		else if (readDimension==true)
//		{
//			//char * pch;
//			JOB_NUM = atoi(strtok (line," "));
//			//cout<<"JOB NUM: "<<JOB_NUM<<endl;
//			MACHINE_NUM = atoi(strtok (NULL, " "));
//			//cout<<"MACHINE NUM: "<<MACHINE_NUM<<endl;
//			readDimension=false;
//			
//		}
//		else if (readMatrix)
//		{;
//			if (data=="")
//				data = line;
//			else
//				data = data+' '+line;
//		}
//		else if (t.strContains(line,"processing times :"))
//		{
//			readMatrix=true;
//		}
//	}
//	indata.close();
//	
//	//BUILD JOB PROCESSING MATRIX
//	//cout << "--> BUILDING JOB PROCESSING MATRIX" << endl;
//	JOBPROCESSINGMATRIX = new int*[MACHINE_NUM];
//	for (int i=0;i<MACHINE_NUM;i++)
//	{
//		JOBPROCESSINGMATRIX[i]= new int[JOB_NUM];
//	}
//	
//	//FILL JOB PROCESSING MATRIX
//	//cout << "--> FILLING JOB PROCESSING MATRIX: "<<data << endl;
//	istringstream iss(data);
//	int i=0;
//	int j=0;
//	do
//	{
//		string sub;
//	    iss >> sub;
//	    if (sub!="")
//	    {
//			//save distance in distances matrix. Save negative distance in order to minimize fitness instead of
//			//maximize.
//	    	JOBPROCESSINGMATRIX[i][j]= atoi(sub.c_str());
//	    	if (j==JOB_NUM-1)
//	    	{
//	    		i++;
//	    		j=0;
//	    	}
//	    	else
//	    	{
//	    		j++;
//	    	}
//	    }
//	    else
//	    {
//	    	break;
//	    }
//	} while (iss);
//	
//	//initialize data structures for fast evaluations.
////	jobsTimeTable_aux=(int**)malloc(sizeof(int*)*MACHINE_NUM);
//	for (int i=0;i<MACHINE_NUM;i++)
////		jobsTimeTable_aux[i]=(int*)malloc(sizeof(int)*JOB_NUM);
//	genes_aux=new int[JOB_NUM];
//	inverted_aux=new int[JOB_NUM];
//	
//	
//	//BUILD JOB PROCESSING MATRIX
//	//cout << "--> BUILDING JOB PROCESSING MATRIX" << endl;
//	JOBPROCESSINGMATRIX2 = new int*[JOB_NUM];
//	for (int i=0;i<JOB_NUM;i++)
//	{
//		JOBPROCESSINGMATRIX2[i]= new int[MACHINE_NUM];
//	}
//	
//	for(int i=0;i<JOB_NUM;i++)
//		for (int j=0;j<MACHINE_NUM;j++)
//			JOBPROCESSINGMATRIX2[i][j]=JOBPROCESSINGMATRIX[j][i];
//		
//	return (JOB_NUM);
//}
//


int PFSP::EvaluateFSPTotalFlowtime(int * genes)
{
	EVALUATIONS++;
	int* timeTable = new int[MACHINE_NUM];
	int j, z, job;
	int machine;
	int prev_machine = 0;
	//int genes[20]={2,16,8,14,13,7,18,12,15,5,6,0,1,3,4,17,19,11,10,9};
	int first_gene = genes[0];
	timeTable[0] = JOBPROCESSINGMATRIX[0][first_gene];
	for (j = 1; j<MACHINE_NUM; j++)
	{
		timeTable[j] = timeTable[j - 1] + JOBPROCESSINGMATRIX[j][first_gene];
	}

	int fitness = timeTable[MACHINE_NUM - 1];
	for (z = 1; z<JOB_NUM; z++)
	{
		job = genes[z];

		//machine 0 is always incremental, so:
		timeTable[0] += JOBPROCESSINGMATRIX[0][job];
		prev_machine = timeTable[0];
		for (machine = 1; machine<MACHINE_NUM; machine++)
		{
			timeTable[machine] = max(prev_machine, timeTable[machine]) + JOBPROCESSINGMATRIX[machine][job];
			prev_machine = timeTable[machine];
		}

		fitness += timeTable[MACHINE_NUM - 1];
	}
	delete[] timeTable;// ###ORC
	return fitness;
}


double PFSP::EvaluateFSPMakespan(int * genes)
{
	//    EVALUATIONS++;
	double fitness = 0;
	double* timeTable = new double[MACHINE_NUM];

	for (int j = 0; j<MACHINE_NUM; j++)
	{
		timeTable[j] = 0;
	}
	for (int z = 0; z<JOB_NUM; z++)
	{
		int job = genes[z];
		//cout<<"Job "<<job<<endl;
		for (int machine = 0; machine<MACHINE_NUM; machine++)
		{
			double processingTime = JOBPROCESSINGMATRIX[machine][job];
			if (machine == 0)
			{
				timeTable[machine] += processingTime;
			}
			else
			{
				if (timeTable[machine - 1]<timeTable[machine])
				{
					timeTable[machine] = timeTable[machine] + processingTime;
				}
				else
				{
					timeTable[machine] = timeTable[machine - 1] + processingTime;
				}
			}
			//cout<<"M "<<machine<<" Time "<<timeTable[machine]<<endl;
		}
	}

	fitness = timeTable[MACHINE_NUM - 1];
	delete[] timeTable;// ###ORC
	return fitness;
}

int PFSP::getNumberofEvaluation()
{
	return EVALUATIONS;
}
