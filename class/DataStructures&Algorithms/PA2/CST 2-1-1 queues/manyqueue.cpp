#include"manyqueue.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

Vector::Vector() {
    value=new unsigned int[MIN_SIZE];
    capacity=MIN_SIZE;
    tail=0;
}
Vector::~Vector() {
    delete [] value;
}
unsigned int Vector::size() {
    return tail;
}
void Vector::expand() {
    capacity*=2;
    unsigned int *new_value=new unsigned int[capacity];
    for (unsigned int i=0; i<tail; ++i) {
        new_value[i]=value[i];
    }
    delete [] value;
    value=new_value;
}
void Vector::push_back(unsigned int x) {
    if (tail>=capacity) expand();
    value[tail++]=x;
}
void Vector::shrink() {
    if (tail*2<capacity && capacity>MIN_SIZE) {
        capacity/=2;
        unsigned int *new_value=new unsigned int[capacity];
        for(unsigned int i=0;i<tail;++i) {
            new_value[i]=value[i];
        }
        delete [] value;
        value=new_value;
    }
}
unsigned int Vector::pop_back() {
    if (!tail) exit(1);
    unsigned int result=value[--tail];
    shrink();
    return result;
}
unsigned int& Vector::operator[](unsigned int x) {
    if(x>=tail) exit(1);
    return value[x];
}

QueueManager::QueueManager(unsigned int m){
    memset(head, 0, sizeof head);
}
QueueManager::~QueueManager(){
}
void QueueManager::push(unsigned int k, unsigned int x){
    queue[k].push_back(x);
}
void QueueManager::pop(unsigned int k){
    if (head[k]<queue[k].size()) {
        ++head[k];
        if (head[k]*2>=queue[k].size()) {
            for (unsigned int i=head[k]; i<queue[k].size(); ++i) {
                queue[k][i-head[k]]=queue[k][i];
            }
            for (unsigned int i=0; i<head[k]; ++i) {
                queue[k].pop_back();
            }
            head[k]=0;
        }
    }
}
unsigned int QueueManager::query(unsigned int k, unsigned int i){
    if (head[k]>=queue[k].size()) {
        return 0;
    }
    if (head[k]+i-1>=queue[k].size()) {
        return queue[k][queue[k].size()-1];
    }
    return queue[k][head[k]+i-1];
}
