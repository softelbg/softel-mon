

var thrift = require('thrift');
var trade = require('./gen-nodejs/BaseMachine');
var ttypes = require('./gen-nodejs/trade_types');

const https = require('https');
var express = require('express');

var app = express();

//transport = thrift.TFramedTransport()
transport = require('./node_modules/thrift/lib/nodejs/lib/thrift/framed_transport.js')
protocol = thrift.TJSONProtocol()

var connection = thrift.createConnection("localhost", 6010, {
  transport : transport,
  protocol : protocol
});

connection.on('error', function(err) {
  console.log("error: ", err);
});

// Create a Trade client with the connection
var thriftClient = thrift.createClient(trade, connection);

thriftClient.getSymbolsSrc(function (err, thriftResponse) {
	console.log("getSymbolsSrc: " + thriftResponse);
	console.log("sym = " + thriftResponse[0].name);
});


//app.use(express.static(__dirname));

app.get('/', function (req, res) {
	res.send("Use /v0/apiCall for API ver 0 <br><a href=\"/v0/getSymbolsSrc\">getSymbolsSrc</a>"); 
});

app.get('/v0/getSymbolsSrc', function (req, res) {
	thriftClient.getSymbolsSrc("", function (err, thriftResponse) {
		console.log("getSymbolsSrc: " + thriftResponse);
		res.send("getSymbolsSrc: " + thriftResponse);
	});
});

var port = process.argv[2];

app.listen(port, function () {
  console.log('listening on port ' + port);
});


