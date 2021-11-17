#ifndef HTML_BUILDER_H
#define HTML_BUILDER_H

#include "core/string.h"
#include "core/containers/vector.h"

class HTMLParserAttribute {
public:
	String attribute;
	String data;

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

	Vector<HTMLParserTag> tags;
	Vector<HTMLParserAttribute> attributes;

	void process();
	void parse_args(const String& args);

	HTMLParserTag();
	virtual ~HTMLParserTag();
};

class HTMLParser {
public:
	HTMLParserTag html;

	void parse(const String &data);
	//void parse_tag(const String &data, const int index);

	HTMLParser();
	virtual ~HTMLParser();
};

#endif