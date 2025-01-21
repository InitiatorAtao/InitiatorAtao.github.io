/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Initial Team",
    /* First member's full name */
    "陶文华",
    /* First member's email address */
    "twh23@mails.tsinghua.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};
//For one block: ||id|user||
struct Node {//Splay 中的节点
    void *data;     //数据指针
    int l,r,f;      //左儿子,右儿子,父亲
};
#define REALLOCATE_LIMIT 8
/* 16 bytes alignment */
#define ALIGNMENT_LENGTH 4

#define ALIGNMENT (1<<ALIGNMENT_LENGTH)

#define PTR_SIZE sizeof(void*)

#define INT_SIZE sizeof(int)

#define SIZE_T_SIZE sizeof(size_t)

#define ALIGNED_SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define META_SIZE (2*ALIGNED_SIZE_T_SIZE)

#define MIN_SIZE (META_SIZE+2*INT_SIZE)             //两侧是块大小和低位 STATE,中间是至少 16 字节的数据块

#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define FLOOR(size) ((size) & ~(ALIGNMENT-1))

#define STATE(p) ((*(size_t*)(p)) & (ALIGNMENT-1))  //当前块是否为空,如果结果不为 0 (可能为 1~15) 存储一些额外信息
                                                    //1:当前 PREV 指向对应树节点 2:当前 PREV 指向上一块同大小空块
#define LEFT_SIZE(p) ((size_t*)(p))

#define PREV(p) ((void**)((p)+ALIGNED_SIZE_T_SIZE))         //STATE=1 存储对应的树节点下标,STATE=2 存储上一块同样大小空块的位置

#define NEXT(p) ((void**)((p)+ALIGNED_SIZE_T_SIZE+SIZE_T_SIZE))       //STATE!=0 时同理存储下一块同样大小的空块的相对位置

#define SIZE(p) (FLOOR(*LEFT_SIZE(p)))

#define RIGHT_SIZE(p) ((size_t*)((char*)(p)+SIZE(p)-ALIGNED_SIZE_T_SIZE))

#define DATA(p) ((void*)((char*)(p)+ALIGNED_SIZE_T_SIZE))

#define META(p) ((void*)((char*)(p)-ALIGNED_SIZE_T_SIZE))

#define NEXT_BLOCK(p) ((void*)((char*)(p)+SIZE(p)))

#define PREV_BLOCK(p) ((void*)((char*)(p)-FLOOR(*((size_t*)((char*)(p)-ALIGNED_SIZE_T_SIZE)))))

inline void create_block(void *p,size_t size,int state) {
    *LEFT_SIZE(p)=size|state;
    *RIGHT_SIZE(p)=size|state;
    void *next=NEXT_BLOCK(p),*prev=PREV_BLOCK(p);
}

inline void set_state(void *p,int v) { 
    *LEFT_SIZE(p) &= ~(ALIGNMENT - 1);
    *LEFT_SIZE(p) |= v;
    *RIGHT_SIZE(p) &= ~(ALIGNMENT - 1);
    *RIGHT_SIZE(p) |= v;
}

struct Node *t=NULL;        //Splay 数组 (可能被整体移动) 本来是应该用 B 树的,但是 DSA 那边只写了 Splay 和 AVL,凑合用吧
int root;                   //Splay 树根

size_t tree_size,tree_capacity;
int *tree_pool;             //树节点的回收池 (可能被整体移动)
size_t tree_pool_size,tree_pool_capacity;

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    //fprintf(stderr,"initiating...\n");
    root=tree_size=tree_pool_size=0;
    tree_capacity=tree_pool_capacity=REALLOCATE_LIMIT<<1;
    t=mm_malloc(tree_capacity*sizeof(struct Node));
    t[0].data=NULL; t[0].l=t[0].r=t[0].f=0;                     //Splay 在根部的操作会频繁访问 t[0],需要保证其值
    tree_pool=mm_malloc(tree_pool_capacity*sizeof(int));
    if(!t || !tree_pool) return 1;
    return 0;
}

