#include <iostream>
#include <string>
#include <dlfcn.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <wchar.h>

#include "ft2build.h"
#include "png.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H

enum function_order { nPrintf, nStrlen, nWcslen, nBtows, 
nFopen, nFclose, nMemcpy, nMemset,
nFTinit, nFTnewface, nFTsetpixelsize, nFTloadchar, nFTgetcharidx,
nFTgetglyph, nFTloadglyph, nFTrenderglyph, nFTglyphtobitmap, nFTdoneglyph,
nPNGcws, nPNGcis, nPNGdws, nPNGjmpbuf, nPNGlongjmp, 
nPNGinit, nPNGsetihdr, nPNGsetrows, nPNGwp, nPNGwe};

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

    main_func_t fn;
    *(void**)(&fn)=l_run_elf_module(m, function_name.c_str());

    void* functions[30];
    functions[nFopen]=(void*)(fopen);
    functions[nFclose]=(void*)(fclose);
    functions[nMemcpy]=(void*)(memcpy);
    functions[nMemset]=(void*)(memset);
    functions[nStrlen]=(void*)(strlen);
    functions[nPrintf]=(void*)(printf);
    functions[nBtows]= (void*)(btowc);
    functions[nWcslen]=(void*)(wcslen);

    // functions[nFTinit]=(void*)FT_Init_FreeType;
    // functions[nFTnewface]=(void*)FT_New_Face;
    // functions[nFTsetpixelsize]=(void*)FT_Set_Pixel_Sizes;
    // functions[nFTloadchar]=(void*)FT_Load_Char;
    // functions[nFTgetglyph]=(void*)FT_Get_Glyph;
    // functions[nFTloadglyph]=(void*)FT_Load_Glyph;
    // functions[nFTrenderglyph]=(void*)FT_Render_Glyph;
    // functions[nFTglyphtobitmap]=(void*)FT_Glyph_To_Bitmap;
    // functions[nFTdoneglyph]=(void*)FT_Done_Glyph;
    // functions[nFTgetcharidx]=(void*)FT_Get_Char_Index;

    // functions[nPNGcws]=(void*)png_create_write_struct;
    // functions[nPNGcis]=(void*)png_create_info_struct;
    // functions[nPNGdws]=(void*)png_destroy_write_struct;
    // functions[nPNGjmpbuf]=(void*)png_set_longjmp_fn;
    // functions[nPNGlongjmp]=(void*)longjmp;
    // functions[nPNGinit]=(void*)png_init_io;
    // functions[nPNGsetihdr]=(void*)png_set_IHDR;
    // functions[nPNGsetrows]=(void*)png_set_rows;
    // functions[nPNGwp]=(void*)png_write_png;
    // functions[nPNGwe]=(void*)png_write_end;


    fn(argc, argv, functions);

    
	clock_t endTime = clock();

	printf("Proccess finished!\n");
	printf("Total time: %ld ticks\n", 
	endTime-startTime);

    return 0;
}