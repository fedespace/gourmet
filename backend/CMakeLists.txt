cmake_minimum_required(VERSION 3.10)

# Project name
project(gourmet_backend)

# Drogon requires C++17
set(CMAKE_CXX_STANDARD 17)

# Specify where to find PostgreSQL
set(PostgreSQL_INCLUDE_DIR /opt/homebrew/include/postgresql@18)
set(PostgreSQL_LIBRARY /opt/homebrew/lib/postgresql@18/libpq.dylib)
link_directories(/opt/homebrew/lib/postgresql@18)

# Check if Drogon is installed in the system, otherwise it fails
find_package(Drogon CONFIG REQUIRED)

add_executable(gourmet_backend
    main.cc
    controllers/RecipeController.cc
)

# Conecting Drogon to this executable specified the step before
target_link_libraries(gourmet_backend PRIVATE Drogon::Drogon pq)
target_link_libraries(gourmet_backend PRIVATE /opt/homebrew/lib/postgresql@18/libpq.dylib)