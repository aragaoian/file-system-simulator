#include "file_management.cpp"
#include <iostream>
using namespace std;

// • change_directory [OK]
// • list_directory_contents [OK]
// • btree_search [OK]
// • btree_insert [OK]
// • btree_traverse [OK]
// • btree_delete []
// • delete_txt_file []
// • delete_directory []
// • save_img []
// • load_img []

int main() {
    FileSystem fs = FileSystem("test.img");
    fs.touch("file1.txt", "content oi");
    fs.touch("file2.txt", "content oieee");
    fs.mkdir("folder1");

    fs.cd("folder1");
    fs.touch("file3.txt", "content oieee 2");
    fs.mkdir("folder2");
    fs.cd("folder2");
    fs.touch("file4.txt", "content oieee 3");
    fs.cd("..");
    fs.cd("..");
    fs.mkdir("folder3");
    fs.cd("folder3");
    fs.touch("file5.txt", "content oi 2");
    fs.cd("..");
    fs.cd("..");

    // file1.txt
    // file2.txt
    // folder1
    //  file3.txt
    //  folder2
    //   file4.txt
    // folder3
    //  file5.txt

    return 0;
}
