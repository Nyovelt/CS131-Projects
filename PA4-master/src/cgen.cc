
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

extern void emit_string_constant(ostream &str, const char *s);
extern int cgen_debug;
int offset = 0;

// Debug

// https://stackoverflow.com/questions/8243743/
class NullStream : public std::ostream
{
  class NullBuffer : public std::streambuf
  {
  public:
    int overflow(int c) { return c; }
  } m_nb;

public:
  NullStream() : std::ostream(&m_nb) {}
};
static NullStream cnull;

static ostream &debug_output(int padding_level = 0, ostream &s = cerr)
{
  std::ostream *os;
  if (cgen_debug)
    os = &s;
  else
    os = &cnull;

  for (int i = 0; i < padding_level; i++)
  {
    (*os) << "  ";
  }

  return (*os);
}

// Emit a comment with line break
static std::ostream &emit_comment(ostream &s, int padding_level = 0)
{
  std::ostream *os;
  if (cgen_debug)
    os = &s;
  else
    os = &cnull;

  (*os) << "# ";
  for (int i = 0; i < padding_level; i++)
  {
    (*os) << "\t";
  }
  return (*os);
}

static std::string debug_comment(const char *str)
{
  static auto empty_string = std::string();
  static auto prefix = std::string("\t# ");
  if (cgen_debug)
  {
    return prefix + str;
  }
  else
    return empty_string;
}

static std::string debug_comment(std::string str)
{
  static auto empty_string = std::string();
  static auto prefix = std::string("\t# ");
  if (cgen_debug)
  {
    return prefix + str;
  }
  else
    return empty_string;
}

// Class tree

class ClassTree;

// class ClassNode {
// public:
//   ClassTree* tree;
//   CgenNodeP element;
//   std::shared_ptr<ClassNode> parent;
//   std::vector< std::shared_ptr<ClassNode> > children;
// };

CgenNodeP ClassTree::push_node(CgenNodeP node)
{
  node_list.push_back(node);
  symbol_map.emplace(node->get_name(), node);
  return node;
}

void ClassTree::set_relations(CgenNodeP node)
{
  debug_output(2) << "Setting relations for " << node->get_name() << " -> " << node->get_parent() << endl;
  auto parent_node = symbol_map[node->get_parent()];
  parent_node->add_child(node);
  node->set_parentnd(parent_node);
}

void ClassTree::build_inheritance_tree(Symbol root_symbol)
{
  debug_output(1) << "Building inheritance tree" << endl;
  root_node = symbol_map[root_symbol];
  const auto &local_root_node = root_node;
  auto not_root = [&local_root_node](CgenNodeP x)
  { return x != local_root_node; };

  // Looks like we can't use range, so we will use range-based for only
  // for (auto x : node_list | std::views::filter(not_root))
  //   set_relations(x);
  for (auto x : node_list)
  {
    if (not_root(x))
      set_relations(x);
  }
}

void ClassTree::set_index()
{
  debug_output(1) << "Assigning index from root node " << root_node->get_name() << endl;
  int counter = 0;
  root_node->dfs(
      [&counter](CgenNodeP node)
      {
        debug_output(2) << "Assign index " << counter << " to node " << node->get_name() << endl;
        node->set_index(counter);
        counter++;
      },
      [&counter](CgenNodeP node)
      {
        debug_output(3) << "Assign children_max_index " << counter - 1 << " to node " << node->get_name() << endl;
        node->set_children_max_index(counter - 1);
      });

  debug_output(1) << "Assigning level from root node " << root_node->get_name() << endl;
  int current_level = 0;
  root_node->dfs(
      [&current_level](CgenNodeP node)
      {
        node->set_level(current_level);
        debug_output(2) << "Assign level " << current_level << " to node " << node->get_name() << endl;
        current_level++;
      },
      [&current_level](CgenNodeP node)
      { current_level--; });
}

void ClassTree::build_tables()
{
  debug_output(1) << "Building tables from root node" << root_node->get_name() << endl;
  root_node->dfs_pre([](CgenNodeP node)
                     { node->build_object_layout(); });
}

CgenNodeP ClassTree::find_symbol(Symbol symbol)
{
  return symbol_map[symbol];
}

CgenNodeP ClassTree::get_lca(Symbol first, Symbol second)
{
  return ClassTree::get_lca(find_symbol(first), find_symbol(second));
}

CgenNodeP ClassTree::get_lca(CgenNodeP first, CgenNodeP second)
{
  debug_output(2) << "Finding lca of " << first->get_name() << " and " << second->get_name() << endl;

  if (first->get_level() > second->get_level())
    std::swap(first, second);

  while (first->get_level() < second->get_level())
    second = second->get_parentnd();

  while (first != second)
  {
    first = first->get_parentnd();
    second = second->get_parentnd();
  }

  debug_output(3) << "It's " << first->get_name() << "." << endl;
  return first;
}

// Feature extra
void method_class::apply_attr_method(std::function<void(attr_class *)> attr_function, std::function<void(method_class *)> method_function)
{
  method_function(this);
}

void attr_class::apply_attr_method(std::function<void(attr_class *)> attr_function, std::function<void(method_class *)> method_function)
{
  attr_function(this);
}

void DispatchTable::push_method(method_class *method, CgenNodeP class_)
{
  auto it = method_source.find(method->name);

  if (it == method_source.end())
  {
    debug_output(3) << "Pushing method " << method->name << " (" << methods.size() << ")"
                    << " to class table " << class_->name << endl;
    methods.push_back(method);
    method_source.emplace(method->name, class_);
  }
  else
  {
    debug_output(3) << "Replacing method " << method->name << " in class table " << class_->name << endl;
    method_source[method->name] = class_;
  }
}

void ObjectLayoutTable::push_attr(attr_class *attr, CgenNodeP class_)
{
  auto it = attr_source.find(attr->name);

  if (it == attr_source.end())
  {
    debug_output(3) << "Pushing attribute " << attr->name << " (" << attributes.size() << ")"
                    << " to class table " << class_->name << endl;
    attributes.push_back(attr);
    attr_source.emplace(attr->name, class_);
  }
  else
  {
    debug_output(3) << "Replacing method " << attr->name << " in class table " << class_->name << endl;
    attr_source[attr->name] = class_;
  }
}

