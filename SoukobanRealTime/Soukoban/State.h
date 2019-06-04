#pragma once

#include "Image.h"

template<class T> class Array2D {
public:
	Array2D() : mArray(0) {}
	~Array2D() {
		delete[] mArray;
		mArray = 0;
	}
	void setSize(int width, int height) {
		mWidth = width;
		mHeight = height;
		mArray = new T[width * height];
	}
	T& operator()(int x, int y) {
		return mArray[y * mWidth + x];
	}
	const T& operator()(int x, int y) const {
		return mArray[y * mWidth + x];
	}
private:
	T* mArray;
	int mWidth;
	int mHeight;
};

class State {
public:
	unsigned mPrevTime = 0;
	State(const char* stageData, int stageSize);
	//~State();
	void update(unsigned frameTime);
	void drawStage() const;
	bool clearCheck() const;
private:
  class Object;
	void setSize(const char* stage, int size);

	int mStageWidth;
	int mStageHeight;
	char* mStageData;
	Image* mImg; //画像データ
	int mPx, mPy; //プレイヤーの座標
	Array2D<Object> mObjects;
	char mPrevKey;
  double mMoveCount; //動き始めてから何画素移動したか。 0 <= mMovingPlace < 32
};
