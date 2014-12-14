var app = require('express')();
var http = require('http');
var http_server = http.Server(app);
var io = require('socket.io')(http_server);

app.get('/', function (req, res) {
	// get current light status
	http.get('http://arduino.local/arduino/digital/13', function (res) {
		res.on('data', function (chunk) {
			console.log(chunk.toString());
			io.emit('change status', chunk.toString());
		})
	});
	// show home
	res.sendFile(__dirname + '/index.html');
});

function changeStatus(status) {
	if(status == 'on') {
		// turn lights on
		http.get('http://arduino.local/arduino/digital/13/1');
	} else {
		// turn lights off
		http.get('http://arduino.local/arduino/digital/13/0');
	}
}

// listen for user connections
io.on('connection', function (socket) {
	console.log('a user has connected');

	socket.on('disconnect', function () {
		console.log('user disconnected');
	});

	socket.on('change status', function (msg) {
		console.log('Status: ', msg);
		changeStatus(msg);
		io.emit('change status', msg);
	});

});

http_server.listen(3000, function () {
	console.log('Listening on port 3000');
});
