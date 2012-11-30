CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 

UNITS = BedFile.o BedItem.o Utils.o GtfItem.o AbstractFile.o GtfFile.o GtfTranscriptItem.o GtfTranscriptItemFile.o

OBJS = mbsmd.o 

TESTS =	mbsmd_test.o BedFile_test.o Utils_test.o GtfFile_test.o GtfItem_test.o GtfTranscriptItem_test.o GtfTranscriptItemFile_test.o

LIBS = -lgtest

TARGET = mbsmd_gcc.exe

TARGET_TEST = mbsmd_test_gcc.exe

$(TARGET):	$(OBJS) $(UNITS)
	$(CXX) -o $(TARGET) $(OBJS) $(UNITS) $(LIBS)

$(TARGET_TEST):	${TESTS} $(UNITS)
	$(CXX) -o $(TARGET_TEST) $(TESTS) $(UNITS) $(LIBS)

all:	$(TARGET) $(TARGET_TEST)

clean:
	rm -f $(OBJS) $(TARGET)
