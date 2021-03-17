//#include <iostream>
//#include <vector>

#include "ft2build.h"
#include "png.h"
#include <dlfcn.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

#define MAX_CHAR_IN_STRING 10
#define LINE_SPACING 2
#define FONT_SIZE 60


unsigned char** image;

FT_Library ft_library;
FT_Face ft_face;


void** functions_list;

#define png_jmpbuf(png_ptr) (*png_set_longjmp_fn((png_ptr), longjmp, (sizeof (jmp_buf))))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

enum function_order { nPrintf, nStrlen, nWcslen, nBtows, 
nFopen, nFclose, nMemcpy, nMemset, nFstat,
nFree, nMalloc, nPuts, nGetenv, nMemmove, nStrcmp, nLongjmp,
nStrstr, nStrtol, nMemcmp, nStrncpy, nStrncmp, nStrcat, nQsort,
nStrrchr, nMemchr, nSprintf, nStrcpy, nSetjmp, nRealloc, nMunmap,
nOpen, nClose, nRead, nFcntl, nMmap, nFloor, nPow, nFrexp, nModf,
nFprintf, nFputc, nAtof, nAbort, nFread, nStrerror, nGmtime, nFflush,
nFerror, nFwrite, nRemove, nStderr};

FILE* stderr;

typedef long off_t;
typedef long time_t;
typedef int8_t wint_t;

int * __errno_location(void){
	return NULL;
}

void abort(){
	((void (*)())functions_list[nAbort])();
	return;
}

typedef int (*my_remove_t)(const char *__filename);
int remove(const char *__filename){
	//printf("remove\n");
return ((my_remove_t)functions_list[nRemove])(__filename);

}
typedef size_t (*my_fwrite_t)(const void *__restrict__ __ptr, size_t __size, size_t __n, FILE *__restrict__ __s);
size_t fwrite(const void *__restrict__ __ptr, size_t __size, size_t __n, FILE *__restrict__ __s){
//printf("fwrite\n");
return ((my_fwrite_t)functions_list[nFwrite])(__ptr, __size, __n, __s);
}

typedef int (*my_ferror_t)(FILE *__stream);
int ferror(FILE *__stream){
	//printf("ferror\n");
return ((my_ferror_t)functions_list[nFerror])(__stream);
}

typedef int (*my_fflush_t)(FILE *__stream);
int fflush(FILE *__stream){
	//printf("fflush\n");
return ((my_fflush_t)functions_list[nFflush])(__stream);
}

typedef struct tm *(*my_gmtime_t)(const time_t *__timer);
struct tm *gmtime(const time_t *__timer){
	//printf("gmtime\n");
return ((my_gmtime_t)functions_list[nGmtime])(__timer);
}

typedef char *(*my_strerror_t)(int __errnum);
char *strerror(int __errnum){
	//printf("strerror\n");
return ((my_strerror_t)functions_list[nStrerror])(__errnum);
}

typedef size_t (*my_fread_t)(void *__restrict__ __ptr, size_t __size, size_t __n, FILE *__restrict__ __stream);
size_t fread(void *__restrict__ __ptr, size_t __size, size_t __n, FILE *__restrict__ __stream){
	//printf("fread\n");
return ((my_fread_t)functions_list[nFread])(__ptr, __size, __n, __stream);
}

typedef double (*my_atof_t)(const char *__nptr);
double atof(const char *__nptr){
	//printf("atof\n");
return ((my_atof_t)functions_list[nAtof])(__nptr);
}

typedef int (*my_fputc_t)(int __c, FILE *__stream);
int fputc(int __c, FILE *__stream){
	//printf("fputc\n");
return ((my_fputc_t)functions_list[nFputc])(__c, __stream);
}

typedef int (*my_fprintf_t)(FILE *__restrict__ __stream, const char *__restrict__ __format, ...);
int fprintf(FILE *__restrict__ __stream, const char *__restrict__ __format, ...){
	//printf("fprintf\n");
return ((my_fprintf_t)functions_list[nFprintf])(__stream, __format);
}

