#include <fstream>
#include <iostream>
using namespace std;

int main() {
	//setlocale(LC_ALL, "");
	//��ȡ�ļ�
	ifstream inputFile("stage.txt", ifstream::binary);
	//�ƶ����ļ�ĩβ
	inputFile.seekg(0, ifstream::end);
	//λ��==�ļ���С
	int fileSize = static_cast<int>(inputFile.tellg()) ;
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
