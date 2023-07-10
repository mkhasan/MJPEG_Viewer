#ifndef _FILE_READER_H
#define _FILE_READER_H

#include <string>
#include <vector>

class FileReader {
    int startIndex;
    int size;
    
    std::string path;
    std::vector<std::string> encodings;
    std::vector<std::string> filenames;
    
public:

    FileReader(const std::string& path, int size, int startIndex=0);

    std::string& GetEncoding(int index);

    std::string& GetFilename(int index);

    int GetSize() const;

    static int get_file_size(const std::string& filename);

    static void ReadEntireFile(const std::string& filename,
                             std::string& content);
};
#endif /* _FILE_READER_H */
