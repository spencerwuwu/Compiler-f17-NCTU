/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMICOLON = 258,
    MOD = 259,
    KWBEGIN = 260,
    KWEND = 261,
    IDENT = 262,
    KWARRAY = 263,
    IDENTEQUAL = 264,
    LS = 265,
    LE = 266,
    SE = 267,
    NOT = 268,
    AND = 269,
    OR = 270,
    KWBOOLEAN = 271,
    KWDEF = 272,
    KWDO = 273,
    KWELSE = 274,
    KWFALSE = 275,
    KWFOR = 276,
    KWINTEGER = 277,
    KWIF = 278,
    KWOF = 279,
    KWPRINT = 280,
    KWREAD = 281,
    KWREAL = 282,
    KWSTRING = 283,
    KWTHEN = 284,
    KWTO = 285,
    KWTRUE = 286,
    KWRETURN = 287,
    KWVAR = 288,
    KWWHILE = 289,
    INTEGER = 290,
    OCTAL = 291,
    FLOAT = 292,
    SCIENTIFIC = 293,
    STRING = 294
  };
#endif
/* Tokens.  */
#define SEMICOLON 258
#define MOD 259
#define KWBEGIN 260
#define KWEND 261
#define IDENT 262
#define KWARRAY 263
#define IDENTEQUAL 264
#define LS 265
#define LE 266
#define SE 267
#define NOT 268
#define AND 269
#define OR 270
#define KWBOOLEAN 271
#define KWDEF 272
#define KWDO 273
#define KWELSE 274
#define KWFALSE 275
#define KWFOR 276
#define KWINTEGER 277
#define KWIF 278
#define KWOF 279
#define KWPRINT 280
#define KWREAD 281
#define KWREAL 282
#define KWSTRING 283
#define KWTHEN 284
#define KWTO 285
#define KWTRUE 286
#define KWRETURN 287
#define KWVAR 288
#define KWWHILE 289
#define INTEGER 290
#define OCTAL 291
#define FLOAT 292
#define SCIENTIFIC 293
#define STRING 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
