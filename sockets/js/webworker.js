// Script to maintain socket connection

const socket = new WebSocket('ws://localhost:3009');
let i;


onmessage = function handleMessageFromMain(msg){
}

//im_buff defintion
nums_in_msg=5;
nums_size_bytes = 4;
image_pixels_wide = 1280
image_pixels_high = 720
im_buff = Buffer.alloc(nums_in_msg*num_size_bytes*image_pixels_wide*image_pixels_high)
im_buff_index=0
im_buff_sent_index=0


socket.onmessage = function(event) { 
	event.data.arrayBuffer().then( (value) => 
        {
            //console.log(value)
            debug_len = value.byteLength
	        msg_num = value.byteLength/20;
            postMessage(debug_len) 
            if (msg_num ==0)
	        {
		    //throw new Error("Error: How come it read 0 messages from the socket?");
                status_msg = 'no data'
    	    }
            else
            {
                image_data = []
	            for ( i = 0 ; i < msg_num ; i ++)
	            {
                    try
                    {
                        image_data.push(Array.from(
                        new Uint32Array(value.slice(i*20, (i+1)*20))));
                        //postMessage(image_data)
                        status_msg = 'data'
                    }
                    catch(err) 
                    {
                        console.log(err);
                        status_msg='error'
                    }
            }
            value = null
            }
            setTimeout(socket.send(status_msg), 7000)
    });
	//console.log(data.byteLength);
};

