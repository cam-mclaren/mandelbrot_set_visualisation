
/* Example of interprocess communication in Node.js using a UNIX domain socket */

var net = require('net'),
	fs = require('fs'),
	connections = {},
	server;


const SERVER_PATH = './socket.sock';


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
			msg = msg.toString();

			console.log(msg);

			if (msg === 'here come dat boi'){
				//stream.write('Kill yourself.')
				}
			//stream.write(msg)

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

server = createServer(SERVER_PATH);
