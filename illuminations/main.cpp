#include "geneticCore.hpp"
#include "situation.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void printNewMax(float score, const std::vector<bool>& seq)
{
    for(unsigned int i=0;i<seq.size();i++)
        std::cout<<seq[i];
    std::cout<<std::endl<<"gives a score of "<<score<<std::endl;
}

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(std::getline(ss, tok, delimiter))
        internal.push_back(tok);

    return internal;
}

int main()
{
    /*std::vector<std::vector<int> > interruptors(3);
    interruptors[0] = std::vector<int>(3);
    interruptors[1] = std::vector<int>(2);
    interruptors[2] = std::vector<int>(2);
    interruptors[0][0] = 1;
    interruptors[0][1] = 2;
    interruptors[0][2] = 3;
    interruptors[1][0] = 1;
    interruptors[1][1] = 2;
    interruptors[2][0] = 0;
    interruptors[2][1] = 4;

    std::vector<bool> test(3);
    test[0] = 0;
    test[1] = 0;
    test[2] = 0;
    Situation s(5,interruptors);
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 1;
    test[1] = 0;
    test[2] = 0;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 0;
    test[1] = 1;
    test[2] = 0;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 0;
    test[1] = 0;
    test[2] = 1;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 1;
    test[1] = 1;
    test[2] = 0;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 0;
    test[1] = 1;
    test[2] = 1;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 1;
    test[1] = 0;
    test[2] = 1;
    std::cout<<s.weight(test)<<std::endl;
    test[0] = 1;
    test[1] = 1;
    test[2] = 1;
    std::cout<<s.weight(test)<<std::endl;*/


    std::ifstream input("input.txt",std::ios::in);
    std::string tmp;
    int nLights=0, size=0, cur=0;
    std::vector<std::vector<int> > interruptors;
    while(input>>tmp)
    {
        std::vector<std::string> x = split(tmp,',');
        if(!interruptors.size())
        {
            std::istringstream iss(x[0]);
            iss>>nLights;
            std::istringstream iss2(x[1]);
            iss2>>size;
            std::cout<<nLights<<" "<<size<<std::endl;
            interruptors.resize(size);
            cur = 0;
        }
        else
        {
            interruptors[cur].resize(x.size());
            for(unsigned int i=0;i<x.size();i++)
            {
                std::istringstream iss(x[i]);
                iss>>interruptors[cur][i];
            }
            cur++;
        }
    }

    Situation s(nLights,interruptors);

    srand(time(NULL));

    std::ofstream logSuper("superLog.txt",std::ios::out|std::ios::trunc);

    unsigned int nbGenerations = 10000;
    Genetics<float> genetics(100,nLights,0.6,0.3,0.01,1,3,true,
                     std::bind(&Situation::weight,s,std::placeholders::_1),
                     std::bind(&generateRandom,std::placeholders::_1,std::placeholders::_2),
                     std::bind(&tanForExponent,std::placeholders::_1,nbGenerations,0.1),
                     std::bind(&hammingDistance,std::placeholders::_1,std::placeholders::_2),
                     100,0.68,
                     std::bind(&printNewMax,std::placeholders::_1,std::placeholders::_2));

    std::map<float,std::vector<std::vector<bool> > > res = genetics.itere(nbGenerations);

    unsigned int k = 0;
    std::ofstream out("out.txt",std::ios::out|std::ios::trunc);
    for(std::map<float,std::vector<std::vector<bool> > >::iterator it = res.begin();it != res.end()&&k<200;it++)
    {
        for(unsigned int i=0;i<it->second.size()&&i<200;i++)
        {
            out<<"Following seq has a weight of "<<-it->first<<std::endl;
            for(unsigned int j=0;j<it->second[i].size();j++)
                out<<(it->second[i])[j];
            out<<std::endl;
        }
        k+=it->second.size();
    }
}
