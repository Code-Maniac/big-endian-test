FROM multiarch/ubuntu-core:s390x-focal

RUN apt-get update
RUN apt-get install -y vim gcc cmake

RUN mkdir /home/big-endian-test
RUN mkdir /home/big-endian-test/src

COPY src/main.c /home/big-endian-test/src/main.c
COPY CMakeLists.txt /home/big-endian-test/CMakeLists.txt

RUN mkdir /home/big-endian-test/build && cd /home/big-endian-test/build && cmake .. && make && cp ./big-endian-test /bin

WORKDIR /home
