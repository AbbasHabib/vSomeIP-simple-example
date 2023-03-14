FROM vsomeip_build:v0 as builder

# build example
COPY ./example/ /example_service/

# build service cpp file
RUN cd /example_service/; \
    mkdir build;\
    cd build; \
    cmake ..;\
    make;

RUN mkdir /etc/vsomeip/

RUN mv /example_service/config-service.json /etc/vsomeip/config-service.json

ENV VSOMEIP_CONFIGURATION=/etc/vsomeip/config-service.json

CMD [ "/example_service/build/service-basic" ]
