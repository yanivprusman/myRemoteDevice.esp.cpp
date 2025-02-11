idf_component_register(SRCS "myRemoteDevice.cpp" "main.cpp"
    INCLUDE_DIRS "."
)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)