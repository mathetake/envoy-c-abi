load("@rules_cc//cc:defs.bzl", "cc_library")
load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_binary",
)

package(default_visibility = ["//visibility:public"])

envoy_cc_binary(
    name = "envoy",
    repository = "@envoy",
    deps = [
        "//examples/my-plugin:lib",
        "@envoy//source/exe:envoy_main_entry_lib",
    ],
)

cc_library(
    name = "include",
    hdrs = glob(["include/proxy-wasm-rust-nullvm/**/*.h"]),
    deps = [
        "@proxy_wasm_cpp_host//:include",
    ],
)
