// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/unittest_no_arena_lite.proto

#ifndef PROTOBUF_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto__INCLUDED
#define PROTOBUF_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3001000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3001000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace protobuf_unittest_no_arena {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();
void protobuf_InitDefaults_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();
void protobuf_AssignDesc_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();
void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();

class ForeignMessageLite;

// ===================================================================

class ForeignMessageLite : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:protobuf_unittest_no_arena.ForeignMessageLite) */ {
 public:
  ForeignMessageLite();
  virtual ~ForeignMessageLite();

  ForeignMessageLite(const ForeignMessageLite& from);

  inline ForeignMessageLite& operator=(const ForeignMessageLite& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_.GetNoArena(
        &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }

  inline ::std::string* mutable_unknown_fields() {
    return _unknown_fields_.MutableNoArena(
        &::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }

  static const ForeignMessageLite& default_instance();

  static const ForeignMessageLite* internal_default_instance();

  void Swap(ForeignMessageLite* other);

  // implements Message ----------------------------------------------

  inline ForeignMessageLite* New() const { return New(NULL); }

  ForeignMessageLite* New(::google::protobuf::Arena* arena) const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const ForeignMessageLite& from);
  void MergeFrom(const ForeignMessageLite& from);
  void Clear();
  bool IsInitialized() const;

  size_t ByteSizeLong() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ForeignMessageLite* other);
  void UnsafeMergeFrom(const ForeignMessageLite& from);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _arena_ptr_;
  }
  inline ::google::protobuf::Arena* MaybeArenaPtr() const {
    return _arena_ptr_;
  }
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 c = 1;
  bool has_c() const;
  void clear_c();
  static const int kCFieldNumber = 1;
  ::google::protobuf::int32 c() const;
  void set_c(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:protobuf_unittest_no_arena.ForeignMessageLite)
 private:
  inline void set_has_c();
  inline void clear_has_c();

  ::google::protobuf::internal::ArenaStringPtr _unknown_fields_;
  ::google::protobuf::Arena* _arena_ptr_;

  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::int32 c_;
  friend void  protobuf_InitDefaults_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto_impl();
  friend void  protobuf_AddDesc_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto_impl();
  friend void protobuf_AssignDesc_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();
  friend void protobuf_ShutdownFile_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto();

  void InitAsDefaultInstance();
};
extern ::google::protobuf::internal::ExplicitlyConstructed<ForeignMessageLite> ForeignMessageLite_default_instance_;

// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// ForeignMessageLite

// optional int32 c = 1;
inline bool ForeignMessageLite::has_c() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ForeignMessageLite::set_has_c() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ForeignMessageLite::clear_has_c() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ForeignMessageLite::clear_c() {
  c_ = 0;
  clear_has_c();
}
inline ::google::protobuf::int32 ForeignMessageLite::c() const {
  // @@protoc_insertion_point(field_get:protobuf_unittest_no_arena.ForeignMessageLite.c)
  return c_;
}
inline void ForeignMessageLite::set_c(::google::protobuf::int32 value) {
  set_has_c();
  c_ = value;
  // @@protoc_insertion_point(field_set:protobuf_unittest_no_arena.ForeignMessageLite.c)
}

inline const ForeignMessageLite* ForeignMessageLite::internal_default_instance() {
  return &ForeignMessageLite_default_instance_.get();
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace protobuf_unittest_no_arena

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_google_2fprotobuf_2funittest_5fno_5farena_5flite_2eproto__INCLUDED