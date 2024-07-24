{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    clang_17
    lldb
    clang-tools
    cmake
    valgrind
    ninja
  ];
}
