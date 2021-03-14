load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_rust//rust:defs.bzl", "rust_static_library")
load("@proxy_wasm_rust_nullvm//bazel:proxy_wasm_symbols.bzl", "prefix_proxy_wasm_symbols")

package(default_visibility = ["//visibility:public"])

rust_static_library(
    name = "rust_plugin_lib",
    srcs = [
        "plugin.rs",
    ],
    edition = "2018",
    deps = [
        "@proxy_wasm_rust_sdk//:proxy_wasm",
        "@proxy_wasm_rust_sdk//bazel/cargo:log",
    ],
)

prefix_proxy_wasm_symbols(
    src = ":rust_plugin_lib",
    out = "plugin_lib.a",
    prefix = "my_plugin_",
)

cc_library(
    name = "lib",
    srcs = [
        "plugin.cc",
        "plugin_lib.a",
    ],
    linkstatic = True,
    deps = [
        "@proxy_wasm_cpp_host//:include",
        "@proxy_wasm_rust_nullvm//:include",
    ],
    alwayslink = True,
)
