#include "GameLib/Framework.h"
#include "State.h"
//#include "Image.h"

#include <algorithm>

using namespace GameLib;

State::State(const char* stageData, int stageSize) {

	setSize(stageData, stageSize);
	mObjects.setSize(mStageWidth, mStageHeight);
	mGoalFlags.setSize(mStageWidth, mStageHeight);

	//画像の読み込み
	mImg = new Image("Images.dds");


	//mObjects,mGoalFlagsの初期化 いるの？？
	for (int y = 0; y < mStageHeight; ++y) {
		for (int x = 0; x < mStageWidth; ++x) {
			mObjects(x, y) = OBJ_WALL;
			mGoalFlags(x, y) = false;
		}
	}

	int x = 0;
	int y = 0;
	for (int i = 0; i < stageSize; i++) {
		Object t;
		bool goalFlag = false;
		switch (stageData[i]) {
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case 'O': t = OBJ_BLOCK; goalFlag = true; break;
		case '.': t = OBJ_SPACE; goalFlag = true; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN; goalFlag = true; break;
		case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
		default: t = OBJ_UNKNOWN; break;
		}

		if (t == OBJ_MAN) {
			mPx = x;
			mPy = y;
		}

		if (t != OBJ_UNKNOWN) {
			mObjects(x, y) = t;
			mGoalFlags(x, y) = goalFlag;
			x++;
		}
	}
}

void State::setSize(const char* stage, int size) {
	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		switch (stage[i]) {
		case '#': case 'o': case 'O': case '.':
		case 'p': case 'P': case ' ':
			x++;
			break;
		case '\n':
			y++;
			mStageWidth = std::max(mStageWidth, x);
			mStageHeight = std::max(mStageHeight, y);
			x = 0;
			break;
		}
	}
}

bool State::clearCheck() const {
	for (int y = 0; y < mStageHeight; y++) {
		for (int x = 0; x < mStageWidth; x++) {
			if (mObjects(x, y) == OBJ_BLOCK && mGoalFlags(x, y) == false) return false;
		}
	}
	return true;
}


void State::drawCell(int x, int y, ImageID imageID) const {
	mImg->drawPicture(mCellWidth * x, mCellHeight * y, imageID * mCellWidth, 0, mCellWidth, mCellHeight);
}

void State::drawStage() const {
	for (int y = 0; y < mStageHeight; y++) {
		for (int x = 0; x < mStageWidth; x++) {
			bool goalFlag = mGoalFlags(x, y);
			switch (mObjects(x, y)) {
			case OBJ_WALL: drawCell(x, y, IMAGE_ID_WALL); break;
			case OBJ_SPACE:
				if (goalFlag == true)  drawCell(x, y, IMAGE_ID_GOAL);
				else drawCell(x, y, IMAGE_ID_SPACE);
				break;
			case OBJ_BLOCK:
				if (goalFlag == true) { drawCell(x, y, IMAGE_ID_GOAL); drawCell(x, y, IMAGE_ID_BLOCK); }
				else { drawCell(x, y, IMAGE_ID_SPACE); drawCell(x, y, IMAGE_ID_BLOCK);}
				break;
			case OBJ_MAN:
				if (goalFlag == true) { drawCell(x, y, IMAGE_ID_GOAL); drawCell(x, y, IMAGE_ID_PLAYER); }
				else { drawCell(x, y, IMAGE_ID_SPACE); drawCell(x, y, IMAGE_ID_PLAYER);}
				break;
			case OBJ_UNKNOWN: break;
			default: break;
			}
		}
	}
}

void State::update() {
	Framework f = Framework::instance();
	int dPx = 0, dPy = 0;

	//入力が何もない場合のmPreviousKeyのリセット
	if (!f.isKeyOn('a') && !f.isKeyOn('s') && !f.isKeyOn('d') && !f.isKeyOn('w')) mPreviousKey = '_';//どれでもない何か

	if (f.isKeyOn('a') && mPreviousKey != 'a') {dPx = -1; mPreviousKey = 'a';}
	else if (f.isKeyOn('s') && mPreviousKey != 's') {dPy = 1; mPreviousKey = 's';}
	else if (f.isKeyOn('d') && mPreviousKey != 'd') {dPx = 1; mPreviousKey = 'd'; }
	else if (f.isKeyOn('w') && mPreviousKey != 'w') {dPy = -1; mPreviousKey = 'w'; };


	//範囲チェック
	if (mPx + dPx < 0 || mStageWidth <= mPx + dPx || mPy + dPy < 0 || mStageWidth <= mPy + dPy) return;
	switch (mObjects(mPx + dPx, mPy + dPy)) {
	case OBJ_SPACE:
		mObjects(mPx + dPx, mPy + dPy) = OBJ_MAN;
		mObjects(mPx, mPy) = OBJ_SPACE;
		mPx += dPx; mPy += dPy;
		break;
	case OBJ_BLOCK:
		//範囲チェック
		if (mPx + 2 * dPx < 0 || mStageWidth <= mPx + 2 * dPx || mPy + 2 * dPy < 0 || mStageWidth <= mPy + 2 * dPy) {
			return;
		}
		switch (mObjects(mPx + 2 * dPx, mPy + 2 * dPy)) {
			//'#','o','O'の場合はその場に残る
		case OBJ_SPACE:
			mObjects(mPx + 2 * dPx, mPy + 2 * dPy) = OBJ_BLOCK;
			mObjects(mPx + dPx, mPy + dPy) = OBJ_MAN;
			mObjects(mPx, mPy) = OBJ_SPACE;
			mPx += dPx; mPy += dPy;
			break;
		}
		break;
	}
}