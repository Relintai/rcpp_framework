#include "bbcode_parser.h"
#include "core/error_macros.h"

bool BBCodeParserAttribute::match_attrib(const String &attrib) {
	return attribute == attrib;
}
bool BBCodeParserAttribute::match_data(const String &d) {
	return data == d;
}
bool BBCodeParserAttribute::match_data(const Vector<String> &d) {
	// todo
	return false;
}
bool BBCodeParserAttribute::contains_data(const String &d) {
	return data.find(d) != -1;
}

String BBCodeParserAttribute::to_string() {
	if (single) {
		return attribute;
	}

	if (data.find('"' == -1)) {
		return attribute + "=\"" + data + "\"";
	} else {
		return attribute + "=\'" + data + "\'";
	}
}

void BBCodeParserAttribute::print() {
	to_string().print();
}

BBCodeParserAttribute::BBCodeParserAttribute() {
	single = false;
}

BBCodeParserAttribute::~BBCodeParserAttribute() {
}

BBCodeParserTag *BBCodeParserTag::get_first(const String &t) {
	if (tag == t) {
		return this;
	}

	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *ht = tags[i]->get_first(t);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

BBCodeParserTag *BBCodeParserTag::get_first(const String &t, const String &attrib, const String &val) {
	if (tag == t) {
		if (has_attribute(attrib, val)) {
			return this;
		}
	}

	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *ht = tags[i]->get_first(t, attrib, val);

		if (ht) {
			return ht;
		}
	}

	return nullptr;
}

String BBCodeParserTag::get_attribute_value(const String &attrib) {
	BBCodeParserAttribute *a = get_attribute(attrib);

	if (a) {
		return a->data;
	}

	return "";
}

BBCodeParserAttribute *BBCodeParserTag::get_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return a;
		}
	}

	return nullptr;
}

bool BBCodeParserTag::has_attribute(const String &attrib) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib)) {
			return true;
		}
	}

	return false;
}

BBCodeParserAttribute *BBCodeParserTag::get_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return a;
		}
	}

	return nullptr;
}

bool BBCodeParserTag::has_attribute(const String &attrib, const String &contains_val) {
	for (int i = 0; i < attributes.size(); ++i) {
		BBCodeParserAttribute *a = attributes[i];

		if (a->match_attrib(attrib) && a->contains_data(contains_val)) {
			return true;
		}
	}

	return false;
}

