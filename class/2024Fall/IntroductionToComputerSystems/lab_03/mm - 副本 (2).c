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
//For one block: ||size|node|user|size||
struct Node {
    void *l,*r,*fa,*p,*n;//左儿子,右儿子,父亲,链表下一项,链表上一项
};

/* 16 bytes alignment */
#define ALIGNMENT 16

#define PTR_SIZE sizeof(void*)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define HEAD_SIZE (ALIGN(sizeof(size_t)+sizeof(struct Node)))

#define META_SIZE (HEAD_SIZE+SIZE_T_SIZE)

#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define FLOOR(size) ((size) & ~(ALIGNMENT-1))

//p 指向块的第一个字节 (META(p) 中指向的是用户数据块的第一个字节)

#define USING(p) ((*(size_t*)(p))&1)

#define LEFT_SIZE(p) ((size_t*)(p))

#define SIZE(p) (FLOOR(*LEFT_SIZE(p)))

#define RIGHT_SIZE(p) ((size_t*)((char*)(p)+SIZE(p)-SIZE_T_SIZE))

#define PREV_BLOCK(p) ((void*)((char*)(p)-FLOOR((*(size_t*)((char*)(p)-SIZE_T_SIZE)))))

#define DATA(p) ((void*)((char*)(p)+HEAD_SIZE))

#define META(p) ((void*)((char*)(p)-HEAD_SIZE))

#define NEXT_BLOCK(p) ((void*)((char*)(p)+(SIZE(p))))

#define NODE(p) ((struct Node*)((char*)(p)+SIZE_T_SIZE))

void *empty=NULL;  //Splay root

void update(void *u) {//更新节点的信息
    if(!u) return;
    void *l=NODE(u)->l,*r=NODE(u)->r;
    if(l) NODE(l)->fa=u;
    if(r) NODE(r)->fa=u;
}

void left(void **root,void *u) {//将 u 代表的块左旋
/*
int f=fa[u];
int p=fa[f];
rs[f]=ls[u];
ls[u]=f;
if(p) (ls[p]==f? ls[p]:rs[p])=u;
else fa[u]=0,root=u;
*/
    if(!(*root) || !u) return;
    void *f=NODE(u)->fa;
    void *p=NODE(f)->fa;
    NODE(f)->r=NODE(u)->l;
    NODE(u)->l=f;
    if(p) {
        if(NODE(p)->l==f)   NODE(p)->l=u;
        else                NODE(p)->r=u;
    }
    else {
        NODE(u)->fa=NULL;
        *root=u;
    }
    update(f);
    update(u);
    update(p);
}

void right(void **root,void *u) {//将 u 代表的块右旋
/*
int f=fa[u];
int p=fa[f];
ls[f]=rs[u];
rs[u]=f;
if(p) (ls[p]==f? ls[p]:rs[p])=u;
else fa[u]=0,root=u;
*/
    if(!(*root) || !u) return;
    void *f=NODE(u)->fa;
    void *p=NODE(f)->fa;
    NODE(f)->l=NODE(u)->r;
    NODE(u)->r=f;
    if(p) {
        if(NODE(p)->l==f)   NODE(p)->l=u;
        else                NODE(p)->r=u;
    }
    else {
        NODE(u)->fa=NULL;
        *root=u;
    }
    update(f);
    update(u);
    update(p);
}

void rotate(void **root,void *u) {//块单旋
/*
if (!fa[u]) return;
(ls[fa[u]]==u)? right(u):left(u);
*/
    if(!(*root) || !u)  return;
    void *f=NODE(u)->fa;
    if(!f)  return;
    if(NODE(f)->l==u)   right(root,u);
    else                left(root,u);
}

bool rotate2(void **root,void *u) {//块双旋,返回是否成功
/*
int f=fa[u];
int p=fa[f];
if (!p) return false;
if ((ls[f]==u)^(ls[p]==f))  rotate(u),rotate(u);
else                        rotate(f),rotate(u);
return true;
*/
    if(!(*root) || !u) return false;
    void *f=NODE(u)->fa;
    if(!f) return false;
    void *p=NODE(f)->fa;
    if(!p) return false;
    if((NODE(f)->l==u)^(NODE(p)->l==f)) rotate(root,u),rotate(root,u);
    else                                rotate(root,f),rotate(root,u);
    return true;
}

