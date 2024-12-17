#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=1e6+10,mod=1e9+7;

inline int max(int a,int b) {
    return a<b? b:a;
}

inline int qpow(int frac,int k) {
    int result=1;
    while(k) {
        if(k&1) result=1ll*result*frac%mod;
        frac=2ll*frac%mod;
        k>>=1;
    }
    return result;
}

inline bool isdigit(char c) {
    return '0'<=c && c<='9';
}

inline int read(char *&p) {
    int result=0;
    while(isdigit(*p)) {
        result=(10ll*result+((*(p++))-'0'))%mod;
    }
    return result;
}

inline bool ismulti(char c) {
    return isdigit(c) || c=='x';
}

class Multi {
private:
    static const int M=65;
    int value[M],rank;
public:
    void get_rank() {
        rank=M-1;
        while (rank && !value[rank]) {
            --rank;
        }
    }
    Multi(){memset(value, 0, sizeof value); rank=0;}
    Multi(int k) {
        memset(value, 0, sizeof value);
        value[0]=k;
        rank=0;
    }
    Multi(int frac,int exp){
        memset(value, 0, sizeof value);
        value[exp]=frac;
        rank=exp;
    }
    Multi(char *&p) {
        memset(value, 0, sizeof value);
        if (isdigit(*p)) {
            value[0]=read(p);
            rank=0;
        }
        else {
            value[1]=1;
            rank=1;
            ++p;
        }
        /*
        frac=frac? frac:1;
        //printf("read frac %d\n",frac);
        int exp=0;
        if ((*p)=='x') {
            exp=1;
            if((*(++p))=='^') {
                exp=read(++p);
            }
        }
        //printf("exp=%d\n",exp);
        value[exp]=frac;
        */
    }
    int& operator[](int i) {return value[i];}
    int operator[](int i) const {return value[i];}
    Multi operator+(const Multi& other) const {
        Multi result;
        for (int i=0; i<M; ++i) {
            result[i]=(1ll*value[i]+other[i])%mod;
        }
        result.get_rank();
        return result;
    }
    Multi operator-(const Multi& other) const {
        Multi result;
        for (int i=0; i<M; ++i) {
            result[i]=(1ll*value[i]-other[i]+mod)%mod;
        }
        result.get_rank();
        return result;
    }
    Multi operator*(const Multi& other) const {
        Multi result;
        for(int i=0;i<=rank;++i)
            for(int j=0;j<=other.rank;++j)
                result[i+j]=(1ll*result[i+j]+1ll*value[i]*other[j]%mod)%mod;
        result.get_rank();
        return result;
    }
    Multi operator^(const Multi& other) const {
        Multi result(1);
        for(int i=0;i<other[0];++i)
            result=result*(*this);
        result.get_rank();
        return result;
    }
    void print() {
        int p=rank;
        while(p>=0)
            printf("%d ",value[p--]);
        puts("");
    }
};


char s[N],sta[N];
int top,tops,len;
Multi stas[N>>1];
bool last_read_is_operator=true;

bool operator_cmp(char a,char b) {
    switch (b) {
        case '(':
            return false;
        case '+':
        case '-':
            return a==')' || a=='+' || a=='-';
        case '*':
            return a==')' || a=='+' || a=='-' || a=='*';
        case '^':
            return a==')' || a=='+' || a=='-' || a=='*' || a=='^';
        default: exit(1);
    }
}

Multi operate(Multi a,Multi b,char o) {
    //printf("doing operator %c of Multi: \n",o);
    //a.print();
    //b.print();
    switch (o) {
        case '+':return a+b;
        case '-':return a-b;
        case '*':return a*b;
        case '^':return a^b;
        default: exit(2);
    }
}

inline void pop(char c) {
    while(top && operator_cmp(c,sta[top-1])) {
        if (tops<2) exit(3);
        Multi b=stas[--tops];
        Multi a=stas[--tops];
        stas[tops++]=operate(a,b,sta[--top]);
    }
    if (top && sta[top-1]=='(' && c==')') {
        --top;
    }
}

inline void complete_multi() {
    pop('*');
    sta[top++]='*';
}

int main() {
    scanf("%s",s);
    len=strlen(s);
    char *p=s;
    sta[top++]='(';
    while(p-s<len) {
        //printf("now p=%s\n",p);
        if (ismulti(*p)) {
            if ((*p)=='x' && !last_read_is_operator) {
                complete_multi();
            }
            stas[tops++]=Multi(p);
            last_read_is_operator=false;
        }
        else {
            if ((*p)=='(' && !last_read_is_operator) {
                complete_multi();
                last_read_is_operator=true;
            }
            else if ((*p)!='(' && (*p)!=')') {
                last_read_is_operator=true;
            }
            pop(*p);
            if((*p)!=')')
                sta[top++]=(*(p++));
            else ++p;
        }
        /*
        puts("sta:");
        for(int i=0;i<top;++i)
            printf("%c ",sta[i]);
        puts("");
        puts("stas:");
        for(int i=0;i<tops;++i)
            stas[i].print();
        puts("");
        */
    }
    pop(')');
    assert(tops==1);
    stas[0].print();
    return 0;
}
