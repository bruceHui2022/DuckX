#include "copyFile.hpp"
#include <duckx.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    CopyFile("from2.docx ", "from.docx");
    CopyFile("to2.docx ", "to.docx");

    duckx::Document doc("from.docx");
    duckx::Document docTo("to.docx");
    doc.open();
    docTo.open();

    duckx::searchInFromdoocxAndPasteInTodocx(
        doc, docTo, u32string(U"第一段"), u32string(U"[^。，]*工作[^。，]*[。，]"));

    duckx::searchInFromdoocxAndPasteInTodocx(
        doc, docTo, u32string(U"第二段"),
        u32string(U"[^。，]*开发[^。，]*[。，]"));

    duckx::searchInFromdoocxAndPasteInTodocx(
        doc, docTo, u32string(U"第三段"),
        u32string(U"[^。，]*经济[^。，]*[。，]"));
    cout << "\n" << endl;

    doc.save();
    docTo.save();
    return 0;
}
