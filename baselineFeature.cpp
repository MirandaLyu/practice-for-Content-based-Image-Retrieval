/* Project 2
    a class for defining baseline feature*/
#include "baselineFeature.h"

Mat baselineFeatureGenerate(const Mat& img) {
    // find the center of the image
    int centerX = img.rows / 2;
    int centerY = img.cols / 2;

    // extract 9x9 square and put every pixel into a matrix
    Mat featureVector(9, 9, CV_8UC3);
    int newX = 0;
    int newY = 0;
    for (int i = centerX - 4; i <= centerX + 4; i++) {
        for (int j = centerY - 4; j <= centerY + 4; j++) {
            featureVector.at<Vec3b>(newX, newY) = img.at<Vec3b>(i, j);
            newY++;
        }
        newX++;
        newY = 0;
    }

    return featureVector;
}
