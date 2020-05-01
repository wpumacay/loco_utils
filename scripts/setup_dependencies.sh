#!/usr/bin/env bash

#### # using pybind11 from upstream (pybind)
#### GIT_DEPS_REPO=(pybind11 spdlog)
#### GIT_DEPS_USER=(pybind gabime)
#### GIT_DEPS_BRANCH=(master v1.x)

# using pybind11 from RobotLocomotion fork
GIT_DEPS_REPO=(pybind11 spdlog)
GIT_DEPS_USER=(RobotLocomotion gabime)
GIT_DEPS_BRANCH=(drake v1.x)

for i in {0..1}
do
    USER=${GIT_DEPS_USER[$i]}
    REPO=${GIT_DEPS_REPO[$i]}
    BRANCH=${GIT_DEPS_BRANCH[$i]}
    URL=https://github.com/${USER}/${REPO}
    if [ ! -d "ext/${GIT_DEPS_REPO[$i]}" ]
    then
        echo "===> Cloning ${USER}/${REPO} @ github - ${BRANCH} branch"
        git clone --branch=${BRANCH} ${URL} ext/${REPO}
    else
        echo "===> Repository ${USER}/${REPO} @ github already checked out"
    fi
done