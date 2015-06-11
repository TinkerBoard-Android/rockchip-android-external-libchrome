// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BASE_TRACE_EVENT_TRACE_EVENT_MEMORY_OVERHEAD_H_
#define BASE_TRACE_EVENT_TRACE_EVENT_MEMORY_OVERHEAD_H_

#include "base/base_export.h"
#include "base/containers/hash_tables.h"
#include "base/containers/small_map.h"

namespace base {

class RefCountedString;
class Value;

namespace trace_event {

class ProcessMemoryDump;

// Used to estimate the memory overhead of the tracing infrastructure.
class BASE_EXPORT TraceEventMemoryOverhead {
 public:
  TraceEventMemoryOverhead();
  ~TraceEventMemoryOverhead();

  void Add(const char* object_type, size_t size_in_bytes);
  void AddString(const std::string& str);
  void AddValue(const Value& value);
  void AddRefCountedString(const RefCountedString& str);

  // Call this after all the Add* methods above to account the memory used by
  // this TraceEventMemoryOverhead instance itself.
  void AddSelf();

  // Adds up and merges all the values from |other| to this instance.
  void Update(const TraceEventMemoryOverhead& other);

  void DumpInto(const char* base_name, ProcessMemoryDump* pmd) const;

 private:
  struct ObjectCountAndSize {
    size_t count;
    size_t size_in_bytes;
  };
#define TRACE_EVENT_MEMORY_OVERHEAD_MAP_SIZE 16
  using map_type = SmallMap<hash_map<const char*, ObjectCountAndSize>,
                            TRACE_EVENT_MEMORY_OVERHEAD_MAP_SIZE>;
  map_type allocated_objects_;

  void AddOrCreateInternal(const char* object_type,
                           size_t count,
                           size_t size_in_bytes);

  DISALLOW_COPY_AND_ASSIGN(TraceEventMemoryOverhead);
};

}  // namespace trace_event
}  // namespace base

#endif  // BASE_TRACE_EVENT_TRACE_EVENT_MEMORY_OVERHEAD_H_
