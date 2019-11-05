# CopyFilesNoSameContent
## C++17

Copy all the files from a directory to another directory,
not copying the ones existing - files can have equal content with
different names.

The solution developed has these properties:
1. It is not recursive, to guarantee that user selects the directories they really desire.
2. Two files are equal if, and only if, they have the same content. This means:
  2.1. If two files have the same name but different content, then it will be copied with a different name (not erasing the equal named file).
  2.2. If two files have equal or different names, but same content, then it will not be copied.
