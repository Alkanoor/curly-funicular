#include "geneticCore.hpp"
#include "situation.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

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


    std::map<int,std::map<int,std::vector<int> > > lights;
    for(unsigned int i=0;i<1000;i++)
    {
        std::vector<int> okVec;
        for(unsigned int j=0;j<interruptors.size();j++)
        {
            bool ok = false;
            for(unsigned int k=0;k<interruptors[j].size()&&!ok;k++)
                if(interruptors[j][k]==i)
                    ok = true;
            if(ok)
                okVec.push_back(j);
        }
        lights[okVec.size()][i] = okVec;
    }

    for(auto it : lights)
    {
        std::cout<<it.first<<":"<<std::endl;
        for(auto it2 : it.second)
            std::cout<<it2.first<<" ";
        std::cout<<std::endl;
    }

    exit(0);

    Situation s(nLights,interruptors);

    srand(time(NULL));

    std::vector<std::vector<bool> > pool1(10000);
    std::vector<std::vector<int> > pool2(10000), pool3(10000);

    std::clock_t begin = std::clock();

    for(unsigned int i=0;i<10000;i++)
    {
        pool1[i].resize(300);
        pool2[i].resize(300/32+1);
    }

    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for resizing "<<std::endl;

    begin = std::clock();
    for(unsigned int i=0;i<10000;i++)
        for(unsigned int j=0;j<300;j++)
            pool1[i][j] = rand()%2;
    end = std::clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for filling method 1"<<std::endl;

    begin = std::clock();
    for(unsigned int i=0;i<10000;i++)
        for(unsigned int j=0;j<300/32+1;j++)
            pool2[i][j] = rand();
    end = std::clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for filling method 2"<<std::endl;

    begin = std::clock();
    for(unsigned int i=0;i<10000;i++)
        if(i==0)
        {
            std::cout<<s.weight(pool1[i])<<std::endl;
            auto tmp = convertBinToInt(pool1[i]);
            for(unsigned int j=0;j<pool1[i].size();j++)
                if(pool1[i][j])
                    std::cout<<1;
                else
                    std::cout<<0;
            std::cout<<std::endl;
            for(unsigned int j=0;j<tmp.size();j++)
                std::cout<<convertIntToBinaryString(tmp[j]);
            std::cout<<std::endl;
        }
        else if(i<20)
            std::cout<<s.weight(pool1[i])<<std::endl;
        else
            s.weight(pool1[i]);
    end = std::clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for testing method 1"<<std::endl;


    begin = std::clock();
    for(unsigned int i=0;i<10000;i++)
        pool3[i] = convertBinToInt(pool1[i]);
    end = std::clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for establishing correspondances"<<std::endl;

    begin = std::clock();
    for(unsigned int i=0;i<10000;i++)
        if(i<20)
            std::cout<<s.weight(pool3[i])<<std::endl;
        else
            s.weight(pool3[i]);
    end = std::clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout<<elapsed_secs<<" for testing method 2"<<std::endl;



    /*std::ofstream logSuper("superLog.txt",std::ios::out|std::ios::trunc);

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
    }*/
}
