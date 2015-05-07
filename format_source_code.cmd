@echo off
pushd	%~dp0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;rem	format source code files.
for %%E in (h c cpp inc) do (
	dir /b "inc\simple\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --formatted "inc\simple\*.%%E"		> nul	2<&1
	dir /b "tests\*.%%E"		> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --formatted "tests\*.%%E"		> nul	2<&1

	dir /b "inc\simple\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --formatted "inc\simple\*.%%E"		> nul	2<&1
	dir /b "tests\*.%%E"		> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --formatted "tests\*.%%E"		> nul	2<&1
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;rem	end clean
goto	END
:ERROR
pause

:END

popd
echo	on