// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_FBMOVEVEC_FB_H_
#define FLATBUFFERS_GENERATED_FBMOVEVEC_FB_H_

#include "flatbuffers/flatbuffers.h"

#include "FB_generated.h"
#include "FB_Move_generated.h"

namespace FB {

struct MoveVec;

struct MoveVec FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MOVEVECTOR = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<Move>> *movevector() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Move>> *>(VT_MOVEVECTOR);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_MOVEVECTOR) &&
           verifier.Verify(movevector()) &&
           verifier.VerifyVectorOfTables(movevector()) &&
           verifier.EndTable();
  }
};

struct MoveVecBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_movevector(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Move>>> movevector) {
    fbb_.AddOffset(MoveVec::VT_MOVEVECTOR, movevector);
  }
  explicit MoveVecBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MoveVecBuilder &operator=(const MoveVecBuilder &);
  flatbuffers::Offset<MoveVec> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MoveVec>(end);
    return o;
  }
};

inline flatbuffers::Offset<MoveVec> CreateMoveVec(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Move>>> movevector = 0) {
  MoveVecBuilder builder_(_fbb);
  builder_.add_movevector(movevector);
  return builder_.Finish();
}

inline flatbuffers::Offset<MoveVec> CreateMoveVecDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Move>> *movevector = nullptr) {
  return FB::CreateMoveVec(
      _fbb,
      movevector ? _fbb.CreateVector<flatbuffers::Offset<Move>>(*movevector) : 0);
}

inline const FB::MoveVec *GetMoveVec(const void *buf) {
  return flatbuffers::GetRoot<FB::MoveVec>(buf);
}

inline const FB::MoveVec *GetSizePrefixedMoveVec(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<FB::MoveVec>(buf);
}

inline bool VerifyMoveVecBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<FB::MoveVec>(nullptr);
}

inline bool VerifySizePrefixedMoveVecBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<FB::MoveVec>(nullptr);
}

inline void FinishMoveVecBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::MoveVec> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMoveVecBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<FB::MoveVec> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace FB

#endif  // FLATBUFFERS_GENERATED_FBMOVEVEC_FB_H_