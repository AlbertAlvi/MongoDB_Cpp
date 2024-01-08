#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

namespace mongo_handler
{
    std::string lookup_env(const std::string &name)
    {
        char *env = std::getenv(name.c_str());
        if (!env)
        {
            throw std::runtime_error("missing environment variable: " + name);
        }
        return env;
    }

    // char uriName[] = lookup_env("MONGO_CONNECTION_STRING");
    constexpr char uriName[] = "mongodb://admin:123@mongodb:27017";
    constexpr char databaseName[] = "enterprise";
    constexpr char collectionName[] = "employees";

    class EnterpriseHandler
    {
    private:
        mongocxx::uri uri;
        mongocxx::client client; // conn
        mongocxx::database db;

    public:
        EnterpriseHandler() : uri(mongocxx::uri{lookup_env("MONGO_CONNECTION_STRING")}),
                              client(mongocxx::client{uri}),
                              db(client[databaseName])
        {
        }
        // EnterpriseHandler() : uri(mongocxx::uri{uriName}),
        //                       client(mongocxx::client{uri}),
        //                       db(client[databaseName])
        // {
        // }

        void insertEmployee(const std::string &name, const int &age, const std::string& role, const std::string& department)
        {
            mongocxx::collection collection{db[collectionName]};
            auto builder = bsoncxx::builder::stream::document{};

            bsoncxx::v_noabi::document::value new_doc =
                builder << "name" << name
                        << "age" << age
                        << "role" << role
                        << "department" << department
                        << bsoncxx::builder::stream::finalize;

            collection.insert_one(new_doc.view());
        }

        void showEmployees() {
            mongocxx::collection collection{db[collectionName]};
            mongocxx::cursor cursor = collection.find({});

            for (auto &&document : cursor) {
                std::string json = bsoncxx::to_json(document);
                std::cout << json << "\n";
            }
        }
    };
}