
# Copyright (c) 2001-03
# Aleksey Gurtovoy
#
# Permission to use, copy, modify, distribute and sell this software
# and its documentation for any purpose is hereby granted without fee, 
# provided that the above copyright notice appears in all copies and 
# that both the copyright notice and this permission notice appear in 
# supporting documentation. No representations are made about the 
# suitability of this software for any purpose. It is provided "as is" 
# without express or implied warranty.
#
# See http://www.boost.org/libs/mpl for documentation.

import fileinput
import os
import re
import string
import sys

if_else = lambda a,b,c:(a and [b] or [c])[0]
max_len = 79
ident = 4

def nearest_ident_pos(text):
    return (len(text)/ident) * ident
    
def block_format(limits,text,first_sep='  ',sep=',',need_last_ident=1):
    words = string.split(
          string.join(string.split(text),' ')
        , if_else(sep != ',' or string.find(text,'<') == -1,sep,' %s '%sep)
        )
    s = reduce(lambda t,x: '%s '%t, range(0,limits[0]), '')
    max_len = limits[1]
    return '%s\n%s' \
        % (
         reduce(
            lambda t,w,max_len=max_len,s=s,sep=sep:
                if_else(t[1] + len(w) < max_len
                    , ('%s%s%s'% (t[0],t[2],w), t[1]+len(w)+len(t[2]), sep)
                    , ('%s\n%s%s%s'% (t[0],s,sep,w), len(s)+len(w)+len(sep), sep)
                    )
            , words
            , (s,len(s)+len(first_sep),first_sep)
            )[0]
        , if_else(need_last_ident,s,'')
        )

def handle_args(match):
    if re.compile('^\s*(typedef|struct|static)\s+.*?$').match(match.group(0)):
        return match.group(0)
    
    return '%s'\
        % block_format(
              (nearest_ident_pos(match.group(1)),max_len)
            , match.group(3)
            , match.group(2)
            , ','
            , 0
            )

def handle_inline_args(match):
    if len(match.group(0)) < max_len:
        return match.group(0)

    if match.group(9) == None:
        return '%s%s<\n%s>\n'\
            % (
                  match.group(1)
                , match.group(3)
                , block_format(
                      (nearest_ident_pos(match.group(1))+ident,max_len)
                    , match.group(4)
                    )
            )
        
    return '%s%s<\n%s>\n%s%s'\
        % (
              match.group(1)
            , match.group(3)
            , block_format(
                 (nearest_ident_pos(match.group(1))+ident,max_len-len(match.group(9)))
                , match.group(4)
                )
            , string.replace(match.group(1),',',' ')
            , match.group(9)
          )

def handle_simple_list(match):
    if match.group(1) == 'template':
        return match.group(0)

    single_arg = re.compile('^\s*(\w|\d)+\s*$').match(match.group(2))
    return if_else(single_arg,'%s<%s>','%s< %s >') %\
        (
          match.group(1)
        , string.join(string.split(match.group(2)), '')
        )

def handle_static(match):
    if len(match.group(0)) < max_len:
        return match.group(0)

    (first_sep,sep) = if_else(string.find(match.group(0),'+') == -1, (' ',' '),('  ','+'))
    return '%s%s\n%s%s' %\
        (
          match.group(1)
        , string.join(string.split(match.group(2)), ' ')
        , block_format(
              (nearest_ident_pos(match.group(1))+ident,max_len)
            , match.group(4)
            , first_sep
            , sep
            )
        , match.group(5)
        )

def handle_typedefs(match):
    if string.count(match.group(2), ';') == 1:
        return match.group(0)

    join_sep = ';\n%s' % match.group(1)
#    return if_else(string.find(match.group(0), '\n') == -1, '%s%s\n', '%s%s') \
    return '%s%s\n' \
        % (
            match.group(1)
          , string.join(map(string.strip, string.split(match.group(2), ';')), join_sep)
          )
    
