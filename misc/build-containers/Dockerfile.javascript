ARG img_version
FROM pandemonium-fedora:${img_version}

ENV EMSCRIPTEN_VERSION=3.1.18

RUN dnf -y install --setopt=install_weak_deps=False \
      java-openjdk && \
    git clone --branch ${EMSCRIPTEN_CLASSICAL} --progress https://github.com/emscripten-core/emsdk emsdk && \
    emsdk/emsdk install ${EMSCRIPTEN_CLASSICAL} && \
    emsdk/emsdk activate ${EMSCRIPTEN_CLASSICAL}

CMD /bin/bash
