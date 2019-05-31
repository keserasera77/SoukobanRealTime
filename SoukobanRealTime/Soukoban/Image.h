#include "File.h"

class Image {
public:
	Image(const char* filename);
	~Image();
	int width() const;
	int height() const;
	void drawPicture(int dstX, int dstY, int srcX, int srcY, int width, int height) const;
private:
	int mWidth;
	int mHeight;
	unsigned* mData;
};