// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2018
// MIT License

#pragma once

#include <stddef.h>  // for NULL

#include "../Memory/JsonBufferAllocated.hpp"

namespace ArduinoJson {
namespace Internals {

// A node for a singly-linked list.
// Used by List<T> and its iterators.
template <typename T>
struct ListNode : public Internals::JsonBufferAllocated {
  ListNode() NOEXCEPT : next(NULL) {}

  ListNode<T> *next;
  T content;
};
}  // namespace Internals
}  // namespace ArduinoJson
