#include <fstream>
#include <iostream>
using namespace std;

int main() {
	//setlocale(LC_ALL, "");
	//读取文件
	ifstream inputFile("stage.txt", ifstream::binary);
	//移动到文件末尾
	inputFile.seekg(0, ifstream::end);
	//位置==文件大小
	int fileSize = static_cast<int>(inputFile.tellg()) ;
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
