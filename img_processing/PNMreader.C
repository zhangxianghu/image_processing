#include <PNMreader.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

PNMreader::PNMreader(char *filename) {
	file = new char[strlen(filename) + 1];
	for(int i = 0; i < strlen(filename) + 1; i++) {
		file[i] = filename[i];
	}

	s_width = 0;
	s_height = 0;
}

void PNMreader::Execute() {
	Image *image = GetOutput();

	if(image == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	ifstream f_in;
	f_in.open(file);
	char magicNumber[128];
	int input_width, input_height, max_number;
    
    f_in >> magicNumber;
    int flag = strncmp(magicNumber, "P6", 2);
    if(flag != 0){
        char msg[1024];
        sprintf(msg,"%s: no pnm format!", SourceName());
        DataFlowException e(SourceName(),msg);
        throw e;
    }

    f_in >> input_width >> input_height;
    f_in >> max_number;
	f_in.ignore(256,'\n');
    
    image->set_width(input_width);
	image->set_height(input_height);
	image->set_data(input_width, input_height);

    f_in.read((char *)image->get_data(), input_width * input_height * 3);
    f_in.close();
	
	// fscanf(f_in, "P6\n%d %d\n255\n", &s_width, &s_height);
	// image = (Image *)malloc(sizeof(struct Image));
	// printf("%d\n", image->width);
	// image->set_width(s_width);
	// image->set_height(s_height);
	// image->set_data(s_width, s_height);
	// printf("%d, %d, %s\n", image->width, image->height, image->data);
	// fread(image->get_data(), sizeof(unsigned char), image->get_width() * image->get_height() * 3, f_in);
	// fclose(f_in);
}

PNMreader::~PNMreader() {
	delete[] file;
}