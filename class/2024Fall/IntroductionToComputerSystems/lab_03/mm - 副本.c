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

/* 16 bytes alignment */
#define ALIGNMENT 16

#define PTR_SIZE sizeof(void*)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define META_SIZE (2*SIZE_T_SIZE)

#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define FLOOR(size) ((size) & ~(ALIGNMENT-1))

#define USING(p) ((*(size_t*)(p))&1)

#define LEFT_SIZE(p) ((size_t*)(p))

#define SIZE(p) (FLOOR(*LEFT_SIZE(p)))

#define RIGHT_SIZE(p) ((size_t*)((char*)(p)+SIZE(p)-SIZE_T_SIZE))

#define PREV_BLOCK(p) ((void*)((char*)(p)-FLOOR((*(size_t*)((char*)(p)-SIZE_T_SIZE)))))

#define DATA(p) ((void*)((char*)(p)+SIZE_T_SIZE))

#define META(p) ((void*)((char*)(p)-SIZE_T_SIZE))

#define NEXT_BLOCK(p) ((void*)((char*)(p)+(SIZE(p))))

/*for one block: ||size|node|user|size||*/

void mem_print() {
    for(void *i=mem_heap_lo();i<=mem_heap_hi();i=NEXT_BLOCK(i))
        ;//fprintf(stderr,"%ld(%ld) ",SIZE(i),USING(i));
    ;//fprintf(stderr,"\n");
}

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void) {
    return 0;
}

void *mem_search(size_t size) {
    ;//fprintf(stderr,"start mem_search with heap [%lx,%lx]\n",(unsigned long)mem_heap_lo(),(unsigned long)mem_heap_hi());
    void *p=mem_heap_lo();
    while(p<=mem_heap_hi() && (USING(p) || (SIZE(p)<size && NEXT_BLOCK(p)<=mem_heap_hi())))
        p=NEXT_BLOCK(p);
    if(p<=mem_heap_hi() && SIZE(p)<size) {
        mem_sbrk(size-SIZE(p));
        *LEFT_SIZE(p)=size;
        *RIGHT_SIZE(p)=size;
    }
    return p<=mem_heap_hi()? p:NULL;
}

void *mem_get(size_t *size) {
    void *p=NULL;
    if(!mem_heapsize() || !(p=mem_search(*size))) {
        ;//fprintf(stderr,"sbrking %ld\n",*size);
        void *p=mem_sbrk(*size);
        return (p==(void *)-1)? NULL:p;
    }
    else {
        if(SIZE(p)<=(*size)+META_SIZE) 
            *size=SIZE(p);
        else {
            size_t origin_size=SIZE(p);
            *LEFT_SIZE(p)=(*size)|1;
            *RIGHT_SIZE(p)=(*size)|1;
            *LEFT_SIZE(NEXT_BLOCK(p))=(origin_size-(*size));
            *RIGHT_SIZE(NEXT_BLOCK(p))=(origin_size-(*size));
        }
    }
    return p;
}

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    ;//fprintf(stderr,"\nfor malloc(%ld):\n",size);
    size_t newsize = ALIGN(SIZE_T_SIZE+ size + SIZE_T_SIZE);
    void *p = mem_get(&newsize);
    if(p) {
        *LEFT_SIZE(p)=newsize|1;
        *RIGHT_SIZE(p)=newsize|1;
        ;//fprintf(stderr,"allocating %ld %lx\n",size,(unsigned long)DATA(p));
        mem_print();
        return DATA(p);
    }
    ;//fprintf(stderr,"allocating failed with get 0\n");
    return p;
}

/*
 * mm_free
 */
void mm_free(void *p) {
    ;//fprintf(stderr,"\nfor free(%lx):\n",(unsigned long)p);
    p=META(p);
    void *l=p,*r=p;
    size_t size=SIZE(p);
    while(mem_heap_lo()<l && !USING(PREV_BLOCK(l)))
        l=PREV_BLOCK(l),size+=SIZE(l);
    while(NEXT_BLOCK(r)<=mem_heap_hi() && !USING(NEXT_BLOCK(r)))
        r=NEXT_BLOCK(r),size+=SIZE(r);
    *LEFT_SIZE(l)=size;
    *RIGHT_SIZE(l)=size;
    ;//fprintf(stderr,"freeing %ld %lx\n",SIZE(l),(unsigned long)DATA(l));
    mem_print();
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
  copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
  if (size < copySize)
    copySize = size;
  memcpy(newptr, oldptr, copySize);
  mm_free(oldptr);
  return newptr;
}
