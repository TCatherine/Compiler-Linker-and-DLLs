#include <iostream>
#include <vector>
#include <time.h>

#include "ft2build.h"
#include "png.h"

#include FT_FREETYPE_H
#include FT_GLYPH_H

#define MAX_CHAR_IN_STRING 10
#define LINE_SPACING 2
#define FONT_SIZE 60

FT_Library ft_library;
FT_Face ft_face;

#ifdef _DYNAMIC_
#include <dlfcn.h>

#define PATH_ZLIB "zlib/lib/libz.so"
#define PATH_FREETYPE "freetype/lib/libfreetype.so"
#define PATH_LIBPNG "libpng/lib/libpng.so"


#define FT_Init_FreeType	l_FT_Init_FreeType
#define FT_New_Face			l_FT_New_Face
#define FT_Set_Pixel_Sizes	l_FT_Set_Pixel_Sizes
#define FT_Load_Char		l_FT_Load_Char
#define FT_Get_Char_Index	l_FT_Get_Char_Index
#define FT_Load_Glyph		l_FT_Load_Glyph
#define FT_Render_Glyph		l_FT_Render_Glyph
#define FT_Get_Glyph		l_FT_Get_Glyph
#define FT_Glyph_To_Bitmap	l_FT_Glyph_To_Bitmap
#define FT_Done_Glyph		l_FT_Done_Glyph

#define png_create_write_struct		l_png_create_write_struct
#define png_create_info_struct		l_png_create_info_struct
#define png_init_io					l_png_init_io
#define png_set_IHDR				l_png_set_IHDR
#define png_write_end				l_png_write_end
#define png_set_longjmp_fn			l_png_set_longjmp_fn
#define png_set_rows				l_png_set_rows
#define png_write_png				l_png_write_png
#define png_destroy_write_struct	l_png_destroy_write_struct


void *so_zlib;
void *so_libpng;
void *so_freetype;

int (*l_FT_Init_FreeType)(FT_Library *);
int (*l_FT_New_Face)(FT_Library, const char *, FT_Long, FT_Face *);
int (*l_FT_Set_Pixel_Sizes)(FT_Face, FT_UInt, FT_UInt);
int (*l_FT_Load_Char)(FT_Face, FT_ULong, FT_Int32);
int (*l_FT_Get_Char_Index)(FT_Face face, FT_ULong charcode);
int (*l_FT_Load_Glyph)(FT_Face face, FT_UInt glyph_index, FT_Int32 load_flags);
int (*l_FT_Render_Glyph)(FT_GlyphSlot slot, FT_Render_Mode  render_mode);
int (*l_FT_Get_Glyph)(FT_GlyphSlot slot, FT_Glyph *aglyph);
int (*l_FT_Glyph_To_Bitmap)(FT_Glyph* the_glyph, FT_Render_Mode render_mode, FT_Vector* origin, FT_Bool destroy);
int (*l_FT_Done_Glyph)(FT_Glyph glyph);

png_structp (*l_png_create_write_struct)(const char *, void *, void *, void *);
png_infop (*l_png_create_info_struct)(void *);
void (*l_png_init_io) (png_structp png_ptr, FILE *fp);
void (*l_png_set_IHDR)(void *, void *, unsigned int, unsigned int, int, int, int, int, int);
void (*l_png_write_end)(void *, void *);
jmp_buf* (*l_png_set_longjmp_fn)(png_structrp png_ptr, png_longjmp_ptr longjmp_fn, size_t jmp_buf_size);
void (*l_png_set_rows)(png_const_structrp png_ptr, png_inforp info_ptr, png_bytepp row_pointers);
void (*l_png_write_png)(png_structrp png_ptr, png_inforp info_ptr, int transforms, png_voidp params);
void (*l_png_destroy_write_struct)(void *, void *);

char* load_zlib(){
    so_zlib=dlopen(PATH_ZLIB, RTLD_LAZY);
	return dlerror();
}

char* load_libpng(){

    so_libpng=dlopen(PATH_LIBPNG, RTLD_LAZY);
	if(!so_libpng){
		return dlerror();
	}
    
	char* error;
	*(void**)(&l_png_create_write_struct) = dlsym(so_libpng, "png_create_write_struct");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_init_io) = dlsym(so_libpng, "png_init_io");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_create_info_struct) = dlsym(so_libpng, "png_create_info_struct");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_set_IHDR) = dlsym(so_libpng, "png_set_IHDR");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_write_end) = dlsym(so_libpng, "png_write_end");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_set_longjmp_fn) = dlsym(so_libpng, "png_set_longjmp_fn");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_set_rows) = dlsym(so_libpng, "png_set_rows");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_write_png) = dlsym(so_libpng, "png_write_png");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_png_destroy_write_struct) = dlsym(so_libpng, "png_destroy_write_struct");

	return dlerror();
}



char* load_freetypelib() {

    so_zlib=dlopen(PATH_FREETYPE, RTLD_LAZY);
	if(!so_zlib){
		return dlerror();
	}

	char* error;
	*(void**)(&l_FT_Init_FreeType) = dlsym(so_zlib, "FT_Init_FreeType");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_New_Face) = dlsym(so_zlib, "FT_New_Face");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Set_Pixel_Sizes) = dlsym(so_zlib, "FT_Set_Pixel_Sizes");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Load_Char) = dlsym(so_zlib, "FT_Load_Char");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Get_Char_Index) = dlsym(so_zlib, "FT_Get_Char_Index");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Load_Glyph) = dlsym(so_zlib, "FT_Load_Glyph");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Render_Glyph) = dlsym(so_zlib, "FT_Render_Glyph");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Get_Glyph) = dlsym(so_zlib, "FT_Get_Glyph");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Glyph_To_Bitmap) = dlsym(so_zlib, "FT_Glyph_To_Bitmap");
	if ((error=dlerror())!=NULL)
		return error;
	*(void**)(&l_FT_Done_Glyph) = dlsym(so_zlib, "FT_Done_Glyph");

	return dlerror();
}

