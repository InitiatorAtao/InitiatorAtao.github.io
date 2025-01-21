#include<string>
#include<map>
#include<vector>
#include<sstream>
#include<iostream>
#include<cstring>
#include"Parser.h"
using namespace std;

string toupper(string s)
{
    for(char &c:s)
    {
        c=toupper(c);
    }
    return s;
}

void Parser::error(string s)
{
    cout<<"usage: ./main [--help]";
    for(auto p:arguments)
    {
        cout<<" [--"<<p.first<<" "<<toupper(p.first)<<"]";
    }
    cout<<endl;
    cout<<s;
    exit(0);
}

void Parser::add_argument(string name,int def,string help)
{
    arguments[name.substr(2)]=make_pair(def,help);
}

void Parser::init(string args)
{
    istringstream io(args);
    vector<string> vs;
    string arg;
    while(getline(io,arg,' '))
    {
        if(arg=="--help")
        {
            cout<<"usage: ./main [--help]";
            for(auto p:arguments)
            {
                cout<<" [--"<<p.first<<" "<<toupper(p.first)<<"]";
            }
            cout<<endl<<endl<<"optional arguments:"<<endl;
            cout<<"--help   "<<"show this help message and exit"<<endl;
            for(auto p:arguments)
            {
                cout<<"--"<<p.first<<" "<<toupper(p.first)<<"   "<<p.second.second<<endl;
            }
            exit(0);
        }
        vs.push_back(arg);
    }
    for(int i=0;i<vs.size();++i)
    {
        string s=vs[i];
        string name=s.substr(2);
        if(!arguments.count(name))
            error("./main: error: unrecognized arguments: "+s);
        if(i+1>=vs.size())
            error("./main: error: argument "+s+": expected an argument");
        string values=vs[++i];
        int value=0;
        try
        {
            value=stoi(values);
        }
        catch(const std::invalid_argument& e)
        {
            error("./main: error: argument "+s+": invalid int value: '"+values+"'");
        }
        argument_values[name]=value;
    }
}

int Parser::get_argument(string name)
{
    if(!argument_values.count(name))
        return arguments[name].first;
    return argument_values[name];
}