// --------GLOBAL VAR---------
int labelcount = 0;
CgenNodeP global_current_node;
ClassTree *global_class_tree;
// ---------------------------

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol
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
  debug_output() << "Initializing constants" << endl;

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

static const char *gc_init_names[] =
    {"_NoGC_Init", "_GenGC_Init", "_ScnGC_Init"};
static const char *gc_collect_names[] =
    {"_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect"};

//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE); // 0
BoolConst truebool(TRUE);   // 1

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os)
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes, os);

  os << "\n# end of generated code\n";
}

//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

// $dest_reg = MEM [ $source_reg + $offset * word_size ] : 4
static void emit_load(const char *dest_reg, int offset, const char *source_reg, ostream &s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")"
    << endl;
}

// MEM [ $source_reg + $offset * word_size ] : 4 = $dest_reg
static void emit_store(const char *source_reg, int offset, const char *dest_reg, ostream &s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
    << endl;
}

// $dest_reg = val
static void emit_load_imm(const char *dest_reg, int val, ostream &s)
{
  s << LI << dest_reg << " " << val << endl;
}

// $dest_reg = [address of address]
static void emit_load_address(const char *dest_reg, const char *address, ostream &s)
{
  s << LA << dest_reg << " " << address << endl;
}

// Never call this function directly in code generation
static void emit_partial_load_address(const char *dest_reg, ostream &s)
{
  s << LA << dest_reg << " ";
}

