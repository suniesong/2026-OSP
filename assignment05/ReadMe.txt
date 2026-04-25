===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. adaptivethreshold.cpp
----------------------------------------------------

PURPOSE
  Performs image segmentation on a grayscale image
  using Adaptive Thresholding with Moving Averages.
  Each pixel is binarized based on a local threshold
  computed from the weighted mean of its neighborhood,
  making it robust to uneven illumination.

COMPILE
  g++ -o adaptivethreshold adaptivethreshold.cpp \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./adaptivethreshold

HOW TO ADJUST PARAMETERS
  Open adaptivethreshold.cpp and find:

      output = adaptive_thres(input_gray, 2, 0.9);

  - N (2nd argument)
      Kernel radius. Kernel size = (2N+1) x (2N+1)
      Larger value = wider neighborhood for local mean.
      Default: 2

  - b (3rd argument)
      Weight applied to the local mean to determine threshold.
      T(i,j) = b x m(i,j)
      b < 1.0 : more pixels classified as foreground (255)
      b = 1.0 : threshold equals local mean
      b > 1.0 : fewer pixels classified as foreground
      Default: 0.9

NOTE
  Output image consists of pixel values 0 or 255 only.
  Input image file is "writing.jpg".
  Place the image in the same directory before running.

----------------------------------------------------
2. kmeans.cpp
----------------------------------------------------

PURPOSE
  Performs image segmentation using OpenCV's K-means
  clustering function. Supports four feature space
  configurations:
    1) Grayscale intensity only (1-D)
    2) Grayscale intensity + position (3-D)
    3) RGB color only (3-D)
    4) RGB color + position (5-D)
  When position is included, intensity and position
  are normalized to [0, 1] before clustering.

COMPILE
  g++ -o kmeans kmeans.cpp \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./kmeans

HOW TO ADJUST PARAMETERS
  Open kmeans.cpp and find:

      int clusterCount = 10;
      int attempts = 5;
      #define sigma 512.0f

  - clusterCount
      Number of clusters K.
      Larger value = finer segmentation.
      Default: 10

  - attempts
      Number of times K-means is run with different
      random initializations. The best result
      (lowest SSD) is selected.
      Default: 5

  - sigma
      Scaling factor for position normalization.
      Position is divided by sigma to map to [0, 1].
      Larger sigma = position has less influence
      relative to intensity.
      Smaller sigma = position has more influence.
      Default: 512.0

NOTE
  Input image file is "lena.jpg".
  Place the image in the same directory before running.

----------------------------------------------------
3. meanshift.cpp
----------------------------------------------------

PURPOSE
  Performs image segmentation using OpenCV's
  pyrMeanShiftFiltering function.
  Unlike K-means, the number of clusters is determined
  automatically by finding modes in the feature space.
  Applied to both grayscale and color images.
  For grayscale input, the image is temporarily
  converted to 3-channel format before filtering,
  then converted back to grayscale.

COMPILE
  g++ meanshift.cpp -o meanshift \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./meanshift

HOW TO ADJUST PARAMETERS
  Open meanshift.cpp and find:

      pyrMeanShiftFiltering(input, color_output, 31, 20, 3);
      pyrMeanShiftFiltering(gray_3ch, gray_output, 31, 20, 3);

  - sp (3rd argument)
      Spatial window radius.
      Larger value = wider spatial region is merged
      into the same cluster.
      Default: 31

  - sr (4th argument)
      Color window radius.
      Larger value = pixels with larger color difference
      are merged into the same cluster.
      Default: 20

  - maxLevel (5th argument)
      Maximum level of the Gaussian pyramid.
      0 = no pyramid, applied directly to original image.
      Larger value = faster processing, smoother boundaries,
      but finer details may be lost.
      Default: 3

NOTE
  Input image file is "lena.jpg".
  Place the image in the same directory before running.

----------------------------------------------------
4. edison_binary
----------------------------------------------------

PURPOSE
  Performs Mean Shift segmentation using the EDISON
  binary provided by the instructor.
  Refer to the included specification.html and
  Readme.txt inside the edison_binary folder
  for detailed usage instructions.

COMPILE
  g++ meanshift.cpp -o meanshift \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./meanshift

===================================================