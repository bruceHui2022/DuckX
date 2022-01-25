#include "duckx.hpp"
#include <Windows.h>
#include <cctype>
#include <iostream>
#include <codecvt>
#include <vector>


// Hack on pugixml
// We need to write xml to std string (or char *)
// So overload the write function
struct xml_string_writer : pugi::xml_writer {
    std::string result;

    virtual void write(const void *data, size_t size) {
        result.append(static_cast<const char *>(data), size);
    }
};

duckx::Run::Run() {}

duckx::Run::Run(pugi::xml_node parent, pugi::xml_node current) {
    this->set_parent(parent);
    this->set_current(current);
}

void duckx::Run::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:r");
}

void duckx::Run::set_current(pugi::xml_node node) { this->current = node; }

std::string duckx::Run::get_text() const {
    return this->current.child("w:t").text().get();
}

bool duckx::Run::set_text(const std::string &text) const {
    return this->current.child("w:t").text().set(text.c_str());
}

bool duckx::Run::set_text(const char *text) const {
    return this->current.child("w:t").text().set(text);
}

duckx::Run &duckx::Run::next() {
    this->current = this->current.next_sibling();
    return *this;
}

bool duckx::Run::has_next() const { return this->current != 0; }

// Table cells
duckx::TableCell::TableCell() {}

duckx::TableCell::TableCell(pugi::xml_node parent, pugi::xml_node current) {
    this->set_parent(parent);
    this->set_current(current);
}

void duckx::TableCell::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:tc");

    this->paragraph.set_parent(this->current);
}

void duckx::TableCell::set_current(pugi::xml_node node) {
    this->current = node;
}

bool duckx::TableCell::has_next() const { return this->current != 0; }

duckx::TableCell &duckx::TableCell::next() {
    this->current = this->current.next_sibling();
    return *this;
}

duckx::Paragraph &duckx::TableCell::paragraphs() {
    this->paragraph.set_parent(this->current);
    return this->paragraph;
}

// Table rows
duckx::TableRow::TableRow() {}

duckx::TableRow::TableRow(pugi::xml_node parent, pugi::xml_node current) {
    this->set_parent(parent);
    this->set_current(current);
}

void duckx::TableRow::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:tr");

    this->cell.set_parent(this->current);
}

void duckx::TableRow::set_current(pugi::xml_node node) { this->current = node; }

duckx::TableRow &duckx::TableRow::next() {
    this->current = this->current.next_sibling();
    return *this;
}

duckx::TableCell &duckx::TableRow::cells() {
    this->cell.set_parent(this->current);
    return this->cell;
}

bool duckx::TableRow::has_next() const { return this->current != 0; }

// Tables
duckx::Table::Table() {}

duckx::Table::Table(pugi::xml_node parent, pugi::xml_node current) {
    this->set_parent(parent);
    this->set_current(current);
}

void duckx::Table::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:tbl");

    this->row.set_parent(this->current);
}

bool duckx::Table::has_next() const { return this->current != 0; }

duckx::Table &duckx::Table::next() {
    this->current = this->current.next_sibling();
    this->row.set_parent(this->current);
    return *this;
}

void duckx::Table::set_current(pugi::xml_node node) { this->current = node; }

duckx::TableRow &duckx::Table::rows() {
    this->row.set_parent(this->current);
    return this->row;
}

duckx::Paragraph::Paragraph() {}

duckx::Paragraph::Paragraph(pugi::xml_node parent, pugi::xml_node current) {
    this->set_parent(parent);
    this->set_current(current);
}

char *duckx::Utf8ToGBK(const char *strUtf8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
    unsigned short *wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL,
                              NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL,
                        NULL);
    return szGBK;
}

std::string duckx::Paragraph::getFont(pugi::xml_node Node) {
    pugi::xml_node fontNode = Node.child("w:rPr").child("w:rFonts");
    std::string fontStr = fontNode.attribute("w:eastAsia").value();
    return std::string(Utf8ToGBK(fontStr.c_str()));
}

