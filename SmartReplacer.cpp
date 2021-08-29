// SmartReplacer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "comms-Math/comms-Math.h"
#include "SmartReplacer.h"
#include "mdfile.h"
#include <filesystem>

#ifdef _WIN32
#include <windows.h>

comms::cStr SmartReplacer::LogFileName;

void printlog(const char* mask,...) {
	if(SmartReplacer::LogFileName.Length()) {
		FILE* f = nullptr;
		fopen_s(&f, SmartReplacer::LogFileName, "a");
		if(f) {
			va_list args;
			va_start(args, mask);
			vfprintf(f, mask, args);
			va_end(args);
			fclose(f);
		}
	}
}

int console_width() {
	static int width = 0;
	if (width == 0) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}
	return width;
}
#else
int console_width() {
	return 80;
}
#endif
int SmartReplacer::Verbose = 0;
int SmartReplacer::ChangedFiles = 0;
int SmartReplacer::SummaryReplacements = 0;

namespace fs = std::filesystem;

void get_files(const char* folder, const cList<cStr>& ext, cList<cStr>& result, bool recursive) {
	const fs::path pathToShow{ folder };

	for (const auto& entry : fs::directory_iterator(pathToShow)) {
		const auto filenameStr = entry.path().filename().string();
		if (entry.is_directory()) {
			if(recursive) {
				cStr dir= folder;
				dir.TrimEnd("\\/");
				dir += "/";
				dir += filenameStr.c_str();
				get_files(dir, ext, result, recursive);
			}
		}
		else if (entry.is_regular_file()) {
			bool valid = false;
			cStr fn = filenameStr.c_str();
			cStr fext = fn.GetFileExtension();
			bool has = false;
			for(int i=0;i<ext.Count();i++) {
				if(cStr::EqualsNoCase(ext[i],fext)) {
					cStr fpath = folder;
					fpath.TrimEnd("\\/");
					fpath += "/";
					fpath += fn;
					result.Add(fpath);
				}
			}
		}
	}
}

cStr _todir(const char* path) {
	cStr s = path;
	s.Trim("\"");
	s.TrimEnd("\\/");
	s += "/";
	return s;
}
bool check_stupid_del(const char* path) {
	return strlen(path) <= 3;
}

cStr BackupFolder;

std::tuple<int,int,int> xyz() {
	return std::tuple<int, int, int>(1, 2, 3);
}
int main(int argc, char* argv[]) {
	int x, y, z;
	std::tuple<int, int, int>(x, y, z) = xyz();
	cStr s1 = "wsxw";
	s1 += 1;
	
	cStr infile;
	cStr outfile;
	cStr infolder;
	cStr resfolder = "";
	bool recursive = false;
	bool cleanup = false;
	cList<cStr> ext;
	cStr patfile = "patterns.txt";
	cStr md = read_text("readme.md");


	auto curt = std::chrono::system_clock::now();
	auto end_time = std::chrono::system_clock::to_time_t(curt);
	char timebuf[256];
	ctime_s(timebuf, sizeof(timebuf), &end_time);
	BackupFolder = cStr("backups/") + cStr(timebuf);
	BackupFolder.Trim("\n\t\t ");
	BackupFolder.Replace(":", "_");
	BackupFolder += "/";
	
	if(md.Length()) {
		md.Replace("\\", "[SLASH]");
		md.Replace("[SLASH]", "\\\\");
		
		md.Replace("\"", "[QUOTE]");
		md.Replace("[QUOTE]", "\\\"");
		
		md.Replace("\r\n", "[CRLF]");
		md.Replace("[CRLF]", "\\n\"\r\n\"");
		
		md.Insert(0, "const char* mdfile=\"");
		md += "\";";
		save_text("mdfile.h", md);
	}
	if (argc <= 1) {
		SmartReplacer sr;

		cStr md = mdfile;
		int us = md.IndexOf("Usage:");
		if(us!=-1) {
			md.Remove(0, us);
		}
		md.Replace("```", "");
		md.Replace("[end]", "");
		int id1 = md.IndexOf("List of tokens:");
		int id2= md.IndexOf("***");
		if(id1!=-1 && id2!=-1) {
			md.Remove(id1, id2 - id1);
			cStr ins;
			for (int i = 0; i < sr.palette.Count(); i++) {
				const char* h = sr.palette[i]->hint();
				if (h) {
					ins += cStr::Format(" %s - %s  \n", sr.palette[i]->tokenID.ToCharPtr(), h);
				}
			}
			ins += "\n";
			md.Insert(id1, ins);
		}
		printf("%s", md.ToCharPtr());
		//printf("\n%d\n", console_width());
	}
	else {
		for (int i = 0; i < argc; i++) {
			if (i < argc - 1) {
				if (!strcmp(argv[i], "/i")) {
					infile = argv[i + 1];
					infile.Trim("\"");
				}
				if (!strcmp(argv[i], "/o")) {
					outfile = argv[i + 1];
					outfile.Trim("\"");
				}
				if (!strcmp(argv[i], "/p")) {
					patfile = argv[i + 1];
					patfile.Trim("\"");
				}
				if (!strcmp(argv[i], "/r")) {
					infolder = _todir(argv[i + 1]);
				}
				if (!strcmp(argv[i], "/r")) {
					infolder = _todir(argv[i + 1]);
					recursive = true;
				}
				if (!strcmp(argv[i], "/c")) {
					resfolder = _todir(argv[i + 1]);
				}
				if (!strcmp(argv[i], "/f")) {
					infolder = _todir(argv[i + 1]);
					recursive = false;
				}
			}
			if (!strcmp(argv[i], "/remove")) {
				cleanup = true;
			}
			if (!strncmp(argv[i], "/.",2)) {
				cStr s = argv[i];
				s.Remove(0, min(2,s.Length()));
				ext.Add(s);
			}
		}
	}
	if(infolder.Length()) {
		std::filesystem::path patt(patfile.ToCharPtr());
		std::filesystem::path patbk((BackupFolder + cStr("pattern.txt")).ToCharPtr());
		std::filesystem::create_directories(std::filesystem::path(BackupFolder.ToCharPtr()));
		std::filesystem::copy_file(patt, patbk);
		resfolder += "changed/";
		std::filesystem::create_directories(std::filesystem::path(resfolder.ToCharPtr()));
		SmartReplacer::LogFileName = resfolder + cStr("changes.txt");
		if(cleanup) {
			std::filesystem::path pat(resfolder.ToCharPtr());
			std::filesystem::remove_all(pat);
		}
		cList<cStr> res;
		get_files(infolder, ext, res, recursive);
		for(int i=0;i<res.Count();i++) {
			SmartReplacer sr;
			sr.process_file(res[i], res[i], patfile, infolder, resfolder);
		}
	}
	if (infile.Length()) {
		if (outfile.Length() == 0)outfile = infile;
		SmartReplacer sr;
		sr.process_file(infile, outfile, patfile);
	}
	if (SmartReplacer::SummaryReplacements) {
		printf("\n\nFiles changed: %d\nReplacements: %d\n", SmartReplacer::ChangedFiles, SmartReplacer::SummaryReplacements);
	}
	return 0;
}

