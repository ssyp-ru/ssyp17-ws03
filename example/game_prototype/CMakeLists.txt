project(game_prototype)

set(EXECUTABLE  main.cpp 
                damageTrigger.cpp
                deathTrigger.cpp
                player.cpp
                weakPlatform.cpp
                movingPlatform.cpp
                hud.cpp
                )

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_custom_target(run_game_prototype
    COMMAND game_prototype
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_executable(game_prototype ${EXECUTABLE})
target_link_libraries(game_prototype real_engine)