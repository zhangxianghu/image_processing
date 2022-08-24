#include <PNMwriter.h>
#include <stdio.h>

void PNMwriter::Write(char *filename) {
	FILE *f_out = fopen(filename, "wb");
	// Image *image = get_writer_pointer2();
	fprintf(f_out, "P6\n%d %d\n255\n", img2->get_width(), img2->get_height());
	fwrite(img2->get_data(), sizeof(unsigned char), img2->get_width() * img2->get_height() * 3, f_out);
	fclose(f_out);
}