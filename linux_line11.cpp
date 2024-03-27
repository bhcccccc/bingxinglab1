#include<iostream>
#include <sys/time.h>
using namespace std;
void recursion(int n,int *a) {
                                if (n == 1)
                                        return;
                                else {
                                        for (int i = 0; i < n / 2; i++)
                                        a[i ] += a[n -i - 1];
                                        n = n / 2;
                                        recursion(n,a);} }
int main(){
		cout<<"common:"<<endl;
        for(long long int n = 1024;n<=536870912;n*=2){
        	 struct timeval start;
    struct timeval end;
                int sum = 0;
                int *a=new int [n];
                for(int i = 0;i<n;i++){
                        a[i]  = i;
                }
               
                double times = 0;
                for(int num = 0;num<5;num++){
                	
				gettimeofday(&start,NULL);
                for(int i = 0;i<n;i++){
                        sum+=a[i];
                }

                gettimeofday(&end,NULL);
        		times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
				}
				double time = times/5;
				cout<<n<<" "<<times<<"microseconds"<<endl;
        }
        cout<<"manylines:"<<endl;
         for(long long int n = 1024;n<=536870912;n*=2){
        	 struct timeval start;
    struct timeval end;
                int sum = 0;
                int *a=new int [n];
                for(int i = 0;i<n;i++){
                        a[i]  = i;
                }
               
                double times = 0;
                for(int num = 0;num<5;num++){
                	
				gettimeofday(&start,NULL);
                int sum1 = 0;int sum2 = 0;
                                for (int i = 0;i < n; i += 2) {
                                sum1 += a[i];
                                sum2 += a[i + 1];
                                 }
                                 sum = sum1 + sum2;

                gettimeofday(&end,NULL);
        		times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
				}
				double time = times/5;
				cout<<n<<" "<<times<<"microseconds"<<endl;
        }
       
        cout<<"doublefor:"<<endl;
        for(long long int n = 1024;n<=536870912;n*=2){
        	 struct timeval start;
    struct timeval end;
                int sum = 0;
                int *a=new int [n];
                for(int i = 0;i<n;i++){
                        a[i]  = i;
                }
               
                double times = 0;
                for(int num = 0;num<5;num++){
                	
				gettimeofday(&start,NULL);
                for (int m = n; m > 1; m /= 2)
                                for (int i = 0; i < m / 2; i++)
                                a[i ] = a[i * 2] + a[i * 2 + 1];

                gettimeofday(&end,NULL);
        		times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
				}
				double time = times/5;
				cout<<n<<" "<<times<<"microseconds"<<endl;
        }
       
        cout<<"digui:"<<endl;
                for(long long int n = 1024;n<=536870912;n*=2){
        	 struct timeval start;
    struct timeval end;
                int sum = 0;
                int *a=new int [n];
                for(int i = 0;i<n;i++){
                        a[i]  = i;
                }
               
                double times = 0;
                for(int num = 0;num<5;num++){
                	
				gettimeofday(&start,NULL);
                recursion(n,a);
                gettimeofday(&end,NULL);
        		times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
				}
				double time = times/5;
				cout<<n<<" "<<times<<"microseconds"<<endl;
        }
       
        
        
        cout<<"manylines+unrolling:"<<endl;
        for(long long int n = 1024;n<=536870912;n*=2){
        	 struct timeval start;
    struct timeval end;
                int sum = 0;
                int *a=new int [n];
                for(int i = 0;i<n;i++){
                        a[i]  = i;
                }
               
                double times = 0;
                for(int num = 0;num<5;num++){
                	
				gettimeofday(&start,NULL);
               int sum1 = 0;int sum2 = 0;
                                for (int i = 0;i < n; i += 4) {
                                sum1 += a[i];
                                sum1+=a[i+2];
                                sum2 += a[i + 1];
                                sum2+=a[i+3];
                                 }
                                 sum = sum1 + sum2;
                gettimeofday(&end,NULL);
        		times+=((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))*1.0/1000;
				}
				double time = times/5;
				cout<<n<<" "<<times<<"microseconds"<<endl;
        }
        
        return 0;
}

