#include "situation.hpp"


Situation::Situation(int N_lights, const std::vector<std::vector<int> >& _interruptors) :
    lightState(N_lights),
    interruptors(_interruptors)
{
    lightStateIntegers.resize(N_lights);
    for(unsigned int i=0;i<N_lights;i++)
    {
        lightStateIntegers[i].resize(interruptors.size()/32+1);
        unsigned int cur = 0;
        lightStateIntegers[i][cur] = 0;
        int o = 31;
        for(unsigned int j=0;j<interruptors.size();j++)
        {
            bool ok = false;
            for(unsigned int k=0;k<interruptors[j].size()&&!ok;k++)
                if(interruptors[j][k]==i)
                    ok = true;
            if(ok)
                lightStateIntegers[i][cur] += (1<<o);
            o--;
            if(o<0)
            {
                o = 31;
                cur++;
                lightStateIntegers[i][cur] = 0;
            }
        }
    }
}

float Situation::weight(const std::vector<bool>& bits)
{
    for(unsigned int i=0;i<lightState.size();i++)
        lightState[i] = false;
    for(unsigned int i=0;i<interruptors.size()&&i<bits.size();i++)
        if(bits[i])
            for(unsigned int j=0;j<interruptors[i].size();j++)
                lightState[interruptors[i][j]] = 1-lightState[interruptors[i][j]];

    float ret = 0;
    for(unsigned int i=0;i<lightState.size();i++)
        if(lightState[i])
            ret++;
    return ret;
}
//#include <fstream>
//#include "util.hpp"
float Situation::weight(const std::vector<int>& coeffs)
{
    float ret = 0;
    //std::ofstream log("logqdsqsd.txt",std::ios::out);
    for(unsigned int i=0;i<lightStateIntegers.size();i++)
    {
        int cur = 0;
        for(unsigned int j=0;j<coeffs.size();j++)
        {
            //log<<convertIntToBinaryString(coeffs[j])<<" "<<convertIntToBinaryString(lightStateIntegers[i][j])<<std::endl;
            cur ^= coeffs[j]&lightStateIntegers[i][j];
            //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        }
        cur = (cur & 0xffff) ^ ((cur >> 16) & 0xffff);
        //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        cur = (cur & 0xff) ^ ((cur >> 8) & 0xff);
        //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        cur = (cur & 0xf) ^ ((cur >> 4) & 0xf);
        //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        cur = (cur & 0x3) ^ ((cur >> 2) & 0x3);
        //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        cur = (cur & 0x1) ^ ((cur >> 1) & 0x1);
        //log<<cur<<"=>"<<convertIntToBinaryString(cur)<<std::endl;
        ret += cur;
        //log<<"==================================================";
    }
    //exit(0);
    return ret;
}
