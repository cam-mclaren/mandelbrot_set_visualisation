#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Sep 10 13:29:29 2021

@author: cam
"""



from PIL import Image # The image class we will use 
import colorsys # I think we will need this to go from an integer scale to a rgb colour scale 
import math 
import os 



def logColor(distance, base, const, scale):
    color = -1 * math.log(distance, base)
    rgb = colorsys.hsv_to_rgb(const + scale * color,0.8,0.9)
    return tuple(round(i * 255) for i in rgb)

def powerColor(distance, exp, const, scale):
    color = distance**exp
    rgb = colorsys.hsv_to_rgb(const + scale * color,1 - 0.6 * color,0.9)
    return tuple(round(i * 255) for i in rgb)



#Image parameters 

width=1000 # pixels 
x=-0.65
y=0
x_range=3
aspect_ratio=1
precision =500

# Resulting sub_parameters 

height=round(width/aspect_ratio)
y_range=x_range/aspect_ratio

min_x=x-0.5*x_range
max_x=x+0.5*x_range
min_y=y-0.5*y_range
max_y=y+0.5*y_range



# build an image with the appropriate dimensions populated by black pixels 

img=Image.new('RGB', (width, height), color='black')

pixels=img.load()




# Mandelbrot colouring algorithm (escape time algorithm)


for col in range(width):
    for row in range(height):
        
        
        
        x=min_x+col/width*x_range
        y=min_y+row/height*y_range
        
        c_real=x
        c_imag=y
        
        
        #For that value of c check the behavior of the series 
        
        iteration_number=0
        
        while (iteration_number<=precision)and((x**2+y**2)<4):
            # square
            z_square_real=x**2-y**2
            z_square_img = 2*x*y
            #update: z_n =f(z_{n-1}) 
            x=z_square_real+c_real
            y=z_square_img+c_imag
            
            iteration_number+=1
        
        
            
        if iteration_number<precision:
            distance = (iteration_number+1)/(precision+1)
            #rgb = logColor(distance, , 0.5, .9)
            rgb=powerColor(distance, 0.2, 0.27, 1)
            pixels[col,row]=rgb
            
            

img.save('trial2.png')
            
os.system('open trial2.png')
            
            
            
        
        
        
        
        
        