

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <set>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

// ClassTable *class_table;
// and we need a Class_

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg = idtable.add_string("arg");
    arg2 = idtable.add_string("arg2");
    Bool = idtable.add_string("Bool");
    concat = idtable.add_string("concat");
    cool_abort = idtable.add_string("abort");
    copy = idtable.add_string("copy");
    Int = idtable.add_string("Int");
    in_int = idtable.add_string("in_int");
    in_string = idtable.add_string("in_string");
    IO = idtable.add_string("IO");
    length = idtable.add_string("length");
    Main = idtable.add_string("Main");
    main_meth = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any
    //   user-defined class.
    No_class = idtable.add_string("_no_class");
    No_type = idtable.add_string("_no_type");
    Object = idtable.add_string("Object");
    out_int = idtable.add_string("out_int");
    out_string = idtable.add_string("out_string");
    prim_slot = idtable.add_string("_prim_slot");
    self = idtable.add_string("self");
    SELF_TYPE = idtable.add_string("SELF_TYPE");
    Str = idtable.add_string("String");
    str_field = idtable.add_string("_str_field");
    substr = idtable.add_string("substr");
    type_name = idtable.add_string("type_name");
    val = idtable.add_string("_val");
}

/*******************************************************************
 * ClassNode
 *******************************************************************/

ClassNode::ClassNode(Class_ class_, ClassTable *class_table) : element(class_)
{
    auto features = class_->get_features();
    for (int i = features->first(); features->more(i); i = features->next(i))
    {
        auto f = features->nth(i);
        f->do_attr_method([this, class_table, class_](attr_class *attr)
                          {
                              auto name = attr->get_name();
                            //   auto it = this->attr_map.find(name);
                            //   if (it != attr_map.end())
                            //   {
                            //       class_table->attr_multiply_defined_error(class_, attr);
                            //       return;
                            //   }
                              this->attr_map[name] = attr;
                              this->attr_list.push_back(attr);
                          },
                          [this, class_table, class_](method_class *method)
                          {
                              auto name = method->get_name();
                            //   auto it = this->method_map.find(name);
                            //   if (it != method_map.end())
                            //   {
                            //       class_table->method_multiply_defined_error(class_, method);
                            //       return;
                            //   }
                              this->method_map[name] = method;
                              this->method_list.push_back(method);
                          });
    }
}

Class_ &ClassNode::get_element()
{
    return element;
}

const Class_ &ClassNode::get_element() const
{
    return element;
}

std::shared_ptr<ClassNode> ClassNode::get_parent() { return parent; };

method_class *ClassNode::find_method(Symbol name) const
{
    auto it = method_map.find(name);
    if (it == method_map.end())
        return nullptr;
    return it->second;
}

attr_class *ClassNode::find_attr(Symbol name) const
{
    auto it = attr_map.find(name);
    if (it == attr_map.end())
        return nullptr;
    return it->second;
}

void ClassNode::set_parent(std::shared_ptr<ClassNode> tree_node)
{
    parent = tree_node;
}

void ClassNode::add_child(std::shared_ptr<ClassNode> tree_node)
{
    children.push_back(tree_node);
}

void ClassNode::dfs(std::function<void(const ClassNode &)> function) const
{
    function(*this);

    for (auto &&child : children)
    {
        child->dfs(function);
    }
}

std::unique_ptr<std::vector<Symbol>> ClassNode::class_inherit_list()
{
    std::unique_ptr<std::vector<Symbol>> p;
    if (parent == nullptr)
    {
        p = std::make_unique<std::vector<Symbol>>();
    }
    else
    {
        p = parent->class_inherit_list();
    }
    p->push_back(element->get_name());
    return p;
}

void ClassNode::check_attr(ClassTable *table) const
{
    if (parent == nullptr)
        return;

    std::set<Symbol> attr_set;

    for (const auto &attr : attr_list)
    {
        auto name = attr->get_name();
        auto it = attr_set.find(name);
        if (it != attr_set.end())
            table->attr_multiply_defined_error(element, attr);
        else
            attr_set.insert(name);
    }

    for (const auto &attr : attr_list)
    {
        auto name = attr->get_name();
        if (find_attr_in_parent(name) != nullptr)
        {
            table->attribute_inherited_class_error(element, attr, name);
        }
    }
}

