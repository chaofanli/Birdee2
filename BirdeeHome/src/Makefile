

CUR_DIR=$(shell pwd)
PARENT_DIR=$(shell dirname $(CUR_DIR))
BLIB_DIR=$(PARENT_DIR)/blib
BIN=$(PARENT_DIR)/bin
LIB=$(PARENT_DIR)/lib

all: ${BLIB_DIR}/birdee.o ${BLIB_DIR}/variant.o ${BLIB_DIR}/list.o ${BLIB_DIR}/hash_map.o ${BLIB_DIR}/hash_set.o ${LIB} ${BLIB_DIR}/tuple.o ${BLIB_DIR}/fmt.o ${BLIB_DIR}/vector.o ${BLIB_DIR}/queue.o ${BLIB_DIR}/stack.o ${BLIB_DIR}/unsafe.o

print-%  : ; @echo $* = $($*)

${LIB}:
	rm -f $(PARENT_DIR)/lib
	ln -s $(PARENT_DIR)/../lib ${LIB}

${BIN}:
	rm -f $(PARENT_DIR)/bin
	ln -s $(PARENT_DIR)/../bin ${BIN}

${BLIB_DIR}:
	mkdir -p ${BLIB_DIR}

${BLIB_DIR}/birdee.o: birdee.txt ${BLIB_DIR} ${BIN}
	$(BIN)/birdeec --corelib -i birdee.txt -o $(BLIB_DIR)/birdee.o

${BLIB_DIR}/list.o: list.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i list.txt -o $(BLIB_DIR)/list.o

${BLIB_DIR}/hash_map.o: hash_map.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i hash_map.txt -o $(BLIB_DIR)/hash_map.o

${BLIB_DIR}/hash_set.o: hash_set.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i hash_set.txt -o $(BLIB_DIR)/hash_set.o

${BLIB_DIR}/tuple.o: tuple.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i tuple.txt -o $(BLIB_DIR)/tuple.o

${BLIB_DIR}/fmt.o: fmt.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i fmt.txt -o $(BLIB_DIR)/fmt.o

${BLIB_DIR}/vector.o: vector.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i vector.txt -o $(BLIB_DIR)/vector.o

${BLIB_DIR}/queue.o: queue.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i queue.txt -o $(BLIB_DIR)/queue.o

${BLIB_DIR}/stack.o: stack.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i stack.txt -o $(BLIB_DIR)/stack.o

${BLIB_DIR}/unsafe.o: unsafe.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i unsafe.txt -o $(BLIB_DIR)/unsafe.o

${BLIB_DIR}/variant.o: variant.txt ${BLIB_DIR}/birdee.o
	$(BIN)/birdeec -i variant.txt -o $(BLIB_DIR)/variant.o

clean:
	rm -rf ${BLIB_DIR}