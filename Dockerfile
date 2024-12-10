FROM janvik/oatpp:latest

WORKDIR /
ADD . /selene
WORKDIR ./selene

RUN make testServer
RUN ./testServer

EXPOSE 8400 8400

RUN make seleneServer
ENTRYPOINT ["./seleneServer"]

# Used to allow for connection to container so can use commandline
#ENTRYPOINT ["sleep", "infinity"]
