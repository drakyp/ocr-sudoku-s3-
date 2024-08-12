#include "xor.h"

int main()
{
    // Initialize the rand seed.
    srand(time(NULL));

    // Training data (XOR).
    double inputs[][3] = {
        {0, 0, 0},
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}};

    double targets[] = {0, 1, 1, 0};

    // Network weights.
    double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE];
    double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE];

    // Network biases.
    double hiddenBiases[HIDDEN_SIZE];
    double outputBiases[OUTPUT_SIZE];

    // Network layers.
    double hidden[HIDDEN_SIZE];
    double outputs[OUTPUT_SIZE];

    // Fill weights matrices randomly with values between -1 and 1.
    for (size_t i = 0; i < INPUT_SIZE; i++)
    {
        fill_array_randomly(inputToHiddenWeights[i], HIDDEN_SIZE);
    }

    for (size_t i = 0; i < HIDDEN_SIZE; i++)
    {
        fill_array_randomly(hiddenToOutputWeights[i], OUTPUT_SIZE);
    }

    // Fill bias arrays randomly with values between -1 and 1.
    fill_array_randomly(hiddenBiases, HIDDEN_SIZE);
    fill_array_randomly(outputBiases, OUTPUT_SIZE);

    // Training loop.
    for (size_t i = 0; i < EPOCHS; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            forward(inputs[j],
                    inputToHiddenWeights,
                    hidden,
                    hiddenToOutputWeights,
                    hiddenBiases,
                    outputs,
                    outputBiases);
            backward(inputs[j],
                     inputToHiddenWeights,
                     hidden,
                     hiddenToOutputWeights,
                     hiddenBiases,
                     targets[j],
                     outputs,
                     outputBiases);
        }
    }

    // Show results compared to training data.
    printf("==== RESULTS ====\n");
    for (size_t i = 0; i < 4; i++)
    {
        forward(inputs[i],
                inputToHiddenWeights,
                hidden,
                hiddenToOutputWeights,
                hiddenBiases,
                outputs,
                outputBiases);
        printf("%i : %i -> %f (expected %i)\n", (int)inputs[i][0],
               (int)inputs[i][1],
               outputs[0],
               (int)targets[i]);
    }

    // Returns 0 for succesful.
    return EXIT_SUCCESS;
}
