//#include <iostream>
//#include <vector>

#include "ft2build.h"
#include "png.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H

#define MAX_CHAR_IN_STRING 10
#define LINE_SPACING 2
#define FONT_SIZE 60

unsigned char** image;

FT_Library ft_library;
FT_Face ft_face;

//#ifdef _BLOB_
void** functions_list;

#define png_jmpbuf(png_ptr) (*png_set_longjmp_fn((png_ptr), longjmp, (sizeof (jmp_buf))))

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

enum function_order { nPrintf, nStrlen, nWcslen, nBtows, 
nFopen, nFclose, nMemcpy, nMemset,
nFTinit, nFTnewface, nFTsetpixelsize, nFTloadchar, nFTgetcharidx,
nFTgetglyph, nFTloadglyph, nFTrenderglyph, nFTglyphtobitmap, nFTdoneglyph,
nPNGcws, nPNGcis, nPNGdws, nPNGjmpbuf, nPNGlongjmp, 
nPNGinit, nPNGsetihdr, nPNGsetrows, nPNGwp, nPNGwe};

typedef unsigned long size_t;
typedef long off_t;
typedef long time_t;
typedef int8_t wint_t;

size_t strlen(const char *str){
	return ((size_t (*)(const char *))functions_list[nStrlen])(str);
}

void *memset(void *buf, int ch, size_t count){
	return ((void* (*)(void*, int, size_t))functions_list[nMemset])(buf, ch, count);
}

FILE *fopen(const char *fname, const char *mode){
	return ((FILE * (*)(const char *, const char *))functions_list[nFopen])(fname, mode);
}

void *memcpy(void *dest, const void *source, size_t count){
	return ((void* (*)(void*, const void *, size_t))functions_list[nMemcpy])(dest, source, count);
}

int printf(const char *__restrict__ __format, ...){
	return ((int (*)(const char *__restrict__ __format, ...))functions_list[nPrintf])(__format);
}

wint_t btowc (int c){
	return ((wint_t (*)(int))functions_list[nBtows])(c);
}

size_t wcslen(wchar_t* str){
	return ((size_t (*)(wchar_t*))functions_list[nWcslen])(str);
}

// int FT_Init_FreeType(FT_Library * ft){
// 	return ((int(*)(FT_Library*))functions_list[nFTinit])(ft);
// }

// int FT_New_Face(FT_Library ft, const char * ch, FT_Long num, FT_Face * face){
// 	return ((int (*)(FT_Library, const char *, FT_Long, FT_Face *))functions_list[nFTnewface])(ft, ch, num, face);
// }

// int FT_Set_Pixel_Sizes(FT_Face face, FT_UInt num, FT_UInt n)
// {
// 	return((int (*)(FT_Face, FT_UInt, FT_UInt))functions_list[nFTsetpixelsize])(face, num, n);
// }

// FT_Error FT_Load_Char(FT_Face face, FT_ULong char_code, FT_Int32 load_flags){
// 	return((FT_Error (*)(FT_Face,  FT_ULong,FT_Int32))functions_list[nFTloadchar])(face, char_code, load_flags);
// }

// FT_Error FT_Get_Glyph(FT_GlyphSlot slot, FT_Glyph *aglyph){
// 	return((FT_Error (*)(FT_GlyphSlot,  FT_Glyph*))functions_list[nFTgetglyph])(slot, aglyph);
// }

// FT_Error FT_Load_Glyph(FT_Face face, FT_UInt glyph_index, FT_Int32 load_flags){
// 	return((FT_Error (*)(FT_Face, FT_UInt, FT_Int32))functions_list[nFTloadglyph])(face, glyph_index, load_flags);
// }

// FT_Error FT_Render_Glyph(FT_GlyphSlot slot, FT_Render_Mode render_mode){
// 	return ((FT_Error (*)(FT_GlyphSlot, FT_Render_Mode))functions_list[nFTrenderglyph])(slot, render_mode);
// }

// FT_Error FT_Glyph_To_Bitmap(FT_Glyph *the_glyph, FT_Render_Mode render_mode, FT_Vector *origin, FT_Bool destroy){
// 	return ((FT_Error (*)(FT_Glyph*, FT_Render_Mode, FT_Vector*, FT_Bool))functions_list[nFTglyphtobitmap])(the_glyph, render_mode, origin, destroy);
// }

// void FT_Done_Glyph(FT_Glyph glyph){
// 	return ((void (*)(FT_Glyph))functions_list[nFTdoneglyph])(glyph);
// }

// FT_UInt FT_Get_Char_Index(FT_Face face, FT_ULong charcode){
// 	return ((FT_UInt (*)(FT_Face, FT_ULong))functions_list[nFTgetcharidx])(face,charcode);
// }
// png_structp png_create_write_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn){
// 	return ((png_structp (*)(png_const_charp, png_voidp, png_error_ptr, png_error_ptr))functions_list[nPNGcws])(user_png_ver, error_ptr, error_fn, warn_fn);
// }

// png_infop png_create_info_struct(const png_struct *__restrict__ png_ptr){
// 	return ((png_infop (*)(const png_struct *__restrict__ ))functions_list[nPNGcis])(png_ptr);
// }

// void png_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr){
// 	return ((void (*)(png_structpp, png_infopp))functions_list[nPNGdws])(png_ptr_ptr, info_ptr_ptr);
// }