void ClassNode::check_meth(ClassTable *table) const
{
    if (parent == nullptr)
        return;

        
    std::set<Symbol> meth_set;

    for (const auto &meth : method_list)
    {
        auto name = meth->get_name();
        auto it = meth_set.find(name);
        if (it != meth_set.end())
            table->method_multiply_defined_error(element, meth);
        else
            meth_set.insert(name);
    }

    for (const auto &meth : method_list)
    {
        auto name = meth->get_name();
        auto pmeth = find_method_in_parent(name);
        if (pmeth != nullptr)
        {
            auto rt = meth->get_return_type();
            auto pt = pmeth->get_return_type();
            if (rt != pt)
            {
                table->redefined_method_return_error(element, meth, rt, pt);
                continue;
            }

            auto l_formals = meth->get_formals();
            auto r_formals = pmeth->get_formals();
            auto l_len = l_formals->len();
            auto r_len = r_formals->len();

            if (l_len != r_len)
            {
                table->redefined_method_param_length_error(element, meth);
                continue;
            }

            for (int i = 0; i < l_len; i++)
            {
                auto l_formal = l_formals->nth(i);
                auto r_formal = r_formals->nth(i);
                auto l_type = l_formal->get_type_decl();
                auto r_type = r_formal->get_type_decl();
                if (l_type != r_type)
                {
                    table->redefined_method_param_type_error(element, meth, l_type, r_type);
                    break;
                }
            }
        }
    }
}

/*******************************************************************
 * ClassTable
 *******************************************************************/

ClassTable::ClassTable(Classes classes) : semant_errors(0), error_stream(cerr)
{
    // idtable.print();
    // idtable.lookup(0)->print(std::cout);
    // std::cout << *idtable.lookup(0) << std::endl;
    // for (int i = idtable.first(); idtable.more(i); i = idtable.next(i))
    // {
    //     std::cout << ' ';
    //     idtable.lookup(i)->print(std::cout);
    // }
    // std::cout << std::endl;
    /* Fill this in */
    install_basic_classes();
    install_classes(classes);
    install_dependencies();
}

void ClassTable::install_classes(Classes classes)
{
    for (int i = classes->first(); classes->more(i); i = classes->next(i))
    {
        auto c = classes->nth(i);
        install_class(c);
    }
}

void ClassTable::check_cycle()
{
    std::set<Class_> class_set;

    class_map[Object]->dfs([&class_set](const auto &node)
                           { class_set.insert(node.get_element()); });

    for (auto it = class_list.crbegin(); it != class_list.crend(); it++)
    {
        auto &&class_ = (*it)->get_element();
        if (class_set.find(class_) == class_set.end())
            inheritance_cycle_error(class_);
    }
}

void ClassTable::check_features()
{
    class_map[Object]->dfs([this](const ClassNode &node)
                           {
                               node.check_attr(this);
                               node.check_meth(this);
                           });
    // std::set<Class_> class_set;

    // class_map[Object]->dfs([&class_set](const auto &node)
    //                        { class_set.insert(node.get_element()); });

    // for (auto it = class_list.crbegin(); it != class_list.crend(); it++)
    // {
    //     auto &&class_ = (*it)->get_element();
    //     if (class_set.find(class_) == class_set.end())
    //         inheritance_cycle_error(class_);
    // }
}

void ClassTable::check_main()
{
    auto main_it = class_map.find(Main);
    if (main_it == class_map.end())
    {
        no_main_class_error();
        return;
    }

    auto meth = main_it->second->find_method(main_meth);
    if (meth == nullptr)
    {
        no_main_method_error(main_it->second->get_element());
    }

    auto formal = meth->get_formals();
    if (formal->len() > 0)
    {
        main_has_formal_error(main_it->second->get_element());
    }
}

void ClassTable::install_class(Class_ c, bool allow_redefine)
{
    auto sp = std::make_shared<ClassNode>(c, this);
    auto name = c->get_name();

    if (!allow_redefine)
    {
        for (auto &&basic_class : {Object, IO, Int, Bool, Str, SELF_TYPE})
        {
            if (name == basic_class)
            {
                basic_class_redefination_error(c);
                return;
            }
        }
    }

    auto it = class_map.find(name);
    if (it != class_map.end())
    {
        class_already_defined_error(c);
        return;
    }

    auto symbol = c->get_name();
    class_map.emplace(symbol, sp);
    class_list.push_back(sp);
}

