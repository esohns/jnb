# configure C/C++ exceptions
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../Common/scripts")
include (SetCppExceptions)

include (Threads)

#include (ACE)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../ACEStream/src/modules/dec/scripts")
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
 set (LIB_FILE_SUFFIX "d")
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
include (zlib)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../yarp/scripts")
include (sdl)

if (UNIX)
 include (FindPkgConfig)
 if (SDL2_USE)
  pkg_check_modules (PKG_CONFIG_SDL2_IMAGE SDL2_image REQUIRED)
  if (PKG_CONFIG_SDL2_IMAGE_FOUND)
   set (SDL2_IMAGE_INCLUDE_DIRS ${PKG_CONFIG_SDL2_IMAGE_INCLUDE_DIRS})
   set (SDL2_IMAGE_LIBRARIES ${PKG_CONFIG_SDL2_IMAGE_LIBRARIES})
  endif (PKG_CONFIG_SDL2_IMAGE_FOUND)
 endif (SDL2_USE)

 add_link_options ("LINKER:--as-needed")
elseif (WIN32)
 if (SDL2_USE)
  find_path (SDL2_IMAGE_INCLUDE_DIRS NAMES SDL_image.h
             PATHS "$ENV{LIB_ROOT}/SDL2_image"
             PATH_SUFFIXES include)

  set (SDL2_IMAGE_LIB "SDL2_image.lib")
  find_library (SDL2_IMAGE_LIBRARY
                NAMES ${SDL2_IMAGE_LIB}
                PATHS "$ENV{LIB_ROOT}/SDL2_image/lib"
                PATH_SUFFIXES ${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}
                NO_DEFAULT_PATH)
 endif (SDL2_USE)
endif ()

add_executable (${PROJECT_NAME})

#target_include_directories (${PROJECT_NAME} PUBLIC
#                            $<BUILD_INTERFACE:${ACE_INCLUDE_DIR}>)
if (ZLIB_SUPPORT)
 target_include_directories (${PROJECT_NAME} PUBLIC
                             "$<BUILD_INTERFACE:${ZLIB_INCLUDE_DIRS}>")
endif (ZLIB_SUPPORT)
if (SDL_SUPPORT)
 target_include_directories (${PROJECT_NAME} PUBLIC
                             "$<BUILD_INTERFACE:${SDL_INCLUDE_DIRS}>")
 if (SDL1_USE)
  if (WIN32)
   target_include_directories (${PROJECT_NAME} PUBLIC
                               $<BUILD_INTERFACE:${LIB_ROOT}/SDL_net/include>)
  endif (WIN32)
 elseif (SDL2_USE)
  target_include_directories (${PROJECT_NAME} PUBLIC
                              $<BUILD_INTERFACE:${SDL2_NET_INCLUDE_DIRS}>)
 endif ()
endif (SDL_SUPPORT)

target_include_directories (${PROJECT_NAME} PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../data/assets>)
target_include_directories (${PROJECT_NAME} PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories (${PROJECT_NAME} PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdl>)
target_include_directories (${PROJECT_NAME} PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>)
if (WIN32)
 target_include_directories (${PROJECT_NAME} PUBLIC
                             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>)
endif (WIN32)

add_definitions (-D_CRT_SECURE_NO_WARNINGS)
add_definitions (-DUSE_SDL)
add_definitions (-DUSE_SDL_MIXER)
add_definitions (-DUSE_NET)

############## next target ################
set (source_files
     config.h
     dat.c
     dat.h
     filter.c
     filter.h
     fireworks.c
     gfx.h
     globals.h
     input.h
     interrpt.h
     main.cpp
     menu.c
     menu.h
     net.c
     net.h
     sfx.h
     sdl/gfx.c
     sdl/input.c
     sdl/interrpt.c
     sdl/sfx.c)
set (source_files
     ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_config.h
     ${source_files})
if (WIN32)
 set (source_files
      ../include/targetver.h
      ${source_files})
endif (WIN32)

# Include dir for installed targets
#set (INCLUDE_DIR include/${PROJECT_NAME}-${upstream_version})

foreach (source_file ${source_files})
 get_filename_component (abs_source_file ${source_file} ABSOLUTE)
 list (APPEND absolute_source_files
       PRIVATE ${abs_source_file}
       )
#       $<INSTALL_INTERFACE:${INCLUDE_DIR}/${source_file}>)
endforeach ()
target_sources (${PROJECT_NAME}
#                PRIVATE stdafx.cpp
                PRIVATE stdafx.h
                PRIVATE ${absolute_source_files})
target_precompile_headers (${PROJECT_NAME}
                           PRIVATE stdafx.h)

#add_dependencies (${PROJECT_NAME} ACE)

#target_link_libraries (${PROJECT_NAME} PUBLIC ${ACE_LIBRARY})
if (ZLIB_SUPPORT)
 target_link_libraries (${PROJECT_NAME} PUBLIC ${ZLIB_LIBRARIES})
endif (ZLIB_SUPPORT)
if (SDL_SUPPORT)
 if (SDL1_USE)
  if (WIN32)
   target_link_libraries (${PROJECT_NAME} PUBLIC legacy_stdio_definitions.lib)
  endif (WIN32)
  target_link_libraries (${PROJECT_NAME} PUBLIC ${SDL1_MAIN_LIBRARY})
 endif (SDL1_USE)
 target_link_libraries (${PROJECT_NAME} PUBLIC ${SDL_LIBRARIES})
 if (SDL1_USE)
  if (WIN32)
   target_link_libraries (${PROJECT_NAME} PUBLIC ${LIB_ROOT}/SDL_net/lib/${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}/SDL_net.lib)
  endif (WIN32)
 elseif (SDL2_USE)
  if (UNIX)
   target_link_libraries (${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_LIBRARIES})
  elseif (WIN32)
   target_link_libraries (${PROJECT_NAME} PUBLIC ${SDL2_IMAGE_LIBRARY})
  endif ()
 endif ()
endif (SDL_SUPPORT)

##########################################

# add all targets to the build-tree export set
export (TARGETS ${PROJECT_NAME}
        FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

############### install files ############
install (FILES
         config.h
         dat.h
         filter.h
         gfx.h
         globals.h
         input.h
         interrpt.h
         menu.h
         net.h
         sfx.h
         DESTINATION include
         COMPONENT include)
install (FILES
         ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_config.h
         DESTINATION include
         COMPONENT include)

install (TARGETS ${PROJECT_NAME}
         EXPORT ${PROJECT_NAME}Targets
         RUNTIME DESTINATION bin COMPONENT bin)

if (WIN32)
 install (FILES
          ${LIB_ROOT}\\\\zlib\\\\build\\\\${CMAKE_BUILD_TYPE}\\\\zlib${LIB_FILE_SUFFIX}.dll
          DESTINATION bin COMPONENT bin)
 if (SDL1_USE)
  install (FILES
           ${LIB_ROOT}\\\\SDL\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL.dll
           ${LIB_ROOT}\\\\SDL_mixer\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL_mixer.dll
           ${LIB_ROOT}\\\\SDL_mixer\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\libmikmod-2.dll
#           ${LIB_ROOT}\\\\SDL_ttf\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL_ttf.dll
           ${LIB_ROOT}\\\\SDL_net\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL_net.dll
           DESTINATION bin
           COMPONENT bin)
 elseif (SDL2_USE)
  install (FILES
           ${LIB_ROOT}\\\\SDL2\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL2.dll
           ${LIB_ROOT}\\\\SDL2_mixer\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL2_mixer.dll
 #          ${LIB_ROOT}\\\\SDL2_ttf\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL2_ttf.dll
           ${LIB_ROOT}\\\\SDL2_net\\\\lib\\\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}\\\\SDL2_net.dll
           DESTINATION bin
           COMPONENT bin)
 endif ()
endif (WIN32)

install (FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/../data/jnb.dat
         DESTINATION data
         COMPONENT data)

################ launchers ###############

set (ARGS "")
if (UNIX)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}" COMMAND)
 create_target_launcher (${PROJECT_NAME}
                         COMMAND ${COMMAND}
                         ARGS ${ARGS}
#                         FORWARD_ARGS
                         RUNTIME_LIBRARY_DIRS ${ACE_LIB_PATH}
                         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                         ENVIRONMENT ${ENVIRONMENT})
