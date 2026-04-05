#include <iostream>
#include <cstring>
#include <random>


using namespace std;
class Matrix
{
private:
    static mt19937& getGenerator()
    {
        static mt19937 gen(random_device{}());
        return gen;
    }

    static uniform_real_distribution<float>& getDistribution()
    {
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        return dis;
    }

    float** data;
    int rows;
    int cols;
public:
    Matrix(int rows  , int cols)
    :rows(rows) , cols(cols)
    {
        data = new float*[rows];
        
        for(int i = 0 ; i < rows ; i++)
        {
            (data[i]) = new float[cols];

            for (int j = 0; j < cols; j++) {
                data[i][j] = 0.0f;
            }
        }

    }

    Matrix(const Matrix& op)
    {
        this->rows = op.rows; this->cols = op.cols;
        data = new float*[rows];
        for(int i = 0 ; i < rows ; i++)
        {
            (data[i]) = new float[cols];

            for (int j = 0; j < cols; j++) {
                data[i][j] = op[i][j];
            }
        }
    }

    ~Matrix()
    {
        for(int i = 0 ; i < rows ; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }



    float* operator[](int idx) const
    {
        return data[idx];
    }

    Matrix& operator=(const Matrix& operand)
    {
        if(this == &operand) return *this;
        if(this->rows != operand.rows || this->cols != operand.cols){
            for(int i = 0 ; i < this->rows ; i++){
                delete[] this->data[i];
            }
            delete[] this->data;


            this->rows = operand.rows;
            this->cols = operand.cols;

        this->data = new float*[this->rows];
        for(int i = 0 ; i < this->rows ; i++) {
            this->data[i] = new float[this->cols];
        }
            
        }

        for(int i = 0 ; i < this->rows ; i ++)
        {
            for(int j = 0 ; j < this->cols ; j++)
            {
                (*this)[i][j] = operand[i][j];
            }
        }

        return (*this);
    }


    Matrix operator+(const Matrix& mtrx)
    {
        Matrix result(getRows() , getCols());

        for(int i = 0 ; i < getRows() ; i++)
        {
            int mtrx_row = (mtrx.rows == 1) ? 0 : i;
            for(int j = 0 ; j < getCols() ; j++)
            {
                result[i][j] = (*this)[i][j] + mtrx[mtrx_row][j];
            }
        }

        return result;
    }


    Matrix operator*(const Matrix& mtrx)
    {   
        Matrix result(this->rows , mtrx.cols);

        for(int i = 0 ; i < this->rows ; i++)
        {
            for(int j = 0 ; j < mtrx.cols ; j++)
            {
                float sum = 0.0f;
                for(int k = 0 ; k < this->cols ; k++)
                {
                    sum += (*this)[i][k] * mtrx[k][j];
                }
                result[i][j] = sum;
            }
        }
        return result; 
    }



    void fillRandom()
    {
        for(int i = 0 ; i < rows ; i ++)
        {
            for(int j = 0; j < cols ; j++)
            {
                (*this)[i][j] = getDistribution()(getGenerator());
            }
        }
    }

    

    void print()
    {
        for(int i = 0 ; i < this->rows ; i++){
            cout<<"\n";
         for(int j = 0 ; j < this->cols ; j++){
            cout<<" "<<(*this)[i][j];
         }
        }

    }

    const int getRows()
    {
        return this->rows;
    }

    const int getCols()
    {
        return this->cols;
    }
};





class Activation
{
private:
    char* type;
    float relu(float  value)
    {
        if(value > 0) return value;
        else return 0.0f;
    }

    float sigmoid(float value)
    {
        return (1 / (1 + expf(-value)));
    }
public:
    Activation(const char* type)
    {
        this->type = new char[strlen(type) + 1];
        strcpy(this->type , type);
    }
    
    Activation(const Activation& operand)
    {

        this->type = new char[strlen(operand.type) + 1];
        strcpy(this->type , operand.type);
        
    }

    float operator()(float op)
    {
        float result;
        if(strcmp(this->type , "relu") == 0)
        {
            return relu(op);
        }
        else if(strcmp(this->type , "sigmoid") == 0)
        {
            return sigmoid(op);
        }else{
            cout<<"Not giving correct type";
            return 0.0f;
        }
    }
  
    
    ~Activation(){
        if(this->type) delete[] this->type;
    }
};




class Layer
{
private:
    Matrix* weights;
    Matrix* biases;
    Activation& activation;
public:
    Layer(int inputSize , int outputSize , Activation& activation)
    :activation(activation)
    {
        weights = new Matrix(inputSize  , outputSize);
        biases  = new Matrix(1 , outputSize);
        initializeWeights();
    }

    Matrix forward(Matrix op)
    {
        Matrix output = (op * (*weights)) + (*biases);
        for(int i = 0; i < output.getRows(); i++) {
        for(int j = 0; j < output.getCols(); j++) {
            output[i][j] = activation(output[i][j]); 
        }
    }
        return output;
    }

    void initializeWeights()
    {
        weights->fillRandom();
        biases->fillRandom();
    }

    ~Layer()
    {
        delete weights;
        delete biases;
    }
};


class Network
{
private:
    Layer** layers;
    int layer_count = 0;
    int max_layers;
public:
    Network(int maxLayers): max_layers(maxLayers)
    {
        layers = new Layer*[max_layers];
    }
    ~Network()
    {
        delete[] layers;
    }

    bool addLayer(Layer* layer)
    {
        
        if (layer_count >= max_layers) {
        std::cout << "Hata: Ag maksimum kapasiteye (" << max_layers << ") ulasti! Daha fazla katman eklenemez.\n";
        return false; 
        }
        layers[layer_count] = layer;
        layer_count++;
        
        return true;
    }

    Matrix forward(Matrix matrix)
    {
        Matrix mtrx(matrix);
        for(int i = 0 ; i < layer_count ; i++)
        {
            mtrx = layers[i]->forward(mtrx);       
        }
        return mtrx;

    }

    int getLayerCount() {return layer_count;}

};

int main() {
    Activation relu{"relu"};
    Activation sigmoid{"sigmoid"};

    // Create a simple neural network for XOR problem
    // Input layer: 2 neurons
    // Hidden layer: 3 neurons with ReLU activation
    // Output layer: 1 neuron with Sigmoid activation

    // Create layers
    Layer hiddenLayer{2, 3, relu};
    Layer outputLayer{3, 1, sigmoid};

    // Create network with a maximum of 2 layers and add layers
    Network network{2};
    network.addLayer(&hiddenLayer);
    network.addLayer(&outputLayer);

    std::cout << "Neural Network with " << network.getLayerCount() << " layers created." << std::endl;

    // Create input data for XOR problem
    // [0,0], [0,1], [1,0], [1,1]
    Matrix input{4, 2};
    input[0][0] = 0.0f; input[0][1] = 0.0f;
    input[1][0] = 0.0f; input[1][1] = 1.0f;
    input[2][0] = 1.0f; input[2][1] = 0.0f;
    input[3][0] = 1.0f; input[3][1] = 1.0f;

    std::cout << "Input data:" << std::endl;
    input.print();

    Matrix output = network.forward(input);

    std::cout << "Network output:" << std::endl;
    output.print();

    std::cout << "Note: Since weights are randomly initialized, the output won't match XOR truth table yet." << std::endl;
    std::cout << "Training would be needed to adjust weights for correct predictions." << std::endl;

    return 0;
}