/* Miranda Lyu
    CS 5330
    project 2
    main class*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include "baselineFeature.h"
#include "distanceMetrics.h"
#include "colorHistogramBuilder.h"
#include "textureHistogramBuilder.h"
#include <dirent.h>
#include <map>
#include <cstring>

using namespace cv;
using namespace std;

// the general command line program
int main(int argc, char* argv[]) {

    // check the number of inputs
    if (argc != 6) {
        cout << "the number of arguments is wrong" << endl;
        return -1;
    }

    // read the src image
    Mat src = imread(argv[1]);
    if (src.data == NULL) {
        cerr << "Error: Unable to read image " << argv[1] << endl;
        return -1;
    }

    // extract src feature
    Mat srcFeature;
    Mat srcUpperFeature;
    Mat srcLowerFeature;
    Mat srcColorFeature;
    Mat srcTextureFeature;
    if (strcmp(argv[3], "baseline") == 0) {
        srcFeature = baselineFeatureGenerate(src);
    } else if (strcmp(argv[3], "histogram") == 0) {
        srcFeature = histogram3DGenerate(src);
    } else if (strcmp(argv[3], "multihisto") == 0) {
        srcUpperFeature = upperHistogram3DGenerate(src);
        srcLowerFeature = lowerHistogram3DGenerate(src);
    } else if (strcmp(argv[3], "textureColor") == 0) {
        srcColorFeature = histogram3DGenerate(src);
        srcTextureFeature = magnitudeHistogramBuilder(src);
    } else if (strcmp(argv[3], "magOri") == 0) {
        srcColorFeature = histogram3DGenerate(src);
        srcTextureFeature = fourCellsMagOrientation2DHistoBuilder(src);
    } else {
        cout << "Error: Invalid feature type." << endl;
        return -1;
    }

    // loop each example image and compare with the src image
    DIR* dir;
    struct dirent* entry;
    map<string, float> distanceMap;
    string inputFolder = argv[2];

    if ((dir = opendir(inputFolder.c_str())) != NULL)  {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) { // Check if it's a regular file
            const string fileName = inputFolder + entry->d_name;

            Mat img = imread(fileName);
            if (img.data == NULL) {
                cerr << "Error: Unable to read image " << fileName << endl;
                continue;
            }

            // extract img feature and compare with src
            Mat imgFeature;
            Mat imgUpperFeature;
            Mat imgLowerFeature;
            Mat imgColorFeature;
            Mat imgTextureFeature;
            if (strcmp(argv[3], "baseline") == 0) {
                imgFeature = baselineFeatureGenerate(img);
            } else if (strcmp(argv[3], "histogram") == 0) {
                imgFeature = histogram3DGenerate(img);
            } else if (strcmp(argv[3], "multihisto") == 0) {
                imgUpperFeature = upperHistogram3DGenerate(img);
                imgLowerFeature = lowerHistogram3DGenerate(img);
            } else if (strcmp(argv[3], "textureColor") == 0) {
                imgColorFeature = histogram3DGenerate(img);
                imgTextureFeature = magnitudeHistogramBuilder(img);
            } else if (strcmp(argv[3], "magOri") == 0) {
                imgColorFeature = histogram3DGenerate(img);
                imgTextureFeature = fourCellsMagOrientation2DHistoBuilder(img);
            }

            float distance;
            if (strcmp(argv[4], "SSD") == 0) {
                distance = computeSSD(srcFeature, imgFeature);
            } else if (strcmp(argv[4], "intersection") == 0) {
                if (strcmp(argv[3], "multihisto") == 0) {
                    float upperIntersection = histogram3DIntersection(srcUpperFeature, imgUpperFeature, 8);
                    float lowerIntersection = histogram3DIntersection(srcLowerFeature, imgLowerFeature, 8);
                    distance = 0.5 * upperIntersection + 0.5 * lowerIntersection;
                } else if (strcmp(argv[3], "textureColor") == 0) {
                    float colorIntersection = histogram3DIntersection(srcColorFeature, imgColorFeature, 8);
                    float textureIntersection = histogram1DIntersection(srcTextureFeature, imgTextureFeature, 16);
                    distance = 0.5 * colorIntersection + 0.5 * textureIntersection;
                } else if (strcmp(argv[3], "magOri") == 0) {
                    float colorIntersection = histogram3DIntersection(srcColorFeature, imgColorFeature, 8);
                    float textureIntersection = histogram2DIntersection(srcTextureFeature, imgTextureFeature, 8);
                    distance = 0.5 * colorIntersection + 0.5 * textureIntersection;
                } else {
                distance = histogram3DIntersection(srcFeature, imgFeature, 8);
                }
            } else {
                cout << "Error: Invalid distance metric." << endl;
                return -1;
            }
    

            // store the distance in a map
            distanceMap[fileName] = distance;
            }
        }
        closedir(dir);
    } else {
        cerr << "Error: Input folder not found." << endl;
        return -2;
    }

    // Sort the distance map
    vector<pair<string, float>> sortedDistance(distanceMap.begin(), distanceMap.end());
    sort(sortedDistance.begin(), sortedDistance.end(), [argv](const pair<string, float>& a, const pair<string, float>& b) {
        if (strcmp(argv[4], "SSD") == 0) {
            return a.second < b.second;
        } else if (strcmp(argv[4], "intersection") == 0) {
            return a.second > b.second;
        } else {
            return false;
        }
    });

    // Display the top N matches
    int topK;
    try {
        topK = stoi(argv[5]);
    } catch (const invalid_argument& ia) {
        cout << "Invalid argument: " << ia.what() << endl;
        return -1;
    } catch (const out_of_range& oor) {
        cout << "Out of Range error: " << oor.what() << endl;
        return -1;
    }
    for (int i = 0; i < topK; i++) {
        const string& topMatch = sortedDistance[i].first;
        Mat topMatchImage = imread(topMatch);
        if (!topMatchImage.empty()) {
            string windowName = "Top Match " + to_string(i + 1);
            imshow(windowName, topMatchImage);
            cout << "Image: " << topMatch << ", distance: " << sortedDistance[i].second << endl;
        }
    }

    waitKey(0);

    return 0;
}