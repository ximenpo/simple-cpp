@echo off
@pushd	%~dp0

@call	tools\run_test_cases.cmd	test_*

@popd
@echo	on