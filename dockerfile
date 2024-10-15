FROM ubuntu:22.04

WORKDIR /usr/local/app

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y make
RUN apt-get install -y libcurl4-openssl-dev

COPY src ./src/*
COPY Makefile ./Makefile

CMD ["make"]
