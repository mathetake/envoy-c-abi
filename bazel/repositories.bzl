load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def repositories(name = ""):
    _envoy_deps()
    _http_archives()

def _http_archives():
    http_archive(
        name = "bazel_compdb",
        strip_prefix = "bazel-compilation-database-0.4.5",
        urls = ["https://github.com/grailbio/bazel-compilation-database/archive/0.4.5.tar.gz"],
    )

    http_archive(
        name = "rules_cc",
        sha256 = "9d48151ea71b3e225adfb6867e6d2c7d0dce46cbdc8710d9a9a628574dfd40a0",
        strip_prefix = "rules_cc-818289e5613731ae410efb54218a4077fb9dbb03",
        urls = ["https://github.com/bazelbuild/rules_cc/archive/818289e5613731ae410efb54218a4077fb9dbb03.tar.gz"],
    )

    http_archive(
        name = "com_google_googletest",
        sha256 = "9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb",
        strip_prefix = "googletest-release-1.10.0",
        urls = ["https://github.com/google/googletest/archive/release-1.10.0.tar.gz"],
    )

    http_archive(
        name = "rules_rust",
        strip_prefix = "rules_rust-9aa49569b2b0dacecc51c05cee52708b7255bd98",
        url = "https://github.com/bazelbuild/rules_rust/archive/9aa49569b2b0dacecc51c05cee52708b7255bd98.tar.gz",
    )

    http_archive(
        name = "proxy_wasm_rust_sdk",
        strip_prefix = "proxy-wasm-rust-sdk-28a94df25659b2107b67a11df0112f8f6833558b",
        urls = ["https://github.com/proxy-wasm/proxy-wasm-rust-sdk/archive/28a94df25659b2107b67a11df0112f8f6833558b.tar.gz"],
    )

def _envoy_deps():
    native.new_local_repository(
        name = "envoy_build_config",
        path = "bazel/external/envoy",
        build_file_content = "",
    )

    http_archive(
        name = "envoy",
        sha256 = "55722bf7f1d6ac2d76527c880d1ff824ac146ac8fdbd67f6798ea557f1f27c92",
        strip_prefix = "envoy-50e81276fd8f109ba3a6134e790f65c1cc5bdec9",
        url = "https://github.com/envoyproxy/envoy/archive/50e81276fd8f109ba3a6134e790f65c1cc5bdec9.tar.gz",
    )
