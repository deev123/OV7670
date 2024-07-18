/***
 * tabName is the id of the element to show
 */
let setDataSourceTab = function(evt, tabName)
{
    ///let tab = document.getElementById(tabName);
    //let tabs = tab.parentElement.children;
    let tabs = document.getElementById("select-data-source").children;

    for (i = 0; i < tabs.length; i++)
    {
        if(tabs[i].getAttribute("data-tab") === tabName)
        {
            tabs[i].classList.add("active");
        }
        else
        {
            tabs[i].classList.remove("active");
        }
    }

    let tabContents = document.getElementById("data-source-content").children;
    for (i = 0; i < tabContents.length; i++)
    {
        if(tabContents[i].getAttribute("data-tab") === tabName)
        {
            tabContents[i].style = "display: block";
        }
        else
        {
            tabContents[i].style = "display: none";
        }
    }
    
}


let setImageWidth = function(width)
{

}


let setImageHeight = function(height)
{

}


let toggleBigEndian = function()
{

}


let setOffset = function(offset)
{

}


let restartSerial = function()
{
    serial.restartSerial()
    .then(result => {serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData()})
    .catch((error) => console.error(error));
}

let readSerial = function()
{
    serial.readSerial()
    .then(result => {/*console.log(result);*/ serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData();})
    .catch((error) => console.error(error)); 

}

let sendSerial = function(tx)
{
    return serial.sendSerial(tx + "\r")
    .then(result => {serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData()})   //response is sent back
    .then(readSerial())
    .catch((error) => console.error(error)); 
}

let sendAndReceive = async function(tx)
{
    console.log("sendAndReceive");
    await serial.sendSerial(tx + "\r"); // writes, then sends back empty string

    await serial.readSerial()  // returns the data on the rx serial buffer
    .then(result => {/*console.log(result);*/ serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData();})   //response is sent back
    //.then(readSerial())
    .catch((error) => console.error(error)); 
}

let isStreaming = false;
let toggleStream = async function()
{
    if(isStreaming)
    {
        isStreaming = false;
    }
    else
    {
        isStreaming = true;
        constantStream();
    }
}

let constantStream = async function()
{
    console.log("streaming");
    while(isStreaming)
    {
        await takePic();
        //await new Promise(resolve => setTimeout(resolve, 1000));
    }
}

let takePic = async function()
{
    await sendAndReceive("PIC");
}