std::string duckx::Paragraph::getText(pugi::xml_node Node) {
    pugi::xml_node currentNode = Node.child("w:t");
    return std::string(currentNode.text().get());
}

void duckx::Paragraph::merge() {

    pugi::xml_node currentNodewr = this->current.child("w:r");
    std::string textStr = getText(currentNodewr);
    std::string fontStr = getFont(currentNodewr);
    pugi::xml_node nextNodewr = currentNodewr.next_sibling();
    pugi::xml_node nodetemp;
    while (nextNodewr && strcmp(nextNodewr.name(), "w:r")) {
        nodetemp = nextNodewr.next_sibling();
        this->current.remove_child(nextNodewr);
        nextNodewr = nodetemp;
    }

    while (nextNodewr) {
        textStr.append(getText(nextNodewr));
        this->current.remove_child(currentNodewr);
        currentNodewr = nextNodewr;
        nextNodewr = currentNodewr.next_sibling();
        while (nextNodewr && strcmp(nextNodewr.name(), "w:r")) {
            nodetemp = nextNodewr.next_sibling();
            this->current.remove_child(nextNodewr);
            nextNodewr = nodetemp;
        }
    }
    pugi::xml_node currentNodewt = currentNodewr.child("w:t");
    bool setValueBool = currentNodewt.text().set(textStr.c_str());
    std::cout << Utf8ToGBK(textStr.c_str()) << "\n" << std::endl;
}
std::string duckx::to_utf8(std::u32string str32) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}
        .to_bytes(str32);
}

std::u32string duckx::to_utf32(std::string str) {
    return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}
        .from_bytes(str);
}

void duckx::printU32String(std::u32string u32Str) {
    std::cout << Utf8ToGBK(to_utf8(u32Str).c_str()) << std::endl;
}

std::vector<std::vector<std::u32string>> duckx::Paragraph::regexSearch(std::u32string regexU32String) {
    jpu::Regex rew;
    rew.setPattern(regexU32String).compile();

    jpu::VecNum vec_num32;
    jpcre2::VecOff vec_eoff;

    std::string targetString = getText(this->current.child("w:r"));

    jpu::RegexMatch rmw;
    size_t count = rmw.setRegexObject(&rew)
                       .setSubject(to_utf32(targetString))
                       .setModifier("g")
                       .setNumberedSubstringVector(&vec_num32)
                       .setMatchEndOffsetVector(&vec_eoff)
                       .match();
    return vec_num32;
}
void duckx::Paragraph::set_parent(pugi::xml_node node) {
    this->parent = node;
    this->current = this->parent.child("w:p");

    this->run.set_parent(this->current);
}

void duckx::Paragraph::set_current(pugi::xml_node node) {
    this->current = node;
}

duckx::Paragraph &duckx::Paragraph::next() {
    this->current = this->current.next_sibling();
    this->run.set_parent(this->current);
    return *this;
}

bool duckx::Paragraph::has_next() const { return this->current != 0; }

duckx::Run &duckx::Paragraph::runs() {
    this->run.set_parent(this->current);
    return this->run;
}

duckx::Run &duckx::Paragraph::add_run(const std::string &text,
                                      duckx::formatting_flag f) {
    return this->add_run(text.c_str(), f);
}

duckx::Run &duckx::Paragraph::add_run(const char *text,
                                      duckx::formatting_flag f) {
    // Add new run
    pugi::xml_node new_run = this->current.append_child("w:r");
    // Insert meta to new run
    pugi::xml_node meta = new_run.append_child("w:rPr");

    if (f & duckx::bold)
        meta.append_child("w:b");

    if (f & duckx::italic)
        meta.append_child("w:i");

    if (f & duckx::underline)
        meta.append_child("w:u").append_attribute("w:val").set_value("single");

    if (f & duckx::strikethrough)
        meta.append_child("w:strike")
            .append_attribute("w:val")
            .set_value("true");

    if (f & duckx::superscript)
        meta.append_child("w:vertAlign")
            .append_attribute("w:val")
            .set_value("superscript");
    else if (f & duckx::subscript)
        meta.append_child("w:vertAlign")
            .append_attribute("w:val")
            .set_value("subscript");

    if (f & duckx::smallcaps)
        meta.append_child("w:smallCaps")
            .append_attribute("w:val")
            .set_value("true");

    if (f & duckx::shadow)
        meta.append_child("w:shadow")
            .append_attribute("w:val")
            .set_value("true");

    pugi::xml_node new_run_text = new_run.append_child("w:t");
    // If the run starts or ends with whitespace characters, preserve them using
    // the xml:space attribute
    if (*text != 0 && (isspace(text[0]) || isspace(text[strlen(text) - 1])))
        new_run_text.append_attribute("xml:space").set_value("preserve");
    new_run_text.text().set(text);

    return *new Run(this->current, new_run);
}

