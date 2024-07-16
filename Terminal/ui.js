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
    .then(result => {serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData()})
    .catch((error) => console.error(error)); 

}

let sendSerial = function(tx)
{
    serial.sendSerial(tx + "\r")
    .then(result => {serialRxBox.value = result; document.getElementById("hex-input-box").value = result; renderHexData()})   //response is sent back
    .catch((error) => console.error(error)); 

}