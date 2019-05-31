//製作時間::ちょうど5時間ほど
//改良::1時間  元々のmoveの役割をmoveWithInputへ変更し、moveWithInputの部品としてmoveを追加。さらにその部品にleaveを追加
//サンプルコードを参考に、ステージの表現方法の改変、Array2D,Stateの追加、移動時の範囲チェックの追加、readFileの追加
//Stateのメンバ変数mPx,mPyを追加

#include "GameLib/Framework.h"
#include <iostream>
//#include "Image.h"
#include "State.h"

using namespace GameLib;

void mainLoop();
//State gStage;

State* gState = 0;

namespace GameLib {
	void Framework::update() {
   mainLoop();	
	 }
}

void mainLoop() {
  Framework f = Framework::instance();

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

	if (f.isKeyOn('q')) {
		f.requestEnd();
	}

	gState->update();
	gState->drawStage();

	//ステージクリアーかチェック
	if (gState->clearCheck()) {
	  cout << "Congratulation!!!" << endl;
		delete gState;
		gState = 0;
		return;
	};

}

