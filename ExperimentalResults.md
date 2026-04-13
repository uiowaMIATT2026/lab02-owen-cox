# Experimental Results

I created a difference image that shows small differences around the edges of the circle.
This indicates that the registration is working as expected and it at least moving in the right direction.
It is possible that more parameter optimization could reduce these small differences, but for now
I want to demonstrate that I have the fundamental steps of the registration process working.

I calculated the RMSE between the fixed circle image and the resulting transformed image
by loading my resulting images into MATLAB and using its rmse() function.
RMSE: 2.5017e-05

This small RMSE indicates that the registration was successful.

Also, my program prints out final transformation parameters that make sense considering the target transformation:
Final scale: 2
Final angle: 0
Final tx: 150
Final ty: 150

We are scaling the moving image by a factor of 0.5 and performing a -150, -150 translation.
My understanding is that these final parameters are from the perspective of the fixed image.

I deliberately set the initial scale value to 1.0 and initial transform values in the x and y directions both to 100 
to show that the registration process is working. However, I noticed that the process did not produce outputs that made 
sense if the circles did not overlap or if the initial scale factor was 0.0.
So, it seems there needs to be some manual selection of these initial parameters.
We should keep in mind some idea of what we expect our transformation to look like during the registration process
so that we can select initial parameters that will push the algorithm in the right direction.

It took several iterations to make sure that the steps in the registration process were working as expected.
Initially, I had set some of the parameters to values that reversed the process because of some misconceptions.
For example, I set the center value for the initial transform to the center of the moving image instead of the fixed image.
These mismatched parameters resulted in output images that did not make sense and nonsensical final transform parameters.

However, since I have corrected those misconceptions, the final parameters are much more clean and make sense.

Most of this is based on examples directly from the ITK Software Guide.
Default settings for registration parameters are based on the simple, comparable, standard examples from the book.

I have included some output images in nii.gz format for the Registration directory.
I used the NIFTI format for testing because I know they will load in MATLAB and Slicer.
