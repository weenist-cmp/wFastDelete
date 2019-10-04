# wFastDelete

Games or programs take too much time to delete because of too many files and capacity, so I did not deserve to find a faster program, I created it myself.  

-Consist of  
wFastDelete-User Manual.  
wFastDelete-Short Description.txt  
wFastDelete.exe  
wFastDelete.ini  
wFastDelete-InfoList (example) .txt  

-Installation, usage  
Copy the file to any folder and Run Administrator Mode it.  
Click [Register] button to use it from now on.  
If you don't want to cancel it after registering, double click on the blank to exit.  
If you want to change the settings, Run Administrator Mode it again and click the [Register] button.  

[Auto close] automatically closes the window when deleting a folder or when folder information (LM) is completed successfully. The time to close the window can be changed by opening the wFastDelete.ini file in Notepad.  
[No message] does not show the delete operation result when deleting a folder.  
[LM] does not require general users, but registers folder information LM) used to obtain a list of folders and files in a folder. At this point, select Directory / File / FileSize. The selected item is selected by default.  

Right-click on a folder in Explorer or other programs.  
Click [Delete Folder] to delete the folder. After the delete operation is finished, the result is output.  
Click [Folder Information] to display the folder information.  
Select [Folder Info (LM)] to create file list. If you select the list type and click [ListMake], specify the file location and name to save and confirm the list file of the folder. At this time, a confirmation window is displayed if the file name already exists.  

-How to delete, Uninstall  
Run Administrator Mode it and click the [Unlock] button and delete all files.  

-Change  
In previous versions, non-standard ANSI filenames could not be deleted, but they can be deleted if they are not used by other programs. It also supported multilingual languages. Open wFastDelete.ini in Notepad for instructions.  
The font used is designed to support multiple languages, so it is not very pretty as a default font. Similarly, if you don't have a pretty window, you're simply trying to make it small, but the reason why the message and the buttons are so wide is that you have a generous amount of folders.  
The right-click menu has been added to the submenu. At this point, the icon is displayed. For how to change the icons or other detailed settings, open the wFastDelete.ini file in Notepad, read the instructions carefully, and change them carefully.  
Added a bar to track progress.  
[ShowWhileWorking] Show progress when viewing folder information or creating list files (slow)  
[ShowWhileRemove] Show progress when deleting (slow)  
[CommaCount] Add comma in the middle of number (0-10 every few letters)  

-Copyright  
For non-commercial use by individuals, it is free to use without any restrictions.  
This program is copyright to weenist.  
Please contact weenist@gmail.com or 010-6494-0862  
If you are studying or interested in the program, please let us know.  
Language C, IDE CodeBlocks, Compiler gcc 8.2.0  

2016.02.17. Initial creation  
2019.09.24. Final modification  
weenist.egloos.com  
github.com/weenist-cmp  

Produced by weenist
