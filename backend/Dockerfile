FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3

# it’s important to note that each RUN command in a Dockerfile creates a new layer in the Docker image. If you install packages and create cache files in one layer, and then delete those cache files in a later layer, the overall size of the image will still include the cache, because it’s stored in the earlier layer.

RUN microdnf upgrade -y && microdnf clean all
RUN microdnf install -y g++ && microdnf clean all
# RUN microdnf install -y curl # conflicts with curl-minimal installed
RUN microdnf install -y wget && microdnf clean all
# RUN microdnf install -y nano && microdnf clean all
# RUN microdnf install -y ncurses && microdnf clean all
# RUN microdnf install -y net-tools && microdnf clean all

WORKDIR /usr/src/app

COPY . .

RUN wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h -O include/httplib.h
RUN wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp -O include/nlohmann/json.hpp

EXPOSE 8080

RUN g++ \
    -o main \
    main.cpp \
    -I/usr/local/include/bsoncxx/v_noabi/ \
    -I/usr/local/include/mongocxx/v_noabi/ \
    -I./include \
    -lmongocxx \
    -lbsoncxx

CMD ["./main"]