# practice-for-Content-based-Image-Retrieval

This project was built in my Computer Vision class, for the aim to learn what is image matching.

It was not done using neural networks. I think professor wanted us to grasp the fundamental approach of how to do matching, according to colors, textures, and spatial layouts. It was fun. Everything was built in C++ and using OpenCV. We were not allowed to use many built-in functions, so there was a lot of math. : )

## general design

The CBIR system was implemented as a command line program. It runs with 5 inputs which are
* *a target image*
* *the image database*
* *a feature calculation method*
* *a distance metric*
* *how many top N matching images you want*

The target image is the one you want to find similar photos for, the feature method is a way to get a photo's mathematical feature, and the distance metric is a way to compare two photos' features. The image database in this project is too large to upload here.

## first, try a simple matching method

This baseline matching only extracts the middle 9x9 pixels in every picture as the feature, and uses sum-of-squared-difference to compare every two features (do calculations for every pixel and add the differences together). One example of the output is this:

<img width="220" alt="Screenshot 2024-03-10 at 9 13 25 PM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/ef759054-77c4-408b-a30e-e834ea062f86">

The upper left corner is the target image and others are the top 3 matches. We can see that the middles are indeed all red. And this is the metric calculated:

<img width="350" alt="Screenshot 2024-03-11 at 1 42 35 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/c58abdd7-7898-443f-aec2-03bf7972c7bb">

## now, color matching

Color matching usually uses histogram so I built a 3-D 8 bins histogram for each image as feature vector. Then every two images are compared using a metric called histogram intersection (the larger the intersection, the more the similarity).

One example of the output is like this:

<img width="220" alt="Screenshot 2024-03-11 at 1 56 33 AM" src="https://github.com/MirandaLyu/practice.for.Content-based.Image.Retrieval/assets/115821003/20f622ad-5645-4ba8-8775-62bc808ac151">

We can see that these color distributions are very alike. But the lack of spatial precision is the disadvantage of color histograms.

## so let's try add a little spatial information





