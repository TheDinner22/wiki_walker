FROM ubuntu:22.04

WORKDIR /usr/local/app

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y make
RUN apt-get install -y libcurl4-openssl-dev
RUN apt-get install -y ccache
#RUN DEBIAN_FRONTEND=noninteractive apt-get install cmatrix -y

EXPOSE 8000/tcp

# in prod it should just copy files
#COPY src ./src/*
#COPY public ./public/*
#COPY Makefile ./Makefile

CMD ["sleep", "infinity"] # CMD["make"] # in prod it should just run
