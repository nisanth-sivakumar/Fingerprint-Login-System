const express = require('express'); // Import
const app = express(); // Create express app
var http = require('http').Server(app); // HTTP server
var server = http.listen(3000, "0.0.0.0", () => { //Start the server, listening on port 3000.
    console.log("Listening to requests on port 3000...");
})

var SerialPort = require("serialport"); // Import

var arduinoCOMPort = "COM4"; // Arduino COM port

var arduinoPort = new SerialPort(arduinoCOMPort,{ // SerialPort 
    baudRate: 9600,
    dataBits: 8,
    parity: 'none',
    stopBits: 1,
    flowControl: false
});

// add listener to read from serial port
var response;
const parsers = SerialPort.parsers;
const parser = new parsers.Readline({delimiter: "\r\n",});
arduinoPort.pipe(parser);
parser.on("data", function (data) {
    response = data;
    console.log("Received data from port: " + data);
});

// Event listener for on functions
arduinoPort.on('open',function() {
    console.log('Serial Port ' + arduinoCOMPort + ' is opened.');
  });

// middleware to load static files
const path = require('path');
app.use(express.static(__dirname + '/public'));

app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname, '/index.html'));
})
  
app.get('/:action', function (req, res) {  
    var action = req.params.action || req.param('action');
    
        arduinoPort.write(action); // Action to arduino
        setTimeout(function() {
            if(response == 'v') { // Response
                res.send('Login Successful!'); // Message to web page
                response = 'i'; // Reset response variable
            }
            else {
                res.send('Login Failed.'); // Message to web page
                response = 'i'; // Reset response variable
            }
        }, 500);  
        return;
   
});
