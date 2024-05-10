default: build

build:
	@bazel build \
		//cmd:gelada \
		//scripts/cmd/install:gelada \
		//scripts/cmd/uninstall:gelada

install:
	@bazel-bin/scripts/cmd/install/gelada

uninstall:
	@bazel-bin/scripts/cmd/uninstall/gelada
