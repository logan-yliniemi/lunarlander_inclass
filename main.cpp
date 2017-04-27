//
//  main.cpp
//  LunarLanderInClass
//
//  Created by Logan Yliniemi on 4/4/17.
//  Copyright © 2017 Logan Yliniemi. All rights reserved.
//

#include <iostream>

#include <vector>
#include <assert.h>
#include <time.h>
#include <random>

#include "LY_NN.h"
#include "LY_EA.h"

using namespace std;

/// equations to mathematically simulate it
/// kinematics

//// velocity
//// gravity -> acceleration (force)
//// Position

#define gravity 1.48
#define timestep 0.1

class rocket{
public:
    double mass;
    double mass_craft;
    double mass_fuel;
    double y;
    double fuel;
    double ydot;
    double yddot;
    double thrust;
    double max_thrust;
    
    void calc_y();
    void calc_ydot();
    void calc_yddot();
    void calc_mass();
    void simulate();
    void init();
};

void rocket::calc_mass(){
    const double FUELCONSTANT = 45;
    mass_fuel = fuel * FUELCONSTANT;
    mass = mass_craft + mass_fuel;
}

void rocket::calc_yddot(){
    yddot = thrust/mass - gravity;
}

void rocket::calc_ydot(){
    ydot = yddot * timestep + ydot;
}

void rocket::calc_y(){
    y = ydot * timestep + y;
}

void rocket::init(){
    y = 100;
    ydot = -10;
    yddot = 0;
    mass_craft = 1;
    fuel = 0;
    calc_mass();
    thrust = 0;
    max_thrust = 3*mass*gravity;
}

void rocket::simulate(){
    calc_mass();
    calc_yddot();
    calc_ydot();
    calc_y();
}

int main() {
    int MAX_GENERATIONS = 300;
    int pop_size = 100;
    srand(time(NULL));
    
    rocket Lander;
    Lander.init();
    
    neural_network NN;
    NN.setup(2,5,1);
    
    NN.set_in_min_max(-5,105); /// Y
    NN.set_in_min_max(-20, 0); /// YDOT
    NN.set_out_min_max(0, Lander.max_thrust);
    
    /// inputs are 1) Y and 2) YDOT
    
    /// INITIALIZE
    vector<policy> population;
    for(int i=0; i<pop_size; i++){
        policy A;
        A.init(NN.get_number_of_weights());
        population.push_back(A);
    }
    assert(population.size() == pop_size);
    
    //vector<policy> population = EA_init();
    for(int generation = 0; generation < MAX_GENERATIONS; generation++){
        /// EVALUATE -------------------------------
        // fitness: find final velocity of the Lander.
        // do once for each policy
        for(int sim=0; sim<pop_size; sim++){
            /// run simulation using "sim"th policy
            NN.set_weights(population.at(sim).weights, true);
            
            int tstep = 0;
            while(Lander.y >= 0){
                /// give NN the state
                vector<double> state;
                state.push_back(Lander.y);
                state.push_back(Lander.ydot);
                NN.set_vector_input(state);
                /// let NN compute what thrust should be
                NN.execute();
                Lander.thrust = NN.get_output(0);
                /// simulate a time step
                Lander.simulate();
                
                population.at(sim).fitness = fabs(Lander.ydot);
                population.at(sim).ts = tstep;
                
                tstep++;
                
                if(tstep > 1000){
                    break;
                }
            }
        }
        /// DOWNSELECT -------------------------------
        // remove half
        
        population = EA_downselect(population, pop_size);
        
        /// REPOPULATION -------------------------------
        // replenish half again (100%)
        
        population = EA_replicate(population, pop_size);
        
        cout << "GENERATION " << generation << endl;
    }
    
    sort(population.begin(), population.end(),less_than_key());
    
    for(int i=0; i<pop_size; i++){
        cout << i << "\t" << population.at(i).fitness << "\t" << population.at(i).ts <<endl;
    }
    
    
    return 0;
}



















