#!/bin/sh
_instdir=${1}

if [ ${_instdir} != "/usr" ]; then 
  semanage fcontext -a -t textrel_shlib_t "${_instdir}/lib/(.*/)?.+.so(\..*)?"
  restorecon -R -v * ${_instdir}/lib
  setsebool -P user_tcp_server=1 
fi
