#pragma once

class BaseToken {
protected:
	CharactersBitset begin;
	CharactersBitset middle;
	CharactersBitset end;
public:
	BaseToken();
	virtual ~BaseToken() {}
	cStr tokenID;
	cStr tokenValue;
	bool isExact;
	bool ZeroLengthAllowed;

	///returns position of token end, walking forward
	virtual int WalkForward(const char* str, int pos);
	///returns position of the token begin, walking back, return -1 if walking back is not supported
	virtual int WalkBack(const char* str, int pos);
	///check if string matches the token
	virtual int matches(const char* str, int pos);
	///return allocated object pointer
	virtual BaseToken* autonew();
	///fills tokenValue using input tokens
	virtual void autofill(const cList<BaseToken*>& tokens);
	///returns state, 1 for exact text, 2 for spaces, 0 othervice
	virtual int state() { return 0; }
	///returns the hint for this token
	virtual const char* hint() { return NULL; }
};

class spaces : public BaseToken {
public:
	spaces();
	virtual BaseToken* autonew();
	int matches(const char* str, int pos) override;
	void autofill(const cList<BaseToken*>& tokens) override;
	int state() override { return 2; }
};

class exact_match : public BaseToken {
public:
	cStr substring;
	exact_match();
	exact_match(const char* str, int begin, int end);
	int fill(const char* str, int pos);
	int WalkForward(const char* str, int pos) override;
	int WalkBack(const char* str, int pos) override;
	int matches(const char* str, int pos) override;
	virtual BaseToken* autonew();
	void autofill(const cList<BaseToken*>& tokens) override;
	int state() override { return 1; }
};

///$string
class string : public BaseToken {
public:
	string();
	int WalkForward(const char* str, int pos) override;
	int WalkBack(const char* str, int pos) override;
	virtual BaseToken* autonew();
	const char* hint() override;
};

///$characters
class characters : public BaseToken {
public:
	characters();
	virtual BaseToken* autonew();
	const char* hint() override;
};

///$floating
class floating : public BaseToken {
public:
	floating();
	virtual BaseToken* autonew();
	const char* hint() override;
};

///$numbers
class numbers : public BaseToken {
public:
	numbers();
	virtual BaseToken* autonew();
	int state() override { return 1; }
	const char* hint() override;;
};

///$expression
class expr : public BaseToken {
public:
	expr();
	virtual BaseToken* autonew();
	int WalkForward(const char* str, int pos) override;
	const char* hint() override;
};

///$block
class block : public expr {
public:
	block();
	virtual BaseToken* autonew();
	const char* hint() override;
};
///$brackets
class brackets : public expr {
public:
	brackets();
	virtual BaseToken* autonew();
	const char* hint() override;
};
///$square
class square : public expr {
public:
	square();
	virtual BaseToken* autonew();
	const char* hint() override;
};
///$variable
class variable : public BaseToken {
public:
	variable();
	virtual BaseToken* autonew();
	const char* hint() override;
};

///$custom
class custom : public expr {
public:
	custom();
	virtual BaseToken* autonew();
	virtual void autofill(const cList<BaseToken*>& tokens);
	const char* hint() override;
};