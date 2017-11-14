REM Use this script as post build event in visual studio
REM call "copy_files_zorro.bat" $(OutDir) $(TargetName) "Your\Zorro\Install\Path"

setlocal

set currentFilename=%~n0%~x0
set currentPath=%~dp0
set outDir=%1
set targetName=%2
set path=%3
set path=%path:"=%
set zorroIncludeCommonPath="%path%\include\common"
set zorroIncludeGenotickPath="%path%\include\genotick"
set zorroStrategyPath="%path%\Strategy"

echo Executing post build event: %currentFilename%
mkdir %zorroIncludeCommonPath% 2>nul
mkdir %zorroIncludeGenotickPath% 2>nul

echo Target: %zorroIncludeCommonPath%

for %%f in (
	"%currentPath%..\..\commonlib\include\common\types.h"
	) do (
	echo Source: %%f
	copy %%f %zorroIncludeCommonPath% /Y
)

echo Target: %zorroIncludeGenotickPath%

for %%f in (
	"%currentPath%..\include\igenotick.h"
	"%currentPath%..\include\igenotick_utils.h"
	"%currentPath%..\jar\genotick.jar"
	"%outDir%%targetName%.dll"
	"%outDir%%targetName%.pdb"
	) do (
	echo Source: %%f
	copy %%f %zorroIncludeGenotickPath% /Y
)

echo Target: %zorroStrategyPath%

for %%f in (
	"%currentPath%..\src\example\GenotickZorro.c"
	) do (
	echo Source: %%f
	copy %%f %zorroStrategyPath% /Y
)

endlocal