//以下为 Splay 操作
int new_node() {//分配一个新节点
    int u=0;
    if(tree_pool_size) {
        --tree_pool_size;
        u=tree_pool[tree_pool_size];
    }
    else{
        if(!(tree_size+REALLOCATE_LIMIT<tree_capacity)) {
            tree_capacity<<=1;
            //fprintf(stderr,"tree reallocing with t=%lx(%ld)\n",t,SIZE(META(t)));
            t=mm_realloc(t,tree_capacity*sizeof(struct Node));   //自持重分配
            //fprintf(stderr,"tree reallocated with t=%lx(%ld)\n",t,SIZE(META(t)));
            assert(SIZE(META(t)));
        }
        ++tree_size;
        u=tree_size;
    }
    t[u].data=NULL; t[u].l=t[u].r=t[u].f=0;
    //fprintf(stderr,"in new node %d\n",u);
    return u;
}

void delete_node(int u) {//回收节点 u
    assert(0<u && u<tree_capacity);
    if(!(tree_pool_size+REALLOCATE_LIMIT<tree_pool_capacity)) {
        tree_pool_capacity<<=1;
        //fprintf(stderr,"tree pool reallocing with t=%lx(%ld)\n",tree_pool,SIZE(META(tree_pool)));
        tree_pool=mm_realloc(tree_pool,tree_pool_capacity*sizeof(int));
        //fprintf(stderr,"tree pool reallocated with t=%lx(%ld)\n",tree_pool,SIZE(META(tree_pool)));
    }
    tree_pool[tree_pool_size]=u;
    ++tree_pool_size;
}

void update(int u) {//更新节点的信息
    if(!u) return;
    int l=t[u].l,r=t[u].r;
    if(l) t[l].f=u;
    if(r) t[r].f=u;
}

void left(int u) {//将 u 左旋
/*
int f=fa[u];
int p=fa[f];
rs[f]=ls[u];
ls[u]=f;
if(p) (ls[p]==f? ls[p]:rs[p])=u;
else fa[u]=0,root=u;
*/
    int f=t[u].f,p=t[f].f;
    t[f].r=t[u].l;
    t[u].l=f;
    if(p) {
        if(t[p].l==f)   t[p].l=u;
        else            t[p].r=u;
    }
    else t[u].f=0,root=u;
    update(f);
    update(u);
    update(p);
}

void right(int u) {//将 u 右旋
/*
int f=fa[u];
int p=fa[f];
ls[f]=rs[u];
rs[u]=f;
if(p) (ls[p]==f? ls[p]:rs[p])=u;
else fa[u]=0,root=u;
*/
    int f=t[u].f,p=t[f].f;
    t[f].l=t[u].r;
    t[u].r=f;
    if(p) {
        if(t[p].l==f)   t[p].l=u;
        else            t[p].r=u;
    }
    else t[u].f=0,root=u;
    update(f);
    update(u);
    update(p);
}

void rotate(int u) {//单旋
/*
if (!fa[u]) return;
(ls[fa[u]]==u)? right(u):left(u);
*/
    int f=t[u].f;
    if(!f)  return;
    if(t[f].l==u)   right(u);
    else            left(u);
}

bool rotate2(int u) {//双旋,返回是否成功
/*
int f=fa[u];
int p=fa[f];
if (!p) return false;
if ((ls[f]==u)^(ls[p]==f))  rotate(u),rotate(u);
else                        rotate(f),rotate(u);
return true;
*/
    int f=t[u].f,p=t[f].f;
    if(!p)  return false;
    if((t[f].l==u)^(t[p].l==f)) rotate(u),rotate(u);
    else                        rotate(f),rotate(u);
    return true;
}

void splay(int u) {//伸展至根
/*
while(rotate2(u));
rotate(u);
*/
    //fprintf(stderr,"in splay %d root=%d t=%lx(%ld) [%lx,%lx]\n",u,root,t,SIZE(META(t)),mem_heap_lo(),mem_heap_hi());
    while(rotate2(u));
    rotate(u);
    //fprintf(stderr,"out splay\n");
}

