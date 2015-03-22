@echo off
pushd	%~dp0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;rem	format source code files.
for %%E in (h hh hpp c cc cpp inc) do (
	dir /s /b "inc\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --formatted "inc\simple\*.%%E"		> nul	2<&1
	dir /s /b "src\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --formatted "src\simple\*.%%E"		> nul	2<&1
	dir /s /b "tests\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --formatted "tests\*.%%E"		> nul	2<&1

	dir /s /b "inc\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --formatted "inc\simple\*.%%E"		> nul	2<&1
	dir /s /b "src\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --formatted "src\simple\*.%%E"		> nul	2<&1
	dir /s /b "tests\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --formatted "tests\*.%%E"		> nul	2<&1
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;rem	end clean
goto	END
:ERROR
pause

:END

popd
echo	on