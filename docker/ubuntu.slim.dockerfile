FROM ubuntu:22.04 as builder

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        git \
        curl \
        g++ \
        make \
        gnupg \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN curl -fsSL https://download.opensuse.org/repositories/home:aggraef:pure/xUbuntu_22.04/Release.key \
    | gpg --dearmor | tee /etc/apt/trusted.gpg.d/home_aggraef_pure.gpg > /dev/null \
    && sh -c "echo 'deb http://download.opensuse.org/repositories/home:/aggraef:/pure/xUbuntu_22.04/ /' | tee /etc/apt/sources.list.d/home:aggraef:pure.list"

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install \
        pure-complete \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/* 

RUN git clone https://github.com/doganulus/montre src/montre
WORKDIR src/montre
RUN git checkout e034a9d 
RUN mv montre pymontre
RUN make && make install

FROM ubuntu:22.04

# ldd /usr/local/montre
#
# linux-vdso.so.1 (0x00007ffcc37ed000)
# libmontre.so => /usr/local/lib/libmontre.so (0x00007f6e44b7e000)
# libpure.so.8 => /lib/libpure.so.8 (0x00007f6e44847000)
# libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f6e4461d000)
# libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f6e44536000)
# libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f6e44516000)
# libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f6e442ec000)
# libLLVM-3.5.so => /lib/libLLVM-3.5.so (0x00007f6e43365000)
# libmpfr.so.6 => /lib/x86_64-linux-gnu/libmpfr.so.6 (0x00007f6e430b6000)
# libgmp.so.10 => /lib/x86_64-linux-gnu/libgmp.so.10 (0x00007f6e43034000)
# /lib64/ld-linux-x86-64.so.2 (0x00007f6e44c2d000)
# libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f6e43018000)
# libffi.so.8 => /lib/x86_64-linux-gnu/libffi.so.8 (0x00007f6e43009000)
# libedit.so.2 => /lib/x86_64-linux-gnu/libedit.so.2 (0x00007f6e42fcf000)
# libtinfo.so.6 => /lib/x86_64-linux-gnu/libtinfo.so.6 (0x00007f6e42f9d000)
# libbsd.so.0 => /lib/x86_64-linux-gnu/libbsd.so.0 (0x00007f6e42f85000)
# libmd.so.0 => /lib/x86_64-linux-gnu/libmd.so.0 (0x00007f6e42f78000)

COPY --from=builder /lib/x86_64-linux-gnu/libmd.so.0 /lib/x86_64-linux-gnu/libmd.so.0
COPY --from=builder /lib/x86_64-linux-gnu/libbsd.so.0 /lib/x86_64-linux-gnu/libbsd.so.0
COPY --from=builder /lib/x86_64-linux-gnu/libedit.so.2 /lib/x86_64-linux-gnu/libedit.so.2
COPY --from=builder /lib/x86_64-linux-gnu/libmpfr.so.6 /lib/x86_64-linux-gnu/libmpfr.so.6
COPY --from=builder /lib/libLLVM-3.5.so /lib/libLLVM-3.5.so
COPY --from=builder /lib/libpure.so.8 /lib/libpure.so.8

COPY --from=builder /usr/local/lib/libmontre.so /usr/local/lib/libmontre.so
COPY --from=builder /usr/local/bin/montre /usr/local/bin/montre

RUN ldconfig

ENTRYPOINT ["montre"]
