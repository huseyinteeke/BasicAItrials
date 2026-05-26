#include <iostream>
#include "InputSequence.h"
#include "MultiHeadAttention.h"

using namespace std;

//g++ main.cpp InputSequence.cpp ProjectionWeights.cpp AttentionHead.cpp MultiHeadAttention.cpp -o main && ./main
int main() {

    InputSequence* batchData{ new InputSequence{"Input_Embeddings"} };

    {
        MultiHeadAttention mhaBlock{ 2, batchData };
        mhaBlock.execute();
    }

    delete batchData;
    
    return 0;
}