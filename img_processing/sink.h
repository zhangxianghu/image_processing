#ifndef SINK_H
#define SINK_H

#include "image.h"

class Sink {
	protected:
		const Image *img2;
		const Image *img3;
	public: 
		Sink();
		// Image *get_writer_pointer2();		//return the output pointer2 to write 
		// Image *get_writer_pointer3();		//return the output pointer3 to write
		void SetInput(const Image *);	//Set img2 value, let it point to the input pointer image2
		void SetInput2(const Image *);	//Set img3 value, let it point to the input pointer image3
		virtual const char *SinkName() = 0;
};

#endif