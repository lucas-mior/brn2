#!/usr/bin/env python3
"""Regression tests for single_use_patterns.py (stdlib only)."""

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


CHECKER = Path(__file__).with_name("single_use_patterns.py")


class SingleUseTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.directory = Path(self.temp.name)

    def write(self, name, contents):
        path = self.directory / name
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(contents, encoding="utf-8")
        return path

    def run_check(self, *args):
        result = subprocess.run(
            [sys.executable, str(CHECKER), *map(str, args)],
            cwd=self.directory, capture_output=True, text=True,
            check=False)
        self.assertFalse(result.stderr, result.stderr)
        self.assertIn(result.returncode, (0, 1))
        return result.stdout

    def test_both_patterns_and_prototype(self):
        self.write("a.c", """
            typedef struct Data { int x; } Data;
            static int twice(int x);
            static int twice(int x) { return x * 2; }
            int run(void) {
                Data data = { 1 };
                return twice(data.x);
            }
        """)
        output = self.run_check(".")
        self.assertIn("struct Data: used only within one function", output)
        self.assertIn("function twice: called once", output)
        self.assertEqual(output.count("\n"), 2)

    def test_multiple_calls_and_functions(self):
        self.write("a.c", """
            typedef struct State { int v; } State;
            static int plus(int x) { return x + 1; }
            int first(void) {
                State s = { 2 };
                return plus(s.v) + plus(s.v);
            }
            int second(void) {
                State s = { 3 };
                return s.v;
            }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_external_uses_and_header_decl(self):
        self.write("a.c", """
            typedef struct Shared { int v; } Shared;
            int public_function(void) { return 0; }
            void use(void) { Shared s; public_function(); }
        """)
        self.write("b.h", """
            typedef struct Shared Shared;
            int public_function(void);
        """)
        self.assertEqual(self.run_check(".", "--include-global"), "")

    def test_static_name_in_unrelated_file(self):
        self.write("a.c", """
            static int helper(void) { return 1; }
            int a(void) { return helper(); }
        """)
        self.write("b.c", """
            static int helper(void) { return 2; }
            int b(void) { return helper(); }
        """)
        output = self.run_check(".")
        self.assertEqual(output.count("function helper:"), 2)

    def test_header_inline_code_is_a_possible_caller(self):
        self.write("a.c", """
            static int helper(void) { return 1; }
            int run(void) { return helper(); }
        """)
        self.write("b.h", """
            static inline int other(void) { return helper(); }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_unrelated_identifier_does_not_prove_struct_use(self):
        self.write("a.c", """
            typedef struct Data { int x; } Data;
            int run(void) { int Data = 1; return Data; }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_tagged_struct_without_typedef(self):
        self.write("a.c", """
            struct Data { int x; };
            int run(void) { struct Data data; return data.x; }
        """)
        self.assertIn("struct Data:", self.run_check("."))

    def test_nonstatic_opt_in(self):
        self.write("a.c", """
            int helper(void) { return 1; }
            int caller(void) { return helper(); }
        """)
        self.assertEqual(self.run_check("."), "")
        output = self.run_check("--include-global", ".")
        self.assertIn("function helper:", output)

    def test_local_function_declaration_is_not_a_call(self):
        self.write("a.c", """
            static int helper(void) { return 1; }
            int run(void) {
                int helper(void);
                return 0;
            }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_function_pointer_and_macro(self):
        self.write("a.c", """
            static int pointed(void) { return 1; }
            static int macros(void) { return 2; }
            int run(void) {
                int (*ptr)(void) = pointed;
                return ptr() + macros();
            }
        """)
        self.write("defs.h", "#define CALL_MACROS() macros()\n")
        self.assertEqual(self.run_check("."), "")

    def test_conditionals_not_safe(self):
        self.write("a.c", """
            typedef struct Maybe { int x; } Maybe;
            static int helper(void) { return 1; }
            int run(void) {
            #if FEATURE
                Maybe s = { 1 };
                return helper() + s.x;
            #endif
                return 0;
            }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_strings_comments_and_identifiers(self):
        self.write("a.c", r'''
            typedef struct Box { int value; } Box;
            static int work(void) { return 4; }
            int main(void) {
                Box box;
                char *unused = "work() Box { }";
                // work(); Box
                /* work(); Box */
                return work() + box.value;
            }
        ''')
        output = self.run_check(".")
        self.assertIn("function work:", output)
        self.assertIn("struct Box:", output)

    def test_tag_and_alias_both_count(self):
        self.write("a.c", """
            typedef struct Node { struct Node *next; } Node;
            int run(void) {
                struct Node n;
                Node other;
                return n.next == &other;
            }
        """)
        output = self.run_check(".")
        self.assertIn("struct Node:", output)
        self.assertEqual(output.count("struct Node:"), 1)

    def test_shadowed_struct_tag_is_not_a_use(self):
        self.write("a.c", """
            struct Data { int x; };
            int run(void) {
                struct Data { int y; };
                struct Data local;
                return local.y;
            }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_multiple_pointer_qualifiers(self):
        self.write("a.c", """
            typedef struct Data { int x; } Data;
            int run(void) {
                Data **volatile pointer = 0;
                return pointer == 0;
            }
        """)
        self.assertIn("struct Data:", self.run_check("."))

    def test_global_use_disqualifies_type(self):
        self.write("a.c", """
            struct Box { int x; };
            struct Box shared;
            int run(void) { struct Box local; return local.x; }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_local_struct_definition_not_reported(self):
        self.write("a.c", """
            int run(void) {
                typedef struct Local { int x; } Local;
                Local x = { 0 };
                return x.x;
            }
        """)
        self.assertEqual(self.run_check("."), "")

    def test_included_c_is_not_standalone(self):
        self.write("inc.c", """
            static int helper(void) { return 1; }
            int run(void) { return helper(); }
        """)
        self.write("main.c", '#include "inc.c"\n')
        self.assertEqual(self.run_check("."), "")

    def test_multiline_directive(self):
        self.write("a.c", """
            static int helper(void) { return 1; }
            int run(void) { return helper(); }
        """)
        self.write("b.h", "#define USE() \\\n helper()\n")
        self.assertEqual(self.run_check("."), "")

    def test_file_list_and_exclude(self):
        a = self.write("a.c", """
            static int helper(void) { return 1; }
            int run(void) { return helper(); }
        """)
        self.write("b.c", """
            static int other(void) { return 2; }
            int use(void) { return other(); }
        """)
        paths = self.write("sources.txt", f"{a}\n")
        output = self.run_check("--file-list", paths)
        self.assertIn("function helper:", output)
        self.assertNotIn("function other:", output)
        self.assertEqual(self.run_check("--exclude", "a.c", a), "")


if __name__ == "__main__":
    unittest.main()
