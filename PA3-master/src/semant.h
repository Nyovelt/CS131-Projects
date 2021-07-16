#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <map>
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"

#define TRUE 1
#define FALSE 0

class ClassTable;
using ClassTableP = ClassTable *;

class EnvironmentNode;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

// using ClassNode = TreeNode<Class_>;
// using Class_ptr = Class__class *;

class ClassNode
{
protected:
  Class_ element;
  std::shared_ptr<ClassNode> parent;
  std::vector<std::shared_ptr<ClassNode>> children;
  std::map<Symbol, attr_class *> attr_map;
  std::map<Symbol, method_class *> method_map;
  std::vector<attr_class *> attr_list;
  std::vector<method_class *> method_list;

public:
  ClassNode(Class_, ClassTable *);
  // void set_parent();

  Class_ &get_element();
  const Class_ &get_element() const;

  std::shared_ptr<ClassNode> get_parent();
  void set_parent(std::shared_ptr<ClassNode>);

  void add_child(std::shared_ptr<ClassNode>);

  method_class *find_method(Symbol) const;
  attr_class *find_attr(Symbol) const;
  void dfs(std::function<void(const ClassNode &)>) const;

  std::unique_ptr<std::vector<Symbol>> class_inherit_list();

  virtual void temp(EnvironmentNode &environment_node) const;

  attr_class *find_attr_in_parent(Symbol attr) const
  {
    if (parent == nullptr)
      return nullptr;

    auto pattr = parent->find_attr(attr);

    if (pattr == nullptr)
      return parent->find_attr_in_parent(attr);

    return pattr;
  }

  method_class *find_method_in_parent(Symbol meth) const
  {

    if (parent == nullptr)
      return nullptr;

    auto pmeth = parent->find_method(meth);

    if (pmeth == nullptr)
      return parent->find_method_in_parent(meth);

    return pmeth;
  }

  void check_attr(ClassTable *table) const;
  void check_meth(ClassTable *table) const;

  void add_attr(EnvironmentNode &environment_node, bool self_error = true) const;

  void add_parent_attr(EnvironmentNode &environment_node) const
  {
    if (parent == nullptr)
      return;
    parent->add_attr(environment_node, false);
    parent->add_parent_attr(environment_node);
  }
};

class ClassTable
{
private:
  int semant_errors;
  void install_basic_classes();
  ostream &error_stream;

  std::map<Symbol, std::shared_ptr<ClassNode>> class_map;
  std::vector<std::shared_ptr<ClassNode>> class_list;

public:
  bool debugging = false;

  ClassTable(Classes);
  int errors() { return semant_errors; }

  ostream &semant_error();
  ostream &semant_error(Class_ c);
  ostream &semant_error(Symbol, tree_node *);
  ostream &inherits_from_undefined_class_errror(Class_, Symbol);
   ostream &inherits_from_self_type(Class_);
  ostream &cannot_inherit_errror(Class_, Class_);
  ostream &class_already_defined_error(Class_);
  ostream &inheritance_cycle_error(Class_);
  ostream &no_main_class_error();
  ostream &no_main_method_error(Class_);

  ostream &basic_class_redefination_error(Class_ class_)
  {
    return semant_error(class_) << "Redefinition of basic class " << class_->get_name() << "." << std::endl;
  }

  ostream &main_has_formal_error(Class_ c)
  {
    return semant_error(c) << "'main' method in class " << c->get_name() << " should have no arguments." << std::endl;
  }

  ostream &attribute_inherited_class_error(Class_ c, tree_node *node, Symbol attr)
  {
    return semant_error(c->get_filename(), node) << "Attribute " << attr << " is an attribute of an inherited class." << std::endl;
  }

  ostream &redefined_method_return_error(Class_ c, method_class *method, Symbol re_type, Symbol ori_type)
  {
    return semant_error(c->get_filename(), method) << "In redefined method " << method->get_name() << ", return type " << re_type << " is different from original return type " << ori_type << "." << std::endl;
  }

  ostream &redefined_method_param_type_error(Class_ c, method_class *method, Symbol decl_type, Symbol ori_type)
  {
    return semant_error(c->get_filename(), method) << "In redefined method " << method->get_name() << ", parameter type " << decl_type << " is different from original type " << ori_type << std::endl;
  }

