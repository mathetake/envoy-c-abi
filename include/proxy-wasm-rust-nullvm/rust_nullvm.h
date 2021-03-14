#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "include/proxy-wasm/null_vm_plugin.h"
#include "include/proxy-wasm/wasm.h"
#include "include/proxy-wasm/exports.h"

#define DECLARE_OVERRIDE(_t) void getFunction(std::string_view function_name, _t *f) override;
#define WS(_x) Word(static_cast<uint64_t>(_x))
#define WR(_x) Word(reinterpret_cast<uint64_t>(_x))

#define DEFINE_NULLVM_RUST_PLUGIN(plugin_name)                                                     \
  namespace proxy_wasm {                                                                           \
  namespace rust_nullvm {                                                                          \
  namespace plugin_name {                                                                          \
  /* Exported to Rust*/                                                                            \
  extern "C" {                                                                                     \
  inline WasmResult wordToWasmResult(Word w) { return static_cast<WasmResult>(w.u64_); }           \
                                                                                                   \
  void *dlsym(void *handle, const char *symbol) { /* exported somewhere in the static lib of Rust  \
                                                     but we should not allow this to be used. */   \
    return nullptr;                                                                                \
  }                                                                                                \
  WasmResult plugin_name##_proxy_get_configuration(const char **configuration_ptr,                 \
                                                   size_t *configuration_size) {                   \
    return wordToWasmResult(exports::get_configuration(current_context_, WR(configuration_ptr),    \
                                                       WR(configuration_size)));                   \
  }                                                                                                \
                                                                                                   \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_status(uint32_t *code_ptr, const char **ptr, size_t *size) {  \
    return wordToWasmResult(                                                                       \
        exports::get_status(current_context_, WR(code_ptr), WR(ptr), WR(size)));                   \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_log(LogLevel level, const char *logMessage, size_t messageSize) { \
    return wordToWasmResult(                                                                       \
        exports::log(current_context_, WS(level), WR(logMessage), WS(messageSize)));               \
  }                                                                                                \
  WasmResult plugin_name##_proxy_get_log_level(LogLevel *level) {                                  \
    return wordToWasmResult(exports::get_log_level(current_context_, WR(level)));                  \
  }                                                                                                \
  WasmResult plugin_name##_proxy_set_tick_period_milliseconds(uint32_t millisecond) {              \
    return wordToWasmResult(                                                                       \
        exports::set_tick_period_milliseconds(current_context_, Word(millisecond)));               \
  }                                                                                                \
  WasmResult plugin_name##_proxy_get_current_time_nanoseconds(uint64_t *result) {                  \
    return wordToWasmResult(exports::get_current_time_nanoseconds(current_context_, WR(result)));  \
  }                                                                                                \
  WasmResult plugin_name##_proxy_get_property(const char *path_ptr, size_t path_size,              \
                                              const char **value_ptr_ptr,                          \
                                              size_t *value_size_ptr) {                            \
    return wordToWasmResult(exports::get_property(current_context_, WR(path_ptr), WS(path_size),   \
                                                  WR(value_ptr_ptr), WR(value_size_ptr)));         \
  }                                                                                                \
  WasmResult plugin_name##_proxy_set_property(const char *key_ptr, size_t key_size,                \
                                              const char *value_ptr, size_t value_size) {          \
    return wordToWasmResult(exports::set_property(current_context_, WR(key_ptr), WS(key_size),     \
                                                  WR(value_ptr), WS(value_size)));                 \
  }                                                                                                \
  WasmResult plugin_name##_proxy_continue_request() {                                              \
    return wordToWasmResult(exports::continue_request(current_context_));                          \
  }                                                                                                \
  WasmResult plugin_name##_proxy_continue_response() {                                             \
    return wordToWasmResult(exports::continue_response(current_context_));                         \
  }                                                                                                \
  WasmResult plugin_name##_proxy_continue_stream(WasmStreamType stream_type) {                     \
    return wordToWasmResult(exports::continue_stream(current_context_, WS(stream_type)));          \
  }                                                                                                \
  WasmResult plugin_name##_proxy_close_stream(WasmStreamType stream_type) {                        \
    return wordToWasmResult(exports::close_stream(current_context_, WS(stream_type)));             \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_send_local_response(                                              \
      uint32_t response_code, const char *response_code_details_ptr,                               \
      size_t response_code_details_size, const char *body_ptr, size_t body_size,                   \
      const char *additional_response_header_pairs_ptr,                                            \
      size_t additional_response_header_pairs_size, int32_t grpc_status) {                         \
    return wordToWasmResult(exports::send_local_response(                                          \
        current_context_, WS(response_code), WR(response_code_details_ptr),                        \
        WS(response_code_details_size), WR(body_ptr), WS(body_size),                               \
        WR(additional_response_header_pairs_ptr), WS(additional_response_header_pairs_size),       \
        WS(grpc_status)));                                                                         \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_clear_route_cache() {                                             \
    return wordToWasmResult(exports::clear_route_cache(current_context_));                         \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_shared_data(const char *key_ptr, size_t key_size,             \
                                                 const char **value_ptr, size_t *value_size,       \
                                                 uint32_t *cas) {                                  \
    return wordToWasmResult(exports::get_shared_data(current_context_, WR(key_ptr), WS(key_size),  \
                                                     WR(value_ptr), WR(value_size), WR(cas)));     \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_set_shared_data(const char *key_ptr, size_t key_size,             \
                                                 const char *value_ptr, size_t value_size,         \
                                                 uint32_t cas) {                                   \
    return wordToWasmResult(exports::set_shared_data(current_context_, WR(key_ptr), WS(key_size),  \
                                                     WR(value_ptr), WS(value_size), WS(cas)));     \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_register_shared_queue(const char *queue_name_ptr,                 \
                                                       size_t queue_name_size, uint32_t *token) {  \
    return wordToWasmResult(exports::register_shared_queue(current_context_, WR(queue_name_ptr),   \
                                                           WS(queue_name_size), WR(token)));       \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_resolve_shared_queue(const char *vm_id_ptr, size_t vm_id_size,    \
                                                      const char *queue_name_ptr,                  \
                                                      size_t queue_name_size, uint32_t *token) {   \
    return wordToWasmResult(exports::resolve_shared_queue(current_context_, WR(vm_id_ptr),         \
                                                          WS(vm_id_size), WR(queue_name_ptr),      \
                                                          WS(queue_name_size), WR(token)));        \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_dequeue_shared_queue(uint32_t token, const char **data_ptr,       \
                                                      size_t *data_size) {                         \
    return wordToWasmResult(                                                                       \
        exports::dequeue_shared_queue(current_context_, WS(token), WR(data_ptr), WR(data_size)));  \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_enqueue_shared_queue(uint32_t token, const char *data_ptr,        \
                                                      size_t data_size) {                          \
    return wordToWasmResult(                                                                       \
        exports::enqueue_shared_queue(current_context_, WS(token), WR(data_ptr), WS(data_size)));  \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_buffer_bytes(WasmBufferType type, size_t start,               \
                                                  size_t length, const char **ptr, size_t *size) { \
    return wordToWasmResult(exports::get_buffer_bytes(current_context_, WS(type), WS(start),       \
                                                      WS(length), WR(ptr), WR(size)));             \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_buffer_status(WasmBufferType type, size_t *length_ptr,        \
                                                   uint32_t *flags_ptr) {                          \
    return wordToWasmResult(                                                                       \
        exports::get_buffer_status(current_context_, WS(type), WR(length_ptr), WR(flags_ptr)));    \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_set_buffer_bytes(WasmBufferType type, size_t start,               \
                                                  size_t length, const char *data, size_t size) {  \
    return wordToWasmResult(exports::set_buffer_bytes(current_context_, WS(type), WS(start),       \
                                                      WS(length), WR(data), WS(size)));            \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_add_header_map_value(WasmHeaderMapType type, const char *key_ptr, \
                                                      size_t key_size, const char *value_ptr,      \
                                                      size_t value_size) {                         \
    return wordToWasmResult(exports::add_header_map_value(                                         \
        current_context_, WS(type), WR(key_ptr), WS(key_size), WR(value_ptr), WS(value_size)));    \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_header_map_value(WasmHeaderMapType type, const char *key_ptr, \
                                                      size_t key_size, const char **value_ptr,     \
                                                      size_t *value_size) {                        \
    return wordToWasmResult(exports::get_header_map_value(                                         \
        current_context_, WS(type), WR(key_ptr), WS(key_size), WR(value_ptr), WR(value_size)));    \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_header_map_pairs(WasmHeaderMapType type, const char **ptr,    \
                                                      size_t *size) {                              \
    return wordToWasmResult(                                                                       \
        exports::get_header_map_pairs(current_context_, WS(type), WR(ptr), WR(size)));             \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_set_header_map_pairs(WasmHeaderMapType type, const char *ptr,     \
                                                      size_t size) {                               \
    return wordToWasmResult(                                                                       \
        exports::set_header_map_pairs(current_context_, WS(type), WR(ptr), WS(size)));             \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_replace_header_map_value(WasmHeaderMapType type,                  \
                                                          const char *key_ptr, size_t key_size,    \
                                                          const char *value_ptr,                   \
                                                          size_t value_size) {                     \
    return wordToWasmResult(exports::replace_header_map_value(                                     \
        current_context_, WS(type), WR(key_ptr), WS(key_size), WR(value_ptr), WS(value_size)));    \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_remove_header_map_value(WasmHeaderMapType type,                   \
                                                         const char *key_ptr, size_t key_size) {   \
    return wordToWasmResult(                                                                       \
        exports::remove_header_map_value(current_context_, WS(type), WR(key_ptr), WS(key_size)));  \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_header_map_size(WasmHeaderMapType type, size_t *size) {       \
    return wordToWasmResult(exports::get_header_map_size(current_context_, WS(type), WR(size)));   \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_http_call(const char *uri_ptr, size_t uri_size,                   \
                                           void *header_pairs_ptr, size_t header_pairs_size,       \
                                           const char *body_ptr, size_t body_size,                 \
                                           void *trailer_pairs_ptr, size_t trailer_pairs_size,     \
                                           uint32_t timeout_milliseconds, uint32_t *token_ptr) {   \
    return wordToWasmResult(exports::http_call(                                                    \
        current_context_, WR(uri_ptr), WS(uri_size), WR(header_pairs_ptr), WS(header_pairs_size),  \
        WR(body_ptr), WS(body_size), WR(trailer_pairs_ptr), WS(trailer_pairs_size),                \
        WS(timeout_milliseconds), WR(token_ptr)));                                                 \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_define_metric(MetricType type, const char *name_ptr,              \
                                               size_t name_size, uint32_t *metric_id) {            \
    return wordToWasmResult(exports::define_metric(current_context_, WS(type), WR(name_ptr),       \
                                                   WS(name_size), WR(metric_id)));                 \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_increment_metric(uint32_t metric_id, int64_t offset) {            \
    return wordToWasmResult(exports::increment_metric(current_context_, WS(metric_id), offset));   \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_record_metric(uint32_t metric_id, uint64_t value) {               \
    return wordToWasmResult(exports::record_metric(current_context_, WS(metric_id), value));       \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_get_metric(uint32_t metric_id, uint64_t *value) {                 \
    return wordToWasmResult(exports::get_metric(current_context_, WS(metric_id), WR(value)));      \
  }                                                                                                \
                                                                                                   \
  WasmResult plugin_name##_proxy_set_effective_context(uint32_t context_id) {                      \
    return wordToWasmResult(exports::set_effective_context(current_context_, WS(context_id)));     \
  }                                                                                                \
  WasmResult plugin_name##_proxy_done() {                                                          \
    return wordToWasmResult(exports::done(current_context_));                                      \
  }                                                                                                \
  }                                                                                                \
                                                                                                   \
                                                                                                   \
  /* Exported by Rust */                                                                           \
  /* TODO: Some of them are not implemented in Rust SDK */                                         \
  extern "C" {                                                                                     \
  /* WasmCallVoid<0> */                                                                            \
  void plugin_name##__start();                                                                     \
  /* WasmCallVoid<1> */                                                                            \
  void plugin_name##_proxy_on_tick(uint32_t);                                                      \
  void plugin_name##_proxy_on_delete(uint32_t);                                                    \
  void plugin_name##_proxy_on_log(uint32_t);                                                       \
  /* WasmCallVoid<2> */                                                                            \
  void plugin_name##_proxy_on_context_create(uint32_t, uint32_t);                                  \
  void plugin_name##_proxy_on_downstream_connection_close(uint32_t, uint32_t);                     \
  void plugin_name##_proxy_on_upstream_connection_close(uint32_t, uint32_t);                       \
  void plugin_name##_proxy_on_queue_ready(uint32_t, uint32_t);                                     \
                                                                                                   \
  /* WasmCallVoid<3> */                                                                            \
                                                                                                   \
  /* WasmCallVoid<5> */                                                                            \
  void plugin_name##_proxy_on_http_call_response(uint32_t, uint32_t, size_t, size_t, size_t);      \
                                                                                                   \
  /* WasmCallWord<1> */                                                                            \
  void *plugin_name##_proxy_on_memory_allocate(size_t);                                            \
  uint32_t plugin_name##_proxy_on_new_connection(uint32_t);                                        \
  bool plugin_name##_proxy_on_done(uint32_t);                                                      \
                                                                                                   \
  /* WasmCallWord<2> */                                                                            \
  bool plugin_name##_proxy_on_vm_start(uint32_t, size_t);                                          \
  bool plugin_name##_proxy_on_configure(uint32_t, size_t);                                         \
  uint32_t plugin_name##_proxy_on_request_trailers(uint32_t, size_t);                              \
  uint32_t plugin_name##_proxy_on_response_trailers(uint32_t, size_t);                             \
                                                                                                   \
  /* WasmCallWord<3> */                                                                            \
  uint32_t plugin_name##_proxy_on_downstream_data(uint32_t, size_t, bool);                         \
  uint32_t plugin_name##_proxy_on_upstream_data(uint32_t, size_t, bool);                           \
  uint32_t plugin_name##_proxy_on_request_body(uint32_t, size_t, bool);                            \
  uint32_t plugin_name##_proxy_on_response_body(uint32_t, size_t, bool);                           \
  uint32_t plugin_name##_proxy_on_request_headers(uint32_t, size_t);  /* ABI 0.1.0 */              \
  uint32_t plugin_name##_proxy_on_response_headers(uint32_t, size_t); /* ABI 0.1.0 */              \
  }                                                                                                \
                                                                                                   \
  class NullVmPluginImpl : public proxy_wasm::NullVmPlugin {                                       \
  public:                                                                                          \
    NullVmPluginImpl() {                                                                           \
      call_void_0_ = {                                                                             \
          {                                                                                        \
              "_start",                                                                            \
              [](ContextBase *context) {                                                           \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##__start();                                                            \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
      call_void_1_ = {{                                                                            \
                          "proxy_on_tick",                                                         \
                          [](ContextBase *context, Word context_id) {                              \
                            SaveRestoreContext saved_context(context);                             \
                            plugin_name##_proxy_on_tick(context_id);                               \
                          },                                                                       \
                      },                                                                           \
                      {                                                                            \
                          "proxy_on_log",                                                          \
                          [](ContextBase *context, Word context_id) {                              \
                            SaveRestoreContext saved_context(context);                             \
                            plugin_name##_proxy_on_log(context_id);                                \
                          },                                                                       \
                      },                                                                           \
                      {                                                                            \
                          "proxy_on_delete",                                                       \
                          [](ContextBase *context, Word context_id) {                              \
                            SaveRestoreContext saved_context(context);                             \
                            plugin_name##_proxy_on_delete(context_id);                             \
                          },                                                                       \
                      }};                                                                          \
      call_void_2_ = {                                                                             \
          {                                                                                        \
              "proxy_on_context_create",                                                           \
              [](ContextBase *context, Word context_id, Word parent_context_id) {                  \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##_proxy_on_context_create(context_id, parent_context_id);              \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_downstream_connection_close",                                              \
              [](ContextBase *context, Word context_id, Word peer_type) {                          \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##_proxy_on_downstream_connection_close(context_id, peer_type);         \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_upstream_connection_close",                                                \
              [](ContextBase *context, Word context_id, Word parent_context_id) {                  \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##_proxy_on_upstream_connection_close(context_id, parent_context_id);   \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_queue_ready",                                                              \
              [](ContextBase *context, Word context_id, Word token) {                              \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##_proxy_on_queue_ready(context_id, token);                             \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
      call_void_5_ = {                                                                             \
          {                                                                                        \
              "proxy_on_http_call_response",                                                       \
              [](ContextBase *context, Word context_id, Word token, Word headers, Word body_size,  \
                 Word trailers) {                                                                  \
                SaveRestoreContext saved_context(context);                                         \
                plugin_name##_proxy_on_http_call_response(context_id, token, headers, body_size,   \
                                                          trailers);                               \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
      call_word_1_ = {                                                                             \
          {                                                                                        \
              "proxy_on_memory_allocate",                                                          \
              [](ContextBase *context, Word size) -> Word {                                        \
                SaveRestoreContext saved_context(context);                                         \
                return reinterpret_cast<uint64_t>(plugin_name##_proxy_on_memory_allocate(size));   \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_new_connection",                                                           \
              [](ContextBase *context, Word context_id) -> Word {                                  \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_new_connection(context_id);                          \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_done",                                                                     \
              [](ContextBase *context, Word context_id) -> Word {                                  \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_done(context_id);                                    \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
      call_word_2_ = {                                                                             \
          {                                                                                        \
              "proxy_on_vm_start",                                                                 \
              [](ContextBase *context, Word context_id, Word configuration_size) -> Word {         \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_vm_start(context_id, configuration_size);            \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_configure",                                                                \
              [](ContextBase *context, Word context_id, Word configuration_size) -> Word {         \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_configure(context_id, configuration_size);           \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_request_trailers",                                                         \
              [](ContextBase *context, Word context_id, Word trailers) -> Word {                   \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_request_trailers(context_id, trailers);              \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_response_trailers",                                                        \
              [](ContextBase *context, Word context_id, Word trailers) -> Word {                   \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_response_trailers(context_id, trailers);             \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
      call_word_3_ = {                                                                             \
          {                                                                                        \
              "proxy_on_downstream_data",                                                          \
              [](ContextBase *context, Word context_id, Word body_buffer_length,                   \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_downstream_data(context_id, body_buffer_length,      \
                                                              end_of_stream);                      \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_upstream_data",                                                            \
              [](ContextBase *context, Word context_id, Word body_buffer_length,                   \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_upstream_data(context_id, body_buffer_length,        \
                                                            end_of_stream);                        \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
                                                                                                   \
              "proxy_on_request_headers", /* translate to v0.1.0 signature */                      \
              [](ContextBase *context, Word context_id, Word headers,                              \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_request_headers(context_id, headers);                \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_request_body",                                                             \
              [](ContextBase *context, Word context_id, Word body_buffer_length,                   \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_request_body(context_id, body_buffer_length,         \
                                                           end_of_stream);                         \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_response_headers", /* translate to v0.1.0 signature */                     \
              [](ContextBase *context, Word context_id, Word headers,                              \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_response_headers(context_id, headers);               \
              },                                                                                   \
          },                                                                                       \
          {                                                                                        \
              "proxy_on_response_body",                                                            \
              [](ContextBase *context, Word context_id, Word body_buffer_length,                   \
                 Word end_of_stream) -> Word {                                                     \
                SaveRestoreContext saved_context(context);                                         \
                return plugin_name##_proxy_on_response_body(context_id, body_buffer_length,        \
                                                            end_of_stream);                        \
              },                                                                                   \
          },                                                                                       \
      };                                                                                           \
    };                                                                                             \
    ~NullVmPluginImpl() override = default;                                                        \
                                                                                                   \
    FOR_ALL_WASM_VM_EXPORTS(DECLARE_OVERRIDE)                                                      \
                                                                                                   \
  private:                                                                                         \
    std::unordered_map<std::string, proxy_wasm::WasmCallVoid<0>> call_void_0_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallVoid<1>> call_void_1_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallVoid<2>> call_void_2_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallVoid<3>> call_void_3_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallVoid<5>> call_void_5_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallWord<1>> call_word_1_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallWord<2>> call_word_2_;                     \
    std::unordered_map<std::string, proxy_wasm::WasmCallWord<3>> call_word_3_;                     \
  };                                                                                               \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallVoid<0> *f) {         \
    auto func = call_void_0_.find(std::string(function_name));                                     \
    if (func != call_void_0_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallVoid<1> *f) {         \
    auto func = call_void_1_.find(std::string(function_name));                                     \
    if (func != call_void_1_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallVoid<2> *f) {         \
    auto func = call_void_2_.find(std::string(function_name));                                     \
    if (func != call_void_2_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallVoid<3> *f) {         \
    auto func = call_void_3_.find(std::string(function_name));                                     \
    if (func != call_void_3_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallVoid<5> *f) {         \
    auto func = call_void_5_.find(std::string(function_name));                                     \
    if (func != call_void_5_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallWord<1> *f) {         \
    auto func = call_word_1_.find(std::string(function_name));                                     \
    if (func != call_word_1_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallWord<2> *f) {         \
    auto func = call_word_2_.find(std::string(function_name));                                     \
    if (func != call_word_2_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  void NullVmPluginImpl::getFunction(std::string_view function_name, WasmCallWord<3> *f) {         \
    auto func = call_word_3_.find(std::string(function_name));                                     \
    if (func != call_word_3_.end()) {                                                              \
      *f = func->second;                                                                           \
    };                                                                                             \
  }                                                                                                \
                                                                                                   \
  /* register the plugin */                                                                        \
  proxy_wasm::RegisterNullVmPluginFactory plugin_name(#plugin_name, [] {       \
    return std::make_unique<NullVmPluginImpl>();                                                   \
  });                                                                                              \
  }                                                                                                \
  }                                                                                                \
  }
