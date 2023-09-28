var fs = require('fs')
var {create, all} = require("mathjs");
var config= {
  number: 'BigNumber',      // Default type of number:
                            // 'number' (default), 'BigNumber', or 'Fraction'
  precision: 500,            // Number of significant digits for BigNumbers
  epsilon: 1e-200
};

var mathjs = create(all, config);
let param_list
let data = fs.readFileSync('./image_parameters.txt', 'utf8')

param_list = data.split('\n')
format_config = {'precision':50, 'notation' : 'fixed'}
let x = mathjs.bignumber(param_list[0]) // The centre x coordinate
let y = mathjs.bignumber(param_list[1]) // The centre y cooridnate
let w = mathjs.bignumber(param_list[2]) // The width of the image
let a = [ x, y , w ]
for (let i = 0; i<3 ; i++)
{
    console.log(mathjs.format( a[i] ,format_config))
}

client_data = {
    'x' : mathjs.bignumber(200), 
    'y' : mathjs.bignumber(200) 
}
function nextcentre(x,y,w,x_pixel_target,y_pixel_target, debug = false)
    {

    function debug_nextcentre(var_name, var_val, isDebug)
    {
    if (isDebug){console.log(`${var_name} = ${mathjs.format(var_val, format_config)}`)}
    }

    image_dim = {
        'height' : mathjs.bignumber(720),
        'width' : mathjs.bignumber(1280)
        }
    // Image x procedures
    left_boundary = mathjs.chain(x).subtract(mathjs.divide(w,mathjs.bignumber(2))).done()
    debug_nextcentre('left_boundary', left_boundary , debug)

    x_offset = mathjs.chain(w).multiply(x_pixel_target).divide(image_dim['width']).done() 
    debug_nextcentre('x_offset', x_offset , debug)

    new_centre_x = mathjs.add(left_boundary, x_offset)
    debug_nextcentre('new_centre_x', new_centre_x  , debug)  

    // Image y procedures
    aspect_ratio = mathjs.divide(image_dim['height'], image_dim['width'])
    height = mathjs.multiply(w, aspect_ratio)
    debug_nextcentre('height', height  , debug)

    bottom_boundary = mathjs.subtract(y, mathjs.divide(height,mathjs.bignumber(2)))
    debug_nextcentre('bottom_boundary', bottom_boundary  , debug)  

    y_pixel_target = mathjs.subtract(image_dim['height'], y_pixel_target)
    debug_nextcentre('y_pixel_target', y_pixel_target , debug) 

    y_offset = mathjs.chain(height).multiply(y_pixel_target).divide(image_dim['height']).done() 
    debug_nextcentre('y_offset', y_offset  , debug)

    new_centre_y = mathjs.add(y_offset,bottom_boundary)
    debug_nextcentre('new_centre_y', new_centre_y   , debug)   
}
//nextcentre(x,y,w,client_data['x'],client_data['y'], true)

exports.nextcentre
