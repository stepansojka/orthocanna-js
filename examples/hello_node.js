const http = require('http');

const hostname = '0.0.0.0';
const port = 10000;

const server = http.createServer((req, res) => {
    res.end('Hello World\n');
});

server.listen(port, hostname, () => {
    console.log(`running at http://${hostname}:${port}/`);
});
