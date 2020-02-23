# $Id: Makefile,v 7.46 2019-08-15 17:02:48-07 - - $

ALL:= ${basename ${filter %.mm, ${shell ls -t}}}
NEW:= ${firstword ${ALL}}

GROFFDIR = /afs/cats.ucsc.edu/courses/cse110a-wm/groff
DEPS     = ${GROFFDIR}/tmac/Tmac.mm-etc Makefile
DIROPTS  = -F${GROFFDIR}/font -I${GROFFDIR}/tmac -M${GROFFDIR}/tmac
GROFFMM  = groff -mgm -U -b -ww ${DIROPTS}
PSOPTS   = -spte -Tps
TTOPTS   = -st -Tlatin1 -P-cbuo
SQUEEZE  = perl -00pe0
STARS    = ************************************************************
LABEL    = echo "${STARS}"; echo $@: $< ${1}; echo "${STARS}"

newest : ${NEW}.tt ${NEW}.view ${NEW}.pdf

all : tt ps pdf

ps : ${ALL:%=%.ps}
tt : ${ALL:%=%.tt}
pdf : ${ALL:%=%.pdf}

% : %.view

%. : %.view

%.view : %.ps
	- pkill -s 0 gv || true
	gv $< &

%.ps : %.mm ${DEPS}
	@ ${call LABEL, ${PSOPTS}}
	cid + $<
	${GROFFMM} ${PSOPTS} -z -rRef=1 $<
	${GROFFMM} ${PSOPTS} $< >$@
	letterbbox $@

%.tt : %.mm ${DEPS}
	@ ${call LABEL, ${TTOPTS}}
	cid + $<
	${GROFFMM} ${TTOPTS} -z -rRef=1 $<
	${GROFFMM} ${TTOPTS} $< | ${SQUEEZE} >$@

%.pdf : %.ps
	mkpdf $<