void splay(void **root,void *u) {//将块伸展至根
/*
while(rotate2(u));
rotate(u);
*/
    ;//fprintf("stderr,"in splay\n");
    while(rotate2(root,u));
    rotate(root,u);
}

void search(void **root,size_t size,void **u,void **f) {//查找一个大小为 size 的块,返回其链表头块指针 u (可能为 NULL)和父节点指针 f
    *u=*root; *f=NULL;
    while(*u && SIZE(*u)!=size) {
        *f=*u;
        if(size<SIZE(*u))   *u=NODE(*u)->l;
        else                *u=NODE(*u)->r;
    }
}

void insert(void **root,void *p) {//向树中插入一个指针为 p 的块(大小从块的元数据中读取),允许重复,保证成功
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
    void *u=*root,*f=NULL;
    search(root,SIZE(p),&u,&f);
    if(!u) {//没有对应大小的块
        if(f) {
            if(SIZE(p)<SIZE(f)) NODE(f)->l=p;
            else                NODE(f)->r=p;
            update(f);
        }
        else *root=p,NODE(p)->fa=NULL;
        NODE(p)->n=NULL;
        NODE(p)->p=NULL;
        NODE(p)->l=NULL;
        NODE(p)->r=NULL;
    }
    else {//存在对应大小的块,加入链表
        if(f) {
            if(NODE(f)->l==u)   NODE(f)->l=p;
            else                NODE(f)->r=p;
            update(f);
        }
        else *root=p;
        *NODE(p)=*NODE(u);
        NODE(p)->n=u;
        NODE(p)->p=NULL;
        NODE(u)->p=p;
        update(p);
    }
    splay(root,p);
}

void next(void **root) {//在树中将树根的后继翻转到根
    ;//fprintf("stderr,"in next\n");
    if(!(*root)) return;
    void *u=NODE(*root)->r;
    if(!u) return;
    while(NODE(u)->l) u=NODE(u)->l;
    splay(root,u);
}

void prev(void **root) {//在树中将树根的前驱翻转到根
    ;//fprintf("stderr,"in prev\n");
    if(!(*root)) return;
    void *u=NODE(*root)->l;
    if(!u) return;
    while(NODE(u)->r) u=NODE(u)->r;
    splay(root,u);
}

void *query(void **root,size_t size) {//查找大小不小于 size 的最小的块,没有找到返回 NULL
    ;//fprintf("stderr,"in query\n");
    void *u=NULL,*f=NULL;
    search(root,size,&u,&f);
    //fprintf(stderr,"in query u=%lx, f=%lx\n",u,f);
    if(u)   return splay(root,u),u;
    else if(f && size<SIZE(f))   return splay(root,f),f;
    else if(f) {
        splay(root,f);
        if(!(NODE(f)->r))   return NULL;
        else {
            next(root);
            return *root;
        }
    }
    return NULL;
}

void delete(void **root,void *p) {//将指定的块 p 从树中删除
    //fprintf(stderr,"in delete\n");
    if(NODE(p)->p) {
        void *prev=NODE(p)->p,*next=NODE(p)->n;
        if(next) NODE(next)->p=prev;
        NODE(prev)->n=next;
    }
    else if(NODE(p)->n) {
        void *n=NODE(p)->n,*f=NODE(p)->fa;
        void *l=NODE(p)->l,*r=NODE(p)->r;
        if(f) {
            if(NODE(f)->l==p)   NODE(f)->l=n;
            else                NODE(f)->r=n;
            update(f);
        }
        else *root=n,NODE(n)->fa=NULL;
        NODE(n)->l=l;
        NODE(n)->r=r;
        NODE(n)->p=NULL;
        update(n);
    }
    else {
        splay(root,p);
        next(root);
        splay(root,p);
        void *l=NODE(p)->l,*r=NODE(p)->r;
        if(r) {
            NODE(r)->l=l;
            update(r);
            *root=r;
        }
        else *root=l;
        if(*root) NODE(*root)->fa=NULL;
    }
    //fprintf(stderr,"out delete\n");
}

