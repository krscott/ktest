{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";

    kcli = {
      url = "github:krscott/kcli";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      kcli,
    }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
    in
    flake-utils.lib.eachSystem supportedSystems (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

        # Final derivation including any overrides made to output package
        inherit (self.packages.${system}) ktest ktest-gcc;

        devPkgs =
          with pkgs;
          [
            shfmt
            cmake-format
            clang-tools # NOTE: clang-tools must come before clang
            clang
          ]
          ++ ktest.buildInputs;

        mkApp = text: {
          type = "app";
          program = pkgs.lib.getExe (
            pkgs.writeShellApplication {
              name = "app";
              runtimeInputs = devPkgs;
              inherit text;
            }
          );
        };
      in
      {
        packages = {
          ktest = pkgs.callPackage ./. {
            inherit (kcli.packages.${system}) kcli;
            stdenv = pkgs.clangStdenv;
          };

          ktest-gcc = ktest.override {
            inherit (pkgs) stdenv;
          };

          ktest-win = ktest.override {
            inherit (pkgs.pkgsCross.mingwW64) stdenv;
          };

          default = ktest;

          ktest-test = ktest.override {
            doCheck = true;
          };
          ktest-gcc-test = ktest-gcc.override {
            doCheck = true;
          };
        };

        devShells = {
          default = pkgs.mkShell {
            inputsFrom = [ ktest ];
            nativeBuildInputs = devPkgs;
            shellHook = ''
              source dev_shell.sh
            '';
          };
        };

        apps = {
          format = mkApp ''
            ./format.sh
          '';
        };

        formatter = pkgs.nixfmt-rfc-style;
      }
    );
}
