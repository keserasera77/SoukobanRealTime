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
	Image* mImg; //�摜�f�[�^
	int mPx, mPy; //�v���C���[�̍��W
	Array2D<Object> mObjects;
	char mPrevKey;
  double mMoveCount; //�����n�߂Ă��牽��f�ړ��������B 0 <= mMovingPlace < 32
};