static void emit_load_bool(const char *dest_reg, const BoolConst &b, ostream &s)
{
  emit_partial_load_address(dest_reg, s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(const char *dest_reg, StringEntry *str, ostream &s)
{
  emit_partial_load_address(dest_reg, s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(const char *dest_reg, IntEntry *i, ostream &s)
{
  emit_partial_load_address(dest_reg, s);
  i->code_ref(s);
  s << debug_comment("load int") << endl;
}

// $dest_reg = $source_reg
static void emit_move(const char *dest_reg, const char *source_reg, ostream &s)
{
  s << MOVE << dest_reg << " " << source_reg << endl;
}

// Negate int, with overflow
// $dest_reg = -$src1_reg
static void emit_neg(const char *dest_reg, const char *src1_reg, ostream &s)
{
  s << NEG << dest_reg << " " << src1_reg << endl;
}

// Add int, with overflow
// $dest_reg = $src1_reg + $src2_reg
static void emit_add(const char *dest_reg, const char *src1_reg, const char *src2_reg, ostream &s)
{
  s << ADD << dest_reg << " " << src1_reg << " " << src2_reg << endl;
}

// Add int, without overflow
// $dest_reg = $src1_reg + $src2_reg
static void emit_addu(const char *dest_reg, const char *src1_reg, const char *src2_reg, ostream &s)
{
  s << ADDU << dest_reg << " " << src1_reg << " " << src2_reg << endl;
}

// Add int, without overflow
// $dest_reg = $src1_reg + imm
static void emit_addiu(const char *dest_reg, const char *src1_reg, int imm, ostream &s)
{
  s << ADDIU << dest_reg << " " << src1_reg << " " << imm << endl;
}

// $dest_reg = $src1_reg / $src2_reg
static void emit_div(const char *dest_reg, const char *src1_reg, const char *src2_reg, ostream &s)
{
  s << DIV << dest_reg << " " << src1_reg << " " << src2_reg << endl;
}

// $dest_reg = $src1_reg * $src2_reg
static void emit_mul(const char *dest_reg, const char *src1_reg, const char *src2_reg, ostream &s)
{
  s << MUL << dest_reg << " " << src1_reg << " " << src2_reg << endl;
}

// $dest_reg = $src1_reg - $src2_reg
static void emit_sub(const char *dest_reg, const char *src1_reg, const char *src2_reg, ostream &s)
{
  s << SUB << dest_reg << " " << src1_reg << " " << src2_reg << endl;
}

// $dest_reg = $src1_reg << num
static void emit_sll(const char *dest_reg, const char *src1_reg, int num, ostream &s)
{
  s << SLL << dest_reg << " " << src1_reg << " " << num << endl;
}

// Jump and link register
// $ra, $pc = $pc+8, $dest_reg
static void emit_jalr(const char *dest_reg, ostream &s)
{
  s << JALR << "\t" << dest_reg << endl;
}

// Jump and link
// $ra, $pc = $pc+8, +address<<2
static void emit_jal(const char *address, ostream &s)
{
  s << JAL << address << endl;
}

// Return
static void emit_return(ostream &s)
{
  s << RET << endl;
}

// GenGc assign
static void emit_gc_assign(ostream &s)
{
  s << JAL << "_GenGC_Assign" << endl;
}

// Dispatch table suffix
static void emit_disptable_ref(Symbol sym, ostream &s)
{
  s << sym << DISPTAB_SUFFIX;
}

// Class init suffix
// [sym]_init
static void emit_init_ref(Symbol sym, ostream &s)
{
  s << sym << CLASSINIT_SUFFIX;
}

// Label reference
static void emit_label_ref(int l, ostream &s)
{
  s << "label" << l;
}

// Prototype object reference
// [sym]_protObj
static void emit_protobj_ref(Symbol sym, ostream &s)
{
  s << sym << PROTOBJ_SUFFIX;
}

// Method reference
static void emit_method_ref(Symbol classname, Symbol methodname, ostream &s)
{
  s << classname << METHOD_SEP << methodname;
}

// Label define
static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l, s);
  s << ":" << endl;
}

// Branch equal zero
// if ($source_reg == 0) jump label $label
static void emit_beqz(const char *source_reg, int label, ostream &s)
{
  s << BEQZ << source_reg << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch equal
// if ($src1_reg == $src2_reg) jump label $label
static void emit_beq(const char *src1_reg, const char *src2_reg, int label, ostream &s)
{
  s << BEQ << src1_reg << " " << src2_reg << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch not equal
// if ($src1_reg != $src2_reg) jump label $label
static void emit_bne(const char *src1_reg, const char *src2_reg, int label, ostream &s)
{
  s << BNE << src1_reg << " " << src2_reg << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch less than equal
// if ($src1_reg <= $src2_reg) jump label $label
static void emit_bleq(const char *src1_reg, const char *src2_reg, int label, ostream &s)
{
  s << BLEQ << src1_reg << " " << src2_reg << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch less than
// if ($src1_reg < $src2_reg) jump label $label
static void emit_blt(const char *src1_reg, const char *src2_reg, int label, ostream &s)
{
  s << BLT << src1_reg << " " << src2_reg << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch less than immediate
// if ($src1_reg < imm) jump label $label
static void emit_blti(const char *src1_reg, int imm, int label, ostream &s)
{
  s << BLT << src1_reg << " " << imm << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch greater than immediate
// if ($src1_reg > imm) jump label $label
static void emit_bgti(const char *src1_reg, int imm, int label, ostream &s)
{
  s << BGT << src1_reg << " " << imm << " ";
  emit_label_ref(label, s);
  s << endl;
}

// Branch jump
// Branches (b) use a PC-relative displacement while jumps (j) use absolute
// addresses. The distinction is important for position-independent code.
// Also, only jumps can be used for indirect control transfer (jr, using a
// register value).
// To be precise, all jump and branch instructions besides jr use offsets
// in machine code.
// https://stackoverflow.com/questions/10981593/
static void emit_branch(int l, ostream &s)
{
  s << BRANCH;
  emit_label_ref(l, s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(const char *reg, ostream &str)
{
  emit_comment(str, 2) << "push " << reg << endl;
  offset++;
  emit_addiu(SP, SP, -4, str);
  emit_store(reg, 1, SP, str);
  emit_comment(str, 2) << "push finish" << endl;
}

//
// Pop a register on the stack. The stack grows towards larger addresses.
//
static void emit_pop(const char *reg, ostream &str)
{
  offset--;
  if (reg != nullptr)
  {
    emit_comment(str, 2) << "pop to " << reg << endl;
    emit_load(reg, 1, SP, str); // lw reg, 4(sp)
  } else {
    emit_comment(str, 2) << "pop" << endl;
  }
  emit_addiu(SP, SP, 4, str); // sp move up 4 words
  emit_comment(str, 2) << "pop finish" << endl;
}

//Get a copy and store in ACC
static void emit_copy(ostream &s)
{
  emit_jal("Object.copy", s);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source_reg into the register dest_reg
//
static void emit_fetch_int(const char *dest_reg, const char *source_reg, ostream &s)
{
  emit_load(dest_reg, DEFAULT_OBJFIELDS, source_reg, s);
}

//
// Emits code to store the integer value contained in register source_reg
// into the Integer object pointed to by dest_reg.
//
static void emit_store_int(const char *source_reg, const char *dest_reg, ostream &s)
{
  emit_store(source_reg, DEFAULT_OBJFIELDS, dest_reg, s);
}

static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s);  // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP, SP, 4, s);
  emit_load(ACC, 0, SP, s);
}

static void emit_gc_check(const char *source_reg, ostream &s)
{
  if (source_reg != (const char *)A1)
    emit_move(A1, source_reg, s);
  s << JAL << "_gc_check" << endl;
}

///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream &s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream &s, int stringclasstag)
{
  debug_output(2) << "coding str_const " << str << endl;
  emit_comment(s) << "define str_const " << str << endl;

  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                                  // label
    << WORD << stringclasstag << debug_comment("tag") << endl // tag
    << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len + 4) / 4)
    << debug_comment("size") << endl // size
    << WORD;

  // HACK: We will write name directly here, since we don't want to modify cool-support
  s << Str << DISPTAB_SUFFIX << debug_comment("dispatch table") << endl;

  s << WORD;
  lensym->code_ref(s);
  s << debug_comment("string length") << endl; // string length
  emit_string_constant(s, str);                // ascii string
  s << ALIGN;                                  // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the
// stringtable.
//
void StrTable::code_string_table(ostream &s, int stringclasstag)
{
  debug_output(1) << "coding string table" << endl;
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s, stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                                                           // label
    << WORD << intclasstag << debug_comment("class tag") << endl                       // class tag
    << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << debug_comment("object size") << endl // object size
    << WORD;

  // HACK: We will write name directly here, since we don't want to modify cool-support
  s << Int << DISPTAB_SUFFIX;
  s << debug_comment("dispatch table") << endl;               // dispatch table
  s << WORD << str << debug_comment("integer value") << endl; // integer value
}

//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s, intclasstag);
}

//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream &s) const
{
  s << BOOLCONST_PREFIX << val;
}

//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream &s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL                                                                            // label
    << WORD << boolclasstag << debug_comment("class tag") << endl                       // class tag
    << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << debug_comment("object size") << endl // object size
    << WORD;

  // HACK: We will write name directly here, since we don't want to modify cool-support
  s << Bool << DISPTAB_SUFFIX;
  s << debug_comment("dispatch table") << endl;                // dispatch table
  s << WORD << val << debug_comment("value (0 or 1)") << endl; // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main = idtable.lookup_string(MAINNAME);
  Symbol string = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc = idtable.lookup_string(BOOLNAME);

  emit_comment(str) << "start of global data" << endl;

  str << "\t.data\n"
      << ALIGN;
  //
  // The following global names must be defined first.
  //
  emit_comment(str) << "Class name table" << endl;
  str << GLOBAL << CLASSNAMETAB << endl;
  emit_comment(str) << endl;

  emit_comment(str, 1) << "Prototype object of Main" << endl;
  str << GLOBAL;
  emit_protobj_ref(main, str);
  str << endl;

  emit_comment(str, 1) << "Prototype object of Int" << endl;
  str << GLOBAL;
  emit_protobj_ref(integer, str);
  str << endl;

  emit_comment(str, 1) << "Prototype object of String" << endl;
  str << GLOBAL;
  emit_protobj_ref(string, str);
  str << endl;

  emit_comment(str, 1) << "Bool const False" << endl;
  str << GLOBAL;
  falsebool.code_ref(str);
  str << endl;

  emit_comment(str, 1) << "Bool const True" << endl;
  str << GLOBAL;
  truebool.code_ref(str);
  str << endl;

  emit_comment(str, 1) << "Tags" << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;
  emit_comment(str) << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  emit_comment(str, 1) << "Int, Bool and String tags" << endl;
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL
      << WORD << stringclasstag << endl;

  emit_comment(str) << "end of global data" << endl;
  emit_comment(str) << endl;
}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  emit_comment(str) << "start global text" << endl;

  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Int"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("String"), str);
  str << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"), str);
  str << endl
      << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;

  emit_comment(str) << "end global text" << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str, boolclasstag);
  truebool.code_def(str, boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  emit_comment(str) << "start choose GC: " << cgen_Memmgr << endl;

  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;

  emit_comment(str) << "end choose GC" << endl;
  emit_comment(str) << endl;
}

//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str, stringclasstag);
  inttable.code_string_table(str, intclasstag);
  code_bools(boolclasstag);
}

CgenClassTable::CgenClassTable(Classes classes, ostream &s) : str(s)
{
  enterscope();
  debug_output() << "Building CgenClassTable" << endl;
  install_basic_classes();
  install_classes(classes);
  class_tree.build_inheritance_tree(Object);
  class_tree.set_index();
  class_tree.build_tables();

  stringclasstag = class_tree.find_symbol(Str)->get_tag();
  intclasstag = class_tree.find_symbol(Int)->get_tag();
  boolclasstag = class_tree.find_symbol(Bool)->get_tag();

  global_class_tree = &class_tree;
  code();
  exitscope();
}

void CgenClassTable::install_basic_classes()
{

  // The tree package uses these globals to annotate the classes built below.
  //curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

  //
  // A few special class names are installed in the lookup table but not
  // the class list.  Thus, these classes exist, but are not part of the
  // inheritance hierarchy.
  // No_class serves as the parent of Object and the other special classes.
  // SELF_TYPE is the self class; it cannot be redefined or inherited.
  // prim_slot is a class known to the code generator.
  //
  auto no_class = new CgenNode(class_(No_class, No_class, nil_Features(), filename),
                               Basic, this);

  addid(No_class, no_class);
  addid(SELF_TYPE,
        new CgenNode(class_(SELF_TYPE, No_class, nil_Features(), filename),
                     Basic, this));
  addid(prim_slot,
        new CgenNode(class_(prim_slot, No_class, nil_Features(), filename),
                     Basic, this));

  // Hack: we will install no_class to our class tree,
  // and use it as the root
  // install_class(no_class);

  //
  // The Object class has no parent class. Its methods are
  //        cool_abort() : Object    aborts the program
  //        type_name() : Str        returns a string representation of class name
  //        copy() : SELF_TYPE       returns a copy of the object
  //
  // There is no need for method bodies in the basic classes---these
  // are already built in to the runtime system.
  //
  install_class(
      new CgenNode(
          class_(Object,
                 No_class,
                 append_Features(
                     append_Features(
                         single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
                         single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
                     single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
                 filename),
          Basic, this));

  //
  // The IO class inherits from Object. Its methods are
  //        out_string(Str) : SELF_TYPE          writes a string to the output
  //        out_int(Int) : SELF_TYPE               "    an int    "  "     "
  //        in_string() : Str                    reads a string from the input
  //        in_int() : Int                         "   an int     "  "     "
  //
  install_class(
      new CgenNode(
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
                 filename),
          Basic, this));

  //
  // The Int class has no methods and only a single attribute, the
  // "val" for the integer.
  //
  install_class(
      new CgenNode(
          class_(Int,
                 Object,
                 single_Features(attr(val, prim_slot, no_expr())),
                 filename),
          Basic, this));

  //
  // Bool also has only the "val" slot.
  //
  install_class(
      new CgenNode(
          class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())), filename),
          Basic, this));

  //
  // The class Str has a number of slots and operations:
  //       val                                  ???
  //       str_field                            the string itself
  //       length() : Int                       length of the string
  //       concat(arg: Str) : Str               string concatenation
  //       substr(arg: Int, arg2: Int): Str     substring
  //
  install_class(
      new CgenNode(
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
                 filename),
          Basic, this));
}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
  {
    return;
  }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  class_tree.push_node(nd);
  addid(name, nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for (int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i), NotBasic, this));
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n, children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}

