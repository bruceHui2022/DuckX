#include <duckx.hpp>
#include <iostream>
using namespace std;

int main() {
    
    duckx::Document doc("from.docx");
    doc.open();

    duckx::Paragraph p =  doc.paragraphs();
    for (; p.has_next(); p.next()) {
        p.merge();
     }

    doc.save();

    return 0;
}
