ARG img_version
FROM pandemonium-fedora:${img_version}

ENV EMSCRIPTEN_VERSION=3.1.14

RUN dnf -y install --setopt=install_weak_deps=False \
      java-openjdk && \
    git clone --branch ${EMSCRIPTEN_VERSION} --progress https://github.com/emscripten-core/emsdk emsdk && \
    emsdk/emsdk install ${EMSCRIPTEN_VERSION} && \
    emsdk/emsdk activate ${EMSCRIPTEN_VERSION}

CMD /bin/bash
