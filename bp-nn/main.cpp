#include <iostream>
#include <fstream>

#include "database.hpp"
#include "neuralNet.hpp"

int main()
{
    std::vector<std::pair<int, cv::Mat>> images = get_images(red_c);

    std::size_t count_images = images.size();

    int numIn = 50 * 50;
    int numHid = 30;
    int numOut = 4;
    int maxEpochs = 80;

    neuralNet nn(numIn, numHid, numOut);

    double **input = new double *[count_images];
    int *output = new int[count_images];

    for(std::size_t i = 0; i < count_images; i++)
    {
        input[i] = image_to_array(images[i].second);
        output[i] = images[i].first;
    }
    
    nn.trainBatch(input,output, count_images, maxEpochs);

    // Saving weights
    nn.saveWeights("weights.dat");

    // Loading weights
    // nn.loadWeights("weights.dat");

    // Testing some outputs
    int correct = 0;
    int incorrect = 0;

    for(std::size_t i = 0; i < count_images; i++)
    {
        if (output[i] == nn.classify(input[i]).second)
            correct++;
         if (nn.classify(input[i]).first < 0.5)
            incorrect++;
    }

    std::cout << "correct: " << correct << " / " << count_images << " (id != res)" << std::endl;
    std::cout << "incorrect: " << incorrect << " / " << count_images << " (percent < 50%)" << std::endl;

    for(int i = 0; i < count_images; i++)
        delete[] input[i];
    delete[] input;
    delete[] output;

    {
        std::cout << "Test recognition" << std::endl;

        cv::Mat cv_image = cv::imread("../tsr/ustupi.png", CV_LOAD_IMAGE_GRAYSCALE);
        cv::Mat cv_resize_image;

        cv::resize(cv_image, cv_resize_image, cv::Size(50, 50));
    
        std::cout << "percent: " << nn.classify(image_to_array(cv_resize_image)).first << " id: " << nn.classify(image_to_array(cv_resize_image)).second << std::endl;
    }

    return 0;
}
