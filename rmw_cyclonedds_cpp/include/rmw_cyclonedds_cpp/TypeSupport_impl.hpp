// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
// Copyright 2018 ADLINK Technology
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RMW_CYCLONEDDS_CPP__TYPESUPPORT_IMPL_HPP_
#define RMW_CYCLONEDDS_CPP__TYPESUPPORT_IMPL_HPP_

#include <cassert>
#include <functional>
#include <string>
#include <vector>

#include "rmw_cyclonedds_cpp/TypeSupport.hpp"
#include "rmw_cyclonedds_cpp/macros.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"

#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

#include "rosidl_generator_c/primitives_sequence_functions.h"

#include "serdes.hpp"

namespace rmw_cyclonedds_cpp
{

template<typename T>
struct GenericCSequence;

// multiple definitions of ambiguous primitive types
SPECIALIZE_GENERIC_C_SEQUENCE(bool, bool)
SPECIALIZE_GENERIC_C_SEQUENCE(byte, uint8_t)
SPECIALIZE_GENERIC_C_SEQUENCE(char, char)
SPECIALIZE_GENERIC_C_SEQUENCE(float32, float)
SPECIALIZE_GENERIC_C_SEQUENCE(float64, double)
SPECIALIZE_GENERIC_C_SEQUENCE(int8, int8_t)
SPECIALIZE_GENERIC_C_SEQUENCE(int16, int16_t)
SPECIALIZE_GENERIC_C_SEQUENCE(uint16, uint16_t)
SPECIALIZE_GENERIC_C_SEQUENCE(int32, int32_t)
SPECIALIZE_GENERIC_C_SEQUENCE(uint32, uint32_t)
SPECIALIZE_GENERIC_C_SEQUENCE(int64, int64_t)
SPECIALIZE_GENERIC_C_SEQUENCE(uint64, uint64_t)

typedef struct rosidl_generator_c__void__Sequence
{
  void * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} rosidl_generator_c__void__Sequence;

inline
bool
rosidl_generator_c__void__Sequence__init(
  rosidl_generator_c__void__Sequence * sequence, size_t size, size_t member_size)
{
  if (!sequence) {
    return false;
  }
  void * data = nullptr;
  if (size) {
    data = static_cast<void *>(calloc(size, member_size));
    if (!data) {
      return false;
    }
  }
  sequence->data = data;
  sequence->size = size;
  sequence->capacity = size;
  return true;
}

inline
void
rosidl_generator_c__void__Sequence__fini(rosidl_generator_c__void__Sequence * sequence)
{
  if (!sequence) {
    return;
  }
  if (sequence->data) {
    // ensure that data and capacity values are consistent
    assert(sequence->capacity > 0);
    // finalize all sequence elements
    free(sequence->data);
    sequence->data = nullptr;
    sequence->size = 0;
    sequence->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == sequence->size);
    assert(0 == sequence->capacity);
  }
}

template<typename MembersType>
TypeSupport<MembersType>::TypeSupport()
{
    name = "";
}

template<typename MembersType>
void TypeSupport<MembersType>::setName(const std::string& name)
{
    this->name = std::string(name);
}

static inline void *
align_(size_t __align, void * & __ptr) noexcept
{
  const auto __intptr = reinterpret_cast<uintptr_t>(__ptr);
  const auto __aligned = (__intptr - 1u + __align) & ~(__align - 1);
  return __ptr = reinterpret_cast<void *>(__aligned);
}

