#ifndef SOURCE_H
#define SOURCE_H

#include <image.h>
#include <logging.h>
// #include "image.h"
// class HANK;
class Source {
	protected:
		Image img;
		virtual void Execute() = 0;
	public:
		Image *GetOutput();
		Source(){img.SetSrc(this);};
		// void SetInput(Image *image1);
		virtual void Update();
		virtual const char *SourceName() = 0; 
};

#endif