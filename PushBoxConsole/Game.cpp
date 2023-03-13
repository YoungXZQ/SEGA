#include <fstream>
#include <iostream>
using namespace std;

//#表示墙壁 _代表空间 .代表目的地 o代表箱子 p代表玩家
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
	OBJ_SPACE,//空地
	OBJ_WALL, //墙
	OBJ_GOAL, //目标点
	OBJ_BLOCK,//箱子
	OBJ_BLOCK_ON_GOAL, //箱子到达目标点
	OBJ_MAN, //玩家
	OBJ_MAN_ON_GOAL, //玩家到达目标点

	OBJ_UNKNOWN,
};

//函数原型
//场景数据字符串转化为Object数组
void initialize(Object* state, int w, int h, const char* stageData);
//画面绘制
void draw(const Object* state, int w, int h);
//更新函数
void update(Object* state, char input, int w, int h);
//通关检测
bool checkClear(const Object* state, int w, int h);

/*int main() {
	//创建状态数组
	Object* state = new Object[gStageWidth * gStageHeight];
	//初始化场景
	initialize(state, gStageWidth, gStageHeight, gStageData);
	//主循环
	while (true) {
		//绘制
		draw(state, gStageWidth, gStageHeight);
		//通关检测
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}
		//获取输入
		cout << "a:left d:right w:up s:down. command?" << endl;
		char input;
		cin >> input;
		update(state, input, gStageWidth, gStageHeight);
	}

	//胜利通关提示
	cout << "Congratulation's! You win!" << endl;
	//移除创建的数组 delete[]而不是单delete
	delete[] state;
	//移除后指针赋值为0 避免指针bug
	state = 0; 
	
	
	//return 0;
}*/

void initialize(Object* state, int w, int h, const char* stageData) {
	//读取指针
	const char* d = stageData;
	//x代表一维数组列  y代表一维数组行 下标值为y*width+x
	int x = 0;
	int y = 0;
	//遍历字符串数组
	while (*d != '\0') {
		Object target;
		switch (*d)
		{
		case '#': target = OBJ_WALL; break;
		case ' ': target = OBJ_SPACE; break;
		case 'o': target = OBJ_BLOCK; break;
		case 'O': target = OBJ_BLOCK_ON_GOAL; break;
		case '.': target = OBJ_GOAL; break;
		case 'p': target = OBJ_MAN; break;
		case 'P': target = OBJ_MAN_ON_GOAL; break;
		case '\n': {
			x = 0;
			++y;
			//没有数据
			target = OBJ_UNKNOWN;
			break;
		}
		//非法数据
		default: target = OBJ_UNKNOWN;break;
		}
		//指针移动 游戏加载的数据
		++d;
		if (target != OBJ_UNKNOWN) {
			//写入数组
			state[y * w + x] = target;
			//下标移动 游戏运行的数据
			++x;
		}
	}
}

void draw(const Object* state, int w, int h) {
	//Object枚举类型的顺序
	const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			//枚举类型做下标
			Object o = state[y * w + x];
			cout << font[o];
		}
		cout << endl;
	}
}

void update(Object* state, char input, int w, int h) {
	//输入 转换为移动量
	int distanceX = 0;
	int distanceY = 0;
	switch (input) {
	case 'a': distanceX = -1; break;//左
	case 'd': distanceX = 1; break;//右
	case 'w': distanceY = 1; break;//上 y轴下方向为正
	case 's': distanceY = -1; break;//下
	default: break;
	}

	int playerIndex = 0;
	//查找玩家坐标  可以考虑存储玩家位置 并在游戏过程中维护
	for (playerIndex = 0; playerIndex < w * h; playerIndex++) {
		if (state[playerIndex] == OBJ_MAN || state[playerIndex] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	cout << playerIndex << endl;
	//转化为游戏数据数组指针
	int x = playerIndex % w;
	int y = playerIndex / w;
	cout << "x" << x << "y" << y << endl;

	//移动
	int targetX = x + distanceX;
	int targetY = y + distanceY;
	cout << "dX" << distanceX << "dY" << distanceY << endl;
	//检测移动目标是否在范围内
	if (targetX < 0 || targetY < 0 || targetX >= w || targetY >= h) {
		return;
	}
	int posIndex = y * w + x;
	int targetPos = targetY * w + targetX;
	cout << targetPos << endl;
	//空地或目标点 直接移动
	if (state[targetPos] == OBJ_SPACE || state[targetPos] == OBJ_GOAL) {
		state[targetPos] = state[targetPos] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[posIndex] = state[posIndex] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
		cout << "tp" << state[targetPos] << endl;
		cout << "pI" << state[posIndex] << endl;
		cout << (state[targetPos] == OBJ_GOAL) << endl;
		cout << (state[posIndex] == OBJ_MAN_ON_GOAL) << endl;
	//移动位置有箱子
	}else if(state[targetPos] == OBJ_BLOCK ||state[targetPos] == OBJ_BLOCK_ON_GOAL){
		//检测该方向第二个网格的位置是否在允许范围内
		int targetXNext = targetX + distanceX;
		int targetYNext = targetY + distanceY;
		//无法推动
		if (targetXNext < 0 || targetXNext >= w || targetYNext >= h) {
			return;
		}
		//目标位置的下个位置的坐标
		int targetPosNext = (targetY + distanceY) * w + (targetX + distanceX);
		if (state[targetPosNext] == OBJ_SPACE || state[targetPosNext] == OBJ_GOAL) {
			//逐个替换 从最远的位置开始换
			state[targetPosNext] = (state[targetPosNext] == OBJ_GOAL) ?
				OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[targetPos] = (state[targetPos] == OBJ_BLOCK_ON_GOAL) ?
				OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[posIndex] = (state[posIndex] == OBJ_MAN_ON_GOAL) ?
				OBJ_GOAL : OBJ_SPACE;
		}
	}



}

bool checkClear(const Object* state, int w, int h) {
	for (int i = 0; i < w * h; i++) {
		//箱子在目标点上 就不存在OBJ_BLOCK
		if (state[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}

void loadMapData() {
	//setlocale(LC_ALL, "");
	//读取文件
	ifstream inputFile("stage.txt", ifstream::binary);
	//移动到文件末尾
	inputFile.seekg(0, ifstream::end);
	//位置==文件大小
	int fileSize = static_cast<int>(inputFile.tellg());
	//移动回初始位置
	inputFile.seekg(0, ifstream::beg);
	//创建足够空间
	char* fileImage = new char[fileSize];
	//读取文件
	inputFile.read(fileImage, fileSize);
	cout << fileSize << endl;
	for (int i = 0; i < fileSize; i++) {
		cout << fileImage[i];
	}
	//cout << *fileImage;
	//cout.write(fileImage, fileSize);
}

