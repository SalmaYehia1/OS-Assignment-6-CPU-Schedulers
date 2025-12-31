FROM debian:stable-slim

RUN apt-get update && \
    apt-get install -y g++ make diffutils && \
    rm -rf /var/lib/apt/lists/*
WORKDIR /app

COPY . .


RUN g++ -std=c++11 -O2 -o lab7 lab7.cpp
CMD ["/bin/bash"]