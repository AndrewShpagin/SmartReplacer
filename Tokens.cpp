#include "SmartReplacer.h"

spaces::spaces() {
	tokenID = " ";
	begin.smart(" \t");
	middle = end = begin;
}

BaseToken* spaces::autonew() { return _autonew(); }

int spaces::matches(const char* str, int pos) {
	int r = WalkForward(str, pos);
	return r;
}

void spaces::autofill(const cList<BaseToken*>& tokens) {
	tokenValue = " ";
}

BaseToken::BaseToken() {
	begin.add(" \t");
	end = middle = begin;
	isExact = false;
	ZeroLengthAllowed = false;
}

int BaseToken::WalkForward(const char* str, int pos) {
	int pos0 = pos;
	if (begin.check(str[pos])) {
		char last = str[pos];
		pos++;
		while (middle.check(str[pos])) {
			last = str[pos];
			pos++;
		}
		if (end.check(str[pos])) {
			last = str[pos];
			pos++;
		}
		if (!end.check(last))return pos0;
	}
	return pos;
}

int BaseToken::WalkBack(const char* str, int pos) {
	if (pos) {
		int pos0 = pos--;
		if (end.check(str[pos])) {
			char last = str[pos];
			pos--;
			while (middle.check(last = str[pos]))pos--;
			if (begin.check(str[pos])) {
				last = str[pos];
				pos--;
			}
			if (!begin.check(last))return pos0;
		}
	}
	return pos + 1;
}

int BaseToken::matches(const char* str, int pos) {
	int r = WalkForward(str, pos);
	tokenValue = cStr(str, pos, r - pos);
	if (ZeroLengthAllowed)return r;
	return r == pos ? -1 : r;
}

BaseToken* BaseToken::autonew() { return _autonew(); }

void BaseToken::autofill(const cList<BaseToken*>& tokens) {
	if (tokenID[0] == '$') {
		for (int i = 0; i < tokens.Count(); i++) {
			if (tokenID == tokens[i]->tokenID) {
				tokenValue = tokens[i]->tokenValue;
			}
		}
	}
}

exact_match::exact_match() {
	tokenID = "";
	begin.allow_all();
	begin.remove(' ');
	begin.remove('\t');
	begin.remove(0);
	//begin.remove('\n');
	//begin.remove('\r');
	begin.remove('$');
	middle = end = begin;
	isExact = true;
}

exact_match::exact_match(const char* str, int begin, int end) {
	exact_match();
	substring = cStr(str, begin, end - begin);
	//substring.Replace("\n", "");
	//substring.Replace("\r", "");
}

int exact_match::fill(const char* str, int pos) {
	int p0 = pos;
	while (middle.check(str[pos]))pos++;
	substring = cStr(str, p0, pos - p0);;
	return pos;
}

int exact_match::WalkForward(const char* str, int pos) {
	const auto p00 = pos;
	if (substring.Length()) {
		int p0 = 0;
		while (str[pos] == substring[p0]) {
			pos++;
			p0++;
		}
		if (p0 == substring.Length()) { return pos; }
	}
	return p00;
}

int exact_match::WalkBack(const char* str, int pos) { return -1; }

int exact_match::matches(const char* str, int pos) {
	const auto L = substring.Length();
	int k;
	// ReSharper disable once CppPossiblyErroneousEmptyStatements
	for (k = 0; k < L && str[pos + k] == substring[k]; k++);
	if (k == L) {
		tokenValue = cStr(str, pos, L);
	}
	if (ZeroLengthAllowed)return pos + L;
	return k == L ? pos + L : -1;
}

BaseToken* exact_match::autonew() { return _autonew(); }

void exact_match::autofill(const cList<BaseToken*>& tokens) {
	tokenValue = substring;
}

string::string() { tokenID = classname(); }

int string::WalkForward(const char* str, int pos) {
	const auto p0 = pos;
	if (str[pos] == '"') {
		while (true) {
			char c = str[pos++];
			if (c == '\\')pos++;
			else {
				if (c == '\"') { return pos; }
				else
					if (c == 0 || c == '\n' || c == '\r')return p0;
			}
		}
	}
	return p0;
}

int string::WalkBack(const char* str, int pos) {
	int p0 = pos--;
	if (str[pos] == '"') {
		while (true) {
			char c = str[pos--];
			if (c == '\"' && str[pos] == '\\')pos--;
			else {
				if (c == '\"') { return pos + 1; }
				else
					if (c == 0 || c == '\n' || c == '\r')return p0;
			}
		}
	}
	return p0;
}

