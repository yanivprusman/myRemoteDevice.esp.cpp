// main.cpp
// #include <iostream>
// #include "./../../flatBuffers/monster/monster_generated.h"
// using namespace MyGame;
// int main() {
//     flatbuffers::FlatBufferBuilder builder(1024);
//     auto name = builder.CreateString("Orc");
//     auto monster = CreateMonster(builder, name, 80, 50);
//     builder.Finish(monster);
//     uint8_t *buf = builder.GetBufferPointer();
//     int size = builder.GetSize();
//     auto verifier = flatbuffers::Verifier(buf, size);
//     bool ok = VerifyMonsterBuffer(verifier);
//     if (!ok) {
//         std::cerr << "Invalid buffer" << std::endl;
//         return 1;
//     }
//     auto monster_read = GetMonster(buf);
//     std::cout << "Monster name: " << monster_read->name()->c_str() << std::endl;
//     std::cout << "HP: " << monster_read->hp() << std::endl;
//     std::cout << "Mana: " << monster_read->mana() << std::endl;
//     return 0;
// }
#include <iostream>
#include "./../../flatBuffers/mrd/mrd_generated.h"
using namespace theMRDNameSpace;
int main() {
    flatbuffers::FlatBufferBuilder builder(1024);
    auto name = builder.CreateString("Orc");
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
    // std::cout << "type: " << EnumNamestypes()[mrd_read->type()]<< std::endl;
    // std::cout << "action: " << EnumNameactions()[mrd_read->action()] << std::endl;
    std::cout << "type: " << EnumNametypes(mrd_read->type()) << std::endl;
    std::cout << "action: " << EnumNameactions(mrd_read->action()) << std::endl;
    std::cout << "data: " << mrd_read->data()->c_str() << std::endl;
    return 0;
}