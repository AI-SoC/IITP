// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "verilog_parser.tab.hh"


// Unqualified %code blocks.
#line 32 "verilog_parser.yy"

  #include <iostream>
  #include <cstdlib>
  #include <fstream>
  #include <utility>
  #include <tuple>
  
   /* include for all driver functions */
  #include "verilog_driver.hpp"

#undef yylex
#define yylex scanner.yylex

#line 59 "verilog_parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 5 "verilog_parser.yy"
namespace verilog {
#line 151 "verilog_parser.tab.cc"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  VerilogParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  VerilogParser::VerilogParser (VerilogScanner &scanner_yyarg, ParserVerilogInterface *driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  VerilogParser::~VerilogParser ()
  {}

  VerilogParser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  VerilogParser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 84: // net_by_name
        value.move< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (std::move (that.value));
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.move< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (std::move (that.value));
        break;

      case 58: // port_type
        value.move< std::pair<verilog::PortDirection, verilog::ConnectionType> > (std::move (that.value));
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.move< std::string > (std::move (that.value));
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.move< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (std::move (that.value));
        break;

      case 82: // nets_by_position
        value.move< std::vector<std::vector<verilog::NetConcat>> > (std::move (that.value));
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.move< std::vector<verilog::NetConcat> > (std::move (that.value));
        break;

      case 70: // assignment
        value.move< verilog::Assignment > (std::move (that.value));
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.move< verilog::Constant > (std::move (that.value));
        break;

      case 80: // instance
        value.move< verilog::Instance > (std::move (that.value));
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.move< verilog::Net > (std::move (that.value));
        break;

      case 65: // net_type
        value.move< verilog::NetType > (std::move (that.value));
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.move< verilog::Port > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  VerilogParser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 84: // net_by_name
        value.copy< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.copy< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (YY_MOVE (that.value));
        break;

      case 58: // port_type
        value.copy< std::pair<verilog::PortDirection, verilog::ConnectionType> > (YY_MOVE (that.value));
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.copy< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (YY_MOVE (that.value));
        break;

      case 82: // nets_by_position
        value.copy< std::vector<std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.copy< std::vector<verilog::NetConcat> > (YY_MOVE (that.value));
        break;

      case 70: // assignment
        value.copy< verilog::Assignment > (YY_MOVE (that.value));
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.copy< verilog::Constant > (YY_MOVE (that.value));
        break;

      case 80: // instance
        value.copy< verilog::Instance > (YY_MOVE (that.value));
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.copy< verilog::Net > (YY_MOVE (that.value));
        break;

      case 65: // net_type
        value.copy< verilog::NetType > (YY_MOVE (that.value));
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.copy< verilog::Port > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  VerilogParser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  VerilogParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 84: // net_by_name
        value.move< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (YY_MOVE (s.value));
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.move< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (YY_MOVE (s.value));
        break;

      case 58: // port_type
        value.move< std::pair<verilog::PortDirection, verilog::ConnectionType> > (YY_MOVE (s.value));
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.move< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (YY_MOVE (s.value));
        break;

      case 82: // nets_by_position
        value.move< std::vector<std::vector<verilog::NetConcat>> > (YY_MOVE (s.value));
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.move< std::vector<verilog::NetConcat> > (YY_MOVE (s.value));
        break;

      case 70: // assignment
        value.move< verilog::Assignment > (YY_MOVE (s.value));
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.move< verilog::Constant > (YY_MOVE (s.value));
        break;

      case 80: // instance
        value.move< verilog::Instance > (YY_MOVE (s.value));
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.move< verilog::Net > (YY_MOVE (s.value));
        break;

