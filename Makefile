.PHONY: all clean

CFLAGS=-I./libpng/include/ -I./freetype/include/freetype2/ -I./zlib/include/
CFLAGS_BLOB = -I./elfloader
CFLAGS_ELF_LOADER = -I./../libpng/include/ -I./../freetype/include/freetype2/ -I./../zlib/include/
LDFLAGS=-L./libpng/lib/ -L./freetype/lib/ -L./zlib/lib/ -lpng -lfreetype -lz -lm

all: clean_all static dynamic blob

static: 
	clean
	g++ $(CFLAGS) app/application.cpp $(LDFLAGS) -o app-static

dynamic:
	clean
	g++ $(CFLAGS) -D_DYNAMIC_ app/application.cpp -ldl -o app-dynamic

blob:
	clean 
	elf_loader
	gcc $(CFLAGS) -pie -fPIE -fPIC -shared app/blob.c $(LDFLAGS) -o blob
	g++ $(CFLAGS_BLOB) $(CFLAGS) app/app_blob.cpp -o app-blob -ldl

elf_loader:
	clean
	gcc  ./elfloader/elf_loader.c ./elfloader/main.c ./elfloader/wheelc/list.c -c
	gcc -shared -o ./elfloader/lib_elf_loader.so elf_loader.o main.o list.o -ldl -lpthread -fPIC
	rm -f elf_loader.o main.o list.o 

clean_all: clean
	rm -f *.png

clean:
	rm -f app-*
	rm -f ./elfloader/lib_elf_loader.so
	rm -f blob
