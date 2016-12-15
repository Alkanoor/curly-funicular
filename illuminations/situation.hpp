#ifndef SITUATION_HPP_INCLUDED
#define SITUATION_HPP_INCLUDED


#include <vector>


class Situation
{
    public:
        Situation(int N_lights, const std::vector<std::vector<int> >& _interruptors);

        float weight(const std::vector<bool>& bits);
        float weight(const std::vector<int>& coeffs);

    private:
        std::vector<bool> lightState;
        std::vector<std::vector<int> > interruptors;
        std::vector<std::vector<int> > lightStateIntegers;
};


#endif
