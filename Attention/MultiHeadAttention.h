#ifndef MULTI_HEAD_ATTENTION_H
#define MULTI_HEAD_ATTENTION_H

class AttentionHead;
class InputSequence;

class MultiHeadAttention {
private:
    int numHeads;
    AttentionHead** heads; // COMPOSITION
    const InputSequence* x;      // AGGREGATION (read-only)

public:
    MultiHeadAttention(int n_heads, const InputSequence* inputTensor);
    MultiHeadAttention(const MultiHeadAttention& other);
    ~MultiHeadAttention();
    void execute() const;
};

#endif