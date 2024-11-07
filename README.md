### dependencies

HTTPRequest - https://github.com/elnormous/HTTPRequest
curl lib - https://curl.se/libcurl/
simdjson - json parser for c++
htmx - worlds best front-end

```
I have no idea how to us lexbor and want to die
heres an example
https://stackoverflow.com/questions/69827153/lexbor-webscraping-an-html-table-in-c
```

### docker

I have the whole thing stuck into a docker container so that it's easy to deploy

you can build the image and then run it with docker desktop
`docker build -t sampleapp:v5 .`

#### docker dev

after building an image, run it in docker desktop
make sure you
- map 8000:8000
- mount project folder to /usr/local/app

finally
 `docker exec -it <name> sh`
 will give you a terminal inside the machine

run `make`
to run the server.

You should be able to modify files in vs code and see them update in Docker