void ClassTable::install_dependencies()
{
    for (auto it = class_list.crbegin(); it != class_list.crend(); it++)
    {
        auto &&node = *it;
        auto symbol = node->get_element()->get_name();
        auto parent = node->get_element()->get_parent();

        // symbol->print(std::clog) << " -> ";
        // parent->print(std::clog);

        auto parent_it = class_map.find(parent);

        if (parent == No_class && symbol == Object)
            continue;

        if (parent == SELF_TYPE){
            inherits_from_self_type(node->get_element());
            continue;
        }

        if (parent_it == class_map.end())
        {
            inherits_from_undefined_class_errror(node->get_element(), parent);
            continue;
        }
        auto parent_node = parent_it->second;

        // TODO
        if (parent_node->get_element()->is_sealed())
        {
            cannot_inherit_errror(node->get_element(), parent_node->get_element());
            continue;
        }
    }

    // hack
    for (const auto &node : class_list)
    {
        auto symbol = node->get_element()->get_name();
        auto parent = node->get_element()->get_parent();

        if (parent == No_class && symbol == Object)
            continue;

        auto parent_it = class_map.find(parent);
        if (parent_it == class_map.end())
            continue;

        auto parent_node = parent_it->second;
        if (parent_node->get_element()->is_sealed())
            continue;
        node->set_parent(parent_node);
        parent_node->add_child(node);
    }
}

