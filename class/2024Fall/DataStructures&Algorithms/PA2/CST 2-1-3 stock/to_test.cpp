#include <cstdio>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

class Node
{
private:
public:
    unsigned int shares;
    /*[startDay, endDay)*/
    unsigned int startDay;
    // unsigned int endDay;
    Node *next;
    Node *prev;


    Node(unsigned int _shares, unsigned int _startDay, unsigned int _endDay)
    {
        this->shares = _shares;
        this->startDay = _startDay;
        // this->endDay = _endDay;
        this->next = nullptr;
        this->prev = nullptr;
    }
    Node(Node *n)
    {
        this->shares = n->shares;
        this->startDay = n->startDay;
        // this->endDay = n->endDay;
        this->next = nullptr;
        this->prev = nullptr;
    }
    void insertPrev(Node *n)
    {
        n->prev = this->prev;
        n->next = this;
        n->prev->next = n;
        this->prev = n;
    }
};

class List
{
private:
public:
    unsigned int size;
    Node *head;
    Node *tail;
    List()
    {
        size = 0;
        head = new Node(0, 0, 0);
        tail = new Node(0, 0, 0);
        head->prev = nullptr;
        head->next = tail;
        tail->prev = head;
        tail->next = nullptr;
    }
    ~List()
    {
        while(size--)
            remove(head->next);
        delete head;
        delete tail;
    }
    void insertPrev(unsigned int _shares, unsigned int _startDay, Node *n)
    {
        Node *nd = new Node(_shares, _startDay, _startDay);
        n->insertPrev(nd);
        size++;
    }
    void insertPrev(Node *n1, Node *n2)
    {
        Node *nd = new Node(n1);
        n2->insertPrev(nd);
        size++;
    }
    void remove(Node *n)
    {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        size--;
        delete n;
    }
    Node * first()
    {
        return head->next;
    }
    Node * last()
    {
        return tail->prev;
    }
};

class Queue: public List
{
public:
    void enqueue(unsigned int _shares, unsigned int _startDay)
    {
        insertPrev(_shares, _startDay, tail);
    }
    void enqueue(Node *nd)
    {
        insertPrev(nd, tail);
    }
    void dequeue()
    {
        remove(head->next);
    }
    void queue_remove(Node *n)
    {
        remove(n); // 不懂有没有问题
    }
    Node * top()
    {
        return head->next;
    }
    bool empty()
    {
        return size == 0;
    }
};



unsigned int str2uint(string s, unsigned int begin, unsigned int end)
{
    unsigned int res = 0;
    for (int i = begin; i < end; i++)
        res = res * 10 + s[i] - '0';
    return res;
}

void getInput(string input, unsigned int &days, unsigned int &shares, int &sign)
{
    int i = 0;
    int j = 0;
    while ('0' <= input[i] && input[i] <= '9')
    {
        i++;
        if(input[i] == ' ')
            j = 1;
    }
    days = str2uint(input, 0, i);
    if(j == 0)
    {
        shares = 0;
        sign = 0;
    }
    else
    {
        i++;
        j = i;
        while('0' <= input[i] && input[i] <= '9')
            i++;
        shares = str2uint(input, j, i);
        sign = 1;
    }
}


void printQueue(Queue *q)
{
    Node *nd = q->first();
    printf("-------\n");
    while(nd != q->tail)
    {
        printf("%u ", nd->startDay);
        nd = nd->next;
    }
    printf("\n-------\n");
}


Queue *momoStocks = new Queue();
Queue *timeStocks = new Queue();

/**
 * 可能是两个股之间有一段时间是没有股的
*/

int main()
{
    unsigned int day = 0; // 现在的时间
    unsigned int beginCountDay = 0;
    unsigned int N;
    unsigned long long int sum = 0;
    // momoStocks->enqueue(0, 0);
    // timeStocks->enqueue(0, 0); // 初始化
    scanf("%u", &N);
    cin.ignore();
    for (int i = 0; i < 2 * N; i++)
    {
        unsigned int days, shares;
        int sign = 0; // 用于判断到底有没有第二个输入参数
        string input;
        getline(cin, input);
        sign=sscanf(input.c_str(),"%u%u", &days, &shares);
        //getInput(input, days, shares, sign);
        if(sign != 1)
        {
            day += days;
            if(timeStocks->empty())
                beginCountDay = day;
            timeStocks->enqueue(shares, day);
            momoStocks->enqueue(shares, day);
            //puts("?");
            // printf("++++++\n%u %u\n++++++\n", momoStocks->top()->startDay, timeStocks->top()->startDay);
            if(shares >= momoStocks->top()->shares)
            {
                sum += 1ll*momoStocks->top()->shares * (day - beginCountDay);
                while(momoStocks->top()->startDay != day) // 因为当前的shares的前面不可能会有股的shares比它大，或者和它相等，判别也许可以改成时间
                    momoStocks->dequeue();
                beginCountDay = day;
            }
            else // 如果不是，就要判断，当前节点的前面有没有比他小的，如果有，就删掉
            {
                Node *nd1 = momoStocks->first();
                Node *nd2;
                while(nd1->startDay != day)
                {
                    // puts("?");
                    nd2 = nd1->next;
                    if(nd1->shares <= shares)
                    {
                        momoStocks->queue_remove(nd1);
                    }
                    nd1 = nd2;
                }
            }
            // printQueue(momoStocks);
            // printQueue(timeStocks);`
        }
        else if(sign == 1)
        {
            day += days;
            if(momoStocks->top()->startDay == timeStocks->top()->startDay) // 如果现在的队列的头到达endDay要更新
            {
                sum += 1ll*momoStocks->top()->shares * (day - beginCountDay);
                momoStocks->dequeue();
                beginCountDay = day;
            }
            timeStocks->dequeue();
            // printQueue(momoStocks);
            // printQueue(timeStocks);
        }
        // printf("-----\n%u %u %llu\n%u %u\n%u\n------\n", momoStocks->top()->shares, momoStocks->top()->startDay, sum, timeStocks->top()->shares, timeStocks->top()->startDay, beginCountDay);
    }
    printf("%llu\n", sum);
    // 为什么这里不用delete
    // delete[] timeStocks;
    // delete[] momoStocks;4
    return 0;
}

// 1
// 0 100
// 5
// 0
