#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "../lib/tm_usage.h"
#include <iostream>
#define N 100
using namespace std;
/*void Test1(int n)
{
	for(int i=0;i<100000;++i)
	{
	cout << i << " ";
		
	}
cout << endl;
cout << n <<endl;
}
void Test2(int n)
{
	for(int i=0;i<100000;++i)
	{
	cout << i << " " ;	
	}
	cout << endl;
cout << n << endl;
}
int main()
{
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat  stat;
	
	tmusg.periodStart();
	for(int i =0;i<5;++i)
		Test1(i);
	tmusg.getPeriodUsage(stat);
	
	CommonNs::TmUsage tmusg2;
	CommonNs::TmStat stat2;
	tmusg2.periodStart();
	#pragma omp parallel for
	for(int i = 0;i<5;++i)
		Test2(i);
	tmusg2.getPeriodUsage(stat2);
	cout << "# run time 1 = " << (stat.uTime+stat.sTime)/1000000.0 << "sec" <<endl;
	cout << "# run time 2 = " << (stat2.uTime+stat2.sTime)/1000000.0 << "sec"     <<endl;
}*/






int main()
 {   
     int nthreads,tid,i;
     float a[N],b[N],c[N];
     
     for(int i = 0;i<N;i++)
         a[i] = b[i] = i;
     #pragma omp parallel shared(a,b,c,threads) private(i,itd)
     {
         tid = omp_get_thread_num();
         if(tid == 0)
         {
             nthreads = omp_get_num_threads();
             cout << "Number of threads = " << nthreads<<endl;
         }
         
         cout << "Thread " << tid << "starting..."<<endl;
     #pragma omp for
     for(int i = 0;i<N;i++)
     {                                                                                                                                                               
         c[i] = a[i] + b[i];
         cout << "Thread " << tid << ": c["<<i<<"] = " << c[i] <<endl;
     }
     }
 } 

