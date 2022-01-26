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

    
    duckx::Paragraph pTo = docTo.paragraphs();
    for (; pTo.has_next(); pTo.next()) {
        pTo.merge();
        vector<vector<u32string>> regexMatchResult = pTo.regexSearch(U"第一段");
        if (regexMatchResult.size() != 0) {
            duckx::Paragraph pFrom = doc.paragraphs();
            string regexResult = "";
            for (; pFrom.has_next(); pFrom.next()) {
                vector<vector<u32string>> regexMatchResultFrom =
                    pFrom.regexSearch(U"[^。]*[工作][^。]*[。]");
                vector<u32string> vectorU32string =
                    duckx::reshapeVvToV(regexMatchResultFrom);
                for (auto i : vectorU32string) {
                    regexResult.append(duckx::to_utf8(i));
                }
            }
            pTo.next().runs().set_text(regexResult);
            // cout << trs << endl; //.runs().set_text(regexResult);
        }
    }

    cout << "\n" << endl;

    doc.save();
    docTo.save();
    return 0;
}
