// textfile.h: interface for reading and writing text files
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
///reads a text file. Returns zero length array if not successful.
//////////////////////////////////////////////////////////////////////

char *textFileRead(char *fn);
///writes in a text file. Returns 1 if successful, 0 if not.
int textFileWrite(char *fn, char *s);