void ClassTable::install_basic_classes()
{

    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.

    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    //
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Class_ Object_class =
        class_(Object,
               No_class,
               append_Features(
                   append_Features(
                       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                   single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
               filename);

    //
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class =
        class_(IO,
               Object,
               append_Features(
                   append_Features(
                       append_Features(
                           single_Features(method(out_string, single_Formals(formal(arg, Str)),
                                                  SELF_TYPE, no_expr())),
                           single_Features(method(out_int, single_Formals(formal(arg, Int)),
                                                  SELF_TYPE, no_expr()))),
                       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
                   single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
               filename);

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer.
    //
    Class_ Int_class =
        class_(Int,
               Object,
               single_Features(attr(val, prim_slot, no_expr())),
               filename);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
        class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //
    Class_ Str_class =
        class_(Str,
               Object,
               append_Features(
                   append_Features(
                       append_Features(
                           append_Features(
                               single_Features(attr(val, Int, no_expr())),
                               single_Features(attr(str_field, prim_slot, no_expr()))),
                           single_Features(method(length, nil_Formals(), Int, no_expr()))),
                       single_Features(method(concat,
                                              single_Formals(formal(arg, Str)),
                                              Str,
                                              no_expr()))),
                   single_Features(method(substr,
                                          append_Formals(single_Formals(formal(arg, Int)),
                                                         single_Formals(formal(arg2, Int))),
                                          Str,
                                          no_expr()))),
               filename);

    Int_class->set_seal();
    Bool_class->set_seal();
    Str_class->set_seal();

    install_class(Object_class, true);
    install_class(IO_class, true);
    install_class(Int_class, true);
    install_class(Bool_class, true);
    install_class(Str_class, true);
}

bool ClassTable::conform(Symbol child, Symbol parent)
{
    if (child == No_type)
        return true;

    debugging &&std::cerr << "=> conform: " << child << " <= " << parent << std::endl;
    auto child_node = find_class(child);
    auto parent_node = find_class(parent);
    return conform(child_node, parent_node);
}

std::shared_ptr<ClassNode> ClassTable::find_class(Symbol name)
{
    auto it = class_map.find(name);
    if (it == class_map.end())
        return nullptr;
    else
        return it->second;
}

method_class *ClassTable::find_method(std::shared_ptr<ClassNode> class_node, Symbol method_name)
{
    if (class_node == nullptr)
        return nullptr;

    auto method = class_node->find_method(method_name);
    if (method == nullptr)
        return find_method(class_node->get_parent(), method_name);

    return method;
}

method_class *ClassTable::find_method(Symbol class_, Symbol method)
{
    auto class_node = find_class(class_);
    return find_method(class_node, method);
}

void ClassTable::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> ClassTable" << std::endl;

    auto object = object_node();
    object->dfs([&environment_node](const ClassNode &tree_node)
                { tree_node.temp(environment_node); });
}

bool ClassTable::conform(std::shared_ptr<ClassNode> child, std::shared_ptr<ClassNode> parent)
{
    if (child == nullptr || parent == nullptr)
        return false;
    if (child->get_element()->get_name() != parent->get_element()->get_name())
    {
        return conform(child->get_parent(), parent);
    }
    return true;
}

std::shared_ptr<ClassNode> ClassTable::object_node()
{
    return class_map[Object];
}

void ClassTable::exit_if_error()
{
    if (errors())
    {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

/*******************************************************************
 * Errors
 *******************************************************************/

ostream &ClassTable::inherits_from_undefined_class_errror(Class_ current, Symbol undefined)
{
    return semant_error(current) << "Class " << current->get_name() << " inherits from an undefined class " << undefined << "." << std::endl;
}

ostream &ClassTable::inherits_from_self_type(Class_ current)
{
    return semant_error(current) << "Class " << current->get_name() << " cannot inherit class SELF_TYPE." << std::endl;
}

ostream &ClassTable::class_already_defined_error(Class_ class_)
{
    return semant_error(class_) << "Class " << class_->get_name() << " was previously defined." << std::endl;
}

ostream &ClassTable::inheritance_cycle_error(Class_ class_)
{
    return semant_error(class_) << "Class " << class_->get_name() << ", or an ancestor of " << class_->get_name() << ", is involved in an inheritance cycle." << std::endl;
}

ostream &ClassTable::cannot_inherit_errror(Class_ class_, Class_ parent)
{
    return semant_error(class_) << "Class " << class_->get_name() << " cannot inherit class " << parent->get_name() << "." << std::endl;
}

ostream &ClassTable::no_main_class_error()
{
    return semant_error() << "Class Main is not defined." << std::endl;
}

ostream &ClassTable::no_main_method_error(Class_ main)
{
    return semant_error(main) << "No 'main' method in class " << main->get_name() << "." << std::endl;
}

ostream &EnvironmentNode::assign_not_conform(tree_node *t, Symbol expr_type, Symbol name_type, Symbol identifier_name)
{
    // std::cerr << "Remove me: ERROR: assign not conform: "
    //           << "Type " << expr_type << " of assigned expression does not conform to declared type " << name_type << " of identifier " << identifier_name << "." << std::endl;
    return semant_error(t) << "Type " << expr_type << " of assigned expression does not conform to declared type " << name_type << " of identifier " << identifier_name << "." << std::endl;

    //Type x of assigned expression does not conform to declared type y of identifier z.
};

ostream &EnvironmentNode::undefined_class(tree_node *t, Symbol type_name)
{
    return semant_error(t) << "'new' used with undefined class " << type_name << "." << std::endl;
    // 'new' used with undefined class xxx.
}

ostream &EnvironmentNode::undeclared_identifier(tree_node *t, Symbol name)
{
    return semant_error(t) << "Undeclared identifier " << name << "." << std::endl;
    //  Undeclared identifier xxx.
}

ostream &EnvironmentNode::non_int_args_lt(tree_node *t, Symbol e1, Symbol e2)
{
    return semant_error(t) << "non-Int arguments: " << e1 << " < " << e2 << std::endl;
    //  non-Int arguments: Int < xxx
}

ostream &EnvironmentNode::non_int_args_neg(tree_node *t, Symbol type)
{
    return semant_error(t) << "Argument of '~' has type " << type << " instead of Int." << std::endl;
}

ostream &EnvironmentNode::non_int_args_divide(tree_node *t, Symbol e1, Symbol e2)
{
    return semant_error(t) << "non-Int arguments: " << e1 << " / " << e2 << std::endl;
    //  non-Int arguments: Int / xxx
}

ostream &EnvironmentNode::non_int_args_mul(tree_node *t, Symbol e1, Symbol e2)
{
    return semant_error(t) << "non-Int arguments: " << e1 << " * " << e2 << std::endl;
    //  non-Int arguments: Int * xxx
}

ostream &EnvironmentNode::non_int_args_sub(tree_node *t, Symbol e1, Symbol e2)
{
    return semant_error(t) << "non-Int arguments: " << e1 << " - " << e2 << std::endl;
    //  non-Int arguments: Int - xxx
}

ostream &EnvironmentNode::non_int_args_plus(tree_node *t, Symbol e1, Symbol e2)
{
    return semant_error(t) << "non-Int arguments: " << e1 << " + " << e2 << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &EnvironmentNode::unexist_dispatch_method(tree_node *t, Symbol name)
{
    return semant_error(t) << "Dispatch to undefined method " << name << "." << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &EnvironmentNode::unexist_dispatch_class(tree_node *t, Symbol name)
{
    return semant_error(t) << "Dispatch on undefined class " << name << "." << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &EnvironmentNode::unexist_static_dispatch_method(tree_node *t, Symbol name)
{
    return semant_error(t) << "Static dispatch to undefined method " << name << "." << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &EnvironmentNode::unexist_static_dispatch_class(tree_node *t, Symbol name)
{
    return semant_error(t) << "Static dispatch on undefined class " << name << "." << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &EnvironmentNode::unallowed_comp(tree_node *t)
{
    // std::cerr << "Remove me: ERROR: unallowed_comp:"
    //           << " Illegal comparison with a basic type." << std::endl;

    return semant_error(t) << "Illegal comparison with a basic type." << std::endl;
    //  non-Int arguments: Int + xxx
}

ostream &ClassTable::semant_error(Class_ c)
{
    return semant_error(c->get_filename(), c);
}

ostream &ClassTable::semant_error(Symbol filename, tree_node *t)
{
    // TODO: core dumped because error_stream
    //environment_node().debugging && std::cerr << std::endl;
    //environment_node().debugging && std::cerr << ":" << t->get_line_number() << std::endl;
    error_stream << filename << ":" << t->get_line_number() << ": ";
    //environment_node().debugging && std::cerr << std::endl;
    return semant_error();
}

ostream &ClassTable::semant_error()
{
    semant_errors++;
    return error_stream;
}

/*******************************************************************
 * Environment
 *******************************************************************/

Environment::Environment() : parent(nullptr), self_class(nullptr)
{
}

Environment::Environment(const Environment &rhs)
{
    // if(class_table->debugging) std::cerr << ".." << &parent << std::endl;
    // class_table = rhs.class_table;
    self_class = rhs.self_class;
    variable_list = rhs.variable_list;
    parent = rhs.parent;
}

void Environment::dump_variable_list()
{
    std::cerr << "Length: " << variable_list.size() << std::endl;
    for (const auto &[key, value] : variable_list)
    {
        std::cerr << key << " -> " << value << std::endl;
    }
}

void Environment::add_variable(Symbol identifier, Symbol type)
{
    variable_list[identifier] = type;
}

Symbol Environment::find_variable(Symbol symbol)
{
    auto it = variable_list.find(symbol);
    if (it == variable_list.end())
        return nullptr;
    return it->second;
}

void Environment::set_self(Symbol self)
{
    self_class = self;
}

Symbol Environment::get_self()
{
    return self_class;
}

std::shared_ptr<Environment> Environment::get_parent()
{
    return parent;
}

void Environment::set_parent(std::shared_ptr<Environment> p)
{
    parent = p;
}

/*******************************************************************
 * EnvironmentNode
 *******************************************************************/

EnvironmentNode::EnvironmentNode(ClassTable *class_table) : class_table(class_table) {}

EnvironmentNode::EnvironmentNode(std::shared_ptr<Environment> root, ClassTable *class_table) : class_table(class_table)
{
    environments.push_back(root);
};

Symbol EnvironmentNode::extract_self(Symbol type)
{
    return type == SELF_TYPE ? current()->get_self() : type;
}

Symbol EnvironmentNode::find_lca(Symbol lhs, Symbol rhs)
{
    if (lhs == No_type)
        return rhs;

    debugging() && std::cerr << "TODO lca: " << lhs << ", " << rhs << std::endl;

    if (lhs == SELF_TYPE && rhs == SELF_TYPE)
        return SELF_TYPE;

    lhs = extract_self(lhs);
    rhs = extract_self(rhs);

    auto list_l = find_class(lhs)->class_inherit_list();
    auto list_r = find_class(rhs)->class_inherit_list();
    auto len = std::min(list_l->size(), list_r->size());
    auto last_same = Object;

    for (size_t i = 0; i < len; i++)
    {
        auto res_l = list_l->at(i);
        auto res_r = list_r->at(i);
        if (res_l == res_r)
            last_same = res_l;
        else
            break;
    }

    return last_same;
}

std::shared_ptr<ClassNode> EnvironmentNode::find_class(Symbol name)
{
    debugging() && std::cerr << "class_table: " << class_table << " find " << name << std::endl;
    return class_table->find_class(name);
}

method_class *EnvironmentNode::find_method(Symbol class_, Symbol method)
{
    return class_table->find_method(class_, method);
}

std::shared_ptr<Environment> EnvironmentNode::current()
{
    return environments.back();
}

void EnvironmentNode::set_classtable(ClassTable *table)
{
    class_table = table;
}

Symbol EnvironmentNode::find_variable(Symbol symbol)
{
    debugging() && std::cerr << "TODO find " << symbol << std::endl;
    for (auto it = environments.crbegin(); it != environments.crend(); it++)
    {
        if (debugging())
            (*it)->dump_variable_list();
        auto symb = (*it)->find_variable(symbol);
        if (symb != nullptr)
            return symb;
    }
    debugging() && std::cerr << "...find " << symbol << " failed" << std::endl;

    return nullptr;
}

std::shared_ptr<Environment> EnvironmentNode::push()
{
    auto environment = std::make_shared<Environment>();

    if (!environments.empty())
    {
        auto c = current();
        environment->set_parent(c);
        environment->set_self(c->get_self());
    }

    environments.push_back(environment);
    return environment;
};

std::shared_ptr<Environment> EnvironmentNode::pop()
{
    auto back = current();
    environments.pop_back();
    return back;
}

int EnvironmentNode::errors()
{
    auto error_cnt = 0;
    if (class_table != nullptr)
        error_cnt += class_table->errors();
    return error_cnt;
}

bool EnvironmentNode::conform(Symbol child, Symbol parent)
{
    debugging() && std::cerr << "TODO conform: " << child << " <= " << parent << " in " << current()->get_self() << std::endl;
    if (child == No_type)
        return true;
    if (parent == No_type)
        return false;

    if (child == SELF_TYPE && parent == SELF_TYPE)
        return true;
    if (child != SELF_TYPE && parent != SELF_TYPE)
        return class_table->conform(child, parent);

    auto current_self = current()->get_self();

    if (child == SELF_TYPE)
        return class_table->conform(current_self, parent);

    if (parent == SELF_TYPE)
        return false;

    return false;
}

/*******************************************************************
 * extra define
 *******************************************************************/

Symbol class__class::get_name()
{
    return name;
}

Symbol class__class::get_parent()
{
    return parent;
}

Features class__class::get_features()
{
    return features;
}

void class__class::set_seal(bool seal)
{
    sealed = seal;
}

bool class__class::is_sealed()
{
    return sealed;
}

/*******************************************************************
 * semant
 *******************************************************************/

/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */

void program_class::semant()
{
    initialize_constants();

    auto class_table = new ClassTable(classes);
    auto environment = std::make_shared<Environment>();
    auto environment_node = EnvironmentNode(environment, class_table);
    // class_table->debugging = true;

    /* ClassTable constructor may do some semantic analysis */
    class_table->exit_if_error();

    /* some semantic analysis code may go here */
    class_table->check_cycle();
    class_table->exit_if_error();

    class_table->check_features();
    // class_table->exit_if_error();

    class_table->check_main();

    // exit_if_error(class_table);

    class_table->temp(environment_node);
    class_table->exit_if_error();

    delete class_table;
    environment_node.set_classtable(nullptr);
}

void ClassNode::add_attr(EnvironmentNode &environment_node, bool self_error) const
{
    auto current = environment_node.current();
    for (const auto &[key, attr] : attr_map)
    {
        if (key == self)
        {
            if (self_error)
                environment_node.self_cannot_be_attribute(attr);
            continue;
        }
        current->add_variable(attr->get_name(), attr->get_type_decl());
    }
}

void ClassNode::temp(EnvironmentNode &environment_node) const
{
    environment_node.debugging() && std::cerr << "-> ClassNode: " << element->get_name() << std::endl;

    auto current = environment_node.push();
    current->set_self(element->get_name());

    add_attr(environment_node);
    add_parent_attr(environment_node);
    
    for (const auto &attr : attr_list)
    {
        attr->temp(environment_node);
    }

    for (const auto &method : method_list)
    {
        method->temp(environment_node);
    }

    environment_node.pop();

    environment_node.debugging() && std::cerr << std::endl;
}

void attr_class::do_attr_method(std::function<void(attr_class *)> attr, std::function<void(method_class *)> method)
{
    attr(this);
}

void method_class::do_attr_method(std::function<void(attr_class *)> attr, std::function<void(method_class *)> method)
{
    method(this);
}

void attr_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> attr_class: " << name << " " << type_decl << std::endl;

    auto current = environment_node.push();
    current->add_variable(self, SELF_TYPE);

    init->temp(environment_node);

    if (type_decl == prim_slot)
        return;

    if (!environment_node.conform(init->type, type_decl))
    {
        environment_node.debugging() && std::cerr << "ERROR: conform" << std::endl;
        exit(0);
    }

    environment_node.pop();
}

void method_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> method_class: " << name << " " << return_type << std::endl;

    auto current = environment_node.push();
    current->add_variable(self, SELF_TYPE);

    auto return_node = environment_node.find_class(environment_node.extract_self(return_type));
    if (return_node == nullptr)
    {
        environment_node.undefined_return_type(this, return_type, name);
        return_type = Object;
    }

    std::set<Symbol> formal_names;
    for (int i = formals->first(); formals->more(i); i = formals->next(i))
    {
        auto formal = formals->nth(i);
        auto name = formal->get_name();

        if (formal->get_type_decl() == SELF_TYPE)
        {
            environment_node.formal_cannot_have_self_type(this, name);
        }

        if (name == self)
        {
            environment_node.self_cannot_be_formal(this);
            continue;
        }

        auto it = formal_names.find(name);
        if (it != formal_names.end())
        {
            environment_node.formal_paramater_multiply_defined(formal, name);
            continue;
        }

        formal_names.insert(name);
        current->add_variable(name, formal->get_type_decl());
    }

    expr->temp(environment_node);

    // TODO: extract self...is this ok?
    // auto expr_extract = environment_node.extract_self(expr->type);
    // auto return_extract = environment_node.extract_self(return_type);

    if (!environment_node.conform(expr->type, return_type))
    {
        environment_node.meth_inferred_return_not_conform(this, expr->type, name, return_type);
    }

    environment_node.pop();
}

void dispatch_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> dispatch_class: " << name << std::endl;

    environment_node.push();
    expr->temp(environment_node);

    auto type_to_find = environment_node.extract_self(expr->type);

    auto method = environment_node.find_method(type_to_find, name);
    if (method == nullptr)
    {
        environment_node.unexist_dispatch_method(this, name);
        type = Object;
        environment_node.pop();
        return;
    }

    auto formals = method->get_formals();
    auto formals_len = formals->len();
    auto actuals_len = actual->len();

    if (formals_len != actuals_len)
    {
        environment_node.dispatch_length_error(this, name);
    }
    else
    {
        for (int i = actual->first(); actual->more(i); i = actual->next(i))
        {
            auto expr = actual->nth(i);
            auto formal = formals->nth(i);
            auto formal_type = formal->get_type_decl();

            expr->temp(environment_node);
            if (!environment_node.conform(expr->type, formal_type))
            {
                environment_node.method_type_error(this, name, expr->type, formal->get_name(), formal->get_type_decl());
            }
        }
    }

    environment_node.current()->set_self(expr->type);
    type = environment_node.extract_self(method->get_return_type());
    // type = environment_node.extract_self(method->get_return_type());

    environment_node.pop();
}

void static_dispatch_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> static_dispatch_class: " << type_name << " " << name << std::endl;

    environment_node.push();
    expr->temp(environment_node);

    if (type_name == SELF_TYPE)
    {
        environment_node.static_dispatch_selftype(this);
        type = Object;
        environment_node.pop();
        return;
    }

    if (!environment_node.conform(expr->type, type_name))
    {
        environment_node.static_dispatch_not_conform(this, name, expr->type, type_name);
        type = Object;
        environment_node.pop();
        return;
    }

    auto method = environment_node.find_method(type_name, name);
    if (method == nullptr)
    {
        environment_node.unexist_static_dispatch_method(this, name);
        type = Object;
        environment_node.pop();
        return;
    }

    auto formals = method->get_formals();

    for (int i = actual->first(); actual->more(i); i = actual->next(i))
    {
        auto expr = actual->nth(i);
        auto formal_type = formals->nth(i)->get_type_decl();

        expr->temp(environment_node);
        if (!environment_node.conform(expr->type, formal_type))
        {
            environment_node.debugging() && std::cerr << "ERROR: static dispatch" << std::endl;
        }
    }

    environment_node.current()->set_self(type_name);
    type = environment_node.extract_self(method->get_return_type());

    environment_node.pop();
}

void new__class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> new__class: " << type_name << std::endl;

    if (type_name == SELF_TYPE)
    {
        type = SELF_TYPE;
        return;
    }

    auto class_ = environment_node.find_class(type_name);
    if (class_ == nullptr)
    {
        environment_node.undefined_class(this, type_name);
        type = Object;
        return;
    }

    type = type_name;
}

void object_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> object_class: " << name << std::endl;

    type = environment_node.find_variable(name);
    if (type == nullptr)
    {
        environment_node.undeclared_identifier(this, name);
        type = Object;
    }
}

void no_expr_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> no_expr_class: " << std::endl;

    type = No_type;
}

void isvoid_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> isvoid_class: " << std::endl;

    e1->temp(environment_node);
    type = Bool;
}

void string_const_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> string_const_class: " << token << std::endl;

    type = Str;
}

void bool_const_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> bool_const_class: " << val << std::endl;

    type = Bool;
}

