#include <omp.h>
#include <stdlib.h>
#include <iostream>
#define N 100
using namespace std;
/*int main()
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
}*/
int main()
{
	int omp_get_thread_num();
	//omp_set_num_threads(4);
	# pragma omp parallel 
	{
		cout << "Thread rank: " <<omp_get_thread_num()<<endl;
	}
}

