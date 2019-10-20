FROM ubuntu:18.04
RUN apt-get update && apt-get -y install sudo
# install tzdata without interaction
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/New_York
RUN sudo apt-get install -y tzdata
# Prereqs
RUN sudo apt-get install -qqy gnupg wget git g++ make pkg-config
# update Pure repo
RUN wget -nv https://download.opensuse.org/repositories/home:aggraef:pure/xUbuntu_18.04/Release.key
RUN sudo apt-key add Release.key
RUN sudo sh -c "echo 'deb http://download.opensuse.org/repositories/home:aggraef:pure/xUbuntu_18.04/ /' > /etc/apt/sources.list.d/home:aggraef:pure.list"
RUN sudo apt update
# install Pure
RUN sudo apt -y install pure-complete
ENV PKG_CONFIG_PATH="/usr/local/bin:/usr/local/include/pure:/usr/local/lib:/usr/local/lib/pkg-config:/usr/local/lib/pure"
# install Montre
RUN git clone https://github.com/doganulus/montre
WORKDIR montre
# make file expects to create a dir named montre
RUN mv montre pymontre
RUN make && make install
RUN ldconfig
WORKDIR ..
