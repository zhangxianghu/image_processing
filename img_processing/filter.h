#ifndef FILTER_H
#define FILTER_H

// #include "image.h"
#include <source.h>
#include <sink.h>

class Filters: public Source, public Sink {
	public:
    	virtual void Update();
    	
    	// Adding logger
    	virtual const char *FilterName()=0;

    	const char *SourceName(){
    		return FilterName();
    	};
    	const char *SinkName(){
    		return FilterName();
    	};
};

class Shrinker: public Filters {
	public:
		virtual void Execute();
		const char *FilterName() {
			return "Shrinker";
		};
};

class LRCombine: public Filters {
	public:
		virtual void Execute();
		const char *FilterName() {
			return "LRCombine";
		};
};

class TBCombine: public Filters {
	public:
		virtual void Execute();
		const char *FilterName() {
			return "TBCombine";
		};
};

class Blender: public Filters {
	private:
		double factor;
	public:
		void SetFactor(double factor);
		double get_factor();
		virtual void Execute();
		const char *FilterName() {
			return "Blender";
		};
};

class Mirror: public Filters {
	public: 
		virtual void Execute();
		const char *FilterName() {
			return "Mirror";
		};
};

class Rotate: public Filters
{
    public:
        virtual void Execute();
        const char *FilterName() {
        	return "Rotate";
        };
};

class Subtract: public Filters
{
    public:
        virtual void Execute();
        const char *FilterName() {
        	return "Subtract";
        };
};

class Grayscale: public Filters
{
    public:
        virtual void Execute();
        const char *FilterName() {
        	return "Grayscale";
        };
};

class Blur: public Filters
{
    public:
        virtual void Execute();
        const char *FilterName() {
        	return "Blur";
        };
};

class CheckSum: public Sink
{
	private:
		unsigned char red, green, blue;
	public:
		CheckSum(){
			red = 0;
			green = 0;
			blue = 0;
		}

		void OutputCheckSum(const char*); 
		const char *SinkName(){
			return "CheckSum";
		};
};

class Color: public Source{
	protected:
    	int color_width, color_height, red, green, blue;
    public:
    	Color(int widht_input, int height_input, int red_input, int green_input, int blue_input){
    		color_width = widht_input;
    		color_height = height_input;
    		red = red_input;
    		green = green_input;
    		blue = blue_input;
    	}
    	virtual void Execute();
        const char *SourceName() {
        	return "ConstantColor";
        };
};

#endif






