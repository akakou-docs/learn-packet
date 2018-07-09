build:
	gcc server.c
debug:
	gcc -g server.c && sudo gdb --args ./a.out wlp3s0
log:
	chmod +x a.out && sudo ./a.out wlp3s0 > server.log
run : a.out
	chmod +x a.out && sudo ./a.out wlp3s0
