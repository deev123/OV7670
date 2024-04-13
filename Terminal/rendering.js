let hexInput = document.getElementById("hex-input-box");


let renderHexData = function()
{
    //console.log(hexInput.value);
    image.setBufPos(0);
    image.appendDataBuf(hexStringToUint8Array(hexInput.value));
    image.render();
}


let hexStringToUint8Array = function(hex_string)
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


let rgb565ToRgba = function(bigEndian, bytes)  //return 4 bytes rgba from 2 bytes rgb565
{
    let r,g,b,a;

    if(bigEndian)
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


class Renderer
{
    /**
     * 
     * @param {*} canvasId The canvas element to render the image to.
     * 
     * Width and height of the render will match the canvas but can be changed by the `resize` method
     */
    constructor(canvasId)
    {
        this.canvas = document.getElementById(canvasId);
        this.context = canvas.getContext("2d");

        // array to store the raw data in for the image
        this.bufPos = 0;
        this.dataBuf = new Uint8Array(this.canvas.width * this.canvas.height * 2);  // TODO: Variable length size could be better so any extra could be transparent
        this.dataBufFormat = "rgb565";
       
        // the buffer to store canvas rgba values in for the context
        this.imageData = new Uint8Array(this.canvas.width * this.canvas.height * 4);  // saves the data into here // this remember the data for when the canvas resizes
        this.dataBufOffset = 0; //if one byte is missing this allows us to correct that
        this.bigEndian = true;
    }


    /**
     * fill dataBuf with 0s
     */
    clear()  // clear the data and image buffers to black
    {
        for(let i = 0; i < this.dataBuf.length; i++)
        {
            this.dataBuf[i] = 0;
        }
        for(let i = 0; i < this.imageData.length; i++)
        {
            this.dataBuf[i] = 0;
        }
        this.dataBufToImageData();
    }


    /**
     * 
     * @param {*} x width to set the image and canvas to
     * @param {*} y height to set the image and canvas to
     */
    resize(x, y)
    {
        this.canvas.width=x;
        this.canvas.height=y;

        this.dataBuf = new Uint8Array(this.canvas.width*this.canvas.height*2);  //TODO: This loses the data from the buffers
        this.imageData = new Uint8Array(this.canvas.width*this.canvas.height*4);
        
        renderHexData();
        this.imageDataToCanvas();
        
    }


    /**
     * 
     * @param {*} pos the index of dataBuf to set the position to append from to
     */
    setBufPos(pos)
    {
        this.bufPos = pos;
    }


    /**
     * 
     * @param {*} dataArray the array of values to add to data buff starting from bufPos
     * 
     * bufPos will move to end of the current bufPos + the added values
     * set bufPos with setBufPos method
     */
    appendDataBuf(dataArray)
    {
        for(let i = 0; (i < dataArray.length) && (this.bufPos < this.dataBuf.length); i++)
        {
            this.dataBuf[this.bufPos] = dataArray[i];
            this.bufPos++;
        }
    }


    dataBufToImageData() // interprets data buf values (in rgb565) to rgba8888 values for canvas context and saves to this.imageData
    {
        if(this.dataBufFormat == "rgb565")
        {
                for(let i = 0; (i < (this.imageData.length/4)) && (i+this.dataBufOffset < (this.dataBuf.length/2)); i++)
                {
                    // whether half a pixel or whole pixel determines if we draw black pixel
                    if(i+this.dataBufOffset < 0)  // every second byte we draw a black pixel
                    {
                        if(i+this.dataBufOffset % 2 === 0)
                        {
                            this.imageData[(i*4)] = 0
                            this.imageData[(i*4)+1] = 0
                            this.imageData[(i*4)+2] = 0
                            this.imageData[(i*4)+3] = 255;
                            continue;

                        }
                    }

                    let rgbaVal = rgb565ToRgba(this.bigEndian, [this.dataBuf[(i * 2)+this.dataBufOffset], this.dataBuf[(i * 2) + 1 + this.dataBufOffset]]);
                    this.imageData[(i*4)] = rgbaVal[0]
                    this.imageData[(i*4)+1] = rgbaVal[1]
                    this.imageData[(i*4)+2] = rgbaVal[2]
                    this.imageData[(i*4)+3] = rgbaVal[3]
                }
            
            
            

        }
        else
        {
            alert("Unknown format")
        }

    }
    

    imageDataToCanvas()  // draws this.imageData to canvas
    {
        let newData = this.context.getImageData(0, 0, this.canvas.width, this.canvas.height);
        for(let i = 0; (i < newData.data.length) && (i < this.imageData.length); i++)
        {
            newData.data[i] = this.imageData[i];
        }
        this.context.putImageData(newData, 0, 0);
    }
    

    render()    // create imageData from dataBuf and draw to canvas
    {
        this.dataBufToImageData();
        this.imageDataToCanvas();
    }


}


//let testData = new Uint8Array([0b11111000,0b00000000,0b00000111,0b11100000,0b00000000,0b00011111]); //big endian
let testData = new Uint8Array([0b00000000,0b11111000,0b11100000,0b00000111,0b00011111,0b00000000]); // little endian
let image = new Renderer("canvas");