cmake_minimum_required(VERSION 3.12.4)

project(esp32_dashboard_sim C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ESP32_APP_DIR "${CMAKE_CURRENT_LIST_DIR}/../main")
set(LV_PORT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../lv_port_pc_vscode")
set(LVGL_DIR "${LV_PORT_DIR}/lvgl")
set(LVGL_LIB_DIR "${LV_PORT_DIR}/build/lvgl/lib")

find_package(SDL2 REQUIRED)

add_executable(esp32_ui_sim
    sim_main.c
    esp_random.c
    "${ESP32_APP_DIR}/core/app_state.c"
    "${ESP32_APP_DIR}/assets/fonts/app_cjk_font.c"
    "${ESP32_APP_DIR}/ui/ui_style.c"
    "${ESP32_APP_DIR}/ui/ui.c"
    "${ESP32_APP_DIR}/ui/page_home.c"
    "${ESP32_APP_DIR}/ui/page_notes.c"
    "${ESP32_APP_DIR}/ui/page_calendar.c"
    "${ESP32_APP_DIR}/ui/page_weather_env.c"
    "${ESP32_APP_DIR}/ui/page_widgets.c"
    "${ESP32_APP_DIR}/services/weather_service.c"
    "${ESP32_APP_DIR}/services/sensor_service.c"
    "${ESP32_APP_DIR}/services/sync_service.c"
    "${ESP32_APP_DIR}/services/holiday_service.c"
)

target_compile_definitions(esp32_ui_sim PRIVATE LV_CONF_INCLUDE_SIMPLE)
target_include_directories(esp32_ui_sim PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}"
    "${ESP32_APP_DIR}"
    "${ESP32_APP_DIR}/assets/fonts"
    "${ESP32_APP_DIR}/core"
    "${ESP32_APP_DIR}/ui"
    "${ESP32_APP_DIR}/services"
    "${LV_PORT_DIR}"
    "${LVGL_DIR}"
    ${SDL2_INCLUDE_DIRS}
)

set_target_properties(esp32_ui_sim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
    LINKER_LANGUAGE CXX
)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(esp32_ui_sim PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

if(MSVC)
    target_link_libraries(esp32_ui_sim
        "${LVGL_LIB_DIR}/liblvgl.a"
        "${LVGL_LIB_DIR}/liblvgl_thorvg.a"
        ${SDL2_LIBRARIES}
    )
    target_link_options(esp32_ui_sim PRIVATE "/SUBSYSTEM:CONSOLE")
else()
    target_link_libraries(esp32_ui_sim
        "${LVGL_LIB_DIR}/liblvgl.a"
        "${LVGL_LIB_DIR}/liblvgl_thorvg.a"
        ${SDL2_LIBRARIES}
        m
        pthread
    )
    if(WIN32)
        target_link_options(esp32_ui_sim PRIVATE "-mconsole")
    endif()
endif()

if(WIN32 AND EXISTS "C:/SDL2-2.32.4/x86_64-w64-mingw32/bin/SDL2.dll")
    add_custom_command(TARGET esp32_ui_sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/SDL2-2.32.4/x86_64-w64-mingw32/bin/SDL2.dll"
            "$<TARGET_FILE_DIR:esp32_ui_sim>"
    )
endif()

add_custom_target(run
    COMMAND esp32_ui_sim
    DEPENDS esp32_ui_sim
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
)
