// Lexilla lexer library
/** @file Lexilla.cxx
 ** Lexer infrastructure.
 ** Provides entry points to shared library.
 **/
// Copyright 2019 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstring>

#include <vector>
#include <initializer_list>

#if defined(_WIN32)
#define EXPORT_FUNCTION __declspec(dllexport)
#define CALLING_CONVENTION __stdcall
#else
#define EXPORT_FUNCTION __attribute__((visibility("default")))
#define CALLING_CONVENTION
#endif

#include "ILexer.h"

#include "LexerModule.h"
#include "CatalogueModules.h"

using namespace Lexilla;

//++Autogenerated -- run lexilla/scripts/LexillaGen.py to regenerate
//**\(extern LexerModule \*;\n\)
extern LexerModule lmAHK;
extern LexerModule lmAs;
extern LexerModule lmAsm;
extern LexerModule lmAU3;
extern LexerModule lmAVS;
extern LexerModule lmBash;
extern LexerModule lmBatch;
extern LexerModule lmCmake;
extern LexerModule lmCoffeeScript;
extern LexerModule lmConf;
extern LexerModule lmCPP;
extern LexerModule lmCPPNoCase;
extern LexerModule lmCss;
extern LexerModule lmCSV;
extern LexerModule lmD;
extern LexerModule lmDart;
extern LexerModule lmDiff;
extern LexerModule lmF77;
extern LexerModule lmFortran;
extern LexerModule lmHTML;
extern LexerModule lmInno;
extern LexerModule lmJSON;
extern LexerModule lmJulia;
extern LexerModule lmKix;
extern LexerModule lmKotlin;
extern LexerModule lmLatex;
extern LexerModule lmLua;
extern LexerModule lmMake;
extern LexerModule lmMarkdown;
extern LexerModule lmMatlab;
extern LexerModule lmNim;
extern LexerModule lmNsis;
extern LexerModule lmNull;
extern LexerModule lmOctave;
extern LexerModule lmPascal;
extern LexerModule lmPerl;
extern LexerModule lmPHPSCRIPT;
extern LexerModule lmPowerShell;
extern LexerModule lmProps;
extern LexerModule lmPython;
extern LexerModule lmR;
extern LexerModule lmRegistry;
extern LexerModule lmRuby;
extern LexerModule lmRust;
extern LexerModule lmSQL;
extern LexerModule lmSysVerilog;
extern LexerModule lmTCL;
extern LexerModule lmTOML;
extern LexerModule lmVB;
extern LexerModule lmVBScript;
extern LexerModule lmVerilog;
extern LexerModule lmVHDL;
extern LexerModule lmXML;
extern LexerModule lmYAML;

//--Autogenerated -- end of automatically generated section

namespace {

CatalogueModules catalogueLexilla;

void AddEachLexer() {

	if (catalogueLexilla.Count() > 0) {
		return;
	}

	catalogueLexilla.AddLexerModules({
//++Autogenerated -- run scripts/LexillaGen.py to regenerate
//**\(\t\t&\*,\n\)
		&lmAHK,
		&lmAs,
		&lmAsm,
		&lmAU3,
		&lmAVS,
		&lmBash,
		&lmBatch,
		&lmCmake,
		&lmCoffeeScript,
		&lmConf,
		&lmCPP,
		&lmCPPNoCase,
		&lmCss,
		&lmCSV,
		&lmD,
		&lmDart,
		&lmDiff,
		&lmF77,
		&lmFortran,
		&lmHTML,
		&lmInno,
		&lmJSON,
		&lmJulia,
		&lmKix,
		&lmKotlin,
		&lmLatex,
		&lmLua,
		&lmMake,
		&lmMarkdown,
		&lmMatlab,
		&lmNim,
		&lmNsis,
		&lmNull,
		&lmOctave,
		&lmPascal,
		&lmPerl,
		&lmPHPSCRIPT,
		&lmPowerShell,
		&lmProps,
		&lmPython,
		&lmR,
		&lmRegistry,
		&lmRuby,
		&lmRust,
		&lmSQL,
		&lmSysVerilog,
		&lmTCL,
		&lmTOML,
		&lmVB,
		&lmVBScript,
		&lmVerilog,
		&lmVHDL,
		&lmXML,
		&lmYAML,

//--Autogenerated -- end of automatically generated section
	});

} // AddEachLexer()

} // namspace

extern "C" {

EXPORT_FUNCTION int CALLING_CONVENTION GetLexerCount() {
	AddEachLexer();
	return static_cast<int>(catalogueLexilla.Count());
}

EXPORT_FUNCTION void CALLING_CONVENTION GetLexerName(unsigned int index, char *name, int buflength) {
	AddEachLexer();
	*name = 0;
	const char *lexerName = catalogueLexilla.Name(index);
	if (static_cast<size_t>(buflength) > strlen(lexerName)) {
		strcpy(name, lexerName);
	}
}

EXPORT_FUNCTION LexerFactoryFunction CALLING_CONVENTION GetLexerFactory(unsigned int index) {
	AddEachLexer();
	return catalogueLexilla.Factory(index);
}

EXPORT_FUNCTION Scintilla::ILexer5 * CALLING_CONVENTION CreateLexer(const char *name) {
	AddEachLexer();
	for (size_t i = 0; i < catalogueLexilla.Count(); i++) {
		const char *lexerName = catalogueLexilla.Name(i);
		if (0 == strcmp(lexerName, name)) {
			return catalogueLexilla.Create(i);
		}
	}
	return nullptr;
}

EXPORT_FUNCTION const char * CALLING_CONVENTION LexerNameFromID(int identifier) {
	AddEachLexer();
	const LexerModule *pModule = catalogueLexilla.Find(identifier);
	if (pModule) {
		return pModule->languageName;
	}
	return nullptr;
}

EXPORT_FUNCTION const char * CALLING_CONVENTION GetLibraryPropertyNames() {
	return "";
}

EXPORT_FUNCTION void CALLING_CONVENTION SetLibraryProperty(const char *, const char *) {
	// Null implementation
}

EXPORT_FUNCTION const char * CALLING_CONVENTION GetNameSpace() {
	return "lexilla";
}

}

// Not exported from binary as LexerModule must be built exactly the same as
// modules listed above
void AddStaticLexerModule(LexerModule *plm) {
	AddEachLexer();
	catalogueLexilla.AddLexerModule(plm);
}