void BBCodeParserTag::process() {
	if (type != BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
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

		type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG;
	} else if (data[1] == '!') {
		if (data.size() < 8) {
			return;
		}

		// test for comment. <!-- -->
		++start_index;
		if (data[2] == '-' && data[3] == '-') {
			type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_COMMENT;

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

		type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_DOCTYPE;

		tag = data.substr(doctype_start_index + 8, data.size() - doctype_start_index - 8 - 1);
	} else {
		String tag_text;

		if (data[data.size() - 2] == '/') {
			// will catch all that looks like <br/>
			// tags that look like <br> will be caught later in a post process, in a way
			// which also tries to catch erroneously not closed tags that supposed to be closed
			type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;

			tag_text = data.substr(1, data.size() - 3);
		} else {
			type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG;

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

		if (fspc_index + 1 == tag_text.size()) {
			// no args, but had a space like <br />
			return;
		}

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

void BBCodeParserTag::parse_args(const String &args) {
	attributes.clear();

	int i = 0;
	while (i < args.size()) {
		if (args[i] == ' ') {
			//"trim"
			++i;
			continue;
		}

		int equals_index = args.find('=', i);

		BBCodeParserAttribute *a = new BBCodeParserAttribute();

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

String BBCodeParserTag::to_string(const int level) {
	String s;

	s.append_repeat(" ", level);

	if (type == BBCODE_PARSER_TAG_TYPE_CONTENT) {
		s += data + "\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!CONTENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
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
	} else if (type == BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
		// BBCodeParserTag should handle this automatically
		// it's here for debugging purposes though
		s += "</" + tag + "(!)>";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!CLOSING TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
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
	} else if (type == BBCODE_PARSER_TAG_TYPE_COMMENT) {
		s += "<!-- " + data + " -->\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!COMMENT TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_DOCTYPE) {
		s += data + "\n";

		if (tags.size() != 0) {
			s.append_repeat(" ", level);
			s += "(!DOCTYPE TAG HAS TAGS!)\n";

			for (int i = 0; i < tags.size(); ++i) {
				s += tags[i]->to_string(level + 1) + "\n";
			}
		}
	} else if (type == BBCODE_PARSER_TAG_TYPE_NONE) {
		for (int i = 0; i < tags.size(); ++i) {
			s += tags[i]->to_string(level) + "\n";
			s.append_repeat(" ", level);
		}
	}

	return s;
}
void BBCodeParserTag::print() {
	to_string().print();
}

BBCodeParserTag::BBCodeParserTag() {
	type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE;
}

BBCodeParserTag::~BBCodeParserTag() {
	for (int i = 0; i < tags.size(); ++i) {
		delete tags[i];
	}

	for (int i = 0; i < attributes.size(); ++i) {
		delete attributes[i];
	}
}

void BBCodeParser::parse(const String &data) {
	Vector<BBCodeParserTag *> tags;

	// <script> content parsing is based on https://stackoverflow.com/questions/14574471/how-do-browsers-parse-a-script-tag-exactly
	const int STATE_NONE = 0;
	const int STATE_DATA_1 = 1;
	const int STATE_DATA_2 = 2;
	const int STATE_DATA_3 = 3;

	int state = STATE_NONE;

	// split into tags
	for (int i = 0; i < data.size(); ++i) {
		if (state == STATE_NONE) {
			if (data[i] == '<') {
				// tag

				if (data.is_word_at(i, "<script")) {
					// after the opening <script> tag, the parser goes to data1 state
					state = STATE_DATA_1;
					// no else, we need to process the tag istelf!
				}

				for (int j = i + 1; j < data.size(); ++j) {
					if (data[j] == '>') {
						BBCodeParserTag *t = new BBCodeParserTag();

						t->data = data.substr(i, j - i + 1);
						t->process();

						tags.push_back(t);

						i = j;
						break;
					}
				}
			} else {
				// content

				for (int j = i + 1; j < data.size(); ++j) {
					if (data[j] == '<') {
						BBCodeParserTag *t = new BBCodeParserTag();

						t->data = data.substr(i, j - i);
						t->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT;

						tags.push_back(t);

						i = j - 1;
						break;
					}
				}
			}
		} else {
			// script tag content

			bool done = false;
			for (int j = i; j < data.size(); ++j) {
				char c = data[j];

				if (c != '<' && c != '-') {
					continue;
				}

				if (data.is_word_at(j, "-->")) {
					// if --> is encountered while in any state, switch to data1 state
					state = STATE_DATA_1;
					continue;
				}

				if (state == STATE_DATA_1) {

					if (data.is_word_at(j, "<!--")) {
						// if <!-- is encountered while in data1 state, switch to data2 state
						state = STATE_DATA_2;
					} else if (data.is_word_at(j, "</script")) {
						// if </script[\s/>] is encountered while in any other state (than data3), stop parsing
						done = true;
					}

				} else if (state == STATE_DATA_2) {

					if (data.is_word_at(j, "<script")) {
						// if <script[\s/>] is encountered while in data2 state, switch to data3 state
						state = STATE_DATA_3;
					} else if (data.is_word_at(j, "</script")) {
						// if </script[\s/>] is encountered while in any other state (than data3), stop parsing
						done = true;
					}

				} else if (state == STATE_DATA_3) {

					// if </script[\s/>] is encountered while in data3 state, switch to data2 state
					if (data.is_word_at(j, "</script")) {
						state = STATE_DATA_2;
					}
				}

				if (done) {
					state = STATE_NONE;
					BBCodeParserTag *t = new BBCodeParserTag();

					t->data = data.substr(i, j - i);
					t->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT;

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

	root = new BBCodeParserTag();

	// process tags into hierarchical order
	Vector<BBCodeParserTag *> tag_stack;
	for (int i = 0; i < tags.size(); ++i) {
		BBCodeParserTag *t = tags[i];

		if (t == nullptr) {
			RLOG_ERR("BBCodeParser::parse: t == nullptr!");
			continue;
		}

		if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE) {
			RLOG_ERR("BBCodeParser::parse: t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_NONE!");
			delete t;
			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_OPENING_TAG) {
			tag_stack.push_back(t);

			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CONTENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_COMMENT) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_DOCTYPE) {
			if (tag_stack.size() == 0) {
				root->tags.push_back(t);
			} else {
				tag_stack[tag_stack.size() - 1]->tags.push_back(t);
			}

			tags[i] = nullptr;
			continue;
		} else if (t->type == BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_CLOSING_TAG) {
			if (tag_stack.size() == 0) {
				delete t;
				tags[i] = nullptr;

				// ill-formed html
				continue;
			}

			// find it's pair
			int tag_index = 0;
			for (int j = tag_stack.size() - 1; j > 0; --j) {
				BBCodeParserTag *ts = tag_stack[j];

				// we sould only have opening tags on the stack
				if (ts->tag == t->tag) {
					// found
					tag_index = j;
					break;
				}
			}

			BBCodeParserTag *opening_tag = tag_stack[tag_index];

			// mark everything else that we found before finding the opening tag as self closing, and add them to out opening tag
			// If the html is ill formed, it just grabs everything from the tag stack
			for (int j = tag_index + 1; j < tag_stack.size(); ++j) {
				BBCodeParserTag *ts = tag_stack[j];

				ts->type = BBCodeParserTag::BBCODE_PARSER_TAG_TYPE_SELF_CLOSING_TAG;
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
		BBCodeParserTag *t = tags[i];

		if (t != nullptr) {
			RLOG_ERR("BBCodeParser::parse(const String &data): tag was not processed!\n");
			t->print();

			delete t;
		}
	}
}

String BBCodeParser::to_string() {
	if (!root) {
		return "";
	}

	return root->to_string();
}
void BBCodeParser::print() {
	if (root) {
		root->print();
	}
}

BBCodeParser::BBCodeParser() {
	root = nullptr;
}

BBCodeParser::~BBCodeParser() {
	if (root) {
		delete root;
	}
}
