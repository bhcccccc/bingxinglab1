#include<iostream>
#include <sys/time.h>
using namespace std;
void common(){
	int step = 10;
        for(int n= 10;n<=10000;n+=step){
        struct timeval start;
    struct timeval end;
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
		
        gettimeofday(&start,NULL);

        for(int i = 0; i < n; i++){
                sum[i] = 0;
                for(int j = 0; j < n; j++){
                        sum[i] += b[j][i] *a[j ];
        }

        }
        gettimeofday(&end,NULL);
        times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time<<"microseconds"<<endl;
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
        struct timeval start;
    struct timeval end;
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
        double times = 0.0;
		
        gettimeofday(&start,NULL);

        for(int num = 0;num<5;num++){

        for(int i=0;i<n;i++){
                sum[i] = 0;
        }
        for(int j = 0; j < n; j++){
                for(int i = 0; i < n; i++){
				sum[i] += b[j][i] *a[j ];
        }
        }
        gettimeofday(&end,NULL);
        times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time<<"microseconds"<<endl;
    	if(n==100){
    		step = 100;
		}
		if(n==1000){
			step = 1000;
		}
        }
}

void cacheunrolling(){
		int step = 10;
        for(int n= 10;n<=10000;n+=step){
        struct timeval start;
    struct timeval end;
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
		
        gettimeofday(&start,NULL);

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
        gettimeofday(&end,NULL);
        times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
    	}
    	double time = times /5;
    	cout<<n<<" "<<time<<"microseconds"<<endl;
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


