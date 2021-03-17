.PHONY: all static dynamic elf_loader clean clean_all elf-loader.o blob app_blob

LIB_A = ./libpng/lib/libpng.a ./freetype/lib/libfreetype.a ./zlib/lib/libz.a
LIB_SO = ./libpng/lib/libpng.so ./freetype/lib/libfreetype.so ./zlib/lib/libz.so
CFLAGS=-I./libpng/include/ -I./freetype/include/ -I./zlib/include/
CFLAGS_BLOB = -I./elfloader
CFLAGS_ELF_LOADER = -I./../libpng/include/ -I./../freetype/include/freetype2/ -I./../zlib/include/
LDFLAGS= -L./libpng/lib/ -L./freetype/lib/ -L./zlib/lib/ -lpng -lfreetype -lz -lm
LDFLAGS_BLOB = -L./libs -lfreetype -lpng -lz

all: clean_all static dynamic app_blob

static: app/application.cpp $(LIB_A)
	g++ $(CFLAGS) -static app/application.cpp $(LDFLAGS) -o app-static

dynamic: app/application.cpp $(LIB_SO)
	g++ $(CFLAGS) -D_DYNAMIC_ app/application.cpp -ldl -o app-dynamic

app_blob: blob app/app_blob.cpp elf_loader
	g++ $(CFLAGS_BLOB) app/app_blob.cpp -o app-blob -ldl -g

blob: app/blob.c  $(LIB_A)
	gcc $(CFLAGS) -fPIC -fno-stack-protector app/blob.c -nostdlib -emain $(LDFLAGS_BLOB) -o blob -g

elf_loader: ./elfloader/elf_loader.c ./elfloader/main.c ./elfloader/wheelc/list.c
	gcc  ./elfloader/elf_loader.c ./elfloader/main.c ./elfloader/wheelc/list.c -shared  -fPIC -lpthread -o ./elfloader/lib_elf_loader.so 

clean_all: clean
	rm -f *.png

clean:
	rm -f app-*
	rm -f ./elfloader/lib_elf_loader.so
	rm -f blob
	rm -f blob.bin
