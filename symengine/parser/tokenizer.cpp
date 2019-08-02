/* Generated by re2c 1.1.1 on Fri Aug  2 09:17:07 2019 */
#line 1 "tokenizer.re"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <istream>
#include <fstream>
#include <memory>

#include "tokenizer.h"
#include "unique.h"

enum num_t {
    // Unrecognized token. This is caused by a syntax error, unless there is a
    // bug in the tokenizer rules.
    ERR_UNKNOWN_TOKEN,

    // Null character \x00 encountered in the input file (we use it to
    // terminate). With a different SIZE, this Null character might be
    // mis-recognized as the END token. Solution: ensure the file does not
    // contain the null character.
    ERR_NULL,

    // The buffer input_t::buf is not big enough to hold `need` chars. The
    // `need <= YYMAXFILL`, where `YYMAXFILL` is the longest (sequence) of
    // tokens needed to be in the buffer simultaneously.
    // Solution: Increase SIZE.
    // Note: Whenever the buffer fills, the data (all the loaded tokens in the
    // buffer) gets moved in it and we start over. To ensure this happens
    // rarely, make SIZE large enough, for example 64*1024 (64 KB). The amount
    // of data to move is typically low, so this size should be large enough
    // not to be noticeable. One can run a benchmark study what size stops
    // being noticeable for a large file.
    ERR_BUF,

    // We reached the end of input.
    END,
    WS, OPERATOR, POW, LE, EQ, GE, IDENTIFIER, NUMERIC,
    IMPLICIT_MUL};

#define YYMAXFILL 3

static const size_t SIZE = 64*1024;

size_t sread(char *ptr, size_t size, size_t count, std::istream &stream)
{
    stream.read(ptr, count);
    return stream.gcount();
}

using SymEngine::Parser;


struct input_t {
    unsigned char buf[SIZE + YYMAXFILL];
    unsigned char *lim;
    unsigned char *cur;
    unsigned char *mar;
    unsigned char *tok;
    bool eof;

    std::istream &file;

    SymEngine::ParserBase::STYPE__ *val;

    input_t(std::istream &f)
        : buf()
        , lim(buf + SIZE)
        , cur(lim)
        , mar(lim)
        , tok(lim)
        , eof(false)
        , file(f)
    {}
    bool fill(size_t need)
    {
        if (eof) {
            return false;
        }
        const size_t free = tok - buf;
        if (free < need) {
            return false;
        }
        memmove(buf, tok, lim - tok);
        lim -= free;
        cur -= free;
        mar -= free;
        tok -= free;
        lim += sread((char*)lim, 1, free, file);
        if (lim < buf + SIZE) {
            eof = true;
            memset(lim, 0, YYMAXFILL);
            lim += YYMAXFILL;
        }
        return true;
    }
};