BaseToken* string::autonew() { return _autonew(); }

const char* string::hint() {
	return "quoted string \"...\".";
}

characters::characters() {
	tokenID = classname();
	begin.smart("a..zA..Z0..9_.");
	middle = end = begin;
}

BaseToken* characters::autonew() { return _autonew(); }

const char* characters::hint() {
	return "regular characters - [a..z,A..Z,0..9,_,.]";
}

floating::floating() {
	tokenID = classname();
	begin.smart("+-0..9.");
	middle.smart("0..9.");
	end.smart("0..9.f");
}

BaseToken* floating::autonew() { return _autonew(); }

const char* floating::hint() {
	return "floating-point value.";
}

numbers::numbers() {
	tokenID = classname();
	begin.smart("0..9");
	middle = end = begin;
}

BaseToken* numbers::autonew() { return _autonew(); }

square::square() {
	tokenID = classname();
	begin.smart("[");
	middle.nothing();
	middle.remove(0);
	end.smart("]");
}

BaseToken* square::autonew() { return _autonew(); }

const char* square::hint() {
	return "block between square brackets [ ... ].";
}


brackets::brackets() {
	tokenID = classname();
	begin.smart("(");
	middle.nothing();
	middle.remove(0);
	end.smart(")");
}

BaseToken* brackets::autonew() { return _autonew(); }

const char* brackets::hint() {
	return "block between round brackets ( ... ).";
}

block::block() {
	tokenID = classname();
	begin.smart("{");
	middle.nothing();
	middle.remove(0);
	end.smart("}");
}

BaseToken* block::autonew() { return _autonew(); }

const char* block::hint() {
	return "block between brackets { ... }.";
}

const char* numbers::hint() {
	return "any amount of numbers [0..9].";
}

expr::expr() {
	tokenID = classname();
	begin.smart("#a..zA..Z0..9([{+-*&~\\!:_\"'%$`^@");
	middle.smart("#a..zA..Z0..9()[]{}+-*&~\\!:_\"' /=<>%$`.^@");
	end.smart("#a..zA..Z0..9)]}+-*&~\\!:_\"' /%$`^@");
}

BaseToken* expr::autonew() { return _autonew(); }



int expr::WalkForward(const char* str, int pos) {
	BracketsWalker qd;
	const int pos0 = pos;
	if (begin.check(str[pos])) {
		do {
			if (str[pos] == 0)return pos;
			auto nps = qd.detect_bracket(str, pos);
			if (nps > pos) {
				if (qd.is_negative()) {
					return pos;
				}
				pos = nps;
				continue;
			}
			else {
				if (qd.is_zero()) {
					if (!middle.check(str[pos])) {
						return pos;
					}
				}
				pos++;
			}
		} while (true);
	}
	return pos;
}

const char* expr::hint() {
	return "correct expression.";
}

variable::variable() {
	tokenID = classname();
	begin.smart("a..zA..Z_");
	middle.smart("a..zA..Z_0..9");
	end = middle;
}

BaseToken* variable::autonew() { return _autonew(); }

const char* variable::hint() {
	return "correct variable name.";
}

custom::custom() {
	tokenID = classname();
}

BaseToken* custom::autonew() {
	return _autonew();
}

void custom::autofill(const cList<BaseToken*>& tokens) {
	if (tokenID[0] == '$') {
		for (int i = 0; i < tokens.Count(); i++) {
			if (tokenID == tokens[i]->tokenID) {
				cStr text = tokens[i]->tokenValue;
				cStr text1;
				text.Trim("\" \n\r\t");
				text.ReplaceAny(";.,", ' ');
				if (text.Length() > 1 && text.IndexOf(" ") == -1) {
					int v = atoi(text);
					if (v) {
						int nn = v % 10;
						v /= 10;
						for (int k = 0; k<nn; k++) {
							char cc[3];
							sprintf_s<3>(cc,"%d ",v%10);
							v /= 10;
							text1 += cc;
						}
					}
					text1.Trim(" ");
				}
				else {
					text1 = text;
				}
				text1.Insert(0, "\"");
				text1 += "\"";
				tokenValue = text1;
			}
		}
	}
}

const char* custom::hint() {
	return nullptr;
}
