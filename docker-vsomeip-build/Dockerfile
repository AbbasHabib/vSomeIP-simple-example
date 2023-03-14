FROM boost_build:v0 as build

LABEL description="Build vsomeip"

ENV VSOMEIP_VERSION 3.1.20.3

RUN set -eux; 
# download the vsomeop version
RUN curl -o vsomeip.tar.gz -fsSL "https://github.com/GENIVI/vsomeip/archive/${VSOMEIP_VERSION}.tar.gz"; 
RUN tar -xf vsomeip.tar.gz -C /tmp/; 
# cleanup download
RUN rm vsomeip.tar.gz; 
# build
RUN mv /tmp/vsomeip-${VSOMEIP_VERSION} /tmp/vsomeip;
RUN cd /tmp/vsomeip; \ 
    mkdir build && cd build; \
    cmake ..;   \
    make; \
    make install;

# load libraries into linker
RUN ldconfig