void mm_print() {
    //fprintf(stderr,"mm_print with heap [%lx,%lx] size %ld: ",mem_heap_lo(),mem_heap_hi(),mem_heapsize());
    assert((char*)mem_heap_lo()<=(char*)mem_heap_hi());
    for(void *i=mem_heap_lo();i<=mem_heap_hi();i=NEXT_BLOCK(i)) {
        //fprintf(stderr,"%ld(%ld) ",SIZE(i),USING(i));
    }
    //fprintf(stderr,"\n");
}

void splay_print(void *u) {
    if(!u) return;
    //fprintf(stderr,"empty block %lx size %ld with l=%lx,r=%lx,n=%lx,p=%lx,fa=%lx\n",u,SIZE(u),NODE(u)->l,NODE(u)->r,NODE(u)->n,NODE(u)->p,NODE(u)->fa);
    splay_print(NODE(u)->l);
    splay_print(NODE(u)->r);
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    empty=NULL;
    return 0;
}

void *mm_get(size_t size) {
    ;//fprintf("stderr,"in mm_get\n");
    void *p=query(&empty,size);
    if(p) {
        //fprintf(stderr,"found block %lx with size %ld\n",p,SIZE(p));
        delete(&empty,p);
        if(size+META_SIZE<SIZE(p)) {
            size_t empty_size=SIZE(p)-size;
            //fprintf(stderr,"empty size is %ld\n",empty_size);
            *LEFT_SIZE(p)=size|1;
            *RIGHT_SIZE(p)=size|1;
            void *next=NEXT_BLOCK(p);
            //fprintf(stderr,"next is %lx\n",next);
            *LEFT_SIZE(next)=empty_size;
            *RIGHT_SIZE(next)=empty_size;
            insert(&empty,next);
        }
        else {
            size=SIZE(p);
            *LEFT_SIZE(p)=size|1;
            *RIGHT_SIZE(p)=size|1;
        }
    }
    else {
        //fprintf(stderr,"sbrking %ld\n",size);
        if(mem_heapsize()) {
            void *last=((char*)mem_heap_hi())+1;
            last=PREV_BLOCK(last);
            //fprintf(stderr,"last is %lx\n",last);
            if(!USING(last)) {
                delete(&empty,last);
                mem_sbrk(size-SIZE(last));
                *LEFT_SIZE(last)=size|1;
                *RIGHT_SIZE(last)=size|1;
                return last;
            }
        }
        void *p=mem_sbrk(size);
        if(p==(void*)-1) return NULL;
        else {
            *LEFT_SIZE(p)=size|1;
            *RIGHT_SIZE(p)=size|1;
            return p;
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
        mm_print();
        splay_print(empty);
        return DATA(p);
    }
    //fprintf(stderr,"allocating failed with get 0\n");
    return NULL;
}

/*
 * mm_free
 */
void mm_free(void *p) {
    //fprintf(stderr,"\nfor free(%lx):\n",(unsigned long)p);
    p=META(p);
    void *l=p,*r=p;
    size_t size=SIZE(p);
    while(mem_heap_lo()<l && !USING(PREV_BLOCK(l))) {
        l=PREV_BLOCK(l);
        size+=SIZE(l);
        //fprintf(stderr,"found neibour empty block %lx size %ld\n",l,SIZE(l));
        delete(&empty,l);
    }
    while(NEXT_BLOCK(r)<=mem_heap_hi() && !USING(NEXT_BLOCK(r))) {
        r=NEXT_BLOCK(r);
        size+=SIZE(r);
        //fprintf(stderr,"found neibour empty block %lx size %ld\n",r,SIZE(r));
        delete(&empty,r);
    }
    //fprintf(stderr,"free delete complete: \n");
    splay_print(empty);
    *LEFT_SIZE(l)=size;
    *RIGHT_SIZE(l)=size;
    insert(&empty,l);
    //fprintf(stderr,"freeing %ld %lx\n",SIZE(l),(unsigned long)DATA(l));
    mm_print();
    splay_print(empty);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
  void *oldptr = ptr;
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = SIZE(META(oldptr));
  if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}
