#include <cstdlib>
#include <string>
#include <fstream>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "mongodb_handler.hpp"

void handleDefaultRoute(const httplib::Request &req, httplib::Response &res)
{
  std::ifstream t("frontend-code/index.html");
  std::string html((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
  res.set_content(html, "text/html");
}

int main()
{
  const int tries = 10;
  for(size_t i = 0; i < tries; ++i) 
  {
    try
    {
      mongocxx::instance inst{};
      mongo_handler::EnterpriseHandler mongoHandler{};
      // std::cout << "env: " << mongo_handler::lookup_env("MONGO_CONNECTION_STRING") << "\n"
      //           << "uriName: " << mongo_handler::uriName << "\n";

      // Create HTTP server
      httplib::Server server;

      // Enable CORS for all routes
      server.set_mount_point("/", "./");

      // Serve HTML, CSS, and JS files on the root endpoint
      server.Get("/", handleDefaultRoute);
      server.Get("/index.html", handleDefaultRoute);

      server.Get("/data", [&mongoHandler](const httplib::Request &req, httplib::Response &res)
                {
          auto fields = mongoHandler.retrieveAllFields();
          res.set_content(fields.dump(), "application/json"); });

      server.Get("/styles.css", [](const httplib::Request &req, httplib::Response &res)
                {
          std::ifstream t("frontend-code/styles.css");
          std::string css((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
          res.set_content(css, "text/css"); });

      server.Get("/script.js", [](const httplib::Request &req, httplib::Response &res)
                {
          std::ifstream t("frontend-code/script.js");
          std::string js((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
          res.set_content(js, "application/javascript"); });

      // Define a route to handle form submissions
      server.Post("/submit", [&mongoHandler](const httplib::Request &req, httplib::Response &res)
                  {
          res.set_header("Access-Control-Allow-Origin", "*");
          res.set_header("Access-Control-Allow-Methods", "POST");

          // Extract data from the request
          auto json = nlohmann::json::parse(req.body);
          std::string name = json["name"];
          int age = json["age"];
          std::string role = json["role"];
          std::string department = json["department"];
          
          // std::cout << name << "\n";
          // std::cout << age << "\n";
          // std::cout << role << "\n";
          // std::cout << department << "\n";

          // Insert data into MongoDB
          bool success = mongoHandler.insertEmployee(name, age, role, department);

          // Send response
          if (success) {
              res.set_content("Form submitted successfully", "text/plain");
          } else {
              res.set_content("Error submitting form", "text/plain");
          } });

      server.Post("/remove", [&mongoHandler](const httplib::Request &req, httplib::Response &res)
                  {
          res.set_header("Access-Control-Allow-Origin", "*");
          res.set_header("Access-Control-Allow-Methods", "POST");

          auto json = nlohmann::json::parse(req.body);
          std::string _id = json["_id"];
        
          bool success = mongoHandler.removeEmployee(_id);

          if (success) {
              res.set_content("Form submitted successfully", "text/plain");
          } else {
              res.set_content("Error submitting form", "text/plain");
          } });

      // Start the server on port 8080
      std::cout << "Listening on port 8080..."
                << "\n";
      server.listen("0.0.0.0", 8080);
    }
    catch (const std::exception &e)
    {
      std::cerr << "Exception: " << e.what() << std::endl;
    }
  }

  return (EXIT_SUCCESS);
}