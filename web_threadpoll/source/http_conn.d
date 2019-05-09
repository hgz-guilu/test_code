default-source/http_conn.d:
source/http_conn.o: source/http_conn.cpp include/http_conn.h include/locker.h \
 include/url_handle/url_handler.h
	@echo "         CXX   $(notdir $<)"
	@$(CXX) $(DECODER_CFLAG) $(DECODER_DFLAG) $(DECODER_INCLUDES) -Isource/ -L -lstdc++ -lrt  -ldl -lm -pthread -lpython2.7  -L/lib64/ -c $(filter %.cpp, $^) -o $(filter %.o, $@)
