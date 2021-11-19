#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include "core/containers/vector.h"
#include "core/string.h"

class HTMLParserAttribute {
public:
	String attribute;
	String data;
	bool single;

	bool match_attrib(const String &attrib);
	bool match_data(const String &d);
	bool match_data(const Vector<String> &d);
	bool contains_data(const String &d);

	String to_string();
	void print();

	HTMLParserAttribute();
	virtual ~HTMLParserAttribute();
};

class HTMLParserTag {
public:
	enum HTMLParserTagType {
		HTML_PARSER_TAG_TYPE_NONE = 0,
		HTML_PARSER_TAG_TYPE_OPENING_TAG,
		HTML_PARSER_TAG_TYPE_CLOSING_TAG,
		HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG,
		HTML_PARSER_TAG_TYPE_COMMENT,
		HTML_PARSER_TAG_TYPE_DOCTYPE,
		HTML_PARSER_TAG_TYPE_CONTENT
	};

	int type;

	String tag;
	String data;

	Vector<HTMLParserTag*> tags;
	Vector<HTMLParserAttribute*> attributes;

	void process();
	void parse_args(const String &args);

	String to_string(const int level = 0);
	void print();

	HTMLParserTag();
	virtual ~HTMLParserTag();
};

class HTMLParser {
public:
	HTMLParserTag *root;

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	String to_string();
	void print();

	HTMLParser();
	virtual ~HTMLParser();
};

#endif