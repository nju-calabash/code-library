#!/usr/bin/env python3
import re, os, hashlib, sys
import subprocess
import oyaml as yaml

LANGS = {'c': 'C', 'cpp': 'C++', 'py': 'Python', 'java': 'Java'}

def digest(s) :
    return hashlib.md5(re.sub(r'\s|//.*', '', s).encode('utf8')).hexdigest()[-4:]

line_cnt = 0

recipe = yaml.load(open('recipe.yaml').read(), Loader = yaml.SafeLoader)
for cat, ls in recipe.items() :
    print('\\section{%s}' % cat.replace('_', ' ').title())
    for fn in ls :
        filename = 'src/' + cat + '/' + fn
        ext = fn.split('.')[-1]
        meta = yaml.load(
            subprocess.check_output(['./script/extract.py', '%%%', filename]),
            Loader = yaml.SafeLoader
        )
        code = subprocess.check_output(['./script/extract.py', '+++', filename]).decode('utf8')
        title = meta['title']
        desc = meta.get('desc', None)
        usage = meta.get('usage', None)
        sect = [ '\\subsection{%s}' % title ]
        if desc :
            sect.append(desc.strip())
            sect.append('')
        if usage :
            sect.append('\\textbf{Usage:}\\par')
            sect.append('\\begin{tabular}{p{3.5cm} p{8cm}}')
            for funct, descript in usage.items() :
                sect.append('\lstinline|{0}| & {1} \\\\'.format(funct, descript))
            sect.append('\\end{tabular}')
        for line in code.split('\n'):
            sect.append('\\createlinenumber{%d}{%s}' % (line_cnt, digest(line)))
            line_cnt += 1
        sect.append('\\begin{lstlisting}[language=%s]' % 
            LANGS.get(ext, '{}a').lower())
        sect.append(code)
        sect.append('\\end{lstlisting}')
        print('\n'.join(sect))
