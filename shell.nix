{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "CString";
  nativeBuildInputs = with pkgs; [
    clang
    cmake
  ];

  shellHook = ''
    export PATH=$PATH;${pkgs.clang}/bin
  '';
}
