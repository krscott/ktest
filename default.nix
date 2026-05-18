{
  cmake,
  lib,
  stdenv,
  doCheck ? false,
}:
stdenv.mkDerivation {
  name = "ktest";
  src = lib.cleanSource ./.;
  inherit doCheck;

  nativeBuildInputs = [ cmake ];

  buildInputs = [ ];

  configurePhase = ''
    cmake -B build
  '';

  buildPhase = ''
    cmake --build build
  '';

  installPhase = ''
    mkdir -p "$out/include"
    cp -r include "$out"
  '';

  checkPhase = ''
    (
      cd build/test
      ctest --output-on-failure
    )
  '';
}
