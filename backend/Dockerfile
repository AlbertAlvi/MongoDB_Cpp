FROM mongodb/mongo-cxx-driver:3.9.0-redhat-ubi-9.3

RUN microdnf upgrade -y
RUN microdnf install -y g++
# RUN microdnf install -y curl # conflicts with curl-minimal installed
RUN microdnf install -y wget
RUN microdnf install -y nano
RUN microdnf install -y ncurses
RUN microdnf install -y net-tools

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