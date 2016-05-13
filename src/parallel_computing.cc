#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include "../lib/tm_usage.h"
#include <iostream>
using namespace std;
void Test1(int n)
{
	for(int i =0;i<100000;++i)
	{
		
	}
	
cout << n <<endl;
}
void Test2(int n)
{
	for(int i=0;i<100000;++i)
	{
	
	}
cout << n << endl;
}
int main()
{
	CommonNs::TmUsage tmusg;
	CommonNs::TmStat  stat;
	
	tmusg.periodStart();
	for(int i =0;i<10;++i)
		Test1(i);
	tmusg.getPeriodUsage(stat);
	cout << "# run time 1 = " << (stat.uTime+stat.sTime)/1000000.0 << "sec" <<endl;
	
	CommonNs::TmUsage tmusg2;
	CommonNs::TmStat stat2;
	tmusg2.periodStart();
	#pragma omp parallel for
	for(int i = 0;i<10;++i)
		Test2(i);
	tmusg2.getPeriodUsage(stat2);
	cout << "# run time 1 = " << (stat2.uTime+stat2.sTime)/1000000.0 << "sec"     <<endl;
}