typedef double (*my_modf_t)(double __x, double *__iptr);
double modf(double __x, double *__iptr){
	//printf("modf\n");
return ((my_modf_t)functions_list[nModf])(__x, __iptr);
}

typedef double (*my_frexp_t)(double __x, int *__exponent);
double frexp(double __x, int *__exponent){
	//printf("frexp\n");
return ((my_frexp_t)functions_list[nFrexp])(__x, __exponent);
}

typedef double (*my_pow_t)(double __x, double __y);
double pow(double __x, double __y){
	//printf("pow\n");
return ((my_pow_t)functions_list[nPow])(__x, __y);
}

typedef double (*my_floor_t)(      double val );
double floor(      double val ){
	//printf("floor\n");
	return ((my_floor_t)functions_list[nFloor])(val);
}
typedef void *(*my_mmap_t)(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset);
void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, off_t __offset){
//printf("mmap\n");
return ((my_mmap_t)functions_list[nMmap])(__addr, __len, __prot, __flags, __fd, __offset);
}
typedef int (*my_fcntl_t)(int __fd, int __cmd, ...);
int fcntl(int __fd, int __cmd, ...){
	//printf("fcntl\n");
	return ((my_fcntl_t)functions_list[nFcntl])(__fd, __cmd);
}
typedef ssize_t (*my_read_t)(int __fd, void *__buf, size_t __nbytes);
ssize_t read(int __fd, void *__buf, size_t __nbytes){
	//printf("read\n");
	return ((my_read_t)functions_list[nRead])(__fd, __buf, __nbytes);
}

typedef int (*my_close_t)(int __fd);
int close(int __fd){
	//printf("clode\n");
return ((my_close_t)functions_list[nClose])(__fd);
}

size_t strlen(const char *str){
	return ((size_t (*)(const char *))functions_list[nStrlen])(str);
}

void *memset(void *dest,int c,size_t count){
	//printf("memset\n");
	((void* (*)(void*, int, size_t))functions_list[nMemset])(dest, c, count);
	return dest;
}

FILE *fopen(const char *fname, const char *mode){
	//printf("fopen\n");
	return ((FILE * (*)(const char *, const char *))functions_list[nFopen])(fname, mode);
}

int fclose(FILE *__stream){
	//printf("fclose\n");
	return ((int (*)(FILE*))functions_list[nFclose])(__stream);
}

void *memcpy(void *dest, const void *source, size_t count){
	//printf("memcpy\n");
	return ((void* (*)(void*, const void *, size_t))functions_list[nMemcpy])(dest, source, count);
}


int printf(const char * format, ...){
	return ((int (*)(const char * format, ...))functions_list[nPrintf])(format);
}

wint_t btowc (int c){
	return ((wint_t (*)(int))functions_list[nBtows])(c);
}

size_t wcslen(wchar_t* str){
	return ((size_t (*)(wchar_t*))functions_list[nWcslen])(str);
}

int fstat(int handle, struct stat *statbuf){
	//printf("fstat\n");
	return((int (*)(int, struct stat*))functions_list[nFstat])(handle, statbuf);	
}

typedef int (*my_open_t)(const char *__file, int __oflag, ...);
int open(const char *__file, int __oflag, ...){
	//printf("open\n");
	return ((my_open_t)functions_list[nOpen])(__file, __oflag);
}
typedef void (*my_free_t) (void *ptr);
void free(void *ptr){
	//printf("free\n");
	return ((my_free_t)functions_list[nFree])(ptr);	
}

typedef void* (*my_malloc_t)(size_t __size);
void *malloc(size_t __size){
	//printf("malloc %ul\n", __size);
	return ((my_malloc_t)functions_list[nMalloc])(__size);	
}

typedef void *(*my_realloc_t)(void *__ptr, size_t __size);
void *realloc(void *__ptr, size_t __size)
{
	//printf("realloc\n");
	return ((my_realloc_t)functions_list[nRealloc])(__ptr, __size);
}

