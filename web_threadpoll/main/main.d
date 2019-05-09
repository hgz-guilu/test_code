default-main/main.d:
main/main.o: main/main.cpp include/locker.h include/threadpool.h \
 include/locker.h include/http_conn.h
	@echo "         CXX   $(notdir $<)"
	@$(CXX) $(DECODER_CFLAG) $(DECODER_DFLAG) $(DECODER_INCLUDES) -Imain/ -L -lstdc++ -lrt  -ldl -lm -pthread -lpython2.7  -L/lib64/ -c $(filter %.cpp, $^) -o $(filter %.o, $@)
