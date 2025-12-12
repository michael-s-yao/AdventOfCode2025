FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive \
    BOOST_VERSION=1.89.0 \
    BOOST_DIR=boost_1_89_0 \
    BOOST_TARBALL=boost_1_89_0.tar.bz2 \
    PATH=/usr/local/bin:$PATH

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
       build-essential \
       g++ \
       wget \
       curl \
       ca-certificates \
       libboost-all-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . ./
CMD ["/bin/bash", "solve.sh"]
