@echo off
rmdir /s /q .vs
rmdir /s /q Debug
rmdir /s /q Release
del *.iobj
del *.ipdb
del *.user
pause
