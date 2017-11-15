#declare variable
#Note : $ @is the namte of the file being generated
#$ < the prerequisite

CC = g++ - std = c++ 11 CFLAGS =
         -g - Wall

                  CPPFILES = main.cpp OBJS = $(CPPFILES
                                               :.cpp =.o)

             TARGET = stressTracking

                              all : $(TARGET)

                                        $(TARGET)
    : $(OBJS) $(CC) $(CFLAGS) $(OBJS) -
      o $ @

          .cpp.o : $(CPPFILES) $(CC) $(CFLAGS) -
      c $ <

                      clean : rm - f $(OBJS) $(TARGET)