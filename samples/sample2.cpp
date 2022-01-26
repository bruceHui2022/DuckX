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
        doc, docTo, u32string(U"��һ��"), u32string(U"[^����]*����[^����]*[����]"));

    duckx::searchInFromdoocxAndPasteInTodocx(
        doc, docTo, u32string(U"�ڶ���"),
        u32string(U"[^����]*����[^����]*[����]"));

    duckx::searchInFromdoocxAndPasteInTodocx(
        doc, docTo, u32string(U"������"),
        u32string(U"[^����]*����[^����]*[����]"));
    cout << "\n" << endl;

    doc.save();
    docTo.save();
    return 0;
}
