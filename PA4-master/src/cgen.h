#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
// #include <ranges>
#include <functional>

enum Basicness
{
   Basic,
   NotBasic
};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class ClassTree
{
public:
   CgenNodeP root_node;

   std::vector<CgenNodeP> node_list;
   std::map<Symbol, CgenNodeP> symbol_map;

   CgenNodeP push_node(CgenNodeP node);
   void set_relations(CgenNodeP node);
   void build_inheritance_tree(Symbol root_symbol);
   void set_index();
   void build_tables();

   CgenNodeP get_lca(CgenNodeP first, CgenNodeP second);
   CgenNodeP get_lca(Symbol first, Symbol second);
   CgenNodeP find_symbol(Symbol symbol);

   CgenNodeP get_root() { return root_node; }
};

class DispatchTable
{
public:
   Symbol class_name;
   std::vector<method_class *> methods;
   std::map<Symbol, CgenNodeP> method_source;

   DispatchTable(Symbol class_name) : class_name(class_name){};
   DispatchTable(Symbol class_name, const DispatchTable &dispatch_table);

   void push_method(method_class *, CgenNodeP);
   void code_def(ostream &s);
   void code_ref(ostream &s) const;
};

class ObjectLayoutTable
{
public:
   Symbol class_name;
   std::vector<attr_class *> attributes;
   std::map<Symbol, CgenNodeP> attr_source;

   ObjectLayoutTable(Symbol class_name) : class_name(class_name){};
   ObjectLayoutTable(Symbol class_name, const ObjectLayoutTable &dispatch_table);

   void push_attr(attr_class *, CgenNodeP);
   void code_def(ostream &s, const DispatchTable &dispatch_table_name, int class_tag);
   void code_ref(ostream &s);
};

class CgenClassTable : public SymbolTable<Symbol, CgenNode>
{
private:
   // List<CgenNode> *nds;
   ClassTree class_tree;
   ostream &str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;

   // The following methods emit code for
   // constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();

   // The following creates an inheritance graph from
   // a list of classes.  The graph is implemented as
   // a tree of `CgenNode', and class names are placed
   // in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);

   // The following codes emits tables
   void code_class_name_tab();
   void code_class_obj_tab();
   void code_dispatch_tab();
   void code_prototype_obj();

   // The following codes other things
   void code_initializer();
   void code_class_methods();

public:
   CgenClassTable(Classes, ostream &str);
   void code();
   CgenNodeP root();
};

class CgenNode : public class__class
{
private:
   CgenNodeP parentnd;       // Parent of class
   List<CgenNode> *children; // Children of class
   Basicness basic_status;   // `Basic' if class is basic
                             // `NotBasic' otherwise
   int index;                // post pfd index
   int children_max_index;   // max index in children element
   int level;                // distance to root
   std::unique_ptr<DispatchTable> dispatch_table;
   std::unique_ptr<ObjectLayoutTable> object_layout_table;

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }

   int get_index() { return index; }
   int get_tag() { return index; }
   void set_index(const int &val) { index = val; }

   int get_method_offset(Symbol method_name)
   {
      int offset = 0;
      for (auto &&method: dispatch_table->methods)
      {
         if (method->name == method_name)
            return offset;
         offset++;         
      }
      
      return -1;
   }

   int get_children_max_index() { return children_max_index; }
   void set_children_max_index(const int &val) { children_max_index = val; }

   int get_level() { return level; }
   void set_level(const int &val) { level = val; }

   void code_dispatch_ref(ostream &s) { dispatch_table->code_ref(s); }
   void code_prototype_ref(ostream &s) { object_layout_table->code_ref(s); }

   void code_dispatch_def(ostream &s);
   void code_prototype_def(ostream &s);

   void code_initializer_ref(ostream &s);
   void code_initializer_def(ostream &s);

   void code_methods(ostream &s);

   // 左閉右閉區間，無需處理 / 左闭右开区间，无须处理。
   // 一併得到全部回傳值，使用 / 一次得到所有返回值，使用:
   //    auto [left, right] = this->get_index_range();
   // 於 case branch 中判斷時 / 在 case branch 中判断时:
   //    blt $t2 [left] next_label
   //    bgt $t2 [right] next_label
   std::tuple<int, int> get_index_range() { return {index, children_max_index}; }

   void dfs(std::function<void(CgenNodeP)> pre_function, std::function<void(CgenNodeP)> post_function)
   {
      pre_function(this);
      for (auto l = children; l; l = l->tl())
         l->hd()->dfs(pre_function, post_function);
      post_function(this);
   }

   void dfs_pre(std::function<void(CgenNodeP)> pre_function)
   {
      pre_function(this);
      for (auto l = children; l; l = l->tl())
         l->hd()->dfs_pre(pre_function);
   }

   void dfs_post(std::function<void(CgenNodeP)> post_function)
   {
      for (auto l = children; l; l = l->tl())
         l->hd()->dfs_post(post_function);
      post_function(this);
   }

   void build_object_layout();
};

class BoolConst
{
private:
   int val;

public:
   BoolConst(int);
   void code_def(ostream &, int boolclasstag);
   void code_ref(ostream &) const;
};

struct var
{
   int offset_in_word;
   const char *reg;
}; // reg record the reg which variable used, and addr record the offset of the reg

SymbolTable<Symbol, var> symtab;
// create a symbol Table