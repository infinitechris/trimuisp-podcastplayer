#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
image="${IMAGE:-rockbox-toolchain:bookworm}"

if ! docker image inspect "$image" >/dev/null 2>&1; then
  docker build -f "$repo_root/tools/Dockerfile.rockbox" -t "$image" "$repo_root"
fi

docker run --rm \
  -v "$repo_root:/workspace" \
  -w /workspace \
  -u "$(id -u):$(id -g)" \
  -e LANG=C.UTF-8 \
  -e LC_ALL=C.UTF-8 \
  "$image" bash -lc 'mkdir -p build && cd build && ../tools/configure --target=210 --type=n && make -j"$(nproc)"'
