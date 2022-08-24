#include <filter.h>
#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <string.h>
#include <vector>

using namespace std;

static vector<Image*> filter_test;

void Filters::Update()
{
	if(img2 != NULL){
		char msg[1024];
		sprintf(msg, "%s: about to update input1", SourceName());
		Logger::LogEvent(msg);
		
		int fsize_21 = filter_test.size();
        filter_test.push_back(GetOutput());
		int fszie_22 =filter_test.size();

        if (filter_test.size() != 0){
        	for (int i = 0; i < filter_test.size(); i++){
        		if (filter_test[i] == img2){
        			char msg[1024];
					sprintf(msg, "%s: input points to same object's output!", SourceName());
					DataFlowException e(SourceName(),msg);
					throw e;
        		}
        	}
        }

		img2->Update();
		sprintf(msg, "%s: done updating input1", SourceName());
		Logger::LogEvent(msg);
		if (fsize_21 != fszie_22 && filter_test.size() != 0){
        	filter_test.pop_back();
        }
	}
	if(img3 != NULL){
		char msg[1024];
		sprintf(msg, "%s: about to update input2", SourceName());
		Logger::LogEvent(msg);

		int fsize_31 = filter_test.size();
        filter_test.push_back(GetOutput());
		int fszie_32 =filter_test.size();

        if (filter_test.size() != 0){
        	for (int i = 0; i < filter_test.size(); i++){
        		if (filter_test[i] == img3){
        			char msg[1024];
					sprintf(msg, "%s: input points to same object's output!", SourceName());
					DataFlowException e(SourceName(),msg);
					throw e;
        		}
        	}
        }

		img3 ->Update();
		sprintf(msg, "%s: done updating input2", SourceName());
		Logger::LogEvent(msg);
        if (fsize_31 != fszie_32 && filter_test.size() != 0){
        	filter_test.pop_back();
        }
	}

	char msg[1024];
    sprintf(msg, "%s: about to execute", SourceName());
    Logger::LogEvent(msg);
	Execute();
	sprintf(msg, "%s: done executing", SourceName());
    Logger::LogEvent(msg);
}

