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
    fs.new_file("oi.txt", "content oi");
    fs.new_file("oiee.txt", "content oieee");
    fs.mkdir("testePasta");
    fs.ls();
    cout << "-------------------\n";
    fs.cd("testePasta");
    fs.new_file("oiee.txt", "content oieee 2");
    fs.ls();
    fs.cat("oiee.txt");
    cout << "-------------------\n";
    fs.cd("..");
    fs.ls();
    fs.cd("..");
    return 0;
}
