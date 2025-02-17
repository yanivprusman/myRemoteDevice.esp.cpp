idf_component_register(SRCS "myRemoteDevice.cpp" "main.cpp"
    INCLUDE_DIRS "."
    "/home/yaniv/101_coding/flatbuffers/include"
)
set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 17)