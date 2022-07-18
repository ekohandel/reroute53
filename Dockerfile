ARG INSTALL_PREFIX="/opt"
ARG LD_LIBRARY_PATH="${INSTALL_PREFIX}/lib"
ARG PKG_CONFIG_PATH="${INSTALL_PREFIX}/lib/pkgconfig"

######## Development Container ###########
FROM debian as development-container

ARG INSTALL_PREFIX
ARG LD_LIBRARY_PATH
ARG PKG_CONFIG_PATH

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH
ENV PKG_CONFIG_PATH=$PKG_CONFIG_PATH

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    build-essential \
    ca-certificates \
    cmake \
    gdb \
    git \
    libboost-all-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    pkg-config \
    sudo

RUN git clone https://github.com/jpbarrette/curlpp.git /var/curlpp \
    && cd /var/curlpp \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX .. \
    && make install

RUN git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp /var/aws-sdk-cpp \
    && cd /var/aws-sdk-cpp \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_CXX_FLAGS="-Wno-error=deprecated-declarations" -DCMAKE_BUILD_TYPE=Release -DBUILD_ONLY="route53" -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
    && make install aws-cpp-sdk-route53

ARG uid=1000
ARG gid=1000
ARG user=user
RUN addgroup --gid $gid $user
RUN adduser --uid $uid --gid $gid $user
RUN usermod -a -G sudo $user
RUN echo "${user} ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

######## Build Container ###########
FROM development-container as build-container

ARG INSTALL_PREFIX

ADD . /src
RUN cd /src \
    && mkdir build \
    && cd build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX \
    && make install reroute53

######## Runtime Container ###########
FROM debian as runtime-container

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    ca-certificates \
    libcurl4

ARG INSTALL_PREFIX
ARG LD_LIBRARY_PATH

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH
ENV INSTALL_PREFIX=$INSTALL_PREFIX

COPY --from=build-container $INSTALL_PREFIX $INSTALL_PREFIX

ENTRYPOINT ["/bin/bash", "-c", "$INSTALL_PREFIX/bin/reroute53"]
