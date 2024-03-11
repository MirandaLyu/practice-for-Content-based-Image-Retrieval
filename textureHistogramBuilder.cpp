/* Project 2
    class for computing texture histograms*/
#include "textureHistogramBuilder.h"

Mat sobelX3x3(const Mat& src) {
    if (src.empty()) {
        std::cerr << "Input image is empty." << std::endl;
        return Mat();
    }

    // turn to greyscale image
    Mat grey;
    cvtColor(src, grey, cv::COLOR_BGR2GRAY);

    // apply some gaussian filter
    Mat blur;
    GaussianBlur(grey, blur, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // convert to float matrix
    Mat floatBlur;
    blur.convertTo(floatBlur, CV_32F);

    // define a float dst matrix because there will be negative numbers
    Mat temp = Mat::zeros(floatBlur.size(), CV_32F);

    // kernel looks like
    // -1 0 1
    // -2 0 2
    // -1 0 1

    // apply kernel, ignore the border
    for (int x = 1; x < blur.rows - 1; x++) {
        for (int y = 1; y < blur.cols - 1; y++) {
            temp.at<float>(x, y) = (floatBlur.at<float>(x-1, y+1))
                                + 2 * (floatBlur.at<float>(x, y+1))
                                + (floatBlur.at<float>(x+1, y+1))
                                - (floatBlur.at<float>(x-1, y-1))
                                - 2 * (floatBlur.at<float>(x, y-1))
                                - (floatBlur.at<float>(x+1, y-1));
        }
    }

    return temp;

}

// 3x3 Sobel Y filter
Mat sobelY3x3(const Mat& src) {
    if (src.empty()) {
        std::cerr << "Input image is empty." << std::endl;
        return Mat();
    }

    // turn to greyscale image
    Mat grey;
    cvtColor(src, grey, cv::COLOR_BGR2GRAY);

    // apply some gaussian filter
    Mat blur;
    GaussianBlur(grey, blur, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    // convert to float matrix
    Mat floatBlur;
    blur.convertTo(floatBlur, CV_32F);

    // define a float dst matrix because there will be negative numbers
    Mat temp = Mat::zeros(floatBlur.size(), CV_32F);

    // kernel looks like
    // -1 -2 -1
    // 0 0 0
    // 1 2 1

    // apply kernel, ignore the border
    for (int x = 1; x < blur.rows - 1; x++) {
        for (int y = 1; y < blur.cols - 1; y++) {
            temp.at<float>(x, y) = (floatBlur.at<float>(x+1, y-1))
                                + 2 * (floatBlur.at<float>(x+1, y))
                                + (floatBlur.at<float>(x+1, y+1))
                                - (floatBlur.at<float>(x-1, y-1))
                                - 2 * (floatBlur.at<float>(x-1, y))
                                - (floatBlur.at<float>(x-1, y+1));
        }
    }

    return temp;
}

Mat gradientMagnitude(const Mat& sobelX, const Mat& sobelY) {
    if (sobelX.empty() || sobelY.empty()) {
        std::cerr << "Input image is empty." << std::endl;
        return Mat();
    }

    // define a float dst matrix to store gradient magnitudes
    Mat temp = Mat::zeros(sobelX.size(), CV_32F);
    Mat dst;

    // start with index 0,0
    for (int x = 0; x < sobelX.rows; x++) {
        for (int y = 0; y < sobelX.cols; y++) {
            temp.at<float>(x, y) = sqrt(sobelX.at<float>(x, y) * sobelX.at<float>(x, y) +
                                        sobelY.at<float>(x, y) * sobelY.at<float>(x, y));
        }
    }

    // converting to CV_8U for further processing
    convertScaleAbs(temp, dst);

    return dst;
}

Mat gradientOrientations(const Mat& sobelX, const Mat& sobelY) {
    Mat grad_orientation = Mat::zeros(sobelX.size(), CV_32F);

    for (int x = 0; x < sobelX.rows; x++) {
        for (int y = 0; y < sobelX.cols; y++) {
            float gx = sobelX.at<float>(x, y);
            float gy = sobelY.at<float>(x, y);
            grad_orientation.at<float>(x, y) = std::atan2(gy, gx) * 180 / CV_PI;
        }
    }

    Mat temp;
    Mat normalized_orientation;
    Mat dst;
    grad_orientation.convertTo(temp, CV_32F, 1.0, 180.0); // Mapping [-180, 180] to [0, 360]
    temp.convertTo(normalized_orientation, CV_32F, 255.0 / 360.0, 0); // Mapping [0, 360] to [0, 255]

    normalized_orientation.convertTo(dst, CV_8U); // Convert to 8-bit unsigned
    
    return dst;
}

Mat magnitudeHistogramBuilder(const Mat& img) {
    // get gradient magnitudes matrix first
    Mat sobelX = sobelX3x3(img);
    Mat sobelY = sobelY3x3(img);
    Mat gradientMag = gradientMagnitude(sobelX, sobelY); // this is single channel image

    // build 1D texture histogram
    int binSize = 16;
    Mat hist = Mat(1, binSize, CV_32F, Scalar(0));

    float max = 0;

    for (int i = 0; i < gradientMag.rows; i++) {
        for (int j = 0; j < gradientMag.cols; j++) {
            // grab the data
            float pixelValue = static_cast<float>(gradientMag.at<uchar>(i, j));

            // compute indexes
            int bindex = (int)(pixelValue * binSize / 256);

            // increment the histogram
            hist.at<float>(bindex)++;
            float newvalue = hist.at<float>(bindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }

    // Normalize the histogram (divide by total pixels)
    hist /= (img.rows * img.cols);

    return hist;
}

Mat fourCellsMagOrientation2DHistoBuilder(const Mat& img) {
    // get gradient magnitudes matrix first
    Mat sobelX = sobelX3x3(img);
    Mat sobelY = sobelY3x3(img);
    Mat gradientMag = gradientMagnitude(sobelX, sobelY); // this is single channel image
    Mat gradientOri = gradientOrientations(sobelX, sobelY);

    // build 2D texture histogram
    int binSize = 8;
    Mat hist1 = Mat(binSize, binSize, CV_32F, Scalar(0));
    Mat hist2 = Mat(binSize, binSize, CV_32F, Scalar(0));
    Mat hist3 = Mat(binSize, binSize, CV_32F, Scalar(0));
    Mat hist4 = Mat(binSize, binSize, CV_32F, Scalar(0));

    float max = 0;

    for (int i = 0; i < gradientMag.rows / 2; i++) {
        for (int j = 0; j < gradientMag.cols / 2; j++) {
            // grab the data
            float pixelMag = static_cast<float>(gradientMag.at<uchar>(i, j));
            float pixelOri = static_cast<float>(gradientOri.at<uchar>(i, j));

            // compute indexes
            int mindex = (int)(pixelMag * binSize / 256);
            int oindex = (int)(pixelOri * binSize / 256);

            // increment the histogram
            hist1.at<float>(mindex, oindex)++;
            float newvalue = hist1.at<float>(mindex, oindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }
    // Normalize the histogram (divide by total pixels)
    hist1 /= ((img.rows / 2) * (img.cols / 2));

    for (int i = 0; i < gradientMag.rows / 2; i++) {
        for (int j = gradientMag.cols / 2; j < gradientMag.cols; j++) {
            // grab the data
            float pixelMag = static_cast<float>(gradientMag.at<uchar>(i, j));
            float pixelOri = static_cast<float>(gradientOri.at<uchar>(i, j));

            // compute indexes
            int mindex = (int)(pixelMag * binSize / 256);
            int oindex = (int)(pixelOri * binSize / 256);

            // increment the histogram
            hist2.at<float>(mindex, oindex)++;
            float newvalue = hist2.at<float>(mindex, oindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }
    hist2 /= ((img.rows / 2) * (img.cols - img.cols / 2));

    for (int i = gradientMag.rows / 2; i < gradientMag.rows; i++) {
        for (int j = 0; j < gradientMag.cols / 2; j++) {
            // grab the data
            float pixelMag = static_cast<float>(gradientMag.at<uchar>(i, j));
            float pixelOri = static_cast<float>(gradientOri.at<uchar>(i, j));

            // compute indexes
            int mindex = (int)(pixelMag * binSize / 256);
            int oindex = (int)(pixelOri * binSize / 256);

            // increment the histogram
            hist3.at<float>(mindex, oindex)++;
            float newvalue = hist3.at<float>(mindex, oindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }
    hist3 /= ((img.rows - img.rows / 2) * (img.cols / 2));

    for (int i = gradientMag.rows / 2; i < gradientMag.rows; i++) {
        for (int j = gradientMag.cols / 2; j < gradientMag.cols; j++) {
            // grab the data
            float pixelMag = static_cast<float>(gradientMag.at<uchar>(i, j));
            float pixelOri = static_cast<float>(gradientOri.at<uchar>(i, j));

            // compute indexes
            int mindex = (int)(pixelMag * binSize / 256);
            int oindex = (int)(pixelOri * binSize / 256);

            // increment the histogram
            hist4.at<float>(mindex, oindex)++;
            float newvalue = hist4.at<float>(mindex, oindex);

            // conditional assignment
            max = newvalue > max ? newvalue : max;
        }
    }
    hist4 /= ((img.rows - img.rows / 2) * (img.cols - img.cols / 2));

    // concatenate 4 histograms into 1
    Mat resultHist;
    std::vector<Mat> mats = {hist1, hist2, hist3, hist4};

    vconcat(mats, resultHist);

    return resultHist;
}