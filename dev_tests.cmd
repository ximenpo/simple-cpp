@echo off
@pushd	%~dp0

@call	tools\run_test_cases.cmd	_test_*

@popd
@echo	on