#include <fstream>
#include <iostream>
using namespace std;

//#��ʾǽ�� _����ռ� .����Ŀ�ĵ� o�������� p�������
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
	OBJ_SPACE,//�յ�
	OBJ_WALL, //ǽ
	OBJ_GOAL, //Ŀ���
	OBJ_BLOCK,//����
	OBJ_BLOCK_ON_GOAL, //���ӵ���Ŀ���
	OBJ_MAN, //���
	OBJ_MAN_ON_GOAL, //��ҵ���Ŀ���

	OBJ_UNKNOWN,
};

//����ԭ��
//���������ַ���ת��ΪObject����
void initialize(Object* state, int w, int h, const char* stageData);
//�������
void draw(const Object* state, int w, int h);
//���º���
void update(Object* state, char input, int w, int h);
//ͨ�ؼ��
bool checkClear(const Object* state, int w, int h);

/*int main() {
	//����״̬����
	Object* state = new Object[gStageWidth * gStageHeight];
	//��ʼ������
	initialize(state, gStageWidth, gStageHeight, gStageData);
	//��ѭ��
	while (true) {
		//����
		draw(state, gStageWidth, gStageHeight);
		//ͨ�ؼ��
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}
		//��ȡ����
		cout << "a:left d:right w:up s:down. command?" << endl;
		char input;
		cin >> input;
		update(state, input, gStageWidth, gStageHeight);
	}

	//ʤ��ͨ����ʾ
	cout << "Congratulation's! You win!" << endl;
	//�Ƴ����������� delete[]�����ǵ�delete
	delete[] state;
	//�Ƴ���ָ�븳ֵΪ0 ����ָ��bug
	state = 0; 
	
	
	//return 0;
}*/

void initialize(Object* state, int w, int h, const char* stageData) {
	//��ȡָ��
	const char* d = stageData;
	//x����һά������  y����һά������ �±�ֵΪy*width+x
	int x = 0;
	int y = 0;
	//�����ַ�������
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
			//û������
			target = OBJ_UNKNOWN;
			break;
		}
		//�Ƿ�����
		default: target = OBJ_UNKNOWN;break;
		}
		//ָ���ƶ� ��Ϸ���ص�����
		++d;
		if (target != OBJ_UNKNOWN) {
			//д������
			state[y * w + x] = target;
			//�±��ƶ� ��Ϸ���е�����
			++x;
		}
	}
}

void draw(const Object* state, int w, int h) {
	//Objectö�����͵�˳��
	const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			//ö���������±�
			Object o = state[y * w + x];
			cout << font[o];
		}
		cout << endl;
	}
}

void update(Object* state, char input, int w, int h) {
	//���� ת��Ϊ�ƶ���
	int distanceX = 0;
	int distanceY = 0;
	switch (input) {
	case 'a': distanceX = -1; break;//��
	case 'd': distanceX = 1; break;//��
	case 'w': distanceY = 1; break;//�� y���·���Ϊ��
	case 's': distanceY = -1; break;//��
	default: break;
	}

	int playerIndex = 0;
	//�����������  ���Կ��Ǵ洢���λ�� ������Ϸ������ά��
	for (playerIndex = 0; playerIndex < w * h; playerIndex++) {
		if (state[playerIndex] == OBJ_MAN || state[playerIndex] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	cout << playerIndex << endl;
	//ת��Ϊ��Ϸ��������ָ��
	int x = playerIndex % w;
	int y = playerIndex / w;
	cout << "x" << x << "y" << y << endl;

	//�ƶ�
	int targetX = x + distanceX;
	int targetY = y + distanceY;
	cout << "dX" << distanceX << "dY" << distanceY << endl;
	//����ƶ�Ŀ���Ƿ��ڷ�Χ��
	if (targetX < 0 || targetY < 0 || targetX >= w || targetY >= h) {
		return;
	}
	int posIndex = y * w + x;
	int targetPos = targetY * w + targetX;
	cout << targetPos << endl;
	//�յػ�Ŀ��� ֱ���ƶ�
	if (state[targetPos] == OBJ_SPACE || state[targetPos] == OBJ_GOAL) {
		state[targetPos] = state[targetPos] == OBJ_GOAL ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		state[posIndex] = state[posIndex] == OBJ_MAN_ON_GOAL ? OBJ_GOAL : OBJ_SPACE;
		cout << "tp" << state[targetPos] << endl;
		cout << "pI" << state[posIndex] << endl;
		cout << (state[targetPos] == OBJ_GOAL) << endl;
		cout << (state[posIndex] == OBJ_MAN_ON_GOAL) << endl;
	//�ƶ�λ��������
	}else if(state[targetPos] == OBJ_BLOCK ||state[targetPos] == OBJ_BLOCK_ON_GOAL){
		//���÷���ڶ��������λ���Ƿ�������Χ��
		int targetXNext = targetX + distanceX;
		int targetYNext = targetY + distanceY;
		//�޷��ƶ�
		if (targetXNext < 0 || targetXNext >= w || targetYNext >= h) {
			return;
		}
		//Ŀ��λ�õ��¸�λ�õ�����
		int targetPosNext = (targetY + distanceY) * w + (targetX + distanceX);
		if (state[targetPosNext] == OBJ_SPACE || state[targetPosNext] == OBJ_GOAL) {
			//����滻 ����Զ��λ�ÿ�ʼ��
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
		//������Ŀ����� �Ͳ�����OBJ_BLOCK
		if (state[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;
}

void loadMapData() {
	//setlocale(LC_ALL, "");
	//��ȡ�ļ�
	ifstream inputFile("stage.txt", ifstream::binary);
	//�ƶ����ļ�ĩβ
	inputFile.seekg(0, ifstream::end);
	//λ��==�ļ���С
	int fileSize = static_cast<int>(inputFile.tellg());
	//�ƶ��س�ʼλ��
	inputFile.seekg(0, ifstream::beg);
	//�����㹻�ռ�
	char* fileImage = new char[fileSize];
	//��ȡ�ļ�
	inputFile.read(fileImage, fileSize);
	cout << fileSize << endl;
	for (int i = 0; i < fileSize; i++) {
		cout << fileImage[i];
	}
	//cout << *fileImage;
	//cout.write(fileImage, fileSize);
}

