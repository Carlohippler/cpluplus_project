cmake_minimum_required(VERSION 3.10)
project(MediaEscolar)

# Configurações de C++ e Windows
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WIN32_EXECUTABLE ON)

# Encontrar wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# Criar o Executável (Incluindo os arquivos do SQLite)
add_executable(MediaEscolar 
    WXf1.cpp 
    sqlite3.c 
    sqlite3.h
)

# Linkar Bibliotecas
target_link_libraries(MediaEscolar ${wxWidgets_LIBRARIES})

# Copiar a pasta Config para onde o .exe estiver (Importante para a logo!)
add_custom_command(TARGET MediaEscolar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Config"
        "$<TARGET_FILE_DIR:MediaEscolar>/Config")

        # Configurações para o Instalador
include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "MediaEscolar")
set(CPACK_PACKAGE_VENDOR "MeuProjeto")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_DISPLAY_NAME "Media Escolar")
set(CPACK_NSIS_PACKAGE_NAME "Media Escolar")

install(TARGETS MediaEscolar DESTINATION bin)
set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\MediaEscolar.lnk' '$INSTDIR\\\\bin\\\\MediaEscolar.exe'")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Config" DESTINATION bin)
set(CPACK_NSIS_MODIFY_PATH ON)
set(CPACK_NSIS_MUI_FINISHPAGE_RUN "bin\\\\MediaEscolar.exe")
if(WIN32)
    # Pega o caminho das DLLs baseado na variável wxWidgets_ROOT que você configurou
    file(GLOB WX_DLLS "${wxWidgets_ROOT}/lib/vc_x64_dll/*.dll")
    install(FILES ${WX_DLLS} DESTINATION bin)
    include(InstallRequiredSystemLibraries)
endif()



include(CPack)