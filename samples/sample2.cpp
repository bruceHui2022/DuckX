#include <duckx.hpp>
#include <iostream>
#define PCRE2_STATIC
#include <jpcre2.hpp>
#include <codecvt> 
#include <string>
#include <clocale>
#include <cuchar>
#include <climits>
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

     std::mbstate_t state{};
    char out[MB_LEN_MAX]{};
    std::u32string testStr = U"Ò»¶þÈý";
    cout << "size:  " << testStr.size() << endl;
    for (char32_t c : testStr) {
        std::size_t rc = std::c32rtomb(out, c, &state);
        std::cout << c << " converted to [ ";
        if (rc != (std::size_t)-1)
            for (unsigned char c8 : std::string{out, rc})
                std::cout << +c8 << ' ';
        std::cout << "]\n";
    }
    doc.save();

    return 0;
}
