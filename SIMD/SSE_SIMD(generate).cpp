#include<iostream>
#include<windows.h>
#include <stdio.h>
#include<typeinfo>
#include <stdlib.h>
#include<tmmintrin.h>
#include<xmmintrin.h>
#include<emmintrin.h>
#include<pmmintrin.h>
#include<smmintrin.h>
#include<nmmintrin.h>
#include<immintrin.h>
using namespace std;
#define N 2000
float** align_m = NULL;  //���������
float** unalign = NULL;  // δ���������
void unalign_init() {     //δ���������ĳ�ʼ��
    unalign = new float*[N+1];
    for (int i = 0; i <= N; i++) {
        unalign[i] = new float[N+1];
    }
    for (int i = 0; i <= N; i++) {
        unalign[i][i] = 1.0;
        for (int j = i + 1; j <= N; j++) {
            unalign[i][j] = rand() % 1000;
        }

    }
    for (int k = 0; k <=N; k++) {
        for (int i = k + 1; i <=N; i++) {
            for (int j = 0; j <=N; j++) {
                unalign[i][j] += unalign[k][j];
                unalign[i][j] = (int)unalign[i][j] % 1000;
            }
        }
    }
}

void deleteUnalign() {
    for (int i = 0; i < N; i++) {
        delete[] unalign[i];
    }
    delete unalign;
}

void align_init(int alignment) {
    if (align_m == NULL) {
        align_m = (float**)_aligned_malloc(sizeof(float*) * N, alignment);
        for (int i = 0; i < N; i++) {
            align_m[i] = (float*)_aligned_malloc(sizeof(float) * N, alignment);
            //ʹ�þ���ÿһ�����ڴ��а���alignment���룬SSEΪ16��AVXΪ32
            //cout << align_m[i] << endl;
        }
    }
    for (int i = 0; i < N; i++) {
        align_m[i][i] = 1.0;          //�������ĳ�ʼ��
        for (int j = i + 1; j < N; j++) {
            align_m[i][j] = rand() % 1000;
        }

    }
    for (int k = 0; k < N; k++) {
        for (int i = k + 1; i < N; i++) {
            for (int j = 0; j < N; j++) {
                align_m[i][j] += align_m[k][j];
                align_m[i][j] = (int)align_m[i][j] % 1000;
            }
        }
    }
}


void print(float **a) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << a[i][j] << " ";
        }
        cout << endl;
    }
}
void generation(int n, float **m, float x[]) {
    //�����һ��δ֪����ʼ�������������
    for (int i = n - 1; i >= 0; --i) {
        // δ֪������"���ں�"�����ֵ����ϵ��
        x[i] = m[i][n] / m[i][i];
        for (int j = i - 1; j >= 0; --j) {
            // ���x[i]�����δ��������ÿһ�����̣�����"���ں�"�����ֵ
            m[j][n] -= x[i] * m[j][i];
        }
    }
}
void LU(float **m) {    //��ͨ��Ԫ�㷨
    for (int k = 0; k < N; k++) {
        for (int j = k + 1; j < N; j++) {
            m[k][j] = m[k][j] / m[k][k];
        }
        m[k][k] = 1.0;
        for (int i = k + 1; i < N; i++) {
            for (int j = k + 1; j < N; j++) {
                m[i][j] = m[i][j] - m[i][k] * m[k][j];
            }
            m[i][k] = 0;
        }
    }
}

void sse_optimized(float **m) {            //û�ж����SSE�㷨
    for (int k = 0; k < N; k++) {
        __m128 t1 = _mm_set1_ps(m[k][k]);
        int j = 0;
        for (j = k + 1; j + 4 <= N; j += 4) {
            __m128 t2 = _mm_loadu_ps(&m[k][j]);   //δ���룬��loadu��storeuָ��
            t2 = _mm_div_ps(t2, t1);
            _mm_storeu_ps(&m[k][j], t2);
        }
        for (; j < N; j++) {
            m[k][j] = m[k][j] / m[k][k];
        }
        m[k][k] = 1.0;
        for (int i = k + 1; i < N; i++) {
            __m128 vik = _mm_set1_ps(m[i][k]);
            for (j = k + 1; j + 4 <= N; j += 4) {
                __m128 vkj = _mm_loadu_ps(&m[k][j]);
                __m128 vij = _mm_loadu_ps(&m[i][j]);
                __m128 vx = _mm_mul_ps(vik, vkj);
                vij = _mm_sub_ps(vij, vx);
                _mm_storeu_ps(&m[i][j], vij);
            }
            for (; j < N; j++) {
                m[i][j] = m[i][j] - m[i][k] * m[k][j];
            }
            m[i][k] = 0;
        }
    }
}