void insert(void *p) {//向树中插入块 p
/*
int u=root,f=0;
while (u && k[u]!=key) {
    pushdown(u);
    f=u; u=(key<k[u])? ls[u]:rs[u];
}
if(!u) {
    u=new_node(key,value);
    if(f)   (key<k[f]? ls[f]:rs[f])=u;
    else    root=u;
    update(f);
    splay(u);
    return true;
}
splay(u);
return false;
*/
    //fprintf(stderr,"in insert\n");
    set_state(p,0);                  //预先将该块设为被占用,否则 new_node() 可能触发树的 realloc 抢占位置
    size_t size=SIZE(p),now_size=0;
    int u=root,f=0;
    while(u && (now_size=SIZE(t[u].data))!=size) {
        f=u;
        if(size<now_size)   u=t[u].l;
        else                u=t[u].r;
    }
    if(!u) {//没有对应大小的块,插入新节点
        u=new_node();
        if(f) {
            if(size<now_size)   t[f].l=u;
            else                t[f].r=u;
            update(f);
        }
        else root=u,t[u].f=0;
        t[u].data=p;
        set_state(p,1);
        *PREV(p)=(void*)(size_t)u;
        *NEXT(p)=NULL;
        splay(u);
    }
    else {//存在对应大小的块,加入链表
        *NEXT(p)=t[u].data;
        *PREV(p)=(void*)(size_t)u;
        *PREV(t[u].data)=p;
        set_state(p,1);
        set_state(t[u].data,2);
        t[u].data=p;
        splay(u);
    }
}

void next() {//在树中将树根的后继翻转到根
    ;//fprintf("stderr,"in next\n");
    int u=t[root].r;
    if(!u) return;
    while(t[u].l) u=t[u].l;
    splay(u);
}

void prev() {//在树中将树根的前驱翻转到根
    ;//fprintf("stderr,"in prev\n");
    int u=t[root].l;
    if(!u) return;
    while(t[u].r) u=t[u].r;
    splay(u);
}

int query(size_t size) {//查找大小不小于 size 的最小的块,没有找到返回 0
    if(!root) return 0; //树空直接返回,由于树空间是自持的可能此时树空间都没就绪
    int u=root,f=0;
    size_t now_size=0;
    while(u && (now_size=SIZE(t[u].data))!=size) {
        f=u;
        if(size<now_size)   u=t[u].l;
        else                u=t[u].r;
    }
    //fprintf(stderr,"in query %ld u=%lx, f=%lx\n",size,u,f);
    if(u)   return splay(u),u;
    else if(f && size<now_size) return splay(f),f;
    else if(f) {
        splay(f);
        if(!t[f].r)   return 0;
        else {
            next();
            return root;
        }
    }
    return 0;
}

void delete(void *p) {//将指定的块 p 从树中删除
    //fprintf(stderr,"in delete %lx state %d next %lx\n",p,STATE(p),*NEXT(p));
    int state=STATE(p);
    set_state(p,0);//预先将该块设为被占用,否则 delete_node 可能触发 tree_pool 的 realloc 抢占该位置
    if(state==2) {//链表节点,删掉就行
        void *prev=*PREV(p);
        void *next=*NEXT(p);
        if(next) *PREV(next)=prev;
        *NEXT(prev)=next;
    }
    else if(*NEXT(p)) {//有后续链表节点的树节点
        int u=(size_t)*PREV(p);
        void *next=*NEXT(p);
        *PREV(next)=(void*)(size_t)u;
        set_state(next,1);
        t[u].data=next;
    }
    else {//单个树节点,需要树删除
        int u=(size_t)*PREV(p);
        //fprintf(stderr,"deleting tree node %d\n",u);
        splay(u); next(); splay(u);
        int l=t[u].l,r=t[u].r;
        if(r) {
            t[r].l=l;
            update(r);
            root=r;
        }
        else root=l;
        if(root) t[root].f=0;
        delete_node(u);
    }
    //fprintf(stderr,"out delete\n");
}

