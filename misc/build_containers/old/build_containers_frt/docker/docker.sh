#! /bin/sh
set -e

if [ $# -ne 1 ] ; then
	echo 'usage: docker.sh arch'
	exit 1
fi

exec docker run -it --rm \
  -v /etc/passwd:/etc/passwd:ro \
  -v /etc/shadow:/etc/shadow:ro \
  -v /etc/group:/etc/group:ro \
  -v /opt:/opt \
  -v $HOME:$HOME \
crossbuild:$1
