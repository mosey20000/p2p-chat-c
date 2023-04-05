FROM ubuntu:22.04

WORKDIR /usr/src/app

COPY . .

RUN apt update
RUN apt install net-tools

CMD ["./C_P2P_Chat", "-name", "Hosting"]
