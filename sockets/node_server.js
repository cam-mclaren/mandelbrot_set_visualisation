/* Example of interprocess communication in Node.js using a UNIX domain socket */

var net = require('net'),
	fs = require('fs'),
	express = require('express'),
	http = require('http'),
	path = require('path'),
	process = require('process'),
    bodyParser = require('body-parser'),
    nextcentre = require('./nextcentre')['nextcentre']
	connections = {};

var {create, all} = require("mathjs");
var config= {
  number: 'BigNumber',      // Default type of number:
                            // 'number' (default), 'BigNumber', or 'Fraction'
  precision: 200,            // Number of significant digits for BigNumbers
  epsilon: 1e-200
};
var mathjs = create(all, config);
var image_x
var image_y
var image_width
var firstMessage = true

var image_pixels_high = 720
var image_pixels_wide = 1280


var WebSocket = require('ws');
var webSocket_server = new WebSocket.Server( { port: 3009 });



// Using this to call the docker container
var { spawn } = require("child_process");




const SOCKET_PATH = './socket.sock';
var nums_in_msg = 5
var num_size_bytes = 4

function createServer(socket){
	console.log('Creating server.');

	var server = net.createServer(function(stream) 
        {
		    console.log('Connection acknowledged.');
            stream.image_data = Buffer.alloc(nums_in_msg*num_size_bytes*image_pixels_wide*image_pixels_high)
            stream.received_image_index = 0
            stream.sent_data_index = 0
            this.mySocket = stream

		    var nowTime = Date.now();

		    connections[nowTime] = (stream); // <- What this is doing I am unsure

		    stream.on('end', function() {
			    console.log('Client disconnected.');
			    delete connections[nowTime];
		    })

        function firstMessage(msg) {
            console.log("First Message")
            // this = domain socket
            for(let index = 0; index < msg.length; index++)
            {   
                this.image_data[this.received_image_index + index ]=msg[index]
            }

            this.received_image_index = this.received_image_index + msg.length
            webSocket_server.websock.send(msg) 
            this.sent_data_index = this.sent_data_index + msg.length
            console.log(0,msg.length) 
            this.off('data', firstMessage)
            this.on('data', function(msg) {
            for(let index = 0; index < msg.length; index++)
            {   
                this.image_data[this.received_image_index + index ]=msg[index]
            }
            this.received_image_index = this.received_image_index + msg.length
            //console.log(this.received_image_index)
		    });
        }
        stream.on('data', firstMessage)
        

	})
	.listen(socket, () =>	{
			fs.chmodSync(socket, 777);
		}
	)
	.on('connection', function(socket){
		console.log('Client connected');
		//console.log('Send boop test');
		//stream.write('Boop test')
	});

		return server;
}


fs.unlink(path.join(process.cwd(),"socket.sock"), (err) =>
	{
		if (err) throw err;
		console.log("Hopefully the bloody socket got deleted");
	}
)

unix_sock_server = createServer(SOCKET_PATH);

webSocket_server.on('connection', function(ws) {
        
        console.log("Client connected to webSocket_server")
        ws.on('message', (msg) => {
            //console.log(msg.toString())
            start_index = unix_sock_server.mySocket.sent_data_index
            end_index = unix_sock_server.mySocket.received_image_index - unix_sock_server.mySocket.received_image_index % 20

            //console.log(start_index, end_index)
            ws.send( unix_sock_server.mySocket.image_data.slice(start_index,end_index))
            unix_sock_server.mySocket.sent_data_index = end_index
        });
        this.websock = ws
        //console.log(ws)
});




var app = express();
app.use(bodyParser.json({ extended: true}));

//var web_sockets = [];

var httpServer = http.createServer(app);

app.get('/', (req, res) => {
	res.sendFile(path.join(process.cwd(), 'html','index.html'))
})

app.get('/js/script.js', (req, res) => {
	res.sendFile(path.join(process.cwd(), 'js', 'script.js'));
})

app.get('/js/webworker.js', (req, res) => {
	res.sendFile(path.join(process.cwd(), 'js', 'webworker.js'));
})

app.get('/init-params', (req, res) => {
    console.log('init-params endpoint');
        res.json({
            "x": "-1.10941930270254405808689884994886904427000585937500",
            "y": "-0.25988683407403302019496355878071500627552734375000",
            "width": "0.000000000000000000000000000000000000009125"
        });
})

app.post('/submit-image-params', (req, res) => {
    client_data = req.body;
    console.log(client_data);
    let old_image_width= mathjs.bignumber(client_data['w']);

    next_x_y = nextcentre(
        mathjs.bignumber(client_data['x']),
        mathjs.bignumber(client_data['y']),
        mathjs.bignumber(client_data['w']),
        mathjs.bignumber(client_data['x_pixel_target']),
        mathjs.bignumber(client_data['y_pixel_target']),
        mathjs.bignumber(image_pixels_wide),
        mathjs.bignumber(image_pixels_high))

    image_x = next_x_y['new_centre_x']
    image_y = next_x_y['new_centre_y']
    image_width = mathjs.chain(old_image_width).divide(image_pixels_wide).multiply(client_data['rect_size']).done() 
    res.json(
                {
                    "x": mathjs.format(image_x, 200),
                    "y": mathjs.format(image_y, 200),
                    "width": mathjs.format(image_width,{'precision':200, 'notation' : 'fixed'})
                }
            )
})

app.get('/image.jpg', (req, res) => {
    res.sendFile(path.join(process.cwd(), 'image.jpg'));
})

app.get('/submitrender', (req, res) => {
    console.log("Render request received");

    let perm_proc = spawn("docker" , ["exec", "-w", "/usr/project/single_image/", "image_builder", "chmod", "777", "../sockets/socket.sock"]); 
    let docker_proc = spawn("docker" , ["exec", "-w", "/usr/project/single_image/", "image_builder", "/usr/project/single_image/main.out", mathjs.format(image_x, 200), mathjs.format(image_y, 200), mathjs.format(image_width,{'precision':200, 'notation' : 'fixed'}) ]); 

//    docker_proc.stdout.on("data", data => {
//        console.log(`stdout: ${data}`);
//    });
//
    docker_proc.stderr.on("data", data => {
        console.log(`stderr: ${data}`);
    });
    docker_proc.on('error', (error) => { console.log(`error: ${error}`)}); 


    res.send("Got it");
})


var httpPort = 3000
httpServer.listen( httpPort, () => {
	console.log(`Listening on Port ${httpPort}`)
}
)

