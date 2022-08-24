#ifndef PNMREADER_H
#define PNMREADER_H

#include <source.h>
#include <logging.h>


class PNMreader: public Source {
	private:
		char *file;
		int s_width;
		int s_height;
	public:
		PNMreader(char *filename);
		virtual void Execute();
		~PNMreader();
		const char *SourceName() {
			return "PNMreader";
		};
};

#endif