typedef int (*my_memcmp_t)(const void *__s1, const void *__s2, size_t __n);
int memcmp(const void *__s1, const void *__s2, size_t __n){
	//printf("memcmp\n");
	return ((my_memcmp_t)functions_list[nMemcmp])(__s1, __s2, __n);
}

typedef int (*my_put_s)(const char *__s);
int puts(const char *__s){
	return ((my_put_s)functions_list[nPuts])(__s);
}

typedef char *(*my_getenv_t)(const char *__name);
char *getenv(const char *__name){
	//printf("getenv\n");
	return ((my_getenv_t)functions_list[nGetenv])(__name);
}

typedef int (*my_munmap_t)(void *__addr, size_t __len);
int munmap(void *__addr, size_t __len){
	//printf("munmap\n");
	return ((my_munmap_t)functions_list[nMunmap])(__addr, __len);
}
typedef void *(*my_memmove_t)(void *__dest, const void *__src, size_t __n);
void *memmove(void *__dest, const void *__src, size_t __n){
	//printf("memmove\n");
	return ((my_memmove_t)functions_list[nMemmove])(__dest, __src, __n);
}

typedef int (*my_strcmp_t)(const char *__s1, const char *__s2);
int strcmp(const char *__s1, const char *__s2){
	//printf("strcmp\n");
	return ((my_strcmp_t)functions_list[nStrcmp])(__s1, __s2);
}

typedef void (*my_longjmp_t)(struct __jmp_buf_tag *__env, int __val);
void longjmp(struct __jmp_buf_tag *__env, int __val){
	//printf("longjmp\n");
	((my_longjmp_t)functions_list[nLongjmp])(__env, __val);}

typedef char * (*my_strstr_t)(const char * string1, const char * string2 );
char * strstr(const char * string1, const char * string2 ){
	//printf("strstr\n");
	return ((my_strstr_t)functions_list[nStrstr])(string1, string2);
}

typedef long (*my_strtol_t)(const char *__restrict__ __nptr, char **__restrict__ __endptr, int __base);
long strtol(const char *__restrict__ __nptr, char **__restrict__ __endptr, int __base){
	//printf("strtol\n");
	return ((my_strtol_t)functions_list[nStrtol])(__nptr, __endptr, __base);
}

typedef char *(*my_strcat_t)(char *__restrict__ __dest, const char *__restrict__ __src);
char *strcat(char *__restrict__ __dest, const char *__restrict__ __src)
{
	//printf("strcat\n");
	return ((my_strcat_t)functions_list[nStrcat])(__dest, __src);
}

typedef void (*my_qsort_t)(void *__base, size_t __nmemb, size_t __size,  int ( * comparator ) ( const void *, const void * ));
void qsort(void *__base, size_t __nmemb, size_t __size,  int ( * comparator ) ( const void *, const void * )){
	//printf("qsort\n");
	return ((my_qsort_t)functions_list[nQsort])(__base, __nmemb, __size, comparator);
}

typedef char *(*my_strcpy_t)(char *__restrict__ __dest, const char *__restrict__ __src);
char *strcpy(char *__restrict__ __dest, const char *__restrict__ __src){
	//printf("strcpy\n");
	return ((my_strcpy_t)functions_list[nStrcpy])(__dest, __src);
}
FT_Glyph get_glyph(FT_Face face, unsigned int charcode)
{
	//printf("get glyph\n");
    FT_Load_Char(face, charcode, FT_LOAD_RENDER);
    FT_Glyph glyph = 0;
	//printf("get glyph2\n");
    FT_Get_Glyph(face->glyph, &glyph);
	FT_UInt glyph_index =FT_Get_Char_Index(ft_face, charcode);
	FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
	FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL);
	FT_Get_Glyph(ft_face->glyph, &glyph);
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    return glyph;
}

typedef  char * (*my_strrchr_t)(const char * string, int symbol );
 char * strrchr(const char * string, int symbol ){
	//printf("strrchr\n");
	return ((my_strrchr_t)functions_list[nStrrchr])(string, symbol);
 }

typedef char *(*my_strncpy_t)(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n);
char *strncpy(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n){
	//printf("strncpy\n");
	return ((my_strncpy_t)functions_list[nStrncpy])(__dest, __src, __n);
}

