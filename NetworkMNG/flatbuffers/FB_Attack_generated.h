// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FBATTACK_FB_H_
#define FLATBUFFERS_GENERATED_FBATTACK_FB_H_

#include "flatbuffers/flatbuffers.h"

#include "FB_generated.h"

namespace FB {

struct Attack;

struct Attack FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_CODE = 4,
    VT_STATE = 6,
    VT_YAW = 8
  };
  int32_t code() const {
    return GetField<int32_t>(VT_CODE, 0);
  }
  AttackState state() const {
    return static_cast<AttackState>(GetField<int8_t>(VT_STATE, 0));
  }
  float yaw() const {
    return GetField<float>(VT_YAW, 0.0f);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_CODE) &&
           VerifyField<int8_t>(verifier, VT_STATE) &&
           VerifyField<float>(verifier, VT_YAW) &&
           verifier.EndTable();
  }
};

struct AttackBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_code(int32_t code) {
    fbb_.AddElement<int32_t>(Attack::VT_CODE, code, 0);
  }
  void add_state(AttackState state) {
    fbb_.AddElement<int8_t>(Attack::VT_STATE, static_cast<int8_t>(state), 0);
  }
  void add_yaw(float yaw) {
    fbb_.AddElement<float>(Attack::VT_YAW, yaw, 0.0f);
  }
  explicit AttackBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AttackBuilder &operator=(const AttackBuilder &);
  flatbuffers::Offset<Attack> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Attack>(end);
    return o;
  }
};

inline flatbuffers::Offset<Attack> CreateAttack(
    flatbuffers::FlatBufferBuilder &_fbb,
    int32_t code = 0,
    AttackState state = AttackState_Combo1,
    float yaw = 0.0f) {
  AttackBuilder builder_(_fbb);
  builder_.add_yaw(yaw);
  builder_.add_code(code);
  builder_.add_state(state);
  return builder_.Finish();
}

inline const FB::Attack *GetAttack(const void *buf) {
  return flatbuffers::GetRoot<FB::Attack>(buf);
}

inline const FB::Attack *GetSizePrefixedAttack(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FB::Attack>(buf);
}

inline bool VerifyAttackBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FB::Attack>(nullptr);
}

inline bool VerifySizePrefixedAttackBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FB::Attack>(nullptr);
}

inline void FinishAttackBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::Attack> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedAttackBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::Attack> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace FB

#endif  // FLATBUFFERS_GENERATED_FBATTACK_FB_H_
