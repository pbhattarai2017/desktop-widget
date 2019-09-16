main: clean
	gcc `pkg-config --cflags gtk+-3.0 webkit2gtk-4.0` main.c -o main `pkg-config --libs gtk+-3.0 webkit2gtk-4.0`
clean:
	if [ -e "main" ];then rm main;fi;
