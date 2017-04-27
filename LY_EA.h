//
//  main.cpp
//  EvoAlg_Example
//
//  Created by Logan Yliniemi on 3/6/17.
//  Copyright © 2017 Logan Yliniemi. All rights reserved.
//

#include <iostream>
#include <random>
#include <time.h>
#include <assert.h>
#include <algorithm>

#define LYRAND (double)rand()/RAND_MAX
#define PI 3.1415296

using namespace std;



class policy{
public:
    
    void init(int nw);
    void eval(double f);
    void mutate(double amount);
    
    double fitness;
    int ts;
    vector<double> weights;
};

/// from google "sort a vector of a class"
////http://stackoverflow.com/questions/1380463/sorting-a-vector-of-custom-objects

struct less_than_key
{
    inline bool operator() (const policy& a, const policy& b)
    {
        return (a.fitness < b.fitness);
    }
};
///sort(vec.begin(), vec.end(), less_than_key());
/// end cite

void policy::eval(double f){
    fitness = f;
}

void policy::mutate(double mut_size){
    for(int i = 0; i<weights.size(); i++){
        if(rand()%2==0){
            weights.at(i)+= mut_size * LYRAND - mut_size * LYRAND;
        }
    }
}

void policy::init(int num_of_weights){
    vector<double> b;
    for(int i = 0; i<num_of_weights; i++){
        b.push_back(LYRAND-LYRAND);
    }
    weights = b;
}

vector<policy> EA_init(int pop_size, int num_weights){
    vector<policy> population;
    
    for(int i=0; i<pop_size; i++){
    
    }
    
    return population;
}

vector<policy> EA_replicate(vector<policy> P,int pop_size){
    vector<policy> population;
    population = P;
    
    assert(population.size() == pop_size/2);
    
    while(population.size()<pop_size){
        int spot = rand()%population.size();
        policy A;
        A = population.at(spot);
        A.mutate(0.2);
        //test_mututation(population.at(spot),A);
        population.push_back(A);
    }
    
    assert(population.size() == pop_size);
    
    return population;
}

vector<policy> EA_evaluate(vector<policy> P, vector<double> fit, int pop_size){
    vector<policy> population;
    population = P;
    for(int testing=0; testing<population.size(); testing++){
        population.at(testing).fitness = -1;
    }
    
    /// assign fitness
    for(int i=0; i<population.size(); i++){
        //population.at(i).eval();
        assert(population.at(i).fitness != -1);
    }
    
    for(int testing=0; testing<population.size(); testing++){
        assert(population.at(testing).fitness != -1);
    }
    assert(population.size() == pop_size);
    return population;
}

vector<policy> EA_downselect(vector<policy> P, int pop_size){
    vector<policy> population;
    /// population is empty
    assert(population.size() == 0);
    assert(P.size() == pop_size);
    
    bool binarytournament = true;
    bool strict = false;
    
    if(strict){
        /*
         /// select the top half, only.
         vector<double> fits;
         for(int i=0; i<P.size(); i++){
         fits.push_back(P.at(i).fitness);
         }
         sort(fits.begin(),fits.end());
         //cout << fits.at(0) << "\t" << fits.back() << endl;
         double survivorline = fits.at(pop_size/2);
         for(int i=P.size(); i>0; i--){
         if(P.at(i).fitness > survivorline){
         P.erase(P.begin()+i);
         }
         }
         */
        population = P;
        sort(population.begin(),population.end(),less_than_key());
        //cout << population.at(0).fitness << endl;
        //cout << population.at(1).fitness << endl;
        //cout << population.at(5).fitness << endl;
        
        population.erase(population.begin()+pop_size/2,population.end());
        
    }
    
    
    if(binarytournament){
        /// remove half by binary tournament
        while(population.size() < pop_size / 2){
            /// choose 2 at random
            int spot1 = rand()%P.size();
            int spot2 = rand()%P.size();
            while(spot2 == spot1){
                spot2 = rand()%P.size();
            }
            assert(spot1!=spot2);
            double fit1 = P.at(spot1).fitness;
            double fit2 = P.at(spot2).fitness;
            
            if(fit1<fit2){
                //// fit1 wins
                //  copy fit1 into "population"
                policy A1 = P.at(spot1);
                population.push_back(A1);
            }
            else if(fit2<=fit1){
                //// fit2 wins
                // copy fit2 into "population"
                policy A2 = P.at(spot2);
                population.push_back(A2);
            }
        }
    }
    
    assert(population.size() == pop_size/2);
    return population;
}