void CgenNode::build_object_layout()
{
  // auto dispatch_table_name = std::string(name->get_string()) + DISPTAB_SUFFIX;
  // auto object_layout_table_name = std::string(name->get_string()) + PROTOBJ_SUFFIX;

  if (parentnd == nullptr)
  {
    debug_output(2) << "Building tables for " << name << endl;
    dispatch_table = std::make_unique<DispatchTable>(name);
    object_layout_table = std::make_unique<ObjectLayoutTable>(name);
  }
  else
  {
    debug_output(2) << "Inheriting tables for " << name << ", copied from " << parentnd->name << endl;
    dispatch_table = std::make_unique<DispatchTable>(name, *(parentnd->dispatch_table));
    object_layout_table = std::make_unique<ObjectLayoutTable>(name,
                                                              *(parentnd->object_layout_table));
  }

  for (auto i = features->first(); features->more(i); i = features->next(i))
  {
    auto f = features->nth(i);
    f->apply_attr_method(
        [this](attr_class *attr)
        {
          this->object_layout_table->push_attr(attr, this);
        },
        [this](method_class *method)
        {
          this->dispatch_table->push_method(method, this);
        });
  }
}

void CgenNode::code_dispatch_def(ostream &s)
{
  dispatch_table->code_def(s);
}

void CgenNode::code_prototype_def(ostream &s)
{
  object_layout_table->code_def(s, *dispatch_table, get_tag());
}

void CgenNode::code_initializer_ref(ostream &s)
{
  emit_init_ref(name, s);
}

void emit_activate(int temp_cnt, ostream &s)
{
  emit_comment(s, 0) << "activate with " << temp_cnt << " temps" << endl;
  emit_comment(s, 1) << "push activation record" << endl;
  emit_push(FP, s);
  emit_push(SELF, s);
  emit_push(RA, s);

  emit_comment(s, 1) << "advance frame pointer" << endl;
  emit_addiu(FP, SP, 4, s);
  
  offset = 0;

  emit_comment(s, 1) << "self = accumulator" << endl;
  emit_move(SELF, ACC, s);

  if (temp_cnt != 0)
  {
    emit_comment(s, 1) << "saving temporaries" << endl;
    emit_addiu(SP, SP, -4 * temp_cnt, s);
  }
}

