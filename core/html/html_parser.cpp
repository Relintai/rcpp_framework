#include "html_parser.h"
#include "core/error_macros.h"

bool HTMLParserAttribute::match_attrib(const String &attrib) {
	return attribute == attrib;
}
bool HTMLParserAttribute::match_data(const String &d) {
	return data == d;
}
bool HTMLParserAttribute::match_data(const Vector<String> &d) {
	// todo
	return false;
}
bool HTMLParserAttribute::contains_data(const String &d) {
	return data.find(d) != -1;
}

String HTMLParserAttribute::to_string() {
	if (single) {
		return attribute;
	}

	if (data.find('"' == -1)) {
		return attribute + "=\"" + data + "\"";
	} else {
		return attribute + "=\'" + data + "\'";
	}
}

void HTMLParserAttribute::print() {
	to_string().print();
}

HTMLParserAttribute::HTMLParserAttribute() {
	single = false;
}

HTMLParserAttribute::~HTMLParserAttribute() {
}

HTMLParserTag *HTMLParserTag::get_first(const String &t) {
	if (tag == t) {
		return this;
	}

	for (int i = 0; i < tags.size(); ++i) {
		HTMLParserTag *ht = tags[i]->get_first(t);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

HTMLParserTag *HTMLParserTag::get_first(const String &t, const String &attrib, const String &val) {
	if (tag == t) {
		if (has_attribute(attrib, val)) {
			return this;
		}
	}

	for (int i = 0; i < tags.size(); ++i) {
		HTMLParserTag *ht = tags[i]->get_first(t, attrib, val);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

String HTMLParserTag::get_attribute_value(const String &attrib) {
	HTMLParserAttribute *a = get_attribute(attrib);

	if (a) {
		return a->data;
	}

	return "";
}

HTMLParserAttribute *HTMLParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		HTMLParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return nullptr;
}

bool HTMLParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		HTMLParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

HTMLParserAttribute *HTMLParserTag::get_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		HTMLParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return nullptr;
}

bool HTMLParserTag::has_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		HTMLParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return true;
		}
	}

	return false;
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

		// test for comment. <!-- -->
		++start_index;
		if (data[2] == '-' && data[3] == '-') {
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_COMMENT;

			int comment_start_index = data.find(' ', 3);

			if (comment_start_index == -1) {
				comment_start_index = 4;
			}

			tag = data.substr(comment_start_index, data.size() - comment_start_index - 3);
		}

		if (data.size() < 11) {
			return;
		}

		// test for doctype. <!doctype >
		int doctype_start_index = data.find("doctype ", 2);

		if (doctype_start_index == -1) {
			return;
		}

		type = HTMLParserTag::HTML_PARSER_TAG_TYPE_DOCTYPE;

		tag = data.substr(doctype_start_index + 8, data.size() - doctype_start_index - 8 - 1);
	} else {
		String tag_text;

		if (data[data.size() - 2] == '/') {
			// will catch all that looks like <br/>
			// tags that look like <br> will be caught later in a post process, in a way
			// which also tries to catch erroneously not closed tags that supposed to be closed
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = data.substr(1, data.size() - 3);
		} else {
			type = HTMLParserTag::HTML_PARSER_TAG_TYPE_OPENING_TAG;

			tag_text = data.substr(1, data.size() - 2);
		}

		int fspc_index = tag_text.find(' ');

		if (fspc_index == -1) {
			// no args
			tag = tag_text;
			return;
		}

		// grab the tag itself
		tag = tag_text.substr(0, fspc_index);

		String args = tag_text.substr(fspc_index + 1, tag_text.size() - fspc_index - 1);
		parse_args(args);
	}

	int tag_end_index = data.find(' ', start_index);

	if (tag_end_index == -1) {
		// simple tag
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

		HTMLParserAttribute *a = new HTMLParserAttribute();

		if (equals_index == -1) {
			a->attribute = args.substr(i, args.size() - i);
			a->single = true;
			attributes.push_back(a);

			return;
		}

		a->attribute = args.substr(i, equals_index - i);

		// todo
		// a.trim();

		int next_char_index = equals_index + 1;

		if (next_char_index >= args.size()) {
			// an attribute looks like this "... attrib="
			attributes.push_back(a);
			return;
		}

		// skip spaces
		while (args[next_char_index] == ' ') {
			++next_char_index;

			if (next_char_index >= args.size()) {
				// an attribute looks like this "... attrib=     "
				attributes.push_back(a);
				return;
			}
		}

		char c = args[next_char_index];
		char find_char = ' ';

		if (c == '"' || c == '\'') {
			++next_char_index;
			find_char = c;
		}

		int end_index = args.find(find_char, next_char_index);

		if (end_index == -1) {
			// missing closing ' or " if c is ' or "
			// else missing parameter

			a->data = args.substr(next_char_index, args.size() - next_char_index - 1);
			attributes.push_back(a);
			return;
		}

		a->data = args.substr(next_char_index, end_index - next_char_index);
		attributes.push_back(a);

		i = end_index + 1;
	}
}

