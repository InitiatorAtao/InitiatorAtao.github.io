#include <bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
using namespace std;
const int N=500000,M=100000,A=2;

char a[4000000];
int m, len;

void play(int t) {
    int l = t - 1, r = t + 1;
    while (l >= 0 && a[l] == a[t]) --l;
    while (r < len && a[r] == a[t]) ++r;
    if (r - l > 3) {
        memmove(a + l + 1, a + r, len - r + 1);
        len -= (r - l - 1);
        if (l >= 0) play(l);
    }
}

void handwriting_data(){
    freopen("data.in", "w", stdout);
    int n=N,m=M;
    for (int i=0; i<n; i+=4) {
        printf("%s","CCCC");
    }
    printf("\n%d\n",m);
    for (int i=0; i<m; i+=2) {
        puts("0 B");
        puts("0 B");
        puts("0 A");
        puts("0 A");
    }
    exit(0);
    printf("\n%d\n",m);
    while (m--) {
        puts("0 A");
    }
    exit(0);
}

int main() {
    handwriting_data();
    srand(time(0));
    freopen("data.in", "w", stdout);
    int n=rand()%(N+1),m=M;
    len=n;
    a[len]='\0';
    for (int i=0; i<len; ++i) {
        do {
            a[i]=rand()%A+'A';
        }while (i>=2 && (a[i-2]==a[i-1] && a[i-1]==a[i]));
    }
    printf("%s\n%d\n",a,m);
    while (m--) {
        char col=rand()%A+'A';
        int pos=rand()%(len+1);
        printf("%d %c\n",pos,col);
        memmove(a+pos+1, a+pos, len-pos+1);
        a[pos]=col;
        ++len;
        play(pos);
    }
    freopen("data.ans", "w", stdout);
    puts(a);
    return 0;
}
