var fs = require('fs')
var {create, all} = require("mathjs");
var config= {
  number: 'BigNumber',      // Default type of number:
                            // 'number' (default), 'BigNumber', or 'Fraction'
  precision: 500,            // Number of significant digits for BigNumbers
  epsilon: 1e-200
};

var mathjs = create(all, config);
    image_dim = {
        'height' : mathjs.bignumber(720),
        'width' : mathjs.bignumber(1280)
        }

function nextcentre(x,y,w,x_pixel_target,y_pixel_target, image_pixels_wide, image_pixels_high, debug = false)
/***********************************************    
    * Type information 
    * x : mathjs.bignumber
    * y : mathjs.bignumber
    * w : mathjs.bignumber
    * x_pixel_target : mathjs.bignumber
    * y_pixel_target :mathjs.bignumber
    * image_pixels_wide : mathjs.bignumber
    * image_pixels_high : mathjs.bignumber
    * debug : bool
    *******************************************/
    {

    function debug_nextcentre(var_name, var_val, isDebug)
    {
    if (isDebug){console.log(`${var_name} = ${mathjs.format(var_val, format_config)}`)}
    }

    // Image x procedures
    left_boundary = mathjs.chain(x).subtract(mathjs.divide(w,mathjs.bignumber(2))).done()
    debug_nextcentre('left_boundary', left_boundary , debug)

    x_offset = mathjs.chain(w).multiply(x_pixel_target).divide(image_pixels_wide).done() 
    debug_nextcentre('x_offset', x_offset , debug)

    new_centre_x = mathjs.add(left_boundary, x_offset)
    debug_nextcentre('new_centre_x', new_centre_x  , debug)  

    // Image y procedures
    aspect_ratio = mathjs.divide(image_pixels_high, image_pixels_wide)
    height = mathjs.multiply(w, aspect_ratio)
    debug_nextcentre('height', height  , debug)

    bottom_boundary = mathjs.subtract(y, mathjs.divide(height,mathjs.bignumber(2)))
    debug_nextcentre('bottom_boundary', bottom_boundary  , debug)  

    y_pixel_target = mathjs.subtract(image_pixels_high, y_pixel_target)
    debug_nextcentre('y_pixel_target', y_pixel_target , debug) 

    y_offset = mathjs.chain(height).multiply(y_pixel_target).divide(image_pixels_high).done() 
    debug_nextcentre('y_offset', y_offset  , debug)

    new_centre_y = mathjs.add(y_offset,bottom_boundary)
    debug_nextcentre('new_centre_y', new_centre_y   , debug)   

    return {"new_centre_x": new_centre_x, "new_centre_y": new_centre_y}
}
//nextcentre(x,y,w,client_data['x'],client_data['y'], true)

exports.nextcentre = nextcentre
