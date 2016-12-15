#include "util.hpp"


template <typename T>
class Genetics
{
    public:
        Genetics(unsigned int populationSize, unsigned int numberBitsInIndividual, float probaMelt, float probaMutate, float mutationRate, int elitism,
             unsigned int numberCrossovers, bool sharing = false,
             const std::function<T(const std::vector<bool>&)>& evaluationFunction = std::function<T(const std::vector<bool>&)>(),
             const std::function<std::vector<bool>(unsigned int, unsigned int)>& generatePopulation = std::function<std::vector<bool>(unsigned int, unsigned int)>(),
             const std::function<T(unsigned int)>& exponentForScaling = std::function<T(unsigned int)>(),
             const std::function<T(const std::vector<bool>&, const std::vector<bool>&)>& distanceBetweenIndividuals = std::function<T(const std::vector<bool>&, const std::vector<bool>&)>(),
             T sigmaSharing = 0, T alphaSharing = 0,
             const std::function<void(float,const std::vector<bool>&)>& toCallWhenNewMax = std::function<void(float,const std::vector<bool>&)>());

        void generateInitialPopulation();
        void evaluate(unsigned int iteration, bool updateMap = false, bool scaling = true);
        void select();
        void melt();
        void mutate();

        void setExponentForScaling(const std::function<T(unsigned int)>& exponentForScaling);
        void setPopulationSize(unsigned int populationSize);
        void setNumberBitsInIndividual(unsigned int numberBits);
        void setProbaMelt(float p);
        void setProbaMutate(float p);
        void setMutationRate(float r);
        void setElitism(unsigned int e);
        void setNumberCrossovers(unsigned int n);
        void setSharing(bool share);

        std::map<T,std::vector<std::vector<bool> > > itere(unsigned int Niterations);

    private:
        std::vector<std::vector<bool> > population;
        std::vector<std::vector<bool> > newPopulation;
        std::vector<T> populationEvaluation;
        std::map<T, std::vector<unsigned int> > evaluationIndex;
        std::map<unsigned int, bool> elitMap;
        std::map<unsigned int, bool> elitMapNewPop;
        std::vector<bool> indexMelted;

        std::function<T(unsigned int)> exponentForScaling;
        std::function<std::vector<bool>(unsigned int, unsigned int)> generatePopulation;
        std::function<T(const std::vector<bool>&)> evaluationFunction;
        std::function<T(const std::vector<bool>&, const std::vector<bool>&)> distanceBetweenIndividuals;

        T currentMax;
        std::function<void(float,const std::vector<bool>&)> toCallWhenNewMax;

        float probaMelt;
        float probaMutate;
        bool sharing;
        int mutationRate;
        unsigned int populationSize;
        unsigned int numberBitsInIndividual;
        unsigned int elitism;
        unsigned int numberCrossovers;
        T sigmaSharing;
        T alphaSharing;

        std::default_random_engine generator;

        T evaluate(const std::vector<bool>& vec, unsigned int iteration, bool scaling) const;
        void shareBasic(T sigma, T alpha, bool updateMap);
};



template <typename T>
Genetics<T>::Genetics(unsigned int populationSize, unsigned int numberBitsInIndividual, float probaMelt, float probaMutate, float mutationRate, int elitism, unsigned int numberCrossovers, bool sharing, const std::function<T(const std::vector<bool>&)>& evaluationFunction, const std::function<std::vector<bool>(unsigned int, unsigned int)>& generatePopulation, const std::function<T(unsigned int)>& exponentForScaling, const std::function<T(const std::vector<bool>&, const std::vector<bool>&)>& distanceBetweenIndividuals, T sigmaSharing, T alphaSharing, const std::function<void(float,const std::vector<bool>&)>& toCallWhenNewMax)
{
    population.resize(populationSize);
    newPopulation.resize(populationSize);
    populationEvaluation.resize(populationSize);
    indexMelted.resize(populationSize);
    this->numberBitsInIndividual = numberBitsInIndividual;
    this->populationSize = populationSize;
    this->probaMelt = probaMelt;
    this->probaMutate = probaMutate;
    this->mutationRate = (int)(1.f/mutationRate);
    if(this->mutationRate<1)
        this->mutationRate = 1;
    if(elitism<(int)populationSize/4)
        this->elitism = elitism;
    else
        this->elitism = populationSize/4;
    this->numberCrossovers = numberCrossovers;
    this->sharing = sharing;
    this->exponentForScaling = exponentForScaling;
    this->generatePopulation = generatePopulation;
    this->evaluationFunction = evaluationFunction;
    this->distanceBetweenIndividuals = distanceBetweenIndividuals;
    this->sigmaSharing = sigmaSharing;
    this->alphaSharing = alphaSharing;
    this->toCallWhenNewMax = toCallWhenNewMax;
    this->currentMax = 0;
}