void Shrinker::Execute() {
	//shrinker's input is reader's output, use get_writer_pointer() to get this pointer
	// Image *input = get_writer_pointer2();
	//writer's input is shrinker's output, use GetOutput() to get this pointer
	Image *output = GetOutput();

	if(img2 == NULL){
		char msg[1024];
		sprintf(msg, "%s: no input!", SinkName());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = ((int) img2->get_width()) / 2;
	int output_height = ((int) img2->get_height()) / 2;

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	int i, j;
    for (int i=0; i < output_width; i++){
    	for(int j =0; j < output_height; j++){
    		output->get_data()[3 * (j * output_width + i) + 0] = img2->get_data()[3 * (2 * j * img2->get_width() + 2*i) + 0];
    		output->get_data()[3 * (j * output_width + i) + 1] = img2->get_data()[3 * (2 * j * img2->get_width() + 2*i) + 1];
    		output->get_data()[3 * (j * output_width + i) + 2] = img2->get_data()[3 * (2 * j * img2->get_width() + 2*i) + 2];
    	}
    }

	// for(int i = 0; i < output_height; i++) {
	// 	for(int j = 0; j < output_width; j++) {
	// 		output->get_data()[3 * (i * output_width + j) + 0] = img2->get_data()[3 * (4 * i * output_width + 2 * j) + 0];
	// 		output->get_data()[3 * (i * output_width + j) + 1] = img2->get_data()[3 * (4 * i * output_width + 2 * j) + 1];
	// 		output->get_data()[3 * (i * output_width + j) + 2] = img2->get_data()[3 * (4 * i * output_width + 2 * j) + 2];
	// 	}
	// }
}

void LRCombine::Execute() {
	// Image *leftinput = get_writer_pointer2();
	// Image *rightinput = get_writer_pointer3();
	Image *output = GetOutput();

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}
	if(img2 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input1!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    if(img3 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input2!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if(img2->get_height() != img3->get_height()){
		char msg[1024];
		sprintf(msg,"%s: heights must match: %d, %d", SinkName(),img2->get_height(),img3->get_height());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	int output_width = img2->get_width() + img3->get_width();
	int output_height = img2->get_height();
	
	output->set_width(output_width);
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for(int i = 0; i < output->get_height(); i++) {
		for(int j = 0; j < output->get_width(); j++) {
			if(j < img2->get_width()) {
				output->get_data()[3 * (i * output->get_width() + j) + 0] = img2->get_data()[3 * (i * img2->get_width() + j) + 0];
				output->get_data()[3 * (i * output->get_width() + j) + 1] = img2->get_data()[3 * (i * img2->get_width() + j) + 1];
				output->get_data()[3 * (i * output->get_width() + j) + 2] = img2->get_data()[3 * (i * img2->get_width() + j) + 2];
			} else {
				output->get_data()[3 * (i * output->get_width() + j) + 0] = img3->get_data()[3 * (i * img3->get_width() + (j - img2->get_width())) + 0];
				output->get_data()[3 * (i * output->get_width() + j) + 1] = img3->get_data()[3 * (i * img3->get_width() + (j - img2->get_width())) + 1];
				output->get_data()[3 * (i * output->get_width() + j) + 2] = img3->get_data()[3 * (i * img3->get_width() + (j - img2->get_width())) + 2];
			}
		}
	}
}

void TBCombine::Execute() {
	// Image *topInput = get_writer_pointer2();
	// Image *bottomInput = get_writer_pointer3();
	Image *output = GetOutput();

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	if(img2 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input1!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    if(img3 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input2!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if(img2->get_width() != img3->get_width()){
		char msg[1024];
		sprintf(msg,"%s: widths must match: %d, %d", SinkName(),img2->get_width(),img3->get_width());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	int output_width = img2->get_width();
	int output_height = img2->get_height() + img3->get_height();
	
	output->set_width(output_width);
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for(int i = 0; i < output->get_height(); i++) {
		for(int j = 0; j < output->get_width(); j++) {
			if(i < img2->get_height()) {
				output->get_data()[3 * (i * output->get_width() + j) + 0] = img2->get_data()[3 * (i * img2->get_width() + j) + 0];
				output->get_data()[3 * (i * output->get_width() + j) + 1] = img2->get_data()[3 * (i * img2->get_width() + j) + 1];
				output->get_data()[3 * (i * output->get_width() + j) + 2] = img2->get_data()[3 * (i * img2->get_width() + j) + 2];
			} else {
				output->get_data()[3 * (i * output->get_width() + j) + 0] = img3->get_data()[3 * ((i - img2->get_height()) * img3->get_width() + j) + 0];
				output->get_data()[3 * (i * output->get_width() + j) + 1] = img3->get_data()[3 * ((i - img2->get_height()) * img3->get_width() + j) + 1];
				output->get_data()[3 * (i * output->get_width() + j) + 2] = img3->get_data()[3 * ((i - img2->get_height()) * img3->get_width() + j) + 2];
			}
		}
	}
}

void Blender::SetFactor(double factor) {
	this->factor = factor;	
}

double Blender::get_factor() {
	return this->factor;
}

void Blender::Execute() {
	// Image *input1 = get_writer_pointer2();
	// Image *input2 = get_writer_pointer3();
	Image *output = GetOutput();

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	if(img2 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input1!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    if(img3 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input2!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if(img2->get_width() != img3->get_width()){
		char msg[1024];
		sprintf(msg,"%s: widths must match: %d, %d", SinkName(),img2->get_width(),img3->get_width());
		DataFlowException e(SinkName(),msg);
		throw e;
	}
	
	if(img2->get_height() != img3->get_height()){
		char msg[1024];
		sprintf(msg,"%s: heights must match: %d, %d", SinkName(),img2->get_height(),img3->get_height());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	double factor1 = this->get_factor();
	double factor2 = 1 - factor1;

	if (factor < 0 || factor > 1){
    	char msg[1024];
    	sprintf(msg, "Invalid factor for Blender: %f", factor);
    	DataFlowException e(SinkName(), msg);
    	throw e;
    }

	int output_width = img2->get_width();
	int output_height = img3->get_height();
	
	output->set_width(output_width);
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for(int i = 0; i < output->get_height(); i++) {
		for(int j = 0; j < output->get_width(); j++) {
			output->get_data()[3 * (i * output->get_width() + j) + 0] = img2->get_data()[3 * (i * output->get_width() + j) + 0] * factor1 + img3->get_data()[3 * (i * output->get_width() + j) + 0] * factor2;
			output->get_data()[3 * (i * output->get_width() + j) + 1] = img2->get_data()[3 * (i * output->get_width() + j) + 1] * factor1 + img3->get_data()[3 * (i * output->get_width() + j) + 1] * factor2;
			output->get_data()[3 * (i * output->get_width() + j) + 2] = img2->get_data()[3 * (i * output->get_width() + j) + 2] * factor1 + img3->get_data()[3 * (i * output->get_width() + j) + 2] * factor2;
		}
	}
}

void Mirror::Execute() {
	// Image *input = get_writer_pointer2();
	Image *output = GetOutput();

	if(img2 == NULL){
		char msg[1024];
		sprintf(msg, "%s: no input!", SinkName());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = img2->get_width();
	int output_height = img2->get_height();

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for(int i = 0; i < output_height; i++) {
		for(int j = 0; j < output_width; j++) {
			output->get_data()[3 * (i * output_width + j) + 0] = img2->get_data()[3 * (i * output_width + output_width - 1 - j) + 0];
			output->get_data()[3 * (i * output_width + j) + 1] = img2->get_data()[3 * (i * output_width + output_width - 1 - j) + 1];
			output->get_data()[3 * (i * output_width + j) + 2] = img2->get_data()[3 * (i * output_width + output_width - 1 - j) + 2];
		}
	}
}

void Rotate::Execute() {
	// Image *input = get_writer_pointer2();
	Image *output = GetOutput();

	if(img2 == NULL){
		char msg[1024];
		sprintf(msg, "%s: no input!", SinkName());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = img2->get_height();
	int output_height = img2->get_width();

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for (int i = 0; i < output_width; i++){
    	for(int j = 0;j < output_height; j++){
    		int p = output_width - i -1; //input's j
    		int q = j;            //input's i
    		output->get_data()[3 * (j * output_width + i) + 0] = img2->get_data()[3 * ((output_width - i -1) * output_height + j) + 0];
    		output->get_data()[3 * (j * output_width + i) + 1] = img2->get_data()[3 * ((output_width - i -1) * output_height + j) + 1];
    		output->get_data()[3 * (j * output_width + i) + 2] = img2->get_data()[3 * ((output_width - i -1) * output_height + j) + 2];
    	}
    }
}

void Subtract::Execute() {
	// Image *input1 = get_writer_pointer2();
	// Image *input2 = get_writer_pointer3();
	Image *output = GetOutput();

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	if(img2 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input1!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }
    if(img3 == NULL){
        char msg[1024];
        sprintf(msg, "%s: no input2!", SinkName());
        DataFlowException e(SinkName(), msg);
        throw e;
    }

    if(img2->get_width() != img3->get_width()){
		char msg[1024];
		sprintf(msg,"%s: widths must match: %d, %d", SinkName(),img2->get_width(),img3->get_width());
		DataFlowException e(SinkName(),msg);
		throw e;
	}
	if(img2->get_height() != img3->get_height()){
		char msg[1024];
		sprintf(msg,"%s: heights must match: %d, %d", SinkName(),img2->get_height(),img3->get_height());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	int output_width = img2->get_width();
	int output_height = img2->get_height();

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for (int i = 0; i < output_width; i++){
		for(int j = 0; j < output_height; j++){
    		if(img2->get_data()[3 * (j * output_width + i) + 0] > img3->get_data()[3 * (j * output_width + i)] + 0){
    			output->get_data()[3 * (j * output_width + i) + 0] = img2->get_data()[3 * (j * output_width + i) + 0] - img3->get_data()[3 * (j * output_width + i) + 0];
    		}else{
    			output->get_data()[3 * (j * output_width + i) + 0] = 0;
    		}
    		if(img2->get_data()[3 * (j * output_width + i) + 1] > img3->get_data()[3 * (j * output_width + i) + 1]){
    			output->get_data()[3 * (j * output_width + i) + 1] = img2->get_data()[3 * (j * output_width + i) + 1] - img3->get_data()[3 * (j * output_width + i) + 1];
    		}else{
    			output->get_data()[3 * (j * output_width + i) + 1] = 0;
    		}
    		if(img2->get_data()[3 * (j * output_width + i) + 2] > img3->get_data()[3 * (j * output_width + i) + 2]){
    			output->get_data()[3 * (j * output_width + i) + 2] = img2->get_data()[3 * (j * output_width + i) + 2] - img3->get_data()[3 * (j * output_width + i) + 2];
    		}else{
    			output->get_data()[3 * (j * output_width + i) + 2] = 0;
    		}
    	}
    }
}


void Grayscale::Execute(){
	// Image *input = get_writer_pointer2();
	Image *output = GetOutput();

	if(img2 == NULL){
		char msg[1024];
		sprintf(msg, "%s: no input!", SinkName());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = img2->get_width();
	int output_height = img2->get_height();

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	unsigned char grayscale;
    for (int i = 0; i < output_width; i++){
    	for(int j = 0;j < output_height; j++){
    		//I not sure int or unsigned char in order to use integer division
			grayscale = (img2->get_data()[3 * (j * output_width + i)]/5) + (img2->get_data()[3 * (j * output_width + i) + 1]/2) + (img2->get_data()[3 * (j * output_width + i)+2]/4);
    		output->get_data()[3 * (j * output_width + i)] = grayscale;
    		output->get_data()[3 * (j * output_width + i) + 1] = grayscale;
    		output->get_data()[3 * (j * output_width+i) + 2] = grayscale;
    	}
    }
};

void Blur::Execute() {
	// Image *input = get_writer_pointer2();
	Image *output = GetOutput();

	if(img2 == NULL){
		char msg[1024];
		sprintf(msg, "%s: no input!", SinkName());
		DataFlowException e(SinkName(),msg);
		throw e;
	}

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = img2->get_width();
	int output_height = img2->get_height();

	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);
    
    for (int i = 0; i < output_width; i++){
    	for(int j = 0; j < output_height; j++){
    		if (i == 0 || i == output_width - 1 || j==0 || j == output_height - 1){
    			output->get_data()[3 * (j * output_width + i) + 0] = img2->get_data()[3 * (j * output_width + i + 0)];
    		    output->get_data()[3 * (j * output_width + i) + 1] = img2->get_data()[3 * (j * output_width + i) + 1];
    		    output->get_data()[3 * (j * output_width + i) + 2] = img2->get_data()[3 * (j * output_width + i) + 2];
    		}else{
    			output->get_data()[3 * (j * output_width+i) + 0] = img2->get_data()[3 * ((j - 1) * output_width + i - 1) + 0]/8 + img2->get_data()[3 * ((j - 1) * output_width + i) + 0]/8 + img2->get_data()[3 * (( j - 1) * output_width+i+1) + 0]/8 + img2->get_data()[3 * (j * output_width + i - 1) + 0]/8 + img2->get_data()[3 * (j * output_width + i + 1) + 0]/8 + img2->get_data()[3 * ((j + 1) * output_width+i - 1) + 0]/8 + img2->get_data()[3 * ((j + 1) * output_width+i) + 0]/8 + img2->get_data()[3 * ((j + 1) * output_width+i + 1) + 0]/8;
    		    output->get_data()[3 * (j * output_width+i) + 1] = img2->get_data()[3 * ((j - 1) * output_width + i - 1) + 1]/8 + img2->get_data()[3 * ((j - 1) * output_width + i) + 1]/8 + img2->get_data()[3 * (( j - 1) * output_width+i+1) + 1]/8 + img2->get_data()[3 * (j * output_width + i - 1) + 1]/8 + img2->get_data()[3 * (j * output_width + i + 1) + 1]/8 + img2->get_data()[3 * ((j + 1) * output_width+i - 1) + 1]/8 + img2->get_data()[3 * ((j + 1) * output_width+i) + 1]/8 + img2->get_data()[3 * ((j + 1) * output_width+i + 1) + 1]/8;
    		    output->get_data()[3 * (j * output_width+i) + 2] = img2->get_data()[3 * ((j - 1) * output_width + i - 1) + 2]/8 + img2->get_data()[3 * ((j - 1) * output_width + i) + 2]/8 + img2->get_data()[3 * (( j - 1) * output_width+i+1) + 2]/8 + img2->get_data()[3 * (j * output_width + i - 1) + 2]/8 + img2->get_data()[3 * (j * output_width + i + 1) + 2]/8 + img2->get_data()[3 * ((j + 1) * output_width+i - 1) + 2]/8 + img2->get_data()[3 * ((j + 1) * output_width+i) + 2]/8 + img2->get_data()[3 * ((j + 1) * output_width+i + 1) + 2]/8;
    		}
        }
    }
};

void Color::Execute(){
	Image *output = GetOutput();

	if(output == NULL){
		char msg[1024];
		sprintf(msg, "%s: no output!", SourceName());
		DataFlowException e(SourceName(),msg);
		throw e;
	}

	int output_width = color_width;
	int output_height = color_height;
	
	output->set_width(output_width); 
	output->set_height(output_height);
	output->set_data(output_width, output_height);

	for (int i = 0; i < color_width * color_height; i++){
		output->get_data()[3 * i + 0] = red;
		output->get_data()[3 * i + 1] = green;
		output->get_data()[3 * i + 2] = blue;
	}
};

void CheckSum::OutputCheckSum(const char *filename){
	// Image *input = get_writer_pointer2();
	FILE *file = fopen(filename,"w");

	int total = img2->get_width() * img2->get_height();
	for (int i = 0; i < total; i++){
		red += img2->get_data()[3 * i];
		green += img2->get_data()[3 * i + 1];
		blue += img2->get_data()[3 * i + 2];
	}
	red = red % 256;
	green = green % 256;
	blue = blue % 256;
	fprintf(file, "CHECKSUM: %d, %d, %d\n", red, green, blue);
	fclose(file);
}










