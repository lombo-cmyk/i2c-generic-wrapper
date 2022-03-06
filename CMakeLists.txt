# For IDF v4.x:
#idf_component_register(SRCS "x.c" INCLUDE_DIRS include)

set(CMAKE_CXX_STANDARD 17)

idf_component_register(SRCS "src/I2CWrapper.cpp"
        INCLUDE_DIRS "include"
        REQUIRES esp32-smbus CppSingletonTemplate)