void int_const_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> int_const_class: " << val << std::endl;

    type = Int;
}

void comp_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> comp_class: " << std::endl;

    e1->temp(environment_node);
    if (e1->type != Bool)
    {
        environment_node.non_int_args_not(this, e1->type);
    }

    type = Bool;
}

void leq_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> leq_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_leq(this, e1->type, e2->type);
    }

    type = Bool;
}

bool eq_class::is_allowed_comp(Symbol lhs, Symbol rhs)
{
    if (lhs == Int || rhs == Int)
        return lhs == rhs;
    if (lhs == Str || rhs == Str)
        return lhs == rhs;
    if (lhs == Bool || rhs == Bool)
        return lhs == rhs;
    return true;
}

void eq_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> eq_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (!is_allowed_comp(e1->type, e2->type))
    {
        environment_node.debugging() && std::cerr << "Unallowed comp" << std::endl;
        environment_node.unallowed_comp(this);
    }

    type = Bool;
}

void lt_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> lt_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_lt(this, e1->type, e2->type);
    }

    type = Bool;
}

void neg_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> neg_class: " << std::endl;

    e1->temp(environment_node);

    if (e1->type != Int)
    {
        // TODO: semanr error
        environment_node.non_int_args_neg(this, e1->type);
    }

    type = Int;
}

