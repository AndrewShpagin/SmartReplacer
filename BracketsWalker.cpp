#include "SmartReplacer.h"

BracketsWalker::BracketsWalker() { for (int i = 0; i < nbrackets; i++)depth[i] = 0; }

int BracketsWalker::detect_bracket(const char* str, int pos) {
	char c0 = str[pos];
	if (!c0)return pos;
	char c1 = str[pos + 1];
	if(c0=='/' && c1=='/') {
		while (str[pos] && str[pos] != '\n')pos++;
		return pos;
	}
	int open_idx = -1;
	int close_idx = -1;
	size_t L = 0;
	for (int k = 0; k < nbrackets; k++) {
		if (c0 == open_brackets[k][0]) {
			char b1 = open_brackets[k][1];
			if (b1 == 0 || (b1 && c1 && c1 == b1)) {
				open_idx = k;
				L = strlen(open_brackets[k]);
			}
		}
		if (c0 == close_brackets[k][0]) {
			char b1 = close_brackets[k][1];
			if (b1 == 0 || (b1 && c1 && c1 == b1)) {
				close_idx = k;
				L = strlen(close_brackets[k]);
			}
		}
	}
	if (close_idx >= 0 && depth[close_idx] > 0)depth[close_idx]--;
	else if (open_idx >= 0)depth[open_idx]++;
	else if (close_idx >= 0)depth[close_idx]--;
	return int(pos + L);
}

bool BracketsWalker::is_zero() {
	for (int i = 0; i < nbrackets; i++) { if (depth[i])return false; }
	return true;
}

bool BracketsWalker::is_negative() {
	for (int i = 0; i < nbrackets; i++) { if (depth[i] < 0)return true; }
	return false;
}
