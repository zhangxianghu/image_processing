#include "image.h"
#include <stdlib.h>
#include <source.h>

Image::Image(void) {
	width = 0;
	height = 0;
	data = NULL;
}

Image::Image(int w, int h) {
	width = w;
	height = h;
	unsigned char *data = new unsigned char[width*height*3];
	// data = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));
}

Image::Image(Image &img) {
	this->width = img.width;
	this->height = img.height;
	// data = (unsigned char*)malloc(width*height*3*sizeof(unsigned char));
	unsigned char *data = new unsigned char[width*height*3];
	for(int i = 0; i < img.width * img.height * 3; i++) {
		data[i] = img.data[i];
	}
}

int Image::get_width() const{
	return this->width;
}

int Image::get_height() const{
	return this->height;
}

unsigned char *Image::get_data() const{
	return this->data;
}

void Image::set_width(int size_w) {
	this->width = size_w;
}

void Image::set_height(int size_h) {
	this->height = size_h;
}

void Image::set_data(int data_width, int data_height) {
	if(data != NULL){
		delete[] data;
	}
	data = NULL;
	this->data = new unsigned char[data_width * data_height * 3];
}

void Image::ResetSize(int w, int h) {
	this->width = w;
	this->height = h;
}

void Image::SetSrc(Source* src){
	this->src = src;
}

void Image::Update() const{
	if(src != NULL) {
		src->Update();
	}
}

Image::~Image(){
	// free(data);
	delete[] data;
};










