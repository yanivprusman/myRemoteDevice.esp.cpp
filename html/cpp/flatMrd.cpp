// main.cpp
// home/yaniv/101_coding/myRemoteDeviceEspCpp/html/cpp/main.cpp
#include <iostream>
#include "./../../flatBuffers/mrd/mrd_generated.h"
using namespace theMRDNameSpace;
int main() {
    flatbuffers::FlatBufferBuilder builder(1024);
    auto mrd = CreateMRD( builder, types_server, actions_getPage,builder.CreateString(""));
    builder.Finish(mrd);
    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();
    auto verifier = flatbuffers::Verifier(buf, size);
    bool ok = VerifyMRDBuffer(verifier);
    if (!ok) {
        std::cerr << "Invalid buffer" << std::endl;
        return 1;
    }
    auto mrd_read = GetMRD(buf);
    std::cout << "type: " << EnumNametypes(mrd_read->type()) << std::endl;
    std::cout << "action: " << EnumNameactions(mrd_read->action()) << std::endl;
    std::cout << "data: " << mrd_read->data()->c_str() << std::endl;
    return 0;
}