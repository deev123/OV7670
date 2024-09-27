class SerialConnection
{
    constructor()
    {
        this.baudRate = 115200;
        this.comPort = "COM4";
        this.server = "http://localhost:5000";
        this.bufferSize = 0;        // TODO: Use all these fields
    }


    async restartSerial()
    {
        console.log("restartSerial()");
        const requestOptions = {
        method: "GET",
        redirect: "follow"
        };

        let data = await fetch(this.server, requestOptions)
        .then((response) => response.text())
        .then((result) => {return result;})
        .catch((error) => console.error(error)); 

        return data;
    }


    async readSerial()
    {
        console.log("readSerial()");
        const requestOptions = {
        method: "GET",
        redirect: "follow"
        };

        let data = await fetch(this.server + "/serial", requestOptions)
        .then((response) => response.text())
        .then((result) => {return result;})
        .catch((error) => console.error(error)); 

        return data;
    }
    

    async sendSerial(tx)
    {
        const myHeaders = new Headers();
        myHeaders.append("Content-Type", "text/plain");

        const requestOptions = {
        method: "POST",
        headers: myHeaders,
        body: tx,
        redirect: "follow"
        };

        let data = await fetch(this.server + "/send", requestOptions)
        .then((response) => response.text())
        .then((result) => {return result})
        .catch((error) => console.error(error));

        return data;
    }



}

let serial = new SerialConnection();
let serialRxBox = document.getElementById("serial-rx-out-text-box");