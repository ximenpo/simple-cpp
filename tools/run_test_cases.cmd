pushd	%~dp0
cd	..

;;
set	test_files=%1
if "%test_files%" == "" (
set	test_files=*
)

;;rem	clean prvious temp files.
if exist *.exe		@del /f /q	*.exe
if exist *.obj		@del /f /q	*.obj
if exist *.lib		@del /f /q	*.lib
if exist *.exp		@del /f /q	*.exp
if exist *.module	@del /f /q	*.module

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;rem	compile and run the tests.
if "%OSTYPE%"=="cygwin"	(
	gcc	tests/dummy_module.cpp		-shared	-o dummy.module	-D NDEBUG	-lstdc++
	gcc	inc/simple/*.cpp inc/simple/*.c tests/main.cpp tests/%test_files%.cpp	-o _test.exe	-D NDEBUG	-I inc	-I tests	-lstdc++
) else (
	cl	tests\dummy_module.cpp		/LD	/Fedummy.module	-D NDEBUG
	cl	inc\simple\*.cpp inc\simple\*.c tests\main.cpp tests\%test_files%.cpp	/Fe_test.exe	-D NDEBUG	/Iinc			/EHa /wd4819
)
if not	exist _test.exe			goto ERROR
if exist *.obj		@del /f /q	*.obj
echo	==========================================
echo		run tests now ...
_test.exe	--version
echo	==========================================
if "%OSTYPE%"=="cygwin"	(
	_test.exe	--output=color	||	goto ERROR
) else (
	_test.exe	--output=vs	||	goto ERROR
)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;rem	end clean
goto	END
:ERROR

;;rem	alert.
if "%OSTYPE%"=="cygwin"	(
	pause
) else (
	color 4f
	pause
	color
)

:END

;;rem	clean temp files.
if exist *.exe		@del /f /q	*.exe
if exist *.obj		@del /f /q	*.obj
if exist *.lib		@del /f /q	*.lib
if exist *.exp		@del /f /q	*.exp
if exist *.module	@del /f /q	*.module

popd
