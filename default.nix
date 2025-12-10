{
  cmake,
  lib,
  stdenv,
  kcli,
  ktl,
  doCheck ? false,
}:
stdenv.mkDerivation {
  name = "ktest";
  src = lib.cleanSource ./.;
  inherit doCheck;

  nativeBuildInputs = [ cmake ];

  buildInputs = [
    (kcli.override { inherit stdenv; })
    (ktl.override { inherit stdenv; })
  ];

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
