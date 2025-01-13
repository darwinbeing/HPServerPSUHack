java -jar "%~dp0ezbl_tools.jar" --blobber -dump -artifact="%1" > "%TEMP%\%~n1-%~n0.txt"
@if not errorlevel 0 goto ABNORMAL_END
@pushd "%TEMP%"
start "%~n0" "%~n1-%~n0.txt"
@popd
@goto END

:ABNORMAL_END
@pause

:END
