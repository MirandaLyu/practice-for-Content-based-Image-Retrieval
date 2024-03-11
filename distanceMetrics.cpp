/* Project2
    class for defining distance metrics
*/
#include "distanceMetrics.h"


// Function to compute the sum-of-squared-difference (SSD) between two feature vectors
float computeSSD(const Mat& featureVector1, const Mat& featureVector2) {
    float sum;
    for (int i = 0; i < featureVector1.rows; i++) {
        for (int j = 0; j < featureVector1.cols; j++) {
            for (int c = 0; c < featureVector1.channels(); c++) {
                sum += (featureVector1.at<Vec3b>(i, j)[c] - featureVector2.at<Vec3b>(i, j)[c]) *
                          (featureVector1.at<Vec3b>(i, j)[c] - featureVector2.at<Vec3b>(i, j)[c]);
            }
        }
    }
    return sum;
}

// Function to compute the histogram intersection between two histograms
float histogram3DIntersection(const Mat& hist1, const Mat& hist2, int binSize) {
    float min;
    float sum = 0;
    for (int i = 0; i < binSize; i++) {
        for (int j = 0; j < binSize; j++) {
            for (int k = 0; k < binSize; k++) {
                min = hist1.at<float>(i, j, k) < hist2.at<float>(i, j, k) ? hist1.at<float>(i, j, k) : hist2.at<float>(i, j, k);
                sum += min;
            }
        }
    }
    return sum;
}

float histogram1DIntersection(const Mat& hist1, const Mat& hist2, int binSize) {
    float min;
    float sum = 0;
    for (int i = 0; i < binSize; i++) {
                min = hist1.at<float>(i) < hist2.at<float>(i) ? hist1.at<float>(i) : hist2.at<float>(i);
                sum += min;
            }
    return sum;
}

float histogram2DIntersection(const Mat& hist1, const Mat& hist2, int binSize) {
    float min;
    float sum = 0;
    for (int i = 0; i < binSize * 4; i++) {
        for (int j = 0; j < binSize; j++) {
                min = hist1.at<float>(i, j) < hist2.at<float>(i, j) ? hist1.at<float>(i, j) : hist2.at<float>(i, j);
                sum += min;
            }
    }
    return sum;
}