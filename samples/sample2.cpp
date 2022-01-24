#include <duckx.hpp>
#include <iostream>
#define PCRE2_STATIC
#include <jpcre2.hpp>
typedef jpcre2::select<char32_t> jpu;
using namespace std;

int main() {

    duckx::Document doc("from.docx");
    doc.open();

    duckx::Paragraph p = doc.paragraphs();
    for (; p.has_next(); p.next()) {
        p.merge();
    }

    jpu::Regex rew;
    rew.setPattern(U"\\d+").compile();

    jpu::VecNum vec_num32;
    jpcre2::VecOff vec_eoff;

    jpu::RegexMatch rmw;
    size_t count = rmw.setRegexObject(&rew)
                       .setSubject(U"12345asd6789")
                       .setModifier("g")
                       .setNumberedSubstringVector(&vec_num32)
                       .setMatchEndOffsetVector(&vec_eoff)
                       .match();

    char32_t *testStr = U"123";
    cout << "size:  " << testStr.size() << endl;
    doc.save();

    return 0;
}
