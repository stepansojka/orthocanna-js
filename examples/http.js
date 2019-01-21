server = new http.Server();

server.on('GET', '/hello', (req, resp) => {
    resp.status = 200;
    resp.body = "hello js";
});

server.listen(10000).then(
    () => console.log('listening on core', core.number())
);


