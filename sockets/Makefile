
runmain: main
	chmod 777 socket.sock && ./main

main: socket_client.c
	gcc socket_client.c -o main && chmod +x main

node:
	node node_server.js

clean: 
	rm main
