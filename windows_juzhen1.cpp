#include<iostream>
#include <windows.h>
using namespace std;
void common(){
	int step = 10;
        for(int n= 10;n<=10000;n+=step){
        LARGE_INTEGER nFreq;
     	LARGE_INTEGER nBeginTime;
     	LARGE_INTEGER nEndTime;
        long long  int sum[n];
        long long int **b = new long long  int*[n];
        for(int i=0;i<n;i++){
        	b[i] = new long long  int[n];
		}
        long long  int *a = new long long  int[n];
        for(int i = 0;i<n;i++){
                a[i] = i;
                for(int j = 0;j<n;j++){
                        b[i][j] = j;
                }
        }
        double times = 0;
        for(int num = 0 ;num<5;num++){
		
        QueryPerformanceFrequency(&nFreq);
     	QueryPerformanceCounter(&nBeginTime);
        for(int i = 0; i < n; i++){
                sum[i] = 0;
                for(int j = 0; j < n; j++){
                        sum[i] += b[j][i] *a[j ];
        }

        }
        QueryPerformanceCounter(&nEndTime);
        times+=(double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time*1000.0<<"microseconds"<<endl;
    	if(n==100){
    		step = 100;
		}
		if(n==1000){
			step = 1000;
		}
        }
}
void cache(){
	int step = 10;
        for(int n= 10;n<=10000;n+=step){
        LARGE_INTEGER nFreq;
     	LARGE_INTEGER nBeginTime;
     	LARGE_INTEGER nEndTime;
        long long  int sum[n];
        long long  int **b =  new long long  int*[n];
        for(int i=0;i<n;i++){
        	b[i] = new long long  int[n];
		}
        long long int *a = new long long int[n];
        for(int i = 0;i<n;i++){
                a[i] = i;
                for(int j = 0;j<n;j++){
                        b[i][j] = j;
                }
        }
        double times = 0;

		
        QueryPerformanceFrequency(&nFreq);
     	QueryPerformanceCounter(&nBeginTime);
        for(int num = 0;num<5;num++){

        for(int i=0;i<n;i++){
                sum[i] = 0;
        }
        for(int j = 0; j < n; j++){
                for(int i = 0; i < n; i++){
				sum[i] += b[j][i] *a[j ];
        }
        }
        QueryPerformanceCounter(&nEndTime);
        times+=(double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time*1000.0<<"microseconds"<<endl;
    	if(n==100){
    		step = 100;
		}
		if(n==1000){
			step = 1000;
		}
        }
        
}
void cacheunrolling(){
	int  step = 10;
        for(int n= 10;n<=10000;n+=step){
        LARGE_INTEGER nFreq;
     	LARGE_INTEGER nBeginTime;
     	LARGE_INTEGER nEndTime;
        long long int sum[n];
        long long int **b = new long long int*[n];
        for(int i=0;i<n;i++){
        	b[i] = new long long int[n];
		}
        long long int *a = new long long int[n];
        for(int i = 0;i<n;i++){
                a[i] = i;
                for(int j = 0;j<n;j++){
                        b[i][j] = j;
                }
        }
        double times = 0;
        for(int num = 0 ;num<5;num++){
		
        QueryPerformanceFrequency(&nFreq);
     	QueryPerformanceCounter(&nBeginTime);
        for(int i=0;i<n;i+=2){
                sum[i] = 0;
                sum[i+1] = 0;
        }
        for(int j = 0; j < n; j++){
                for(int i = 0; i < n; i+=2){
				sum[i] += b[j][i] *a[j ];
				sum[i+1] += b[j][i+1]*a[j];
        }
        }
        QueryPerformanceCounter(&nEndTime);
        times+=(double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time*1000.0<<"microseconds"<<endl;
    	if(n==100){
    		step = 100;
		}
		if(n==1000){
			step = 1000;
		}
        }
}
int main(){

		cout<<"common:"<<endl;
		common();
        cout<<"cache:"<<endl;
        cache();
    
        cout<<"cache+unrolling:"<<endl;
        cacheunrolling();
        return 0;
}


