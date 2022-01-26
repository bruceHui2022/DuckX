/*
 * Under MIT license
 * Author: Amir Mohamadi (@amiremohamadi)
 * DuckX is a free library to work with docx files.
 */

#ifndef DUCKX_H
#define DUCKX_H

#include <cstdio>
#include <stdlib.h>
#include <string>

#include <constants.hpp>
#include <duckxiterator.hpp>
#include <pugixml.hpp>
#include <zip.h>

#define PCRE2_STATIC
#include <jpcre2.hpp>

typedef jpcre2::select<char32_t> jpu;

// TODO: Use container-iterator design pattern!
char *Utf8ToGBK(const char *strUtf8);

    namespace duckx {
// Run contains runs in a paragraph
class Run {
  private:
    friend class IteratorHelper;
    // Store the parent node (a paragraph)
    pugi::xml_node parent;
    // And store current node also
    pugi::xml_node current;

  public:
    Run();
    Run(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    std::string get_text() const;
    bool set_text(const std::string &) const;
    bool set_text(const char *) const;

    Run &next();
    bool has_next() const;
};

// Paragraph contains a paragraph
// and stores runs
class Paragraph {
  private:
    friend class IteratorHelper;
    // Store parent node (usually the body node)
    pugi::xml_node parent;
    // And store current node also
    pugi::xml_node current;
    // A paragraph consists of runs
    Run run;

  public:
    Paragraph();
    Paragraph(pugi::xml_node, pugi::xml_node);
    std::string getFont(pugi::xml_node fontNode);
    std::string getText(pugi::xml_node Node);
    void merge();
    std::vector<std::vector<std::u32string>>
    duckx::Paragraph::regexSearch(std::u32string regexU32String);

    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Paragraph &next();
    bool has_next() const;

    Run &runs();
    Run &add_run(const std::string &, duckx::formatting_flag = duckx::none);
    Run &add_run(const char *, duckx::formatting_flag = duckx::none);
    Paragraph &insert_paragraph_after(const std::string &,
                                      duckx::formatting_flag = duckx::none);
};

// TableCell contains one or more paragraphs
class TableCell {
  private:
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    Paragraph paragraph;

  public:
    TableCell();
    TableCell(pugi::xml_node, pugi::xml_node);

    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Paragraph &paragraphs();

    TableCell &next();
    bool has_next() const;
};

// TableRow consists of one or more TableCells
class TableRow {
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    TableCell cell;

  public:
    TableRow();
    TableRow(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    TableCell &cells();

    bool has_next() const;
    TableRow &next();
};

// Table consists of one or more TableRow objects
class Table {
  private:
    friend class IteratorHelper;
    pugi::xml_node parent;
    pugi::xml_node current;

    TableRow row;

  public:
    Table();
    Table(pugi::xml_node, pugi::xml_node);
    void set_parent(pugi::xml_node);
    void set_current(pugi::xml_node);

    Table &next();
    bool has_next() const;

    TableRow &rows();
};

// Document contains whole the docx file
// and stores paragraphs
class Document {
  private:
    friend class IteratorHelper;
    std::string directory;
    Paragraph paragraph;
    Table table;
    pugi::xml_document document;

  public:
    Document();
    Document(std::string);
    void file(std::string);
    void open();
    void save() const;

    Paragraph &paragraphs();
    Table &tables();
};
char *Utf8ToGBK(const char *strUtf8);
std::string to_utf8(std::u32string str32);
std::u32string to_utf32(std::string str);
void printU32String(std::u32string u32Str);
std::vector<std::u32string>
reshapeVvToV(std::vector<std::vector<std::u32string>> Vv);
void searchInFromdoocxAndPasteInTodocx(duckx::Document &docFrom,
                                       duckx::Document &docTo,
                                       std::u32string regexSearchOfTodoc,
                                       std::u32string regexSearchOfFromdoc);

} // namespace duckx

#endif
