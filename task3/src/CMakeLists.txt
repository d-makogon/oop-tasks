set(HEADERS_LIST
        "${task3_SOURCE_DIR}/src/game/board/include/board.h"
        "${task3_SOURCE_DIR}/src/game/board/include/boardShip.h"
        "${task3_SOURCE_DIR}/src/game/logic/include/gameLogic.h"
        "${task3_SOURCE_DIR}/src/game/ships/include/coordinate.h"
        "${task3_SOURCE_DIR}/src/game/ships/include/ship.h"
        "${task3_SOURCE_DIR}/src/game/ships/include/shipCreator.h"
        "${task3_SOURCE_DIR}/src/game/ships/include/shipTypes.h"
        "${task3_SOURCE_DIR}/src/game/ships/include/shotStatus.h"

        "${task3_SOURCE_DIR}/src/graphics/console/include/consoleView.h"
        "${task3_SOURCE_DIR}/src/graphics/include/gameView.h"

        "${task3_SOURCE_DIR}/src/playerControllers/include/consolePlayerController.h"
        "${task3_SOURCE_DIR}/src/playerControllers/include/optimalPlayerController.h"
        "${task3_SOURCE_DIR}/src/playerControllers/include/playerController.h"
        "${task3_SOURCE_DIR}/src/playerControllers/include/randomPlayerController.h"

        "${task3_SOURCE_DIR}/src/utility/include/console.h"
        "${task3_SOURCE_DIR}/src/utility/include/random.h"
        "${task3_SOURCE_DIR}/src/utility/include/factory.h"
        "${task3_SOURCE_DIR}/src/utility/include/utils.h"
        )

add_library(task3_library
        "${task3_SOURCE_DIR}/src/game/board/board.cpp"
        "${task3_SOURCE_DIR}/src/game/board/boardShip.cpp"
        "${task3_SOURCE_DIR}/src/game/logic/gameLogic.cpp"
        "${task3_SOURCE_DIR}/src/game/ships/coordinate.cpp"
        "${task3_SOURCE_DIR}/src/game/ships/ship.cpp"
        "${task3_SOURCE_DIR}/src/game/ships/shipCreator.cpp"

        "${task3_SOURCE_DIR}/src/graphics/console/consoleView.cpp"
        "${task3_SOURCE_DIR}/src/graphics/gameView.cpp"

        "${task3_SOURCE_DIR}/src/playerControllers/consolePlayerController.cpp"
        "${task3_SOURCE_DIR}/src/playerControllers/optimalPlayerController.cpp"
        "${task3_SOURCE_DIR}/src/playerControllers/randomPlayerController.cpp"

        "${task3_SOURCE_DIR}/src/utility/console.cpp"
        "${task3_SOURCE_DIR}/src/utility/random.cpp"
        )

target_include_directories(task3_library PUBLIC
        "${task3_SOURCE_DIR}/src/game/board/include"
        "${task3_SOURCE_DIR}/src/game/logic/include"
        "${task3_SOURCE_DIR}/src/game/ships/include"
        "${task3_SOURCE_DIR}/src/graphics/console/include"
        "${task3_SOURCE_DIR}/src/graphics/include"
        "${task3_SOURCE_DIR}/src/playerControllers/include"
        "${task3_SOURCE_DIR}/src/utility/include"
        )
