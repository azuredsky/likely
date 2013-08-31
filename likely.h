/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright 2013 Joshua C. Klontz                                           *
 *                                                                           *
 * Licensed under the Apache License, Version 2.0 (the "License");           *
 * you may not use this file except in compliance with the License.          *
 * You may obtain a copy of the License at                                   *
 *                                                                           *
 *     http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                           *
 * Unless required by applicable law or agreed to in writing, software       *
 * distributed under the License is distributed on an "AS IS" BASIS,         *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 * See the License for the specific language governing permissions and       *
 * limitations under the License.                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef LIKELY_H
#define LIKELY_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

// Don't worry about this
#if defined LIKELY_LIBRARY
#  if defined _WIN32 || defined __CYGWIN__
#    define LIKELY_EXPORT __declspec(dllexport)
#  else
#    define LIKELY_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  if defined _WIN32 || defined __CYGWIN__
#    define LIKELY_EXPORT __declspec(dllimport)
#  else
#    define LIKELY_EXPORT
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Contents of the Likely Standard Library, standard.likely
LIKELY_EXPORT const char *likely_standard_library();

// Stores matrix size
typedef uint32_t likely_size;

// Encodes matrix metadata
typedef uint32_t likely_hash; /* Depth : 8
                                 Signed : 1
                                 Floating : 1
                                 Parallel : 1
                                 Heterogeneous : 1
                                 Single-channel : 1
                                 Single-column : 1
                                 Single-row : 1
                                 Single-frame : 1
                                 Owner : 1
                                 Reserved : 15 */

// Convenience values for editing a likely_hash
enum likely_hash_field
{
    likely_hash_null = 0x00000000,
    likely_hash_depth = 0x000000FF,
    likely_hash_signed = 0x00000100,
    likely_hash_floating = 0x00000200,
    likely_hash_type = likely_hash_depth | likely_hash_signed | likely_hash_floating,
    likely_hash_u8  = 8,
    likely_hash_u16 = 16,
    likely_hash_u32 = 32,
    likely_hash_u64 = 64,
    likely_hash_i8  = 8  | likely_hash_signed,
    likely_hash_i16 = 16 | likely_hash_signed,
    likely_hash_i32 = 32 | likely_hash_signed,
    likely_hash_i64 = 64 | likely_hash_signed,
    likely_hash_f16 = 16 | likely_hash_floating | likely_hash_signed,
    likely_hash_f32 = 32 | likely_hash_floating | likely_hash_signed,
    likely_hash_f64 = 64 | likely_hash_floating | likely_hash_signed,
    likely_hash_parallel = 0x00000400,
    likely_hash_heterogeneous = 0x00000800,
    likely_hash_single_channel = 0x00001000,
    likely_hash_single_column = 0x00002000,
    likely_hash_single_row = 0x00004000,
    likely_hash_single_frame = 0x00008000,
    likely_hash_owner = 0x00010000,
    likely_hash_reserved = 0xFFFE0000
};

// The only struct in the API
struct likely_matrix
{
    uint8_t *data;
    likely_hash hash;
    likely_size channels, columns, rows, frames;
};

// You shouldn't need to call these directly
inline int likely_get(likely_hash hash, likely_hash_field mask) { return hash & mask; }
inline void likely_set(likely_hash &hash, int i, likely_hash_field mask) { hash &= ~mask; hash |= i & mask; }
inline bool likely_get_bool(likely_hash hash, likely_hash_field mask) { return hash & mask; }
inline void likely_set_bool(likely_hash &hash, bool b, likely_hash_field mask) { b ? hash |= mask : hash &= ~mask; }

