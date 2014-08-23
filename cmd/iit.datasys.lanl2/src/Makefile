#
# storage_mgr.c
#
#  Created on: Feb 25, 2013
#      Author: Xiaobing Zhou
#

TARGETS = main sleep allone gen_conf proc_data proc_data2 proc_data3 proc_data4 proc_data5
CC = g++ 
#CCFLAGS = -g -Wall -pedantic
CCFLAGS = -g -std=gnu++0x

all:	$(TARGETS)
	
main: job.cpp Util.cpp
	$(CC) $(CCFLAGS) -DLOG_START -o $@ $^
	
sleep: job.cpp Util.cpp
	$(CC) $(CCFLAGS) -DLOG_END -o $@ $^	
	
allone: job.cpp Util.cpp
	$(CC) $(CCFLAGS) -DLOG_ALL -o $@ $^
	
gen_conf: conf_gen.cpp String_Tokenizer.cpp str_trim.cpp
	$(CC) $(CCFLAGS) -o $@ $^
	
proc_data: data_proc.cpp String_Tokenizer.cpp str_trim.cpp Util.cpp
	$(CC) $(CCFLAGS) -DWAY1 -o $@ $^
	
proc_data2: data_proc.cpp String_Tokenizer.cpp str_trim.cpp Util.cpp
	$(CC) $(CCFLAGS) -DWAY2 -o $@ $^
	
proc_data3: data_proc.cpp String_Tokenizer.cpp str_trim.cpp Util.cpp
	$(CC) $(CCFLAGS) -DWAY3 -o $@ $^
		
proc_data4: data_proc.cpp String_Tokenizer.cpp str_trim.cpp Util.cpp
	$(CC) $(CCFLAGS) -DWAY4 -o $@ $^

proc_data5: data_proc.cpp String_Tokenizer.cpp str_trim.cpp Util.cpp
	$(CC) $(CCFLAGS) -DWAY5 -o $@ $^
	
.PHONY:	clean

clean:
	rm $(TARGETS)
	rm *.o	
	rm *.gch*
	rm *.bin*
