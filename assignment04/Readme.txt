===================================================
  Build Instructions & Program Description
===================================================

----------------------------------------------------
1. salt_and_pepper.cpp
----------------------------------------------------

PURPOSE
  Adds salt-and-pepper noise to grayscale and color images,
  then removes the noise using median filtering.
  Supports three boundary processing methods:
  zero-padding, mirroring, and adjustkernel.

COMPILE
  g++ -o salt_and_pepper salt_and_pepper.cpp \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./salt_and_pepper

HOW TO ADJUST PARAMETERS
  Open salt_and_pepper.cpp and find:

      Mat noise_Gray = Add_salt_pepper_Noise(input_gray, 0.1f, 0.1f);
      Mat noise_RGB  = Add_salt_pepper_Noise(input,      0.1f, 0.1f);

  - ps (2nd argument)
      Density of salt noise (white pixels). Range: 0.0 ~ 1.0
      Default: 0.1

  - pp (3rd argument)
      Density of pepper noise (black pixels). Range: 0.0 ~ 1.0
      Default: 0.1

  To adjust the median filter:

      Mat Denoised_Gray = Salt_pepper_noise_removal_Gray(noise_Gray, 2, "zero-padding");

  - n (2nd argument)
      Kernel radius. Kernel size = (2n+1) x (2n+1)
      Default: 2

  - boundary option (3rd argument)
      "zero-padding" : fill outside pixels with 0
      "mirroring"    : reflect pixels at the boundary
      "adjustkernel" : use only valid pixels inside boundary

NOTE
  Input image file is "lena.jpg".
  Place the image in the same directory before running.

----------------------------------------------------
2. Gaussian.cpp
----------------------------------------------------

PURPOSE
  Adds Gaussian noise to grayscale and color images,
  then removes the noise using Gaussian filtering.
  Supports three boundary processing methods:
  zero-padding, mirroring, and adjustkernel.

COMPILE
  g++ -o gaussian Gaussian.cpp \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./gaussian

HOW TO ADJUST PARAMETERS
  Open Gaussian.cpp and find:

      Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
      Mat noise_RGB  = Add_Gaussian_noise(input,      0, 0.1);

  - mean (2nd argument)
      Mean of Gaussian noise distribution.
      Default: 0

  - sigma (3rd argument)
      Standard deviation of Gaussian noise.
      Larger value = stronger noise.
      Default: 0.1

  To adjust the Gaussian filter:

      Mat Denoised_Gray = Gaussianfilter_Gray(noise_Gray, 5, 3, 3, "zero-padding");

  - n (2nd argument)
      Kernel radius. Kernel size = (2n+1) x (2n+1)
      Default: 5

  - sigmaT (3rd argument)
      Standard deviation in the x-direction.
      Default: 3

  - sigmaS (4th argument)
      Standard deviation in the y-direction.
      Default: 3

  - boundary option (5th argument)
      "zero-padding" : fill outside pixels with 0
      "mirroring"    : reflect pixels at the boundary
      "adjustkernel" : use only valid pixels inside boundary

NOTE
  Pixel values are normalized to [0, 1] before processing.
  Input image file is "lena.jpg".
  Place the image in the same directory before running.

----------------------------------------------------
3. Bilateral.cpp
----------------------------------------------------

PURPOSE
  Adds Gaussian noise to grayscale and color images,
  then removes the noise using bilateral filtering.
  Bilateral filter preserves edges by considering both
  spatial distance and intensity difference between pixels.
  Supports three boundary processing methods:
  zero-padding, mirroring, and adjustkernel.

COMPILE
  g++ -o bilateral Bilateral.cpp \
    $(pkg-config --cflags --libs opencv4)

RUN
  ./bilateral

HOW TO ADJUST PARAMETERS
  Open Bilateral.cpp and find:

      Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
      Mat noise_RGB  = Add_Gaussian_noise(input,      0, 0.1);

  - mean (2nd argument)
      Mean of Gaussian noise distribution.
      Default: 0

  - sigma (3rd argument)
      Standard deviation of Gaussian noise.
      Default: 0.1

  To adjust the bilateral filter:

      Mat Denoised_Gray = bilateralfilter_Gray(noise_Gray, 5, 3, 3, 0.2, "zero-padding");

  - n (2nd argument)
      Kernel radius. Kernel size = (2n+1) x (2n+1)
      Default: 5

  - sigmaT (3rd argument)
      Standard deviation in the x-direction (spatial).
      Default: 3

  - sigmaS (4th argument)
      Standard deviation in the y-direction (spatial).
      Default: 3

  - sigmaR (5th argument)
      Standard deviation for intensity difference (range).
      Smaller value = stronger edge preservation.
      Larger value = closer to Gaussian filter behavior.
      Default: 0.2

  - boundary option (6th argument)
      "zero-padding" : fill outside pixels with 0
      "mirroring"    : reflect pixels at the boundary
      "adjustkernel" : use only valid pixels inside boundary

NOTE
  Pixel values are normalized to [0, 1] before processing.
  For color images, intensity difference is computed as
  the Euclidean distance between RGB color vectors.
  Input image file is "lena.jpg".
  Place the image in the same directory before running.

===================================================