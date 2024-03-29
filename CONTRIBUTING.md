# Contributing

This repository is still under development, and we welcome user engagement to
find bugs, resolve issues and add useful features.  

Before contributing to this repository, please first discuss the change you wish to make
by using the repository [Discussions](https://github.com/epfl-lasa/control-libraries/discussions),
opening an [issue](https://github.com/epfl-lasa/control-libraries/issues),
or by contacting the maintainers of this repository directly. 


## Development Environment

Our development and testing workflow uses a Docker container to host the project build dependencies
which an IDE then accesses remotely via SSH.

The following section describes the configuration steps to use this workflow.
Of course, contributors may use whatever development environment they prefer,
as long as they adhere to the overall contribution guidelines. 


### Configuring the development environment

Prerequisites: Install [Docker](https://docs.docker.com/get-docker/) and [CLion IDE](https://www.jetbrains.com/clion/download).

Step 1: Build and run the development container server with `./dev-server.sh`. The script requires access
to your public RSA key file. You may additionally specify a custom port.
Run `./dev-server.sh --help` for more details.

Step 2: If not already done, [create a remote toolchain in CLion](https://www.jetbrains.com/help/clion/remote-projects-support.html#remote-toolchain)
using the following credentials:
 - Host: `127.0.0.1`
 - Port: `2222` (or custom specified port)
 - User name: `remote`
 - Authentication type: `Key pair`
 
Step 3: If not already done, [create a CMake profile that uses the remote toolchain](https://www.jetbrains.com/help/clion/remote-projects-support.html#CMakeProfile).

Step 4: [Resync the header search paths](https://www.jetbrains.com/help/clion/remote-projects-support.html#resync).
Repeat this step any time you change the compiler or project dependencies.
Optionally you can enable automatic synchronization.

Step 5: [Select the remote CMake profile](https://www.jetbrains.com/help/clion/remote-projects-support.html#WorkWithRemote)
to build, run and debug library and test targets entirely with the remote toolchain.

## Style guide

This project primarily follows the [LASA C++ Style Guide](https://wiki.epfl.ch/lasa/cpp-style-guide),
except for the use of `snake_case` instead of `lowerCamelCase` for function and variable naming.

Doxygen headers are also required for the public API.

## Unit Testing

Every new feature should have associated unit tests.
This not only helps reduce the likelihood of bugs in the library,
but also helps the author and reviewers of a feature to understand what the expected
behaviour really is.

Refer to the [Google Test Primer](https://github.com/google/googletest/blob/master/docs/primer.md)
for a quickstart guide to the testing framework.

If you are using a properly integrated development environment, you can run and debug the tests
for each module locally to check the behaviour. 

You may also use the script `./build-test.sh` to build the project libraries and run all tests
within a Docker container. Because this process rebuilds the project from scratch, each evaluation may
take several minutes. 


## Pull Request Process

1. Ensure your code follows the style guide and is portable; remove any references to local paths or files.
2. Check that the new changes are appropriately covered by unit tests.
3. Document the header files and public functions with doxygen comments, and update any relevant README.md
   or documentation files with details of changes to the interface.
4. Update the [changelog](CHANGELOG.md) with your feature / fix / improvement in the "Upcoming changes" section.
5. Open a pull request into the `develop` branch. Write a meaningful title and description for the PR to make it
   clear what changes you have made, why you have made them, and how you have tested the changes.
6. You may merge the pull request into `develop` once you have the sign-off of one other developer and all CI tests pass.
   Always use the "Squash and Merge" option to ensure your changes are contained within a single commit, maintaining
   a linear git history. If unsure, you may request another reviewer to merge it for you.


## Release strategy

The `main` branch is updated from `develop` at regular release intervals using traditional semantic versioning (major.minor.patch).

The major release number should be incremented whenever there is a breaking change to the public API
(for example, when a previously existing function is deleted or renamed). Minor version numbers contain
general new features and improvements, while patch numbers represent simple and small fixes.

The `develop` branch is always considered to be a "release candidate" that contains only release-ready
code. If, at release time, there are features on `develop` that are considered unfinished or broken,
they can be marked as `EXPERIMENTAL` to exclude them from compilation.

At the time of release, a release branch should be made from development. In the release branch,
the project version number should be updated in the following locations:
- The [top-level CMakeLists](./source/CMakeLists.txt) 
- The [python bindings setup.py](./python/setup.py)
- The [clproto bindings CMakeLists](./protocol/clproto_cpp/CMakeLists.txt)
- The PROJECT_NUMBER in the [doxygen config](./doxygen/doxygen.conf)

In addition, the release branch should be used to finalize the [changelog](CHANGELOG.md), which includes
moving all content from the "Upcoming changes (in development)" header under a new header with the corresponding
release version.

Once the changes specific to the release have been approved, a linear GitFlow strategy is used to
merge this release branch into `main`, and then additionally squash and rebase the release branch back into `develop`.

View and join the full discussion surrounding release workflow and strategy here: \
https://github.com/epfl-lasa/control-libraries/discussions/77