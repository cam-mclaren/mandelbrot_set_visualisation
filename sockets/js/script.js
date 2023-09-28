//const input = prompt("Please enter some input:");
//if (!input){
//	alert('No input provided. Please refresh the page and try again.');
//}
// Create the WebSocket connection


const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');
const data_div_current = document.getElementById('image_param');
const render_button    = document.getElementById('submitrender');
let image_display      = { "x":'', "y":'', "width":''};
let next_image_display      = { "x":'', "y":'', "width":''};
let rect_size = 10
let x = 0;
let y = 0;
let  unclicked =0;

let w = new Worker('/js/webworker.js');


function colorPixel(pixel) {

	//const [x, y, r, g, b ] = pixel;

	// Set the fill style to the provided rgb colour
	//calculate size of pixel and draw it.
    let imgData = ctx.createImageData(1,1)
    
    imgData.data[0] = pixel[2];
    imgData.data[1] = pixel[3];
    imgData.data[2] = pixel[4];
    imgData.data[3] = 255;
    ctx.putImageData(imgData, pixel[0], pixel[1]);
}


w.onmessage = function(event){
    requestAnimationFrame(() =>
        {
            for (let i =0; i < event.data.length; i++)
            {
                colorPixel(event.data[i]);
            }
            event = null; 
        });
};


function update_image_param(server_data, data_div, data_to_update)
{
    text = data_div.innerHTML
    console.log(text);
    console.log(data_to_update);
    text=text.replace('x:'+data_to_update["x"], 'x:'+server_data['x']);
    text=text.replace("y:"+data_to_update["y"]+'</p>', 'y:'+server_data['y']+'</p>');
    text=text.replace('width:'+data_to_update["width"], 'width:'+server_data['width']);
    console.log(text)
    data_to_update["x"]=server_data['x'];
    data_to_update["y"]=server_data['y'];
    data_to_update["width"]=server_data['width'];   
    
    data_div.innerHTML=text;
};

let http_init_params = new XMLHttpRequest();
http_init_params.open('GET', 'init-params', true);
http_init_params.setRequestHeader("Content-type", "application/json");
http_init_params.responseType="json";
http_init_params.onload= function() {
    console.log(this.response);
    update_image_param(this.response, data_div_current, image_display);
};
http_init_params.send();










// Drawing Logic
function centreRect(context, x, y, width, height)
{
	let c_x = x - width/2
	let c_y = y - height/2
	context.rect(c_x, c_y, width, height);
}

canvas.onmousewheel = function(event){
	event.preventDefault();
};

canvas.onwheel = function(event){
	event.preventDefault();
	if ((event.wheelDelta < 0) & (rect_size > 10))
	{
		rect_size = rect_size -10;
	}
	else if ((event.wheelDelta>0)&(rect_size < 1020))
	{
		rect_size = rect_size +10;
	}
	console.log(rect_size);
};
// draw initial image.
const img = document.querySelector('img')
ctx.beginPath();
//ctx.drawImage(img,0,0);
ctx.stroke();
ctx.closePath();

function subsetImage(){
	ctx.beginPath();
	ctx.drawImage(img,0,0);
	centreRect(ctx,x,y, rect_size, canvas.height/canvas.width*rect_size);
	ctx.stroke();
	ctx.closePath();
};
function onetimeClick(){
    // Why is this removing the mouse functions? I don't see what actual purpose it serves. Maybe it should also     // remove the rectangle from the image.
	unclicked = 0;
    canvas.removeEventListener('wheel', subsetImage);
	canvas.removeEventListener('click',onetimeClick);
	ctx.beginPath();  // Added to remove rectangle after click.
	ctx.drawImage(img,0,0);
	ctx.stroke();
	ctx.closePath();

};
canvas.addEventListener('click', function(event) {

	// Get the x and y coordinates of the click 
	
	if (unclicked ==0)
	{	
		//rect_size = 10
		ctx.clearRect(0,0,canvas.width, canvas.height);	
		x = event.offsetX;
		y = event.offsetY;
		ctx.beginPath();
		ctx.drawImage(img,0,0);
		centreRect(ctx,x,y, rect_size,canvas.height/canvas.width*rect_size);
		ctx.stroke();
		ctx.closePath();
		// Log the coordinates to the console
		console.log(`Clicked at x=${x}, y=${y}`);

		unclicked =1;
		
		canvas.addEventListener('click',  onetimeClick);
        canvas.addEventListener('wheel', subsetImage);
	}

});




// End Drawing Logic

// Submit button Logic
const submit_button = document.getElementById('submit');
const data_div_next = document.getElementById('next-image-data');

submit_button.addEventListener('click', () => {
    console.log('Button Clicked');

    var params = JSON.stringify({"Name" : "Cameron"});
    console.log(params);
    let http = new XMLHttpRequest()
    http.open('POST', 'submit-image-params', true)
    http.setRequestHeader("Content-type", "application/json");
    http.responseType="json";
    http.onload = function() {
        console.log(this.response);
        update_image_param(this.response, data_div_next, next_image_display);
    };
    req_payload = JSON.stringify(
        {
            "rect_size": rect_size,
            "w":image_display['width'],
            "x":image_display['x'],
            "y":image_display['y'],
            "x_pixel_target":x,
            "y_pixel_target":y
        });
    console.log(req_payload)
    http.send(req_payload);
});
// End of Submit button logic. 

// Start Render button logic.
render_button.addEventListener('click', () =>{
        // Send a message to the server to render the image. The server probably already knows the 
        // coordinates given that it was the thing that sent the to the client in the first place.
        let http_render_params = new XMLHttpRequest();
         
        http_render_params.open('GET', 'submitrender', true);
        http_render_params.setRequestHeader("Content-type", "application/json");
        http_render_params.responseType="json";
        http_render_params.onload = function() {
            console.log(this.response);
        }
        http_render_params.send();
        console.log('Render Button pressed');
        //ctx.clearRect(0,0,canvas.width, canvas.height);
       //rendering = 1;
    }
);


// End Render button logic.


