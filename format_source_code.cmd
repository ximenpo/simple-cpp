@echo off
pushd	%~dp0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;rem	format source code files.
for %%E in (h hh hpp c cc cpp inc) do (
	dir /s /b "inc\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --recursive --formatted "inc\*.%%E"	> nul	2<&1
	dir /s /b "src\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --recursive --formatted "src\*.%%E"	> nul	2<&1
	dir /s /b "tests\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=linux --suffix=none --recursive --formatted "tests\*.%%E"	> nul	2<&1

	dir /s /b "inc\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --recursive --formatted "inc\*.%%E"	> nul	2<&1
	dir /s /b "src\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --recursive --formatted "src\*.%%E"	> nul	2<&1
	dir /s /b "tests\*.%%E"	> nul	2<&1	&& tools\AStyle.exe --style=java --suffix=none --recursive --formatted "tests\*.%%E"	> nul	2<&1
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;rem	end clean
goto	END
:ERROR
pause

:END

popd
echo	on