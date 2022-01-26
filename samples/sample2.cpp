#include <duckx.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "copyFile.hpp"
using namespace std;


int main() {
    CopyFile("from2.docx ",
        "from.docx");
    duckx::Document doc("from.docx");
    doc.open();

    duckx::Paragraph p = doc.paragraphs();
    for (; p.has_next(); p.next()) {
        p.merge();
        vector<vector<u32string>> regexMatchResult = p.regexSearch(U"[^��]*����[^��]*[��]");
        cout << "\n" << endl;
    }


    std::u32string testStr = U"һ����";
    cout << "size:  " << testStr.size() << endl;
    doc.save();

    return 0;
}