// void png_init_io(png_struct *__restrict__ png_ptr, png_FILE_p fp){
// 	return ((void (*)(png_struct *__restrict__ , png_FILE_p))functions_list[nPNGinit])(png_ptr,fp);
// }

// void png_set_IHDR(const png_struct *__restrict__ png_ptr, png_info *__restrict__ info_ptr, png_uint_32 width, png_uint_32 height, int bit_depth, int color_type, int interlace_method, int compression_method, int filter_method){
// 	return ((void (*)(const png_struct *__restrict__, png_info *__restrict__, png_uint_32, png_uint_32, int, int, int, int, int))
// 	functions_list[nPNGsetihdr])(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);
// }

// void png_set_rows(const png_struct *__restrict__ png_ptr, png_info *__restrict__ info_ptr, png_bytepp row_pointers){
// 	return ((void (*)(const png_struct *__restrict__, png_info *__restrict__, png_bytepp))
// 	functions_list[nPNGsetrows])(png_ptr, info_ptr, row_pointers);
// }

// void png_write_png(png_struct *__restrict__ png_ptr, png_info *__restrict__ info_ptr, int transforms, png_voidp params){
// 	return ((void (*)(png_struct *__restrict__, png_info *__restrict__, int, png_voidp))
// 	functions_list[nPNGwp])(png_ptr, info_ptr, transforms, params);
// }

// void png_write_end(png_struct *__restrict__ png_ptr, png_info *__restrict__ info_ptr){
// 	return ((void (*)(png_struct *__restrict__, png_info *__restrict__ ))
// 	 functions_list[nPNGwe])(png_ptr,info_ptr);
// }

// jmp_buf *png_set_longjmp_fn(png_struct *__restrict__ png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size){
// 	return ((jmp_buf* (*)(png_struct *__restrict__ , png_longjmp_ptr , size_t))
// 	functions_list[nPNGjmpbuf])(png_ptr, longjmp_fn, jmp_buf_size);
// }

// void longjmp(struct __jmp_buf_tag* __env, int __val){
// 	return ((void (*)(struct __jmp_buf_tag*, int))functions_list[nPNGlongjmp])(__env,__val);
// }
//#endif

FT_Glyph get_glyph(FT_Face face, unsigned int charcode)
{
    FT_Load_Char(face, charcode, FT_LOAD_RENDER);
    FT_Glyph glyph = 0;
    FT_Get_Glyph(face->glyph, &glyph);
	FT_UInt glyph_index =FT_Get_Char_Index(ft_face, charcode);
	FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
	FT_Render_Glyph(ft_face->glyph, FT_RENDER_MODE_NORMAL);
	FT_Get_Glyph(ft_face->glyph, &glyph);
	FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    return glyph;
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
	 FT_New_Face(ft_library, font_file, 0, &ft_face);
	 FT_Set_Pixel_Sizes(ft_face, 0, FONT_SIZE);
}

void markup(unsigned int* height, unsigned int* width,
	unsigned int left_space, unsigned int bottom_space, wchar_t* w_text){
	unsigned int space_char = FONT_SIZE / 2;
	unsigned int pos_x = left_space;
	unsigned int pos_y = 0;

	unsigned int height_string = *height + bottom_space + LINE_SPACING;
	unsigned int shift = FONT_SIZE / 6;
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
	unsigned char* line = (char*)malloc(sizeof(char)*(line_len+1));
	memset(line, 0xFF, line_len);

	int mas_len = *width;
	image = (unsigned char**)malloc(sizeof(unsigned char*)*(mas_len));
	for (unsigned int i = 0; i < *width; i++){
		image[i] = (char*)malloc(sizeof(char)*(line_len+1));
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
					unsigned char elem =bitmap.buffer[y*bitmap.width+x];
					for (unsigned int k = 0; k < 3; k++)
						image[img_y][img_x*3+k] -= bitmap.buffer[pos_buf];
				}
			}

		pos_x += bitmap.width + bitmap_glyph->left + left_space;
		FT_Done_Glyph(glyph);
	}
	
	*width=new_width;

	for (int i = 0; i < mas_len; i++){
		unsigned char* new_line = (char*)malloc(sizeof(char)*(*width*3+1));
		image[i][*width*3]=0;
		memcpy(new_line, image[i], *width*3);
		free(image[i]);
		image[i]=new_line;
	}

	free(line);
}

int main(int argc, char *argv[], void **functions){
	functions_list=functions;

	char* font_name = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
	char* text = (char*)malloc(sizeof(char)*(strlen(argv[2])+1));
	char* image_name = (char*)malloc(sizeof(char)*(strlen(argv[3])+1));
	wchar_t* w_text = (wchar_t*)malloc(sizeof(wchar_t)*(strlen(argv[2])+1));

	memcpy(font_name, argv[1], strlen(argv[1])+1);
	memcpy(text, argv[2], strlen(argv[2])+1); 
	memcpy(image_name, argv[3], strlen(argv[3])+1);

	for (unsigned int i = 0; i<strlen(text); i++)
		w_text[i] = btowc((int)(text[i]));

	// std::string font_name(argv[1]);
	// std::string text(argv[2]);
	// std::string image_name(argv[3]);

	// std::wstring w_text(text.begin(), text.end());

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
