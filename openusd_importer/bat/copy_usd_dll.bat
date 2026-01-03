@echo off

setlocal

set BIN_DIR=%1\bin
set LIB_DIR=%1\lib
set DEST_DIR=%2
set CONFIG=%3

:: Copy DLLs
if /i "%CONFIG%"=="Debug" (
    call :copy_bin_dll tbb_debug.dll
) else (
    call :copy_bin_dll tbb.dll
)

call :copy_lib_dll usd_ar.dll
call :copy_lib_dll usd_arch.dll
call :copy_lib_dll usd_gf.dll
call :copy_lib_dll usd_js.dll
call :copy_lib_dll usd_kind.dll
call :copy_lib_dll usd_pcp.dll
call :copy_lib_dll usd_plug.dll
call :copy_lib_dll usd_python.dll
call :copy_lib_dll usd_sdf.dll
call :copy_lib_dll usd_tf.dll
call :copy_lib_dll usd_trace.dll
call :copy_lib_dll usd_ts.dll
call :copy_lib_dll usd_usd.dll
call :copy_lib_dll usd_vt.dll
call :copy_lib_dll usd_work.dll

:: Copy plugInfos
set PLUGINFO_SRC_DIR=%LIB_DIR%\usd
set PLUGINFO_DEST_DIR=%DEST_DIR%\usd

if not exist "%PLUGINFO_DEST_DIR%" (
    mkdir "%PLUGINFO_DEST_DIR%"
)

copy %PLUGINFO_SRC_DIR%\plugInfo.json %PLUGINFO_DEST_DIR%\plugInfo.json /y
call :copy_pluginfo_dir ar
call :copy_pluginfo_dir sdf

endlocal

exit /b

:copy_bin_dll
copy %BIN_DIR%\%1 %DEST_DIR%\%1 /y
exit /b

:copy_lib_dll
copy %LIB_DIR%\%1 %DEST_DIR%\%1 /y
exit /b

:copy_pluginfo_dir
xcopy %PLUGINFO_SRC_DIR%\%1 %PLUGINFO_DEST_DIR%\%1 /s /e /i /y
exit /b