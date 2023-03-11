#### test_task

# Web-server on pure C-language in Docker container without external libraries.

### Server start
`make run` start the server on port `8085` and use the directory `./src/templates/` for html files<br>

### Command Line Interface
`-p --port` set any free port from 1 to 65535 (default 8085)<br>
`-d -directory` set the path to the directory with html files (default `./srd/templates/`) <br>

Arguments can be set in the `ARGS` variable when calling `make run` <br>
For example, `make run ARGS="-p 4422 -d ./"` starts server on port 4422 and looks for html files in the current
directory <br>

### Requests
For any request type (except DELETE), the server searches for the requested .html file in the directory mentioned at
start. If it is found, the server sends the content in response.<br>

`http://127.0.0.1:8085/about_me.html` requests can be sent through the browser line, just enter a similar address
to the command line<br>
`curl http://127.0.0.1:8085/hello.html` you can also send a request through the terminal using `curl` utility<br>
`curl -X DELETE http://127.0.0.1:8085/hello.html` this command will delete file `hello.html` on the server<br>

### Docker
The web server can be hosted in an Ubuntu 20.04 container <br>
`docker-compose up` start the server on port 8085 and use the directory `/server/src/templates/` <br>
This directory is mounted to a similar directory on the local machine<br>

If you need more detailed port settings, you can build the docker manually:<br>
`docker build -t web_c .` to build the image, you need to call this command from the root directory of the project<br>
`docker run --name test -it -v $(pwd)/src/templates:/server/src/templates -d -p 8082:8085 web_c` run server <br>

In this configuration, requests can be sent to machine port 8082, the server inside the container will be launched on
8085 port <br>
`docker exec -it test make test` run unit-tests inside a docker container <br>

### Tests
- `make test` run unit-tests implemented with the `-lcheck` library on the local machine<br>
- `pytest` run web server tests<br>
  IMPORTANT! To run the tests correctly, a web-server inside a Docker must be running on the default port 8082, bound to
  the default directory `./src/templates`<br>
- `make docker-test` run unit-tests in Docker. Before executing this command, you must also raise the web server in
  Docker<br>

### Makefile
- `run` start the server on the local machine<br>
- `build` build object files<br>
- `test` run tests on the local machine<br>
- `docker-test` run tests inside a Docker container<br>
- `make create_lib` build a static library with functions from `cnet` and `http` modules<br>
- `make clean` clear `.o`, `.out`, and `.a` files<br>
