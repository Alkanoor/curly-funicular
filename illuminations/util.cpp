#include "util.hpp"


std::vector<bool> generateRandom(unsigned int cur, unsigned int N)
{
    std::vector<bool> ret(N);
    for(unsigned int i=0;i<N;i++)
        ret[i] = rand()%2;
    return ret;
}

std::vector<bool> generateNull(unsigned int cur, unsigned int N)
{
    std::vector<bool> ret(N);
    for(unsigned int i=0;i<N;i++)
        ret[i] = 0;
    return ret;
}

float tanForExponent(unsigned int n, unsigned int N, float p)
{
    return exp(p*log(tan(M_PI/2.f*(float)(n+1)/(float)(N+1))));
}

std::vector<std::array<unsigned char,3> > readTrios(const std::string& path)
{
    std::ifstream ifs(path.c_str(),std::ios::in);
    std::vector<std::array<unsigned char,3> > ret;
    unsigned char temp;
    unsigned int cur = 3;
    while(ifs>>temp)
    {
        if(cur>=3)
        {
            ret.push_back(std::array<unsigned char,3>());
            cur = 0;
        }
        ret[ret.size()-1][cur] = temp;
        cur++;
    }
    return ret;
}

float evaluateSumBits(const std::vector<bool>& vec)
{
    float ret = 0;
    for(unsigned int i=0;i<vec.size();i++)
        if(vec[i])
            ret++;
    return ret;
}

float hammingDistance(const std::vector<bool>& v1, const std::vector<bool>& v2)
{
    float ret = 0;
    for(unsigned int i=0;i<v1.size();i++)
        ret += (v1[i]!=v2[i]);
    return ret;
}

std::vector<int> convertBinToInt(const std::vector<bool>& v)
{
    std::vector<int> ret((v.size()-1)/32+1);
    unsigned int cur = 0;
    ret[cur] = 0;
    int o = 31;
    for(unsigned int i=0;i<v.size();i++)
    {
        if(v[i])
            ret[cur] += (1<<o);
        o--;
        if(o<0)
        {
            o = 31;
            cur++;
            ret[cur] = 0;
        }
    }

    return ret;
}

std::string convertIntToBinaryString(int i)
{
    std::string ret;
    ret.resize(32);
    for(unsigned int j=0;j<32;j++)
        if(i&(1<<(31-j)))
            ret[j] = '1';
        else
            ret[j] = '0';
    return ret;
}



















std::vector<std::vector<unsigned char> > convertStringToMat(const std::string& str, unsigned int size)
{
    std::vector<std::vector<unsigned char> > ret(size);
    for(unsigned int i=0;i<str.size();i++)
        ret[i%size].push_back(str[i]);

    return ret;
}

std::vector<int> multMat(const std::vector<std::vector<int> >& ref, const std::vector<int>& toMultiplyWith, int modulo)
{
    std::vector<int> ret(ref[0].size());
    for(unsigned int i=0;i<ref[0].size();i++)
    {
        int k = 0;
        for(unsigned int j=0;j<toMultiplyWith.size()&&j<ref.size();j++)
            k+=toMultiplyWith[j]*ref[j][i];
        if(modulo)
            ret[i] = k%modulo;
        else
            ret[i] = k;
    }
    return ret;
}
