// Script to maintain socket connection


const socket = new WebSocket('ws://localhost:3009');
let i;
let time = Date.now();

let image_data = []

socket.onmessage = function(event) { 
	event.data.arrayBuffer().then( (value) => {

        //console.log(value)

	    msg_num = value.byteLength/20;
        
        if (msg_num ==0)
	    {
		    throw new Error("Error: How come it read 0 messages from the socket?");
    	}
	    for ( i = 0 ; i < msg_num ; i ++)
	    {
            try
            {
                //console.log(data.slice(i*20, (i+1)*20));
                image_data.push(Array.from(
                        new Uint32Array(value.slice(i*20, (i+1)*20))
                    )
                );
            }
            catch(err) 
            {
                console.log(err);
            }
            //console.log(uint32Array); 
        }
        value = null
        if ((Date.now() -time) > 100)
        {
            time = Date.now();
            postMessage(image_data);
            image_data=[];
        }


    });
	//console.log(data.byteLength);

	

};

