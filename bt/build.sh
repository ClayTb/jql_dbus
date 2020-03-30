gcc `pkg-config --cflags glib-2.0 gio-2.0` -o bt main.c bt-dbus.c dbus-fun.c `pkg-config --libs glib-2.0 gio-2.0`
