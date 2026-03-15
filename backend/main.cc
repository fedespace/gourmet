#include <drogon/drogon.h>

int main() {
    std::cout << "Avvio server..." << std::endl;

    // Load config from yaml file and start the server
    drogon::app()
        .loadConfigFile("config.yaml")
        .run();

    return 0;
};