  ostream &redefined_method_param_length_error(Class_ c, method_class *method)
  {
    return semant_error(c->get_filename(), method) << "Incompatible number of formal parameters in redefined method " << method->get_name() << "." << std::endl;
  }

  ostream &attr_multiply_defined_error(Class_ c, attr_class *attr)
  {
    return semant_error(c->get_filename(), attr) << "Attribute " << attr->get_name() << " is multiply defined in class." << std::endl;
  }

  ostream &method_multiply_defined_error(Class_ c, method_class *meth)
  {
    return semant_error(c->get_filename(), meth) << "Method " << meth->get_name() << " is multiply defined." << std::endl;
  }

  void install_classes(Classes);
  void install_class(Class_, bool = false);
  void install_dependencies();
  void check_cycle();
  void check_main();

  void check_features();

  static std::shared_ptr<ClassNode> find_lca(Symbol, Symbol);

  std::shared_ptr<ClassNode> object_node();
  std::shared_ptr<ClassNode> find_class(Symbol);

  method_class *find_method(std::shared_ptr<ClassNode>, Symbol);
  method_class *find_method(Symbol type_decl, Symbol method);

  void temp(EnvironmentNode &environment_node);
  bool conform(std::shared_ptr<ClassNode>, std::shared_ptr<ClassNode>);
  bool conform(Symbol, Symbol);

  void exit_if_error();
};

class Environment
{
private:
  Symbol self_class;
  std::map<Symbol, Symbol> variable_list;
  std::shared_ptr<Environment> parent;

public:
  Environment();
  Environment(const Environment &rhs);

  void dump_variable_list();

  method_class *find_method(Symbol);
  method_class *find_method(Symbol, Symbol);

  void add_variable(Symbol identifier, Symbol type);
  Symbol find_variable(Symbol symbol);

  Symbol get_self();
  void set_self(Symbol self);

  void set_parent(std::shared_ptr<Environment> p);
  std::shared_ptr<Environment> get_parent();
};

class EnvironmentNode
{
private:
  ClassTable *class_table;
  std::vector<std::shared_ptr<Environment>> environments;

public:
  bool debugging() { return class_table == nullptr ? false : class_table->debugging; };

  EnvironmentNode() = delete;
  EnvironmentNode(ClassTable *class_table);
  EnvironmentNode(std::shared_ptr<Environment>, ClassTable *);

  void set_classtable(ClassTable *table);

  std::shared_ptr<Environment> push();
  std::shared_ptr<Environment> pop();
  std::shared_ptr<Environment> current();

  std::shared_ptr<ClassNode> find_class(Symbol);
  Symbol find_variable(Symbol);
  method_class *find_method(Symbol, Symbol);

  Symbol extract_self(Symbol);
  bool conform(Symbol, Symbol);
  Symbol find_lca(Symbol, Symbol);

  int errors();
  auto get_class_table() { return class_table; }

  ostream &semant_error(tree_node *node)
  {
    auto self = current()->get_self();
    if (self == nullptr)
    {
      std::cerr << "ERROR: self IS NULLPTR" << std::endl;
      exit(1);
    }

    auto file_name = find_class(self)->get_element()->get_filename();
    return class_table->semant_error(file_name, node);
  }

  ostream &let_bound_id_undefined(tree_node *node, Symbol identifier, Symbol type_decl)
  {
    return semant_error(node) << "Class " << type_decl << " of let-bound identifier " << identifier << " is undefined." << std::endl;
  }

  ostream &let_inferred_type_not_conform(tree_node *node, Symbol inferred, Symbol identifier, Symbol type_decl)
  {
    return semant_error(node) << "Inferred type " << inferred << " of initialization of " << identifier << " does not conform to identifier's declared type " << type_decl << "." << std::endl;
  }

  ostream &meth_inferred_return_not_conform(tree_node *node, Symbol inferred, Symbol meth_name, Symbol return_type)
  {
    return semant_error(node) << "Inferred return type " << inferred << " of method " << meth_name << " does not conform to declared return type " << return_type << "." << std::endl;
  }

  ostream &non_int_args_leq(tree_node *node, Symbol lhs, Symbol rhs)
  {
    return semant_error(node) << "non-Int arguments: " << lhs << " <= " << rhs << std::endl;
  }

