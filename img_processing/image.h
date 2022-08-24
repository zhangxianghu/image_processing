#ifndef IMAGE_H
#define IMAGE_H

// #include <source.h>
class Source;

class Image {
	private:
		unsigned char *data;
		int width;
		int height;
		Source* src;
		
	public: 
		Image(void);
		Image(int width, int height);
		Image(Image &);
		int get_width() const;
		int get_height() const;
		unsigned char *get_data() const;
		void set_width(int size_w);
		void set_height(int size_h);
		void set_data(int data_width, int data_height);

		void ResetSize(int width, int height);
		void SetSrc(Source *);
		virtual void Update() const;

		virtual ~Image();
};

#endif