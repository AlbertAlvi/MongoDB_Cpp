#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/uri.hpp>

#include <nlohmann/json.hpp>

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
        mongocxx::client client; // connection
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

        bool insertEmployee(const std::string &name, const int &age, const std::string &role, const std::string &department)
        {
            try
            {
                mongocxx::collection collection{db[collectionName]};
                // bsoncxx::types::b_oid bson_id{bsoncxx::oid{_id}};

                auto builder = bsoncxx::builder::stream::document{};
                bsoncxx::document::value new_doc =
                    builder //<< "_id" << bson_id
                    << "name" << name
                    << "age" << age
                    << "role" << role
                    << "department" << department
                    << bsoncxx::builder::stream::finalize;

                bsoncxx::stdx::optional<mongocxx::result::insert_one> result =
                    collection.insert_one(new_doc.view());
                if (result)
                    return result->inserted_id().get_oid().value.to_string().size() != 0;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error inserting employee: " << e.what() << std::endl;
            }

            // happens for both !if and catch
            return false;
        }

        bool updateDepartment(const std::string &_id, const std::string &new_department)
        {
            try
            {
                mongocxx::collection collection{db[collectionName]};

                auto filter = bsoncxx::builder::stream::document{}
                              << "_id" << bsoncxx::oid{_id}
                              << bsoncxx::builder::stream::finalize;

                auto dep_update = bsoncxx::builder::stream::document{}
                                  << "$set" << bsoncxx::builder::stream::open_document
                                  << "department" << new_department
                                  << bsoncxx::builder::stream::close_document
                                  << bsoncxx::builder::stream::finalize;

                bsoncxx::stdx::optional<mongocxx::result::update> result =
                    collection.update_one(filter.view(), dep_update.view());

                return result->modified_count();
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error removing employee: " << e.what() << std::endl;
            }

            return false;
        }

        bool removeEmployee(const std::string &_id)
        {
            try
            {
                mongocxx::collection collection{db[collectionName]};

                auto builder = bsoncxx::builder::stream::document{};
                bsoncxx::document::value filter =
                    builder
                    << "_id" << bsoncxx::oid{_id}
                    << bsoncxx::builder::stream::finalize;

                bsoncxx::stdx::optional<mongocxx::result::delete_result> result =
                    collection.delete_one(filter.view());

                return result->deleted_count();
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error removing employee: " << e.what() << std::endl;
            }

            return false;
        }

        void showEmployees()
        {
            mongocxx::collection collection{db[collectionName]};
            mongocxx::cursor cursor = collection.find({});

            for (auto &&document : cursor)
            {
                std::string json = bsoncxx::to_json(document);
                std::cout << json << "\n";
            }
        }

        nlohmann::json retrieveAllFields()
        {
            try
            {
                mongocxx::collection collection{db[collectionName]};
                mongocxx::cursor cursor = collection.find({});

                nlohmann::json fieldsArray = nlohmann::json::array();

                for (auto &&document : cursor)
                {
                    std::string json = bsoncxx::to_json(document);
                    nlohmann::json documentJson = nlohmann::json::parse(json);
                    fieldsArray.push_back(documentJson);
                }

                return fieldsArray;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error retrieving all fields: " << e.what() << std::endl;
            }

            return nlohmann::json::array();
        }
    };
}