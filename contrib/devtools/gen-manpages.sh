#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

ZioncoinD=${ZioncoinD:-$SRCDIR/Zioncoind}
ZioncoinCLI=${ZioncoinCLI:-$SRCDIR/Zioncoin-cli}
ZioncoinTX=${ZioncoinTX:-$SRCDIR/Zioncoin-tx}
ZioncoinQT=${ZioncoinQT:-$SRCDIR/qt/Zioncoin-qt}

[ ! -x $ZioncoinD ] && echo "$ZioncoinD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
SYSVER=($($ZioncoinCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for Zioncoind if --version-string is not set,
# but has different outcomes for Zioncoin-qt and Zioncoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$ZioncoinD --version | sed -n '1!p' >> footer.h2m

for cmd in $ZioncoinD $ZioncoinCLI $ZioncoinTX $ZioncoinQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${SYSVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${SYSVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