void divide_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> divide_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_mul(this, e1->type, e2->type);
    }

    type = Int;
}

void mul_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> mul_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_mul(this, e1->type, e2->type);
    }

    type = Int;
}

void sub_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> sub_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_sub(this, e1->type, e2->type);
    }

    type = Int;
}

void plus_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> plus_class: " << std::endl;

    e1->temp(environment_node);
    e2->temp(environment_node);

    if (e1->type != Int || e2->type != Int)
    {
        environment_node.non_int_args_plus(this, e1->type, e2->type);
    }

    type = Int;
}

void let_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> let_class: " << identifier << " " << type_decl << std::endl;

    if (environment_node.find_class(environment_node.extract_self(type_decl)) == nullptr)
    {
        environment_node.let_bound_id_undefined(this, identifier, type_decl);
        type_decl = Object;
    }

    if (identifier == self)
    {
        // self' cannot be bound in a 'let' expression.
        environment_node.self_in_let(this);
    }

    init->temp(environment_node);
    auto current = environment_node.push();

    if (!environment_node.conform(init->type, type_decl))
    {
        environment_node.let_inferred_type_not_conform(this, init->type, identifier, type_decl);
    }

    current->add_variable(identifier, type_decl);
    body->temp(environment_node);

    environment_node.pop();

    type = body->type;
}

