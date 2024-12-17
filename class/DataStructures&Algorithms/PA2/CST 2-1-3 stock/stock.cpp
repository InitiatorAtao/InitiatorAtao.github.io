#include <cstdio>
#include <cstdlib>
#include <cwchar>
const int N=1e6+10,S=1024;

char buff[S];
int n,queue[N],rubbish[N],head,tail;
long long ans;

int main() {
    scanf("%d",&n);
    fgets(buff,sizeof buff,stdin);
    n*=2;
    for(int i=0;i<n;++i) {
        fgets(buff,sizeof buff,stdin);
        int d=0,x=0;
        int num=sscanf(buff, "%d%d",&d,&x);
        if (head<tail) {
            //printf("ans+=%d*%d=%lld\n",queue[head],d,1ll*queue[head]*d);
            ans+=1ll*queue[head]*d;
        }
        if (num<=1) {
            rubbish[head]? (--rubbish[head]):(++head);
        }
        else {
            int rub=0;
            while(head<tail && queue[tail-1]<=x)
                rub+=rubbish[--tail]+1;
            rubbish[tail]=rub;
            queue[tail++]=x;
        }
        /*
        printf("queue: ");
        for (int j=head; j<tail; ++j) {
            printf("%d ",queue[j]);
        }
        puts("");
        */
    }
    printf("%lld\n",ans);
    return 0;
}
