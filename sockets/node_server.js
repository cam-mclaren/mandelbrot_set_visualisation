




/* Example of interprocess communication in Node.js using a UNIX domain socket */

var 	net = require('net'),
	fs = require('fs'),
	express = require('express'),
	http = require('http'),
	path = require('path'),
	process = require('process'),
	io = require('socket.io')(server, { cors: { origin: '*'}}),
	connections = {},
	server;



var app = express();


//var web_sockets = [];
var my_web_socket

io.on('connection', function(socket) {
	console.log('A user connected');

	socket.on('event', function() {
		io.emit('another_event', message);
	})

	socket.on('disconnect', function() {
		console.log('A user disconnected.');
	})

	my_web_socket = socket;
})



var httpServer = http.createServer(app);

app.get('/', (req, res) => {
	res.sendFile(path.join(process.cwd(), 'html','index.html'))
})

app.get('/js/script.js', (req, res) => {
	res.sendFile(path.join(process.cwd(), 'js', 'script.js'));
})	

;


const SOCKET_PATH = './socket.sock';


function createServer(socket){
	console.log('Creating server.');
	var server = net.createServer(function(stream) {
		console.log('Connection acknowledged.');


		var self = Date.now();

		connections[self] = (stream); // <- What this is doing I am unsure
		stream.on('end', function() {
			console.log('Client disconnected.');
			delete connections[self];
		})

		stream.on('data', (msg) => {
			//msg = msg.toString();
			my_web_socket.emit('news', msg);			
			//console.log("msg:\n"+msg+"\nmsgend");	
		});

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

server = createServer(SOCKET_PATH);

var httpPort = 3000
httpServer.listen( httpPort, () => {
	console.log(`Listening on Port ${httpPort}`)
}
)

io.listen(3009)
