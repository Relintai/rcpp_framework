#include "html_parser.h"

HTMLParserAttribute::HTMLParserAttribute() {
}

HTMLParserAttribute::~HTMLParserAttribute() {
}

void HTMLParserTag::process() {
	if (type != HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE) {
		return;
	}

	if (data.size() < 2) {
		return;
	}

	ERR_FAIL_COND(data[0] != '<');
	ERR_FAIL_COND(data[data.size() - 1] != '>');

	int start_index = 1;
	if (data[1] == '/') {
		++start_index;

		type = HTMLParserTag::HTML_PARSER_TAG_TYPE_CLOSING_TAG;
	} else if (data[1] == '!') {
		if (data.size() < 8) {
			return;
		}

		//test for comment. <!-- -->
		++start_index;
		if (data[2] == '-' && data[3] == '-') {
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_COMMENT;

			int comment_start_index = data.find(' ', 3);

			if (comment_start_index == -1) {
				comment_start_index = 4;
			}

			tag = data.substr(comment_start_index, comment_start_index - data.size() - 3);
		}

		if (data.size() < 11) {
			return;
		}

		//test for doctype. <!doctype >
		int doctype_start_index = data.find("doctype ", 2);

		if (doctype_start_index == -1) {
			return;
		}

		type = HTMLParserTag::HTML_PARSER_TAG_TYPE_DOCTYPE;

		tag = data.substr(doctype_start_index + 8, data.size() - doctype_start_index - 8 - 1);
	} else {
		String tag_text;

		if (data[data.size() - 2] == '/') {
			//will catch all that looks like <br/>
			//tags that look like <br> will be caught later in a post process, in a way
			//which also tries to catch erroneously not closed tags that supposed to be closed
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = data.substr(1, data.size() - 3);
		} else {
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_OPENING_TAG;

			tag_text = data.substr(1, data.size() - 2);
		}

		int fspc_index = tag_text.find(' ');

		if (fspc_index == -1) {
			//no args
			tag = tag_text;
			return;
		}

		//grab the tag itself
		tag = tag_text.substr(0, fspc_index + 1);

		String args = tag_text.substr(fspc_index + 1, tag_text.size() - fspc_index - 1);
		parse_args(args);
	}

	int tag_end_index = data.find(' ', start_index);

	if (tag_end_index == -1) {
		//simple tag
		tag = data.substr(start_index, data.size() - start_index - 1);
		return;
	}
}

void HTMLParserTag::parse_args(const String &args) {
	attributes.clear();

	int i = 0;
	while (i < args.size()) {
		if (args[i] == ' ') {
			//"trim"
			++i;
			continue;
		}

		int equals_index = args.find('=', i);

		HTMLParserAttribute a;

		if (equals_index == -1) {
			a.attribute = args.substr(i, args.size() - 1);
			attributes.push_back(a);

			return;
		}

		a.attribute = args.substr(i, args.size() - equals_index - 1);
		a.attribute.print();

		//todo
		//a.trim();

		int next_char_index = equals_index;

		//skip spaces
		while (data[next_char_index] == ' ') {
			++next_char_index;

			if (next_char_index >= data.size()) {
				//an attribute looks like this "attrib=     "
				attributes.push_back(a);
				return;
			}
		}

		char c = data[next_char_index];
		char find_char;

		if (c == '"' || c == '\'') {
			++next_char_index;
			find_char = c;
		} else {
			find_char = ' ';
		}

		int end_index = args.find(find_char, next_char_index);

		if (end_index == -1) {
			//missing closing ' or " if c is ' or "
			//else missing parameter

			a.data = args.substr(next_char_index, args.size() - next_char_index - 1);
			attributes.push_back(a);
			//a.data.print();
			return;
		}

		a.data = args.substr(next_char_index, args.size() - end_index - 2);
		attributes.push_back(a);
		//a.data.print();

		i = end_index + 1;
	}
}

HTMLParserTag::HTMLParserTag() {
	type = HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE;
}

HTMLParserTag::~HTMLParserTag() {
}

void HTMLParser::parse(const String &data) {
	Vector<HTMLParserTag> tags;

	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '<') {
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == '>') {
					HTMLParserTag t;

					t.data = data.substr(i, j - i + 1);

					tags.push_back(t);

					i = j;
					break;
				}
			}
		} else {
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == '<') {
					HTMLParserTag t;

					t.data = data.substr(i, j - i);
					t.type = HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT;

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < tags.size(); ++i) {
		tags[i].process();
	}
}

HTMLParser::HTMLParser() {
}

HTMLParser::~HTMLParser() {
}
