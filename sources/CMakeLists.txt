set(EXECUTABLE_NAME "RDB_Didplomaterv_Monitor_${PROJECT_VERSION}")
set(SOURCE_FILES "main.cpp"
                 "data_point.cpp"
                 "data_line.cpp"
)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_compile_options(${EXECUTABLE_NAME} PUBLIC ${COMPILER_FLAGS})