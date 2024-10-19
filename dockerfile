FROM ubuntu:22.04

WORKDIR /usr/local/app


RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y make
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y curl


RUN mkdir -p  /etc/apt/keyrings
RUN curl https://lexbor.com/keys/lexbor_signing.key | gpg --dearmor --output /etc/apt/keyrings/lexbox.gpg

RUN echo "deb-src [signed-by=/etc/apt/keyrings/lexbox.gpg] https://packages.lexbor.com/ubuntu/ focal liblexbor-dev" >> /etc/apt/sources.list.d/lexbor.list
RUN echo "deb [signed-by=/etc/apt/keyrings/lexbox.gpg] https://packages.lexbor.com/ubuntu/ focal liblexbor" >> /etc/apt/sources.list.d/lexbor.list

RUN apt-get update
RUN apt-get install -y liblexbor
RUN apt-get install -y liblexbor-dev

COPY src ./src/*
COPY Makefile ./Makefile

CMD ["make"]
