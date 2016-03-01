#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED


#ifndef M_PI
#define M_PI 3.14159265
#endif


#include <functional>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <list>
#include <map>



static std::vector<std::array<unsigned char,3> > trios;

template <typename T>
T evaluate(const std::vector<bool>& vec, unsigned int maxNumberLetters)
{
    T ret = 36*15;
    std::map<unsigned char, bool> hasBeenFound;
    for(unsigned int i=0;i<vec.size();i++)
        if(vec[i])
        {
            ret++;
            for(unsigned int j=0;j<3;j++)
                hasBeenFound[trios[i][j]] = true;
        }

    if(hasBeenFound.size()>maxNumberLetters)
        ret = (36-(hasBeenFound.size()-maxNumberLetters))*15;

    return ret;
}

std::vector<bool> generateRandom(unsigned int cur, unsigned int N);
std::vector<bool> generateNull(unsigned int cur, unsigned int N);
float tanForExponent(unsigned int n, unsigned int N, float p);
std::vector<std::array<unsigned char,3> > readTrios(const std::string& path);
float evaluateSumBits(const std::vector<bool>& vec);
float hammingDistance(const std::vector<bool>& v1, const std::vector<bool>& v2);
std::vector<int> convertBinToInt(const std::vector<bool>& v);
std::string convertIntToBinaryString(int i);


















std::vector<std::vector<unsigned char> > convertStringToMat(const std::string& str, unsigned int size);

template <typename T>
std::vector<std::vector<T> > transposeWithSameSize(const std::vector<std::vector<T> >& ref)
{
    std::vector<std::vector<T> > ret(ref.size());
    for(unsigned int i=0;i<ref.size();i++)
        ret[i].resize(ref[i].size());
    unsigned int i2 = 0, j2 = 0;
    for(unsigned int i=0;i<ref.size();i++)
        for(unsigned int j=0;j<ref[i].size();j++)
        {
            ret[i][j] = ref[i2][j2];
            i2++;
            if(i2>=ref.size())
            {
                i2 = 0;
                j2++;
            }
        }

    return ret;
}

std::vector<int> multMat(const std::vector<std::vector<int> >& ref, const std::vector<int>& toMultiplyWith, int modulo = 36);

template <typename T, typename U>
std::vector<std::vector<U> > apply(const std::vector<std::vector<T> >& ref, std::map<T,U>& correspondances)
{
    std::vector<std::vector<U> > ret(ref.size());
    for(unsigned int i=0;i<ref.size();i++)
    {
        ret[i].resize(ref[i].size());
        for(unsigned int j=0;j<ref[i].size();j++)
            ret[i][j] = correspondances[ref[i][j]];
    }

    return ret;
}

template <typename T, typename U>
std::vector<std::vector<U> > apply(const std::vector<std::vector<T> >& ref, std::map<T,std::vector<U> >& correspondances)
{
    std::vector<std::vector<U> > ret(ref.size());
    for(unsigned int i=0;i<ref.size();i++)
    {
        ret[i].resize(ref[i].size());
        for(unsigned int j=0;j<ref[i].size();j++)
            if(correspondances.count(ref[i][j]))
                ret[i][j] = correspondances[ref[i][j]][rand()%correspondances[ref[i][j]].size()];
            else
                ret[i][j] = 0;
    }

    return ret;
}

template <typename T>
void print(const std::vector<std::vector<T> >& mat)
{
    for(unsigned int i=0;i<mat.size();i++)
    {
        for(unsigned int j=0;j<mat[i].size();j++)
            std::cout<<mat[i][j]<<" ";
        std::cout<<std::endl;
    }
}

template <typename T>
void printNoSpace(const std::vector<std::vector<T> >& mat)
{
    for(unsigned int i=0;i<mat.size();i++)
    {
        for(unsigned int j=0;j<mat[i].size();j++)
            std::cout<<mat[i][j];
        std::cout<<std::endl;
    }
}


#endif
