dnl
dnl libevent 関連のマクロ定義
dnl

dnl libevent の include ファイルとライブラリが存在するか調査します
dnl
dnl 以下の変数に値が格納されます
dnl
dnl     libevent_found  : "yes", "no" 
dnl     libevent_inc : "libevent の include ファイルまでのパス" 
dnl     libevent_lib : "libevent の ライブラリまでのパス" 
dnl
AC_DEFUN([SBT_FIND_LIBEVENT], 
[
    libevent_path="no"
    libevent_found="no"
    libevent_inc="no"
    libevent_lib="no"
    
	AC_MSG_CHECKING([for libevent])
	AC_ARG_WITH([libevent],
	           [AS_HELP_STRING([--with-libevent=@<:@ARG@:>@],[prefix for installed libevent])],
	           [libevent_path=$withval],
	           [libevent_path="no"])
    
    if test "$libevent_path" = "no" || test "$libevent_path" = "yes"; then
    	for lookdir in "/usr/include" "/usr/local/include"
    	do
    		if test -f "${lookdir}/event.h"; then
    			libevent_inc=${lookdir}
    			break
    		fi
    	done
    	
        for lookdir in "/usr/lib" "/usr/local/lib"
        do
            if test -f "${lookdir}/libevent.la"; then
                libevent_lib=${lookdir}
                break
            fi
        done
    else
        if test -d $libevent_path; then
            if test -d "${libevent_path}/include"; then
            	if test -f "${libevent_path}/include/event.h"; then
            	   libevent_inc="${libevent_path}/include"
            	fi
            fi
            
            if test -d "${libevent_path}/lib"; then
                if test -x "${libevent_path}/lib/libevent.la"; then
                   libevent_lib="${libevent_path}/lib"
                fi
            fi
        fi
    fi
    
    if test "$libevent_inc" = "no" || test "$libevent_lib" = "no"; then
        AC_MSG_RESULT([--with-libevent requires a directory or file to be provided])
        SBT_DOWNLOAD_LIBEVENT
    else
        libevent_found="yes"
        AC_MSG_RESULT([$libevent_found])
    fi
])

dnl libevent の include ファイルとライブラリのパスを定義します
dnl
dnl 以下の変数に値が定義されます
dnl
dnl     SBT_LIBEVENT_INCLUDE  : "-Iincludeパス"
dnl     SBT_LIBEVENT_LIBS     : "-Lライブラリパス"
dnl     SBT_HAVE_LIBEVENT     : "1"
dnl
AC_DEFUN([SBT_LIB_LIBEVENT],
[
    AC_MSG_NOTICE([libevent library configuration])

    SBT_FIND_LIBEVENT
    
    if test $libevent_found = "yes"; then
        AC_MSG_NOTICE([libevent include: $libevent_inc])
        AC_MSG_NOTICE([libevent libevent: $libevent_lib])
        
	    SBT_LIBEVENT_INCLUDE="-I${libevent_inc}"
        SBT_LIBEVENT_EXPORT_LIBS="-L${libevent_lib} -levent"
        SBT_LIBEVENT_LIBS="${libevent_lib}/libevent.a -lrt"
	    SBT_HAVE_LIBEVENT="1"
	    
	    AC_SUBST(SBT_LIBEVENT_INCLUDE)
	    AC_SUBST(SBT_LIBEVENT_EXPORT_LIBS)
	    AC_SUBST(SBT_LIBEVENT_LIBS)
        AC_SUBST(SBT_HAVE_LIBEVENT)
    else
        AC_MSG_ERROR([no suitable libevent found])
    fi
])

dnl libevent のダウンロードサイトを提示します
dnl
AC_DEFUN([SBT_DOWNLOAD_LIBEVENT],
[
  echo "The libevent library cannot be found."
  echo "Please install libevent on this system and supply the appropriate"
  echo "--with-libevent option to 'configure'"
  echo ""
  echo "or"
  echo ""
  echo "get it with libevent and put it in a subdirectory of this source:"
  echo ""
  echo "    http://www.monkey.org/~provos/libevent/ \\"
  echo "    libevent"
  echo ""
])
