#!/usr/bin/env python3
"""Report potentially redundant C functions and file-scope struct types.

This is a conservative lexical check, not a preprocessor or a C compiler.
It deliberately omits uncertain cases instead of proposing unsafe refactors.
See --help for usage and limitations.
"""

import argparse
from collections import defaultdict
from dataclasses import dataclass
from fnmatch import fnmatch
from pathlib import Path
import re
import sys


IDENTIFIER = re.compile(r"[A-Za-z_]\w*\Z", re.ASCII)
LEXEMES = re.compile(r"[A-Za-z_]\w*|[^\s]", re.ASCII)
COMMENTS_AND_LITERALS = re.compile(
    r"/\*.*?\*/|//[^\n]*|(?:u8|[uUL])?\"(?:\\.|[^\"\\])*\""
    r"|(?:u8|[uUL])?'(?:\\.|[^'\\])*'",
    re.DOTALL,
)
DIRECTIVE = re.compile(r"^[ \t]*#\s*(\w+)(.*)$")
INCLUDE_C = re.compile(r'^\s*[<"]([^>"]+\.c)[>"]')
CONTROL = {"if", "while", "for", "switch", "sizeof", "_Alignof",
           "_Generic", "return", "typedef", "__attribute__"}
SKIP_DIRS = {".git", ".cache", "__pycache__", "bin", "build"}


@dataclass(frozen=True)
class Token:
    value: str
    line: int


@dataclass
class Function:
    name: str
    line: int
    name_index: int
    start: int
    end: int
    is_static: bool
    conditional: bool


@dataclass
class Struct:
    name: str
    line: int
    names: tuple
    tag: str
    alias: str
    start: int
    end: int
    conditional: bool


class Source:
    def __init__(self, path):
        self.path = path
        original = path.read_text(encoding="utf-8", errors="replace")
        lines = original.splitlines(keepends=True)
        directives = set()
        conditional = set()
        self.included_c = set()
        self.prototype_indices = set()
        condition_depth = 0
        continued = False

        for number, line in enumerate(lines, 1):
            is_continuation = continued
            match = DIRECTIVE.match(line) if not continued else None
            if match:
                kind = match.group(1)
                if kind in ("if", "ifdef", "ifndef"):
                    condition_depth += 1
                elif kind == "endif":
                    condition_depth = max(0, condition_depth - 1)
                if kind == "include":
                    included = INCLUDE_C.match(match.group(2))
                    if included:
                        self.included_c.add(Path(included.group(1)).name)
                continued = line.rstrip("\r\n").endswith("\\")
            elif continued:
                continued = line.rstrip("\r\n").endswith("\\")
            else:
                continued = False
            if match or is_continuation:
                directives.add(number)
            if condition_depth:
                conditional.add(number)
        def mask(match):
            return re.sub(r"[^\n]", " ", match.group())

        code = COMMENTS_AND_LITERALS.sub(mask, original)
        self.tokens = []
        self.directive_uses = set()
        for number, line in enumerate(code.splitlines(), 1):
            for item in LEXEMES.finditer(line):
                value = item.group()
                if number in directives:
                    if IDENTIFIER.fullmatch(value):
                        self.directive_uses.add(value)
                else:
                    self.tokens.append(Token(value, number))
        self.conditional = conditional
        self.functions = []
        self.structs = []
        self.owners = [-1] * len(self.tokens)
        self._parse()

    def _parse(self):
        tokens = self.tokens
        pairs = {}
        stack = []
        for index, token in enumerate(tokens):
            if token.value in "({[" and len(token.value) == 1:
                stack.append((token.value, index))
            elif token.value in ")}]" and len(token.value) == 1:
                opening = {")": "(", "]": "[", "}": "{"}[token.value]
                if stack and stack[-1][0] == opening:
                    _, start = stack.pop()
                    pairs[start] = index
                    pairs[index] = start
                else:
                    # Unbalanced input is unsuitable for scope analysis.
                    return
        if stack:
            return
        self.pairs = pairs

        start = 0
        index = 0
        while index < len(tokens):
            value = tokens[index].value
            if value == ";":
                self._prototype(start, index, pairs)
                start = index + 1
            elif value == "{":
                end = pairs[index]
                head = tokens[start:index]
                function = self._function(head, start, index, end, pairs)
                if function is not None:
                    owner = len(self.functions)
                    self.functions.append(function)
                    for item in range(index + 1, end):
                        self.owners[item] = owner
                    index = end
                else:
                    struct = self._struct(head, start, index, end)
                    if struct is not None:
                        self.structs.append(struct)
                    index = end
                start = end + 1
            index += 1

    def _prototype(self, start, end, pairs):
        tokens = self.tokens
        if end <= start or tokens[end - 1].value != ")":
            return
        paren = pairs.get(end - 1)
        if paren is None or paren <= start:
            return
        name = tokens[paren - 1].value
        prefix = [token.value for token in tokens[start:paren - 1]]
        if (IDENTIFIER.fullmatch(name) and name not in CONTROL and prefix
                and "typedef" not in prefix and "=" not in prefix
                and "(" not in prefix and ")" not in prefix):
            self.prototype_indices.add(paren - 1)

    def _function(self, head, start, brace, end, pairs):
        tokens = self.tokens
        if not head or tokens[brace - 1].value != ")":
            return None
        paren = pairs.get(brace - 1)
        if paren is None or paren <= start:
            return None
        name_index = paren - 1
        name = tokens[name_index].value
        if not IDENTIFIER.fullmatch(name) or name in CONTROL:
            return None
        prefix = tokens[start:name_index]
        if not prefix or any(token.value in ("=", "typedef", ":")
                             for token in prefix):
            return None
        # A function definition must have a declarator, not an initializer.
        if any(token.value in ("{", "}", ";") for token in prefix):
            return None
        return Function(name, tokens[name_index].line, name_index,
                        brace + 1, end,
                        any(token.value == "static" for token in prefix),
                        tokens[name_index].line in self.conditional)

    def _struct(self, head, start, brace, end):
        tokens = self.tokens
        if not head:
            return None
        # Only simple file-scope struct definitions, with a single declarator.
        # Do not mistake a compound literal or initialized object for a type.
        if any(token.value == "=" for token in head):
            return None
        values = [token.value for token in head]
        if "struct" not in values:
            return None
        struct_pos = len(values) - 1 - values[::-1].index("struct")
        after_struct = values[struct_pos + 1:]
        if len(after_struct) > 1:
            return None
        if after_struct and not IDENTIFIER.fullmatch(after_struct[0]):
            return None
        tag = after_struct[0] if after_struct else None
        typedef = "typedef" in values[:struct_pos]
        if not typedef and not tag:
            return None

        stop = end + 1
        while stop < len(tokens) and tokens[stop].value != ";":
            # Avoid swallowing the next declaration if this isn't simple.
            if tokens[stop].value in ("{", "}"):
                return None
            stop += 1
        if stop == len(tokens):
            return None
        trailing = [token.value for token in tokens[end + 1:stop]]
        alias = None
        if typedef:
            if len(trailing) != 1 or not IDENTIFIER.fullmatch(trailing[0]):
                return None
            alias = trailing[0]
            names = tuple(dict.fromkeys(filter(None, (tag, alias))))
        else:
            if trailing:
                return None
            names = (tag,)
        return Struct(names[-1], tokens[struct_pos + start].line, names,
                      tag, alias, start, stop + 1,
                      tokens[struct_pos + start].line in self.conditional)