void mm_print() {
    //fprintf(stderr,"mm_print with heap [%lx,%lx] size %ld: ",mem_heap_lo(),mem_heap_hi(),mem_heapsize());
    assert((char*)mem_heap_lo()<=(char*)mem_heap_hi());
    for(void *i=mem_heap_lo();i<=mem_heap_hi();i=NEXT_BLOCK(i)) {
        assert(*LEFT_SIZE(i)==*RIGHT_SIZE(i));
        //fprintf(stderr,"%ld(%ld) ",SIZE(i),STATE(i));
    }
    //fprintf(stderr,"\n");
}

void splay_print(int u) {
    if(!u) return;
    //fprintf(stderr,"empty block %d with size %ld, l=%d,r=%d,data=%lx,fa=%d,next=%lx,prev=%lx\n",u,SIZE(t[u].data),t[u].l,t[u].r,t[u].data,t[u].f,*NEXT(t[u].data),*PREV(t[u].data));
    assert(mem_heap_lo()<=t[u].data && t[u].data<=mem_heap_hi());
    for(void *i=*NEXT(t[u].data);i;i=*NEXT(i))
        assert(mem_heap_lo()<=t[u].data && t[u].data<=mem_heap_hi());
        //assert(i!=*NEXT(i)),fprintf(stderr,"\tempty block %lx next=%lx,prev=%lx\n",i,*NEXT(i),*PREV(i));
    splay_print(t[u].l);
    splay_print(t[u].r);
}

void *mm_get(size_t size) {
    //fprintf(stderr,"in mm_get\n");
    int u=query(size);
    void *p=NULL;
    if(u) {
        //fprintf(stderr,"found block %lx with size %ld\n",p,SIZE(p));
        p=t[u].data;
        delete(p);
        if(size+MIN_SIZE<SIZE(p)) {//空块分裂
            size_t empty_size=SIZE(p)-size;
            //fprintf(stderr,"empty size is %ld\n",empty_size);
            create_block(p,size,0);
            void *next=NEXT_BLOCK(p);
            //fprintf(stderr,"next is %lx\n",next);
            create_block(next,empty_size,0);
            insert(next);
        }
        else set_state(p,0);
    }
    else {
        //fprintf(stderr,"sbrking %ld\n",size);
        p=((char*)mem_heap_hi())+1;
        if(mem_heapsize() && STATE(PREV_BLOCK(p))) {
            p=PREV_BLOCK(p);
            //fprintf(stderr,"last is %lx\n",last);
            //fprintf(stderr,"found tail empty block %lx with size %ld\n",p,SIZE(p));
            delete(p);
            //fprintf(stderr,"delete p complete\n");
            if(!(SIZE(p)<size+MIN_SIZE)) {
                size_t empty_size=SIZE(p)-size;
                create_block(p,size,0);
                create_block(NEXT_BLOCK(p),empty_size,0);
                insert(NEXT_BLOCK(p));
            }
            else if(!(SIZE(p)<size)) set_state(p,0);
            else {
                if(mem_sbrk(size-SIZE(p))==(void*)-1)
                    return NULL;
                create_block(p,size,0);
            }
        }
        else {
            //fprintf(stderr,"sbrking all size %ld\n",size);
            p=mem_sbrk(size);
            if(p==(void*)-1) return NULL;
            else create_block(p,size,0);
        }
    }
    return p;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    //fprintf(stderr,"\nfor malloc(%ld):\n",size);
    size_t total_size = ALIGN(size+META_SIZE);
    void *p = mm_get(total_size);
    if(p) {
        //mm_print();
        //splay_print(root);
        return DATA(p);
    }
    //fprintf(stderr,"allocating failed with get 0\n");
    return NULL;
}

