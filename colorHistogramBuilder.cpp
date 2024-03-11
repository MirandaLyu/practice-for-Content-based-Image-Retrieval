/* Project 2
    class for generating color histogram features*/
#include "colorHistogramBuilder.h"

Mat histogram3DGenerate(const Mat& img) {

    // initialize a histogram matrix
    int binSize = 8; // number of bins
    int size[] = {binSize, binSize, binSize}; // dimensions of the 3D histogram
    Mat hist = Mat(3, size, CV_32FC1, Scalar(0));

    float max = 0;

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            // grab the data
            float B = img.at<cv::Vec3b>(i, j)[0];
            float G = img.at<cv::Vec3b>(i, j)[1];
            float R = img.at<cv::Vec3b>(i, j)[2];

            // compute indexes
            int bindex = (int)( B * binSize / 256);
            int gindex = (int)( G * binSize / 256);
            int rindex = (int)( R * binSize / 256);

            // increment the histogram
            hist.at<float>(bindex, gindex, rindex)++;
            float newvalue = hist.at<float>(bindex, gindex, rindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }

    // Normalize the histogram (divide by total pixels)
    hist /= (img.rows * img.cols);

    return hist;
}

Mat upperHistogram3DGenerate(const Mat& img) {
    // initialize a histogram matrix
    int binSize = 8; // number of bins
    int size[] = {binSize, binSize, binSize}; // dimensions of the 3D histogram
    Mat hist = Mat(3, size, CV_32FC1, Scalar(0));

    float max = 0;

    for (int i = 0; i < img.rows / 2; i++) {
        for (int j = 0; j < img.cols; j++) {
            // grab the data
            float B = img.at<cv::Vec3b>(i, j)[0];
            float G = img.at<cv::Vec3b>(i, j)[1];
            float R = img.at<cv::Vec3b>(i, j)[2];

            // compute indexes
            int bindex = (int)( B * binSize / 256);
            int gindex = (int)( G * binSize / 256);
            int rindex = (int)( R * binSize / 256);

            // increment the histogram
            hist.at<float>(bindex, gindex, rindex)++;
            float newvalue = hist.at<float>(bindex, gindex, rindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }

    // Normalize the histogram (divide by total pixels)
    hist /= (img.rows / 2 * img.cols);

    return hist;
}

Mat lowerHistogram3DGenerate(const Mat& img) {
    // initialize a histogram matrix
    int binSize = 8; // number of bins
    int size[] = {binSize, binSize, binSize}; // dimensions of the 3D histogram
    Mat hist = Mat(3, size, CV_32FC1, Scalar(0));

    float max = 0;

    for (int i = img.rows / 2; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            // grab the data
            float B = img.at<cv::Vec3b>(i, j)[0];
            float G = img.at<cv::Vec3b>(i, j)[1];
            float R = img.at<cv::Vec3b>(i, j)[2];

            // compute indexes
            int bindex = (int)( B * binSize / 256);
            int gindex = (int)( G * binSize / 256);
            int rindex = (int)( R * binSize / 256);

            // increment the histogram
            hist.at<float>(bindex, gindex, rindex)++;
            float newvalue = hist.at<float>(bindex, gindex, rindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }

    // Normalize the histogram (divide by total pixels)
    hist /= ((img.rows - img.rows / 2) * img.cols);

    return hist;
}

