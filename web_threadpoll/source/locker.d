default-source/locker.d:
source/locker.o: source/locker.cpp include/locker.h
	@echo "         CXX   $(notdir $<)"
	@$(CXX) $(DECODER_CFLAG) $(DECODER_DFLAG) $(DECODER_INCLUDES) -Isource/ -L -lstdc++ -lrt  -ldl -lm -pthread -lpython2.7  -L/lib64/ -c $(filter %.cpp, $^) -o $(filter %.o, $@)
