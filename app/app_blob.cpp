#include <iostream>
#include <ctime>
#include <cmath>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "./../elfloader/elf_loader.h"
#include <dlfcn.h>
#include <string>


enum function_order { nPrintf, nStrlen, nWcslen, nBtows, 
nFopen, nFclose, nMemcpy, nMemset, nFstat,
nFree, nMalloc, nPuts, nGetenv, nMemmove, nStrcmp, nLongjmp,
nStrstr, nStrtol, nMemcmp, nStrncpy, nStrncmp, nStrcat, nQsort,
nStrrchr, nMemchr, nSprintf, nStrcpy, nSetjmp, nRealloc, nMunmap,
nOpen, nClose, nRead, nFcntl, nMmap, nFloor, nPow, nFrexp, nModf,
nFprintf, nFputc, nAtof, nAbort, nFread, nStrerror, nGmtime, nFflush,
nFerror, nFwrite, nRemove, nStderr};

static const std::string path_blob = "blob";
static const std::string function_name = "main";
static const std::string path_lib = "./elfloader/lib_elf_loader.so";

typedef int (*main_func_t)(int argc, char** argv, void **functions);

void *so_lib;
bool (*l_read_elf_file)(const char *fname, void **buf, size_t *len);
struct elf_module *(*l_load_elf_module)(const char *name, const void *elf_data, size_t elf_len);
void* (*l_run_elf_module)(struct elf_module *m, const char *func);

char* load_lib(){
    so_lib=dlopen(path_lib.c_str(), RTLD_LAZY);
	if(!so_lib){
		return dlerror();
	}
    
    char *error;

    *(void**)(&l_load_elf_module) = dlsym(so_lib, "load_elf_module");
    if ((error = dlerror()) != NULL)  {
        return error;
    }
    *(void**)(&l_run_elf_module) = dlsym(so_lib, "run_elf_module");
    if ((error = dlerror()) != NULL)  {
        return error;
    }
    *(void**)(&l_read_elf_file) = dlsym(so_lib, "read_elf_file");
    if ((error = dlerror()) != NULL)  {
        return error;
    }

    return dlerror();
}

int main(int argc, char *argv[]){

    clock_t startTime=clock();

    if ( argc != 4) {
        std::cout << "Arguments Error" << std::endl;
        std::cout << "FORMAT: <font_file> \"<text for drawing>\" <image file>\n" << std::endl;
        return 1;
    }

    std::string font_name(argv[1]);
	std::string text(argv[2]);
	std::string image_name(argv[3]);

	std::wstring w_text(text.begin(), text.end());

    FILE *test_font=fopen(font_name.c_str(), "rb");
	if(!test_font){
		//std::cout << "Bad font name!"<< std::endl;
		printf("Bad font name!\n");
		return 1;
	}
	fclose(test_font);

    FILE *fp=fopen(image_name.c_str(), "wb");
	if(!fp){
        printf("Bad file to save!\n");
		return 1;
	}

    char* ret_error;
    if ((ret_error=load_lib())){
		return 1;
	}

    struct elf_module *m;
    //void* m;
    void *bin;
    size_t len;
    if (!l_read_elf_file(path_blob.c_str(), &bin, &len)) {
        free(bin);
        return 0;
    }

    m = l_load_elf_module(path_blob.c_str(), bin, len);
    if (m == NULL) {
        free(bin);
        std::cout << "load_elf_module " << path_blob << " failed" << std::endl;
        return 0;
    }

    free(bin);
    main_func_t fn = (main_func_t)(m->entry);
    std::cout << "Address entry point: " << std::hex << m->entry << std::endl;
    
    
        //*(void**)(&fn)=l_run_elf_module(m, function_name.c_str());

    typedef const char * (*strstr_t)( const char * string1, const char * string2 );
    typedef const char * (*strrchr_t)( const char * string, int symbol );
    typedef const void * (*memchr_t)( const void * memptr, int val, size_t num );
    void* functions[51];
    functions[nFopen]=(void*)(fopen);
    functions[nFclose]=(void*)(fclose);
    functions[nMemcpy]=(void*)(memcpy);
    functions[nMemset]=(void*)(memset);
    functions[nStrlen]=(void*)(strlen);
    functions[nPrintf]=(void*)printf;
    functions[nBtows]= (void*)(btowc);
    functions[nWcslen]=(void*)(wcslen);
    functions[nFstat]=(void*)fstat;
    functions[nFree]=(void*)free;
    functions[nMalloc]=(void*)malloc;
    functions[nPuts]=(void*)puts;
    functions[nGetenv]=(void*)getenv;
    functions[nMemmove]=(void*)memmove;
    functions[nStrcmp]=(void*)strcmp;
    functions[nLongjmp]=(void*)longjmp;
    functions[nStrstr]=(void*)(strstr_t)strstr;
    functions[nStrtol]=(void*)strtol;
    functions[nMemcmp]=(void*)memcmp;
    functions[nStrncpy]=(void*)strncpy;
    functions[nStrncmp]=(void*)strncmp;
    functions[nStrcat]=(void*)strcat;
    functions[nQsort]=(void*)qsort;
    functions[nStrrchr]=(void*)(strrchr_t)strrchr;
    functions[nMemchr]=(void*)(memchr_t)memchr;
    functions[nSprintf]=(void*)sprintf;
    functions[nStrcpy]=(void*)strcpy;
    functions[nSetjmp]=(void*)setjmp;
    functions[nRealloc]=(void*)realloc;
    functions[nMunmap]=(void*)munmap;
    functions[nOpen]=(void*)open;
    functions[nClose]=(void*)close;
    functions[nRead]=(void*)read;
    functions[nFcntl]=(void*)fcntl;
    functions[nMmap]=(void*)mmap;
    functions[nFloor]=(void*)floor;
    functions[nPow]=(void*)pow;
    functions[nFrexp]=(void*)frexp;
    functions[nModf]=(void*)modf;
    functions[nFprintf]=(void*)fprintf;
    functions[nFputc]=(void*)fputc;
    functions[nAtof]=(void*)atof;
    functions[nAbort]=(void*)abort;
    functions[nFread]=(void*)fread;
    functions[nStrerror]=(void*)strerror;
    functions[nGmtime]=(void*)gmtime;
    functions[nFflush]=(void*)fflush;
    functions[nFerror]=(void*)ferror;
    functions[nFwrite]=(void*)fwrite;
    functions[nRemove]=(void*)remove;
    functions[nStderr]=(void*)stderr;

    char buf[100];
    ((void* (*)(void*, int, size_t))functions[nMemset])(buf, 0, 100);
    printf("There are not in BLOB!!\n");
    fn(argc, argv, functions);

    
	clock_t endTime = clock();

	printf("Proccess finished!\n");
	printf("Total time: %ld ticks\n", 
	endTime-startTime);

    return 0;
}
