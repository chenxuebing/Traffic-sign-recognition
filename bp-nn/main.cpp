#include <iostream>
#include <fstream>

#include "database.hpp"
#include "neuralNet.hpp"

int main(int argc, char** argv)
{
    std::vector<std::pair<int, cv::Mat>> images = get_images(red_c);

    std::size_t count_images = images.size();

    int numIn = 50 * 50;
    int numHid = 30;
    int numOut = 4;
    int maxEpochs = 200;

    neuralNet nn(numIn, numHid, numOut);

    float **input = new float *[count_images];
    int *output = new int[count_images];

    for(std::size_t i = 0; i < count_images; i++)
    {
        input[i] = image_to_array(images[i].second);
        output[i] = images[i].first;
    }
    
    nn.trainBatch(input,output, count_images, maxEpochs);

    nn.saveWeights("weights.dat");

    int correct = 0;
    int incorrect = 0;

    for(std::size_t i = 0; i < count_images; i++)
    {
        // std::cout << output[i] << " - " << nn.classify(input[i]).second << std::endl;
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

    return 0;
}

int test_main(int argc, char** argv)
{
    int numIn = 50 * 50;
    int numHid = 30;
    int numOut = 4;

    neuralNet nn(numIn, numHid, numOut);

    nn.loadWeights("weights.dat");


    std::cout << "Test recognition" << std::endl;

    cv::Mat cv_image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat cv_resize_image;

    cv::resize(cv_image, cv_resize_image, cv::Size(50, 50));
    
    float *array = image_to_array(cv_resize_image);

    std::cout << "percent: " << nn.classify(array).first << " id: " << nn.classify(array).second << std::endl;

    return 0;
}