// Convenience functions for querying and editing the hash
inline int  likely_depth(likely_hash hash) { return likely_get(hash, likely_hash_depth); }
inline void likely_set_depth(likely_hash &hash, int depth) { likely_set(hash, depth, likely_hash_depth); }
inline bool likely_is_signed(likely_hash hash) { return likely_get_bool(hash, likely_hash_signed); }
inline void likely_set_signed(likely_hash &hash, bool is_signed) { likely_set_bool(hash, is_signed, likely_hash_signed); }
inline bool likely_is_floating(likely_hash hash) { return likely_get_bool(hash, likely_hash_floating); }
inline void likely_set_floating(likely_hash &hash, bool is_floating) { likely_set_bool(hash, is_floating, likely_hash_floating); }
inline int  likely_type(likely_hash hash) { return likely_get(hash, likely_hash_type); }
inline void likely_set_type(likely_hash &hash, int type) { likely_set(hash, type, likely_hash_type); }
inline bool likely_is_parallel(likely_hash hash) { return likely_get_bool(hash, likely_hash_parallel); }
inline void likely_set_parallel(likely_hash &hash, bool is_parallel) { likely_set_bool(hash, is_parallel, likely_hash_parallel); }
inline bool likely_is_heterogeneous(likely_hash hash) { return likely_get_bool(hash, likely_hash_heterogeneous); }
inline void likely_set_heterogeneous(likely_hash &hash, bool is_heterogeneous) { likely_set_bool(hash, is_heterogeneous, likely_hash_heterogeneous); }
inline bool likely_is_single_channel(likely_hash hash) { return likely_get_bool(hash, likely_hash_single_channel); }
inline void likely_set_single_channel(likely_hash &hash, bool is_single_channel) { likely_set_bool(hash, is_single_channel, likely_hash_single_channel); }
inline bool likely_is_single_column(likely_hash hash) { return likely_get_bool(hash, likely_hash_single_column); }
inline void likely_set_single_column(likely_hash &hash, bool is_single_column) { likely_set_bool(hash, is_single_column, likely_hash_single_column); }
inline bool likely_is_single_row(likely_hash hash) { return likely_get_bool(hash, likely_hash_single_row); }
inline void likely_set_single_row(likely_hash &hash, bool is_single_row) { likely_set_bool(hash, is_single_row, likely_hash_single_row); }
inline bool likely_is_single_frame(likely_hash hash) { return likely_get_bool(hash, likely_hash_single_frame); }
inline void likely_set_single_frame(likely_hash &hash, bool is_single_frame) { likely_set_bool(hash, is_single_frame, likely_hash_single_frame); }
inline bool likely_is_owner(likely_hash hash) { return likely_get_bool(hash, likely_hash_owner); }
inline void likely_set_owner(likely_hash &hash, bool is_owner) { likely_set_bool(hash, is_owner, likely_hash_owner); }
inline int  likely_reserved(likely_hash hash) { return likely_get(hash, likely_hash_reserved); }
inline void likely_set_reserved(likely_hash &hash, int reserved) { likely_set(hash, reserved, likely_hash_reserved); }

// Convenience functions for determining matrix size
inline likely_size likely_elements(const likely_matrix *m) { return m->channels * m->columns * m->rows * m->frames; }
inline likely_size likely_bytes(const likely_matrix *m) { return uint64_t(likely_depth(m->hash)) * uint64_t(likely_elements(m)) / uint64_t(8); }

// Convenience functions for default initializing a matrix
LIKELY_EXPORT void likely_matrix_initialize(likely_matrix *m, likely_hash hash = likely_hash_null, likely_size channels = 0, likely_size columns = 0, likely_size rows = 0, likely_size frames = 0, uint8_t *data = NULL);

// Functions for allocating and freeing matrix data
LIKELY_EXPORT void likely_clone(const likely_matrix *m, likely_matrix *n);
LIKELY_EXPORT void likely_allocate(likely_matrix *m);
LIKELY_EXPORT void likely_free(likely_matrix *m);

// OpenCV Interface
struct CvMat;
LIKELY_EXPORT int likely_cvmat_type(likely_hash hash, int channels = 1); // Returns cv::Mat::type()
LIKELY_EXPORT void likely_to_cvmat(const likely_matrix *m, CvMat *cvMat); // Does not copy data
LIKELY_EXPORT void likely_from_cvmat(const CvMat *cvMat, likely_matrix *m); // Does not copy data

