#include "ProjectionWeights.h"
#include "InputSequence.h"
#include <iostream>

using namespace std;

//TODO: Implement Constructor, Copy Constructor, Destructor
ProjectionWeights::ProjectionWeights(double seed) {
    for (int i = 0; i < D_EMB; i++) {
        for (int j = 0; j < D_K; j++) {
            W_q[i][j] = seed * 0.1 + (i * 0.05) - (j * 0.02);
            W_k[i][j] = seed * -0.1 + (j * 0.05);
            W_v[i][j] = seed * 0.2 - (i * 0.01) + (j * 0.03);
        }
    }
}


//TODO Implement getQ(), getK(), getV() methods using the project() method
void ProjectionWeights::project(const InputSequence* input, const double W[D_EMB][D_K], double out[SEQ_LEN][D_K]) const {
    
    //TODO Implement the projection logic: out = input * W
    
}
