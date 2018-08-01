#ifndef NeuralNet
#define NeuralNet

class Neural_net
{
    public:
        // Constructor and Destructor
        Neural_net(int numIn, int numHid, int numOut);
        ~Neural_net();

        // Saving and Loading
        bool saveWeights(char* outFile);
        bool loadWeights(char* inFile);

        // Training and Classifying
        void setLearningRate(float lr);
        void setMomentum(float m);
        float getLearningRate();
        float getMomentum();

        void trainBatch(float** inputs, int* outputs, int numTests, int maxEpochs);
        float* trainLive(float* in, int out);
        std::pair<float, int> classify(float* in);

    private:
        // Layer Sizes - Set at initialization
        int sizeInput, sizeHidden, sizeOutput;

        // Layer Neurons - Dynamically allocated arrays
        float* neuronsInput;
        float* neuronsHidden;
        float* neuronsOutput;

        // Layer Weights - Dynamically allocated arrays
        float** weightsInputToHidden;
        float** weightsHiddenToOutput;

        // Training Data - Private data used for training
        float learningRate;
        float momentum;

        float* errorHidden;
        float* errorOutput;
        float* idealOutput;
        float** oldDeltaWeightInputToHidden;
        float** oldDeltaWeightHiddenToOutput;
        float** newDeltaWeightInputToHidden;
        float** newDeltaWeightHiddenToOutput;

        // Functions - Private member functions
        void initializeWeights();
        float activationFunction(float in);
        void feedForward(float* in);
};

#endif
