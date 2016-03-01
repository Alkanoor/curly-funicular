#include "situation.hpp"


Situation::Situation(int N_lights, const std::vector<std::vector<int> >& _interruptors) :
    lightState(N_lights),
    interruptors(_interruptors)
{}

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
