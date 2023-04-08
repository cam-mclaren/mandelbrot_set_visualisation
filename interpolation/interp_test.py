import numpy as np

import matplotlib.pyplot as plt 


#################################################

def read_file_of_doubles_into_array(filename):
    
    # count the number of elements in the file 
    size = 0 
    file_pointer = open(filename, "r")
    for lines in file_pointer:
        size+=1
    
    # Initialise numpy array 
    output = np.zeros(size)
    
    # return to start of file 
    file_pointer.seek(0)
    
    #output contents into array 
    for i in range(size):
        output[i]=float(file_pointer.readline())
        
    #close the file 
    file_pointer.close() 
        
    return output


##################################################



my_array=read_file_of_doubles_into_array("colour_parameters/nodes")

print(my_array)

#####################################################

image = read_file_of_doubles_into_array("colour_parameters/test_image")
domain = read_file_of_doubles_into_array("colour_parameters/test_domain")




_ = plt.plot(domain, image, marker = '.', linestyle = 'none')

_ = plt.xlabel("domain [0,1]")

_ = plt.ylabel("spline image")

plt.show()