#endif

FT_Glyph get_glyph(FT_Face face, uint32_t charcode)
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

void get_size(std::wstring text, unsigned int *max_h, unsigned int *max_w, unsigned int *bottom, unsigned int *left){
	*max_h=0;
	*max_w=0;
	*left=0;
	*bottom=0;

	std::vector <int> checked_char(256, 0);
	for (std::wstring::iterator element = text.begin(); element!=text.end(); element++)
		if(checked_char[*element]==0){//проверяли ли этот символ
			checked_char[*element]=1;

			FT_Glyph glyph = get_glyph(ft_face, *element);
		
			FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
			FT_Bitmap bitmap = bitmap_glyph->bitmap;
			
			*left = std::max(*left, static_cast<unsigned int>(abs(bitmap_glyph->left)));
			*bottom = std::max(*bottom, static_cast<unsigned int>(bitmap.rows-bitmap_glyph->top));
			*max_h =  std::max(*max_h, static_cast<unsigned int>(bitmap_glyph->top));
			*max_w =  std::max(*max_w, static_cast<unsigned int>(bitmap.width));

			 FT_Done_Glyph(glyph);
		}
	}

int createPNG(std::vector<std::vector<unsigned char>> image, unsigned int height, unsigned int width, std::string image_name){
	FILE *fp=fopen(image_name.c_str(), "wb");
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
		
	unsigned char** c_image = (unsigned char**)malloc(image.size()*sizeof(unsigned char**));
	for (unsigned int i = 0; i < image.size(); i++){
		c_image[i] = image[i].data();
	}

	 png_set_rows(png_ptr, png_info, c_image);
	 png_write_png(png_ptr, png_info, PNG_TRANSFORM_IDENTITY, NULL);
	 png_write_end(png_ptr, png_info);

	 png_destroy_write_struct(&png_ptr, NULL);
	fclose(fp);
	return 0;
}

void load_font(std::string& font_file){
	 FT_Init_FreeType(&ft_library);
	 FT_New_Face(ft_library, font_file.c_str(), 0, &ft_face);
	 FT_Set_Pixel_Sizes(ft_face, 0, FONT_SIZE);
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

    #ifdef _DYNAMIC_
    char* ret_error;

	if ((ret_error=load_zlib())){
		return 1;
	}

	if ((ret_error=load_libpng())){
		std::cout << "[libpng.so]: Error load (Status: " << ret_error << ")" << std::endl;
		dlclose(so_zlib);
		return 1;
	}


	if ((ret_error=load_freetypelib())){
		std::cout << "[freetypelib.so]: Error load (Status: " << ret_error << ")" << std::endl;
		dlclose(so_zlib);
		dlclose(so_freetype);
		return 1;
	}

	std::cout << "Load Time: " << clock()-startTime << std::endl;

    #endif

	FILE *test_font=fopen(font_name.c_str(), "rb");
	if(!test_font){
		std::cout << "Bad font name!"<< std::endl;
		return 1;
	}
	fclose(test_font);

	if(text.length()>=100){
		printf("Very large text! (limit 100)\n");
		return 1;
	}

	load_font(font_name);

	unsigned int height;
	unsigned int width;
	unsigned int left_space;
	unsigned int bottom_space;

	get_size(w_text, &height, &width, &bottom_space, &left_space);

	unsigned int space_char = FONT_SIZE / 2;
	unsigned int pos_x = left_space;
	unsigned int pos_y = 0;

	unsigned int height_string = height + bottom_space + LINE_SPACING;
	unsigned int shift = FONT_SIZE / 6;
	unsigned int number_string = w_text.length() / MAX_CHAR_IN_STRING;
	if (w_text.length() % MAX_CHAR_IN_STRING!=0) 
		number_string++;

	height = LINE_SPACING + number_string * height_string + bottom_space;

	unsigned int num_letter_string = 0;
	if (MAX_CHAR_IN_STRING > text.length())
		num_letter_string = text.length() - 1;
	else num_letter_string = MAX_CHAR_IN_STRING-1;

	width = (width+left_space) * num_letter_string;

	std::vector<unsigned char> line(3 * width, 0xFF);
	std::vector<std::vector<unsigned char>> alfaImage(width, line);

	unsigned int new_width = 0;
	for(int i = 0; i < w_text.length(); i++ ){
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
						alfaImage[img_y][img_x*3+k] -= bitmap.buffer[pos_buf];
				}
			}

		pos_x += bitmap.width + bitmap_glyph->left + left_space;
		 FT_Done_Glyph(glyph);
	}
	
	width=new_width;

	for (std::vector<std::vector <unsigned char>>::iterator element = alfaImage.begin(); element!=alfaImage.end(); element++)
		 	element->resize(width*3);

	if(createPNG(alfaImage, height, width, (char*)image_name.c_str())!=0){
		std::cout << "Error save to file!" << std::endl;
	}

	clock_t endTime = clock();

	std::cout << "Procces finished!" << std::endl;

	std::cout << "Total Time: " << endTime-startTime << std::endl;
	return 0;
}