#include "neuralNet.h"
#include <iostream>
#include <fstream>

int main()
{
	neuralNet nn(400,30,10);

	// Test data
	double **input; 
	input = new double *[5000];
	for(int i = 0; i < 5000; i++)
		input[i] = new double[400];
	int *output;
	output = new int[5000];

	// Load test data
	std::fstream inputs, outputs;
	inputs.open("input.txt", std::ios::in);
	outputs.open("output.txt", std::ios::in);

	for (int i = 0; i < 5000; i++)
	{
		for (int j = 0; j < 400; j++)
			inputs >> input[i][j];
		outputs >> output[i];
	}

	inputs.close();
	outputs.close();

	// Training
	// ---

	// Batch training
	nn.trainBatch(input,output,5000,20);

	// Live training
	/*
	double* results = new double[10];
	double max = -1;
	int ans = 0;
	double acc = 0;
	for (int i = 0; i < 5000; i++)
	{
		results = nn.trainLive(input[i],output[i]);
		for (int j = 0; j < 10; j++)
		{
			if (results[j] > max)
			{
				max = results[j];
				ans = j;
			}
		}
		if (ans == output[i])
			acc += 1.0;
	}
	acc = acc / 5000.0 * 100.0;
	cout << "Live training accuracy: " << acc << endl;
	*/

	// Save/Load
	// ---

	// Saving weights
	nn.saveWeights("weights.dat");

	// Loading weights
	nn.loadWeights("weights.dat");

	// Testing some outputs
	std::cout << "Value: " << output[20] << ", Result: " << nn.classify(input[20]) << std::endl;
	std::cout << "Value: " << output[300] << ", Result: " << nn.classify(input[300]) << std::endl;
	std::cout << "Value: " << output[2500] << ", Result: " << nn.classify(input[2500]) << std::endl;
	std::cout << "Value: " << output[4800] << ", Result: " << nn.classify(input[4800]) << std::endl;

	for(int i = 0; i < 5000; i++)
		delete[] input[i];
	delete[] input;
	delete[] output;
	return 0;
}
