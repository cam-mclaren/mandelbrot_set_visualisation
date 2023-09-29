const socket = new WebSocket('ws://localhost:3009');

onmessage = function handleMessageFromMain(msg){
}


var nums_in_msg = 5
var INTSIZE = 2
var MSG_SIZE=nums_in_msg*INTSIZE
var image_pixels_high = 720
var image_pixels_wide = 1280
var sab
try {
    sab = new SharedArrayBuffer(image_pixels_high*image_pixels_wide*MSG_SIZE)
    postMessage("Successfully initialised sab")
}
catch(err){
    postMessage(`Could not initialise shared array buffer \n${err}`)
    postMessage("Consider headers to toggle crossOrignIsolatedVariable")
}
postMessage(sab)

var write_index = 0
try {
    var imageData = new Uint16Array(sab)
}
catch(err)
{
    postMessage(`Could not create Uint16array view \n${err}`)
}

function waitFun() {
    socket.send('msg 4rm client')
    state = 'notwaiting' 
}

state = 'notwaiting'
socket.onmessage = function(event){

    event.data.arrayBuffer().then( (value) =>
        {
            num_data = new Uint16Array(value)
            data_len = num_data.length
            imageData.set(num_data,write_index)
            write_index = write_index + data_len
            postMessage({"write_index": write_index})

            if (state = 'notwaiting')
            {
                setTimeout(waitFun, 400)
                state = 'waiting'
            }

        }
    ).catch(err => {
        postMessage(`Could not create Uint16array view \n${err}`)
    });
}





