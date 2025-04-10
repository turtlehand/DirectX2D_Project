del /Q ".\External\Include\Practice\*"
del /Q ".\External\Include\Practice\Scripts\*"
xcopy /s /y /exclude:exclude_list.txt ".\Project\Practice\*.h" ".\External\Include\Practice\"