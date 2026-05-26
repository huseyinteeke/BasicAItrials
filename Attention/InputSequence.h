#ifndef INPUT_SEQUENCE_H
#define INPUT_SEQUENCE_H

#include <string>
#include "config.h"

class InputSequence {
private:
    std::string name;
    double data[SEQ_LEN][D_EMB];

public:
    InputSequence(const std::string& seqName);
    //No need for a copy constructor or assignment operator since we are not managing dynamic memory
    ~InputSequence();
    double getValue(int row, int col) const;
};

#endif