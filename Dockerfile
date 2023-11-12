FROM ubuntu:latest

RUN apt-get update
RUN apt-get install --no-install-recommends -y sudo \
    cmake \
    build-essential \
    qtbase5-dev \
    qt5-qmake \
    && rm -rf /var/lib/apt/lists/*

COPY . /

# Build
WORKDIR /build
RUN cmake .. && make app

WORKDIR /

CMD ["./bin/app"]