      case 65: // net_type
        value.move< verilog::NetType > (YY_MOVE (s.value));
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.move< verilog::Port > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  VerilogParser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  VerilogParser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  VerilogParser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  VerilogParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  VerilogParser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  VerilogParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  VerilogParser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  VerilogParser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  VerilogParser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  VerilogParser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  VerilogParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  VerilogParser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  VerilogParser::symbol_number_type
  VerilogParser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[+state];
  }

  VerilogParser::stack_symbol_type::stack_symbol_type ()
  {}

  VerilogParser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 84: // net_by_name
        value.YY_MOVE_OR_COPY< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.YY_MOVE_OR_COPY< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (YY_MOVE (that.value));
        break;

      case 58: // port_type
        value.YY_MOVE_OR_COPY< std::pair<verilog::PortDirection, verilog::ConnectionType> > (YY_MOVE (that.value));
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.YY_MOVE_OR_COPY< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (YY_MOVE (that.value));
        break;

      case 82: // nets_by_position
        value.YY_MOVE_OR_COPY< std::vector<std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.YY_MOVE_OR_COPY< std::vector<verilog::NetConcat> > (YY_MOVE (that.value));
        break;

      case 70: // assignment
        value.YY_MOVE_OR_COPY< verilog::Assignment > (YY_MOVE (that.value));
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.YY_MOVE_OR_COPY< verilog::Constant > (YY_MOVE (that.value));
        break;

      case 80: // instance
        value.YY_MOVE_OR_COPY< verilog::Instance > (YY_MOVE (that.value));
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.YY_MOVE_OR_COPY< verilog::Net > (YY_MOVE (that.value));
        break;

      case 65: // net_type
        value.YY_MOVE_OR_COPY< verilog::NetType > (YY_MOVE (that.value));
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.YY_MOVE_OR_COPY< verilog::Port > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  VerilogParser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 84: // net_by_name
        value.move< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.move< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (YY_MOVE (that.value));
        break;

      case 58: // port_type
        value.move< std::pair<verilog::PortDirection, verilog::ConnectionType> > (YY_MOVE (that.value));
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.move< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (YY_MOVE (that.value));
        break;

      case 82: // nets_by_position
        value.move< std::vector<std::vector<verilog::NetConcat>> > (YY_MOVE (that.value));
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.move< std::vector<verilog::NetConcat> > (YY_MOVE (that.value));
        break;

      case 70: // assignment
        value.move< verilog::Assignment > (YY_MOVE (that.value));
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.move< verilog::Constant > (YY_MOVE (that.value));
        break;

      case 80: // instance
        value.move< verilog::Instance > (YY_MOVE (that.value));
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.move< verilog::Net > (YY_MOVE (that.value));
        break;

      case 65: // net_type
        value.move< verilog::NetType > (YY_MOVE (that.value));
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.move< verilog::Port > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  VerilogParser::stack_symbol_type&
  VerilogParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 84: // net_by_name
        value.copy< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (that.value);
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.copy< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (that.value);
        break;

      case 58: // port_type
        value.copy< std::pair<verilog::PortDirection, verilog::ConnectionType> > (that.value);
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.copy< std::string > (that.value);
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.copy< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (that.value);
        break;

      case 82: // nets_by_position
        value.copy< std::vector<std::vector<verilog::NetConcat>> > (that.value);
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.copy< std::vector<verilog::NetConcat> > (that.value);
        break;

      case 70: // assignment
        value.copy< verilog::Assignment > (that.value);
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.copy< verilog::Constant > (that.value);
        break;

      case 80: // instance
        value.copy< verilog::Instance > (that.value);
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.copy< verilog::Net > (that.value);
        break;

      case 65: // net_type
        value.copy< verilog::NetType > (that.value);
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.copy< verilog::Port > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  VerilogParser::stack_symbol_type&
  VerilogParser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 84: // net_by_name
        value.move< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > (that.value);
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        value.move< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > (that.value);
        break;

      case 58: // port_type
        value.move< std::pair<verilog::PortDirection, verilog::ConnectionType> > (that.value);
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        value.move< std::string > (that.value);
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        value.move< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (that.value);
        break;

      case 82: // nets_by_position
        value.move< std::vector<std::vector<verilog::NetConcat>> > (that.value);
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        value.move< std::vector<verilog::NetConcat> > (that.value);
        break;

      case 70: // assignment
        value.move< verilog::Assignment > (that.value);
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        value.move< verilog::Constant > (that.value);
        break;

      case 80: // instance
        value.move< verilog::Instance > (that.value);
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        value.move< verilog::Net > (that.value);
        break;

      case 65: // net_type
        value.move< verilog::NetType > (that.value);
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        value.move< verilog::Port > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  VerilogParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  VerilogParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  VerilogParser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  VerilogParser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  VerilogParser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  VerilogParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  VerilogParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  VerilogParser::debug_level_type
  VerilogParser::debug_level () const
  {
    return yydebug_;
  }

  void
  VerilogParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  VerilogParser::state_type
  VerilogParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  VerilogParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  VerilogParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  VerilogParser::operator() ()
  {
    return parse ();
  }

  int
  VerilogParser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 84: // net_by_name
        yylhs.value.emplace< std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ();
        break;

      case 81: // inst_pins
      case 83: // nets_by_name
        yylhs.value.emplace< std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ();
        break;

      case 58: // port_type
        yylhs.value.emplace< std::pair<verilog::PortDirection, verilog::ConnectionType> > ();
        break;

      case 10: // NAME
      case 11: // ESCAPED_NAME
      case 48: // valid_name
        yylhs.value.emplace< std::string > ();
        break;

      case 71: // lhs
      case 72: // lhs_concat
      case 73: // lhs_exprs
      case 74: // lhs_expr
        yylhs.value.emplace< std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ();
        break;

      case 82: // nets_by_position
        yylhs.value.emplace< std::vector<std::vector<verilog::NetConcat>> > ();
        break;

      case 76: // rhs
      case 77: // rhs_concat
      case 78: // rhs_exprs
      case 79: // rhs_expr
        yylhs.value.emplace< std::vector<verilog::NetConcat> > ();
        break;

      case 70: // assignment
        yylhs.value.emplace< verilog::Assignment > ();
        break;

      case 12: // INTEGER
      case 13: // BINARY
      case 14: // OCTAL
      case 15: // DECIMAL
      case 16: // HEX
      case 17: // REAL
      case 18: // EXP
      case 75: // constant
        yylhs.value.emplace< verilog::Constant > ();
        break;

      case 80: // instance
        yylhs.value.emplace< verilog::Instance > ();
        break;

      case 66: // net_decl_statements
      case 67: // net_decl
        yylhs.value.emplace< verilog::Net > ();
        break;

      case 65: // net_type
        yylhs.value.emplace< verilog::NetType > ();
        break;

      case 59: // port_declarations
      case 60: // port_decl
      case 64: // port_decl_statements
        yylhs.value.emplace< verilog::Port > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 95 "verilog_parser.yy"
         { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1239 "verilog_parser.tab.cc"
    break;

  case 3:
#line 96 "verilog_parser.yy"
                 { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); }
#line 1245 "verilog_parser.tab.cc"
    break;

  case 7:
#line 109 "verilog_parser.yy"
    { 
      driver->add_module(std::move(yystack_[1].value.as < std::string > ()));
    }
#line 1253 "verilog_parser.tab.cc"
    break;

  case 9:
#line 114 "verilog_parser.yy"
    {
      driver->add_module(std::move(yystack_[3].value.as < std::string > ()));
    }
#line 1261 "verilog_parser.tab.cc"
    break;

  case 11:
#line 119 "verilog_parser.yy"
    {
      driver->add_module(std::move(yystack_[4].value.as < std::string > ()));
    }
#line 1269 "verilog_parser.tab.cc"
    break;

  case 13:
#line 124 "verilog_parser.yy"
    { 
      driver->add_module(std::move(yystack_[1].value.as < std::string > ())); 
    }
#line 1277 "verilog_parser.tab.cc"
    break;

  case 14:
#line 128 "verilog_parser.yy"
    { 
      driver->add_port(std::move(yystack_[1].value.as < verilog::Port > ())); 
    }
#line 1285 "verilog_parser.tab.cc"
    break;

  case 16:
#line 136 "verilog_parser.yy"
               { }
#line 1291 "verilog_parser.tab.cc"
    break;

  case 17:
#line 137 "verilog_parser.yy"
                               { }
#line 1297 "verilog_parser.tab.cc"
    break;

  case 18:
#line 142 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::INPUT, verilog::ConnectionType::NONE); }
#line 1303 "verilog_parser.tab.cc"
    break;

  case 19:
