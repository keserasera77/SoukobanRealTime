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
	State(const char* stageData, int stageSize);
	//~State();
	void update();
	void drawStage() const;
	bool clearCheck() const;
private:
	enum Object {
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_GOAL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	enum ImageID {
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	int mStageWidth;
	int mStageHeight;
	char* mStageData;
	int mPx, mPy; //プレイヤーの座標
	Image* mImg; //画像データ
	char mPreviousKey;
	int mCellHeight = 32, mCellWidth = 32;
	Array2D<Object> mObjects;
	Array2D<bool> mGoalFlags;
	void drawCell(int x, int y, ImageID imageID) const;
	void setSize(const char* stage, int size);
};
