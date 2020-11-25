g++ -o3 -g main.cpp ./core/settings.cpp ./core/request.cpp ./core/http_server.cpp ./core/file_cache.cpp ./core/application.cpp ./rdn_application.cpp \
    ./core/theme.cpp \
    -o ./bin/server -Ilibs -lpthread -std=c++11