template<typename MembersType>
static size_t calculateMaxAlign(const MembersType * members)
{
  size_t max_align = 0;

  for (uint32_t i = 0; i < members->member_count_; ++i) {
    size_t alignment = 0;
    const auto & member = members->members_[i];

    if (member.is_array_ && (!member.array_size_ || member.is_upper_bound_)) {
      alignment = alignof(std::vector<unsigned char>);
    } else {
      switch (member.type_id_) {
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOL:
          alignment = alignof(bool);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BYTE:
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
          alignment = alignof(uint8_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
          alignment = alignof(char);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT32:
          alignment = alignof(float);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT64:
          alignment = alignof(double);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
          alignment = alignof(int16_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
          alignment = alignof(uint16_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
          alignment = alignof(int32_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
          alignment = alignof(uint32_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
          alignment = alignof(int64_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
          alignment = alignof(uint64_t);
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
          // Note: specialization needed because calculateMaxAlign is called before
          // casting submembers as std::string, returned value is the same on i386
          if (std::is_same<MembersType,
            rosidl_typesupport_introspection_c__MessageMembers>::value)
          {
            alignment = alignof(rosidl_generator_c__String);
          } else {
            alignment = alignof(std::string);
          }
          break;
        case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
          {
            auto sub_members = (const MembersType *)member.members_->data;
            alignment = calculateMaxAlign(sub_members);
          }
          break;
      }
    }

    if (alignment > max_align) {
      max_align = alignment;
    }
  }

  return max_align;
}

// C++ specialization
template<typename T>
void serialize_field(
  const rosidl_typesupport_introspection_cpp::MessageMember * member,
  void * field,
  cycser & ser)
{
  if (!member->is_array_) {
    ser << *static_cast<T *>(field);
  } else if (member->array_size_ && !member->is_upper_bound_) {
    ser.serializeA(static_cast<T *>(field), member->array_size_);
  } else {
    std::vector<T> & data = *reinterpret_cast<std::vector<T> *>(field);
    ser << data;
  }
}

// C specialization
template<typename T>
void serialize_field(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  void * field,
  cycser & ser)
{
  if (!member->is_array_) {
    ser << *static_cast<T *>(field);
  } else if (member->array_size_ && !member->is_upper_bound_) {
    ser.serializeA(static_cast<T *>(field), member->array_size_);
  } else {
    auto & data = *reinterpret_cast<typename GenericCSequence<T>::type *>(field);
    ser.serializeS(reinterpret_cast<T *>(data.data), data.size);
  }
}

template<>
inline
void serialize_field<std::string>(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  void * field,
  cycser & ser)
{
  using CStringHelper = StringHelper<rosidl_typesupport_introspection_c__MessageMembers>;
  if (!member->is_array_) {
    auto && str = CStringHelper::convert_to_std_string(field);
    // Control maximum length.
    if (member->string_upper_bound_ && str.length() > member->string_upper_bound_ + 1) {
      throw std::runtime_error("string overcomes the maximum length");
    }
    ser << str;
  } else {
    // First, cast field to rosidl_generator_c
    // Then convert to a std::string using StringHelper and serialize the std::string
    if (member->array_size_ && !member->is_upper_bound_) {
      // tmpstring is defined here and not below to avoid
      // memory allocation in every iteration of the for loop
      std::string tmpstring;
      auto string_field = static_cast<rosidl_generator_c__String *>(field);
      for (size_t i = 0; i < member->array_size_; ++i) {
        tmpstring = string_field[i].data;
        ser.serialize(tmpstring);
      }
    } else {
      auto & string_array_field = *reinterpret_cast<rosidl_generator_c__String__Sequence *>(field);
      std::vector<std::string> cpp_string_vector;
      for (size_t i = 0; i < string_array_field.size; ++i) {
        cpp_string_vector.push_back(
          CStringHelper::convert_to_std_string(string_array_field.data[i]));
      }
      ser << cpp_string_vector;
    }
  }
}

inline
size_t get_array_size_and_assign_field(
  const rosidl_typesupport_introspection_cpp::MessageMember * member,
  void * field,
  void * & subros_message,
  size_t sub_members_size,
  size_t max_align)
{
  auto vector = reinterpret_cast<std::vector<unsigned char> *>(field);
  void * ptr = reinterpret_cast<void *>(sub_members_size);
  size_t vsize = vector->size() / reinterpret_cast<size_t>(align_(max_align, ptr));
  if (member->is_upper_bound_ && vsize > member->array_size_) {
    throw std::runtime_error("vector overcomes the maximum length");
  }
  subros_message = reinterpret_cast<void *>(vector->data());
  return vsize;
}

inline
size_t get_array_size_and_assign_field(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  void * field,
  void * & subros_message,
  size_t, size_t)
{
  auto tmpsequence = static_cast<rosidl_generator_c__void__Sequence *>(field);
  if (member->is_upper_bound_ && tmpsequence->size > member->array_size_) {
    throw std::runtime_error("vector overcomes the maximum length");
  }
  subros_message = reinterpret_cast<void *>(tmpsequence->data);
  return tmpsequence->size;
}

template<typename MembersType>
bool TypeSupport<MembersType>::serializeROSmessage(
  cycser & ser, const MembersType * members, const void * ros_message)
{
  assert(members);
  assert(ros_message);

  for (uint32_t i = 0; i < members->member_count_; ++i) {
    const auto member = members->members_ + i;
    void * field = const_cast<char *>(static_cast<const char *>(ros_message)) + member->offset_;
    switch (member->type_id_) {
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOL:
        if (!member->is_array_) {
          // don't cast to bool here because if the bool is
          // uninitialized the random value can't be deserialized
          ser << (*static_cast<uint8_t *>(field) ? true : false);
        } else {
          serialize_field<bool>(member, field, ser);
        }
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BYTE:
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
        serialize_field<uint8_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
        serialize_field<char>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT32:
        serialize_field<float>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT64:
        serialize_field<double>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
        serialize_field<int16_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
        serialize_field<uint16_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
        serialize_field<int32_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
        serialize_field<uint32_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
        serialize_field<int64_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
        serialize_field<uint64_t>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
        serialize_field<std::string>(member, field, ser);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
        {
          auto sub_members = static_cast<const MembersType *>(member->members_->data);
          if (!member->is_array_) {
            serializeROSmessage(ser, sub_members, field);
          } else {
            void * subros_message = nullptr;
            size_t array_size = 0;
            size_t sub_members_size = sub_members->size_of_;
            size_t max_align = calculateMaxAlign(sub_members);

            if (member->array_size_ && !member->is_upper_bound_) {
              subros_message = field;
              array_size = member->array_size_;
            } else {
              array_size = get_array_size_and_assign_field(
                member, field, subros_message, sub_members_size, max_align);

              // Serialize length
              ser << (uint32_t)array_size;
            }

            for (size_t index = 0; index < array_size; ++index) {
              serializeROSmessage(ser, sub_members, subros_message);
              subros_message = static_cast<char *>(subros_message) + sub_members_size;
              subros_message = align_(max_align, subros_message);
            }
          }
        }
        break;
      default:
        throw std::runtime_error("unknown type");
    }
  }

  return true;
}

template<typename T>
void deserialize_field(
  const rosidl_typesupport_introspection_cpp::MessageMember * member,
  void * field,
  cycdeser & deser,
  bool call_new)
{
  if (!member->is_array_) {
    deser >> *static_cast<T *>(field);
  } else if (member->array_size_ && !member->is_upper_bound_) {
    deser.deserializeA(static_cast<T *>(field), member->array_size_);
  } else {
    auto & vector = *reinterpret_cast<std::vector<T> *>(field);
    if (call_new) {
      new(&vector) std::vector<T>;
    }
    deser >> vector;
  }
}

template<>
inline void deserialize_field<std::string>(
  const rosidl_typesupport_introspection_cpp::MessageMember * member,
  void * field,
  cycdeser & deser,
  bool call_new)
{
  if (!member->is_array_) {
    if (call_new) {
      // Because std::string is a complex datatype, we need to make sure that
      // the memory is initialized to something reasonable before eventually
      // passing it as a reference.
      new(field) std::string();
    }
    deser >> *static_cast<std::string *>(field);
  } else if (member->array_size_ && !member->is_upper_bound_) {
    std::string * array = static_cast<std::string *>(field);
    if (call_new) {
      for (size_t i = 0; i < member->array_size_; ++i) {
        new(&array[i]) std::string();
      }
    }
    deser.deserializeA(array, member->array_size_);
  } else {
    auto & vector = *reinterpret_cast<std::vector<std::string> *>(field);
    if (call_new) {
      new(&vector) std::vector<std::string>;
    }
    deser >> vector;
  }
}

template<typename T>
void deserialize_field(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  void * field,
  cycdeser & deser,
  bool call_new)
{
  (void)call_new;
  if (!member->is_array_) {
    deser >> *static_cast<T *>(field);
  } else if (member->array_size_ && !member->is_upper_bound_) {
    deser.deserializeA(static_cast<T *>(field), member->array_size_);
  } else {
    auto & data = *reinterpret_cast<typename GenericCSequence<T>::type *>(field);
    int32_t dsize = 0;
    deser >> dsize;
    GenericCSequence<T>::init(&data, dsize);
    deser.deserializeA(reinterpret_cast<T *>(data.data), dsize);
  }
}

template<>
inline void deserialize_field<std::string>(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  void * field,
  cycdeser & deser,
  bool call_new)
{
  (void)call_new;
  if (!member->is_array_) {
    using CStringHelper = StringHelper<rosidl_typesupport_introspection_c__MessageMembers>;
    CStringHelper::assign(deser, field, call_new);
  } else {
    if (member->array_size_ && !member->is_upper_bound_) {
      auto deser_field = static_cast<rosidl_generator_c__String *>(field);
      // tmpstring is defined here and not below to avoid
      // memory allocation in every iteration of the for loop
      std::string tmpstring;
      for (size_t i = 0; i < member->array_size_; ++i) {
        deser.deserialize(tmpstring);
        if (!rosidl_generator_c__String__assign(&deser_field[i], tmpstring.c_str())) {
          throw std::runtime_error("unable to assign rosidl_generator_c__String");
        }
      }
    } else {
      std::vector<std::string> cpp_string_vector;
      deser >> cpp_string_vector;

      auto & string_array_field = *reinterpret_cast<rosidl_generator_c__String__Sequence *>(field);
      if (!rosidl_generator_c__String__Sequence__init(&string_array_field, cpp_string_vector.size())) {
        throw std::runtime_error("unable to initialize rosidl_generator_c__String array");
      }

      for (size_t i = 0; i < cpp_string_vector.size(); ++i) {
        if (!rosidl_generator_c__String__assign(&string_array_field.data[i],
          cpp_string_vector[i].c_str()))
        {
          throw std::runtime_error("unable to assign rosidl_generator_c__String");
        }
      }
    }
  }
}

inline size_t get_submessage_array_deserialize(
  const rosidl_typesupport_introspection_cpp::MessageMember * member,
  cycdeser & deser,
  void * field,
  void * & subros_message,
  bool call_new,
  size_t sub_members_size,
  size_t max_align)
{
  (void)member;
  uint32_t vsize = 0;
  // Deserialize length
  deser >> vsize;
  auto vector = reinterpret_cast<std::vector<unsigned char> *>(field);
  if (call_new) {
    new(vector) std::vector<unsigned char>;
  }
  void * ptr = reinterpret_cast<void *>(sub_members_size);
  vector->resize(vsize * (size_t)align_(max_align, ptr));
  subros_message = reinterpret_cast<void *>(vector->data());
  return vsize;
}

inline size_t get_submessage_array_deserialize(
  const rosidl_typesupport_introspection_c__MessageMember * member,
  cycdeser & deser,
  void * field,
  void * & subros_message,
  bool,
  size_t sub_members_size,
  size_t)
{
  (void)member;
  // Deserialize length
  uint32_t vsize = 0;
  deser >> vsize;
  auto tmparray = static_cast<rosidl_generator_c__void__Sequence *>(field);
  rosidl_generator_c__void__Sequence__init(tmparray, vsize, sub_members_size);
  subros_message = reinterpret_cast<void *>(tmparray->data);
  return vsize;
}

template<typename MembersType>
bool TypeSupport<MembersType>::deserializeROSmessage(
  cycdeser & deser, const MembersType * members, void * ros_message, bool call_new)
{
  assert(members);
  assert(ros_message);

  for (uint32_t i = 0; i < members->member_count_; ++i) {
    const auto * member = members->members_ + i;
    void * field = static_cast<char *>(ros_message) + member->offset_;
    switch (member->type_id_) {
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOL:
        deserialize_field<bool>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BYTE:
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8:
        deserialize_field<uint8_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_CHAR:
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT8:
        deserialize_field<char>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT32:
        deserialize_field<float>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT64:
        deserialize_field<double>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT16:
        deserialize_field<int16_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT16:
        deserialize_field<uint16_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32:
        deserialize_field<int32_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32:
        deserialize_field<uint32_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT64:
        deserialize_field<int64_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT64:
        deserialize_field<uint64_t>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING:
        deserialize_field<std::string>(member, field, deser, call_new);
        break;
      case ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE:
        {
          auto sub_members = (const MembersType *)member->members_->data;
          if (!member->is_array_) {
            deserializeROSmessage(deser, sub_members, field, call_new);
          } else {
            void * subros_message = nullptr;
            size_t array_size = 0;
            size_t sub_members_size = sub_members->size_of_;
            size_t max_align = calculateMaxAlign(sub_members);
            bool recall_new = call_new;

            if (member->array_size_ && !member->is_upper_bound_) {
              subros_message = field;
              array_size = member->array_size_;
            } else {
              array_size = get_submessage_array_deserialize(
                member, deser, field, subros_message,
                call_new, sub_members_size, max_align);
              recall_new = true;
            }

            for (size_t index = 0; index < array_size; ++index) {
              deserializeROSmessage(deser, sub_members, subros_message, recall_new);
              subros_message = static_cast<char *>(subros_message) + sub_members_size;
              subros_message = align_(max_align, subros_message);
            }
          }
        }
        break;
      default:
        throw std::runtime_error("unknown type");
    }
  }

  return true;
}

template<typename MembersType>
bool TypeSupport<MembersType>::serializeROSmessage(
  const void * ros_message, cycser & ser,
  std::function<void(cycser&)> prefix)
{
  assert(ros_message);

  if (prefix) {
    prefix(ser);
  }

  if (members_->member_count_ != 0) {
    TypeSupport::serializeROSmessage(ser, members_, ros_message);
  } else {
    ser << (uint8_t)0;
  }

  return true;
}

template<typename MembersType>
bool TypeSupport<MembersType>::deserializeROSmessage(
  cycdeser & deser, void * ros_message,
  std::function<void(cycdeser&)> prefix)
{
  assert(ros_message);

  if (prefix) {
    prefix(deser);
  }

  if (members_->member_count_ != 0) {
    TypeSupport::deserializeROSmessage(deser, members_, ros_message, false);
  } else {
    uint8_t dump = 0;
    deser >> dump;
    (void)dump;
  }

  return true;
}

template<typename MembersType>
std::string TypeSupport<MembersType>::getName ()
{
  return name;
}

}  // namespace rmw_cyclonedds_cpp

#endif  // RMW_CYCLONEDDS_CPP__TYPESUPPORT_IMPL_HPP_
