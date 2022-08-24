#include "sink.h"
#include <stdlib.h>

Sink::Sink() {
	img2 = NULL;
	img3 = NULL;
}

// Image *Sink::get_writer_pointer2() {
// 	return img2;
// }

// Image *Sink::get_writer_pointer3() {
// 	return img3;
// }

void Sink::SetInput(const Image *image2) {
	this->img2 = image2;
}

void Sink::SetInput2(const Image *image3) {
	this->img3 = image3;
}