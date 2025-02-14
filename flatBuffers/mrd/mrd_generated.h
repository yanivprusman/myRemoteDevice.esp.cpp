// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MRD_THEMRDNAMESPACE_H_
#define FLATBUFFERS_GENERATED_MRD_THEMRDNAMESPACE_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 25 &&
              FLATBUFFERS_VERSION_MINOR == 2 &&
              FLATBUFFERS_VERSION_REVISION == 10,
             "Non-compatible flatbuffers version included");

namespace theMRDNameSpace {

struct MRD;
struct MRDBuilder;

enum types : int8_t {
  types_server = 0,
  types_client = 1,
  types_MIN = types_server,
  types_MAX = types_client
};

inline const types (&EnumValuestypes())[2] {
  static const types values[] = {
    types_server,
    types_client
  };
  return values;
}

inline const char * const *EnumNamestypes() {
  static const char * const names[3] = {
    "server",
    "client",
    nullptr
  };
  return names;
}

inline const char *EnumNametypes(types e) {
  if (::flatbuffers::IsOutRange(e, types_server, types_client)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamestypes()[index];
}

enum actions : int8_t {
  actions_getPage = 0,
  actions_MIN = actions_getPage,
  actions_MAX = actions_getPage
};

inline const actions (&EnumValuesactions())[1] {
  static const actions values[] = {
    actions_getPage
  };
  return values;
}

inline const char * const *EnumNamesactions() {
  static const char * const names[2] = {
    "getPage",
    nullptr
  };
  return names;
}

inline const char *EnumNameactions(actions e) {
  if (::flatbuffers::IsOutRange(e, actions_getPage, actions_getPage)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesactions()[index];
}

struct MRD FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MRDBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE = 4,
    VT_ACTION = 6,
    VT_DATA = 8
  };
  theMRDNameSpace::types type() const {
    return static_cast<theMRDNameSpace::types>(GetField<int8_t>(VT_TYPE, 0));
  }
  theMRDNameSpace::actions action() const {
    return static_cast<theMRDNameSpace::actions>(GetField<int8_t>(VT_ACTION, 0));
  }
  const ::flatbuffers::String *data() const {
    return GetPointer<const ::flatbuffers::String *>(VT_DATA);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_TYPE, 1) &&
           VerifyField<int8_t>(verifier, VT_ACTION, 1) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyString(data()) &&
           verifier.EndTable();
  }
};

struct MRDBuilder {
  typedef MRD Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_type(theMRDNameSpace::types type) {
    fbb_.AddElement<int8_t>(MRD::VT_TYPE, static_cast<int8_t>(type), 0);
  }
  void add_action(theMRDNameSpace::actions action) {
    fbb_.AddElement<int8_t>(MRD::VT_ACTION, static_cast<int8_t>(action), 0);
  }
  void add_data(::flatbuffers::Offset<::flatbuffers::String> data) {
    fbb_.AddOffset(MRD::VT_DATA, data);
  }
  explicit MRDBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<MRD> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<MRD>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<MRD> CreateMRD(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    theMRDNameSpace::types type = theMRDNameSpace::types_server,
    theMRDNameSpace::actions action = theMRDNameSpace::actions_getPage,
    ::flatbuffers::Offset<::flatbuffers::String> data = 0) {
  MRDBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_action(action);
  builder_.add_type(type);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<MRD> CreateMRDDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    theMRDNameSpace::types type = theMRDNameSpace::types_server,
    theMRDNameSpace::actions action = theMRDNameSpace::actions_getPage,
    const char *data = nullptr) {
  auto data__ = data ? _fbb.CreateString(data) : 0;
  return theMRDNameSpace::CreateMRD(
      _fbb,
      type,
      action,
      data__);
}

inline const theMRDNameSpace::MRD *GetMRD(const void *buf) {
  return ::flatbuffers::GetRoot<theMRDNameSpace::MRD>(buf);
}

inline const theMRDNameSpace::MRD *GetSizePrefixedMRD(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<theMRDNameSpace::MRD>(buf);
}

inline bool VerifyMRDBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<theMRDNameSpace::MRD>(nullptr);
}

inline bool VerifySizePrefixedMRDBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<theMRDNameSpace::MRD>(nullptr);
}

inline void FinishMRDBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<theMRDNameSpace::MRD> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMRDBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<theMRDNameSpace::MRD> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace theMRDNameSpace

#endif  // FLATBUFFERS_GENERATED_MRD_THEMRDNAMESPACE_H_
