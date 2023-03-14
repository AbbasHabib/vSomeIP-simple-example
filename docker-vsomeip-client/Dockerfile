FROM vsomeip_build:v0 as builder

# build example
COPY ./example/ /example_client/

# build service cpp file
RUN cd /example_client/; \
    mkdir build;\
    cd build; \
    cmake ..;\
    make;

RUN mkdir /etc/vsomeip/

RUN mv /example_client/config-client.json /etc/vsomeip/config-client.json

ENV VSOMEIP_CONFIGURATION=/etc/vsomeip/config-client.json

CMD [ "/example_client/build/client-basic" ]
