
GLIB_COMPILE_RESOURCES = glib-compile-resources

demo_resources.sci: demo_resources.txt
	../../../../bin/nsp -nw -ns -e "exec('read.sci');exit"

demo_resources.txt: demo_resources.c
	grep '0x' demo_resources.c  > demo_resources.txt

demo_resources.c: demo.gresource.xml gnome-foot.png
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=./ --generate-source ./demo.gresource.xml


