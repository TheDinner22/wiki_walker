### for the grader

information on running is in this README or you can watch this video tutorial (it's just Docker)

https://www.youtube.com/watch?v=ChC0edMr4no

### dependencies

RESTFUL HTTP BACKEND with `HTTPRequest` - https://github.com/elnormous/HTTPRequest

GET requests through curl lib - https://curl.se/libcurl/

simdjson - json parser for c++

FRONTEND with `htmx` - worlds best front-end (seriously I could talk about htmx for hours it's so cool)

```
I have no idea how to us lexbor and want to die
heres an example
https://stackoverflow.com/questions/69827153/lexbor-webscraping-an-html-table-in-c

Good thing I scrapped Lexbor!!
```

### docker

I have the whole thing stuck into a docker container so that it's easy to deploy

you can build the image and then run it with docker desktop

`docker build -t sampleapp:v5 .` note that port forwarding is required (it's a web app what did you expect?)!!!

#### docker dev

after building an image, run it in docker desktop
make sure you
- map 8000:8000
- mount project folder to /usr/local/app

finally
 `docker exec -it <name> sh`
 will give you a terminal inside the machine/image

run `make`
to run the server.

You should be able to modify files in vs code and see them update in Docker

### ideas

- time
- num requests
- pages visited
- min path len
- algo name
- picture of start and end
- the path

### todos

- also add some other web pages
- add set for many graphs
