TEX_BIN=docker run --rm -v `pwd`:/source xiejw/tex pdftex

default:
	${TEX_BIN} -output-directory=`dirname iso.tex` iso.tex
