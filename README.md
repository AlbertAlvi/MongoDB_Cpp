# Objective
Implementation of a web app using a document-oriented database for presentation during the Database II course seminar.

## Description
Web App designed to store employee data using MongoDB.

## Third part dependencies
This app uses [_httplib_](https://github.com/yhirose/cpp-httplib) from [@yhirose](https://github.com/yhirose) and [_nlohmann json_](https://github.com/nlohmann/json) from [@nlohmann](https://github.com/nlohmann).

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

After that, try to insert and delete employees from the database.