#line 143 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::INPUT, verilog::ConnectionType::WIRE); }
#line 1309 "verilog_parser.tab.cc"
    break;

  case 20:
#line 144 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::OUTPUT,verilog::ConnectionType::NONE); }
#line 1315 "verilog_parser.tab.cc"
    break;

  case 21:
#line 145 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::OUTPUT,verilog::ConnectionType::REG);  }
#line 1321 "verilog_parser.tab.cc"
    break;

  case 22:
#line 146 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::INOUT, verilog::ConnectionType::NONE); }
#line 1327 "verilog_parser.tab.cc"
    break;

  case 23:
#line 147 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::INOUT, verilog::ConnectionType::WIRE); }
#line 1333 "verilog_parser.tab.cc"
    break;

  case 24:
#line 148 "verilog_parser.yy"
               { yylhs.value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > () = std::make_pair(verilog::PortDirection::INOUT, verilog::ConnectionType::REG);  }
#line 1339 "verilog_parser.tab.cc"
    break;

  case 25:
#line 154 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Port > () = yystack_[0].value.as < verilog::Port > ();
    }
#line 1347 "verilog_parser.tab.cc"
    break;

  case 26:
#line 158 "verilog_parser.yy"
    {
      driver->add_port(std::move(yystack_[2].value.as < verilog::Port > ()));
      yylhs.value.as < verilog::Port > () = yystack_[0].value.as < verilog::Port > ();
    }
#line 1356 "verilog_parser.tab.cc"
    break;

  case 27:
#line 163 "verilog_parser.yy"
    {
      yystack_[2].value.as < verilog::Port > ().names.emplace_back(std::move(yystack_[0].value.as < std::string > ()));    
      yylhs.value.as < verilog::Port > () = yystack_[2].value.as < verilog::Port > ();
    }
#line 1365 "verilog_parser.tab.cc"
    break;

  case 28:
#line 171 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Port > ().dir  = std::get<0>(yystack_[1].value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > ());
      yylhs.value.as < verilog::Port > ().type = std::get<1>(yystack_[1].value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > ());
      yylhs.value.as < verilog::Port > ().names.emplace_back(std::move(yystack_[0].value.as < std::string > ())); 
    }
#line 1375 "verilog_parser.tab.cc"
    break;

  case 29:
#line 177 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Port > ().dir  = std::get<0>(yystack_[6].value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > ());
      yylhs.value.as < verilog::Port > ().type = std::get<1>(yystack_[6].value.as < std::pair<verilog::PortDirection, verilog::ConnectionType> > ());
      yylhs.value.as < verilog::Port > ().beg = std::stoi(yystack_[4].value.as < verilog::Constant > ().value);
      yylhs.value.as < verilog::Port > ().end = std::stoi(yystack_[2].value.as < verilog::Constant > ().value);
      yylhs.value.as < verilog::Port > ().names.push_back(std::move(yystack_[0].value.as < std::string > ())); 
    }
#line 1387 "verilog_parser.tab.cc"
    break;

  case 35:
#line 199 "verilog_parser.yy"
                             { driver->add_port(std::move(yystack_[1].value.as < verilog::Port > ())); }
#line 1393 "verilog_parser.tab.cc"
    break;

  case 36:
#line 200 "verilog_parser.yy"
                             { driver->add_net(std::move(yystack_[1].value.as < verilog::Net > ())); }
#line 1399 "verilog_parser.tab.cc"
    break;

  case 37:
#line 206 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Port > () = yystack_[0].value.as < verilog::Port > ();
    }
#line 1407 "verilog_parser.tab.cc"
    break;

  case 38:
#line 210 "verilog_parser.yy"
    {
      yystack_[2].value.as < verilog::Port > ().names.emplace_back(std::move(yystack_[0].value.as < std::string > ()));    
      yylhs.value.as < verilog::Port > () = yystack_[2].value.as < verilog::Port > ();
    }
#line 1416 "verilog_parser.tab.cc"
    break;

  case 39:
#line 218 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::WIRE;    }
#line 1422 "verilog_parser.tab.cc"
    break;

  case 40:
#line 219 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::WAND;    }
#line 1428 "verilog_parser.tab.cc"
    break;

  case 41:
#line 220 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::WOR;     }
#line 1434 "verilog_parser.tab.cc"
    break;

  case 42:
#line 221 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::TRI;     }
#line 1440 "verilog_parser.tab.cc"
    break;

  case 43:
#line 222 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::TRIOR;   }
#line 1446 "verilog_parser.tab.cc"
    break;

  case 44:
#line 223 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::TRIAND;  }
#line 1452 "verilog_parser.tab.cc"
    break;

  case 45:
#line 224 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::SUPPLY0; }
#line 1458 "verilog_parser.tab.cc"
    break;

  case 46:
#line 225 "verilog_parser.yy"
             { yylhs.value.as < verilog::NetType > () = verilog::NetType::SUPPLY1; }
#line 1464 "verilog_parser.tab.cc"
    break;

  case 47:
#line 230 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Net > () = yystack_[0].value.as < verilog::Net > ();
    }
#line 1472 "verilog_parser.tab.cc"
    break;

  case 48:
#line 234 "verilog_parser.yy"
    {
      yystack_[2].value.as < verilog::Net > ().names.push_back(std::move(yystack_[0].value.as < std::string > ()));
      yylhs.value.as < verilog::Net > () = yystack_[2].value.as < verilog::Net > ();
    }
#line 1481 "verilog_parser.tab.cc"
    break;

  case 49:
#line 242 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Net > ().type = yystack_[1].value.as < verilog::NetType > ();
      yylhs.value.as < verilog::Net > ().names.push_back(std::move(yystack_[0].value.as < std::string > ())); 
    }
#line 1490 "verilog_parser.tab.cc"
    break;

  case 50:
#line 247 "verilog_parser.yy"
    {
      yylhs.value.as < verilog::Net > ().type = yystack_[6].value.as < verilog::NetType > ();
      yylhs.value.as < verilog::Net > ().beg = std::stoi(yystack_[4].value.as < verilog::Constant > ().value);
      yylhs.value.as < verilog::Net > ().end = std::stoi(yystack_[2].value.as < verilog::Constant > ().value);
      yylhs.value.as < verilog::Net > ().names.push_back(std::move(yystack_[0].value.as < std::string > ())); 
    }