typedef void * (*my_memchr_t)( const void * memptr, int val, size_t num );
void * memchr( const void * memptr, int val, size_t num ){
	//printf("memchr\n");
	return ((my_memchr_t)functions_list[nMemchr])(memptr, val, num);
}

typedef int (*my_sprintf_t)(char *__restrict__ __s, const char *__restrict__ __format, ...);
int sprintf(char *__restrict__ __s, const char *__restrict__ __format, ...){
	//printf("sprintf\n");
	return ((my_sprintf_t)functions_list[nSprintf])(__s, __format);
}
void get_size(wchar_t* text, unsigned int *max_h, unsigned int *max_w, unsigned int *bottom, unsigned int *left){
	*max_h=0;
	*max_w=0;
	*left=0;
	*bottom=0;

	for (unsigned int i = 0; i < wcslen(text); i++){
			FT_Glyph glyph = get_glyph(ft_face, text[i]);

			FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)(glyph);
			FT_Bitmap bitmap = bitmap_glyph->bitmap;
			
			*left = MAX(*left, (unsigned int)(abs(bitmap_glyph->left)));
			*bottom = MAX(*bottom,(unsigned int)(bitmap.rows-bitmap_glyph->top));
			*max_h =  MAX(*max_h,(unsigned int)(bitmap_glyph->top));
			*max_w =  MAX(*max_w,(unsigned int)(bitmap.width));

			 FT_Done_Glyph(glyph);
	}
}

typedef int (*my_setjmp_t)( jmp_buf environment );
int setjmp( jmp_buf environment ){
	//printf("setjmp\n");
	return ((my_setjmp_t)functions_list[nSetjmp])(environment);
}
typedef int (*my_strncmp_t)(const char *__s1, const char *__s2, size_t __n);
int strncmp(const char *__s1, const char *__s2, size_t __n){
	//printf("strncmp\n");
	return ((my_strncmp_t)functions_list[nStrncmp])(__s1, __s2, __n);
}

int createPNG(unsigned int height, unsigned int width, char* image_name){
	FILE *fp=fopen(image_name, "wb");
	if(!fp){
		return 1;
	}

	png_structp png_ptr= png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_ptr){
		fclose(fp);
		return 1;
	}
	png_infop png_info;
	if(!(png_info= png_create_info_struct(png_ptr))){
		 png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		return 1;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		 png_destroy_write_struct(&png_ptr, NULL);
		fclose(fp);
		return 1;
	}

	 png_init_io(png_ptr, fp);

	 png_set_IHDR(png_ptr, png_info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, 
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	 png_set_rows(png_ptr, png_info, image);
	 png_write_png(png_ptr, png_info, PNG_TRANSFORM_IDENTITY, NULL);
	 png_write_end(png_ptr, png_info);

	 png_destroy_write_struct(&png_ptr, NULL);
	fclose(fp);
	return 0;
}

void load_font(char* font_file){
	 FT_Init_FreeType(&ft_library);
	 //printf("Load!\n");
	 FT_New_Face(ft_library, font_file, 0, &ft_face);
	 FT_Set_Pixel_Sizes(ft_face, 0, FONT_SIZE);
}

