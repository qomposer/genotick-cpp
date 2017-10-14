REM Use this script as post build event in visual studio
REM call "copy_files.bat" $(OutDir) $(TargetName) "YOUR CUSTOM TARGET PATH"

setlocal

set currentFilename=%~n0%~x0
set currentPath=%~dp0
set outDir=%1
set targetName=%2
set targetPath=%3

echo Executing post build event: %currentFilename%

for %%f in (
	"%currentPath%..\include\igenotick.h"
	"%currentPath%..\include\igenotick_utils.h"
	"%outDir%%targetName%.dll"
	"%outDir%%targetName%.pdb"
	) do (
	echo Copy file %%f to %targetPath%
	copy %%f %targetPath% /Y
)

endlocal
