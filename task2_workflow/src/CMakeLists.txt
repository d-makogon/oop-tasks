set(HEADERS_LIST "${task2_SOURCE_DIR}/include/argsParser.h"
        "${task2_SOURCE_DIR}/include/worker.h"
        "${task2_SOURCE_DIR}/include/workers.h"
        "${task2_SOURCE_DIR}/include/workersFactory.h"
        "${task2_SOURCE_DIR}/include/workflowExecutor.h"
        "${task2_SOURCE_DIR}/include/workflowParser.h"
        )

add_library(task2_library argsParser.cpp worker.cpp workers.cpp workflowExecutor.cpp workflowParser.cpp
        ${HEADERS_LIST})

target_include_directories(task2_library PUBLIC ../include)

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)
