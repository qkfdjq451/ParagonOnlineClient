// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FBMOVE_FB_H_
#define FLATBUFFERS_GENERATED_FBMOVE_FB_H_

#include "flatbuffers/flatbuffers.h"

#include "FB_generated.h"

namespace FB {

struct Move;

struct Move FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_CODE = 4,
    VT_STATE = 6,
    VT_POSITION = 8,
    VT_FOWARD = 10,
    VT_SPEED = 12
  };
  int32_t code() const {
    return GetField<int32_t>(VT_CODE, 0);
  }
  MoveState state() const {
    return static_cast<MoveState>(GetField<int8_t>(VT_STATE, 0));
  }
  const Vec3 *position() const {
    return GetStruct<const Vec3 *>(VT_POSITION);
  }
  const Vec3 *foward() const {
    return GetStruct<const Vec3 *>(VT_FOWARD);
  }
  float speed() const {
    return GetField<float>(VT_SPEED, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_CODE) &&
           VerifyField<int8_t>(verifier, VT_STATE) &&
           VerifyField<Vec3>(verifier, VT_POSITION) &&
           VerifyField<Vec3>(verifier, VT_FOWARD) &&
           VerifyField<float>(verifier, VT_SPEED) &&
           verifier.EndTable();
  }
};

struct MoveBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_code(int32_t code) {
    fbb_.AddElement<int32_t>(Move::VT_CODE, code, 0);
  }
  void add_state(MoveState state) {
    fbb_.AddElement<int8_t>(Move::VT_STATE, static_cast<int8_t>(state), 0);
  }
  void add_position(const Vec3 *position) {
    fbb_.AddStruct(Move::VT_POSITION, position);
  }
  void add_foward(const Vec3 *foward) {
    fbb_.AddStruct(Move::VT_FOWARD, foward);
  }
  void add_speed(float speed) {
    fbb_.AddElement<float>(Move::VT_SPEED, speed, 0.0f);
  }
  explicit MoveBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MoveBuilder &operator=(const MoveBuilder &);
  flatbuffers::Offset<Move> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Move>(end);
    return o;
  }
};

inline flatbuffers::Offset<Move> CreateMove(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t code = 0,
    MoveState state = MoveState_MOVING,
    const Vec3 *position = 0,
    const Vec3 *foward = 0,
    float speed = 0.0f) {
  MoveBuilder builder_(_fbb);
  builder_.add_speed(speed);
  builder_.add_foward(foward);
  builder_.add_position(position);
  builder_.add_code(code);
  builder_.add_state(state);
  return builder_.Finish();
}

inline const FB::Move *GetMove(const void *buf) {
  return flatbuffers::GetRoot<FB::Move>(buf);
}

inline const FB::Move *GetSizePrefixedMove(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FB::Move>(buf);
}

inline bool VerifyMoveBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FB::Move>(nullptr);
}

inline bool VerifySizePrefixedMoveBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FB::Move>(nullptr);
}

inline void FinishMoveBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::Move> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMoveBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::Move> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace FB

#endif  // FLATBUFFERS_GENERATED_FBMOVE_FB_H_