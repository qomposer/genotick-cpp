REM Use this script as post build event in visual studio
REM call "copy_files_zorro.bat" $(OutDir) $(TargetName) "Your\Zorro\Install\Path"

setlocal

set currentFilename=%~n0%~x0
set currentPath=%~dp0
set outDir=%1
set targetName=%2
set zorroIncludePath="%3\include\genotick"
set zorroStrategyPath="%3\Strategy"

echo Executing post build event: %currentFilename%

for %%f in (
	"%currentPath%..\include\igenotick.h"
	"%currentPath%..\include\igenotick_utils.h"
	"%currentPath%..\jar\genotick.jar"
	"%outDir%%targetName%.dll"
	"%outDir%%targetName%.pdb"
	) do (
	echo Copy file %%f to %zorroIncludePath%
	copy %%f %zorroIncludePath% /Y
)

for %%f in (
	"%currentPath%..\src\example\GenotickZorro.c"
	) do (
	echo Copy file %%f to %zorroStrategyPath%
	copy %%f %zorroStrategyPath% /Y
)

endlocal