void SSE_generation(int n, float **a, float b[]) {
    __m128 temp, t1, t2, sub;

    for (int i = n - 1; i >= 0; i-=1) {
    	 
        b[i] = a[i][n] / a[i][i];
               
        temp = _mm_set1_ps(b[i]);

        // �ʹ����㷨˼·���ƣ������Ƚ�����ת�ã��������
        for (int k = 0; k < i; ++k) {
            swap(a[k][n], a[n][k]);
            swap(a[k][i], a[i][k]);
        }
        //ÿ�ĸ�һ����м���
       
        int j = i - 4;
        for (; j >= 0; j -= 4) {
            t1 = _mm_loadu_ps(a[i] + j);
            t2 = _mm_loadu_ps(a[n] + j);
            sub = _mm_sub_ps(t2, _mm_mul_ps(t1, temp));
            _mm_store_ss(a[n] + j, sub);
        }
        //����ʣ�ಿ��
        for (j += 3; j >= 0; --j) { 
            a[n][j] -= a[i][j] * b[i];
        }
        //ת�û���
        for (int k = 0; k < i; ++k) {
            swap(a[k][n], a[n][k]);
            swap(a[k][i], a[i][k]);
        }
    }
    
}

void sse_align(float **m) {  //�����SSE�㷨
    for (int k = 0; k < N; k++) {
        __m128 t1 = _mm_set1_ps(m[k][k]);
        int j = k+1;

        //cout << &m[k][j];
        while ((long long)(&m[k][j])%16)
        {
            m[k][j] = m[k][j] / m[k][k];
            j++;
        }
        //cout << &m[k][j]<<endl;
        for ( ; j + 4 <= N; j += 4) {
            __m128 t2 = _mm_load_ps(&m[k][j]);   //�Ѷ��룬��load��storeָ��
            t2 = _mm_div_ps(t2, t1);
            _mm_store_ps(&m[k][j], t2);
        }
        for (; j < N; j++) {
            m[k][j] = m[k][j] / m[k][k];
        }
        m[k][k] = 1.0;
        for (int i = k + 1; i < N; i++) {
            __m128 vik = _mm_set1_ps(m[i][k]);
            j = k + 1;
            while ((long long)(&m[k][j])%16)
            {
                m[i][j] = m[i][j] - m[i][k] * m[k][j];
                j++;
            }
            for ( ; j + 4 <= N; j += 4) {
                __m128 vkj = _mm_load_ps(&m[k][j]);
                __m128 vij = _mm_loadu_ps(&m[i][j]);
                __m128 vx = _mm_mul_ps(vik, vkj);
                vij = _mm_sub_ps(vij, vx);
                _mm_storeu_ps(&m[i][j], vij);
            }
            for (; j < N; j++) {
                m[i][j] = m[i][j] - m[i][k] * m[k][j];
            }
            m[i][k] = 0;
        }
    }
}

int main() {
	cout<<"SSE���ݹ�ģ��"<<N<<endl; 
    long long head, tail, freq;
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    LU(unalign);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "LU time:" << (tail - head) * 1000 / freq << "ms" << endl;
    cout << "------------------" << endl;
    deleteUnalign();


    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sse_optimized(unalign);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"SSE(δ����) time:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();
    
    align_init(16);    //SSEָ����Ҫ16λ����
    
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
    sse_align(align_m);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    cout << "SSE(����) time:" << (tail - head) * 1000 / freq << "ms" << endl;
    cout << "------------------" << endl;
    
	float b[N];
	for(int i =0;i<N;i++)
		b[i]=0;
    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);  
    generation(N, unalign, b);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"generatetime:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();

	for(int i =0;i<N;i++)
		b[i]=0;
	unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);

    SSE_generation(N, unalign, b);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"SSEgeneratetime:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();
    
    for(int i =0;i<N;i++)
		b[i]=0;
    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
	LU(unalign);  
    generation(N, unalign, b);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"1time:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();
    
    for(int i =0;i<N;i++)
		b[i]=0;
    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
	sse_optimized(unalign);
    generation(N, unalign, b);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"2time:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();
	
	for(int i =0;i<N;i++)
		b[i]=0;
    unalign_init();
    QueryPerformanceCounter((LARGE_INTEGER*)&head);
	LU(unalign);  
    SSE_generation(N, unalign, b);
    QueryPerformanceCounter((LARGE_INTEGER*)&tail);
    //print(unalign);
    cout<<"3time:"<<(tail-head)*1000/freq<<"ms"<<endl;
    cout << "------------------" << endl;
    deleteUnalign();
}

