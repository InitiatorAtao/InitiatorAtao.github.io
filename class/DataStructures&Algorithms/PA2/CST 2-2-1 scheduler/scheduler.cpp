#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
const int N=1e6+10,P=1e6+10,B=1024;

int n,queue[N<<1],p[P],head=N,tail=N;
bool paused[N<<1];
char op[B];

int main() {
    scanf("%d",&n);
    while (n--) {
        scanf("%s",op);
        if (strcmp(op, "Create")==0) {
            int pid=0;
            scanf("%d",&pid);
            p[pid]=tail;
            queue[tail++]=pid;
        }
        else if (strcmp(op, "Tick")==0) {
            while (head<tail && paused[head]) ++head;
            assert(head<tail);
            printf("%d\n",queue[head]);
            int pid=queue[head++];
            p[pid]=tail;
            queue[tail++]=pid;
        }
        else if (strcmp(op, "Pause")==0) {
            int pid=0;
            scanf("%d",&pid);
            assert(!paused[p[pid]]);
            paused[p[pid]]=true;
        }
        else if (strcmp(op, "Resume")==0) {
            int pid=0;
            scanf("%d",&pid);
            queue[--head]=pid;
            p[pid]=head;
            paused[head]=false;
        }
    }
    return 0;
}