template <typename T>
void Genetics<T>::generateInitialPopulation()
{
    for(unsigned int i=0;i<populationSize;i++)
        population[i] = generatePopulation(i,numberBitsInIndividual);
}

template <typename T>
void Genetics<T>::evaluate(unsigned int iteration, bool updateMap, bool scaling)
{
    evaluationIndex.clear();
    for(unsigned int i=0;i<populationSize;i++)
    {
        T eval = evaluate(population[i],iteration,scaling);
        if(updateMap||elitism>0)
            evaluationIndex[-eval].push_back(i);
        /*std::cout<<i<<" => "<<eval<<std::endl;
        for(unsigned int j=0;j<population[i].size();j++)
            std::cout<<population[i][j];
        std::cout<<std::endl;*/
        populationEvaluation[i] = eval;
        if(eval>currentMax&&toCallWhenNewMax)
        {
            currentMax = eval;
            toCallWhenNewMax(evaluate(population[i],iteration,false),population[i]);
        }
    }

    elitMap.clear();
    unsigned int numberInElit = 0;
    for(typename std::map<T, std::vector<unsigned int> >::iterator it = evaluationIndex.begin();it != evaluationIndex.end();it++)
        for(unsigned int i=0;i<it->second.size()&&numberInElit<elitism;i++,numberInElit++)
        {
            std::cout<<it->second[i]<<" is now an elit with "<<evaluate(population[it->second[i]],iteration,false)<<std::endl;
            /*for(unsigned int j=0;j<population[it->second[i]].size();j++)
                std::cout<<population[it->second[i]][j];
            std::cout<<std::endl;*/
            elitMap[it->second[i]] = true;
        }

    if(sharing)
        shareBasic(sigmaSharing,alphaSharing,updateMap);
}

template <typename T>
void Genetics<T>::select()
{
    T sum = 0;
    for(unsigned int i=0;i<populationSize;i++)
        sum += populationEvaluation[i];

    T sumResiduals = 0;
    for(unsigned int i=0;i<populationSize;i++)
        sumResiduals += populationEvaluation[i]-floor(populationEvaluation[i]*populationSize/sum);

    //std::cout<<"Here is sum : "<<sum<<" and residuals : "<<sumResiduals<<std::endl;

    unsigned int nAdded = 0;
    elitMapNewPop.clear();
    for(unsigned int i=0;i<populationSize&&nAdded<populationSize;i++)
        if(populationEvaluation[i]>0)
        {
            unsigned int toAdd = floor(populationEvaluation[i]*populationSize/sum);
            //std::cout<<"For i = "<<i<<" we add "<<toAdd<<std::endl;
            for(unsigned int j=0;j<toAdd&&nAdded<populationSize;j++)
            {
                newPopulation[nAdded] = population[i];
                nAdded++;
            }
        }

    //std::cout<<"We added "<<nAdded<<" elements instead of "<<populationSize<<std::endl;
    int intSum = sumResiduals;
    std::uniform_int_distribution<int> distribution(0,1000*intSum);
    //std::cout<<"Sum : "<<1000*intSum<<std::endl;
    for(;nAdded<populationSize;nAdded++)
    {
        int delta = distribution(generator);
        //std::cout<<"delta generated : "<<delta<<" "<<intSum<<std::endl;
        unsigned int j = 0;
        while(delta>=0&&j<populationSize)
        {
            delta = delta-1000*(populationEvaluation[j]-floor(populationEvaluation[j]*populationSize/sum));
            if(delta>=0)
                j++;
        }
        if(j<populationSize)
        {
            //std::cout<<"We add population at index "<<j<<std::endl;
            newPopulation[nAdded] = population[j];
        }
        else
        {
            int index = rand()%populationSize;
            newPopulation[nAdded] = population[index];
        }
        //std::cout<<newPopulation[nAdded].size()<<" ";
    }
}

