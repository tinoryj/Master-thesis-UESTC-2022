CC = clang++
CFLAGS = -O3
LIBS = -lcrypto -lssl
INCLUDES =-I./
MAIN_OBJS = FeatureSpy chunking outputFeature processResult

all: FeatureSpy chunking outputFeature processResult

FeatureSpy: ./FeatureSpy.cpp
	$(CC) $(CFLAGS) -o FeatureSpy FeatureSpy.cpp $(INCLUDES) $(LIBS)
processResult: ./processResult.cpp
	$(CC) $(CFLAGS) -o processResult processResult.cpp $(INCLUDES) $(LIBS)
outputFeature: ./outputFeature.cpp
	$(CC) $(CFLAGS) -o outputFeature outputFeature.cpp $(INCLUDES) $(LIBS)
chunking: ./chunking.cpp
	$(shell ! test -d "Content" && mkdir Content)
	$(CC) $(CFLAGS) -o chunking chunking.cpp $(INCLUDES) $(LIBS)

clean:
	@rm -f $(MAIN_OBJS)
	@rm -rf Content
	@rm -f *.chunkInfo