cStr read_text(const char* path) {
	cStr s;
	FILE* F = nullptr;
	fopen_s(&F, path, "rb");
	if (F) {
		fseek(F, 0, SEEK_END);
		int p = ftell(F);
		fseek(F, 0, SEEK_SET);
		char* buf = new char[p + 1];
		fread(buf, 1, p, F);
		buf[p] = 0;
		s = buf;
		fclose(F);
	}
	return s;
}

void save_text(const char* path, const char* text, const char* initialfolder, const char* changedfolder) {
	FILE* F = nullptr;
	cStr fpath = path;
	if(initialfolder && changedfolder && strlen(initialfolder) && strlen(changedfolder)) {
		fpath.Remove(0, (int)strlen(initialfolder));
		fpath.Insert(0, changedfolder);
		cStr dir = fpath;
		dir.RemoveFileName();
		std::filesystem::path p(dir.ToCharPtr());
		std::filesystem::create_directories(p);
	}
	fopen_s(&F, fpath, "wb");
	if (F) {
		fwrite(text, 1, strlen(text), F);
		fclose(F);
	}
}

SmartReplacer::SmartReplacer() {
	palette.Add(new string);
	palette.Add(new characters);
	palette.Add(new floating);
	palette.Add(new numbers);
	palette.Add(new expr);
	palette.Add(new variable);
	palette.Add(new spaces);
	palette.Add(new exact_match);
	palette.Add(new block);
	palette.Add(new square);
	palette.Add(new brackets);
	palette.Add(new custom);
}

SmartReplacer::~SmartReplacer() {
	clear();
	palette.FreeContents();
	palette.Free();
}

void SmartReplacer::clear() {
	parsed_in.FreeContents();
	parsed_out.FreeContents();
	parsed_in.Free();
	parsed_out.Free();
}

bool SmartReplacer::process_file(const char* in_file_name, const char* out_file_name, const char* patterns_file, const char* src_folder, const char*
                                 dst_folder) {
	int dL = console_width() - (int)strlen(in_file_name) - 2;
	if (dL < 0)dL = 0;
	cStr empty(dL, ' ');
	printf("%s%s\r", in_file_name, empty.ToCharPtr());
	auto s_in = read_text(in_file_name);
	auto s_src = s_in;
	auto pat = read_text(patterns_file);
	int start = 0;
	int nch = 0;
	curfile = in_file_name;
	do {
		clear();
		auto end = create_from_textfile(pat, start);
		if (end != -1) {
			nch += process(s_in);
			start = end;
		}
		else break;
	} while (true);
	if(nch) {
		//save to backup first
		save_text(out_file_name, s_src, src_folder, ::BackupFolder);
		printf("%s%s\n", in_file_name, empty.ToCharPtr());
		save_text(out_file_name, s_in, src_folder, dst_folder);
		printf("%d replacements\n", nch);
		ChangedFiles++;
		SummaryReplacements += nch;
	}
	return true;
}