template <typename T>
void Genetics<T>::melt()
{
    int numberMelt = probaMelt*populationSize/2;
    for(unsigned int i=0;i<indexMelted.size();i++)
        indexMelted[i] = false;
    for(int i=0;i<numberMelt;i++)
    {
        int v1 = rand()%populationSize;
        if(indexMelted[v1])
            while(v1>=0&&(indexMelted[v1]||elitMap.count(v1)))
                v1--;
        if(v1>0)
        {
            int v2 = rand()%populationSize;
            if(indexMelted[v2])
                while(v2<(int)populationSize&&(indexMelted[v2]||elitMap.count(v2)))
                    v2++;
            if(v2<(int)populationSize)
            {
                //std::cout<<"We melt at index "<<v1<<" "<<v2<<std::endl;
                std::vector<unsigned int> cuts(numberCrossovers);
                for(unsigned int i=0;i<numberCrossovers;i++)
                    cuts[i] = rand()%(numberBitsInIndividual+1);
                std::sort(cuts.begin(),cuts.end());

                //std::cout<<newPopulation.size()<<" "<<v1<<" "<<numberBitsInIndividual<<" "<<v2<<std::endl;
                //std::cout<<newPopulation[v1].size()<<" "<<newPopulation[v2].size()<<std::endl;
                for(unsigned int j=0;j<numberBitsInIndividual;j++)
                {
                    //std::cout<<j<<" ";
                    bool temp = newPopulation[v1][j];
                    newPopulation[v1][j] = newPopulation[v2][j];
                    newPopulation[v2][j] = temp;
                }
                indexMelted[v1] = true;
                indexMelted[v2] = true;
            }
            else
                i--;
        }
        else
            i--;
    }
}

template <typename T>
void Genetics<T>::mutate()
{
    int numberMutations = probaMutate*populationSize;
    for(int i=0;i<numberMutations;i++)
    {
        int index = rand()%populationSize;
        for(unsigned int j=0;j<numberBitsInIndividual;j++)
            if(rand()%mutationRate==0)
                newPopulation[index][j] = 1-newPopulation[index][j];
    }
    std::map<unsigned int,bool> taken;
    for(std::map<unsigned int,bool>::iterator it=elitMap.begin();it!=elitMap.end();)
    {
        int index = rand()%populationSize;
        if(!taken.count(index))
        {
            newPopulation[index] = population[it->first];
            taken[index] = true;
            it++;
        }
    }
}

template <typename T>
std::map<T,std::vector<std::vector<bool> > > Genetics<T>::itere(unsigned int Niterations)
{
    generateInitialPopulation();

    for(unsigned int i=0;i<Niterations;i++)
    {
        evaluate(i,false);

        /*for(unsigned int i=0;i<population.size();i++)
        {
            for(unsigned int j=0;j<population[i].size();j++)
                std::cout<<population[i][j];
            std::cout<<" weight "<<populationEvaluation[i];
            std::cout<<std::endl;
        }*/

        if(sharing)
            ;
        select();

        /*std::cout<<"All that gives : "<<std::endl;
        for(unsigned int i=0;i<newPopulation.size();i++)
        {
            for(unsigned int j=0;j<newPopulation[i].size();j++)
                std::cout<<newPopulation[i][j];
            std::cout<<std::endl;
        }*/

        melt();
        mutate();
        population = newPopulation;
        std::random_shuffle(population.begin(),population.end());

        /*std::cout<<"Finally : "<<std::endl;
        for(unsigned int i=0;i<newPopulation.size();i++)
        {
            for(unsigned int j=0;j<newPopulation[i].size();j++)
                std::cout<<newPopulation[i][j];
            std::cout<<std::endl;
        }*/
    }

    evaluate(Niterations-1,true,false);
    std::map<T,std::vector<std::vector<bool> > > ret;
    for(typename std::map<T,std::vector<unsigned int> >::iterator it = evaluationIndex.begin();it != evaluationIndex.end();it++)
        for(unsigned int i=0;i<it->second.size();i++)
            ret[it->first].push_back(population[it->second[i]]);

    return ret;
}

template <typename T>
T Genetics<T>::evaluate(const std::vector<bool>& vec, unsigned int iteration, bool scaling) const
{
    T temp = evaluationFunction(vec);
    if(scaling&&temp>=0)
        return exp(exponentForScaling(iteration)*log(temp+0.000000001));
    else
        return temp;
}

template <typename T>
void Genetics<T>::shareBasic(T sigma, T alpha, bool updateMap)
{
    for(unsigned int i=0;i<populationSize;i++)
    {
        T sum = 1;
        for(unsigned int j=0;j<populationSize;j++)
            if(i!=j)
            {
                T d = distanceBetweenIndividuals(population[i],population[j]);
                //std::cout<<d<<" ";
                if(d<sigma)
                    sum += 1-(d/sigma)*alpha;
            }
        //std::cout<<std::endl<<std::endl;
        //std::cout<<sum<<" ";
        if(updateMap||elitism>0)
            evaluationIndex[-sum].push_back(i);
        populationEvaluation[i] = populationEvaluation[i]/sum;
    }
    //std::cout<<std::endl<<std::endl;
}