#line 1501 "verilog_parser.tab.cc"
    break;

  case 54:
#line 265 "verilog_parser.yy"
                { yylhs.value.as < verilog::Assignment > ().lhs = yystack_[2].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (); yylhs.value.as < verilog::Assignment > ().rhs = yystack_[0].value.as < std::vector<verilog::NetConcat> > (); driver->add_assignment(std::move(yylhs.value.as < verilog::Assignment > ())); }
#line 1507 "verilog_parser.tab.cc"
    break;

  case 55:
#line 271 "verilog_parser.yy"
               { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().push_back(std::move(yystack_[0].value.as < std::string > ())); }
#line 1513 "verilog_parser.tab.cc"
    break;

  case 56:
#line 273 "verilog_parser.yy"
    { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().emplace_back(verilog::NetBit(std::move(yystack_[3].value.as < std::string > ()), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1519 "verilog_parser.tab.cc"
    break;

  case 57:
#line 275 "verilog_parser.yy"
    { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().emplace_back(verilog::NetRange(std::move(yystack_[5].value.as < std::string > ()), std::stoi(yystack_[3].value.as < verilog::Constant > ().value), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1525 "verilog_parser.tab.cc"
    break;

  case 58:
#line 276 "verilog_parser.yy"
               { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > () = yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > (); }
#line 1531 "verilog_parser.tab.cc"
    break;

  case 59:
#line 280 "verilog_parser.yy"
                      { std::move(yystack_[1].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().begin(), yystack_[1].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ())); }
#line 1537 "verilog_parser.tab.cc"
    break;

  case 60:
#line 284 "verilog_parser.yy"
             { std::move(yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().begin(), yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ())); }
#line 1543 "verilog_parser.tab.cc"
    break;

  case 61:
#line 286 "verilog_parser.yy"
    { 
      std::move(yystack_[2].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().begin(), yystack_[2].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ()));
      std::move(yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().begin(), yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ()));
    }
#line 1552 "verilog_parser.tab.cc"
    break;

  case 62:
#line 293 "verilog_parser.yy"
               { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().push_back(std::move(yystack_[0].value.as < std::string > ())); }
#line 1558 "verilog_parser.tab.cc"
    break;

  case 63:
#line 295 "verilog_parser.yy"
    { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().emplace_back(verilog::NetBit(std::move(yystack_[3].value.as < std::string > ()), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1564 "verilog_parser.tab.cc"
    break;

  case 64:
#line 297 "verilog_parser.yy"
    { yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().emplace_back(verilog::NetRange(std::move(yystack_[5].value.as < std::string > ()), std::stoi(yystack_[3].value.as < verilog::Constant > ().value), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1570 "verilog_parser.tab.cc"
    break;

  case 65:
#line 299 "verilog_parser.yy"
    { std::move(yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().begin(), yystack_[0].value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::variant<std::string, verilog::NetBit, verilog::NetRange>> > ())); }
#line 1576 "verilog_parser.tab.cc"
    break;

  case 66:
#line 305 "verilog_parser.yy"
             { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1582 "verilog_parser.tab.cc"
    break;

  case 67:
#line 306 "verilog_parser.yy"
           { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1588 "verilog_parser.tab.cc"
    break;

  case 68:
#line 307 "verilog_parser.yy"
          { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1594 "verilog_parser.tab.cc"
    break;

  case 69:
#line 308 "verilog_parser.yy"
            { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1600 "verilog_parser.tab.cc"
    break;

  case 70:
#line 309 "verilog_parser.yy"
        { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1606 "verilog_parser.tab.cc"
    break;

  case 71:
#line 310 "verilog_parser.yy"
         { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1612 "verilog_parser.tab.cc"
    break;

  case 72:
#line 311 "verilog_parser.yy"
         { yylhs.value.as < verilog::Constant > ()=yystack_[0].value.as < verilog::Constant > (); }
#line 1618 "verilog_parser.tab.cc"
    break;

  case 73:
#line 315 "verilog_parser.yy"
               { yylhs.value.as < std::vector<verilog::NetConcat> > ().emplace_back(yystack_[0].value.as < std::string > ()); }
#line 1624 "verilog_parser.tab.cc"
    break;

  case 74:
#line 317 "verilog_parser.yy"
    { yylhs.value.as < std::vector<verilog::NetConcat> > ().emplace_back(verilog::NetBit(std::move(yystack_[3].value.as < std::string > ()), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1630 "verilog_parser.tab.cc"
    break;

  case 75:
#line 319 "verilog_parser.yy"
    { yylhs.value.as < std::vector<verilog::NetConcat> > ().emplace_back(verilog::NetRange(std::move(yystack_[5].value.as < std::string > ()), std::stoi(yystack_[3].value.as < verilog::Constant > ().value), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1636 "verilog_parser.tab.cc"
    break;

  case 76:
#line 320 "verilog_parser.yy"
             { yylhs.value.as < std::vector<verilog::NetConcat> > ().push_back(std::move(yystack_[0].value.as < verilog::Constant > ())); }
#line 1642 "verilog_parser.tab.cc"
    break;

  case 77:
#line 321 "verilog_parser.yy"
               { yylhs.value.as < std::vector<verilog::NetConcat> > () = yystack_[0].value.as < std::vector<verilog::NetConcat> > (); }
#line 1648 "verilog_parser.tab.cc"
    break;

  case 78:
#line 325 "verilog_parser.yy"
                      { std::move(yystack_[1].value.as < std::vector<verilog::NetConcat> > ().begin(), yystack_[1].value.as < std::vector<verilog::NetConcat> > ().end(), std::back_inserter(yylhs.value.as < std::vector<verilog::NetConcat> > ())); }
#line 1654 "verilog_parser.tab.cc"
    break;

  case 79:
#line 329 "verilog_parser.yy"
             { std::move(yystack_[0].value.as < std::vector<verilog::NetConcat> > ().begin(), yystack_[0].value.as < std::vector<verilog::NetConcat> > ().end(), std::back_inserter(yylhs.value.as < std::vector<verilog::NetConcat> > ())); }
#line 1660 "verilog_parser.tab.cc"
    break;

  case 80:
#line 331 "verilog_parser.yy"
    { 
      std::move(yystack_[2].value.as < std::vector<verilog::NetConcat> > ().begin(), yystack_[2].value.as < std::vector<verilog::NetConcat> > ().end(), std::back_inserter(yylhs.value.as < std::vector<verilog::NetConcat> > ()));
      std::move(yystack_[0].value.as < std::vector<verilog::NetConcat> > ().begin(), yystack_[0].value.as < std::vector<verilog::NetConcat> > ().end(), std::back_inserter(yylhs.value.as < std::vector<verilog::NetConcat> > ()));
    }
#line 1669 "verilog_parser.tab.cc"
    break;

  case 81:
#line 338 "verilog_parser.yy"
               { yylhs.value.as < std::vector<verilog::NetConcat> > ().push_back(std::move(yystack_[0].value.as < std::string > ())); }
#line 1675 "verilog_parser.tab.cc"
    break;

  case 82:
#line 340 "verilog_parser.yy"
    { yylhs.value.as < std::vector<verilog::NetConcat> > ().emplace_back(verilog::NetBit(std::move(yystack_[3].value.as < std::string > ()), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1681 "verilog_parser.tab.cc"
    break;

  case 83:
#line 342 "verilog_parser.yy"
    { yylhs.value.as < std::vector<verilog::NetConcat> > ().emplace_back(verilog::NetRange(std::move(yystack_[5].value.as < std::string > ()), std::stoi(yystack_[3].value.as < verilog::Constant > ().value), std::stoi(yystack_[1].value.as < verilog::Constant > ().value))); }
#line 1687 "verilog_parser.tab.cc"
    break;

  case 84:
#line 343 "verilog_parser.yy"
             { yylhs.value.as < std::vector<verilog::NetConcat> > ().push_back(std::move(yystack_[0].value.as < verilog::Constant > ())); }
#line 1693 "verilog_parser.tab.cc"
    break;

  case 85:
#line 345 "verilog_parser.yy"
    { std::move(yystack_[0].value.as < std::vector<verilog::NetConcat> > ().begin(), yystack_[0].value.as < std::vector<verilog::NetConcat> > ().end(), std::back_inserter(yylhs.value.as < std::vector<verilog::NetConcat> > ())); }
#line 1699 "verilog_parser.tab.cc"
    break;

  case 86:
#line 354 "verilog_parser.yy"
    { 
      std::swap(yylhs.value.as < verilog::Instance > ().module_name, yystack_[5].value.as < std::string > ()); 
      std::swap(yylhs.value.as < verilog::Instance > ().inst_name, yystack_[4].value.as < std::string > ()); 
      std::swap(yylhs.value.as < verilog::Instance > ().pin_names, std::get<0>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));  
      std::swap(yylhs.value.as < verilog::Instance > ().net_names, std::get<1>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));  
      driver->add_instance(std::move(yylhs.value.as < verilog::Instance > ()));
    }
#line 1711 "verilog_parser.tab.cc"
    break;

  case 87:
#line 362 "verilog_parser.yy"
    { 
      std::swap(yylhs.value.as < verilog::Instance > ().module_name, yystack_[6].value.as < std::string > ()); 
      std::swap(yylhs.value.as < verilog::Instance > ().inst_name, yystack_[4].value.as < std::string > ()); 
      std::swap(yylhs.value.as < verilog::Instance > ().pin_names, std::get<0>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));  
      std::swap(yylhs.value.as < verilog::Instance > ().net_names, std::get<1>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));  
      driver->add_instance(std::move(yylhs.value.as < verilog::Instance > ()));
    }
#line 1723 "verilog_parser.tab.cc"
    break;

  case 88:
#line 372 "verilog_parser.yy"
    { }
#line 1729 "verilog_parser.tab.cc"
    break;

  case 89:
#line 373 "verilog_parser.yy"
                     { std::swap(std::get<1>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()), yystack_[0].value.as < std::vector<std::vector<verilog::NetConcat>> > ()); }
#line 1735 "verilog_parser.tab.cc"
    break;

  case 90:
#line 375 "verilog_parser.yy"
    {
      std::swap(std::get<0>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()), std::get<0>(yystack_[0].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));
      std::swap(std::get<1>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()), std::get<1>(yystack_[0].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()));
    }
#line 1744 "verilog_parser.tab.cc"
    break;

  case 91:
#line 382 "verilog_parser.yy"
        { yylhs.value.as < std::vector<std::vector<verilog::NetConcat>> > ().emplace_back(std::move(yystack_[0].value.as < std::vector<verilog::NetConcat> > ())); }
#line 1750 "verilog_parser.tab.cc"
    break;

  case 92:
#line 384 "verilog_parser.yy"
    { 
      std::move(yystack_[2].value.as < std::vector<std::vector<verilog::NetConcat>> > ().begin(), yystack_[2].value.as < std::vector<std::vector<verilog::NetConcat>> > ().end(), std::back_inserter(yylhs.value.as < std::vector<std::vector<verilog::NetConcat>> > ()));   
      yylhs.value.as < std::vector<std::vector<verilog::NetConcat>> > ().push_back(std::move(yystack_[0].value.as < std::vector<verilog::NetConcat> > ()));
    }
#line 1759 "verilog_parser.tab.cc"
    break;

  case 93:
#line 393 "verilog_parser.yy"
    { 
      std::get<0>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()).push_back(std::move(std::get<0>(yystack_[0].value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()))); 
      std::get<1>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()).push_back(std::move(std::get<1>(yystack_[0].value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()))); 
    }
#line 1768 "verilog_parser.tab.cc"
    break;

  case 94:
#line 398 "verilog_parser.yy"
    { 
      auto &pin_names = std::get<0>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ());
      auto &net_names = std::get<1>(yystack_[2].value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ());
      std::move(pin_names.begin(), pin_names.end(), std::back_inserter(std::get<0>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ())));
      std::move(net_names.begin(), net_names.end(), std::back_inserter(std::get<1>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ())));

      std::get<0>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()).push_back(std::move(std::get<0>(yystack_[0].value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()))); 
      std::get<1>(yylhs.value.as < std::pair<std::vector<std::variant<std::string, NetBit, NetRange>>, std::vector<std::vector<verilog::NetConcat>>> > ()).push_back(std::move(std::get<1>(yystack_[0].value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()))); 
    }
#line 1782 "verilog_parser.tab.cc"
    break;

  case 95:
#line 412 "verilog_parser.yy"
    { std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[2].value.as < std::string > (); }
#line 1788 "verilog_parser.tab.cc"
    break;

  case 96:
#line 414 "verilog_parser.yy"
    { 
       std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[3].value.as < std::string > (); 
       std::get<1>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()).push_back(std::move(yystack_[1].value.as < std::string > ())); 
    }
#line 1797 "verilog_parser.tab.cc"
    break;

  case 97:
#line 419 "verilog_parser.yy"
    { 
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[6].value.as < std::string > (); 
      std::get<1>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()).emplace_back(verilog::NetBit(std::move(yystack_[4].value.as < std::string > ()), std::stoi(yystack_[2].value.as < verilog::Constant > ().value))); 
    }
#line 1806 "verilog_parser.tab.cc"
    break;

  case 98:
#line 425 "verilog_parser.yy"
    { 
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[3].value.as < std::string > (); 
      std::get<1>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[1].value.as < std::vector<verilog::NetConcat> > (); 
    }
#line 1815 "verilog_parser.tab.cc"
    break;

  case 99:
#line 431 "verilog_parser.yy"
    {
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = verilog::NetBit(std::move(yystack_[5].value.as < std::string > ()), std::stoi(yystack_[3].value.as < verilog::Constant > ().value)); 
    }
#line 1823 "verilog_parser.tab.cc"
    break;

  case 100:
#line 435 "verilog_parser.yy"
    {
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = verilog::NetBit(std::move(yystack_[6].value.as < std::string > ()), std::stoi(yystack_[4].value.as < verilog::Constant > ().value)); 
      std::get<1>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[1].value.as < std::vector<verilog::NetConcat> > (); 
    }
#line 1832 "verilog_parser.tab.cc"
    break;

  case 101:
#line 441 "verilog_parser.yy"
    {
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = verilog::NetRange(std::move(yystack_[7].value.as < std::string > ()), std::stoi(yystack_[5].value.as < verilog::Constant > ().value), std::stoi(yystack_[3].value.as < verilog::Constant > ().value)); 
    }
#line 1840 "verilog_parser.tab.cc"
    break;

  case 102:
#line 445 "verilog_parser.yy"
    {
      std::get<0>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = verilog::NetRange(std::move(yystack_[8].value.as < std::string > ()), std::stoi(yystack_[6].value.as < verilog::Constant > ().value), std::stoi(yystack_[4].value.as < verilog::Constant > ().value)); 
      std::get<1>(yylhs.value.as < std::pair<std::variant<std::string, NetBit, NetRange>, std::vector<verilog::NetConcat>> > ()) = yystack_[1].value.as < std::vector<verilog::NetConcat> > (); 
    }
#line 1849 "verilog_parser.tab.cc"
    break;


#line 1853 "verilog_parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[+yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  VerilogParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  VerilogParser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

        int yyn = yypact_[+yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yy_error_token_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char VerilogParser::yypact_ninf_ = -108;

  const signed char VerilogParser::yytable_ninf_ = -1;

  const short
  VerilogParser::yypact_[] =
  {
    -108,    11,    45,  -108,     9,  -108,  -108,  -108,    43,  -108,
      16,  -108,    32,  -108,    19,    66,   266,  -108,    57,    90,
      75,    18,    69,  -108,    58,     9,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,    12,    -1,  -108,  -108,  -108,
      28,    38,    52,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,   103,  -108,  -108,    74,  -108,  -108,    12,    88,    53,
    -108,    94,  -108,   110,   111,     9,  -108,     9,   135,  -108,
    -108,     9,   290,   112,   115,  -108,  -108,  -108,   116,  -108,
     -19,  -108,   138,  -108,    12,   248,   126,   164,   118,  -108,
     117,  -108,  -108,   143,  -108,   314,   145,    12,  -108,    68,
    -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,   248,   121,
    -108,  -108,  -108,   126,   126,     9,  -108,  -108,    73,    54,
       9,  -108,   124,   125,   127,  -108,   164,   151,   128,   338,
    -108,    78,  -108,   153,  -108,   131,  -108,  -108,    -7,  -108,
     154,  -108,    10,  -108,  -108,   126,   126,   126,   126,   126,
      63,   136,   248,   148,   157,   155,     9,  -108,   159,  -108,
     156,   182,   248,  -108,    80,  -108,    54,   107,   107,  -108,
    -108,   173,   185,  -108,  -108,  -108,   165,     9,  -108,   158,
    -108,    84,  -108,   189,  -108,  -108,    -3,   166,    91,  -108,
    -108,  -108,   191,  -108,   170,  -108,   192,  -108,   193,   176,
     172,  -108,    93,   174,   206,  -108,   177,   199,  -108,   200,
    -108,   215,  -108,  -108,   201,  -108
  };

  const signed char
  VerilogParser::yydefact_[] =
  {
       5,     0,     4,     1,     0,     6,     2,     3,     0,     7,
      13,    30,     0,    16,     0,     0,     0,     9,    18,    20,
      22,     0,     0,    25,     0,     0,     8,    39,    40,    41,
      42,    43,    44,    45,    46,     0,     0,    37,    31,    33,
       0,     0,     0,    47,    32,    34,    30,    19,    21,    24,
      23,     0,    28,    14,     0,    11,    17,     0,    55,     0,
      52,     0,    58,     0,     0,     0,    35,     0,     0,    49,
      36,     0,     0,     0,     0,    27,    26,    30,    62,    65,
       0,    60,     0,    51,     0,     0,     0,    88,     0,    38,
       0,    48,    10,     0,    30,     0,     0,     0,    59,     0,
      53,    66,    67,    68,    69,    70,    71,    72,     0,    73,
      76,    54,    77,     0,     0,     0,   106,   108,     0,   104,
       0,    91,     0,    89,    90,    93,    88,     0,     0,     0,
      12,     0,    61,     0,    56,    81,    84,    85,     0,    79,
       0,   109,     0,   107,   103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    15,     0,    63,
       0,     0,     0,    78,     0,   114,   105,   111,   110,   112,
     113,     0,     0,    86,    92,    94,     0,     0,    29,     0,
      57,     0,    80,     0,    74,    95,     0,     0,     0,    87,
      50,    64,     0,    82,     0,    96,     0,    98,     0,     0,
       0,    75,     0,     0,     0,    83,     0,     0,    99,     0,
      97,     0,   100,   101,     0,   102
  };

  const short
  VerilogParser::yypgoto_[] =
  {
    -108,    -4,  -108,  -108,  -108,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,    -9,   -43,  -108,  -108,  -108,  -108,  -108,
    -108,  -108,  -108,   160,  -108,   -50,  -108,   141,   -83,   -77,
    -107,  -108,    77,  -108,   109,  -108,  -108,    87,  -108,  -108,
     -75
  };

  const short
  VerilogParser::yydefgoto_[] =
  {
      -1,   116,     1,     2,     5,    11,    46,    77,    14,    74,
      15,    21,    22,    37,    16,    38,    39,    40,    41,    42,
      43,    44,    59,    60,    61,    62,    80,    81,   117,   121,
     112,   138,   139,    45,   122,   123,   124,   125,    65,   118,
     119
  };

  const unsigned char
  VerilogParser::yytable_[] =
  {
       8,   137,   110,    72,   110,    23,    13,    79,   111,     6,
       7,     3,    36,   146,   147,   148,   149,    52,    97,     6,
       7,    56,     6,     7,    98,   136,     6,     7,     6,     7,
     162,    58,    64,   195,    95,   196,   163,    69,   141,   142,
      18,    19,    20,   110,    63,    76,   165,    79,     6,     7,
      75,   129,    12,    78,    57,   137,    51,   146,   147,   148,
     149,    88,    66,    89,     4,    67,    17,    91,    36,   110,
     166,   167,   168,   169,   170,   174,    68,     9,    10,   136,
      58,   109,    47,   109,     6,     7,    70,    83,   110,    71,
      84,    36,    55,    78,   187,    18,    19,    20,   171,    49,
      50,   172,    24,    25,   135,    53,    54,   133,   134,   144,
     145,   143,   148,   149,    48,    73,   150,   158,   159,   183,
     184,   110,   109,   192,   193,    36,    82,   209,   110,   113,
     198,   199,   183,   206,   214,    85,     6,     7,   101,   102,
     103,   104,   105,   106,   107,    86,    87,    90,   109,    94,
      99,    93,   178,   126,    96,   128,   127,   131,   135,   140,
     151,   114,   152,   155,   153,   160,   164,   186,   156,   161,
     173,   179,   115,   190,     6,     7,   101,   102,   103,   104,
     105,   106,   107,     6,     7,   101,   102,   103,   104,   105,
     106,   107,   120,   176,   181,   177,   180,   188,   191,   189,
     109,   194,   197,   200,   202,   203,   108,   109,   120,   185,
     201,   204,   205,   210,   207,   108,     6,     7,   101,   102,
     103,   104,   105,   106,   107,     6,     7,   101,   102,   103,
     104,   105,   106,   107,   211,   154,   212,   215,   132,   182,
     175,     0,   208,     0,   100,     0,     0,     0,   108,     0,
       0,   213,     0,     0,     0,     0,     0,   108,     6,     7,
     101,   102,   103,   104,   105,   106,   107,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     6,     7,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    18,    19,    20,
     108,    27,    28,    29,    30,    31,    32,    33,    34,    35,
       6,     7,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    18,    19,    20,     0,    27,    28,    29,    30,    31,
      32,    33,    34,    35,     6,     7,     0,     0,     0,     0,
       0,     0,     0,     0,   130,    18,    19,    20,     0,    27,
      28,    29,    30,    31,    32,    33,    34,    35,     6,     7,
       0,     0,     0,     0,     0,     0,     0,     0,   157,    18,
      19,    20,     0,    27,    28,    29,    30,    31,    32,    33,
      34,    35
  };

  const short
  VerilogParser::yycheck_[] =
  {
       4,   108,    85,    46,    87,    14,    10,    57,    85,    10,
      11,     0,    16,     3,     4,     5,     6,    21,    37,    10,
      11,    25,    10,    11,    43,   108,    10,    11,    10,    11,
      37,    35,    36,    36,    77,    38,    43,    41,   113,   114,
      21,    22,    23,   126,    45,    54,    36,    97,    10,    11,
      54,    94,    36,    57,    42,   162,    38,     3,     4,     5,
       6,    65,    34,    67,    19,    37,    34,    71,    72,   152,
     145,   146,   147,   148,   149,   152,    38,    34,    35,   162,
      84,    85,    25,    87,    10,    11,    34,    34,   171,    37,
      37,    95,    34,    97,   171,    21,    22,    23,    35,    24,
      25,    38,    36,    37,   108,    36,    37,    39,    40,    36,
      37,   115,     5,     6,    24,    12,   120,    39,    40,    39,
      40,   204,   126,    39,    40,   129,    38,   204,   211,     3,
      39,    40,    39,    40,   211,    41,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    35,    35,    12,   152,    34,
      12,    39,   156,    35,    38,    12,    39,    12,   162,    38,
      36,    35,    37,    12,    37,    12,    12,   171,    40,    38,
      34,    12,    46,   177,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    44,    36,    12,    40,    40,    12,    40,    34,
     204,    12,    36,    12,    12,    12,    42,   211,    44,    36,
      40,    35,    40,    36,    40,    42,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    35,   126,    36,    36,    97,   162,
     153,    -1,    36,    -1,    84,    -1,    -1,    -1,    42,    -1,
      -1,    36,    -1,    -1,    -1,    -1,    -1,    42,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    10,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    22,    23,
      42,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      10,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      20,    21,    22,    23,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    10,    11,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    10,    11,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    -1,    25,    26,    27,    28,    29,    30,    31,
      32,    33
  };

  const signed char
  VerilogParser::yystos_[] =
  {
       0,    49,    50,     0,    19,    51,    10,    11,    48,    34,
      35,    52,    36,    48,    55,    57,    61,    34,    21,    22,
      23,    58,    59,    60,    36,    37,    20,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    48,    60,    62,    63,
      64,    65,    66,    67,    68,    80,    53,    25,    24,    24,
      25,    38,    48,    36,    37,    34,    48,    42,    48,    69,
      70,    71,    72,    45,    48,    85,    34,    37,    38,    48,
      34,    37,    61,    12,    56,    48,    60,    54,    48,    72,
      73,    74,    38,    34,    37,    41,    35,    35,    48,    48,
      12,    48,    20,    39,    34,    61,    38,    37,    43,    12,
      70,    12,    13,    14,    15,    16,    17,    18,    42,    48,
      75,    76,    77,     3,    35,    46,    48,    75,    86,    87,
      44,    76,    81,    82,    83,    84,    35,    39,    12,    61,
      20,    12,    74,    39,    40,    48,    75,    77,    78,    79,
      38,    87,    87,    48,    36,    37,     3,     4,     5,     6,
      48,    36,    37,    37,    81,    12,    40,    20,    39,    40,
      12,    38,    37,    43,    12,    36,    87,    87,    87,    87,
      87,    35,    38,    34,    76,    84,    36,    40,    48,    12,
      40,    12,    79,    39,    40,    36,    48,    76,    12,    34,
      48,    40,    39,    40,    12,    36,    38,    36,    39,    40,
      12,    40,    12,    12,    35,    40,    40,    40,    36,    76,
      36,    35,    36,    36,    76,    36
  };

  const signed char
  VerilogParser::yyr1_[] =
  {
       0,    47,    48,    48,    49,    50,    50,    52,    51,    53,
      51,    54,    51,    55,    56,    51,    57,    57,    58,    58,
      58,    58,    58,    58,    58,    59,    59,    59,    60,    60,
      61,    61,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    65,    66,    66,    67,
      67,    68,    69,    69,    70,    71,    71,    71,    71,    72,
      73,    73,    74,    74,    74,    74,    75,    75,    75,    75,
      75,    75,    75,    76,    76,    76,    76,    76,    77,    78,
      78,    79,    79,    79,    79,    79,    80,    80,    81,    81,
      81,    82,    82,    83,    83,    84,    84,    84,    84,    84,
      84,    84,    84,    85,    86,    86,    87,    87,    87,    87,
      87,    87,    87,    87,    87
  };

  const signed char
  VerilogParser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     0,     2,     0,     6,     0,
       8,     0,     9,     0,     0,    10,     1,     3,     1,     2,
       1,     2,     1,     2,     2,     1,     3,     3,     2,     7,
       0,     2,     2,     1,     1,     2,     2,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       7,     3,     1,     3,     3,     1,     4,     6,     1,     3,
       1,     3,     1,     4,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     6,     1,     1,     3,     1,
       3,     1,     4,     6,     1,     1,     6,     7,     0,     1,
       1,     1,     3,     1,     3,     4,     5,     8,     5,     7,
       8,     9,    10,     4,     1,     3,     1,     2,     1,     2,
       3,     3,     3,     3,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const VerilogParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "'-'", "'+'", "'*'", "'/'",
  "UMINUS", "NEWLINE", "UNDEFINED", "NAME", "ESCAPED_NAME", "INTEGER",
  "BINARY", "OCTAL", "DECIMAL", "HEX", "REAL", "EXP", "MODULE",
  "ENDMODULE", "INPUT", "OUTPUT", "INOUT", "REG", "WIRE", "WAND", "WOR",
  "TRI", "TRIOR", "TRIAND", "SUPPLY0", "SUPPLY1", "ASSIGN", "';'", "'('",
  "')'", "','", "'['", "':'", "']'", "'='", "'{'", "'}'", "'.'", "'#'",
  "'`'", "$accept", "valid_name", "design", "modules", "module", "$@1",
  "$@2", "$@3", "$@4", "$@5", "port_names", "port_type",
  "port_declarations", "port_decl", "statements", "statement",
  "declaration", "port_decl_statements", "net_type", "net_decl_statements",
  "net_decl", "statement_assign", "assignments", "assignment", "lhs",
  "lhs_concat", "lhs_exprs", "lhs_expr", "constant", "rhs", "rhs_concat",
  "rhs_exprs", "rhs_expr", "instance", "inst_pins", "nets_by_position",
  "nets_by_name", "net_by_name", "parameters", "param_exprs", "param_expr", YY_NULLPTR
  };

#if YYDEBUG
  const short
  VerilogParser::yyrline_[] =
  {
       0,    95,    95,    96,   100,   103,   104,   109,   108,   114,
     113,   119,   118,   124,   128,   123,   136,   137,   142,   143,
     144,   145,   146,   147,   148,   153,   157,   162,   170,   176,
     187,   188,   189,   193,   194,   199,   200,   205,   209,   218,
     219,   220,   221,   222,   223,   224,   225,   229,   233,   241,
     246,   257,   260,   261,   265,   271,   272,   274,   276,   280,
     284,   285,   293,   294,   296,   298,   305,   306,   307,   308,
     309,   310,   311,   315,   316,   318,   320,   321,   325,   329,
     330,   338,   339,   341,   343,   344,   353,   361,   372,   373,
     374,   382,   383,   392,   397,   411,   413,   418,   424,   430,
     434,   440,   444,   454,   458,   459,   463,   464,   465,   466,
     467,   468,   469,   470,   471
  };

  // Print the state stack on the debug stream.
  void
  VerilogParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  VerilogParser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  VerilogParser::token_number_type
  VerilogParser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    45,     2,     2,     2,     2,
      35,    36,     5,     4,    37,     3,    44,     6,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    34,
       2,    41,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    40,     2,     2,    46,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    42,     2,    43,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33
    };
    const int user_token_number_max_ = 284;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

#line 5 "verilog_parser.yy"
} // verilog
#line 2470 "verilog_parser.tab.cc"

#line 475 "verilog_parser.yy"


void verilog::VerilogParser::error(const location_type &l, const std::string &err_message) {
  std::cerr << "Parser error: " << err_message  << '\n'
            << "  begin at line " << l.begin.line <<  " col " << l.begin.column  << '\n' 
            << "  end   at line " << l.end.line <<  " col " << l.end.column << "\n";
  std::abort();
}