int yylex(input_t &in)
{
    for (;;) {
        in.tok = in.cur;
        
#line 107 "tokenizer.cpp"
{
	unsigned char yych;
	static const unsigned char yybm[] = {
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,  32,  32,  32,   0,  32,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		 32,   0,   0,   0,   0,   0,   0,   0, 
		  0,   0,   0,   0,   0,   0,   0,   0, 
		192, 192, 192, 192, 192, 192, 192, 192, 
		192, 192,   0,   0,   0,   0,   0,   0, 
		  0, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128,   0,   0,   0,   0, 128, 
		  0, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128,   0,   0,   0,   0,   0, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
		128, 128, 128, 128, 128, 128, 128, 128, 
	};
	if ((in.lim - in.cur) < 2) if (!in.fill(2)) return 0;
	yych = *in.cur;
	if (yybm[0+yych] & 32) {
		goto yy6;
	}
	if (yych <= '>') {
		if (yych <= '-') {
			if (yych <= '&') {
				if (yych <= 0x00) goto yy2;
				if (yych <= '%') goto yy4;
				goto yy9;
			} else {
				if (yych <= '\'') goto yy4;
				if (yych == '*') goto yy11;
				goto yy9;
			}
		} else {
			if (yych <= '9') {
				if (yych <= '.') goto yy12;
				if (yych <= '/') goto yy9;
				goto yy13;
			} else {
				if (yych <= ';') goto yy4;
				if (yych <= '<') goto yy16;
				if (yych <= '=') goto yy17;
				goto yy18;
			}
		}
	} else {
		if (yych <= '`') {
			if (yych <= 'Z') {
				if (yych <= '?') goto yy4;
				if (yych <= '@') goto yy19;
				goto yy21;
			} else {
				if (yych <= ']') goto yy4;
				if (yych <= '^') goto yy9;
				if (yych <= '_') goto yy21;
				goto yy4;
			}
		} else {
			if (yych <= '|') {
				if (yych <= 'z') goto yy21;
				if (yych <= '{') goto yy4;
				goto yy9;
			} else {
				if (yych == '~') goto yy9;
				if (yych <= 0x7F) goto yy4;
				goto yy21;
			}
		}
	}
yy2:
	++in.cur;
#line 128 "tokenizer.re"
	{
                    if (in.lim - in.tok == YYMAXFILL) {
                        return 0;
                    } else {
                        printf("ERR NULL.\n");
                        return 0;
                    }
                }
#line 207 "tokenizer.cpp"
yy4:
	++in.cur;
yy5:
#line 127 "tokenizer.re"
	{ printf("ERR unknown token\n"); return 0; }
#line 213 "tokenizer.cpp"
yy6:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return 0;
	yych = *in.cur;
	if (yybm[0+yych] & 32) {
		goto yy6;
	}
#line 136 "tokenizer.re"
	{ continue; }
#line 223 "tokenizer.cpp"
yy9:
	++in.cur;
yy10:
#line 138 "tokenizer.re"
	{ return in.tok[0]; }
#line 229 "tokenizer.cpp"
yy11:
	yych = *++in.cur;
	if (yych == '*') goto yy19;
	goto yy10;
yy12:
	yych = *++in.cur;
	if (yych <= '/') goto yy5;
	if (yych <= '9') goto yy24;
	goto yy5;
yy13:
	++in.cur;
	if ((in.lim - in.cur) < 3) if (!in.fill(3)) return 0;
	yych = *in.cur;
	if (yybm[0+yych] & 64) {
		goto yy13;
	}
	if (yych <= '^') {
		if (yych <= '@') {
			if (yych == '.') goto yy26;
		} else {
			if (yych == 'E') goto yy30;
			if (yych <= 'Z') goto yy27;
		}
	} else {
		if (yych <= 'd') {
			if (yych != '`') goto yy27;
		} else {
			if (yych <= 'e') goto yy30;
			if (yych <= 'z') goto yy27;
			if (yych >= 0x80) goto yy27;
		}
	}
yy15:
#line 147 "tokenizer.re"
	{
                *(in.val) = std::string((char*)in.tok, in.cur-in.tok);
                return Parser::NUMERIC;
            }
#line 268 "tokenizer.cpp"
yy16:
	yych = *++in.cur;
	if (yych == '=') goto yy31;
	goto yy10;
yy17:
	yych = *++in.cur;
	if (yych == '=') goto yy33;
	goto yy5;
yy18:
	yych = *++in.cur;
	if (yych == '=') goto yy35;
	goto yy10;
yy19:
	++in.cur;
#line 139 "tokenizer.re"
	{ return Parser::POW; }
#line 285 "tokenizer.cpp"
yy21:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return 0;
	yych = *in.cur;
	if (yybm[0+yych] & 128) {
		goto yy21;
	}
#line 143 "tokenizer.re"
	{
                *(in.val) = std::string((char*)in.tok, in.cur-in.tok);
                return Parser::IDENTIFIER;
            }
#line 298 "tokenizer.cpp"
yy24:
	++in.cur;
	if ((in.lim - in.cur) < 3) if (!in.fill(3)) return 0;
	yych = *in.cur;
yy25:
	if (yych <= '^') {
		if (yych <= '@') {
			if (yych <= '/') goto yy15;
			if (yych <= '9') goto yy24;
			goto yy15;
		} else {
			if (yych == 'E') goto yy30;
			if (yych <= 'Z') goto yy27;
			goto yy15;
		}
	} else {
		if (yych <= 'd') {
			if (yych == '`') goto yy15;
			goto yy27;
		} else {
			if (yych <= 'e') goto yy30;
			if (yych <= 'z') goto yy27;
			if (yych <= 0x7F) goto yy15;
			goto yy27;
		}
	}
yy26:
	yych = *++in.cur;
	if (yych == 'E') goto yy27;
	if (yych != 'e') goto yy25;
yy27:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return 0;
	yych = *in.cur;
yy28:
	if (yych <= '^') {
		if (yych <= '9') {
			if (yych >= '0') goto yy27;
		} else {
			if (yych <= '@') goto yy29;
			if (yych <= 'Z') goto yy27;
		}
	} else {
		if (yych <= '`') {
			if (yych <= '_') goto yy27;
		} else {
			if (yych <= 'z') goto yy27;
			if (yych >= 0x80) goto yy27;
		}
	}
yy29:
#line 151 "tokenizer.re"
	{
                *(in.val) = std::string((char*)in.tok, in.cur-in.tok);
                return Parser::IMPLICIT_MUL;
            }
#line 355 "tokenizer.cpp"
yy30:
	yych = *(in.mar = ++in.cur);
	if (yych <= ',') {
		if (yych == '+') goto yy37;
		goto yy28;
	} else {
		if (yych <= '-') goto yy37;
		if (yych <= '/') goto yy28;
		if (yych <= '9') goto yy39;
		goto yy28;
	}
yy31:
	++in.cur;
#line 140 "tokenizer.re"
	{ return Parser::LE; }
#line 371 "tokenizer.cpp"
yy33:
	++in.cur;
#line 142 "tokenizer.re"
	{ return Parser::EQ; }
#line 376 "tokenizer.cpp"
yy35:
	++in.cur;
#line 141 "tokenizer.re"
	{ return Parser::GE; }
#line 381 "tokenizer.cpp"
yy37:
	yych = *++in.cur;
	if (yych <= '/') goto yy38;
	if (yych <= '9') goto yy39;
yy38:
	in.cur = in.mar;
	goto yy29;
yy39:
	++in.cur;
	if (in.lim <= in.cur) if (!in.fill(1)) return 0;
	yych = *in.cur;
	if (yych <= '^') {
		if (yych <= '9') {
			if (yych <= '/') goto yy15;
			goto yy39;
		} else {
			if (yych <= '@') goto yy15;
			if (yych <= 'Z') goto yy27;
			goto yy15;
		}
	} else {
		if (yych <= '`') {
			if (yych <= '_') goto yy27;
			goto yy15;
		} else {
			if (yych <= 'z') goto yy27;
			if (yych <= 0x7F) goto yy15;
			goto yy27;
		}
	}
}
#line 155 "tokenizer.re"

    }
}

namespace SymEngine {

Tokenizer::Tokenizer() {
}

Tokenizer::~Tokenizer() {
}

void Tokenizer::scan_stream(std::istream &stream) {
    m_in = SymEngine::make_unique<input_t>(stream);
    m_in->val = dval;
}

int Tokenizer::lex() {
    return yylex(*m_in);
}

} // SymEngine
