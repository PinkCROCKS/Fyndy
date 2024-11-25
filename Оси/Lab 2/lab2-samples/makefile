all: threads

race:
	gcc data_race.c -pthread -g

fix: 
	gcc fix_data_race.c -fsanitize=thread -pthread -g

threads:
	gcc threads.c -fsanitize=thread -pthread -g