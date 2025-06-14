


let hex_input = document.getElementById("hex_input_box");


let render_hex_data = function()
{
    //console.log(hex_input.value);
    image.set_buf_pos(0);
    image.append_data_buf(hex_string_to_uint8_array(hex_input.value));
    image.render();
}

let hex_string_to_uint8_array = function(hex_string)
{
    uint8_array = new Uint8Array(hex_string.length/2);
    for(let i = 0; i < hex_string.length; i+=2)
    {
        let hex = hex_string.substr(i, 2);
        let val = parseInt(hex, 16);
        uint8_array[i/2] = val;
    }
    return uint8_array;
    // return new TextEncoder().encode(hex_string);

}

let rgb565_to_rgba = function(big_endian, bytes)  //return 4 bytes rgba from 2 bytes rgb565
{
    let r,g,b,a;

    if(big_endian)
    {
        r = ((bytes[0] & 0b11111000) >>> 3) * 8.225806451612903;
        g = ( ((bytes[0] & 0b00000111) << 3) | ((bytes[1] & 0b11100000) >>> 5) ) * 4.047619047619048;
        b = (bytes[1] & 0b00011111) * 8.225806451612903;
        a = 255;
    }
    else
    {
        r = ((bytes[1] & 0b11111000) >>> 3) * 8.225806451612903;
        g = ( ((bytes[1] & 0b00000111) << 3) | ((bytes[0] & 0b11100000) >>> 5) ) * 4.047619047619048;
        b = (bytes[0] & 0b00011111) * 8.225806451612903;
        a = 255;
    }
    
    return [r,g,b,a];
}


class Picture
{
    //canvas;
    constructor(canvas_id)
    {
        this.canvas = document.getElementById(canvas_id);
        this.context = canvas.getContext("2d");
        this.buf_pos = 0;
        this.data_buf = new Uint8Array(this.canvas.width*this.canvas.height*2);  // TODO: Variable length size could be better so any extra could be transparent
        this.data_buf_format = "rgb565";
        this.image_data = new Uint8Array(this.canvas.width*this.canvas.height*4);  // saves the data into here // this remember the data for when the canvas resizes
        this.data_buf_offset = 0; //if one byte is missing this allows us to correct that
        this.big_endian = true;
    }

    clear()  // clear the data and image buffers to black
    {
        for(let i = 0; i < this.data_buf.length; i++)
        {
            this.data_buf[i] = 0;
        }
        for(let i = 0; i < this.image_data.length; i++)
        {
            this.data_buf[i] = 0;
        }
        this.data_buf_to_image_data();


    }

    resize(x, y)
    {
        this.canvas.width=x;
        this.canvas.height=y;
        this.data_buf = new Uint8Array(this.canvas.width*this.canvas.height*2);  //TODO: This loses the data from the buffers
        this.image_data = new Uint8Array(this.canvas.width*this.canvas.height*4);
        this.image_data_to_canvas();
    }

    // clear(colour) //TODO: Make this fill data buff instead
    // {
    //     this.context.fillStyle = colour;
    //     this.context.fillRect(0, 0, canvas.width, canvas.height);
    // }

    set_buf_pos(pos)
    {
        this.buf_pos = pos;
    }

    append_data_buf(data_array)
    {
        for(let i = 0; (i < data_array.length) && (this.buf_pos < this.data_buf.length); i++)
        {
            this.data_buf[this.buf_pos] = data_array[i];
            this.buf_pos++;
        }
    }