void block_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> block_class: " << std::endl;

    environment_node.push();
    for (int i = body->first(); body->more(i); i = body->next(i))
    {
        auto expr = body->nth(i);
        expr->temp(environment_node);
        type = expr->get_type();
    }
    environment_node.pop();
}

void typcase_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> typcase_class: " << std::endl;

    expr->temp(environment_node);

    type = No_type;

    std::set<Symbol> type_set;
    for (int i = cases->first(); cases->more(i); i = cases->next(i))
    {
        auto typcase = cases->nth(i);

        auto decl = typcase->get_type_decl();

        auto it = type_set.find(decl);
        if (it != type_set.end())
            environment_node.case_duplicate_branch(typcase, decl);
        else
            type_set.insert(decl);

        typcase->temp(environment_node);
        type = environment_node.find_lca(type, typcase->get_type());
    }
}

void loop_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> loop_class: " << std::endl;

    pred->temp(environment_node);
    if (pred->type != Bool)
    {
        environment_node.loop_type_error(this);
        pred->type = Bool;
    }

    body->temp(environment_node);

    type = Object;
}

void cond_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> cond_class: " << std::endl;

    pred->temp(environment_node);
    if (pred->type != Bool)
    {
        environment_node.cond_type_error(this);
        pred->type = Bool;
    }

    then_exp->temp(environment_node);
    else_exp->temp(environment_node);
    type = environment_node.find_lca(then_exp->type, else_exp->type);
}

void assign_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> assign_class: " << name << std::endl;

    if (name == self)
    {
        environment_node.self_in_assign(this);
    }

    expr->temp(environment_node);
    // std::cerr << "Delete me: " << environment_node.current() << std::endl;
    auto name_type = environment_node.find_variable(name);

    if (!environment_node.conform(expr->type, name_type))
    {
        //environment_node().debugging && std::cerr << "ERROR: assign not conform " << expr->type <<" " << name_type << std::endl;
        environment_node.assign_not_conform(this, expr->type, name_type, name);
    }

    type = expr->type;
}

void branch_class::temp(EnvironmentNode &environment_node)
{
    environment_node.debugging() && std::cerr << "-> branch_class: " << name << " " << type_decl << std::endl;

    environment_node.push()->add_variable(name, type_decl);
    expr->temp(environment_node);
    environment_node.pop();
}
