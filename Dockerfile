FROM alpine:3.11.6 as builder
COPY src/ /pbot/
COPY Makefile /pbot/
RUN apk add g++ boost-dev
RUN make && make install
CMD ["PBotManager"]

