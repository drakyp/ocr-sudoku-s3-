#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Network parameters.
#define INPUT_SIZE 2
#define HIDDEN_SIZE 2
#define OUTPUT_SIZE 1

// Training settings.
#define LEARNING_RATE 0.01
#define EPOCHS 1000000

// Network weights.
extern double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE];
extern double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE];

// Network biases.
extern double hiddenBiases[HIDDEN_SIZE];
extern double outputBiases[OUTPUT_SIZE];

// Network layers.
extern double hidden[HIDDEN_SIZE];
extern double outputs[OUTPUT_SIZE];

double sigmoid(double x);

double sigmoid_derivative(double x);

double random_double();

void fill_array_randomly(double *array, size_t size);

void forward(double inputs[INPUT_SIZE],
             double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE],
             double hidden[HIDDEN_SIZE],
             double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE],
             double hiddenBiases[HIDDEN_SIZE],
             double outputs[OUTPUT_SIZE],
             double outputBiases[OUTPUT_SIZE]);

void backward(double inputs[INPUT_SIZE],
              double inputToHiddenWeights[INPUT_SIZE][HIDDEN_SIZE],
              double hidden[HIDDEN_SIZE],
              double hiddenToOutputWeights[HIDDEN_SIZE][OUTPUT_SIZE],
              double hiddenBiases[HIDDEN_SIZE],
              double target,
              double outputs[OUTPUT_SIZE],
              double outputBiases[OUTPUT_SIZE]);
