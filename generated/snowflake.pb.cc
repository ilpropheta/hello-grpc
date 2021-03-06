// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: snowflake.proto

#include "snowflake.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG
constexpr NextIdRequest::NextIdRequest(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized){}
struct NextIdRequestDefaultTypeInternal {
  constexpr NextIdRequestDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~NextIdRequestDefaultTypeInternal() {}
  union {
    NextIdRequest _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT NextIdRequestDefaultTypeInternal _NextIdRequest_default_instance_;
constexpr NextIdResponse::NextIdResponse(
  ::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized)
  : value_(PROTOBUF_LONGLONG(0)){}
struct NextIdResponseDefaultTypeInternal {
  constexpr NextIdResponseDefaultTypeInternal()
    : _instance(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized{}) {}
  ~NextIdResponseDefaultTypeInternal() {}
  union {
    NextIdResponse _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT NextIdResponseDefaultTypeInternal _NextIdResponse_default_instance_;
static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_snowflake_2eproto[2];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_snowflake_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_snowflake_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_snowflake_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::NextIdRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::NextIdResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::NextIdResponse, value_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::NextIdRequest)},
  { 5, -1, sizeof(::NextIdResponse)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_NextIdRequest_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::_NextIdResponse_default_instance_),
};

const char descriptor_table_protodef_snowflake_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\017snowflake.proto\"\017\n\rNextIdRequest\"\037\n\016Ne"
  "xtIdResponse\022\r\n\005value\030\001 \001(\0032@\n\017Snowflake"
  "Server\022-\n\006NextId\022\016.NextIdRequest\032\017.NextI"
  "dResponse(\0010\001b\006proto3"
  ;
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_snowflake_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_snowflake_2eproto = {
  false, false, 141, descriptor_table_protodef_snowflake_2eproto, "snowflake.proto", 
  &descriptor_table_snowflake_2eproto_once, nullptr, 0, 2,
  schemas, file_default_instances, TableStruct_snowflake_2eproto::offsets,
  file_level_metadata_snowflake_2eproto, file_level_enum_descriptors_snowflake_2eproto, file_level_service_descriptors_snowflake_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK ::PROTOBUF_NAMESPACE_ID::Metadata
descriptor_table_snowflake_2eproto_metadata_getter(int index) {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_snowflake_2eproto);
  return descriptor_table_snowflake_2eproto.file_level_metadata[index];
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY static ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptorsRunner dynamic_init_dummy_snowflake_2eproto(&descriptor_table_snowflake_2eproto);

// ===================================================================

class NextIdRequest::_Internal {
 public:
};

NextIdRequest::NextIdRequest(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:NextIdRequest)
}
NextIdRequest::NextIdRequest(const NextIdRequest& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:NextIdRequest)
}

void NextIdRequest::SharedCtor() {
}

NextIdRequest::~NextIdRequest() {
  // @@protoc_insertion_point(destructor:NextIdRequest)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void NextIdRequest::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void NextIdRequest::ArenaDtor(void* object) {
  NextIdRequest* _this = reinterpret_cast< NextIdRequest* >(object);
  (void)_this;
}
void NextIdRequest::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void NextIdRequest::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void NextIdRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:NextIdRequest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* NextIdRequest::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* NextIdRequest::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:NextIdRequest)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:NextIdRequest)
  return target;
}

size_t NextIdRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:NextIdRequest)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void NextIdRequest::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:NextIdRequest)
  GOOGLE_DCHECK_NE(&from, this);
  const NextIdRequest* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<NextIdRequest>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:NextIdRequest)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:NextIdRequest)
    MergeFrom(*source);
  }
}

void NextIdRequest::MergeFrom(const NextIdRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:NextIdRequest)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void NextIdRequest::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:NextIdRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NextIdRequest::CopyFrom(const NextIdRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:NextIdRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NextIdRequest::IsInitialized() const {
  return true;
}

void NextIdRequest::InternalSwap(NextIdRequest* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
}

::PROTOBUF_NAMESPACE_ID::Metadata NextIdRequest::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class NextIdResponse::_Internal {
 public:
};

NextIdResponse::NextIdResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:NextIdResponse)
}
NextIdResponse::NextIdResponse(const NextIdResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  value_ = from.value_;
  // @@protoc_insertion_point(copy_constructor:NextIdResponse)
}

void NextIdResponse::SharedCtor() {
value_ = PROTOBUF_LONGLONG(0);
}

NextIdResponse::~NextIdResponse() {
  // @@protoc_insertion_point(destructor:NextIdResponse)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void NextIdResponse::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void NextIdResponse::ArenaDtor(void* object) {
  NextIdResponse* _this = reinterpret_cast< NextIdResponse* >(object);
  (void)_this;
}
void NextIdResponse::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void NextIdResponse::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}

void NextIdResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:NextIdResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  value_ = PROTOBUF_LONGLONG(0);
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* NextIdResponse::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // int64 value = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          value_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* NextIdResponse::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:NextIdResponse)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int64 value = 1;
  if (this->value() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt64ToArray(1, this->_internal_value(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:NextIdResponse)
  return target;
}

size_t NextIdResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:NextIdResponse)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int64 value = 1;
  if (this->value() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int64Size(
        this->_internal_value());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void NextIdResponse::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:NextIdResponse)
  GOOGLE_DCHECK_NE(&from, this);
  const NextIdResponse* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<NextIdResponse>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:NextIdResponse)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:NextIdResponse)
    MergeFrom(*source);
  }
}

void NextIdResponse::MergeFrom(const NextIdResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:NextIdResponse)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.value() != 0) {
    _internal_set_value(from._internal_value());
  }
}

void NextIdResponse::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:NextIdResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NextIdResponse::CopyFrom(const NextIdResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:NextIdResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NextIdResponse::IsInitialized() const {
  return true;
}

void NextIdResponse::InternalSwap(NextIdResponse* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(value_, other->value_);
}

::PROTOBUF_NAMESPACE_ID::Metadata NextIdResponse::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::NextIdRequest* Arena::CreateMaybeMessage< ::NextIdRequest >(Arena* arena) {
  return Arena::CreateMessageInternal< ::NextIdRequest >(arena);
}
template<> PROTOBUF_NOINLINE ::NextIdResponse* Arena::CreateMaybeMessage< ::NextIdResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::NextIdResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