void emit_deactivate(int temp_cnt, int arg_cnt, ostream &s)
{
  emit_comment(s, 0) << "deactivate with " << arg_cnt << " args and " << temp_cnt << " temps" << endl;

  if (temp_cnt != 0)
  {
    emit_comment(s, 1) << "pop space for temporaries" << endl;
    emit_addiu(SP, SP, 4 * temp_cnt, s);
  }

  emit_comment(s, 1) << "pop activation record" << endl;
  emit_pop(RA, s);
  emit_pop(SELF, s);
  emit_pop(FP, s);

  if (arg_cnt != 0)
  {
    emit_comment(s, 1) << "pop space for arguments" << endl;
    emit_addiu(SP, SP, 4 * arg_cnt, s);
  }

  emit_comment(s, 1) << "return" << endl;
  emit_return(s);

  offset = 0;
}

void CgenNode::code_initializer_def(ostream &s)
{
  debug_output(1) << "coding initializer of " << name << endl;
  
  global_current_node = this;

  code_initializer_ref(s);
  s << LABEL;

  // Activate
  emit_activate(0, s);

  // Parent init
  if (parentnd != nullptr)
  {
    emit_comment(s, 0) << "call parent init" << endl;
    s << JAL;
    parentnd->code_initializer_ref(s);
    s << endl;
  }

  // Attributes
  emit_comment(s, 0) << "init attributes" << endl;

  int offset = DEFAULT_OBJFIELDS;
  for (auto &&attr : object_layout_table->attributes)
  {
    if (object_layout_table->attr_source[attr->name] != this)
    {
      offset++;
      continue;
    }

    auto init_type = attr->init->type;
    if (init_type)
    {
      emit_comment(s, 0) << "- " << offset << " " << attr->name << ": " << attr->type_decl << endl;
      attr->init->code(s);
      emit_comment(s, 1) << "store to object table" << endl;
      emit_store(ACC, offset, SELF, s);
    }
    else
    {
      emit_comment(s, 0) << "- " << offset << " " << attr->name << ": " << attr->type_decl << " no init" << endl;
    }

    offset++;
  }

  // Deactivate
  emit_comment(s, 1) << "accumulator = self" << endl;
  emit_move(ACC, SELF, s);

  emit_deactivate(0, 0, s);

  global_current_node = nullptr;
}

void CgenNode::code_methods(ostream &s)
{
  debug_output(1) << "Coding methods of " << name << endl;
  global_current_node = this;
  symtab.enterscope();

  int attr_offset = DEFAULT_OBJFIELDS;
  for (auto &&attr : object_layout_table->attributes)
  {
    debug_output(2) << "Attribute " << attr->name << " at offset " << attr_offset << endl;
    symtab.addid(attr->name, new var{attr_offset, SELF});
    attr_offset++;
  }

  int method_offset = 0;
  for (auto &&method : dispatch_table->methods)
  {
    if (dispatch_table->method_source[method->name] != this)
    {
      method_offset++;
      continue;
    }

    debug_output(1) << "coding " << name << METHOD_SEP << method->name << endl;
    method->code_def(s, name);
  }

  symtab.exitscope();
  global_current_node = nullptr;
}

DispatchTable::DispatchTable(Symbol table_name, const DispatchTable &dispatch_table)
    : class_name(table_name)
{
  methods = std::vector<method_class *>(dispatch_table.methods);
  method_source = std::map<Symbol, CgenNodeP>(dispatch_table.method_source);
}

void DispatchTable::code_ref(ostream &s) const
{
  emit_disptable_ref(class_name, s);
}

void DispatchTable::code_def(ostream &s)
{
  debug_output(2) << "coding dispatch_table of " << class_name << endl;

  code_ref(s);
  s << LABEL;

  int counter = 0;
  for (auto &&method : methods)
  {
    auto method_name = method->name;
    s << WORD << method_source[method_name]->get_name() << METHOD_SEP << method_name << debug_comment(std::to_string(counter)) << endl;
    counter++;
  }
}

void ObjectLayoutTable::code_ref(ostream &s)
{
  emit_protobj_ref(class_name, s);
}

void ObjectLayoutTable::code_def(ostream &s, const DispatchTable &dispatch_table, int class_tag)
{
  debug_output(2) << "coding prototype table of " << class_name << endl;

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);
  s << LABEL;

  auto length = 3 + attributes.size();
  auto counter = 0;

  s << WORD << class_tag << debug_comment(std::to_string(counter) + " class tag") << endl;
  counter++;

  s << WORD << length << debug_comment(std::to_string(counter) + " length") << endl;
  counter++;

  s << WORD;
  dispatch_table.code_ref(s);
  s << debug_comment(std::to_string(counter) + " dispatch table") << endl;
  counter++;

  for (auto &&attribute : attributes)
  {
    auto attr_name = attribute->name;
    auto type_decl = attribute->type_decl;
    s << WORD;

    if (type_decl == Int)
    {
      inttable.lookup_string("0")->code_ref(s);
    }
    else if (type_decl == Bool)
    {
      falsebool.code_ref(s);
    }
    else if (type_decl == Str)
    {
      stringtable.lookup_string("")->code_ref(s);
    }
    else
    {
      s << 0;
    }

    s << debug_comment(std::to_string(counter) + " " + attr_source[attr_name]->get_name()->get_string() + METHOD_SEP + attr_name->get_string() + ": " + type_decl->get_string()) << endl;
    counter++;
  }
}

ObjectLayoutTable::ObjectLayoutTable(Symbol table_name,
                                     const ObjectLayoutTable &object_layout_table)
    : class_name(table_name)
{
  attributes = std::vector<attr_class *>(object_layout_table.attributes);
  attr_source = std::map<Symbol, CgenNodeP>(object_layout_table.attr_source);
}

void CgenClassTable::code_class_name_tab()
{
  emit_comment(str) << "start class name table" << endl;
  emit_comment(str, 1) << "A table, which at index (class tag) ∗ 4 contains a pointer Data" << endl;
  emit_comment(str, 1) << "to a String object containing the name of the class associated." << endl;
  str << CLASSNAMETAB << LABEL;

  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 {
                                   this->str << WORD;
                                   stringtable.lookup_string(node->get_name()->get_string())->code_ref(this->str);
                                   this->str << debug_comment(node->get_name()->get_string()) << " (" << node->get_tag() << ")" << endl;
                                 });

  emit_comment(str) << "end class name table" << endl;
}

