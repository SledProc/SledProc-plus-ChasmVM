pnum=1
latex=/home/nlevisrael/texlive/bin/x86_64-linux/pdflatex 
$latex -interaction=nonstopmode --shell-escape -output-dir=out \
--jobname=p$pnum p$pnum.tex


pnum=2
$latex -interaction=nonstopmode --shell-escape -output-dir=out \
--jobname=p$pnum p$pnum.tex


pnum=3
$latex -interaction=nonstopmode --shell-escape -output-dir=out \
--jobname=p$pnum p$pnum.tex