duckx::Paragraph &
duckx::Paragraph::insert_paragraph_after(const std::string &text,
                                         duckx::formatting_flag f) {
    pugi::xml_node new_para =
        this->parent.insert_child_after("w:p", this->current);

    Paragraph *p = new Paragraph();
    p->set_current(new_para);
    p->add_run(text, f);

    return *p;
}

duckx::Document::Document() {
    // TODO: this function must be removed!
    this->directory = "";
}

duckx::Document::Document(std::string directory) {
    this->directory = directory;
}

void duckx::Document::file(std::string directory) {
    this->directory = directory;
}

void duckx::Document::open() {
    void *buf = NULL;
    size_t bufsize;

    // Open file and load "xml" content to the document variable
    zip_t *zip =
        zip_open(this->directory.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

    zip_entry_open(zip, "word/document.xml");
    zip_entry_read(zip, &buf, &bufsize);

    zip_entry_close(zip);
    zip_close(zip);

    this->document.load_buffer(buf, bufsize);

    free(buf);

    this->paragraph.set_parent(document.child("w:document").child("w:body"));
}

void duckx::Document::save() const {
    // minizip only supports appending or writing to new files
    // so we must
    // - make a new file
    // - write any new files
    // - copy the old files
    // - delete old docx
    // - rename new file to old file

    // Read document buffer
    xml_string_writer writer;
    this->document.print(writer);

    // Open file and replace "xml" content

    std::string original_file = this->directory;
    std::string temp_file = this->directory + ".tmp";

    // Create the new file
    zip_t *new_zip =
        zip_open(temp_file.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

    // Write out document.xml
    zip_entry_open(new_zip, "word/document.xml");

    const char *buf = writer.result.c_str();

    zip_entry_write(new_zip, buf, strlen(buf));
    zip_entry_close(new_zip);

    // Open the original zip and copy all files which are not replaced by duckX
    zip_t *orig_zip =
        zip_open(original_file.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');

    // Loop & copy each relevant entry in the original zip
    int orig_zip_entry_ct = zip_total_entries(orig_zip);
    for (int i = 0; i < orig_zip_entry_ct; i++) {
        zip_entry_openbyindex(orig_zip, i);
        const char *name = zip_entry_name(orig_zip);

        // Skip copying the original file
        if (std::string(name) != std::string("word/document.xml")) {
            // Read the old content
            void *entry_buf;
            size_t entry_buf_size;
            zip_entry_read(orig_zip, &entry_buf, &entry_buf_size);

            // Write into new zip
            zip_entry_open(new_zip, name);
            zip_entry_write(new_zip, entry_buf, entry_buf_size);
            zip_entry_close(new_zip);

            free(entry_buf);
        }

        zip_entry_close(orig_zip);
    }

    // Close both zips
    zip_close(orig_zip);
    zip_close(new_zip);

    // Remove original zip, rename new to correct name
    remove(original_file.c_str());
    rename(temp_file.c_str(), original_file.c_str());
}

duckx::Paragraph &duckx::Document::paragraphs() {
    this->paragraph.set_parent(document.child("w:document").child("w:body"));
    return this->paragraph;
}

duckx::Table &duckx::Document::tables() {
    this->table.set_parent(document.child("w:document").child("w:body"));
    return this->table;
}
