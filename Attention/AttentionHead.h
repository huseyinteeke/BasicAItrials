#ifndef ATTENTION_HEAD_H
#define ATTENTION_HEAD_H

#include "config.h"

class ProjectionWeights;
class InputSequence;

class AttentionHead {
private:
    int headId;
    ProjectionWeights* weights; // COMPOSITION
    const InputSequence* inputBatch;  // AGGREGATION (read-only)

public:
    AttentionHead(int id, double seed, const InputSequence* input);
    AttentionHead(const AttentionHead& other);
    ~AttentionHead();
    void forwardPass() const;
};

#endif