    data_buf_to_image_data() // interprets data buf values (in rgb565) to rgba8888 values for canvas context and saves to this.image_data
    {
        if(this.data_buf_format == "rgb565")
        {
                for(let i = 0; (i < (this.image_data.length/4)) && (i+this.data_buf_offset < (this.data_buf.length/2)); i++)
                {
                    // whether half a pixel or whole pixel determines if we draw black pixel
                    if(i+this.data_buf_offset < 0)  // every second byte we draw a black pixel
                    {
                        if(i+this.data_buf_offset % 2 === 0)
                        {
                            this.image_data[(i*4)] = 0
                            this.image_data[(i*4)+1] = 0
                            this.image_data[(i*4)+2] = 0
                            this.image_data[(i*4)+3] = 255;
                            continue;

                        }
                    }

                    let rgba_val = rgb565_to_rgba(this.big_endian, [this.data_buf[(i * 2)+this.data_buf_offset], this.data_buf[(i * 2) + 1 + this.data_buf_offset]]);
                    this.image_data[(i*4)] = rgba_val[0]
                    this.image_data[(i*4)+1] = rgba_val[1]
                    this.image_data[(i*4)+2] = rgba_val[2]
                    this.image_data[(i*4)+3] = rgba_val[3]
                }
            
            
            

        }
        else
        {
            alert("Unknown format")
        }

    }

    image_data_to_canvas()  // draws this.image_data to canvas
    {
        let new_data = this.context.getImageData(0, 0, this.canvas.width, this.canvas.height);
        for(let i = 0; (i < new_data.data.length) && (i < this.image_data.length); i++)
        {
            new_data.data[i] = this.image_data[i];
        }
        this.context.putImageData(new_data, 0, 0);
    }

    render()    // create image_data from data_buf and draw to canvas
    {
        this.data_buf_to_image_data();
        this.image_data_to_canvas();
    }





}




//let test_data = new Uint8Array([0b11111000,0b00000000,0b00000111,0b11100000,0b00000000,0b00011111]); //big endian
let test_data = new Uint8Array([0b00000000,0b11111000,0b11100000,0b00000111,0b00011111,0b00000000]); // little endian
let image = new Picture("canvas");









let port;
let reader;
let inputDone;
let outputDone;
let inputStream;
let outputStream;

const log = document.getElementById('serial_rx_out_text_box');



append_rx_out = function(value)
{
    log.value += String.fromCharCode(value);
}

print_stuff = function(value)
{
    console.log(String.fromCharCode(value));
}



const close_port = 0;
// document.addEventListener('DOMContentLoaded', () =>
// {
//     const notSupported = document.getElementById('notSupported');
//     notSupported.classList.toggle('hidden', 'serial' in navigator);
// });


open_port = async function()
{
    //setInterval(()=>{close_port = 1; console.log("port was closed by setinterval");}, 10000);
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 115200, bufferSize: 100 });

    
    // for(let i = 0; i < 140; i++)
    let func = async function()
    {
        let reader = port.readable.getReader();
        // const textDecoder = new TextDecoderStream();
        // const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
        // const reader = textDecoder.readable.getReader();
        if(port.readable)
        {
        
            //await reader.read();
            try
            {
                console.log("try");
                while(true)
                {
                    let { value, done } = await reader.read();
                    // if (done)
                    // {
                    //     console.log("done");
                    //     return;
                    // }
                    // Do something with |value|...
                    if(!done)
                    {
                        //console.log(value);
                        value.forEach(append_rx_out);
                        //value.forEach(print_stuff);
                        console.log("reading");

                    }
                    else
                    {
                        console.log("done");
                        return;
                    }
                }
            

            
            }
            catch (error)
            {
                console.log("catch");
                console.log(error);
                return;
            }
            finally
            {
                console.log("finally");
                reader.releaseLock();
            }
        }
        
    }
    
    
    setTimeout(func, 0);



    // console.log('Serial port opened successfully.');
    // console.log(port);
    // const reader = port.readable.getReader();

    // let decoder = new TextDecoderStream();
    // port.readable.getReader().releaseLock();
    // port.writable.getWriter().releaseLock();
    // inputDone = port.readable.pipeTo(decoder.writable);
    // inputStream = decoder.readable;

    // reader = inputStream.getReader();
    // read_loop();

}

read_loop = async function()
{
    // while (true) {
    //     const { value, done } = await reader.read();
    //     if (value) {
    //       log.textContent += value + '\n';
    //     }
    //     if (done) {
    //       console.log('[readLoop] DONE', done);
    //       reader.releaseLock();
    //       break;
    //     }
    //   }
}

async function main()
{
    
    //infinite loop of
    //
    // listen to port
    // if received a new frame beginning marker then read data and draw frame
    // if 
    
}


main();
