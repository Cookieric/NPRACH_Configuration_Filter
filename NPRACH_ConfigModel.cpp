#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define LOG(...) printf(__VA_ARGS__)

int Auto_Send_All_Pass_Config_To_ShellScript_MAC_Command(double **,int &);

int main()
{
	srand(time(NULL));
	/*Test Parameters Setting*/
	int rachperiod[8]={40,80,160,240,320,640,1280,2560};
	int rachstart[8]={8,16,32,64,128,256,512,1024};
	int rachrepeat[8]={1,2,4,8,16,32,64,128};
	int rawindow[8]={2,3,4,5,6,7,8,10};// Not include in MAC_Command in shell script
	int rmax[12]={1,2,4,8,16,32,64,128,256,512,1024,2048};
	double gvalue[8]={1.5,2,4,8,16,32,48,64};
	int candidate[4]={1,2,4,8};

	/*Not include shell script*/
	double pdcchoffset[4]={0,0.125,0.25,0.375};// Not include in MAC_Command in shell script

	/*Not include shell script*/
	int dlcqi[13]={0,1,2,3,4,5,6,7,8,9,10,11,12};
	int dlrepeat[16]={1,2,4,8,16,32,64,128,192,256,384,512,768,1024,1536,2048};
	int rachscofst[7]={0,12,24,36,2,18,34};
	int rachnumsc[4]={12,24,36,48};


	//limitCondition1
	int L_rachperiod=sizeof(rachperiod)/sizeof(int);
	int L_rawindow=sizeof(rawindow)/sizeof(int);
	int L_rmax=sizeof(rmax)/sizeof(int);
	double L_gvalue=sizeof(gvalue)/sizeof(double);
	int L_rachrepeat=sizeof(rachrepeat)/sizeof(int);

	//limitCondition2
	int L_candidate=sizeof(candidate)/sizeof(int);
	int L_rachstart=sizeof(rachstart)/sizeof(int);

	//limitCondition3
	int L_dlcqi=sizeof(dlcqi)/sizeof(int);
	int L_dlrepeat=sizeof(dlrepeat)/sizeof(int);

	/*
	if(0<=dlcqi<=4)	if(10*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	if(dlcqi==5)	if(8*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	if(6=<dlcqi<=7)	if(6*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	if(dlcqi==8)	if(5*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	if(9=<dlcqi<=10)	if(4*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	if(11=<dlcqi<=12)	if(3*dlrepeat<(rmax*(gvalue-1)-(5-1)))
	*/


	double limitCondition1[12000][5];
	int cnt=0;
	for (int i = 0; i < L_rachperiod; ++i)
	{
		for (int j = 0; j < L_rachrepeat; ++j)
			for (int k = 0; k < L_rmax; ++k)
				for (int l = 0; l < L_gvalue; ++l)
					for (int m = 0; m < L_rawindow; ++m)
					{
						if(rachperiod[i]>1.4*4*rachrepeat[j]+3+rmax[k]*gvalue[l]*rawindow[m])
						{
							limitCondition1[cnt][0]=rachperiod[i];
							limitCondition1[cnt][1]=rachrepeat[j];
							limitCondition1[cnt][2]=rmax[k];
							limitCondition1[cnt][3]=gvalue[l];
							limitCondition1[cnt][4]=rawindow[m];
							// cout << limitCondition1[cnt][3] <<endl;
							// cout << limitCondition1[cnt][0] <<endl;
							++cnt;
						}
					}
	}
	// cout << "cnt: " << cnt << endl;

	double **passSetting;
	int size=400000;
	passSetting = new double*[size];
	for (int i = 0; i < size; ++i)	passSetting[i] = new double [7];

	int cnt1=0;
	for (int i = 0; i < cnt; ++i)
	{
		for (int j = 0; j < L_rachstart; ++j)
			for (int k = 0; k < L_candidate; ++k)
				if(limitCondition1[i][0]>rachstart[j]&&candidate[k]<=limitCondition1[i][2])
					{
						passSetting[cnt1][0]=limitCondition1[i][0];
						passSetting[cnt1][1]=limitCondition1[i][1];
						passSetting[cnt1][2]=limitCondition1[i][2];
						passSetting[cnt1][3]=limitCondition1[i][3];
						passSetting[cnt1][4]=limitCondition1[i][4];
						passSetting[cnt1][5]=rachstart[j];
						passSetting[cnt1][6]=candidate[k];
						// cout<<passSetting[cnt1][0]<<','<<passSetting[cnt1][1]<<','<<passSetting[cnt1][2]<<','<<passSetting[cnt1][3]<<','<<passSetting[cnt1][4]<<','<<passSetting[cnt1][5]<<','<<passSetting[cnt1][6]<<endl;
						// getchar();
						++cnt1;
					}
	}
	cout << "# of SIB2 Config Pass limitation : " << cnt1 << endl;
	if(Auto_Send_All_Pass_Config_To_ShellScript_MAC_Command(passSetting,cnt1)==0)
		cout<<"Auto Gen Log Done"<<endl;
	// getchar();
	// system(path);
	delete [] passSetting;
	return 0;
}
//istream& getline (istream& is, string& str, char delim);
//Get line from stream into string, extracts characters from is and stores them into str until the delimitation character delim is found (or the newline character, '\n'
int Auto_Send_All_Pass_Config_To_ShellScript_MAC_Command(double **passSetting, int &cnt1)
{
	int round=1;
	int *randomArray = new int[cnt1];
	for (int i = 0; i < cnt1; ++i)	randomArray[i]=0;

	for (int i = 0; i < cnt1; ++i)
	{
		int random= rand()%cnt1;
		if(randomArray[random]!=0)
		{
			--i;
			round--;
		}
		else
		{
			randomArray[random]=1;
			char *path;
			path=(char *)malloc(1000*sizeof(char));
			getcwd(path,1000);//Get the name of the current working directory
			strcat(path,"/run_MSG2_Test001.sh");
			 // char s[] = "Golden Global      View,disk * desk";
			// const char *del0 = ".";
			// const char *del1 = "0";
			for (int j = 0; j < 7; ++j)
			{
				if(j==3)
				{
					if(passSetting[i][j]==1.5)
					{
						// cout<<passSetting[i][j]<<endl;
						string tempStirng = to_string(passSetting[i][j]);
						// char *p = (char *)tempStirng.c_str();
						stringstream ss(tempStirng);//string to istream
						string sub_str;
						getline(ss,sub_str,'0');//delete after 0 (for example from 2.00 to 2)
					    // cout << sub_str << endl;
						// p = strtok(p,del1);//Split string into tokens
						strcat(path," ");
						strcat(path,sub_str.c_str());//c_str() conver from string to constant char*
					}
					else
					{
						string tempStirng = to_string(passSetting[i][j]);
						// char *p = (char *)tempStirng.c_str();
						stringstream ss(tempStirng);
						string sub_str;
						// cout <<"ss:"<< tempStirng << endl;
						getline(ss,sub_str,'.');//delete after . (for example from 2.00 to 2)
						// cout <<"sub_str:"<< sub_str << endl;
						// p = strtok(p,del0);
						strcat(path," ");
						strcat(path,sub_str.c_str());
					}
				}
				else
				{
					string tempStirng = to_string(passSetting[i][j]);
					// char *p = (char *)tempStirng.c_str();
					stringstream ss(tempStirng);
					string sub_str;
					getline(ss,sub_str,'.');
					// cout << sub_str << endl
					// p = strtok(p,del0);
					strcat(path," ");
					strcat(path,sub_str.c_str());
				}
			}
			// cout << "C++:"<< path << endl;
			cout << "Round: "<< round<< ", Test case " << random+1 << ":" << endl;
			// cout << "Path: " << path << endl;
			// getchar();
			string tempStirng = to_string(random+1);
			strcat(path," ");
			strcat(path,tempStirng.c_str());
			// strcat(path," ");
			// string tempStirng1 = to_string(cnt1);
			// strcat(path,tempStirng1.c_str());
			system(path);
			free(path);
			round++;
			if(round%11==0)	getchar();
			// getchar();
		}
	}
	delete []randomArray;
	getchar();
	return 0;
}