void markup(unsigned int* height, unsigned int* width,
	unsigned int left_space, unsigned int bottom_space, wchar_t* w_text){
	unsigned int space_char = FONT_SIZE / 2;
	unsigned int pos_x = left_space;
	unsigned int pos_y = 0;

	unsigned int height_string = *height + bottom_space + LINE_SPACING;
	
	unsigned int number_string = wcslen(w_text) / MAX_CHAR_IN_STRING;
	if (wcslen(w_text) % MAX_CHAR_IN_STRING!=0) 
		number_string++;

	*height = LINE_SPACING + number_string * height_string + bottom_space;

	unsigned int num_letter_string = 0;
	if (MAX_CHAR_IN_STRING > wcslen(w_text))
		num_letter_string = wcslen(w_text) - 1;
	else num_letter_string = MAX_CHAR_IN_STRING-1;

	*width = (*width+left_space) * num_letter_string;

	int line_len = 3 * *width;
	unsigned char* line = (unsigned char*)malloc(sizeof(unsigned char)*(line_len+1));
	memset(line, 0xFF, line_len);

	int mas_len = *width;
	image = (unsigned char**)malloc(sizeof(unsigned char*)*(mas_len));
	for (unsigned int i = 0; i < *width; i++){
		image[i] = (unsigned char*)malloc(sizeof(unsigned char)*(line_len+1));
		memcpy(image[i], line, line_len);
	}

	unsigned int new_width = 0;
	for(int i = 0; i < wcslen(w_text); i++ ){
		if( i % MAX_CHAR_IN_STRING==0 ){
			if (new_width < pos_x) new_width=pos_x;
			pos_y += height_string;
			pos_x = left_space;
		}

		if(w_text[i]==L' '){
			pos_x += space_char;
			continue;
		}

		FT_Glyph glyph = get_glyph(ft_face, w_text[i]);

		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		FT_Bitmap bitmap = bitmap_glyph->bitmap;

		for(unsigned int y = 0; y < bitmap.rows; ++y)
			for(unsigned int x = 0; x < bitmap.width; ++x)
			{
				unsigned int pos_buf = y * bitmap.width + x;
				if (bitmap.buffer[pos_buf]!=0){
					unsigned int img_y = pos_y - bitmap_glyph->top + y;
					unsigned int img_x = pos_x + bitmap_glyph->left + x;
					for (unsigned int k = 0; k < 3; k++)
						image[img_y][img_x*3+k] -= bitmap.buffer[pos_buf];
				}
			}

		pos_x += bitmap.width + bitmap_glyph->left + left_space;
		FT_Done_Glyph(glyph);
	}
	
	*width=new_width;

	for (int i = 0; i < mas_len; i++){
		unsigned char* new_line = (unsigned char*)malloc(sizeof(unsigned char)*(*width*3+1));
		image[i][*width*3]=0;
		memcpy(new_line, image[i], *width*3);
		free(image[i]);
		image[i]=new_line;
	}

	free(line);
}

int main(int argc, char *argv[], void **functions){
	functions_list = functions;
	
	printf("There are in BLOB!\n");
	stderr = (FILE*)functions_list[nStderr];
	char* font_name = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
	char* text = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
	char* image_name = (char*)malloc(sizeof(char)*(strlen(argv[3])+1));
	wchar_t* w_text = (wchar_t*)malloc(sizeof(wchar_t)*(strlen(argv[2])+1));

	memcpy(font_name, argv[1], strlen(argv[1])+1);
	memcpy(text, argv[2], strlen(argv[2])+1); 
	memcpy(image_name, argv[3], strlen(argv[3])+1);

	for (unsigned int i = 0; i<strlen(text); i++)
		w_text[i] = btowc((int)(text[i]));

	// // std::string font_name(argv[1]);
	// // std::string text(argv[2]);
	// // std::string image_name(argv[3]);

	// // std::wstring w_text(text.begin(), text.end());

	 FILE *test_font=fopen(font_name, "rb");
	if(!test_font){
		//std::cout << "Bad font name!"<< std::endl;
		printf("Bad font name!\n");
		return 1;
	}
	fclose(test_font);

    FILE *fp=fopen(image_name, "wb");
	if(!fp){
        printf("Bad file to save!\n");
		return 1;
	}

	if(strlen(text)>=100){
		printf("Very large text! (limit 100)\n");
		return 1;
	}

	load_font(font_name);

	unsigned int height;
	unsigned int width;
	unsigned int left_space;
	unsigned int bottom_space;

	
	get_size(w_text, &height, &width, &bottom_space, &left_space);

	markup(&height, &width, left_space, bottom_space, w_text);

	if(createPNG(height, width, (char*)image_name)!=0){
		printf("Error save to file!");
	}
	
	
	for (int i =0; i<width; i++)
		free(image[i]);
	free(image);
	free(font_name);
	free(image_name);
	free(text);
	free(w_text);
	return 0;
}
