#include "file_reader.h"

#include <fstream>

using namespace std;

FileReader::FileReader(const string& _path, int _size, int _startIndex)  
    : path(_path)
    , size(_size)
    , startIndex(_startIndex)
{
    char filename[1024];
    string encoding;
    for (int i=0; i<size; ++i) {
		sprintf(filename, "%s/%06d.jpg", path.c_str(), startIndex+i);
        encoding.resize(0);
        ReadEntireFile(filename, encoding);
        encodings.push_back(encoding);
	filenames.push_back(filename);
    }
}

int FileReader::GetSize() const {
	return size;
}

string& FileReader::GetEncoding(int index) {
	return encodings[index];
}

string& FileReader::GetFilename(int index) {
	return filenames[index];
}

int FileReader::get_file_size(const std::string& filename) {
	ifstream in_file(filename, ios::binary);
	in_file.seekg(0, ios::end);
	int file_size = in_file.tellg();
	in_file.close();
	return file_size;
}

void FileReader::ReadEntireFile(const std::string& filename,
                             std::string& content) {

	int size = get_file_size(filename);
	//LOG(WARNING) << "filename is " << filename;

	//string content;
	content.resize(size);

	ifstream file( filename, std::ios::binary );

	file.seekg (0, ios::beg);

	file.read(&content[0], size);

	//LOG(WARNING) << "read size is " << read_size;

	file.close();

}
