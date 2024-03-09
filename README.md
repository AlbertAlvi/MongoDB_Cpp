# Objective
Implementation of a system using a document-oriented database for presentation during the Database II course seminar.

## Description
Simple app used to store employee data using MongoDB.

## Thrid part dependencies
This app uses _httplib_ from @yhirose and _nlohmann json_ from @nlohmann:
- https://github.com/yhirose/cpp-httplib
- https://github.com/nlohmann/json

## Install
```bash
git clone https://github.com/AlbertAlvin8080/MongoDB_Cpp.git
```

```bash
cd MongoDB_Cpp
```

## Running the app
Run this docker command in the folder where the docker-compose file is located:
```bash
docker-compose up --build
```

Copy and paste this address into your browser:
```bash
http://localhost:8080/
```

After that, try to insert and delete employees from the database