void mm_free(void *p) {
    p=META(p);
    size_t size=SIZE(p);
    //fprintf(stderr,"\nfor free %lx(%ld):\n",p,size);
    //mm_print();
    void *prev=PREV_BLOCK(p),*next=NEXT_BLOCK(p);
    if(mem_heap_lo()<p && STATE(prev)) {
        p=PREV_BLOCK(p);
        //fprintf(stderr,"deleting %lx(%ld)\n",p,SIZE(p));
        size+=SIZE(p);
        delete(p);
    }
    if(!(mem_heap_hi()<next) && STATE(next)) {
        size+=SIZE(next);
        delete(next);
    }
    create_block(p,size,0);
    //fprintf(stderr,"free delete complete with size %ld: \n",size);
    //splay_print(root);
    insert(p);
    //fprintf(stderr,"freed %ld %lx\n",SIZE(p),DATA(p));
    //mm_print();
    //splay_print(root);
}

void *mm_realloc(void *p, size_t size) {
    if(p==NULL) return mm_malloc(size);
    if(size==0) return mm_free(p),NULL;
    p=META(p);
    size=ALIGN(size+META_SIZE);
    void *prev=PREV_BLOCK(p),*next=NEXT_BLOCK(p);
    size_t origin_size=SIZE(p),prev_size=0,next_size=0;
    if(mem_heap_lo()<p && STATE(prev))
        prev_size=SIZE(prev);
    if(!(mem_heap_hi()<NEXT_BLOCK(p)) && STATE(next)) 
        next_size=SIZE(next);
    //fprintf(stderr,"reallocating p=%lx(%ld) prev=%lx(%ld) next=%lx(%ld), new_size=%ld\n",p,origin_size,prev,prev_size,next,next_size,size);
    if(!(origin_size<size+MIN_SIZE)) {//缩小过多,分裂出一个空块
        //fprintf(stderr,"shrink reallocating...\n");
        if(next_size) delete(next);//注意合并后面可能的空块
        create_block(p,size,0);
        next=NEXT_BLOCK(p);
        create_block(next,origin_size-size,0);
        insert(next);
        return DATA(p);
    }
    else if(!(origin_size<size)) return DATA(p);//容量不增,直接返回
    else if(!(origin_size+next_size<size)) {//后有足够的空块,直接合并
        //fprintf(stderr,"merge reallocating...\n");
        delete(next);
        if(!(origin_size+next_size<size+MIN_SIZE)) {//空块过大,还留有一部分
            create_block(p,size,0);
            next=NEXT_BLOCK(p);
            create_block(next,origin_size+next_size-size,0);
            insert(next);
        }
        else create_block(p,origin_size+next_size,0);//否则直接合并
        return DATA(p);
    }
    else if(!(prev_size+origin_size+next_size<size)) {//前后空块加起来足够,原地合并以提高利用率
        //fprintf(stderr,"merge reallocating...\n");
        delete(prev);
        if(next_size) delete(next);
        memmove(DATA(prev),DATA(p),origin_size-META_SIZE);//先移动防止 create_block 破坏数据,内存块可能重叠,使用 memmove
        if(DATA(p)==t) t=DATA(prev);
        if(DATA(p)==tree_pool) tree_pool=DATA(prev);    //如果是自持重分配,先赋值以免丢失过程中写入的数据
        if(!(prev_size+origin_size+next_size<size+MIN_SIZE)) {//空块过大,还留有一部分
            create_block(prev,size,0);
            next=NEXT_BLOCK(prev);
            create_block(next,prev_size+origin_size+next_size-size,0);
            //fprintf(stderr,"inserting next=%lx(%ld)\n",next,SIZE(next));
            insert(next);
        }
        else create_block(prev,prev_size+origin_size+next_size,0);//否则直接合并
        return DATA(prev);
    }
    else {//暴力移动,此时 free 和 malloc 的顺序无所谓了,反正这里不够大要从别的地方找
        //fprintf(stderr,"brute force reallocating...\n");
        next=mm_malloc(size);
        //splay_print(root);
        memcpy(next,DATA(p),origin_size-META_SIZE);//memcpy 有四种变体,你会写嘛 (孔乙己音)
        //splay_print(root);
        if(DATA(p)==t) t=next;
        //splay_print(root);
        if(DATA(p)==tree_pool) tree_pool=next;    //如果是自持重分配,先赋值以免丢失过程中写入的数据
        mm_free(DATA(p));
        return next;
    }
}
