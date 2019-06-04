//Stateのメンバ変数mPx,mPyや、gCellEdgeを用いている点がサンプルコードと違う。

#include "GameLib/Framework.h"
#include <iostream>
//#include "Image.h"
#include "State.h"

using namespace GameLib;

void mainLoop();

State* gState = 0;
int gCount = 0; //mainLoopのループ数

namespace GameLib {
	void Framework::update() {
	 instance().sleep(1);
   mainLoop();	
	 }
}

void mainLoop() {
  Framework f = Framework::instance();
	bool cleared = false;

	if (f.isEndRequested()) {
	  delete gState;
		gState = 0;
		return;
	}

	if (!gState) {
		//ステージの読み込み
		File stage("stageData.txt");
		if (!stage.data()) {
		  cout << "stagefile could not be read";
		  return;
		}
	  gState = new State(stage.data(), stage.size());
		gState->drawStage();
		return;
	}

	//フレームレート計測
	unsigned currentTime = f.time();
	unsigned frameTime = (currentTime - gState->mPrevTime);
	unsigned frameRate = 1000 / frameTime;
	if (gCount % 60 == 0) cout << "frameRate :: " << frameRate << " (fps)" << endl;
	gCount++;
	//while (f.time() < gState->mPrevTime + 16) f.sleep(1); // 固定フレームレート frameRateの調整 62.5fps
	gState->mPrevTime = currentTime;

	if(gState->clearCheck()) {
	  cleared = true;
	}

	gState->update(frameTime);
  gState->drawStage();

	//ステージクリアーかチェック
	if (cleared) {
		cout << "Congratulation!!!" << endl;
		delete gState;
		gState = 0;
	};

	//終了
	if (f.isKeyOn('q')) {
		f.requestEnd();
	}

}