// Matrix I/O
LIKELY_EXPORT void likely_read(const char *file, likely_matrix *m);
LIKELY_EXPORT void likely_write(const likely_matrix *m, const char *file);

// Convenience functions for debugging; by convention c = channel, x = column, y = row, t = frame
LIKELY_EXPORT double likely_element(const likely_matrix *m, likely_size c = 0, likely_size x = 0, likely_size y = 0, likely_size t = 0);
LIKELY_EXPORT void likely_set_element(likely_matrix *m, double value, likely_size c = 0, likely_size x = 0, likely_size y = 0, likely_size t = 0);
LIKELY_EXPORT const char *likely_hash_to_string(likely_hash h); // Pointer guaranteed until the next call to this function
LIKELY_EXPORT likely_hash likely_string_to_hash(const char *str);
LIKELY_EXPORT void likely_print_matrix(const likely_matrix *m);
LIKELY_EXPORT bool likely_assert(bool condition, const char *format, ...); // Returns condition
LIKELY_EXPORT void likely_dump(); // Print LLVM module contents to stderr

// If set, likely_assert will call this function instead of aborting on an error
typedef void (*likely_error_callback)(const char *error_message);
LIKELY_EXPORT void likely_set_error_callback(likely_error_callback error_callback = NULL);

// Helper library functions; you shouldn't call these directly
typedef const char *likely_description;
typedef uint8_t likely_arity;
typedef void (*likely_nullary_kernel)(likely_matrix *dst, likely_size start, likely_size stop);
typedef void (*likely_unary_kernel)(const likely_matrix *src, likely_matrix *dst, likely_size start, likely_size stop);
typedef void (*likely_binary_kernel)(const likely_matrix *srcA, const likely_matrix *srcB, likely_matrix *dst, likely_size start, likely_size stop);
typedef void (*likely_ternary_kernel)(const likely_matrix *srcA, const likely_matrix *srcB, const likely_matrix *srcC, likely_matrix *dst, likely_size start, likely_size stop);
LIKELY_EXPORT void *likely_make_function(likely_description description, likely_arity arity, const likely_matrix *src, ...);
LIKELY_EXPORT void *likely_make_allocation(likely_description description, likely_arity arity, const likely_matrix *src, ...);
LIKELY_EXPORT void *likely_make_kernel(likely_description description, likely_arity arity, const likely_matrix *src, ...);
LIKELY_EXPORT void likely_parallel_dispatch(void *kernel, likely_arity arity, likely_size start, likely_size stop, likely_matrix *src, ...);

// Core library functions
typedef void (*likely_nullary_function)(likely_matrix *dst);
inline likely_nullary_function likely_make_nullary_function(likely_description description)
    { return (likely_nullary_function)likely_make_function(description, 0, NULL); }

typedef void (*likely_unary_function)(const likely_matrix *src, likely_matrix *dst);
inline likely_unary_function likely_make_unary_function(likely_description description, const likely_matrix *src)
    { return (likely_unary_function)likely_make_function(description, 1, src, NULL); }

typedef void (*likely_binary_function)(const likely_matrix *srcA, const likely_matrix *srcB, likely_matrix *dst);
inline likely_binary_function likely_make_binary_function(likely_description description, const likely_matrix *srcA, const likely_matrix *srcB)
    { return (likely_binary_function)likely_make_function(description, 2, srcA, srcB, NULL); }

typedef void (*likely_ternary_function)(const likely_matrix *srcA, const likely_matrix *srcB, const likely_matrix *srcC, likely_matrix *dst);
inline likely_ternary_function likely_make_ternary_function(likely_description description, const likely_matrix *srcA, const likely_matrix *srcB, const likely_matrix *srcC)
    { return (likely_ternary_function)likely_make_function(description, 3, srcA, srcB, srcC, NULL); }

#ifdef __cplusplus
}
#endif

#endif // LIKELY_H