def files_to_check(inputs, exclusions):
    found = set()
    for path_str in inputs:
        path = Path(path_str)
        if path.is_file():
            if path.suffix.lower() in (".c", ".h"):
                found.add(path.resolve())
        elif path.is_dir():
            for candidate in path.rglob("*"):
                if any(part in SKIP_DIRS for part in candidate.parts):
                    continue
                if (candidate.is_file() and
                        candidate.suffix.lower() in (".c", ".h")):
                    found.add(candidate.resolve())
        else:
            raise ValueError(f"not a C file or directory: {path}")
    return sorted(path for path in found
                  if not any(fnmatch(path.as_posix(), glob) or
                             fnmatch(path.name, glob) for glob in exclusions))



def clear_type_use(source, index, struct):
    """Require C type syntax; a coincidentally named value is not a use."""
    tokens = source.tokens
    value = tokens[index].value
    before = tokens[index - 1].value if index else ""
    after = tokens[index + 1].value if index + 1 < len(tokens) else ""
    before2 = tokens[index - 2].value if index >= 2 else ""
    after2 = tokens[index + 2].value if index + 2 < len(tokens) else ""

    if value == struct.tag and before == "struct":
        return after not in ("{", ";")
    if value != struct.alias:
        return False
    if before == "struct":
        return False
    if before == "(" and before2 in ("sizeof", "_Alignof", "_Generic"):
        return True
    if before == "(" and after == ")" and after2 == "{":
        return True
    # A declaration whose declarator begins with a name, pointer or array.
    if before not in ("{", "}", ";", "(", ",", "const", "volatile",
                      "restrict", "_Atomic", "static", "extern", "register"):
        return False
    if IDENTIFIER.fullmatch(after) and after not in CONTROL:
        return True
    if after == "*":
        cursor = index + 1
        while cursor < len(tokens) and tokens[cursor].value in (
                "*", "const", "volatile", "restrict"):
            cursor += 1
        if cursor < len(tokens) and IDENTIFIER.fullmatch(
                tokens[cursor].value):
            return True
    return False


