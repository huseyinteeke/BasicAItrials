#ifndef PROJECTION_WEIGHTS_H
#define PROJECTION_WEIGHTS_H

#include "config.h"

class InputSequence; 

class ProjectionWeights {
private:
    double W_q[D_EMB][D_K];
    double W_k[D_EMB][D_K];
    double W_v[D_EMB][D_K];

    void project(const InputSequence* input, const double W[D_EMB][D_K], double out[SEQ_LEN][D_K]) const;

public:
    ProjectionWeights(double seed);
    ProjectionWeights(const ProjectionWeights& other);
    ~ProjectionWeights();

    void getQ(const InputSequence* input, double Q[SEQ_LEN][D_K]) const;
    void getK(const InputSequence* input, double K[SEQ_LEN][D_K]) const;
    void getV(const InputSequence* input, double V[SEQ_LEN][D_K]) const;
};

#endif