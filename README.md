# mandelbrot_set_visualisation
I know it's kinda cliched but I want to make my own zoom of the mandelbrot set. 

## The mandelbrot set. 

The mandelbrot set is a set of complex numbers, whose orbit under a function $`f(z) = z^2 +c`$

## ap_main vs main

Visualising the mandelbrot set requires increasing levels of precision with higher magnifications. ap_main makes use of the mpfr arbitary precision float data types. 

## Scope Creep 

Currently using a very naive algrorithm to calculate escape times for pixels. Should using something smarter. One uisng perbutation theory can be 100x faster. 