String HTMLParserTag::to_string(const int level) {
	String s;

	s.append_repeat(" ", level);

	if (type == HTML_PARSER_TAG_TYPE_CONTENT) {
		s += data + "\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == HTML_PARSER_TAG_TYPE_OPENING_TAG) {
		int ln = level + 1;

		s += "<" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->to_string();
		}

		s += ">\n";

		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->to_string(ln);
		}

		s.append_repeat(" ", level);

		s += "</" + tag + ">\n";
	} else if (type == HTML_PARSER_TAG_TYPE_CLOSING_TAG) {
		// HTMLParserTag should handle this automatically
		// it's here for debugging purposes though
		s += "</" + tag + "(!)>";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
		s += "<" + tag;

		for (int i = 0; i < attributes.size(); ++i) {
			s += " " + attributes[i]->to_string();
		}

		s += "/>\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!SELF CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == HTML_PARSER_TAG_TYPE_COMMENT) {
		s += "<!-- " + data + " -->\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!COMMENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == HTML_PARSER_TAG_TYPE_DOCTYPE) {
		s += data + "\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!DOCTYPE TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == HTML_PARSER_TAG_TYPE_NONE) {
		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->to_string(level) + "\n";
			s.append_repeat(" ", level);
		}
	}

	return s;
}
void HTMLParserTag::print() {
	to_string().print();
}

HTMLParserTag::HTMLParserTag() {
	type = HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE;
}

HTMLParserTag::~HTMLParserTag() {
	for (int i = 0; i < tags.size(); ++i) {
		delete tags[i];
	}

	for (int i = 0; i < attributes.size(); ++i) {
		delete attributes[i];
	}
}

void HTMLParser::parse(const String &data) {
	Vector<HTMLParserTag *> tags;

	// split into tags
	for (int i = 0; i < data.size(); ++i) {
		if (data[i] == '<') {
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == '>') {
					HTMLParserTag *t = new HTMLParserTag();

					t->data = data.substr(i, j - i + 1);
					t->process();

					tags.push_back(t);

					i = j;
					break;
				}
			}
		} else {
			for (int j = i + 1; j < data.size(); ++j) {
				if (data[j] == '<') {
					HTMLParserTag *t = new HTMLParserTag();

					t->data = data.substr(i, j - i);
					t->type = HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT;

					tags.push_back(t);

					i = j - 1;
					break;
				}
			}
		}
	}

	if (root) {
		delete root;
	}

	root = new HTMLParserTag();

	// process tags into hierarchical order
	Vector<HTMLParserTag *> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		HTMLParserTag *t = tags[i];

		if (t == nullptr) {
			RLOG_ERR("HTMLParser::parse: t == nullptr!");
			continue;
		}

		if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE) {
			RLOG_ERR("HTMLParser::parse: t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_NONE!");
			delete t;
			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_COMMENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_DOCTYPE) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == HTMLParserTag::HTML_PARSER_TAG_TYPE_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				delete t;
				tags[i] = nullptr;

				// ill-formed html
				continue;
			}

			// find it's pair
			int tag_index = 0;
			for (int j = tag_stack.size() - 1; j > 0; --j) {
				HTMLParserTag *ts = tag_stack[j];

				// we sould only have opening tags on the stack
				if (ts->tag == t->tag) {
					// found
					tag_index = j;
					break;
				}
			}

			HTMLParserTag *opening_tag = tag_stack[tag_index];

			// mark everything else that we found before finding the opening tag as self closing, and add them to out opening tag
			// If the html is ill formed, it just grabs everything from the tag stack
			for (int j = tag_index + 1; j < tag_stack.size(); ++j) {
				HTMLParserTag *ts = tag_stack[j];

				ts->type = HTMLParserTag::HTML_PARSER_TAG_TYPE_SELF_CLOSING_TAG;
				opening_tag->tags.push_back(ts);
			}

			tag_stack.resize(tag_index);

			if (tag_stack.size() == 0) {
				root->tags.push_back(opening_tag);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(opening_tag);
			}

			delete t;
			tags[i] = nullptr;

			continue;
		}
	}

	// add everything remaining on the stack to root
	for (int i = 0; i < tag_stack.size(); ++i) {
		root->tags.push_back(tag_stack[i]);
	}

	for (int i = 0; i < tags.size(); ++i) {
		HTMLParserTag *t = tags[i];

		if (t != nullptr) {
			RLOG_ERR("HTMLParser::parse(const String &data): tag was not processed!\n");
			t->print();

			delete t;
		}
	}
}

String HTMLParser::to_string() {
	if (!root) {
		return "";
	}

	return root->to_string();
}
void HTMLParser::print() {
	if (root) {
		root->print();
	}
}

HTMLParser::HTMLParser() {
	root = nullptr;
}

HTMLParser::~HTMLParser() {
	if (root) {
		delete root;
	}
}
