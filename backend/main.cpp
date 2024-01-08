#include <cstdlib>
#include <string>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

#include "include/mongodb_handler.hpp"

int main()
{
  try
  {
    mongocxx::instance inst{};
    mongo_handler::EnterpriseHandler handler{};
    std::cout << "env: " << mongo_handler::lookup_env("MONGO_CONNECTION_STRING") << "\n"
              << "uriName: " << mongo_handler::uriName << "\n";

    while(true) {
      std::string nome, role, departamento;
      int idade;

      std::cout << "Insira um nome:" << "\n";
      std::cin >> nome;
      std::cout << "Insira uma idade:" << "\n";
      std::cin >> idade;
      std::cout << "Insira um cargo:" << "\n";
      std::cin >> role;
      std::cout << "Insira um departamento:" << "\n";
      std::cin >> departamento;

      handler.insertEmployee(nome, idade, role, departamento);

      char op;
      std::cout << "Continuar? y / n" << "\n";
      std::cin >> op;

      if(op == 'n') break;
    }

    handler.showEmployees();
  }
  catch (const std::exception &e)
  {
    // Handle errors
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return (EXIT_SUCCESS);
}