class pretty:
    def __init__(self, name):
        self.output = open(name, "w")
        self.prev_line = ''

        self.re_header_name_comment = re.compile(r"^\s*//\s+\+\s+file:\s+(boost/mpl(/\w+)+\.hpp)s*$")
        self.header_was_written = 0

        self.re_junk = re.compile(r"^\s*(#|//[^:]).*$")
        self.re_c_comment_start = re.compile(r"^\s*/\*.*")
        self.re_c_comment_end = re.compile(r"^.*\*/\s*$")
        self.inside_c_comment = 0

        self.re_empty_line = re.compile(r"^\s*$")        
        self.re_comma = re.compile(r'(\w+)\s*,\s*')
        self.re_assign = re.compile(r'\s*(=+)\s*')
        self.re_marked_comment = re.compile(r'^(\s*//):(.*)$')
        self.re_marked_empty_comment = re.compile(r'^\s*//\s*$')
        self.re_typedef = re.compile(r'^\s+typedef\s+.*?;$')
        self.re_nsl = re.compile(r'^(\s+typedef\s+.*?;|\s*(private|public):\s*|\s*{\s*|\s*(\w|\d|,)+\s*)$')
        self.re_templ_decl = re.compile(r'^(\s*template\s*<\s*.*?|\s*(private|public):\s*)$')
        self.re_type_const = re.compile(r'(const)\s+((unsigned|signed)?(bool|char|short|int|long))')
        self.re_templ_args = re.compile(r'^(\s*)(, | {2})((\s*(\w+)(\s+|::)\w+\s*.*?,?)+)\s*$')
        self.re_inline_templ_args = re.compile(
            r'^(\s+(,|:\s+)?|struct\s+)(\w+)\s*<((\s*(typename\s+)?\w+\s*(=\s*.*|<(\s*\w+\s*,?)+>\s*)?,?)+)\s*>\s+((struct|class).*?)?$'
            )

        self.re_simple_list = re.compile(r'(\w+)\s*<((\w|,| |-|>|<)+)>')
        self.re_static_const = re.compile(r'(\s*)((static\s+.*?|enum\s*{\s*)value\s*=)(.*?)(}?;)$')
        self.re_typedefs = re.compile(r'(\s*)((\s*typedef\s*.*?;)+)\s*$')
        self.re_closing_curly_brace = re.compile(r'^(}|struct\s+\w+);\s*$')
        self.re_namespace_scope_templ = re.compile(r'^template\s*<\s*$')
        self.re_namespace = re.compile(r'^\n?namespace\s+\w+\s*{\s*\n?$')

    def process(self, line):

        # searching for header line
        if not self.header_was_written and self.re_header_name_comment.match(line):
            self.header_was_written = 1
            match = self.re_header_name_comment.match(line)
            self.output.write( \
                "// preprocessed version of '%s' header\n"\
                "// see the original for copyright information\n\n" \
                % match.group(1)
                )
            return
        
        # skipping preprocessor directives, comments, etc.
        if self.re_junk.match(line):
            return

        if self.inside_c_comment or self.re_c_comment_start.match(line):
            self.inside_c_comment = not self.re_c_comment_end.match(line)
            return

        # restoring some empty lines
        if self.re_templ_decl.match(line) and self.re_typedef.match(self.prev_line) \
           or not self.re_empty_line.match(line) and self.re_closing_curly_brace.match(self.prev_line) \
           or not self.re_empty_line.match(self.prev_line) \
              and ( self.re_namespace_scope_templ.match(line) \
                    or self.re_namespace.match(line) and not self.re_namespace.match(self.prev_line) \
                    ):
            line = '\n%s' % line

        # removing excessive empty lines
        if self.re_empty_line.match(line):
            if self.re_empty_line.match(self.prev_line) or not self.header_was_written:
                return

            # skip empty line after typedef
            if self.re_nsl.match(self.prev_line):
                return

        # formatting
        line = self.re_comma.sub(r'\1, ', line)
        line = self.re_assign.sub(r' \1 ', line)
        line = self.re_marked_comment.sub(r'\1\2', line)
        line = self.re_marked_empty_comment.sub(r'\n', line)
        line = self.re_type_const.sub(r'\2 \1', line)
        line = self.re_templ_args.sub(handle_args, line)
        line = self.re_inline_templ_args.sub(handle_inline_args, line)
        line = self.re_simple_list.sub(handle_simple_list, line)
        line = self.re_static_const.sub(handle_static, line)
        line = self.re_typedefs.sub(handle_typedefs, line)
        
        # write the output
        self.output.write(line)
        self.prev_line = line

def main():
    p = pretty(os.path.basename(sys.argv[2]))
    for line in fileinput.input(sys.argv[1]):
        p.process(line)

main()
