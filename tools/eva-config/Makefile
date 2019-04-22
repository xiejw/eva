eva-config: ${BIN}/eva-config

${BIN}/eva-config: tools/eva-config/main.cpp
	mkdir -p ${BIN} && \
		${CXX} -Wall -std=c++14 -I. -o $@ $<