elseif (WIN32)
 file (TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}.exe" COMMAND)
 string (REPLACE "\\" "\\\\" COMMAND ${COMMAND})
# file (TO_CMAKE_PATH "${ACE_LIB_DIR}" ACE_LIB_PATH)
 file (TO_CMAKE_PATH "${ZLIB_LIB_DIR}" ZLIB_LIB_PATH)
 file (TO_CMAKE_PATH "${SDL_LIB_DIR}" SDL_LIB_PATH)
 if (SDL1_USE)
  file (TO_CMAKE_PATH "${LIB_ROOT}\\SDL_net\\lib\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}" SDL_NET_LIB_PATH)
 elseif (SDL2_USE)
  file (TO_CMAKE_PATH "${LIB_ROOT}\\SDL2_net\\lib\\${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE}" SDL_NET_LIB_PATH)
 endif ()
 create_target_launcher (${PROJECT_NAME}
                         COMMAND ${COMMAND}
                         ARGS ${ARGS}
#                         FORWARD_ARGS
                         RUNTIME_LIBRARY_DIRS ${ZLIB_LIB_PATH}
                                              ${SDL_LIB_PATH} ${SDL_NET_LIB_PATH}
                         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                         ENVIRONMENT ${ENVIRONMENT})
endif ()

################ tests ###################
add_test (NAME ${PROJECT_NAME}_test
          COMMAND ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
          WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
set_tests_properties (${PROJECT_NAME}_test
                      PROPERTIES
                      ENVIRONMENT "PATH=D:\\projects\\ATCD\\ACE\\lib;D:\\projects\\gtk\\bin;D:\\projects\\libglade\\bin;D:\\projects\\libxml2-2.9.1\\.libs;G:\\software\\Development\\dll;${PROJECT_SOURCE_DIR}\\..\\libCommon\\cmake\\src\\Debug;${PROJECT_SOURCE_DIR}\\..\\libCommon\\cmake\\src\\ui\\Debug;${PROJECT_SOURCE_DIR}\\cmake\\src\\Debug;${PROJECT_SOURCE_DIR}\\cmake\\src\\modules\\dev\\Debug;${PROJECT_SOURCE_DIR}\\..\\libACENetwork\\cmake\\src\\Debug;%PATH%")
#PATH=%PATH%;$(ProjectDir)..\..\..\..\ATCD\ACE\lib;$(ProjectDir)..\..\..\..\gtk3\bin;$(ProjectDir)..\..\..\..\libav-11.7\libavcodec;$(ProjectDir)..\..\..\..\libav-11.7\libavresample;$(ProjectDir)..\..\..\..\libav-11.7\libavutil;$(ProjectDir)..\..\..\..\libCommon\cmake\src\Debug;$(ProjectDir)..\..\..\..\libCommon\cmake\src\err\Debug;$(ProjectDir)..\..\..\..\libCommon\cmake\src\ui\Debug;$(ProjectDir)..\..\src\Debug;$(ProjectDir)..\..\src\modules\dev\Debug;$(ProjectDir)..\..\src\modules\vis\Debug
