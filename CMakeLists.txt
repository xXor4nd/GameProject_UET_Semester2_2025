cmake_minimum_required(VERSION 3.20)
project(SpaceshipWar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL_PATH "E:/IT/SDL")

add_executable(SpaceshipWar
    main.cpp
)

target_include_directories(SpaceshipWar PRIVATE
    "${SDL_PATH}/SDL2-devel-2.28.5-mingw/SDL2-2.28.5/x86_64-w64-mingw32/include/SDL2"
    "${SDL_PATH}/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/include/SDL2"
    "${SDL_PATH}/SDL2_mixer-2.8.0/x86_64-w64-mingw32/include/SDL2"
    "${SDL_PATH}/SDL2_ttf-2.22.0/x86_64-w64-mingw32/include/SDL2"
)

target_link_directories(SpaceshipWar PRIVATE
    "${SDL_PATH}/SDL2-devel-2.28.5-mingw/SDL2-2.28.5/x86_64-w64-mingw32/lib"
    "${SDL_PATH}/SDL2_image-devel-2.8.2-mingw/SDL2_image-2.8.2/x86_64-w64-mingw32/lib"
    "${SDL_PATH}/SDL2_mixer-2.8.0/x86_64-w64-mingw32/lib"
    "${SDL_PATH}/SDL2_ttf-2.22.0/x86_64-w64-mingw32/lib"
)

target_link_libraries(SpaceshipWar
    SDL2main
    SDL2
    SDL2_image
    SDL2_mixer
    SDL2_ttf
)

target_compile_definitions(SpaceshipWar PRIVATE SDL_MAIN_HANDLED)

#  Ẩn cửa sổ CMD khi chạy game
if(WIN32)
    set_target_properties(SpaceshipWar PROPERTIES LINK_FLAGS "-mwindows")
endif()

set(DLLS
    SDL2.dll
    SDL2_image.dll
    SDL2_mixer.dll
    SDL2_ttf.dll
)

add_custom_command(TARGET SpaceshipWar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:SpaceshipWar>/assets"
)
