//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include <iostream>
#include <functional>
#include "tree.h"
#include "cool.h"
#include "stringtab.h"
#define yylineno curr_lineno;
extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
using Program = Program_class*;
class Class__class;
using Class_ = Class__class*;
class Feature_class;
using Feature = Feature_class*;
class Formal_class;
using Formal = Formal_class*;
class Expression_class;
using Expression = Expression_class*;
class Case_class;
using Case = Case_class*;

using Classes_class = list_node<Class_> ;
using Classes = Classes_class*;
using Features_class = list_node<Feature> ;
using Features = Features_class*;
using Formals_class = list_node<Formal> ;
using Formals = Formals_class*;
using Expressions_class = list_node<Expression> ;
using Expressions = Expressions_class*;
using Cases_class = list_node<Case> ;
using Cases = Cases_class*;

#define Program_EXTRAS                          \
virtual void semant() = 0;			\
virtual void dump_with_types(ostream&, int) = 0;



#define program_EXTRAS                          \
void semant();     				\
void dump_with_types(ostream&, int);            

#define Class__EXTRAS                   \
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0; \
virtual Symbol get_name() = 0; \
virtual Symbol get_parent() = 0; \
virtual Features get_features() = 0; \
virtual void set_seal(bool = true) = 0; \
virtual bool is_sealed() = 0; \
// virtual void set_environment(std::shared_ptr<Environment>) = 0;

#define class__EXTRAS                                 \
Symbol get_filename() { return filename; }             \
void dump_with_types(ostream&,int);    \
virtual Symbol get_name() override; \
virtual Symbol get_parent() override; \
virtual Features get_features() override; \
virtual void set_seal(bool = true) override; \
virtual bool is_sealed() override; \
// virtual void set_environment(std::shared_ptr<Environment> env) override {environment = env;}


#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0;  \
virtual void do_attr_method(std::function<void(attr_class *)>, std::function<void(method_class *)>) = 0; \
virtual Symbol get_name() = 0; \
virtual void temp(EnvironmentNode &) = 0;


#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    \
virtual void do_attr_method(std::function<void(attr_class *)>, std::function<void(method_class *)>) override; \
virtual Symbol get_name() override {return name;} \
virtual void temp(EnvironmentNode &) override;

#define attr_EXTRAS \
virtual Symbol get_type_decl() {return type_decl;}

#define Formal_EXTRAS                              \
virtual void dump_with_types(ostream&,int) = 0; \
virtual Symbol get_type_decl() = 0; \
virtual Symbol get_name() = 0;


#define formal_EXTRAS                           \
void dump_with_types(ostream&,int); \
virtual Symbol get_type_decl(){return type_decl;}; \
virtual Symbol get_name(){return name;};


#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0; \
virtual void temp(EnvironmentNode &) = 0; \
virtual Symbol get_type() = 0; \
virtual Symbol get_type_decl() = 0;


#define branch_EXTRAS                                   \
void dump_with_types(ostream& ,int); \
virtual void temp(EnvironmentNode&) override; \
virtual Symbol get_type() { return expr -> type; } \
virtual Symbol get_type_decl() {return type_decl;};

#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; } \
virtual void temp(EnvironmentNode &) = 0;

#define Expression_SHARED_EXTRAS           \
void dump_with_types(ostream&,int); \
virtual void temp(EnvironmentNode &) override;

#define method_EXTRAS \
virtual Symbol get_return_type(){return return_type;} \
virtual Formals get_formals() {return formals;} \
virtual bool is_seal() {return seal;} \
virtual void set_seal() {seal = true;}

#define eq_EXTRAS \
static bool is_allowed_comp(Symbol lhs, Symbol rhs);

#endif
