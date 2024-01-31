FROM localhost/cpp-build:latest AS build
ENTRYPOINT ["sh", "-c", "source /opt/rh/devtoolset-8/enable && \"$@\"", "-s"]
WORKDIR /home/zc/base/
ENV BUILD_DIR=/home/zc/base/ \
  THIRD_PARTY_PREFIX=/usr/src/app/third_party/ \
  RUN_DIR=/home/zc/base/

RUN echo $PWD
COPY . ${BUILD_DIR}

# go to the the connectivity direction and build it.
RUN source /opt/rh/devtoolset-8/enable \
  && export NUM_CPU=$(lscpu | grep -E '^CPU\(s\)' | cut -d':' -f2 | sed -e 's/^[[:space:]]*//') \
  && echo "using ${NUM_CPU} CPUs to build" \ 
  && ln -s ${THIRD_PARTY_PREFIX} ${BUILD_DIR}/third_party \
  && cd ${BUILD_DIR} && mkdir build && cd build \
  && cmake3 .. && make -j${NUM_CPU} base

#FROM localhost/cpp-build:latest AS run
#ENV THIRD_PARTY_PREFIX=/usr/src/app/third_party \
#  BUILD_DIR=/home/zc/base \
#  RUN_DIR=/home/zc/base
#
#COPY --from=build ${BUILD_DIR}/build/base ${RUN_DIR}/bin/base
##COPY --from=build ${BUILD_DIR}/config ${RUN_DIR}/config
CMD ${RUN_DIR}/bin/base
