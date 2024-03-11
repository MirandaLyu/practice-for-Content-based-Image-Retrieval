# practice-for-Content-based-Image-Retrieval

This project was built in my Computer Vision class, for the aim to learn what is image matching.

It was not done using neural networks. I think professor wanted us to grasp the fundamental approach of how to do matching, according to colors, textures, and spatial layouts. It was fun. Everything was built in C++ and using OpenCV. We were not allowed to use many built-in functions, so there was a lot of math. : )

## general design

The CBIR system was implemented as a command line program. It runs with 5 inputs which are ***a target image***, ***the image database***, ***a feature calculation method***, ***a distance metric*** and ***how many top N images you want***. The target image is the one you want to find similar photos for, the feature method is a way to get a photo's representation, and the distance metric is a way to compare two photos' mathematical features.

## first, try a simple matching method

This baseline matching only extracts the middle 9x9 pixels in every picture as the feature, and uses sum-of-squared-difference to compare every two features (do calculations for every pixel and add the differences together). One example of the output is this:

The upper left corner is the target image and others are the top 3 matches. We can see that the middles are indeed all red.

