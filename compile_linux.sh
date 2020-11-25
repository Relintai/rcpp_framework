g++ -o3 -g main.cpp ./core/settings.cpp ./core/request.cpp ./core/http_server.cpp ./core/file_cache.cpp ./core/application.cpp ./rdn_application.cpp \
    ./themes/theme.cpp ./themes/theme_map.cpp \
    -o ./bin/server -Ilibs -lpthread -std=c++11