  ostream &non_int_args_not(tree_node *node, Symbol type)
  {
    return semant_error(node) << "Argument of 'not' has type " << type << " instead of Bool." << std::endl;
  }

  ostream &method_type_error(tree_node *t, Symbol method_name, Symbol type, Symbol parameter, Symbol type_decl)
  {
    return semant_error(t) << "In call of method " << method_name << ", type " << type << " of parameter " << parameter << " does not conform to declared type " << type_decl << "." << std::endl;
    // In call of method f, type Object of parameter a does not conform to declared type Int.
  }

  ostream &loop_type_error(tree_node *t)
  {
    return semant_error(t) << "Loop condition does not have type Bool." << std::endl;
  }

  ostream &cond_type_error(tree_node *t)
  {
    return semant_error(t) << "Predicate of 'if' does not have type Bool." << std::endl;
  }

  ostream &static_dispatch_not_conform(tree_node *t, Symbol method, Symbol expr_type, Symbol type_name)
  {
    return semant_error(t) << "Expression type " << expr_type << " does not conform to declared static dispatch type " << type_name << "." << std::endl;
    // Expression type SELF_TYPE does not conform to declared static dispatch type C.
  }

  ostream &static_dispatch_selftype(tree_node *t)
  {
    return semant_error(t) << "Static dispatch to SELF_TYPE." << std::endl;
  }

  ostream &formal_paramater_multiply_defined(tree_node *node, Symbol parameter_name)
  {
    return semant_error(node) << "Formal parameter " << parameter_name << " is multiply defined." << std::endl;
  }

  ostream &formal_cannot_have_self_type(tree_node *node, Symbol parameter_name)
  {
    return semant_error(node) << "Formal parameter " << parameter_name << " cannot have type SELF_TYPE." << std::endl;
  }

  ostream &self_cannot_be_formal(tree_node *node)
  {
    return semant_error(node) << "'self' cannot be the name of a formal parameter." << std::endl;
  }

  ostream &dispatch_length_error(tree_node *node, Symbol name)
  {
    return semant_error(node) << "Method " << name << " called with wrong number of arguments." << std::endl;
  }

  ostream &self_cannot_be_attribute(tree_node *t)
  {
    return semant_error(t) << "'self' cannot be the name of an attribute." << std::endl;
  }

  ostream &self_in_let(tree_node *t)
  {
    return semant_error(t) << "'self' cannot be bound in a 'let' expression." << std::endl;
  }

  ostream &case_duplicate_branch(tree_node *node, Symbol decl)
  {
    return semant_error(node) << "Duplicate branch " << decl << " in case statement." << std::endl;
  }

  ostream &self_in_assign(tree_node *t)
  {
    return semant_error(t) << "Cannot assign to 'self'." << std::endl;
  }

  ostream &undefined_return_type(tree_node *node, Symbol return_type, Symbol name)
  {
    return semant_error(node) << "Undefined return type " << return_type << " in method " << name << "." << std::endl;
  }

  ostream &undefined_class(tree_node *, Symbol);

  ostream &assign_not_conform(tree_node *, Symbol, Symbol, Symbol);
  ostream &undeclared_identifier(tree_node *, Symbol);
  ostream &non_int_args_lt(tree_node *, Symbol, Symbol);
  ostream &non_int_args_neg(tree_node *, Symbol);
  ostream &non_int_args_divide(tree_node *, Symbol, Symbol);
  ostream &non_int_args_mul(tree_node *, Symbol, Symbol);
  ostream &non_int_args_sub(tree_node *, Symbol, Symbol);
  ostream &non_int_args_plus(tree_node *, Symbol, Symbol);
  ostream &unexist_dispatch_method(tree_node *, Symbol);
  ostream &unexist_static_dispatch_method(tree_node *, Symbol);
  ostream &unexist_dispatch_class(tree_node *, Symbol);
  ostream &unexist_static_dispatch_class(tree_node *, Symbol);
  ostream &unallowed_comp(tree_node *);
  // ostream &redefined_method_return_error(tree_node *, Symbol ,Symbol, Symbol);

  void exit_if_error() { class_table->exit_if_error(); }
};

#endif
