FROM ubuntu:22.04

WORKDIR /usr/local/app

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y make
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y ccache

EXPOSE 8000/tcp

# in prod it should just copy files
COPY public ./public
COPY Makefile ./Makefile
COPY bin ./bin
COPY build ./build

CMD ["./bin/main"]
