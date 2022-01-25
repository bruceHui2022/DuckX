#include <duckx.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;


int main() {

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
