const input = prompt("Please enter some input:");
if (!input){
	alert('No input provided. Please refresh the page and try again.');
}
// Create the WebSocket connection
const socket = new io('ws://localhost:3009');

const canvas = document.querySelector('canvas');
const ctx = canvas.getContext('2d');

canvas.addEventListener('click', function(event) {

  // Get the x and y coordinates of the click
  const x = event.offsetX;
  const y = event.offsetY;

  // Log the coordinates to the console
  console.log(`Clicked at x=${x}, y=${y}`);
});



function colorPixel(pixel) {

	const [x, y, r, g, b ] = pixel;

	// Set the fill style to the provided rgb colour
	ctx.fillStyle=`rgb(${r}, ${g}, ${b})`;

	//calculate size of pixel and draw it.
	
	const pixelSize = 1;
	ctx.fillRect(x,y,pixelSize,pixelSize);
}



let uint32Array = new Uint32Array(5);
var x, y, r, g ,b;
var j;
var i;
socket.on('news' , function(data) {

	//console.log(data);
	//console.log(data.byteLength);

	msg_num = data.byteLength/20;
	//console.log(msg_num);
	if (msg_num ==0)
	{
		throw new Error("Error: How come it read 0 messages from the socket?");
	}

	for ( i = 0 ; i < msg_num ; i ++)
	{
		try
		{
			//console.log(data.slice(i*20, (i+1)*20));
			uint32Array.set(
				new Uint32Array
				(
					data.slice
					(
						i*20, (i+1)*20)
					)
				);
		}
		catch(err) 
		{
			console.log(err);
		}
		//console.log(uint32Array);

		colorPixel(Array.from(uint32Array));
	}
})


