#include "xor.h"

double sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}

double sigmoid_derivative(double x)
{
    double sigmoidOfX = sigmoid(x);
    return sigmoidOfX * (1 - sigmoidOfX);
}

double random_double()
{
    return (double)rand() / RAND_MAX * 2 - 1;
}

void fill_array_randomly(double *array, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        array[i] = random_double();
    }
}

void forward(double inputs[INPUT_SIZE],
             double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE],
             double hidden[HIDDEN_SIZE],
             double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE],
             double hiddenBiases[HIDDEN_SIZE],
             double outputs[OUTPUT_SIZE],
             double outputBiases[OUTPUT_SIZE])
{
    // Compute hidden layer values.
    for (size_t i = 0; i < HIDDEN_SIZE; i++)
    {
        hidden[i] = 0;
        for (size_t j = 0; j < INPUT_SIZE; j++)
        {
            hidden[i] += inputs[j] * inputToHiddenWeights[j][i];
        }

        hidden[i] += hiddenBiases[i];
        hidden[i] = sigmoid(hidden[i]);
    }

    // Compute output layer values.
    for (size_t i = 0; i < OUTPUT_SIZE; i++)
    {
        outputs[i] = 0;
        for (size_t j = 0; j < HIDDEN_SIZE; j++)
        {
            outputs[i] += hidden[j] * hiddenToOutputWeights[j][i];
        }

        outputs[i] += outputBiases[i];
        outputs[i] = sigmoid(outputs[i]);
    }
}

void backward(double inputs[INPUT_SIZE],
              double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE],
              double hidden[HIDDEN_SIZE],
              double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE],
              double hiddenBiases[HIDDEN_SIZE],
              double target,
              double outputs[OUTPUT_SIZE],
              double outputBiases[OUTPUT_SIZE])
{
    // Compute output layer error deltas.
    double outputDeltas[OUTPUT_SIZE];
    for (size_t i = 0; i < OUTPUT_SIZE; i++)
    {
      outputDeltas[i] = (target - outputs[i]) * sigmoid_derivative(outputs[i]);
    }

    // Update hidden to output weights and output layer biases.
    for (size_t i = 0; i < OUTPUT_SIZE; i++)
    {
        for (size_t j = 0; j < HIDDEN_SIZE; j++)
        {
            hiddenToOutputWeights[j][i] += LEARNING_RATE 
		           * outputDeltas[i] * hidden[j];
        }

        outputBiases[i] += LEARNING_RATE * outputDeltas[i];
    }

    // Compute hidden layer error deltas.
    double hiddenDeltas[HIDDEN_SIZE];
    for (size_t i = 0; i < HIDDEN_SIZE; i++)
    {
        hiddenDeltas[i] = 0;
        for (size_t j = 0; j < OUTPUT_SIZE; j++)
        {
            hiddenDeltas[i] += outputDeltas[j] * hiddenToOutputWeights[i][j];
        }
        hiddenDeltas[i] *= sigmoid_derivative(hidden[i]);
    }

    // Update input to hidden weights and hidden layer biases.
    for (size_t i = 0; i < HIDDEN_SIZE; i++)
    {
        for (size_t j = 0; j < INPUT_SIZE; j++)
        {
            inputToHiddenWeights[j][i] += LEARNING_RATE 
		          * hiddenDeltas[i] * inputs[j];
        }

        hiddenBiases[i] += LEARNING_RATE * hiddenDeltas[i];
    }
}
