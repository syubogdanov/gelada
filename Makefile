default: build

build:
	bazel build //cmd:gelada
	bazel build //scripts/cmd/install:gelada
	bazel build //scripts/cmd/uninstall:gelada

install:
	@bazel-bin/scripts/cmd/install/gelada

uninstall:
	@bazel-bin/scripts/cmd/uninstall/gelada