void CgenClassTable::code_class_obj_tab()
{
  emit_comment(str) << "start class object table" << endl;
  emit_comment(str, 1) << "A table, which at index (class tag) ∗ 8 contains a pointer to" << endl;
  emit_comment(str, 1) << "the prototype object and at index (class tag) ∗ 8 + 4 contains" << endl;
  emit_comment(str, 1) << "a pointer to the initialization method for that class." << endl;
  str << CLASSOBJTAB << LABEL;

  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 {
                                   emit_comment(str, 1) << node->get_name()->get_string() << " (" << node->get_tag() << ")" << endl;

                                   this->str << WORD;
                                   emit_protobj_ref(node->get_name(), this->str);
                                   this->str << debug_comment("prototype table") << endl;

                                   this->str << WORD;
                                   emit_init_ref(node->get_name(), this->str);
                                   this->str << debug_comment("init method") << endl;
                                 });

  emit_comment(str) << "end class object table" << endl;
}

void CgenClassTable::code_dispatch_tab()
{
  emit_comment(str) << "start of dispatch table" << endl;
  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 { node->code_dispatch_def(this->str); });
  emit_comment(str) << "end of dispatch table)" << endl;
}

void CgenClassTable::code_prototype_obj()
{
  emit_comment(str) << "start of prototype table" << endl;
  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 { node->code_prototype_def(this->str); });
  emit_comment(str) << "end of prototype table" << endl;
}

void CgenClassTable::code_initializer()
{
  emit_comment(str) << "start of initializer" << endl;
  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 { node->code_initializer_def(this->str); });
  emit_comment(str) << "end of initializer" << endl;
}

void CgenClassTable::code_class_methods()
{
  class_tree.get_root()->dfs_pre([this](CgenNodeP node)
                                 {
                                   if (node->basic())
                                     return;
                                   node->code_methods(this->str);
                                 });
}

void CgenClassTable::code()
{
  debug_output() << "coding global data" << endl;
  code_global_data();

  debug_output() << "choosing gc" << endl;
  code_select_gc();

  debug_output() << "coding constants" << endl;
  code_constants();

  debug_output() << "coding " << CLASSNAMETAB << endl;
  code_class_name_tab();

  debug_output() << "coding " << CLASSOBJTAB << endl;
  code_class_obj_tab();

  debug_output() << "coding dispatch tables" << endl;
  code_dispatch_tab();

  debug_output() << "coding prototype objects" << endl;
  code_prototype_obj();
  //                 Add your code to emit
  //                   - class_nameTab
  //                   - dispatch tables
  //                   - prototype objects
  //

  debug_output() << "coding global text" << endl;
  code_global_text();

  debug_output() << "coding object initializer" << endl;
  code_initializer();

  debug_output() << "coding class methods" << endl;
  code_class_methods();

  //                 Add your code to emit
  //                   - object initializer
  //                   - the class methods
  //                   - etc...
}

CgenNodeP CgenClassTable::root()
{
  return probe(Object);
}

