#include "sort.h"
#include <cstdio>
#include <cstdlib>
const int N=1e6+10;

struct triple {
    int res[3];
    int &operator[](int i){return res[i];}
};

triple cmp(int a,int b,int c) {
    int max,min;
    compare(a, b, c, &max, &min);
    return triple{
        min,
        (a!=min && a!=max)? a:
        (b!=min && b!=max)? b:
        (a!=b)? c:a,
        max};
}

template <class T>
inline void swap(T &a, T &b) {
	T c=a; a=b; b=c;
}

template<typename T>
T* upper_bound(T *l,T *r,T v) {
    if(r<=l) exit(1);
    if(r==l+1) {
        triple res=cmp(*l,*l,v);
        return v!=res[2]? l:r;
    }
    if(r==l+2) {
        triple res=cmp(*l,*(l+1),v);
        return v==res[0]? l:
               v==res[1]? l+1:r;
    }
    int step=(r-l)/3;
    T *L=l+step,*R=L+step;
    triple res=cmp(*L,*R,v);
    if(v==res[0]) return upper_bound(l,L,v);
    if(v==res[1]) return L+1<R? upper_bound(L+1,R,v):R;
    if(v==res[2]) return R+1<r? upper_bound(R+1,r,v):r;
    exit(2);
}

template<typename T>
void insert_sort(T *l,T *r) {//数据长度小于等于6时的插入排序
    if(r<=l+1) return;
    if(r==l+2) {
        triple res=cmp(*l,*l,*(l+1));
        *l=res[0];  *(l+1)=res[2];
        return;
    }
    triple res=cmp(*l,*(l+1),*(l+2));
    *l=res[0];  *(l+1)=res[1];  *(l+2)=res[2];
    for(T *i=l+3;i<r;++i) {
        T *p=upper_bound(l,i,*i);
        T tmp=*i;
        for(T *j=i;j>=p;--j)
            *j=*(j-1);
        *p=tmp;
    }
}

template<typename T>
T* get_middle(T* a,T *b,T *c) {
    triple res=cmp(*a,*b,*c);
    return res[1]==*a? a:
           res[1]==*b? b:c;
}

template<typename T>
T* get_middle(T* l,T *r) {//根据数据长度线性增长采样数
    if(r-l<=243)
        return get_middle(l,l+(r-l)/2,r-1);
    else {
        int jump=(r-l)/3;
        return get_middle(
            get_middle(l,l+jump),
            get_middle(l+jump,l+2*jump),
            get_middle(l+2*jump,r-1)
        );
    }
}

template<typename T>
void quick_sort(T *l,T *r) {
    if(r-l<=6) {
        insert_sort(l,r);
        return;
    }
    swap(*l,*get_middle(l,r));//采样中位数作为枢轴以尽可能实现折半分割
    T *p=l;
    for(T *i=l+1;i<r;++i) {
        int a=*l,b=*i,c=(i+1<r)? (*(++i)):(*l);
        triple res=cmp(a,b,c);
        for(int j=0;j<3;++j) {//一次比较两个数,log2(n)/2 比 log3(n) 更有优势
            if(res[j]==(*l)) break;
            if(res[j]==(*i)) swap(*(++p),*i);
            else             swap(*(++p),*(i-1));
        }
    }
    swap(*l, *p);
    quick_sort(l, p);
    quick_sort(p+1,r);
}

void sort(int n, int limit, int *a) {
    quick_sort(a,a+n);
}

