all:
	gcc -Idisplays/default -IGFXfont/Fonts/ -IGFXfont/ test_display_driver_main.c \
	display_driver.c displays/default/console_display.c -o test.exe -D DISP_SCREEN=DISP_CONSOLE

clean:
	rm test.exe