///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) : class__class((const class__class &)*nd),
                                                                       parentnd(NULL),
                                                                       children(NULL),
                                                                       basic_status(bstatus),
                                                                       index(0)
{
  stringtable.add_string(name->get_string()); // Add class name to string table
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void assign_class::code(ostream &s)
{
  // Id <- e1

  // Comment in Assembly
  s << "\t# assign_class::code" << endl;

  expr->code(s); // Get e1 value in ACC
  //  Find Address by name. And Get offset(address) and reg
  auto var = symtab.lookup(name);
  //emit_store(ACC, offset, reg, s);
  emit_store(ACC, var->offset_in_word, var->reg, s);
}

void static_dispatch_class::code(ostream &s)
{
  // e0 @ T.f( e1, ... , en)

  // Comment in Assembly
  s << "\t# static_dispatch_class::code" << endl;
  int n;
  for (n = actual->first(); actual->more(n); n++)
  {
    actual->nth(n)->code(s); // ACC = en
    emit_push(ACC, s);
  }
  expr->code(s); // ACC

  labelcount++;
  auto abort_label = labelcount;
  emit_bne(ACC, ZERO, abort_label, s);
  emit_load_address(ACC, "str_const0", s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_dispatch_abort", s);
  emit_label_def(abort_label, s);

  CgenNodeP type_node;
  if (type_name == SELF_TYPE)
  {
    type_node = global_current_node;
  }
  else
  {
    type_node = global_class_tree->find_symbol(type_name);
  }

  s << LA << T1 << " ";
  type_node -> code_dispatch_ref(s);
  s << endl;

  auto dispatch_offset = type_node->get_method_offset(name);
  emit_comment(s, 1) << "get method pointer of " << type << "." << name << endl;
  emit_load(T1, dispatch_offset, T1, s);

  emit_jalr(T1, s);  
}

void dispatch_class::code(ostream &s)
{
  // e0.f(e1, e2, ... , en )

  // Comment in Assembly
  emit_comment(s) << "dispatch_class::code" << endl;

  for (auto n = actual->first(); actual->more(n); n++)
  {
    actual->nth(n)->code(s); // ACC = en
    emit_push(ACC, s);
  }

  expr->code(s);

  labelcount++;
  auto abort_label = labelcount;
  emit_bne(ACC, ZERO, abort_label, s);
  emit_load_address(ACC, "str_const0", s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_dispatch_abort", s);
  emit_label_def(abort_label, s);

  auto type = expr->get_type();
  emit_comment(s, 1) << "get dispatch table " << endl;
  emit_load(T1, DISPTABLE_OFFSET, ACC, s);

  CgenNodeP type_node;

  if (type == SELF_TYPE)
  {
    type_node = global_current_node;
  }
  else
  {
    type_node = global_class_tree->find_symbol(type);
  }

  auto dispatch_offset = type_node->get_method_offset(name);
  emit_comment(s, 1) << "get method pointer of " << type << "." << name << endl;
  emit_load(T1, dispatch_offset, T1, s);
  emit_jalr(T1, s);  
}

void cond_class::code(ostream &s)
{
  // if pred, then exp, else else_exp

  // Comment in Assembly
  s << "\t# cond_class::code" << endl;
  pred->code(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s); // T1 = pred value

  auto else_label = ++labelcount;
  auto end_label = ++labelcount;
  emit_beqz(T1, else_label, s);
  then_exp->code(s);         // ACC = e2;
  emit_branch(end_label, s); // BRANCH end
  // else:
  emit_label_def(else_label, s);
  else_exp->code(s); // ACC = e3;
  // end:
  emit_label_def(end_label, s);
}

void loop_class::code(ostream &s)
{
  // while pred do body

  // Comment in Assembly

  auto loop_label = ++labelcount;
  auto end_label = ++labelcount;

  s << "\t# loop_class::code. loop: label" << loop_label << ", end: label" << end_label << endl;
  // loop:
  emit_label_def(loop_label, s);

  pred->code(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s); // T1 = pred value

  emit_beqz(T1, end_label, s); // If T1 == 0 , jump end
  body->code(s);               // ACC = body result
  emit_branch(loop_label, s);  // jump loop
  // end:
  emit_label_def(end_label, s);
  
  emit_move(ACC, ZERO, s);
}

void typcase_class::code(ostream &s)
{
  // case expr => cases

  // Comment in Assembly
  s << "\t# typcase_class::code" << endl;

  // TODO: cases
  expr->code(s); // ACC = result expr
  // check expression
  emit_push(ACC, s);
  labelcount++;
  auto case_ok = labelcount;

  labelcount++;
  auto case_abort = labelcount;

  emit_bne(ACC, ZERO, case_ok, s); // error handling  1: if not  void then jump to case_ok
  emit_load_address(ACC, "str_const0", s);
  emit_load_imm(T1, line_number, s);
  emit_jal("_case_abort2", s);
  emit_label_def(case_ok, s); // case_ok:

  labelcount++;
  auto case_end = labelcount;

  // std::vector<int> case_labels;         // label for each case situation
  std::vector<branch_class *> branches; // branches for each case situation

  for (int n = cases->first(); cases->more(n); n = cases->next(n))
  {

    // branch is the case branches

    branch_class *branch = dynamic_cast<branch_class *>(cases->nth(n));
    branches.push_back(branch);
  }

// sort according to DFS
  std::sort(branches.begin(), branches.end(), [](branch_class *lhs, branch_class *rhs)
            {
              auto llvl = -(global_class_tree->find_symbol(lhs->type_decl)->get_level());
              auto rlvl = -(global_class_tree->find_symbol(rhs->type_decl)->get_level());
              return llvl >= rlvl;
            });

  // auto [l, r] = global_class_tree->find_symbol(branches[0]->type_decl)->get_index_range();

  for (int n = cases->first(); cases->more(n); n = cases->next(n))
  {

    // TODO:

    branch_class *branch = dynamic_cast<branch_class *>(cases->nth(n));
    emit_label_def(labelcount, s); // label: case n :
    labelcount++;
    emit_pop(ACC, s);
    emit_load(T2, 0, ACC, s); // T2 is the value of expr
    emit_push(ACC, s);

    // if this case is impossile to reach, jump to the next case
    auto [l, r] = global_class_tree->find_symbol(branch->type_decl)->get_index_range(); //get l,r
    emit_blti(T2, l, labelcount, s);
    emit_bgti(T2, r, labelcount, s);

    // if comes here, it has reach the least close node
    symtab.enterscope();
    branch->expr->code(s);
    symtab.addid(branch->name, new var({offset, FP})); // save result 
    symtab.exitscope();
    emit_branch(case_end, s); // jump to end
  }
  // TODO: run the T2 label, which the tghe least <= expr type

  emit_label_def(labelcount, s); // no match
  emit_jal("_case_abort", s);
  emit_label_def(case_end, s);
}

void block_class::code(ostream &s)
{
  // Expressions body;

  // Comment in Assembly
  s << "\t# block_class::code" << endl;

  for (auto n = body->first(); body->more(n); n++)
  {
    body->nth(n)->code(s); // ACC = en
  }
}

void let_class::code(ostream &s)
{
  // let identifier: type_decl <- init in body
  symtab.enterscope();
  // Comment in Assembly
  s << "\t# let_class::code" << endl;

  // TODO: let
  // TODO: 初始化与判断
  if (init->get_type() == NULL)
  {
    if (type_decl == Str)
    {
      emit_load_string(ACC, stringtable.lookup_string(""), s);
    }
    else if (type_decl == Int)
    {
      emit_load_int(ACC, inttable.lookup_string("0"), s);
    }
    else if (type_decl == Bool)
    {
      emit_load_bool(ACC, falsebool, s);
    }
    else
    {
      emit_move(ACC, ZERO, s);
    }
  }
  else
  {
    init->code(s);
  }
  // ADD temporaries
  emit_push(ACC, s); // stack.push (init)
  symtab.addid(identifier, new var({-offset+1, FP}));
  body->code(s);
  emit_pop(nullptr, s);
  //  where is the data
  symtab.exitscope();
}

void plus_class::code(ostream &s)
{
  // e1 + e2

  // Comment in Assembly
  s << "\t# plus_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)
  emit_copy(s);
  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T1 = lw(ACC)

  emit_add(T1, T1, T2, s);                   // T1 = T1 + T2
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // SW result to [12]ACC
}

void sub_class::code(ostream &s)
{
  // e1 - e2

  // Comment in Assembly
  s << "\t# sub_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)
  emit_copy(s);
  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T1 = lw(ACC)

  emit_sub(T1, T1, T2, s);                   // T1 = T1 - T2
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // SW result to [12]ACC
}

void mul_class::code(ostream &s)
{
  // e1 * e2

  // Comment in Assembly
  s << "\t# mul_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)
  emit_copy(s);
  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T1 = lw(ACC)

  emit_mul(T1, T1, T2, s);                   // T1 = T1 * T2
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // SW result to [12]ACC
}

void divide_class::code(ostream &s)
{
  // e1 / e2

  // Comment in Assembly
  s << "\t# divide_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)
  emit_copy(s);
  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T1 = lw(ACC)

  emit_div(T1, T1, T2, s);                   // T1 = T1 / T2
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // SW result to [12]ACC
}

void neg_class::code(ostream &s)
{
  // - e1

  // Comment in Assembly
  s << "\t# neg_class::code" << endl;

  e1->code(s); // ACC = cgen(e1)
  emit_copy(s);
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s);  // T1 = lw(ACC) = value of e1
  emit_neg(T1, T1, s);                       // T1 = - T1
  emit_store(T1, DEFAULT_OBJFIELDS, ACC, s); // SW result to [12]ACC
}

void lt_class::code(ostream &s)
{
  //     e1 < e2

  // Comment in Assembly
  s << "\t# lt_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)

  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T2 = lw(ACC)

  labelcount++;
  emit_load_bool(ACC, truebool, s);  // la ACC, "bool_const1"
  emit_blt(T1, T2, labelcount, s);   // T1 < T2 then goto sth
  emit_load_bool(ACC, falsebool, s); // la ACC, "bool_const0"
  emit_label_def(labelcount, s);     // label: 1
}

void eq_class::code(ostream &s)
{
  // e1 == e2

  // Comment in Assembly
  s << "\t# eq_class::code (unfinished)" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)

  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)
  emit_move(T2, ACC, s);
  emit_load_bool(ACC, truebool, s);
  labelcount++;
  auto eq_label = labelcount;
  emit_beq(T1, T2, eq_label, s); // compare addresses
  emit_load_bool(ACC, falsebool, s);
  emit_jal("equality_test", s);
  emit_label_def(eq_label, s);
}