int SmartReplacer::create_from_textfile(const char* text, int start) {
	clear();
	const char* nexteq = strstr(text + start, "=====");
	int L = (int)strlen(text);
	if(nexteq) {
		cStr s1(text, start, int(nexteq - text) - start);
		s1.Trim("\n\r\t ");
		const char* nexteq1 = strstr(nexteq, "\n");
		if(nexteq1) {
			const char* nexteq2 = strstr(nexteq1, "=====");
			int pos2 = L;
			if(nexteq2) {
				pos2 = int(nexteq2 - text);
			}
			cStr s2(text, int(nexteq1-text), pos2 - int(nexteq1 - text));
			s2.Trim("\n\r\t ");
			set_patterns(s1, s2);
			if(nexteq2) {
				const char* nexteq3 = strstr(nexteq2, "\n");
				pos2 = nexteq3 ? int(nexteq3 - text) : L;
			}
			return pos2;
		}
	}
	return -1;
}

void SmartReplacer::parse_pattern(const char* string, cList<BaseToken*>& list) {
	int L = (int)strlen(string);
	CharactersBitset vars;
	vars.smart("A..Za..z0..9_");
	bool nextzero = false;
	for (int i = 0; i < L; i++) {
		char c = string[i];
		int p0 = i;
		if (c == '$') {
			i++;
			do {
				c = string[i];
				if (vars.check(c)) { i++; }
				else break;
			}
			while (true);
			if (i > p0 + 1) {
				cStr cvar(string, p0, i - p0);
				if(cvar=="$zero") {
					nextzero = true;
					i--;
				}
				else {
					for (int k = 0; k < palette.Count(); k++) {
						cStr& token = palette[k]->tokenID;
						if (token.Length() > 1) {
							if (!strncmp(cvar, token, token.Length())) {
								BaseToken* bt = palette[k]->autonew();
								bt->tokenID = cvar;
								bt->ZeroLengthAllowed = nextzero;
								nextzero = false;
								list.Add(bt);
								i--;
								break;
							}
						}
					}
				}
			}
		}
		else if (c == ' ' || c == '\t') {
			list.Add(new spaces);
			i = list.GetLast()->WalkForward(string, i) - 1;
		}
		else {
			exact_match* m = new exact_match;
			i = m->fill(string, i);
			list.Add(m);
			i--;
		}
	}
}

void SmartReplacer::set_patterns(const char* to_find, const char* replace) {
	parse_pattern(to_find, parsed_in);
	parse_pattern(replace, parsed_out);
	///insert spaces
	for(int i=0;i< parsed_in.Count();i++) {
		if(parsed_in[i]->state() == 1) {
			if (i > 0 && parsed_in[i - 1]->state() != 2) {
				parsed_in.Insert(i, new spaces);
				i++;
			}
			if (i < parsed_in.Count() - 1 && parsed_in[i + 1]->state() != 2) {
				parsed_in.Insert(i + 1, new spaces);
				i++;
			}
		}
	}
}

std::tuple<int, int> SmartReplacer::find_occurence(const char* text, int start) {
	int s = 0;
	const int L = (int)strlen(text);
	BaseToken* first = nullptr;
	for (; s < parsed_in.Count(); s++) {
		if (parsed_in[s]->isExact) {
			first = parsed_in[s];
			break;
		}
	}
	if (first) {
		for (auto k = start; k < L; k++) {
			int m = first->matches(text, k);
			if (m != -1) {
				auto p = s + 1;
				for (; p < parsed_in.Count(); p++) {
					int nx = parsed_in[p]->matches(text, m);
					if(nx==-1) {
						break;
					}else {
						m = nx;
					}
				}
				if(p== parsed_in.Count()) {
					return std::tuple<int, int>(k,m);
				}
			}
		}
	}
	return std::tuple<int, int>(-1, -1);;
}

int SmartReplacer::replace_with_current_context(cStr& text, int start, int end) {
	cStr newtext;
	for (int i = 0; i < parsed_out.Count(); i++) {
		parsed_out[i]->autofill(parsed_in);
		newtext += parsed_out[i]->tokenValue;
	}
	cStr initial(text, start, end - start);
	text.Remove(start, end - start);
	text.Insert(start, newtext);
	printlog("%s:\n%s ===>\n%s\n\n", curfile.ToCharPtr(), initial.ToCharPtr(), newtext.ToCharPtr());
	if (Verbose == 2) {
		printf("%s ===>\n%s\n\n", initial.ToCharPtr(), newtext.ToCharPtr());
	}
	if (Verbose == 1) {
		printf("Found: %s\n", initial.ToCharPtr());
	}
	return start + newtext.Length();
}

int SmartReplacer::process(cStr& text) {
	int nchanges = 0;
	int start = 0;
	do {
		std::tuple<int, int> end = find_occurence(text, start);
		if (std::get<0>(end) != -1) {
			start = replace_with_current_context(text, std::get<0>(end), std::get<1>(end));
			nchanges++;
		}
		else break;
	} while (true);
	return nchanges;
}
