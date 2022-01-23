#include <duckx.hpp>
#include <iostream>
using namespace std;

int main() {
    duckx::Document doc("my_test.docx");
    doc.open();

    duckx::Paragraph p =
        doc.paragraphs().insert_paragraph_after("You can insert text in ");
    
    doc.save();

    return 0;
}
