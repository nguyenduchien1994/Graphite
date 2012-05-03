#pragma once

#include <string>
using std::string;

#include "fixed_types.h"
#include "cache_line_info.h"
#include "cache.h"

// Everything related to cache sets
class CacheSet
{
public:
   CacheSet(CachingProtocolType caching_protocol_type, SInt32 cache_level, UInt32 associativity, UInt32 line_size);
   virtual ~CacheSet();

   // Functions to create a cache set and replacement policy
   static CacheSet* createCacheSet(Cache::ReplacementPolicy replacement_policy,
                                   CachingProtocolType caching_protocol_type, SInt32 cache_level,
                                   UInt32 associativity, UInt32 line_size);

   void read_line(UInt32 line_index, UInt32 offset, Byte *out_buf, UInt32 bytes);
   void write_line(UInt32 line_index, UInt32 offset, Byte *in_buf, UInt32 bytes);
   CacheLineInfo* find(IntPtr tag, UInt32* line_index = NULL);
   void insert(CacheLineInfo* inserted_cache_line_info, Byte* fill_buf,
               bool* eviction, CacheLineInfo* evicted_cache_line_info, Byte* writeback_buf);

protected:
   CacheLineInfo** _cache_line_info_array;
   char* _lines;
   UInt32 _associativity;
   UInt32 _line_size;
   
private:
   virtual UInt32 getReplacementIndex() = 0;
   virtual void updateReplacementIndex(UInt32) = 0;
};

class CacheSetRoundRobin : public CacheSet
{
public:
   CacheSetRoundRobin(CachingProtocolType caching_protocol_type, SInt32 cache_level, UInt32 associativity, UInt32 line_size);
   ~CacheSetRoundRobin();

private:
   UInt32 getReplacementIndex();
   void updateReplacementIndex(UInt32 accessed_index);
   
   UInt32 _replacement_index;
};

class CacheSetLRU : public CacheSet
{
public:
   CacheSetLRU(CachingProtocolType caching_protocol_type, SInt32 cache_level, UInt32 associativity, UInt32 line_size);
   ~CacheSetLRU();

private:
   UInt32 getReplacementIndex();
   void updateReplacementIndex(UInt32 accessed_index);
   
   UInt8* _lru_bits;
};
