FROM ubuntu
ENV TZ=Europe/Moscow
RUN apt-get clean && apt-get update -y 
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata git sudo build-essential autoconf libtool libssl-dev zlib1g-dev librdkafka-dev pkg-config cmake gcc git g++ mysql-client libmysqlclient-dev libboost-all-dev libssl-dev && apt-get clean
RUN git clone -b poco-1.11.7-release https://github.com/pocoproject/poco.git && cd poco && mkdir cmake-build && cd cmake-build && cmake .. && make && sudo make install && cd .. && cd ..
RUN git clone https://github.com/edenhill/librdkafka.git && cd librdkafka && ./configure && make && make install && cd ..
RUN git clone https://github.com/mfontanini/cppkafka && cd cppkafka && mkdir build  && cd build  && cmake .. && make && make install  && cd .. && cd ..
RUN git clone https://github.com/tdv/redis-cpp.git && cd redis-cpp && mkdir build && cd build && cmake .. && make && make install && cd .. && cd ..
ARG CACHEBUST=1
RUN git clone https://github.com/Kircatle/hl_mai_lab_00.git
RUN sudo ldconfig
ARG CACHEBUST=1
RUN cd hl_mai_lab_00/src && mkdir build && cd build && cmake .. && make
ENTRYPOINT [ "hl_mai_lab_00/src/build/delivery_service" ]