void leq_class::code(ostream &s)
{
  //     e1 <= e2

  // Comment in Assembly
  s << "\t# leq_class::code" << endl;

  e1->code(s);       // ACC = cgen(e1)
  emit_push(ACC, s); // stack.push  ACC (a0) = cgen(e1)

  e2->code(s); // ACC = cgen(e2)

  emit_pop(T1, s); // T1 = stack.pop = prev ACC = cgen(e2)

  emit_load(T1, DEFAULT_OBJFIELDS, T1, s);  // T1 = lw(T1)
  emit_load(T2, DEFAULT_OBJFIELDS, ACC, s); // T2 = lw(ACC)

  labelcount++;
  emit_load_bool(ACC, truebool, s);  // la ACC, "bool_const1"
  emit_bleq(T1, T2, labelcount, s);  // T1 < T2 then goto sth
  emit_load_bool(ACC, falsebool, s); // la ACC, "bool_const0"
  emit_label_def(labelcount, s);     // label: 1
}

void comp_class::code(ostream &s)
{
  // e1 == 0 ? goto : continue;

  // Comment in Assembly
  s << "\t# comp_class::code" << endl;

  e1->code(s);                              // ACC = cgen(e1)
  emit_load(T1, DEFAULT_OBJFIELDS, ACC, s); // T1 = lw(T1) = cgen(e1)
  labelcount++;
  emit_load_bool(ACC, truebool, s);
  emit_beqz(T1, labelcount, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(labelcount, s);
}

void int_const_class::code(ostream &s)
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC, inttable.lookup_string(token->get_string()), s);
}

void string_const_class::code(ostream &s)
{
  emit_load_string(ACC, stringtable.lookup_string(token->get_string()), s);
}

void bool_const_class::code(ostream &s)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s)
{
  // Comment in Assembly
  s << "\t# new__class::code" << endl;

  if (type_name == SELF_TYPE)
  {
    emit_load(T1, 0, SELF, s);             // T1: class tag
    emit_load_address(T2, CLASSOBJTAB, s); // T2: CLASSOBJTAB
    emit_sll(T1, T1, 3, s);
    emit_addu(T1, T1, T2, s); // T1 = T1 + T2
    emit_load(ACC, 0, T1, s); // ACC: name_protObj
    // next is to find the contruct function
    emit_push(T1, s); //T2 will use next time
    emit_jal("Object.copy", s);
    emit_pop(T1, s); // Get T2 again
    emit_load(T1, 1, T1, s);
    emit_jalr(T1, s);
  }
  else
  {
    char buf[64];
    sprintf(buf, "%s%s", type_name->get_string(), PROTOBJ_SUFFIX);
    emit_load_address(ACC, buf, s);
    char buf1[64];
    sprintf(buf1, "%s%s", type_name->get_string(), CLASSINIT_SUFFIX);
    emit_jal("Object.copy", s);
    emit_jal(buf1, s); // object init
  }
}

void isvoid_class::code(ostream &s)
{
  // isvoid e1 : Bool

  // Comment in Assembly
  s << "\t# isvoid_class::code" << endl;

  e1->code(s);           // ACC = e1 result
  emit_move(T1, ACC, s); // T1 = ACC
  auto isvoid_label = ++labelcount;
  emit_load_bool(ACC, truebool, s);
  emit_beqz(T1, isvoid_label, s);
  emit_load_bool(ACC, falsebool, s);
  emit_label_def(isvoid_label, s);
}

void no_expr_class::code(ostream &s)
{
  // Comment in Assembly
  s << "\t# no_expr_class::code" << endl;
}

void object_class::code(ostream &s)
{
  // Comment in Assembly
  emit_comment(s) << "object_class::code load " << name << endl;

  if (name == self)
  {
    emit_move(ACC, SELF, s);
  }
  else
  {
    auto var = symtab.lookup(name);
    emit_load(ACC, var->offset_in_word, var->reg, s);
  }
}

void method_class::code_ref(ostream &s, Symbol class_name)
{
  emit_method_ref(class_name, name, s);
}

void method_class::code_def(ostream &s, Symbol class_name)
{
  code_ref(s, class_name);
  s << LABEL;
  emit_activate(0, s);
  symtab.enterscope();

  int offset = 3;
  std::vector<Formal> formal_vector;
  for (int i = formals->first(); formals->more(i); i = formals->next(i))
  {
    formal_vector.push_back(formals->nth(i));
  }

  for (auto it = formal_vector.rbegin(); it != formal_vector.rend(); it++)
  {
    auto formal = *it;
    debug_output(3) << "Formal " << formal->get_name()->get_string() << " at " << offset * 4 << "($fp)" << endl;
    symtab.addid(formal->get_name(), new var{offset, FP});
    offset++;
  }

  expr->code(s);

  symtab.exitscope();
  emit_deactivate(0, formal_vector.size(), s); // todo: arg count
}