def analyze(sources, global_functions=False):
    uses = defaultdict(list)
    for source in sources:
        for index, token in enumerate(source.tokens):
            if IDENTIFIER.fullmatch(token.value):
                uses[token.value].append((source, index))

    included_c = set()
    directives = set()
    for source in sources:
        included_c.update(source.included_c)
        directives.update(source.directive_uses)

    findings = []
    for source in sources:
        if source.path.suffix != ".c" or source.path.name in included_c:
            continue
        for function in source.functions:
            if function.conditional or (not function.is_static and
                                        not global_functions):
                continue
            if function.name in directives:
                continue
            seen_calls = []
            valid = True
            for other, index in uses[function.name]:
                if other is not source:
                    # Even a mention in a header might be a macro or an API.
                    if other.path.suffix == ".h" or not function.is_static:
                        valid = False
                        break
                    # Distinct static functions can have the same name.
                    continue
                token = other.tokens[index]
                if index == function.name_index or \
                        index in other.prototype_indices:
                    continue
                # A prototype isn't a call; conservatively suppress the
                # result whenever any such declaration exists.
                if other.owners[index] < 0:
                    valid = False
                    break
                if token.line in source.conditional:
                    valid = False
                    break
                if index + 1 >= len(other.tokens) or \
                        other.tokens[index + 1].value != "(":
                    valid = False
                    break
                close = other.pairs.get(index + 1)
                if (close is None or index > 0 and
                        IDENTIFIER.fullmatch(other.tokens[index - 1].value)
                        and other.tokens[index - 1].value not in (
                            "return", "sizeof", "_Alignof", "if", "while")
                        and close + 1 < len(other.tokens)
                        and other.tokens[close + 1].value == ";"):
                    # A declaration inside a function is not a call.
                    valid = False
                    break
                if index > 0 and other.tokens[index - 1].value in (".", ">"):
                    valid = False
                    break
                caller = other.functions[other.owners[index]]
                if caller.name == function.name:
                    valid = False
                    break
                seen_calls.append(index)
                if len(seen_calls) > 1:
                    break
            if valid and len(seen_calls) == 1:
                findings.append((source.path, function.line, "function",
                                 function.name,
                                 "called once, in its defining file"))

        for struct in source.structs:
            if struct.conditional or any(name in directives
                                         for name in struct.names):
                continue
            owners = set()
            valid = True
            found_use = False
            for name in struct.names:
                for other, index in uses[name]:
                    if other is source and struct.start <= index < struct.end:
                        continue
                    if other is not source:
                        valid = False
                        break
                    if other.tokens[index].line in source.conditional:
                        valid = False
                        break
                    owner = other.owners[index]
                    if owner < 0:
                        valid = False
                        break
                    if not clear_type_use(other, index, struct):
                        valid = False
                        break
                    owners.add(owner)
                    found_use = True
                    if len(owners) > 1:
                        valid = False
                        break
                if not valid:
                    break
            if valid and found_use and len(owners) == 1:
                findings.append((source.path, struct.line, "struct",
                                 struct.name,
                                 "used only within one function"))
    return sorted(findings, key=lambda row: (str(row[0]), row[1], row[2]))


def main():
    parser = argparse.ArgumentParser(
        description="Find single-use functions and function-local struct types "
                    "in C source files. Non-static functions are skipped by "
                    "default because external users cannot be ruled out.",
        epilog="Conservative limitations: does not resolve macro expansions, "
               "function pointers, conditional compilation, or complex "
               "declarators. Scan the whole project (including headers) to "
               "reduce false positives. Findings are suggestions, not proof.")
    parser.add_argument("paths", nargs="*",
                        help="C files/directories (default: .)")
    parser.add_argument("--file-list", metavar="PATH",
                        help="newline-separated list of files to inspect")
    parser.add_argument("--exclude", action="append", default=[],
                        metavar="GLOB",
                        help="exclude matching paths or basenames")
    parser.add_argument("--include-global", action="store_true",
                        help="also consider non-static functions (closed-world "
                             "assumption; external consumers may exist)")
    args = parser.parse_args()
    paths = list(args.paths)
    try:
        if args.file_list:
            paths += Path(args.file_list).read_text().splitlines()
        source_paths = files_to_check(paths or ["."], args.exclude)
        sources = [Source(path) for path in source_paths]
    except (OSError, ValueError) as error:
        parser.exit(2, f"{error}\n")
    findings = analyze(sources, args.include_global)
    for path, line, kind, name, explanation in findings:
        try:
            shown = path.relative_to(Path.cwd())
        except ValueError:
            shown = path
        print(f"{shown}:{line}: {kind} {name}: {explanation}")
    return 1 if findings else 0


if __name__ == "__main__":
    sys.exit(main())
