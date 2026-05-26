#include "AttentionHead.h"
#include "ProjectionWeights.h"
#include "InputSequence.h"
#include <iostream>
#include <cmath>

using namespace std;

//TODO: Implement Constructor, Copy Constructor, Destructor
AttentionHead::AttentionHead(int id, double seed, const InputSequence* input):inputBatch(input)
{
    headId  = id;
    weights = new ProjectionWeights;
}
AttentionHead::AttentionHead(const AttentionHead& other):inputBatch(other.inputBatch)
{
    weights = other.weights;
    headId = other.headId
}


AttentionHead::~AttentionHead()
{

}

void AttentionHead::forwardPass() const {
    
    double Q[SEQ_LEN][D_K]{};
    double K[SEQ_LEN][D_K]{};
    double V[SEQ_LEN][D_K]{};
    weights->getQ(inputBatch, Q);
    weights->getK(inputBatch, K);
    weights->getV(inputBatch, V);

    // Q * K^T
    double scores[SEQ_LEN][SEQ_LEN]{};
    double scale{ sqrt(D_K) };
    
    for (int i = 0; i < SEQ_LEN; i++) { 
        for (int j = 0; j < SEQ_LEN; j++) { 
            scores[i][j] = 0.0;
            for (int k = 0; k < D_K; k++) {
                scores[i][j] += Q[i][k] * K[j][k]; 
            }
            scores[i][j] /= scale;
        }
    }

    // Softmax
    for (int i = 0; i < SEQ_LEN; i++) {
        double sum{ 0.0 };
        double max_val{ scores[i][0] };
        for(int j = 1; j < SEQ_LEN; j++) {
            if(scores[i][j] > max_val) max_val = scores[i][j];
        }
        
        for (int j = 0; j < SEQ_LEN; j++) {
            scores[i][j] = exp(scores[i][j] - max_val);
            sum += scores[i][j];
        }
        for (int j = 0; j < SEQ_LEN; j++) {
            scores[i][j] /= sum;
        }
    }

    // 3. Scores * V
    double context[SEQ_LEN][D_K]{};
    for (int i = 0; i < SEQ_LEN; i++) {
        for (int j = 0; j < D_K; j++) {
            context[i][j] = 0.0;
            for (int k = 0; k < SEQ_LEN; k++) {
                context[i][j] += scores[i][k] * V[k][j];
            }
        }
    }

    cout << "  Head " << headId << " Output Matrix (Context):" << endl;
    for (int i = 0; i < SEQ_LEN; i++) {
        cout << "    [ ";
        for (int j = 0; j < D_K; j++) {
            cout << context[i][j] << " ";
        }
        cout << "]" << endl;
    }
}